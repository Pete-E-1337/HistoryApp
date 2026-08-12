#ifndef WAVEBLADEAPP_H
#define WAVEBLADEAPP_H

//#pragma once

#include <boost/asio.hpp>
//#include <SVSLibrary/Std.h>
//#include <SVSLibrary/Execution/BoostIoService.h>
//#include <SVSLibrary/Execution/Mutex.h>

#ifdef _WIN32
#include <wx/msw/wrapwin.h>
#endif

#include <memory>
#include <wx/app.h>
#include "WavebladeDriver.h"
#include "AutonomousController.h"
#include "Settings.h"

#define WAVEBLADE_APP_VERSION 1.0.0.0

class MainForm;
class wxSingleInstanceChecker;

class WavebladeApp : public wxApp
{
public:
   WavebladeApp();
   ~WavebladeApp();

   bool OnInit() override;
   int OnExit() override;

private:
   void StoreExeName();
   bool EnsureSingleInstance();

private:
   template <typename T> using Ptr = std::unique_ptr<T>;
   
   MainForm*                     m_mainWindow;
   Ptr<wxSingleInstanceChecker>  m_instanceCheck;
//   SVS::BoostIoService				m_ioService;

   static std::string s_exeName;

   Settings                m_settings;
   WavebladeDriver*        m_wavebladeDriver       = nullptr;
   AutonomousController*   m_autonomousController  = nullptr;
};

DECLARE_APP(WavebladeApp)

#endif // WAVEBLADEAPP_H