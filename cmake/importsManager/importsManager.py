from __future__ import print_function
from Tkinter import *
import tkMessageBox
import tkFileDialog
import tkSimpleDialog
import ttk
import os
import sys
import zipfile
import pysvn
import subprocess
import re

class SvnHelper(object):
    def __init__(self, logCallback=None, root='svn://192.168.0.250:7000/Imports_v2/trunk'):
        self.platforms = set()
        self.libs = {}
        self.root = root
        self.login = False

        self.cli = pysvn.Client()
        self.cli.callback_get_login = self.getLogin

        if logCallback: self.log = logCallback
        else:           self.log = lambda txt: print(txt)


    def getLogin(self, realn, username, may_save):
        self.log('Unknown SVN credentials')

        self.login = tkSimpleDialog.askstring('Missing Credentials', 'Enter your SVN login')
        self.pwd = tkSimpleDialog.askstring('Missing Credentials', 'Enter your SVN password')

        return True, self.login, self.pwd, False


    def readSvn(self):
        self.log('Reading SVN')
        files = self._listRecursive(self.root)

        # Cleanup
        self.platforms = [i for i in self.platforms]


    def _listRecursive(self, root, depth=0):
        self.log(root)
        sp = root[len(self.root)+1:].split('/')

        if depth == 3 and root.endswith('.zip'):
            plat = root.split('/')[-1][:-4]
            self.platforms.update([plat])
            if (sp[0], sp[1]) not in self.libs.keys():
                self.libs[(sp[0], sp[1])] = []
            self.libs[(sp[0], sp[1])].append(plat)

        # CMake is in imports for legacy reasons
        if root.endswith('cmake'): return []

        # Wrong format, too many directories - don't waste time
        if depth > 2: return

        for e in self.cli.list(root):
            if e[0].path == root: continue
            if depth < 2 and e[0].kind == pysvn.node_kind.file: continue
            if depth == 2 and not e[0].kind == pysvn.node_kind.file: continue
            self._listRecursive(e[0].path, depth+1)

            
    def _svnCmd(self, act, params):
        cmd = ['svn', act, '--non-interactive']
        if self.login:
            cmd.extend(['--no-auth-cache', '--username', self.login, '--password', self.pwd])
        cmd.extend(params)
        return cmd

    
    def checkoutFile(self, lib, ver, fname):
        importsPath = Application.importsDirVar.get()
        self.log('Checking out '+lib+' / '+ver+' / '+fname + ' into: '+importsPath)
        absPath = os.path.abspath(importsPath)

        if not os.path.exists(absPath):
            self.log('Destination directory does not exist, creating ('+absPath+')')
            os.mkdir(absPath)

        if not os.path.exists(os.path.join(absPath, self.root)):
            self.log(subprocess.check_output(self._svnCmd('co', ['--depth', 'empty', self.root, '.']), cwd=absPath))
        if not os.path.exists(os.path.join(absPath, self.root, lib)):
            self.log(subprocess.check_output(self._svnCmd('co', ['--depth', 'empty', self.root+'/'+lib]), cwd=absPath))
        if not os.path.exists(os.path.join(absPath, self.root, lib, ver)):
            self.log(subprocess.check_output(self._svnCmd('co', ['--depth', 'empty', self.root+'/'+lib+'/'+ver]),
                                        cwd=os.path.join(absPath,lib)))
        self.updateFile(lib,ver,fname)


    def updateFile(self, lib, ver, fname):
        absPath = os.path.abspath(Application.importsDirVar.get())
        self.log(subprocess.check_output(self._svnCmd('up', ['--non-interactive', fname]),
                                        cwd=os.path.join(absPath,lib,ver)))

    def checkSvnBinary(self):
        try:
            subprocess.check_output(['svn', 'help'])
        except:
            self.log('---- Error calling SVN, probably not installed on system! ----')


class WorkingCopy(object):
    path = '../../../Imports_v2'

    @staticmethod
    def findExisting():
        ret = []
        importsPath = Application.importsDirVar.get()
        for lib in os.listdir(importsPath):
            if os.path.isfile(os.path.join(importsPath, lib)): continue
            l_path = os.path.join(importsPath, lib)
            for ver in os.listdir(l_path):
                v_path = os.path.join(l_path, ver)
                if os.path.isfile(v_path): continue
                ret.append((lib, ver, [f for f in os.listdir(l_path) if os.path.isfile(f)]))

        return ret


    @staticmethod
    def updateExisting(svn):
        absPath = os.path.abspath(Application.importsDirVar.get())
        for lib in os.listdir(absPath):
            l_path = os.path.join(absPath, lib)
            if os.path.isfile(l_path): continue
            if l_path.endswith('cmake'): continue

            for ver in os.listdir(l_path):
                v_path = os.path.join(l_path, ver)
                if os.path.isfile(v_path): continue
                for f in os.listdir(v_path):
                    if os.path.isfile(os.path.join(v_path,f)):
                        if not f.endswith('.zip'): continue
                        svn.log('Updating:'+lib+' / '+ver+' / '+f)
                        svn.updateFile(lib, ver, f)

        svn.log('Done')


class CmakeParser(object):
    @staticmethod
    def _findImports(txt, filename, logCallback):
        ret = set()
        for r in re.findall('svs_import\((.+?)\)', txt):
            sp = r.split(' ')
            if sp[0] != 'SYSTEM':
                ret.add((sp[1], sp[2]))
                print (sp[1], sp[2])

        for r in re.findall('svs_add_subproject\((.+?)( .+?)?\)', txt):
            fn = os.path.join(os.path.dirname(filename), r[0],'CMakeLists.txt')
            fn = os.path.abspath(fn)
            ret.update(CmakeParser.getImportsList(fn, logCallback))

        for r in re.findall('add_subdirectory\((\$\{CMAKE_CURRENT_SOURCE_DIR\}/)?(.+?)( .+?)?\)', txt):
            fn = os.path.join(os.path.dirname(filename), r[1],'CMakeLists.txt')
            fn = os.path.abspath(fn)
            ret.update(CmakeParser.getImportsList(fn, logCallback))

        return ret


    @staticmethod
    def getImportsList(filename, logCallback):
        logCallback('getImportsList '+filename)

        ret = set()
        with file(filename) as f:
            lines = ''.join(f.read().split("\n"))
            ret.update(CmakeParser._findImports(lines, filename, logCallback))

        return ret



class Application(Frame):
    def __init__(self, parent):
        Frame.__init__(self, parent)
        self.parent = parent
        self.pack(fill=BOTH, expand=1)

        self.svn = SvnHelper(self.log)
        self.after(300, self.updateGUI)

        self.parent.title("Imports Manager")
        self.createBox()
        self.createButtons()
        self.addLibs()
        self.createLog()


    def updateGUI(self):
        self.svn.readSvn()

        for lib, ver in sorted(self.svn.libs.keys()):
            self.library.insert(END, lib+' / '+ver +
                ' / [' + ', '.join(self.svn.libs[(lib, ver)]) +']')

        self.compiler['values'] = sorted(self.svn.platforms)
        self.compiler.current(0)


    def createBox(self):
        self.platform = StringVar()
        self.compiler = ttk.Combobox(self, state="readonly", textvariable=self.platform)
        self.compiler.grid(row=0, columnspan=2)
        self.compiler.pack(fill=X)


    def createButtons(self):
        btn = Button(self, command=self.checkoutSelected)
        btn['text'] = 'Checkout Selected Libraries'
        btn.pack(fill=X)

        labelTo = Label(self, text='Checkout to:')
        labelTo.pack(fill=X)
        
        Application.importsDirVar = StringVar()
        absPath = os.path.abspath(WorkingCopy.path)
        Application.importsDirVar.set(absPath)
        dirEntry = Entry(self, textvariable=Application.importsDirVar)
        dirEntry.pack(fill=X)
        
        btnAll = Button(self, command=self.updateAll)
        btnAll['text'] = 'Update All Local Libraries'
        btnAll.pack(fill=X)

        btnCmake = Button(self, command=self.parseCmake)
        btnCmake['text'] = 'Parse CMake...'
        btnCmake.pack(fill=X)


    def addLibs(self):
        self.libframe = Frame(self)
        self.libframe.pack(fill=BOTH, expand=1)

        self.liblabel = Label(self.libframe, text="Select Libraries:", anchor="nw", justify="left")
        self.liblabel.pack(fill=X)

        sc = Scrollbar(self.libframe)
        sc.pack(side=RIGHT, fill=Y)

        self.library = Listbox(self.libframe, yscrollcommand=sc.set, selectmode='multiple')
        self.library.configure(exportselection=False)
        self.library.pack(fill=BOTH, expand=1)

        sc.config(command=self.library.yview)


    def createLog(self):
        self.logframe = Frame(self)
        self.logframe.pack(fill=BOTH, expand=1)

        self.loglabel = Label(self.logframe, text="Log:", anchor="nw", justify="left")
        self.loglabel.pack(fill=X)

        textScroll = Scrollbar(self.logframe);
        textScroll.pack(side=RIGHT, fill=Y)

        self.logtext = Text(self.logframe, yscrollcommand=textScroll.set)
        self.logtext.pack(fill=BOTH, expand=1)

        textScroll.config(command=self.logtext.yview)


    def updateAll(self):
        WorkingCopy.updateExisting(self.svn)


    def checkoutSelected(self):
        try:
            for sel in [self.library.get(i) for i in self.library.curselection()]:
                sp = sel.split(' / ')
                self.svn.checkoutFile(sp[0], sp[1], self.platform.get()+'.zip')

            self.log('Done')
        except Exception as e:
            self.log('Error:'+str(e))


    def parseCmake(self):
        self.library.selection_clear(0,self.library.size())

        f = tkFileDialog.askopenfilename()
        if not f: return
        try:
            ilist = CmakeParser.getImportsList(f, self.log)
            self.log(ilist)
            for lib in ilist:
                for idx in range(self.library.size()):
                    name = self.library.get(idx).lower()
                    if name.startswith(lib[0].lower()+' / '+lib[1].lower()):
                        self.library.selection_set(idx)
        except Exception as e:
            self.log('Error occured: '+str(e))


    def log(self, txt):
        self.logtext.insert(END, str(txt)+'\n')
        self.logtext.see(END)
        self.update()
        print(txt)
        sys.stdout.flush()



if __name__ == '__main__':
    root = Tk()
    root.geometry('400x400')
    app = Application(root)
    root.mainloop()

