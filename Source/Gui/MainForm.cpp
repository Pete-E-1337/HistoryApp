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

//#define ENABLE_DEBUGGING_GUI

#ifdef _DEBUG
//#define USE_DEBUG_DATA
//#define DEBUG_PID
#endif

//#define BLOCK_AMBIENT	// Use these so that the graph doesnt get screwed up while they arent working. Probably no longer required since being replaced by cpProxy
//#define BLOCK_DT		// Use these so that the graph doesnt get screwed up while they arent working. Probably no longer required since being replaced by deltaT

SVS_WARNING_DISABLE(4100) // Unreferenced formal parameter in boost

const char* l_settingsFilename = "settings.txt";

MainForm::MainForm(wxWindow* parent) :
	Waveblade::MainForm(parent),
   m_ioService(std::max((int32_t)std::thread::hardware_concurrency(), 2), SVS::StartupType::Automatic)
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
}

void MainForm::Initialise()
{
	LoadSettings();
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

