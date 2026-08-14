#include "HistoryApp.h"

#include "Gui/MainForm.h"

//#include <SVSLibrary/StringUtilities.h>
#include <SVSLibrary/Time/SVSdatetime.h>
#include <SVSLibrary/Logger/Log.h>

#ifdef _WIN32
#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#endif

#include <wx/msgdlg.h>
#include <wx/snglinst.h>

std::string HistoryApp::s_exeName = "";

HistoryApp::HistoryApp()// :
//   m_ioService(std::max((int32_t)std::thread::hardware_concurrency(), 2), SVS::StartupType::Automatic)
{
}

HistoryApp::~HistoryApp()
{
   //if (m_autonomousController != nullptr)
   //{
   //   delete m_autonomousController;
   //}

   //if (m_wavebladeDriver != nullptr)
   //{
   //   delete m_wavebladeDriver;
   //}
}

void HistoryApp::StoreExeName()
{
#if _WIN32
   char fileName[MAX_PATH] = { 0 };

   if (GetModuleFileNameA(nullptr, fileName, MAX_PATH))
   {
      s_exeName = PathFindFileNameA(fileName);
   }

#else
#	error Platform not yet implemented
#endif
}

bool HistoryApp::OnInit()
{
	SVS::Log().SetFlags(SVS::Logger::AddTime);

#ifdef _DEBUG
   SVS::Log().SetFileName(std::string("_DEBUG_HistoryAppLog") + ".txt");
#else
   SVS::Log().SetFileName("HistoryAppLog_" + FormatFilenameSafeTimeString(datetimeNow()) + ".txt");
#endif

	SVS::Log() << "HistoryApp.exe started" << std::endl;

   StoreExeName();

   if (!EnsureSingleInstance())
   {
      return (false);
   }

   m_settings.Load("_settings.txt");

   // Create the app
   m_mainWindow = new MainForm(nullptr);

   wxString windowTitle = "History App v" SVS_STRINGIFY_A(HISTORY_APP_VERSION);

   m_mainWindow->SetTitle(windowTitle);

   m_mainWindow->Layout();
   m_mainWindow->Show();
   m_mainWindow->Refresh();
   m_mainWindow->Update();

   //m_wavebladeDriver       = new WavebladeDriver(&m_settings);
   //m_autonomousController  = new AutonomousController(&m_settings, m_wavebladeDriver);

   return (true);
}

int HistoryApp::OnExit()
{
   return (0);
}

bool HistoryApp::EnsureSingleInstance()
{
   const wxString appName = wxString::Format("%s-%s", s_exeName.c_str(), wxGetUserId().c_str());
   m_instanceCheck = std::make_unique<wxSingleInstanceChecker>(appName);

   if (m_instanceCheck->IsAnotherRunning())
   {
//      wxMessageDialog(nullptr, _("The Waveblade App is already running on this machine. Only one instance is allowed to run at a time."), _("Waveblade App Already Running"), (wxICON_ERROR | wxCENTRE | wxOK)).ShowModal();
   	SVS::Log() << "The History App is already running on this machine. Only one instance is allowed to run at a time. Exiting." << std::endl;
      return (false);
   }

   return (true);
}

wxIMPLEMENT_APP(HistoryApp);