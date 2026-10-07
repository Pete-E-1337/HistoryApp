#ifndef MAINFORM_H
#define MAINFORM_H

#pragma warning(push)
#pragma warning(disable : 4996)
// Include your generated wxFormBuilder header file here
#include "GeneratedFiles/History.h"
#pragma warning(pop)

#include <SVSLibrary/Std.h>
#include <SVSLibrary/Execution/BoostIoService.h>
#include <SVSLibrary/Execution/Mutex.h>
#include "ImageDialog.h"
#include "TimelineGLCanvas.h"
#include "../AppData.h"
#include <boost/thread.hpp>
//#include <mutex>

class MainForm : public History::MainForm
{
public:
   MainForm(wxWindow* parent, AppData* appData);
   ~MainForm();

	virtual void OnGuiTimer(wxTimerEvent& event) override;
	virtual void OnImageTimer(wxTimerEvent& event) override;
	virtual void OnRenderTickTimer(wxTimerEvent& event) override;
	virtual void OnSlideshowTimer(wxTimerEvent& event) override;
	virtual void OnDateTextCtrlOnText(wxCommandEvent& event) override;
	virtual void OnDateTextCtrlTextEnter(wxCommandEvent& event) override;
	virtual void OnDateTextCtrlLeftDown(wxMouseEvent& event) override;
	virtual void OnDateTextCtrlKillFocus(wxFocusEvent& event) override;
	virtual void OnDateTextCtrlSetFocus(wxFocusEvent& event) override;
	virtual void OnExitButtonClick(wxCommandEvent& event) override;
	virtual void OnClose(wxCloseEvent& event) override;
	virtual void OnIdle(wxIdleEvent& event) override;
	virtual void OnTimelineDateScrollBarScroll(wxScrollEvent& event) override;
//	virtual void OnTimelineZoomScrollBarScroll(wxScrollEvent& event) override;
	virtual void OnTimelineZoomSliderScroll(wxScrollEvent& event) override;
	//virtual void OnDateSpinBtnSpinDown(wxSpinEvent& event) override;
	//virtual void OnDateSpinBtnSpinUp(wxSpinEvent& event) override;
	virtual void OnBitmapLeftDown(wxMouseEvent& event) override;
	virtual void OnBitmapLeftDClick(wxMouseEvent& event) override;
	virtual void OnBitmapMotion(wxMouseEvent& event) override;
	virtual void OnMainSplitterSplitterSashPosChanged(wxSplitterEvent& event) override;
	//virtual void OnDateLeftStaticTextLeftDown(wxMouseEvent& event) override;
	//virtual void OnDateRightStaticTextLeftDown(wxMouseEvent& event) override;
	virtual void OnDateLeftButtonButtonClick(wxCommandEvent& event) override;
	virtual void OnDateRightButtonButtonClick(wxCommandEvent& event) override;
	virtual void OnFindTextCtrlTextEnter(wxCommandEvent& event) override;
	virtual void OnFindButtonButtonClick(wxCommandEvent& event) override;
	virtual void OnGoogleSearchButtonButtonClick(wxCommandEvent& event) override;
	virtual void OnWikipediaSearchButtonButtonClick(wxCommandEvent& event) override;
	virtual void OnMapSearchButtonButtonClick(wxCommandEvent& event) override;
	virtual void OnPrevButtonButtonClick(wxCommandEvent& event) override;
	virtual void OnPauseButtonButtonClick(wxCommandEvent& event) override;
	virtual void OnPlayButtonButtonClick(wxCommandEvent& event) override;
	virtual void OnNextButtonButtonClick(wxCommandEvent& event) override;
	virtual void OnSlideshowDisplayTimeSpinCtrlSpinCtrl(wxSpinEvent& event) override;
   void Initialise();

	// Image Alignment
	void DoImageAlignment();
//	Mat AlignMapImages(const Mat& referenceImage, const Mat& imageToAlign);

private:
	void OnWebViewNavigated(wxWebViewEvent& event);
	void OnGlobalCharHook(wxKeyEvent& event);
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
	void SetZoomSliderPosition();
	void UpdateDateText();
	void UpdateDateRangeText();
//	void SetDateCtrlText(std::string str);
	void SetDateRangeText();
	void UpdateDateScrollbar();
	void UpdateSelection();
	void UpdateImage(bool calledFromGUI);
	void ImageUpdateThread(void);
	bool LoadHistoryFile(std::string filename, int& id);
	bool LoadBattlesFile(std::string filename, int& id);
	void FindText();
	double StringToDouble(const std::string& str, const std::string& msg);
	double DateStringToDouble(const std::string& str, const std::string& msg);
	std::string DateStringNeaten(const std::string& str);
	bool IsLeapYear(int year);
	int GetDayOfYear(int year, int month, int day);
	void CalculateBattleMapPixelData();
#ifdef USE_THUMBNAILS
	void CreateThumbnails();
#endif
	void DeferredBitmapUpdate();
	void DrawImageAndBattles();

private:
//	wxRichToolTip*					m_bitmapTooltip					= nullptr;	// useless because the text cannot be changed dynamically
	ImageDialog*					m_imageDialog						= nullptr;
   SVS::BoostIoService			m_ioService;
	AppData*							m_appData							= nullptr;
//	AboutDialog*					m_aboutDialog											= nullptr;
// SVS::Mutex						m_graphVectorsLock;
//	FILE*								m_report = nullptr;
	TimelineGLCanvas*				m_timelineCanvas					= nullptr;
	bool								m_firstTimeShown					= true;
	bool								m_updating_date_text_by_gui	= true;	// halts program updates while being manually entered by the user
	bool								m_image_requires_update			= true;
//	double							m_old_date							= std::numeric_limits<double>::lowest();
	int								m_oldImageId						= -1;
#ifdef USE_THUMBNAILS
//	int								m_oldThumbnailId					= -1;
#endif
	bool								m_runImageUpdateThread			= false;
	boost::thread*					m_imageUpdateThread				= nullptr;
	wxLongLong						m_lastRenderTimeMsecs			= 0;
	wxLongLong						m_renderDeltaTimeMSecs			= 0;// milliseconds since last trigger
//	uint								m_findIndex							= 0;
	int								m_imageIndex						= -1;
	bool								m_playing							= false;
//	int								m_slideshowCounter				= 0;
//	std::mutex						m_bitmapMutex;
	wxImage							m_shrunkImg;
	BattleList						m_displayedBattleList;
	int								m_displayedBattleListIndex		= -1;
};

#endif // MAINFORM_H