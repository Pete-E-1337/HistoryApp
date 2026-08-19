#ifndef TIMELINEGLCANVAS_H
#define TIMELINEGLCANVAS_H

#include "wx/dcclient.h"
#include "wx/glcanvas.h"
#include <SVSLibrary/GUI/OpenGLHelper.h>
#include <SVSLibrary/Math/Matrix4.h>
#include <SVSLibrary/Math/AlphaFilter.h>
#include <SVSLibrary/Spatial/Vector3D.h>
#include <mutex>
//#include "GraphDialog.h"
//#include "DxfRenderer.h"
#include "../AppData.h"
//#include "ObjLoader.h"

// Forward declarations
//class wxJoystick;

class TimelineGLCanvas : public wxGLCanvas
{
public:
	//enum GraphType
	//{
	//	GraphType_Yaw,
	//	GraphType_Pitch,
	//	GraphType_Roll,
	//	GraphType_Throttle,
	//	GraphType_Distance,
	//	GraphType_Count
	//};

	TimelineGLCanvas(wxWindow *parent,
							 wxWindowID id = wxID_ANY,
							 const wxPoint& pos = wxDefaultPosition,
							 const wxSize& size = wxDefaultSize,
							 long style = 0,
							 const wxString& name = wxT("TimelineGLCanvas"));
   virtual ~TimelineGLCanvas();

//	void SetAppData(AppData* appData);
//	void SetInputData(const InputData& inputData);
//	void LoadDXF(const wxString& filename);
	void Render();
//	bool ProcessInputData(wxTimer* parentKeyTimer);	// Returns true if exit key is pressed
//	void SetFreezeSimulationPtr(bool* ptr) { m_simulationFrozenPtr = ptr; }
	//void AddPIDGraphData(GraphType graphType, const double& timeSecs, const double& value);
	//void ResetPIDGraphData(GraphType graphType);
//	void ResetPIDGraphData();
//	void SetCurrentGraphType(GraphType graphType) { m_currentGraphType = graphType; }
//	void ClearGraph();
//	void ShowGraph(wxTimer* parentKeyTimer);
//	bool IsGraphShowing();
//	void CloseGraphDialog();
	void SetAppData(AppData* appData) { m_appData = appData; }
	void SetDate(double date);
	void SetZoom(double percentage);
	double GetDate();
	static std::string DateToString(double date);

	const std::string& GetDebugString() { return m_debugString; }
protected:
	void OnPaint(wxPaintEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnEraseBackground(wxEraseEvent& event);
	void OnKeyDown(wxKeyEvent& event);
	void OnKeyUp(wxKeyEvent& event);
	//void OnMouse(wxMouseEvent& event);

private:
	void InitGL();
	void ResetProjectionMode();
	//void RenderVessel();
	//void RenderControlPlanes();
	//void RenderRudder(bool realModel);
	void DEBUG_DrawAxis();
	//void RenderSeaGrid();
//	void ShowGraph(GraphType graphType);
//	void OnUpdateGraphDataCallback(bool clearGraph);
	void DrawTimelineEventDataList();
	void DrawTimelineEvent(const TimelineEventData& eventData, float font_scale, float y1_pos, float y2_pos, uint8_t red = 255, uint8_t green = 255, uint8_t blue = 255);
	void DrawDrawTimelineDateScale(float font_scale, float y_spacing);

private:
	// OpenGL view data
	struct GLData
	{
		bool initialized;           // Has OpenGL been initialized?
		//float beginx, beginy;       // position of mouse
		//float quat[4];              // orientation of object
		//float zoom;                 // field of view in degrees
	};

	//struct JoystickData
	//{
	//	bool joystickLXPressed			= false;
	//	bool joystickLYPressed			= false;
	//	bool joystickTriggerPressed	= false;
	//	bool joystickRYPressed			= false;
	//};

	//struct MouseData
	//{
	//	int lastX = 0;
	//	int lastY = 0;
	//	AlphaFilter deltaXFilter{0.5, 0.0};
	//	AlphaFilter deltaYFilter{0.5, 0.0};
	//};

	//struct VesselData
	//{
	//	double xPos							= 0.0;
	//	double zPos							= 0.0;
	//	double depth						= 0.0;
	//	double yaw							= 0.0;
	//	double pitch						= 0.0;
	//	double roll							= 0.0;
	//	double leftControlPlaneAngle	= 0.0;
	//	double rightControlPlaneAngle	= 0.0;
	//	double rudderAngle				= 0.0;
	//};

//	AppData*												m_appData					= nullptr;
	wxGLContext*										m_glRC						= nullptr; // No longer needed since WXWIN_COMPATIBILITY_2_8???
	GLData												m_gldata;
//	DXFRenderer											m_renderer;
	SVS::Matrix4d										m_cameraMatrix;
//	VesselData											m_vesselData;
//	MouseData											m_mouseData;
//	InputData											m_inputData;
//   DXFRenderer											m_renderer;
//	bool*													m_simulationFrozenPtr	= nullptr;
//	bool													m_holdSimulationInPlace	= true;
//	double												m_cameraSpeedScale		= 0.5;
	std::mutex											m_renderMutex;
	std::string											m_debugString;
//	GraphDialog*										m_graphDialog				= nullptr;
	//AutonomousController::PIDTimeValueVector	m_yawGraphData;
	//AutonomousController::PIDTimeValueVector	m_pitchGraphData;
	//AutonomousController::PIDTimeValueVector	m_rollGraphData;
	//AutonomousController::PIDTimeValueVector	m_throttleGraphData;
	//AutonomousController::PIDTimeValueVector	m_distanceGraphData;
	//AutonomousController::PIDTimeValueVector	m_previousGraphData;
	//GraphType											m_currentGraphType		= GraphType::GraphType_Yaw;
	//AutonomousController::PIDTimeValueVector*	m_graphDataPtr				= nullptr;

	//objl::Loader										m_objLoader;

	//wxJoystick*											m_joyStick					= nullptr;
	//JoystickData										m_joystickData;
	AppData*					m_appData					= nullptr;
	OpenGLFont				m_openglFont;
	double					m_lowerDisplayDate		= 0.0;
	double					m_upperDisplayDate		= 0.0;


	wxDECLARE_NO_COPY_CLASS(TimelineGLCanvas);
	wxDECLARE_EVENT_TABLE();
};

#endif // TIMELINEGLCANVAS_H
