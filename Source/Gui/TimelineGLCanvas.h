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
	double GetZoom() { return m_cameraMatrix.GetPositionZ(); }
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
	void OnLeftDown(wxMouseEvent& event);

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
	void CheckForSelection();
	void DrawTimelineEventDataList();
	void DrawTimelineEvent(const TimelineEventData& eventData, bool selected, float font_scale, float y1_pos, float y2_pos, uint8_t red = 255, uint8_t green = 255, uint8_t blue = 255);
	void DrawTimelineBackground(float font_scale, float events_start_y, float events_end_y);
	void MouseToOpenGLPlane(wxMouseEvent& event, wxGLCanvas* canvas, double& outX, double& outY);

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
	SVS::Matrix4d										m_cameraMatrix;
	GLint													m_viewport[4];
	GLdouble												m_modelview[16];
	GLdouble												m_projection[16];
//	DXFRenderer											m_renderer;
//	VesselData											m_vesselData;
//	MouseData											m_mouseData;
//	InputData											m_inputData;
//   DXFRenderer											m_renderer;
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
	OpenGLFont						m_openglFont;
	int								m_selectedId				= -1;
	AppData*							m_appData					= nullptr;
	//double							m_lowerDisplayDate		= 0.0;
	//double							m_upperDisplayDate		= 0.0;
	double							m_clickPosX					= 0.0;
	double							m_clickPosY					= 0.0;


	wxDECLARE_NO_COPY_CLASS(TimelineGLCanvas);
	wxDECLARE_EVENT_TABLE();
};

#endif // TIMELINEGLCANVAS_H
