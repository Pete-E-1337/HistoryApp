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

		AddEventTimelineEventData(0,					30.0,		"Jesus of Nazareth",	"");
		AddEventTimelineEventData(1939.0,			1945.0,	"World War II",		"");
		//AddEventTimelineEventData(-4540000000.0,	2026.0,	"Earth",					"");
		//AddEventTimelineEventData(-2500000.0,		-3300.0,	"Stone Age",			"");
		AddEventTimelineEventData(-3300.0,			-1200.0,	"Bronze Age",			"");
		AddEventTimelineEventData(-1200.0,			-500.0,	"Iron Age",				"");
		AddEventTimelineEventData(-500.0,			500.0,	"Classical Era",		"");
		AddEventTimelineEventData(500.0,				1500.0,	"Middle Ages",			"");
		AddEventTimelineEventData(-27.0,				406.0,	"",						"Data/Images/362AD.jpg");
		AddEventTimelineEventData(406.0,				420.0,	"",						"Data/Images/406AD.jpg");
		AddEventTimelineEventData(420.0,				451.0,	"",						"Data/Images/420AD.jpg");
		AddEventTimelineEventData(451.0,				476.0,	"",						"Data/Images/451AD.jpg");
		AddEventTimelineEventData(476.0,				528.0,	"",						"Data/Images/476AD.jpg");
		AddEventTimelineEventData(528.0,				565.0,	"",						"Data/Images/528AD.jpg");
		AddEventTimelineEventData(565.0,				600.0,	"",						"Data/Images/565AD.jpg");
		AddEventTimelineEventData(600.0,				626.0,	"",						"Data/Images/600AD.jpg");
		AddEventTimelineEventData(626.0,				651.0,	"",						"Data/Images/626AD.jpg");
		AddEventTimelineEventData(651.0,				737.0,	"",						"Data/Images/651AD.jpg");
		AddEventTimelineEventData(737.0,				771.0,	"",						"Data/Images/737AD.jpg");
		AddEventTimelineEventData(771.0,				830.0,	"",						"Data/Images/771AD.jpg");
		AddEventTimelineEventData(830.0,				888.0,	"",						"Data/Images/830AD.jpg");
		AddEventTimelineEventData(888.0,				925.0,	"",						"Data/Images/888AD.jpg");
		AddEventTimelineEventData(925.0,				1000.0,	"",						"Data/Images/925AD.jpg");
		AddEventTimelineEventData(1000.0,			1030.0,	"",						"Data/Images/1000AD.jpg");
		AddEventTimelineEventData(1030.0,			1071.0,	"",						"Data/Images/1030AD.jpg");
		AddEventTimelineEventData(1071.0,			1092.0,	"",						"Data/Images/1071AD.jpg");
		AddEventTimelineEventData(1092.0,			1100.0,	"",						"Data/Images/1092AD.jpg");
		AddEventTimelineEventData(1100.0,			1130.0,	"",						"Data/Images/1100AD.jpg");
		AddEventTimelineEventData(1130.0,			1173.0,	"",						"Data/Images/1130AD.jpg");
		AddEventTimelineEventData(1173.0,			1212.0,	"",						"Data/Images/1173AD.jpg");
		AddEventTimelineEventData(1212.0,			1230.0,	"",						"Data/Images/1212AD.jpg");
		AddEventTimelineEventData(1230.0,			1278.0,	"",						"Data/Images/1230AD.jpg");
		AddEventTimelineEventData(1278.0,			1346.0,	"",						"Data/Images/1278AD.jpg");
		AddEventTimelineEventData(1346.0,			1361.0,	"",						"Data/Images/1346AD.jpg");
		AddEventTimelineEventData(1361.0,			1401.0,	"",						"Data/Images/1361AD.jpg");
		AddEventTimelineEventData(1401.0,			1430.0,	"",						"Data/Images/1401AD.jpg");
		AddEventTimelineEventData(1430.0,			1483.0,	"",						"Data/Images/1430AD.jpg");
		AddEventTimelineEventData(1483.0,			1500.0,	"",						"Data/Images/1483AD.jpg");
		AddEventTimelineEventData(1500.0,			1800.0,	"Early Modern Era",	"");
		AddEventTimelineEventData(1800.0,			2026.0,	"Modern Era",			"");
		AddEventTimelineEventData(-550.0,			-330.0,	"Achaemenid Empire",	"");
		AddEventTimelineEventData(-27.0,				476.0,	"Roman Empire",		"");
		AddEventTimelineEventData(1206.0,			1368.0,	"Mongol Empire",		"");
		AddEventTimelineEventData(1299.0,			1922.0,	"Ottoman Empire",		"");
		AddEventTimelineEventData(1583.0,			1997.0,	"British Empire",		"");
		AddEventTimelineEventData(-3000.0,			-30.0,	"Ancient Egypt",		"");

		std::sort(m_appData.eventList.begin(), m_appData.eventList.end(),
					 [](const TimelineEventData& a, const TimelineEventData& b) { return a.startDate < b.startDate; });

		std::sort(m_appData.imageList.begin(), m_appData.imageList.end(),
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

void HistoryApp::AddEventTimelineEventData(double startDate, double endDate, const std::string& name, const std::string& imageFilename)
{
	TimelineEventData eventData;
		
	eventData.startDate		= startDate;
	eventData.endDate			= endDate;
	eventData.name				= name;
	eventData.imageFilename	= imageFilename;

	if (name.empty() == false)
	{
		m_appData.eventList.push_back(eventData);
	}

	if (imageFilename.empty() == false)
	{
		m_appData.imageList.push_back(eventData);
	}
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