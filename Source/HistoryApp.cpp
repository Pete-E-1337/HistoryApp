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

	// Add some example event data
	{
		//TimelineEventData eventData;
		//
		//eventData.startDate		= 1939.0;
		//eventData.endDate			= 1945.0;
		//eventData.name				= "World War II";
		//m_appData.eventList.push_back(eventData);

		//eventData.startDate		= 0.0;
		//eventData.endDate			= 30.0;
		//eventData.name				= "Jesus of Nazareth ";
		//m_appData.eventList.push_back(eventData);

		AddEventTimelineEventData(0, 30.0, "Jesus of Nazareth");				// 1
		AddEventTimelineEventData(1939.0, 1945.0, "World War II");			// 2
		AddEventTimelineEventData(-4540000000.0, 2026.0, "Earth");			// 3
		AddEventTimelineEventData(-2500000.0, -3300.0, "Stone Age");		// 4
		AddEventTimelineEventData(-3300.0, -1200.0, "Bronze Age");			// 5
		AddEventTimelineEventData(-1200.0, -500.0, "Iron Age");				// 6
		AddEventTimelineEventData(-500.0, 500.0, "Classical Era");			// 7
		AddEventTimelineEventData(500.0, 1500.0, "Middle Ages");				// 8
		AddEventTimelineEventData(1500.0, 1800.0, "Early Modern Era");		// 9
		AddEventTimelineEventData(1800.0, 2026.0, "Modern Era");				// 10
		AddEventTimelineEventData(-550.0, -330.0, "Achaemenid Empire");	// 11
		AddEventTimelineEventData(-27.0, 476.0, "Roman Empire");				// 12
		AddEventTimelineEventData(1206.0, 1368.0, "Mongol Empire");			// 13
		AddEventTimelineEventData(1299.0, 1922.0, "Ottoman Empire");		// 14
		AddEventTimelineEventData(1583.0, 1997.0, "British Empire");		// 15
		AddEventTimelineEventData(-3000.0, -30.0, "Ancient Egypt");			// 16

		std::sort(m_appData.eventList.begin(), m_appData.eventList.end(),
					 [](const TimelineEventData& a, const TimelineEventData& b) { return a.startDate < b.startDate; });

		FindNewestDate();
	}

   // Create the app
   m_mainWindow = new MainForm(nullptr, &m_appData);

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

void HistoryApp::AddEventTimelineEventData(double startDate, double endDate, const std::string& name)
{
	TimelineEventData eventData;
		
	eventData.startDate		= startDate;
	eventData.endDate			= endDate;
	eventData.name				= name;

	m_appData.eventList.push_back(eventData);
}

void HistoryApp::FindNewestDate()
{
	m_appData.newestDate = std::numeric_limits<int32_t>::min();

	for (TimeLineEventListConstIter iter = m_appData.eventList.begin(); iter != m_appData.eventList.end(); iter++)
	{
		if (iter->endDate > m_appData.newestDate)
		{
			m_appData.newestDate = iter->endDate;
		}
	}
}

wxIMPLEMENT_APP(HistoryApp);