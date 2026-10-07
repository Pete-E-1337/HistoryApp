#include "MainForm.h"

#include <wx/msgdlg.h>
#include <wx/spinctrl.h>
#include <wx/display.h>
#include <wx/app.h>
#include <wx/richtooltip.h>
//#include <wx/webview.h>
#include <wx/dcmemory.h>
#include <windows.h>
#include <fstream>
#include <sstream>
//#include <chrono>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <SVSLibrary/Std.h>
#include <SVSLibrary/StringUtilities.h>
#include <SVSLibrary/Compiler/Warnings.h>
#include <SVSLibrary/Math/MiscMath.h>
#include <SVSLibrary/Spatial/Vector2D.h>
#include "../PenguinMaps.h"

SVS_WARNING_DISABLE(4189) // local variable is initialized but not referenced
SVS_WARNING_DISABLE(4456) // declaration of 'variable' hides previous local declaration
SVS_WARNING_DISABLE(4101) // unreferenced local variable
//SVS_WARNING_DISABLE(4702) // unreachable code

//#define ENABLE_DEBUGGING_GUI

#ifdef _DEBUG
//#define USE_DEBUG_DATA
//#define DEBUG_PID
#define DRAW_DEBUG_TEXT
//#define DRAW_PENGUIN_LAT_LON_MAPPING_ARRAY
#endif

//#define USE_INTERNAL_BROWSER

SVS_WARNING_DISABLE(4100) // Unreferenced formal parameter in boost

const char*				l_settingsFilename				= "settings.txt";
//const char*				l_historyFilename					= "Data/SampleHistory.csv";
//const char*				l_historyFilename					= "Data/1000_significant_history_events.csv";
//const char*				l_historyFilename					= "Data/75_significant_history_events.csv";
const char*				l_historyFilename					= "Data/x_significant_history_events.csv";
//static const double	l_startingDate						= -2560.0;	// pyramid of giza
//static const double	l_startingDate						= -40500.0;	// debug
static const double	l_startingDate						= 651.0;	// debug
static const int		l_guiTimerInterval				= 300;
static const int		l_imageTimerInterval				= 300;
static const int		l_renderTimerInterval			= 40;
static const int		l_slideshowTimerInterval		= 1000;
static const int		l_imageUpdateThreadInterval	= 300;
static const double	l_timelineSplitterProportion	= 0.6;
static const double	l_oneYear							= 1.0;
static const double	l_oneMonth							= 1.0 / 12.0;
static const double	l_oneFortnight						= 14.0 / 365.0;
static const double	l_oneWeek							= 7.0 / 365.0;
static const double	l_oneDay								= 1.0 / 365.0;
static const double	l_minimumEventTime				= l_oneMonth;
static const int		l_explosionImageSize				= 24;

wxImage l_explosionImage;

MainForm::MainForm(wxWindow* parent, AppData* appData) :
	History::MainForm(parent),
   m_ioService(std::max((int32_t)std::thread::hardware_concurrency(), 2), SVS::StartupType::Automatic),
	m_appData(appData)
{
//	SetIcon(wxICON(ISENTRYDISKUSAGECONFIGAPP_LOGO));

	//double ratio = 0.0;
	//double linear_value, log_value, exp_value;
	//for (int i=0; i<=100; i++)
	//{
	//	linear_value = SVS::Math::LinearInterpolate(10.0, 100.0, ratio);
	//	log_value = SVS::Math::LogarithmicInterpolate(10.0, 100.0, ratio);
	//	exp_value = SVS::Math::ExponentialInterpolate(10.0, 100.0, ratio);
	//	ratio += 0.01;
	//}

	LoadSettings();

	// Call this before loading or processing any images
	wxInitAllImageHandlers();

	l_explosionImage.LoadFile("explosion4.png", wxBITMAP_TYPE_PNG);
	l_explosionImage = l_explosionImage.Scale(l_explosionImageSize, l_explosionImageSize, wxIMAGE_QUALITY_HIGH);

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

//	m_bitmapTooltip	= new wxRichToolTip("", "");

	m_dateTextCtrl->SetValidator(wxTextValidator(wxFILTER_NUMERIC));

//	m_dateLeftStaticText->SetLabel(wxString::FromUTF8(u8"◄")); // \u25C4
//	m_dateRightStaticText->SetLabel(wxString::FromUTF8(u8"►")); // \u25BA
	m_dateLeftButton->SetLabel(wxString::FromUTF8(u8"◄")); // \u25C4
	m_dateRightButton->SetLabel(wxString::FromUTF8(u8"►")); // \u25BA
	m_prevButton->SetLabel("l" + wxString::FromUTF8(u8"◄"));
	m_pauseButton->SetLabel("ll");
	m_playButton->SetLabel(wxString::FromUTF8(u8"►"));
	m_nextButton->SetLabel(wxString::FromUTF8(u8"►") + 'l');

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

	m_googleSearchButton->Enable(true);

	m_splitter2->Disconnect(wxEVT_IDLE, wxIdleEventHandler(History::MainForm::m_splitter2OnIdle), NULL, this);
	m_splitter2->SetSashPosition(1600, true);

//	m_webView->LoadURL("https://google.com");
//	m_webView->LoadURL("https://example.com");
	m_webView->Bind(wxEVT_WEBVIEW_NAVIGATED, &MainForm::OnWebViewNavigated, this);

	SetAppData(m_appData);

	Bind(wxEVT_CHAR_HOOK, &MainForm::OnGlobalCharHook, this);

	m_guiTimer.Start(l_guiTimerInterval);
	m_imageTimer.Start(l_imageTimerInterval);
	m_renderTickTimer.Start(l_renderTimerInterval);

	m_image_requires_update = true;
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
	m_imageTimer.Stop();
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
	//{
	//	DoImageAlignment();
	//}

	wxToolTip::SetAutoPop(24 * 60 * 60 * 1000);	// This is global which is not ideal. Maybe remove it if i end up using a wxRichToolTip

	int id = 0;

	// History and Image File loading
	{
		m_appData->eventList.clear();
		m_appData->imageList.clear();

		LoadHistoryFile(l_historyFilename, id);

		std::sort(m_appData->eventList.begin(), m_appData->eventList.end(),
						[](const TimelineEventData& a, const TimelineEventData& b) { return a.startDate < b.startDate; });

		std::sort(m_appData->imageList.begin(), m_appData->imageList.end(),
						[](const TimelineEventData& a, const TimelineEventData& b) { return a.startDate < b.startDate; });

		m_appData->SetLatestDate();
	}

	// Battle File loading
	{
		m_appData->battleList.clear();

		LoadBattlesFile("Data/battles/100 Battles 10BC to 800CE.csv", id);
		LoadBattlesFile("Data/battles/100 Battles before 10BC.csv", id);
		LoadBattlesFile("Data/battles/100 Battles of WWI.csv", id);
		LoadBattlesFile("Data/battles/100 Battles of WWII.csv", id);
		LoadBattlesFile("Data/battles/100 Battles since 1900.csv", id);
		LoadBattlesFile("Data/battles/100 Battles 800 to 1500 v2.csv", id);
		LoadBattlesFile("Data/battles/100 Battles 1500-1900.csv", id);

		std::sort(m_appData->battleList.begin(), m_appData->battleList.end(),
						[](const BattleData& a, const BattleData& b) { return a.startDate < b.startDate; });

		CalculateBattleMapPixelData();
	}

#ifdef USE_THUMBNAILS
	CreateThumbnails();
#endif

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
		int pos = (int)(l_timelineSplitterProportion * (double)this->GetSize().GetHeight());
		m_mainSplitter->SetSashPosition(pos, true);

		m_firstTimeShown = false;
	}

	event.Skip();
}

void MainForm::ImageUpdateThread(void)
{
	while (m_runImageUpdateThread == true)
	{
		UpdateImage(false);

		boost::this_thread::sleep(boost::posix_time::milliseconds(l_imageUpdateThreadInterval));
	}
}

void MainForm::OnImageTimer(wxTimerEvent& event)
{
	//if (m_timelineCanvas != nullptr)
	//{
	//	UpdateImage(true);
	//}

	event.Skip();
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
	m_updating_date_text_by_gui	= true;
	m_image_requires_update			= true;

	SetTimelineDateScrollBarPositionFromDate((double)date);

	SetFocus();

//	SetDateCtrlText(event.GetString().ToStdString());

	event.Skip();
}

void MainForm::OnDateTextCtrlKillFocus(wxFocusEvent& event)
{
	m_updating_date_text_by_gui = true;

	event.Skip();
}

//void MainForm::SetDateCtrlText(std::string str)
//{
//	int date = std::stoi(str);
//
//	m_timelineCanvas->SetDate(date);
//	m_updating_date_text_by_gui		= true;
//	m_image_requires_update	= true;
//
//	SetTimelineDateScrollBarPositionFromDate((double)date);
//}

void MainForm::OnDateTextCtrlLeftDown(wxMouseEvent& event)
{
	m_updating_date_text_by_gui = false;

	event.Skip();
}

void MainForm::OnDateTextCtrlSetFocus(wxFocusEvent& event)
{
	m_updating_date_text_by_gui = false;

	event.Skip();
}

void MainForm::UpdateDateText()
{
//	m_dateTextCtrl->SetValue(std::to_string((int)m_timelineCanvas->GetDate()));

	if (m_updating_date_text_by_gui == true)
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
		m_appData->updateDateRangeText = false;

		SetDateRangeText();
		SetZoomSliderPosition();
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
		m_appData->updateDateScrollbar = false;

		SetTimelineDateScrollBarPositionFromDate(m_timelineCanvas->GetDate());
		m_image_requires_update = true;
	}
}

void MainForm::UpdateSelection()
{
	if (m_appData->updateSelection == true)
	{
		m_appData->updateSelection = false;

		int index = m_timelineCanvas->GetSelectedIndex();

//		m_googleSearchButton->Enable(index != -1);
		m_wikipediaSearchButton->Enable(index != -1);
		m_mapSearchButton->Enable((index != -1) &&
										  (m_appData->eventList[index].latitude != l_latLongUninitialized) &&
										  (m_appData->eventList[index].longitude != l_latLongUninitialized));
	}
}

#ifdef USE_THUMBNAILS
void MainForm::CreateThumbnails()
{
	for (TimeLineEventListIter iter = m_appData->imageList.begin(); iter != m_appData->imageList.end(); iter++)
	{
		if (iter->imageFilename.empty() == false)
		{
			wxBitmap bitmap(iter->imageFilename, wxBITMAP_TYPE_JPEG);

			if (bitmap.IsOk() == false)
			{
				continue;
			}

			wxImage img = bitmap.ConvertToImage();
			wxImage shrunkImg = img.Scale(50, 40, wxIMAGE_QUALITY_HIGH);

			iter->thumbnailBitmap = wxBitmap(shrunkImg);
			iter->thumbnailImage	= shrunkImg;
		}
	}
}
#endif

void MainForm::DeferredBitmapUpdate()
{
//	m_bitmap->SetBitmap(m_shrunkImg);	// Calling this causes a app freeze. Tried mutex but it hasnt fixed the problem.

	DrawImageAndBattles();

	m_bitmapPanel->Layout();
}

void MainForm::UpdateImage(bool calledFromGUI)
{
	if (m_appData->imageList.size() == 0)
	{
		return;
	}

	if (m_image_requires_update == true)
	{
		double date = m_timelineCanvas->GetDate();

		bool image_found = false;
		int i = 0;

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

//					wxImage shrunkImg = img.Scale(w, h, wxIMAGE_QUALITY_HIGH);
					m_shrunkImg = img.Scale(w, h, wxIMAGE_QUALITY_HIGH);

//					if (shrunkImg.IsOk())
					if (m_shrunkImg.IsOk())
					{
//						m_bitmapMutex.lock();
//						m_bitmap->SetBitmap(shrunkImg);
//						m_bitmapMutex.unlock();
//						m_bitmapPanel->Layout();

						if (calledFromGUI == true)
						{
//							m_bitmap->SetBitmap(m_shrunkImg);	// Calling this causes a app freeze. Tried mutex but it hasnt fixed the problem.

							DrawImageAndBattles();

							m_bitmapPanel->Layout();
						}
						else
						{
							// Defer bitmap update to avoid re-entrancy and because we should never call UI methods like SetBitmap()
							// directly from worker threads (even though this is what i have specifically done to avoid stagnating
							// the GUI) because it can cause application freezes.
							CallAfter([this](){ DeferredBitmapUpdate(); });
						}
					}

					if (m_imageDialog != nullptr)
					{
						m_imageDialog->SetImageFilename(iter->imageFilename);
					}

					image_found = true;
					m_oldImageId = iter->id;
					m_imageIndex = i;
					break;
				}
			}

			i++;
		}

		if (image_found == false)
		{
			// Clear the bitmap
//			m_bitmapMutex.lock();
			m_bitmap->SetBitmap(wxNullBitmap);
//			m_bitmapMutex.unlock();
			m_bitmapPanel->Layout();

			if (m_imageDialog != nullptr)
			{
				m_imageDialog->SetImageFilename("");
			}

			m_bitmap->SetToolTip(wxEmptyString);

			m_oldImageId = -1;
			m_imageIndex = -1;
		}

		m_image_requires_update = false;
	}
}

void MainForm::DrawImageAndBattles()
{
	//if (m_imageIndex == -1)
	//	return;

//	wxBitmap battleBmp("explosion.png", wxBITMAP_TYPE_PNG);
	wxBitmap battleBmp(l_explosionImage);

//	m_drawingBitmap = m_bitmap->GetBitmap();
	wxBitmap bitmap = wxBitmap(m_shrunkImg);

	// 2. Create a memory DC to draw on the bitmap
	wxMemoryDC memDC;
	memDC.SelectObject(bitmap);

	//// 3. Draw your shapes, text, or images
	//memDC.SetBackground(*wxWHITE_BRUSH);
	//memDC.Clear();
	//memDC.SetPen(*wxRED_PEN);
	//memDC.SetBrush(*wxBLUE_BRUSH);
	//memDC.DrawCircle(100, 100, 50);
	//memDC.DrawText("Hello wxWidgets", 50, 100);

//	memDC.DrawBitmap(foregroundBmp, 100, 100, true);

	int w = bitmap.GetWidth();
	int h = bitmap.GetHeight();
	float x_scale = (float)w / (float)2570;
	float y_scale = (float)h / (float)2088;
	int half_battle_image_width	= battleBmp.GetWidth() / 2;
	int half_battle_image_height	= battleBmp.GetHeight() / 2;

#ifdef DRAW_PENGUIN_LAT_LON_MAPPING_ARRAY
	// Draw "battles" at the recorded array mercator points
	{
		for (int row = 0; row < 10; row++)
			for (int col = 0; col < 11; col++)
				memDC.DrawBitmap(battleBmp, (int)(l_map_pixel_data[row][col].x * x_scale - half_battle_image_width), (int)(l_map_pixel_data[row][col].y * y_scale - half_battle_image_height), true);
	}
#endif

	//// Draw a "battle" at York England (53.958332, -1.080278)
	//{
	//	MapPixelData pixel_data;

	//	PenguinMaps::GetPixelLocation(53.958332, -1.080278, &pixel_data);
	//	memDC.DrawBitmap(battleBmp, (int)(pixel_data.x * x_scale - half_battle_image_width), (int)(pixel_data.y * y_scale - half_battle_image_height), true);
	//}

	m_displayedBattleList.clear();

	//// in the year the battle occurs
	//double startYear	= m_timelineCanvas->GetDate();
	//double endYear		= startYear + 1;

	// in the range of the current map
	assert(m_imageIndex != -1);
	double startYear	= m_appData->imageList[m_imageIndex].startDate;
	double endYear		= m_appData->imageList[m_imageIndex].endDate;

	for (BattleListIter iter = m_appData->battleList.begin(); iter != m_appData->battleList.end(); iter++)
	{
//		if (SVS::Math::InRange(iter->startDate, startYear, endYear) == true)
		if (SVS::Math::InRange(iter->startDate, startYear, endYear) == true)
		{
			memDC.DrawBitmap(battleBmp, (int)(iter->mapImageXCoord * x_scale - half_battle_image_width), (int)(iter->mapImageYCoord * y_scale - half_battle_image_height), true);
			m_displayedBattleList.push_back(*iter);
		}
		//PenguinMaps::GetPixelLocation(iter->latitude, iter->longitude, &pixel_data);
		//iter->mapImageXCoord = pixel_data.x;
		//iter->mapImageYCoord = pixel_data.y;
	}

	// 4. Deselect the bitmap so it's free to use
	memDC.SelectObject(wxNullBitmap);

	m_bitmap->SetBitmap(bitmap);	// Calling this causes a app freeze. Tried mutex but it hasnt fixed the problem.
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

#ifdef USE_THUMBNAILS
		{
			bool image_found = false;

			for (TimeLineEventListConstIter iter = m_appData->imageList.begin(); iter != m_appData->imageList.end(); iter++)
			{
				if (SVS::Math::InRange(date, iter->startDate, iter->endDate) == true)
				{
//					if (iter->id == m_oldThumbnailId)
					if (iter->id == m_oldImageId)
					{
						image_found = true;
						break;	// no need to reload the same image
					}

					if (iter->imageFilename.empty() == false)
					{
#ifdef USE_THUMBNAIL_RESIZE
						int w, h;

//						wxImage img = iter->thumbnailBitmap.ConvertToImage();
						m_bitmapPanel->GetSize(&w, &h);

						if ((w == 0) || (h == 0))
						{
							image_found = false;
							break;
						}

						// maintain aspect ratio
						{
//							double imgRatio = (double)img.GetWidth() / img.GetHeight();
							double imgRatio = (double)iter->thumbnailImage.GetWidth() / iter->thumbnailImage.GetHeight();
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

//						wxImage shrunkImg = img.Scale(w, h, wxIMAGE_QUALITY_HIGH);
						wxImage shrunkImg = iter->thumbnailImage.Scale(w, h, wxIMAGE_QUALITY_HIGH);
//						m_bitmapMutex.lock();
						m_bitmap->SetBitmap(shrunkImg);
//						m_bitmapMutex.unlock();
#else
//						m_bitmapMutex.lock();
						m_bitmap->SetBitmap(iter->thumbnailBitmap);	// Calling this causes a app freeze. Tried mutex but it hasnt fixed the problem.
//						m_bitmapMutex.unlock();

						// Defer bitmap update to avoid re-entrancy
//						wxCallAfter([this]() { DeferredBitmapUpdate(); });
#endif

						m_bitmapPanel->Layout();

						image_found = true;
//						m_oldThumbnailId = iter->id;
						break;
					}
				}
			}

			if (image_found == false)
			{
				// Clear the bitmap
				//m_bitmap->SetBitmap(wxNullBitmap);
				//m_bitmapPanel->Layout();

//				m_oldThumbnailId = -1;
			}
		}
#endif

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
	if (m_displayedBattleListIndex != -1)
	{
		BattleData& battleData = m_displayedBattleList[m_displayedBattleListIndex];

		wxString topic = battleData.name;

		// 2. Format the Google search URL
		// wxURI::CreateStepwiseEncoded performs basic URL component encoding
		//wxString encodedTopic = wxURI::CreateStepwiseEncoded(topic, wxURI_REGNAME);
		//wxString encodedTopic = wxURI::Escape(topic);
		topic.Replace(" ", "+");
		wxString url = "https://en.wikipedia.org/w/index.php?search=" + topic;

#ifdef USE_INTERNAL_BROWSER
	m_webView->LoadURL(url);
#else
		// 3. Open the default system browser
		bool success = wxLaunchDefaultBrowser(url);
#endif
	}

	event.Skip();
}

void MainForm::OnBitmapLeftDClick(wxMouseEvent& event)
{
	if (m_imageDialog->GetImageFilename().empty() == false)
	{
		if (m_imageDialog->ShowModalDialogue() == true)
		{
		}
	}

	event.Skip();
}

void MainForm::OnBitmapMotion(wxMouseEvent& event)
{
	if (m_imageIndex != -1)
	{
		int mouseX = event.GetX();
		int mouseY = event.GetY();
//		int mouseY = h - event.GetY() - 1;

		int shrunkW, shrunkH;
		shrunkW = m_shrunkImg.GetWidth();
		shrunkH = m_shrunkImg.GetHeight();

		int bitmapW, bitmapH;
		m_bitmap->GetSize(&bitmapW, &bitmapH);

		//int bitmapPanelW, bitmapPanelH;
		//m_bitmapPanel->GetSize(&bitmapPanelW, &bitmapPanelH);

		int bitmapOffsetX = (bitmapW - shrunkW) / 2;
		int bitmapOffsetY = (bitmapH - shrunkH) / 2;

		int bitmapX = mouseX - bitmapOffsetX;
		int bitmapY = mouseY - bitmapOffsetY;

		double pixelX = (bitmapX / (double)shrunkW) * PenguinMaps::GetMapWidth();
		double pixelY = (bitmapY / (double)shrunkH) * PenguinMaps::GetMapHeight();

		SVS::Vector2Dd pixelLocation = SVS::Vector2Dd(pixelX, pixelY);

		double shortestDistanceSquared	= std::numeric_limits<double>::max();
		double distanceSquared				= 0.0;
		int i										= 0;

		m_displayedBattleListIndex			= -1;

		// Test if we are hovering over a battle
		for (BattleListIter iter = m_displayedBattleList.begin(); iter != m_displayedBattleList.end(); iter++)
		{
			if ((SVS::Math::Abs(pixelX - iter->mapImageXCoord) < 10.0) &&
				 (SVS::Math::Abs(pixelY - iter->mapImageYCoord) < 10.0))
			{
				distanceSquared = (SVS::Vector2Dd(iter->mapImageXCoord, iter->mapImageYCoord) - pixelLocation).LengthSq();

				if (distanceSquared < shortestDistanceSquared)
				{
					shortestDistanceSquared = distanceSquared;
					m_displayedBattleListIndex = i;
				}
			}

			i++;
		}

		if (m_displayedBattleListIndex != -1)
		{
			//// Create the rich tooltip with a title and message
			//wxRichToolTip tip(iter->name, "This tooltip will stay open indefinitely!");
			//// Set timeout to 0 to disable automatic hiding entirely
			//tip.SetTimeout(0); 
			//// Show it for a specific window
			//tip.ShowFor(m_bitmap);

			//m_bitmapTooltip->SetTitle
			//m_bitmapTooltip.SetTimeout(0); 
			//m_bitmapTooltip.ShowFor(m_bitmap);

			BattleData& battleData = m_displayedBattleList[m_displayedBattleListIndex];

			std::string str = battleData.name + '\n' +
				"Started: " + battleData.startDateString + '\n' +
				"Ended: " + battleData.endDateString + '\n' +
				"Personnel: " + battleData.personnel + '\n' +
				"Combatants: " + battleData.combatants + '\n' +
				"Winner: " + battleData.winner + '\n' +
				"Victory Size: " + battleData.victory_size;

			m_bitmap->SetToolTip(str);
		}
		else
		{
			m_bitmap->SetToolTip(wxEmptyString);
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
	wxString topic;

	int index = m_timelineCanvas->GetSelectedIndex();

	if (index != -1)
	{
		TimelineEventData& historyEvent = m_appData->eventList[index];

		topic = historyEvent.name;
	}
	else
	{
		//double date = m_timelineCanvas->GetDate();
		//std::string str = TimelineGLCanvas::DateToString(date);
		topic = "historic events in the year " + TimelineGLCanvas::DateToString(m_timelineCanvas->GetDate());
	}

	// 2. Format the Google search URL
	// wxURI::CreateStepwiseEncoded performs basic URL component encoding
	//wxString encodedTopic = wxURI::CreateStepwiseEncoded(topic, wxURI_REGNAME);
	//wxString encodedTopic = wxURI::Escape(topic);
	topic.Replace(" ", "+");
	wxString url = "https://google.com/search?q=" + topic;

	// 3. Open the default system browser
	bool success = wxLaunchDefaultBrowser(url);

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

#ifdef USE_INTERNAL_BROWSER
	m_webView->LoadURL(url);
#else
		// 3. Open the default system browser
		bool success = wxLaunchDefaultBrowser(url);
#endif
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

void MainForm::OnPrevButtonButtonClick(wxCommandEvent& event)
{
	if (m_appData->imageList.empty() == false)
	{
		if (m_imageIndex == 0)
		{
			m_imageIndex = (int)m_appData->imageList.size();
		}

		m_imageIndex--;

		m_timelineCanvas->SetDate(m_appData->imageList[m_imageIndex].startDate);
		m_appData->updateDateScrollbar = true;
		UpdateImage(true);	// bypass the thread load and load directly
	}

	event.Skip();
}

void MainForm::OnPauseButtonButtonClick(wxCommandEvent& event)
{
	m_playing = false;
	m_slideshowTimer.Stop();

	event.Skip();
}

void MainForm::OnPlayButtonButtonClick(wxCommandEvent& event)
{
	m_playing = true;
//	m_slideshowCounter = 0;
	m_slideshowTimer.Start(m_slideshowDisplayTimeSpinCtrl->GetValue() * l_slideshowTimerInterval);

	event.Skip();
}

void MainForm::OnSlideshowDisplayTimeSpinCtrlSpinCtrl(wxSpinEvent& event)
{
	if (m_slideshowTimer.IsRunning() == true)
	{
		m_slideshowTimer.Stop();
		m_slideshowTimer.Start(m_slideshowDisplayTimeSpinCtrl->GetValue() * l_slideshowTimerInterval);
	}

	event.Skip();
}

void MainForm::OnSlideshowTimer(wxTimerEvent& event)
{
//	m_slideshowCounter++;

//	if (m_slideshowCounter >= m_slideshowDisplayTimeSpinCtrl->GetValue())
	{
//		m_slideshowCounter = 0;

		if (m_appData->imageList.empty() == false)
		{
			m_imageIndex++;

			if (m_imageIndex == (int)m_appData->imageList.size())
			{
				m_imageIndex = 0;
			}

			m_timelineCanvas->SetDate(m_appData->imageList[m_imageIndex].startDate);
			m_appData->updateDateScrollbar = true;
			UpdateImage(true);	// bypass the thread load and load directly
		}
	}

	event.Skip();
}

void MainForm::OnNextButtonButtonClick(wxCommandEvent& event)
{
	if (m_appData->imageList.empty() == false)
	{
		m_imageIndex++;

		if (m_imageIndex == (int)m_appData->imageList.size())
		{
			m_imageIndex = 0;
		}

		m_timelineCanvas->SetDate(m_appData->imageList[m_imageIndex].startDate);
		m_appData->updateDateScrollbar = true;
		UpdateImage(true);	// bypass the thread load and load directly
	}

	event.Skip();
}

bool MainForm::LoadHistoryFile(std::string filename, int& id)
{
	std::ifstream file(filename);
	std::string msg;

//wxMessageBox("A problem occurred saving the file.", "Error", wxOK | wxSTAY_ON_TOP);
//std::string msg = "DEBUGGING TEST: \"" + filename + "\"";
//wxMessageDialog(nullptr, msg.c_str(), "Error", wxOK | wxICON_ERROR);

	if (file.is_open() == false)
	{
		msg = "Could not load file: \"" + filename + "\"";
		wxMessageBox(msg.c_str(), "Error", wxOK | wxICON_ERROR);
		return false;
	}

	std::string line;
	std::vector<std::string> strs;
	TimelineEventData eventData;

	// skip the "heading" line
	if (!std::getline(file, line))
	{
		file.close();
		return false;
	}

	int lineNum = 1;

	while (std::getline(file, line))
	{
		lineNum++;

		if (line.empty() == true)
			continue; // skip blank lines

		boost::split(strs, line, boost::is_any_of(","));

		for (auto& str : strs)
		{
			boost::trim(str);											// Remove leading/trailing whitespace and tabs
			boost::trim_if(str, boost::is_any_of("\"'"));	// Strip surrounding quotes
		}

		if (strs.size() < 6)
		{
			msg = "Error in file: \"" + filename + "\" - line " + std::to_string(lineNum) + ". Incorrect number of fields.";
			wxMessageBox(msg.c_str(), "Error", wxOK | wxICON_ERROR);
			continue;	// badly formatted (or blank) line
		}

		// skip "comment" lines (lines that begin with a #)
		if ((strs[0].size() > 0) && (strs[0][0] == '#'))
			continue;

		msg = "Error in file: \"" + filename + "\" - line " + std::to_string(lineNum) + ". Invalid number.";

		eventData.name						= strs[0];
		eventData.startDate				= StringToDouble(strs[1], msg);
		eventData.endDate					= StringToDouble(strs[2], msg);
		eventData.latitude				= StringToDouble(strs[3], msg);
		eventData.longitude				= StringToDouble(strs[4], msg);
		eventData.imageFilename			= strs[5];
		eventData.id						= id++;

		if (eventData.endDate < eventData.startDate)
		{
			msg = "Error in file: \"" + filename + "\" - line " + std::to_string(lineNum) + ". endDate < startDate.";
			wxMessageBox(msg.c_str(), "Error", wxOK | wxICON_ERROR);
			continue;
		}
		else if (eventData.endDate == eventData.startDate)
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

	return true;
}

bool MainForm::LoadBattlesFile(std::string filename, int& id)
{
	std::ifstream file(filename);
	std::string msg;

	if (file.is_open() == false)
	{
		msg = "Could not load file: \"" + filename + "\"";
		wxMessageBox(msg.c_str(), "Error", wxOK | wxICON_ERROR);
		return false;
	}

	std::string line;
	std::vector<std::string> strs;
	BattleData battleData;

	// skip the "heading" line
	if (!std::getline(file, line))
	{
		file.close();
		return false;
	}

	int lineNum = 1;

	while (std::getline(file, line))
	{
		lineNum++;

		if (line.empty() == true)
			continue; // skip blank lines

		boost::split(strs, line, boost::is_any_of(","));

		for (auto& str : strs)
		{
			boost::trim(str);											// Remove leading/trailing whitespace and tabs
			boost::trim_if(str, boost::is_any_of("\"'"));	// Strip surrounding quotes
		}

		if (strs.size() < 9)
		{
			msg = "Error in file: \"" + filename + "\" - line " + std::to_string(lineNum) + ". Incorrect number of fields.";
			wxMessageBox(msg.c_str(), "Error", wxOK | wxICON_ERROR);
			continue;	// badly formatted (or blank) line
		}

		// skip "comment" lines (lines that begin with a #)
		if ((strs[0].size() > 0) && (strs[0][0] == '#'))
			continue;

		msg = "Error in file: \"" + filename + "\" - line " + std::to_string(lineNum) + ". Invalid number.";

//Name of Battle,Latitude,Longitude,Start date,End date,Personnel involved,Combatants,Winner,Victory size
//Battle of Poland,52.2297,21.0122,1939-09-01,1939-10-06,~1500000,Germany-Soviet Union vs Poland,Germany-Soviet Union,Complete victory
		battleData.name					= strs[0];
		battleData.latitude				= StringToDouble(strs[1], msg);
		battleData.longitude				= StringToDouble(strs[2], msg);
		battleData.startDate				= DateStringToDouble(strs[3], msg);
		battleData.endDate				= DateStringToDouble(strs[4], msg);
		battleData.personnel				= strs[5];
		battleData.combatants			= strs[6];
		battleData.winner					= strs[7];
		battleData.victory_size			= strs[8];
		battleData.startDateString		= DateStringNeaten(strs[3]);
		battleData.endDateString		= DateStringNeaten(strs[4]);
		battleData.id						= id++;

		if (battleData.endDate < battleData.startDate)
		{
			msg = "Error in file: \"" + filename + "\" - line " + std::to_string(lineNum) + ". endDate < startDate.";
			wxMessageBox(msg.c_str(), "Error", wxOK | wxICON_ERROR);
			continue;
		}
		//else if (battleData.endDate == battleData.startDate)
		//{
		//	battleData.endDate += l_minimumEventTime;
		//}

		if (battleData.name.empty() == false)
		{
			m_appData->battleList.push_back(battleData);
		}
	}

	file.close();

	return true;
}

double MainForm::StringToDouble(const std::string& str, const std::string& msg)
{
	double num = 0.0;

	try
	{
		num = std::stod(str);
	}
	catch (const std::invalid_argument& e)
	{
		wxMessageBox(msg.c_str(), "Error", wxOK | wxICON_ERROR);
	}

	return num;
}

std::string MainForm::DateStringNeaten(const std::string& str)
{
	std::string neatenedStr = str;

	bool isNegativeYear = (str[0] == '-');

	if (isNegativeYear == true)
	{
		boost::trim_if(neatenedStr, boost::is_any_of("-"));	// Strip beginning negative symbol
		neatenedStr += " BCE";
	}
	else
	{
		neatenedStr += " CE";
	}

	return neatenedStr;
}

double MainForm::DateStringToDouble(const std::string& str, const std::string& msg)
{
	std::vector<std::string> strs;
	std::string tempStr = str;

	int negativeMultiplier = (tempStr[0] == '-') ? -1 : 1;
//	bool isNegativeYear = str[0] == '-');
	boost::trim_if(tempStr, boost::is_any_of("-"));	// Strip beginning negative symbol

	boost::split(strs, tempStr, boost::is_any_of("/-."));

	double num = 0.0;
	int numStrs = strs.size();

	if (numStrs == 0)
	{
		try
		{
			num = std::stod(strs[0]);
		}
		catch (const std::invalid_argument& e)
		{
			wxMessageBox(msg.c_str(), "Error", wxOK | wxICON_ERROR);
		}
	}
	else if (numStrs == 3)
	{
		try
		{
			int year		= std::stoi(strs[0]);
			int month	= std::stoi(strs[1]);
			int day		= std::stoi(strs[2]);

			assert(month <= 12);
			assert(day <= 31);

			year *= negativeMultiplier;
			num = year + (GetDayOfYear(year, month, day) / 365.0);
		}
		catch (const std::invalid_argument& e)
		{
			wxMessageBox(msg.c_str(), "Error", wxOK | wxICON_ERROR);
		}
	}

	return num;
}

bool MainForm::IsLeapYear(int year)
{
	return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// Returns 0 for Jan 1st, 1, for Jan 2nd, etc.
int MainForm::GetDayOfYear(int year, int month, int day)
{
	constexpr int days_before_month[2][12] =
	{
		{0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334}, // Normal year
		{0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335}  // Leap year
	};

	return days_before_month[IsLeapYear(year)][month - 1] + day - 1;
}

void MainForm::CalculateBattleMapPixelData()
{
	MapPixelData pixel_data;

	for (BattleListIter iter = m_appData->battleList.begin(); iter != m_appData->battleList.end(); iter++)
	{
		PenguinMaps::GetPixelLocation(iter->latitude, iter->longitude, &pixel_data);
		iter->mapImageXCoord = pixel_data.x;
		iter->mapImageYCoord = pixel_data.y;
	}
}

void MainForm::OnWebViewNavigated(wxWebViewEvent& event)
{
	// The moment a new page finishes loading/navigating, 
	// manually demand that the UI update its canvas boundaries.
	if (m_webView)
	{
		m_webView->Layout();
		m_webView->Refresh();
		m_webView->Update(); // Instantly flushes the OS painting queue
	}

	event.Skip(); // Ensure other internal processes can read this event safely
}





#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/calib3d.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// Function to align a moving map image to a reference map image
Mat AlignMapImages(const Mat& referenceImage, const Mat& imageToAlign)
{
    // 1. Convert images to grayscale
    Mat grayRef, grayAlign;
    cvtColor(referenceImage, grayRef, COLOR_BGR2GRAY);
    cvtColor(imageToAlign, grayAlign, COLOR_BGR2GRAY);

    // 2. Initialize ORB detector
    int maxFeatures = 5000;
    Ptr<ORB> orb = ORB::create(maxFeatures);

    vector<KeyPoint> keypointsRef, keypointsAlign;
    Mat descriptorsRef, descriptorsAlign;

    // 3. Detect keypoints and compute descriptors
    orb->detectAndCompute(grayRef, noArray(), keypointsRef, descriptorsRef);
    orb->detectAndCompute(grayAlign, noArray(), keypointsAlign, descriptorsAlign);

    // 4. Match descriptors using Hamming distance
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::BRUTEFORCE_HAMMING);
    vector<DMatch> matches;
    matcher->match(descriptorsRef, descriptorsAlign, matches);

    // 5. Sort matches by score/distance
    sort(matches.begin(), matches.end(), [](const DMatch& a, const DMatch& b)
	 {
        return a.distance < b.distance;
    });

    // Keep only the best matches (top 15%)
    int numGoodMatches = max(10, (int)(matches.size() * 0.15));
    matches.erase(matches.begin() + numGoodMatches, matches.end());

    // 6. Extract location of best matches
    vector<Point2f> pointsRef, pointsAlign;

    for (size_t i = 0; i < matches.size(); i++)
	 {
        pointsRef.push_back(keypointsRef[matches[i].queryIdx].pt);
        pointsAlign.push_back(keypointsAlign[matches[i].trainIdx].pt);
    }

    // 7. Find Homography matrix using RANSAC
    Mat homography = findHomography(pointsAlign, pointsRef, RANSAC);

    // 8. Warp the image to align it with the reference
    Mat alignedImage;
    warpPerspective(imageToAlign, alignedImage, homography, referenceImage.size());

    return alignedImage;
}

void MainForm::DoImageAlignment()
{
    // Load reference map and the map to register
    Mat refMap = imread("D:/Work/Projects/HistoryApp/Data/Images - Aligned/2570x2088/_Index Map 2570 x 2088.JPG", IMREAD_COLOR);
    Mat moveMap = imread("D:/Work/Projects/HistoryApp/Data/Images - Aligned/2570x2088/14CE-AH.jpg", IMREAD_COLOR);

    if (refMap.empty() || moveMap.empty())
	 {
        cerr << "Error loading images! Check file paths." << endl;
		  assert(false);
        return;
    }

    // Perform registration
    Mat registeredMap = AlignMapImages(refMap, moveMap);

    // Save and display result
    imwrite("D:/Work/Projects/HistoryApp/Data/Images - Aligned/output/aligned_map_output.png", registeredMap);
    cout << "Map registration complete. Saved output as aligned_map_output.png" << endl;
}
