#include "MainForm.h"

#include <wx/msgdlg.h>
#include <wx/spinctrl.h>
#include <wx/display.h>
#include <windows.h>
#include <fstream>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <SVSLibrary/Std.h>
#include <SVSLibrary/StringUtilities.h>
#include <SVSLibrary/Compiler/Warnings.h>
#include <SVSLibrary/Math/MiscMath.h>
//#include "../PID.h"

SVS_WARNING_DISABLE(4189) // local variable is initialized but not referenced
//SVS_WARNING_DISABLE(4702) // unreachable code

//#define ENABLE_DEBUGGING_GUI

#ifdef _DEBUG
//#define USE_DEBUG_DATA
//#define DEBUG_PID
#define DRAW_DEBUG_TEXT
#endif

//#define BLOCK_AMBIENT	// Use these so that the graph doesnt get screwed up while they arent working. Probably no longer required since being replaced by cpProxy
//#define BLOCK_DT		// Use these so that the graph doesnt get screwed up while they arent working. Probably no longer required since being replaced by deltaT

SVS_WARNING_DISABLE(4100) // Unreferenced formal parameter in boost

const char*				l_settingsFilename				= "settings.txt";
//const char*				l_historyFilename					= "Data/SampleHistory.csv";
//const char*				l_historyFilename					= "Data/1000_significant_history_events.csv";
//const char*				l_historyFilename					= "Data/75_significant_history_events.csv";
const char*				l_historyFilename					= "Data/x_significant_history_events.csv";
static const double	l_startingDate						= -2560.0;	// pyramid of giza
static const int		l_guiTimerInterval				= 300;
static const int		l_renderTimerInterval			= 40;
static const int		l_imageUpdateThreadInterval	= 300;
static const double	l_timelineSplitterProportion	= 0.6;
static const double	l_oneYear							= 1.0;
static const double	l_oneMonth							= 1.0 / 12.0;
static const double	l_oneFortnight						= 14.0 / 365.0;
static const double	l_oneWeek							= 7.0 / 365.0;
static const double	l_oneDay								= 1.0 / 365.0;
static const double	l_minimumEventTime				= l_oneMonth;

MainForm::MainForm(wxWindow* parent, AppData* appData) :
	History::MainForm(parent),
   m_ioService(std::max((int32_t)std::thread::hardware_concurrency(), 2), SVS::StartupType::Automatic),
	m_appData(appData)
{
//	SetIcon(wxICON(ISENTRYDISKUSAGECONFIGAPP_LOGO));
   Initialise();

	//double ratio = 0.0;
	//double linear_value, log_value, exp_value;
	//for (int i=0; i<=100; i++)
	//{
	//	linear_value = SVS::Math::LinearInterpolate(10.0, 100.0, ratio);
	//	log_value = SVS::Math::LogarithmicInterpolate(10.0, 100.0, ratio);
	//	exp_value = SVS::Math::ExponentialInterpolate(10.0, 100.0, ratio);
	//	ratio += 0.01;
	//}
}

MainForm::~MainForm()
{
//	m_spinControlDoubleTopProportional->Disconnect(wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinDoubleEventHandler( MainForm::OnTopProportionalDoubleSpinCtrl ), NULL, this);
//	m_spinControlDoubleTopIntegral->Disconnect(wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinDoubleEventHandler( MainForm::OnTopIntegralDoubleSpinCtrl ), NULL, this);
//	m_spinControlDoubleTopDerivative->Disconnect(wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinDoubleEventHandler( MainForm::OnTopDerivativeDoubleSpinCtrl ), NULL, this);
//	m_spinControlDoubleBottomProportional->Disconnect(wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinDoubleEventHandler( MainForm::OnBottomProportionalDoubleSpinCtrl ), NULL, this);
//	m_spinControlDoubleBottomIntegral->Disconnect(wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinDoubleEventHandler( MainForm::OnBottomIntegralDoubleSpinCtrl ), NULL, this);
//	m_spinControlDoubleBottomDerivative->Disconnect(wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinDoubleEventHandler( MainForm::OnBottomDerivativeDoubleSpinCtrl ), NULL, this);
//
	if (m_imageUpdateThread != nullptr)
	{
		m_runImageUpdateThread = false;
		m_imageUpdateThread->join();
		delete m_imageUpdateThread;
		m_imageUpdateThread = nullptr;
	}

	m_guiTimer.Stop();
	m_renderTickTimer.Stop();
//
//	SaveSettings();
//
//   m_serialSettingsDialog->Destroy();
//   m_serialSettingsDialog = nullptr;
//
//   m_aboutDialog->Destroy();
//   m_aboutDialog = nullptr;
//
//	if (m_serialComms != nullptr)
//	{
//		delete m_serialComms;
//		m_serialComms = nullptr;
//	}
//
//	if (m_report != nullptr)
//	{
//		fclose(m_report);
//		m_report = nullptr;
//	}

   m_imageDialog->Destroy();
   m_imageDialog = nullptr;

	if (m_timelineCanvas != nullptr)
	{
//		delete m_timelineCanvas;	// newed components are deleted by the wxWidgets system
		m_timelineCanvas = nullptr;
	}

}

void MainForm::Initialise()
{
	LoadSettings();

	LoadHistoryFile(l_historyFilename);

	 // Call this before loading or processing any images
    wxInitAllImageHandlers();

	//// load a bitmap
	//{
	//	wxBitmap bitmap(wxT("Data/Images/362AD.jpg"), wxBITMAP_TYPE_JPEG);
	//	wxImage img = bitmap.ConvertToImage();
	//	int w, h;
	//	m_bitmap->GetSize(&w, &h);
	//	wxImage shrunkImg = img.Scale(w, h, wxIMAGE_QUALITY_HIGH);
	//	m_bitmap->SetBitmap(shrunkImg);
	// }

	m_timelineCanvas = new TimelineGLCanvas(m_timelinePanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
	wxSizer* sizer = m_timelinePanel->GetSizer();
	sizer->Add(m_timelineCanvas, 1, wxEXPAND);
//	m_timelineCanvas->SetFocus();

	m_imageDialog		= new ImageDialog(this);

	m_dateTextCtrl->SetValidator(wxTextValidator(wxFILTER_NUMERIC));

//	m_dateLeftStaticText->SetLabel(wxString::FromUTF8(u8"◄")); // \u25C4
//	m_dateRightStaticText->SetLabel(wxString::FromUTF8(u8"►")); // \u25BA
	m_dateLeftButton->SetLabel(wxString::FromUTF8(u8"◄")); // \u25C4
	m_dateRightButton->SetLabel(wxString::FromUTF8(u8"►")); // \u25BA

	m_timelineDateScrollBar->SetScrollbar(5000, 1, 10000, 10, true);
	//m_timelineZoomScrollBar->SetScrollbar(1, 1, 10000, 10, true); // no longer visible

	//m_timelineZoomSlider->Set
//	std::string str = std::to_string(std::lround(m_timelineCanvas->GetZoom() * 6.0)) + " years";
	//int w, h;
	//double aspect_ratio;
	//m_timelineCanvas->GetSize(&w, &h);
	//aspect_ratio = (double)w / (double)h;
	//std::string str = std::to_string(std::lround(m_timelineCanvas->GetZoom() * aspect_ratio * 2.46)) + " years";
	//m_zoomTextCtrl->SetValue(str);
	m_appData->updateDateRangeText = true;

	m_timelineCanvas->SetDate(l_startingDate);
	SetTimelineDateScrollBarPositionFromDate(l_startingDate);

	// Set the application to be (windowed) full screen and centred
	{
		wxDisplay currentDisplay(wxDisplay::GetFromWindow(this));
		wxRect clientRect = currentDisplay.GetClientArea();
		int availableWidth  = clientRect.GetWidth();
		int availableHeight = clientRect.GetHeight();

		this->SetSize(availableWidth, availableHeight);
		this->Centre();

		m_imageDialog->SetSize(availableWidth, availableHeight);
		m_imageDialog->Centre();
	}

	SetAppData(m_appData);

	Bind(wxEVT_CHAR_HOOK, &MainForm::OnGlobalCharHook, this);

	m_guiTimer.Start(l_guiTimerInterval);
	m_renderTickTimer.Start(l_renderTimerInterval);

	m_image_requires_update = true;

	m_runImageUpdateThread = true;
	m_imageUpdateThread = new boost::thread(boost::bind(&MainForm::ImageUpdateThread, this));
}

void MainForm::LoadSettings()
{
	if (boost::filesystem::exists(l_settingsFilename))
	{
	}
}

void MainForm::SaveSettings()
{
}

void MainForm::SetAppData(AppData* appData)
{
	if (m_timelineCanvas != nullptr)
	{
		m_timelineCanvas->SetAppData(appData);
	}
}

void MainForm::OnExitButtonClick(wxCommandEvent& event)
{
	m_appData->rendering = false;

   Close();

	event.Skip();
}

void MainForm::OnClose(wxCloseEvent& event)
{
	m_appData->rendering = false;

	event.Skip();
}

void MainForm::OnIdle(wxIdleEvent& event)
{
	if (m_firstTimeShown == true)
	{
		// Set the splitter position. NB. Couldn't be done during initialization :/
		int pos = l_timelineSplitterProportion * (double)this->GetSize().GetHeight();
		m_mainSplitter->SetSashPosition(pos, true);

		m_firstTimeShown = false;
	}

	event.Skip();
}

void MainForm::ImageUpdateThread(void)
{
	while (m_runImageUpdateThread == true)
	{
		UpdateImage();

		boost::this_thread::sleep(boost::posix_time::milliseconds(l_imageUpdateThreadInterval));
	}
}

void MainForm::OnGuiTimer(wxTimerEvent& event)
{
	if (m_timelineCanvas != nullptr)
	{
#ifdef DRAW_DEBUG_TEXT
		std::string str = m_timelineCanvas->GetDebugString();
		str += std::string(" - Render Delta: ") + std::to_string(m_renderDeltaTimeMSecs.GetValue());
		m_debugTextCtrl->SetValue(str);
#endif

		UpdateDateText();
		UpdateDateRangeText();
		UpdateDateScrollbar();
		UpdateSelection();
	}

	event.Skip();
}

void MainForm::OnRenderTickTimer(wxTimerEvent& event)
{
	{
//		m_renderDeltaTimeMSecs = event.GetInterval();
		wxLongLong currentTimeMsecs = wxGetLocalTimeMillis();
		m_renderDeltaTimeMSecs = currentTimeMsecs - m_lastRenderTimeMsecs;
        
		// milliseconds since last trigger
		m_lastRenderTimeMsecs = currentTimeMsecs;		
	}

	if (m_timelineCanvas != nullptr)
	{
		m_timelineCanvas->Refresh();	// Mark as requiring a redraw
//		m_timelineCanvas->Update();
	}

	event.Skip();
}

void MainForm::OnDateTextCtrlOnText(wxCommandEvent& event)
{
	//double value;

	//m_dateTextCtrl->GetValue().ToDouble(&value);
	//m_timelineCanvas->SetDate(value);

	event.Skip();
}

void MainForm::OnDateTextCtrlTextEnter(wxCommandEvent& event)
{
	std::string str = event.GetString();
	int date = std::stoi(str);

	m_timelineCanvas->SetDate(date);
//	m_timelineCanvas->SetFocus();
	m_updating_date_text		= true;
	m_image_requires_update	= true;

	SetTimelineDateScrollBarPositionFromDate((double)date);

	event.Skip();
}

void MainForm::OnDateTextCtrlLeftDown(wxMouseEvent& event)
{
	m_updating_date_text = false;

	event.Skip();
}

void MainForm::UpdateDateText()
{
//	m_dateTextCtrl->SetValue(std::to_string((int)m_timelineCanvas->GetDate()));

	if (m_updating_date_text == true)
	{
		double date = m_timelineCanvas->GetDate();
		std::string str = TimelineGLCanvas::DateToString(date);

		m_dateTextCtrl->SetValue(str);
	}
}

void MainForm::UpdateDateRangeText()
{
	if (m_appData->updateDateRangeText == true)
	{
		SetDateRangeText();
		SetZoomSliderPosition();
		m_appData->updateDateRangeText = false;
	}
}

void MainForm::SetDateRangeText()
{
	double range = m_timelineCanvas->GetVisibleDateRange();
	std::string str = std::to_string(std::lround(range)) + " years";
	m_zoomTextCtrl->SetValue(str);
}

void MainForm::UpdateDateScrollbar()
{
	if (m_appData->updateDateScrollbar == true)
	{
		SetTimelineDateScrollBarPositionFromDate(m_timelineCanvas->GetDate());
		m_image_requires_update = true;
		m_appData->updateDateScrollbar = false;
	}
}

void MainForm::UpdateSelection()
{
	if (m_appData->updateSelection == true)
	{
		m_appData->updateSelection = false;

		int index = m_timelineCanvas->GetSelectedIndex();

		m_googleSearchButton->Enable(index != -1);
		m_wikipediaSearchButton->Enable(index != -1);
		m_mapSearchButton->Enable((index != -1) &&
										  (m_appData->eventList[index].latitude != l_latLongUninitialized) &&
										  (m_appData->eventList[index].longitude != l_latLongUninitialized));
	}
}

void MainForm::UpdateImage()
{
	if (m_image_requires_update == true)
	{
		double date = m_timelineCanvas->GetDate();

		bool image_found = false;

		for (TimeLineEventListConstIter iter = m_appData->imageList.begin(); iter != m_appData->imageList.end(); iter++)
		{
			if (SVS::Math::InRange(date, iter->startDate, iter->endDate) == true)
			{
				if (iter->id == m_oldImageId)
				{
					image_found = true;
					break;	// no need to reload the same image
				}

				if (iter->imageFilename.empty() == false)
				{
					int w, h;

					wxBitmap bitmap(iter->imageFilename, wxBITMAP_TYPE_JPEG);

					if (bitmap.IsOk() == false)
					{
//						std::string msg = "Could not load file:  \"" + iter->imageFilename + "\"";
//						wxMessageDialog(this, msg.c_str(), "Error", wxOK | wxICON_ERROR);
//						wxMessageBox(msg.c_str(), "Error", wxOK | wxSTAY_ON_TOP);
						image_found = false;
						break;
					}

					wxImage img = bitmap.ConvertToImage();
//					m_bitmap->GetSize(&w, &h);
					m_bitmapPanel->GetSize(&w, &h);

					if ((w == 0) || (h == 0))
					{
//						return;
						image_found = false;
						break;
					}

					// maintain aspect ratio
					{
						double imgRatio = (double)img.GetWidth() / img.GetHeight();
						double targetRatio = (double)w / h;

						if (targetRatio > imgRatio)
						{
							w = std::lround(h * imgRatio);
						}
						else
						{
							h = std::lround(w / imgRatio);
						}
					}

					wxImage shrunkImg = img.Scale(w, h, wxIMAGE_QUALITY_HIGH);
					m_bitmap->SetBitmap(shrunkImg);
					m_bitmapPanel->Layout();

					if (m_imageDialog != nullptr)
					{
						m_imageDialog->SetImageFilename(iter->imageFilename);
					}

					image_found = true;
					m_oldImageId = iter->id;
					break;
				}
			}
		}

		if (image_found == false)
		{
			// Clear the bitmap
			m_bitmap->SetBitmap(wxNullBitmap);
			m_bitmapPanel->Layout();

			if (m_imageDialog != nullptr)
			{
				m_imageDialog->SetImageFilename("");
			}

			m_oldImageId = -1;
		}

		m_image_requires_update = false;
	}
}

void MainForm::OnMainSplitterSplitterSashPosChanged(wxSplitterEvent& event)
{
	m_image_requires_update	= true;
	m_oldImageId				= -1;	// ensure a resize occurs

	// 1845 / 250 => 18 years => x==2.45
	// 1845 / 570 => 8 years => x==2.47
//	std::string str = std::to_string(std::lround(m_timelineCanvas->GetZoom() * 6.0)) + " years";
	//int w, h;
	//double aspect_ratio;
	//m_timelineCanvas->GetSize(&w, &h);
	//aspect_ratio = ((double)w / (double)h) / 7.38;
//	m_zoomTextCtrl->Refresh();
	m_appData->updateDateRangeText = true;

	event.Skip();
}

void MainForm::OnTimelineDateScrollBarScroll(wxScrollEvent& event)
{
	if (m_appData->eventList.empty() == false)
	{
		int pos = event.GetPosition();	// range is 0 to (range - 1) :/
		double percentage = pos / (double)(m_timelineDateScrollBar->GetRange() - 1);
		double date = (percentage * (m_appData->latestDate - m_appData->eventList.begin()->startDate)) + m_appData->eventList.begin()->startDate;

		m_timelineCanvas->SetDate(date);

		UpdateDateText();
		m_image_requires_update = true;
	}

	event.Skip();
}

void MainForm::SetTimelineDateScrollBarPositionFromDate(double date)
{
	if (m_appData->eventList.size() == 0)
		return;

	double percentage = (date - m_appData->eventList.begin()->startDate) / (m_appData->latestDate - m_appData->eventList.begin()->startDate);
	double pos = SVS::Math::LinearInterpolate(0, m_timelineDateScrollBar->GetRange(), percentage);
	m_timelineDateScrollBar->SetThumbPosition((int)pos);
}

//void MainForm::OnTimelineZoomScrollBarScroll(wxScrollEvent& event)
//{
//	int pos = event.GetPosition();	// range is 0 to (range - 1) :/
//	double percentage = pos / (double)(m_timelineZoomScrollBar->GetRange() + 1);
//
//	m_timelineCanvas->SetZoom(percentage);
//
////	std::string str = std::to_string(std::lround(m_timelineCanvas->GetZoom() * 6.0)) + " years";
//	//int w, h;
//	//double aspect_ratio;
//	//m_timelineCanvas->GetSize(&w, &h);
//	//aspect_ratio = (double)w / (double)h;
//	//std::string str = std::to_string(std::lround(m_timelineCanvas->GetZoom() * aspect_ratio * 2.46)) + " years";
//	//m_zoomTextCtrl->SetValue(str);
//	m_appData->updateDateRangeText = true;
//
//	event.Skip();
//}
//
void MainForm::SetZoomSliderPosition()
{
	double percentage = m_timelineCanvas->GetZoomPercentage();
	int pos = std::lround(SVS::MiscMath::LinearInterpolate(m_timelineZoomSlider->GetMin(), m_timelineZoomSlider->GetMax(), percentage));

	m_timelineZoomSlider->SetValue(pos);
}

void MainForm::OnTimelineZoomSliderScroll(wxScrollEvent& event)
{
	int pos = event.GetPosition();	// range is 0 to (range) :/
	double percentage = pos / (double)(m_timelineZoomSlider->GetMax());

	m_timelineCanvas->SetZoom(percentage);

//	std::string str = std::to_string(std::lround(m_timelineCanvas->GetZoom() * 6.0)) + " years";

	//int w, h;
	//double aspect_ratio;
	//m_timelineCanvas->GetSize(&w, &h);
	//aspect_ratio = (double)w / (double)h;
	//std::string str = std::to_string(std::lround(m_timelineCanvas->GetZoom() * aspect_ratio * 2.46)) + " years";
	//m_zoomTextCtrl->SetValue(str);

//	m_appData->updateDateRangeText = true;
	// Do not use the usual method of setting the m_zoomTextCtrl's text by setting m_appData->updateDateRangeText to true as this causes
	// a loop when it sets the sliders value. Instead, update the text directly
	SetDateRangeText();

	event.Skip();
}

void MainForm::OnGlobalCharHook(wxKeyEvent& event)
{
	// https://theasciicode.com.ar/

	float step = (event.ShiftDown() == true) ? 10.0f : 1.0f;

	int keyCode = event.GetKeyCode();

	switch (keyCode)
	{
		case WXK_F3:// F3
			FindText();
			break;
		case 'A':
			m_timelineCanvas->SetDate(m_timelineCanvas->GetDate() - step);
			m_appData->updateDateScrollbar = true;
			break;
		case 'D':
			m_timelineCanvas->SetDate(m_timelineCanvas->GetDate() + step);
			m_appData->updateDateScrollbar = true;
			break;
		case 'W':
			m_timelineCanvas->ZoomIn(step);
			break;
		case 'S':
			m_timelineCanvas->ZoomOut(step);
			break;
		default: break;
	}

	event.Skip(); 
}

//void MainForm::OnDateSpinBtnSpinDown(wxSpinEvent& event)
//{
//	m_timelineCanvas->SetDate(m_timelineCanvas->GetDate() - 1.0);
//
//	UpdateDateText();
//	m_appData->updateDateScrollbar = true;
//	m_image_requires_update = true;
//
//	event.Skip();
//}
//
//void MainForm::OnDateSpinBtnSpinUp(wxSpinEvent& event)
//{
//	m_timelineCanvas->SetDate(m_timelineCanvas->GetDate() + 1.0);
//
//	UpdateDateText();
//	m_appData->updateDateScrollbar = true;
//	m_image_requires_update = true;
//
//	event.Skip();
//}

//void MainForm::OnDateLeftStaticTextLeftDown(wxMouseEvent& event)
//{
//	m_timelineCanvas->SetDate(m_timelineCanvas->GetDate() - 1.0);
//
//	UpdateDateText();
//	m_appData->updateDateScrollbar = true;
//	m_image_requires_update = true;
//
//	event.Skip();
//}
//
//void MainForm::OnDateRightStaticTextLeftDown(wxMouseEvent& event)
//{
//	m_timelineCanvas->SetDate(m_timelineCanvas->GetDate() + 1.0);
//
//	UpdateDateText();
//	m_appData->updateDateScrollbar = true;
//	m_image_requires_update = true;
//
//	event.Skip();
//}

void MainForm::OnDateLeftButtonButtonClick(wxCommandEvent& event)
{
	m_timelineCanvas->SetDate(m_timelineCanvas->GetDate() - 1.0);

	UpdateDateText();
	m_appData->updateDateScrollbar = true;
	m_image_requires_update = true;

	event.Skip();
}

void MainForm::OnDateRightButtonButtonClick(wxCommandEvent& event)
{
	m_timelineCanvas->SetDate(m_timelineCanvas->GetDate() + 1.0);

	UpdateDateText();
	m_appData->updateDateScrollbar = true;
	m_image_requires_update = true;

	event.Skip();
}

void MainForm::OnBitmapLeftDown(wxMouseEvent& event)
{
	if (m_imageDialog->GetImageFilename().empty() == false)
	{
		if (m_imageDialog->ShowModalDialogue() == true)
		{
		}
	}

	event.Skip();
}

void MainForm::OnFindTextCtrlTextEnter(wxCommandEvent& event)
{
	FindText();

	event.Skip();
}

void MainForm::OnFindButtonButtonClick(wxCommandEvent& event)
{
	FindText();

	event.Skip();
}

void MainForm::FindText()
{
	if ((m_findTextCtrl->GetValue().IsEmpty() == false) &&
		(m_appData->eventList.empty() == false))
	{
		std::string searchString = m_findTextCtrl->GetValue();
		std::string str;

		// Convert to lowercase
		SVS::StringUtilities::ToLowerCase(searchString);

		//uint index = m_findIndex + 1;
		uint index = m_timelineCanvas->GetSelectedIndex() + 1;

		for (uint i = 0; i < m_appData->eventList.size(); i++)
		{
			if (index == m_appData->eventList.size())
				index = 0;

			str = m_appData->eventList[index].name;

			// Convert to lowercase
			SVS::StringUtilities::ToLowerCase(str);

			if (str.find(searchString) != std::string::npos)
			{
				// Search string was found
				m_timelineCanvas->SetDate(m_appData->eventList[index].startDate);
				m_appData->updateDateScrollbar = true;
				m_timelineCanvas->SetSelectedIndex(index);
				//m_findIndex = index;
				break;
			}

			index++;
		}
	}
}

void MainForm::OnGoogleSearchButtonButtonClick(wxCommandEvent& event)
{
	int index = m_timelineCanvas->GetSelectedIndex();

	if (index != -1)
	{
		TimelineEventData& historyEvent = m_appData->eventList[index];

		wxString topic = historyEvent.name;

		// 2. Format the Google search URL
		// wxURI::CreateStepwiseEncoded performs basic URL component encoding
		//wxString encodedTopic = wxURI::CreateStepwiseEncoded(topic, wxURI_REGNAME);
		//wxString encodedTopic = wxURI::Escape(topic);
		topic.Replace(" ", "+");
		wxString url = "https://google.com/search?q=" + topic;

		// 3. Open the default system browser
		bool success = wxLaunchDefaultBrowser(url);
	}

	event.Skip();
}

void MainForm::OnWikipediaSearchButtonButtonClick(wxCommandEvent& event)
{
	int index = m_timelineCanvas->GetSelectedIndex();

	if (index != -1)
	{
		TimelineEventData& historyEvent = m_appData->eventList[index];

		wxString topic = historyEvent.name;

		// 2. Format the Google search URL
		// wxURI::CreateStepwiseEncoded performs basic URL component encoding
		//wxString encodedTopic = wxURI::CreateStepwiseEncoded(topic, wxURI_REGNAME);
		//wxString encodedTopic = wxURI::Escape(topic);
		topic.Replace(" ", "+");
		wxString url = "https://en.wikipedia.org/w/index.php?search=" + topic;

		// 3. Open the default system browser
		bool success = wxLaunchDefaultBrowser(url);
	}

	event.Skip();
}

void MainForm::OnMapSearchButtonButtonClick(wxCommandEvent& event)
{
	int index = m_timelineCanvas->GetSelectedIndex();

	if (index != -1)
	{
		TimelineEventData& historyEvent = m_appData->eventList[index];

		//// eg. https://www.google.com/maps/place/41.40338,2.17403/@41.40338,2.17403,10z
		//// 10z is the zoom level which can range from 0 (zoomed out) to 22 (zoomed in)
		//// this method leaves the search bar open and adds a place marker
		//wxString locationStr = std::to_string(lat) + "," + std::to_string(lon) + "/" + "@" + std::to_string(lat) + "," + std::to_string(lon);
		//wxString url = "https://www.google.com/maps/place/" + locationStr + ",10z";

		// eg. http://maps.google.com/maps?ll=54.868705,-1.593018&z=9
		// z= is the zoom level which can range from 0 (zoomed out) to 22 (zoomed in)
		// ll= sets the geographic center of the map using latitude and longitude coordinates
		double lat = historyEvent.latitude;
		double lon = historyEvent.longitude;
		wxString locationStr = std::to_string(lat) + "," + std::to_string(lon);
		wxString url = "https://www.google.com/maps?ll=" + locationStr + "&z=13";

		// 3. Open the default system browser
		bool success = wxLaunchDefaultBrowser(url);
	}

	event.Skip();
}

bool MainForm::LoadHistoryFile(std::string filename)
{
	std::ifstream file(filename);

	if (file.is_open() == false)
	{
		std::string msg = "Could not load file:  \"" + filename + "\"";
		wxMessageDialog(this, msg.c_str(), "Error", wxOK | wxICON_ERROR);
		return false;
	}

	std::string line;
	std::vector<std::string> strs;
	TimelineEventData eventData;
	int id = 0;

	m_appData->eventList.clear();
	m_appData->imageList.clear();
    
	// skip the "heading" line
	if (!std::getline(file, line))
	{
		file.close();
		return false;
	}

	while (std::getline(file, line))
	{
		boost::split(strs, line, boost::is_any_of(","));

		for (auto& str : strs)
		{
			boost::trim(str);											// Remove leading/trailing whitespace and tabs
			boost::trim_if(str, boost::is_any_of("\"'"));	// Strip surrounding quotes
		}

		if (strs.size() < 6)
			continue;	// badly formatted (or blank) line

		// skip "comment" lines (lines that begin with a #)
		if ((strs[0].size() > 1) && (strs[0][0] == '#'))
			continue;

		eventData.name						= strs[0];
		eventData.startDate				= std::stod(strs[1]);
		eventData.endDate					= std::stod(strs[2]);
		eventData.latitude				= std::stod(strs[3]);
		eventData.longitude				= std::stod(strs[4]);
		eventData.imageFilename			= strs[5];
		eventData.id						= id++;

		if (eventData.endDate == eventData.startDate)
		{
			eventData.endDate += l_minimumEventTime;
		}

		if (eventData.name.empty() == false)
		{
			m_appData->eventList.push_back(eventData);
		}

		if (eventData.imageFilename.empty() == false)
		{
			m_appData->imageList.push_back(eventData);
		}
	}

	file.close();

	std::sort(m_appData->eventList.begin(), m_appData->eventList.end(),
					[](const TimelineEventData& a, const TimelineEventData& b) { return a.startDate < b.startDate; });

	std::sort(m_appData->imageList.begin(), m_appData->imageList.end(),
					[](const TimelineEventData& a, const TimelineEventData& b) { return a.startDate < b.startDate; });

	m_appData->SetLatestDate();

	return true;
}

