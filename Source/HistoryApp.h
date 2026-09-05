#ifndef HISTORYAPP_H
#define HISTORYAPP_H

//#pragma once

#include <boost/asio.hpp>
//#include <SVSLibrary/Std.h>
//#include <SVSLibrary/Execution/BoostIoService.h>
//#include <SVSLibrary/Execution/Mutex.h>

#ifdef _WIN32
#include <wx/msw/wrapwin.h>
#endif

#include <memory>
#include <string>
#include <wx/app.h>
//#include "WavebladeDriver.h"
//#include "AutonomousController.h"
#include "AppData.h"
#include "Settings.h"

#define HISTORY_APP_VERSION 1.0.0.0

class MainForm;
class wxSingleInstanceChecker;

class HistoryApp : public wxApp
{
public:
   HistoryApp();
   ~HistoryApp();

   bool OnInit() override;
   int OnExit() override;

private:
   void StoreExeName();
   bool EnsureSingleInstance();
	void AddEventTimelineEventData(double startDate, double endDate, const std::string& name, const std::string& imageFilename);
	//void FindNewestDate();

private:
   template <typename T> using Ptr = std::unique_ptr<T>;
   
   MainForm*                     m_mainWindow;
   Ptr<wxSingleInstanceChecker>  m_instanceCheck;
//   SVS::BoostIoService				m_ioService;

   static std::string s_exeName;

	AppData						m_appData;
   Settings                m_settings;
//   WavebladeDriver*        m_wavebladeDriver       = nullptr;
//   AutonomousController*   m_autonomousController  = nullptr;
};

DECLARE_APP(HistoryApp)

#endif // HISTORYAPP_H