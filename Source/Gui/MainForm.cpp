#include "MainForm.h"

#include <wx/msgdlg.h>
#include <wx/spinctrl.h>
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
								  
//#define ENABLE_DEBUGGING_GUI

#ifdef _DEBUG
//#define USE_DEBUG_DATA
//#define DEBUG_PID
#endif

//#define BLOCK_AMBIENT	// Use these so that the graph doesnt get screwed up while they arent working. Probably no longer required since being replaced by cpProxy
//#define BLOCK_DT		// Use these so that the graph doesnt get screwed up while they arent working. Probably no longer required since being replaced by deltaT

SVS_WARNING_DISABLE(4100) // Unreferenced formal parameter in boost

const char* l_settingsFilename = "settings.txt";

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
//	m_guiTimer.Stop();
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

	if (m_timelineCanvas != nullptr)
	{
//		delete m_timelineCanvas;	// newed components are deleted by the wxWidgets system
		m_timelineCanvas = nullptr;
	}

}

void MainForm::Initialise()
{
	LoadSettings();

	m_timelineCanvas = new TimelineGLCanvas(m_timelinePanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
	wxSizer* sizer = m_timelinePanel->GetSizer();
	sizer->Add(m_timelineCanvas, 1, wxEXPAND);
	m_timelineCanvas->SetFocus();

	m_dateTextCtrl->SetValidator(wxTextValidator(wxFILTER_NUMERIC));

	m_timelineDateScrollBar->SetScrollbar(5000, 1, 10000, 10, true);
	m_timelineZoomScrollBar->SetScrollbar(1, 1, 10000, 10, true);
	SetTimelineDateScrollBarPositionFromDate(0.0);

	SetAppData(m_appData);

	m_guiTimer.Start(1000);
	m_renderTickTimer.Start(40);
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

void MainForm::OnGuiTimer(wxTimerEvent& event)
{
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
	m_updating_date_text = true;

	event.Skip();
}

void MainForm::OnDateTextCtrlLeftDown(wxMouseEvent& event)
{
	m_updating_date_text = false;

	event.Skip();
}

void MainForm::OnIdle(wxIdleEvent& event)
{
	if (m_timelineCanvas != nullptr)
	{
		m_debugTextCtrl->SetValue(m_timelineCanvas->GetDebugString());

		UpdateDateText();
	}

	event.Skip();
}

void MainForm::UpdateDateText()
{
//	m_dateTextCtrl->SetValue(std::to_string((int)m_timelineCanvas->GetDate()));

	if (m_updating_date_text == true)
	{
		std::string str;
		double date = m_timelineCanvas->GetDate();

		str = TimelineGLCanvas::DateToString(date);

		m_dateTextCtrl->SetValue(str);
	}
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

	event.Skip();
}

void MainForm::OnDateSpinBtnSpinUp(wxSpinEvent& event)
{
	m_timelineCanvas->SetDate(m_timelineCanvas->GetDate() + 1.0);

	UpdateDateText();

	event.Skip();
}
