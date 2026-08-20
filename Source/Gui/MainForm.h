#ifndef MAINFORM_H
#define MAINFORM_H

#include "GeneratedFiles/History.h"
#include <SVSLibrary/Std.h>
#include <SVSLibrary/Execution/BoostIoService.h>
#include <SVSLibrary/Execution/Mutex.h>
#include "TimelineGLCanvas.h"
#include "../AppData.h"

class MainForm : public History::MainForm
{
public:
   MainForm(wxWindow* parent, AppData* appData);
   ~MainForm();

	virtual void OnExitButtonClick(wxCommandEvent& event) override;
	virtual void OnGuiTimer(wxTimerEvent& event) override;
	virtual void OnRenderTickTimer(wxTimerEvent& event) override;
	virtual void OnDateTextCtrlOnText(wxCommandEvent& event) override;
	virtual void OnDateTextCtrlTextEnter(wxCommandEvent& event) override;
	virtual void OnDateTextCtrlLeftDown(wxMouseEvent& event) override;
	virtual void OnIdle(wxIdleEvent& event) override;
	virtual void OnTimelineDateScrollBarScroll(wxScrollEvent& event) override;
	virtual void OnTimelineZoomScrollBarScroll(wxScrollEvent& event) override;
	virtual void OnMainFormKeyDown(wxKeyEvent& event) override;
	virtual void OnDateSpinBtnSpinDown(wxSpinEvent& event) override;
	virtual void OnDateSpinBtnSpinUp(wxSpinEvent& event) override;

private:
   void Initialise();
//	void InitialiseSerialComms();
//	bool GetValueString(std::ifstream& file, const std::string& searchStr, std::string& value);
//	bool ParsePidFile(const std::string& filename);
//	bool ParseSensorCalibrationFile(const std::string& filename, SerialComms::SensorCalibrationInfo& sensorCalibrationInfo);
	void LoadSettings();
	void SaveSettings();
//	void StartNewReport();
//	std::string ToString(double num, int decimalPlaces);
	void SetAppData(AppData* appData);
	void SetTimelineDateScrollBarPositionFromDate(double date);
	void UpdateDateText();

private:
   SVS::BoostIoService			m_ioService;
	AppData*							m_appData					= nullptr;
//	AboutDialog*					m_aboutDialog									= nullptr;
// SVS::Mutex						m_graphVectorsLock;
//	FILE*								m_report = nullptr;
	TimelineGLCanvas*				m_timelineCanvas			= nullptr;
	bool								m_updating_date_text		= true;
};

#endif // MAINFORM_H