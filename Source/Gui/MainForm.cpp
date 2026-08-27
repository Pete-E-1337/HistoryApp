#include "MainForm.h"

#include <wx/msgdlg.h>
#include <wx/spinctrl.h>
#include <wx/display.h>
#include <windows.h>
#include <fstream>
#include <sstream>
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
#endif

//#define BLOCK_AMBIENT	// Use these so that the graph doesnt get screwed up while they arent working. Probably no longer required since being replaced by cpProxy
//#define BLOCK_DT		// Use these so that the graph doesnt get screwed up while they arent working. Probably no longer required since being replaced by deltaT

SVS_WARNING_DISABLE(4100) // Unreferenced formal parameter in boost

const char*				l_settingsFilename				= "settings.txt";
static const int		l_guiTimerInterval				= 300;
static const int		l_renderTimerInterval			= 40;
static const int		l_imageUpdateThreadInterval	= 300;
static const double	l_timelineSplitterProportion	= 0.6;

MainForm::MainForm(wxWindow* parent, AppData* appData) :
	History::MainForm(parent),
   m_ioService(std::max((int32_t)std::thread::hardware_concurrency(), 2), SVS::StartupType::Automatic),
	m_appData(appData)
{
//	SetIcon(wxICON(ISENTRYDISKUSAGECONFIGAPP_LOGO));
   Initialise();
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
	m_timelineCanvas->SetFocus();

	m_imageDialog		= new ImageDialog(this);

	m_dateTextCtrl->SetValidator(wxTextValidator(wxFILTER_NUMERIC));

	m_timelineDateScrollBar->SetScrollbar(5000, 1, 10000, 10, true);
	m_timelineZoomScrollBar->SetScrollbar(1, 1, 10000, 10, true);
	SetTimelineDateScrollBarPositionFromDate(0.0);

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
   NOT_USED(event);

   Close();

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
		m_debugTextCtrl->SetValue(m_timelineCanvas->GetDebugString());

		UpdateDateText();
	}

	event.Skip();
}

void MainForm::OnRenderTickTimer(wxTimerEvent& event)
{
//	int deltaTimeMSecs = event.GetInterval();
		
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
	m_timelineCanvas->SetFocus();
	m_updating_date_text		= true;
	m_image_requires_update	= true;

	event.Skip();
}

void MainForm::OnDateTextCtrlLeftDown(wxMouseEvent& event)
{
	m_updating_date_text = false;

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

void MainForm::UpdateImage()
{
	if (m_image_requires_update == true)
	{
		double date = m_timelineCanvas->GetDate();

//		if (SVS::Math::InRange(m_old_date, date - 0.01, date + 0.01) == false)
		{
			bool image_found = false;

			for (TimeLineEventListConstIter iter = m_appData->imageList.begin(); iter != m_appData->imageList.end(); iter++)
			{
				if (SVS::Math::InRange(date, iter->startDate, iter->endDate) == true)
				{
					if (iter->imageFilename.empty() == false)
					{
						int w, h;

						wxBitmap bitmap(iter->imageFilename, wxBITMAP_TYPE_JPEG);

						if (bitmap.IsOk() == false)
						{
//							std::string msg = "Could not load file:  \"" + iter->imageFilename + "\"";
//							wxMessageDialog(this, msg.c_str(), "Error", wxOK | wxICON_ERROR);
//							wxMessageBox(msg.c_str(), "Error", wxOK | wxSTAY_ON_TOP);
							image_found = false;
							break;
						}

						wxImage img = bitmap.ConvertToImage();
//						m_bitmap->GetSize(&w, &h);
						m_bitmapPanel->GetSize(&w, &h);

						if ((w == 0) || (h == 0))
							return;
//							break;

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
//						m_bitmap->Refresh();
//						m_bitmapPanel->Refresh();
						m_bitmapPanel->Layout();

						if (m_imageDialog != nullptr)
						{
							m_imageDialog->SetImageFilename(iter->imageFilename);
						}

						image_found = true;
						break;
					 }
				}
			}

			if (image_found == false)
			{
				// Clear the bitmap
				m_bitmap->SetBitmap(wxNullBitmap);
//				m_bitmap->Refresh();
//				m_bitmapPanel->Refresh();
				m_bitmapPanel->Layout();

				if (m_imageDialog != nullptr)
				{
					m_imageDialog->SetImageFilename("");
				}
			}

			m_old_date = date;
		}

		m_image_requires_update = false;
	}
}

void MainForm::OnMainSplitterSplitterSashPosChanged(wxSplitterEvent& event)
{
	m_image_requires_update = true;

	event.Skip();
}

void MainForm::OnTimelineDateScrollBarScroll(wxScrollEvent& event)
{
	if (m_appData->eventList.empty() == false)
	{
		int pos = event.GetPosition();	// range is 0 to (range - 1) :/
		double percentage = pos / (double)(m_timelineDateScrollBar->GetRange() - 1);
		double date = (percentage * (m_appData->newestDate - m_appData->eventList.begin()->startDate)) + m_appData->eventList.begin()->startDate;

		m_timelineCanvas->SetDate(date);

		UpdateDateText();
		m_image_requires_update = true;
	}

	event.Skip();
}

void MainForm::SetTimelineDateScrollBarPositionFromDate(double date)
{
	double percentage = (date - m_appData->eventList.begin()->startDate) / (m_appData->newestDate - m_appData->eventList.begin()->startDate);
	double pos = SVS::Math::LinearInterpolate(0, m_timelineDateScrollBar->GetRange(), percentage);
	m_timelineDateScrollBar->SetThumbPosition((int)pos);
}

void MainForm::OnTimelineZoomScrollBarScroll(wxScrollEvent& event)
{
	int pos = event.GetPosition();	// range is 0 to (range - 1) :/
	double percentage = pos / (double)(m_timelineZoomScrollBar->GetRange() + 1);

	m_timelineCanvas->SetZoom(percentage);

	event.Skip();
}

void MainForm::OnMainFormKeyDown(wxKeyEvent& event)
{
	// Seems like other controls quickly get focus so probably not a good idea to handle here

//	// https://theasciicode.com.ar/
//
//	float step = (event.ShiftDown() == true) ? 10.0f : 1.0f;
//
//	int keyCode = event.GetKeyCode();
//
//	switch (keyCode)
//	{
//		case 'A':
////			SetDate(GetDate() - step);
//			break;
//		case 'D':
////			SetDate(GetDate() + step);
//			break;
//		case 'W':
////			m_cameraMatrix.SetPositionZ((std::max)(l_nearClipPlane, m_cameraMatrix.GetPositionZ() - step));
//			break;
//		case 'S':
////			m_cameraMatrix.SetPositionZ((std::min)(l_farClipPlane, m_cameraMatrix.GetPositionZ() + step));
//			break;
//	//	case 306:	m_cameraSpeedScale = 2.0;														break;	// Shift key
//	//	case WXK_ESCAPE:
//	//	case 'V':	m_inputData.keyData.exitKeyDown = true;									break;
//		default: break;
//	}

	event.Skip();
}

void MainForm::OnDateSpinBtnSpinDown(wxSpinEvent& event)
{
	m_timelineCanvas->SetDate(m_timelineCanvas->GetDate() - 1.0);

	UpdateDateText();
	m_image_requires_update = true;

	event.Skip();
}

void MainForm::OnDateSpinBtnSpinUp(wxSpinEvent& event)
{
	m_timelineCanvas->SetDate(m_timelineCanvas->GetDate() + 1.0);

	UpdateDateText();
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
