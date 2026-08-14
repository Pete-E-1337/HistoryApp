#ifndef MAINFORM_H
#define MAINFORM_H

#include "GeneratedFiles/History.h"
#include <SVSLibrary/Std.h>
#include <SVSLibrary/Execution/BoostIoService.h>
#include <SVSLibrary/Execution/Mutex.h>
#include "TimelineGLCanvas.h"

class MainForm : public History::MainForm
{
public:
   MainForm(wxWindow* parent);
   ~MainForm();

	virtual void OnExitButtonClick(wxCommandEvent& event) override;
	virtual void OnGuiTimer(wxTimerEvent& event) override;
	virtual void OnRenderTickTimer(wxTimerEvent& event) override;

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

private:
   SVS::BoostIoService						m_ioService;
//	AboutDialog*								m_aboutDialog									= nullptr;
// SVS::Mutex									m_graphVectorsLock;
//	FILE*											m_report = nullptr;
	TimelineGLCanvas*				m_timelineCanvas		= nullptr;
};

#endif // MAINFORM_H