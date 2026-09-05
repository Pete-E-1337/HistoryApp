// ---------------------------------------------------------------------------
// TimelineGLCanvas
// ---------------------------------------------------------------------------

#include "TimelineGLCanvas.h"
#include "wx/wfstream.h"
//#if wxUSE_ZLIB
//#include "wx/zstream.h"
//#endif
#include <SVSLibrary/GUI/OpenGLHelper.h>
#include <SVSLibrary/Math/MiscMath.h>
#include <SVSLibrary/Spatial/Geometrics.h>
//#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#ifdef __DARWIN__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#define DRAW_DEBUG_AXIS
//#define DISPLAY_COMMON_ERA

SVS_WARNING_DISABLE(4100) // Unreferenced formal parameter
SVS_WARNING_DISABLE(4189) // local variable is initialized but not referenced

wxBEGIN_EVENT_TABLE(TimelineGLCanvas, wxGLCanvas)
EVT_SIZE(TimelineGLCanvas::OnSize)
EVT_PAINT(TimelineGLCanvas::OnPaint)
EVT_ERASE_BACKGROUND(TimelineGLCanvas::OnEraseBackground)
//EVT_MOUSE_EVENTS(TimelineGLCanvas::OnMouse)
EVT_LEFT_DOWN(TimelineGLCanvas::OnLeftDown)
EVT_KEY_DOWN(TimelineGLCanvas::OnKeyDown)
EVT_KEY_UP(TimelineGLCanvas::OnKeyUp)
//EVT_JOYSTICK_EVENTS(TimelineGLCanvas::OnJoystickEvent)
//EVT_JOY_BUTTON_DOWN(TimelineGLCanvas::OnJoystickButtonDown)
//EVT_JOY_BUTTON_UP(TimelineGLCanvas::OnJoystickButtonUp)
//EVT_JOY_MOVE(TimelineGLCanvas::OnJoystickMove)
//EVT_JOY_ZMOVE(TimelineGLCanvas::OnJoystickZMove)
wxEND_EVENT_TABLE()

//const double l_speedPerceptionScale = 2.0;
const float		l_debugAxisLength									= 1.0f;
const double	l_nearClipPlane									= 1.0;
const double	l_farClipPlane										= 10000.0;
const double	l_cameraFOV											= 45.0;
const int		l_numLines											= 15;
const int		l_font_height										= 33;
//const float		l_timeline_y_event_font_scale					= 0.003f;
const float		l_timeline_y_event_font_scale					= 0.03f / (float)l_font_height; // 0.003 when font height is 10
//const float		l_timeline_y_date_axis_font_scale			= 0.0025f;
const float		l_timeline_y_date_axis_font_scale			= 0.83f * l_timeline_y_event_font_scale; // 0.0025 when font height is 10
const double	l_datesAxisMultiplier							= 8.0;	// How many times bigger to label the dates axis
const float		l_timeline_y_selection							= 0.0083f;
const float		l_timeline_y_event_text_height				= 0.0417f;
const float		l_timeline_y_gap_event							= 0.05f;
const float		l_timeline_y_gap_date_axis						= 0.007f;
const float		l_timeline_y_date_axis_tick_height			= 0.017f;
const float		l_timeline_y_gap_date_axis_text				= 0.006f;
const float		l_timeline_y_date_axis_text_height			= 0.0245f;
const float		l_timeline_y_total_height						= (l_numLines * l_timeline_y_gap_event) + 
																				l_timeline_y_gap_date_axis + 
																				l_timeline_y_date_axis_tick_height +
																				l_timeline_y_gap_date_axis_text +
																				l_timeline_y_date_axis_text_height;

SVS::Drawing::RGB l_displayColors[] =
{
	{ 208, 240, 192 },	//Pastel Light Green
	{ 255, 197, 211 },	//Pastel Pink
	{ 255, 105,  97 },	//Pastel Red
	{ 255, 192, 103 },	//Pastel Orange
	{ 255, 223, 186 },	//Pastel Yellow
	{ 186, 255, 201 },	//Pastel Green
	{ 167, 199, 231 },	//Pastel Blue
	{ 179, 158, 181 }		//Pastel Purple
};

const int l_numDisplayColors = sizeof(l_displayColors) / sizeof(l_displayColors[0]);

const SVS::Vector3Dd l_worldTranslation = { 0.0, 0.0, 0.0 };

//int l_attributelist[5] = { WX_GL_RGBA,
//									WX_GL_BUFFER_SIZE,
//									_colordepth,
//									0,
//									0};

// Define attributes for the canvas. NB. The atrribute list consists of a list of ints and must be 0 terminated.
#ifdef __WXMSW__
	int* l_attributes = NULL ;
#else
//	int l_attributes [] = { WX_GL_RGBA, WX_GL_MIN_RED, 1, WX_GL_MIN_GREEN, 1, WX_GL_MIN_BLUE, 1, WX_GL_DEPTH_SIZE, 1, WX_GL_DOUBLEBUFFER,
//#	ifdef __WXMAC__
//									GL_NONE };
//#	else
//									None};
//#	endif
int l_attributes [] = { WX_GL_DEPTH_SIZE, 16, 0 };	// Fix for Jetson. If not present, the depth testing did not work
#endif

TimelineGLCanvas::TimelineGLCanvas(wxWindow *parent,
													wxWindowID id,
													const wxPoint& pos,
													const wxSize& size,
													long style,
													const wxString& name)
	: wxGLCanvas(parent, id, l_attributes, pos, size, style | wxFULL_REPAINT_ON_RESIZE, name)
{
//   parser_.SetDocumentCompleteCallback([&jsonSocket, session](const rapidjson::Document& document) { jsonSocket.OnDocumentComplete(document, session); });
//	m_graphDialog->SetUpdateGraphDataCallback([this](bool clearGraph){ OnUpdateGraphDataCallback(clearGraph); });

	// Explicitly create a new rendering context instance for this canvas.
	m_glRC = new wxGLContext(this);

	m_gldata.initialized = false;

	//// initialize view matrix
	//m_gldata.beginx = 0.0f;
	//m_gldata.beginy = 0.0f;
	//m_gldata.zoom = 45.0f;
	////trackball(m_gldata.quat, 0.0f, 0.0f, 0.0f, 0.0f);

//	m_cameraMatrix.SetRotation(SVS::Angle::Degrees(0.0), SVS::Angle::Degrees(-180.0), SVS::Angle::Degrees(0.0));
//	m_cameraMatrix.SetRotation(SVS::Angle::Degrees(30.0), SVS::Angle::Degrees(0.0), SVS::Angle::Degrees(0.0));
//	m_cameraMatrix.SetRotation(SVS::Vector3Dd(0.0, 0.0, 1.0), SVS::Angle::Degrees(20.0));
//	m_cameraMatrix.SetPosition(0.0, 0.0, -20.0);
//	m_cameraMatrix.SetYawPitchRoll(SVS::Angle::Degrees(0.0), SVS::Angle::Degrees(0.0), SVS::Angle::Degrees(20.0));
//	m_cameraMatrix.Rotate(SVS::Angle::Degrees(0.0), SVS::Angle::Degrees(-180.0), SVS::Angle::Degrees(0.0));
//	m_cameraMatrix.Rotate(SVS::Angle::Degrees(20.0), SVS::Angle::Degrees(0.0), SVS::Angle::Degrees(0.0));
//	m_cameraMatrix.RotateRelativeZAxis(SVS::Angle::Degrees(20.0));
	//SVS::Angle::Degrees yaw, pitch, roll;
	//m_cameraMatrix.GetYawPitchRoll(yaw, pitch, roll);
	//BREAK_HERE();

	// Opengl is a right handed coordinate system so x is to the left, y is up and z is positive looking INTO the screen.
	// To set the x axis positive to the right, we will rotate the camera 180 degrees around the Y axis and look backward
	// at our scene. Doing so will mean that the z axis is now positive looking OUT of the screen

	// Set view
	{
		m_cameraMatrix.SetRotation(SVS::Angle::Degrees(0.0), SVS::Angle::Degrees(180.0), SVS::Angle::Degrees(0.0));
		m_cameraMatrix.SetPosition(0.0, 0.0, 3.0);
	}

	//// Apply world translation to camera
	//m_cameraMatrix.Translate(l_worldTranslation.x, l_worldTranslation.y, l_worldTranslation.z);
//SVS::Vector3Dd position = m_cameraMatrix.GetPosition();
//BREAK_HERE();

//	m_selectedIter =  m_appData->eventList.end();
}

TimelineGLCanvas::~TimelineGLCanvas()
{
	if (m_glRC != nullptr)
	{
		delete m_glRC;
		m_glRC = nullptr;
	}

//#ifndef USE_MODAL_GRAPH_DIALOG
//	if (m_graphDialog != nullptr && IsGraphShowing() == true)
//	{
//		m_graphDialog->Close();
//	}
//#endif
//
//	m_graphDialog = nullptr;
}

//void TimelineGLCanvas::SetAppData(AppData* appData)
//{
//	m_appData = appData;
//
//	if (m_graphDialog != nullptr)
//	{
//		m_graphDialog->SetAppData(appData);
//	}
//}

void TimelineGLCanvas::InitGL()
{
	//// Waveblades model render setup
	//{
	//	static const GLfloat light0_pos[4] = { -50.0f, 50.0f, 0.0f, 0.0f };

	//	// white light
	//	static const GLfloat light0_color[4] = { 0.6f, 0.6f, 0.6f, 1.0f };

	//	static const GLfloat light1_pos[4] = { 50.0f, 50.0f, 0.0f, 0.0f };

	//	// cold blue light
	//	static const GLfloat light1_color[4] = { 0.4f, 0.4f, 1.0f, 1.0f };

	//	/* remove back faces */
	//	glEnable(GL_CULL_FACE);
	//	glEnable(GL_DEPTH_TEST);
	//	glClearDepth(1.0f);
	//	glDepthFunc(GL_LEQUAL);

	//	/* speedups */
	//	glEnable(GL_DITHER);
	//	glShadeModel(GL_SMOOTH);
	//	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	//	glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);

	//	/* light */
	//	glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
	//	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_color);
	//	glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
	//	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_color);
	//	glEnable(GL_LIGHT0);
	//	glEnable(GL_LIGHT1);
	//	glEnable(GL_LIGHTING);

	//	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	//	glEnable(GL_COLOR_MATERIAL);
	//}

	// getting rectangles rendering
	{
		glEnable(GL_LINE_STIPPLE);
		// DO NOT ENABLE THESE
	//	glEnable(GL_BLEND);
	//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//	glEnable(GL_DEPTH_TEST);

		glDisable(GL_LIGHTING);
		glClearColor(0, 0, 0, 1);

		//ok = m_openglTargetFont.Init("FreeSans.ttf", 10);
		//ok = m_openglFpsFont.Init("FreeSans.ttf", 10);
		//BREAK_UNLESS(ok);
	}

	bool ok = m_openglFont.Init("FreeSans.ttf", l_font_height);

	//bool ok = true;

	//ok = ok && m_opengl_font[0].Init("FreeSans.ttf", 8);
	//ok = ok && m_opengl_font[1].Init("FreeSans.ttf", 10);
	//ok = ok && m_opengl_font[2].Init("FreeSans.ttf", 11);
	//ok = ok && m_opengl_font[3].Init("FreeSans.ttf", 16);

	if (ok == false)
	{
//		wxMessageDialog(this, "Could not load ttf font", "Error", wxOK | wxICON_ERROR);
		assert(false);
	}
}

void TimelineGLCanvas::ResetProjectionMode()
{
	if (!IsShownOnScreen())
	{
		return;
	}

	// This is normally only necessary if there is more than one wxGLCanvas
	// or more than one wxGLContext in the application.
	SetCurrent(*m_glRC);

	int w, h;
	//GetClientSize(&w, &h);
	//GetParent()->GetSize(&w, &h);
	GetSize(&w, &h);
//h += 200;

	// It's up to the application code to update the OpenGL m_viewport settings.
	// In order to avoid extensive context switching, consider doing this in
	// OnPaint() rather than here, though.
	glViewport(0, 0, (GLint)w, (GLint)h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(l_cameraFOV, (GLfloat)w / h, l_nearClipPlane, l_farClipPlane);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//// Call this once during setup to map coordinates to screen pixels
	//{
	//	glMatrixMode(GL_PROJECTION);
	//	glLoadIdentity();
	//	gluOrtho2D(0.0, (double)w, 0.0, (double)h); // (Left, Right, Bottom, Top)
	//	glMatrixMode(GL_MODELVIEW);
	//}

	//glGetIntegerv(GL_VIEWPORT, m_viewport);
	//glGetDoublev(GL_MODELVIEW_MATRIX, m_modelview);
	//glGetDoublev(GL_PROJECTION_MATRIX, m_projection);
}

//void TimelineGLCanvas::OnPaint(wxPaintEvent& WXUNUSED(event))
void TimelineGLCanvas::OnPaint(wxPaintEvent& event)
{
	std::lock_guard<std::mutex> lock(m_renderMutex);

	if (IsShownOnScreen() == false)
	{
		return;
	}

	// must always be here
	wxPaintDC dc(this);

	SetCurrent(*m_glRC);

	// Initialize OpenGL
	if (!m_gldata.initialized)
	{
		InitGL();
		ResetProjectionMode();
		m_gldata.initialized = true;

		// Apply world translation to camera
		m_cameraMatrix.Translate(l_worldTranslation.x, l_worldTranslation.y, l_worldTranslation.z);
		//m_cameraMatrix.Translate(m_appData->settings->simulationStartingLocation.lon / 1000.0,
		//									0.0,
		//									m_appData->settings->simulationStartingLocation.lat / 1000.0); // working
	}

	if (m_appData->rendering == true)
	{
		Render();
	}

	event.Skip();
}

void TimelineGLCanvas::Render()
{
	if (!IsShownOnScreen())
	{
		return;
	}

	// Clear
//	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	// Black
	glClearColor(255.0f, 255.0f, 255.0f, 1.0f);	// White
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0f, 1.0f, 1.0f);

	//// Transformations
	//glLoadIdentity();
	//glTranslatef(-1.0f, -3.0f, -20.0f);
	//GLfloat m[4][4];
	//build_rotmatrix(m, m_gldata.quat);
	//glMultMatrixf(&m[0][0]);

	glLoadIdentity();

//m_cameraMatrix.RotateRelativeXAxis(SVS::Angle::Degrees(1.0));
//m_cameraMatrix.RotateRelativeYAxis(SVS::Angle::Degrees(1.0));
//m_cameraMatrix.RotateRelativeZAxis(SVS::Angle::Degrees(1.0));
//m_cameraMatrix.Rotate(SVS::Vector3Dd(0.0, 1.0, 0.0), SVS::Angle::Degrees(1.0));

	// Reposition camera
	SVS::Vector3Dd camera_position	= m_cameraMatrix.GetPosition();
	SVS::Vector3Dd camera_target		= camera_position + m_cameraMatrix.GetAt();
	SVS::Vector3Dd camera_up			= m_cameraMatrix.GetUp();

	gluLookAt(camera_position.x, camera_position.y, camera_position.z,
			  camera_target.x, camera_target.y, camera_target.z,
			  camera_up.x, camera_up.y, camera_up.z);

	//glGetIntegerv(GL_VIEWPORT, m_viewport);
	//glGetDoublev(GL_MODELVIEW_MATRIX, m_modelview);
	//glGetDoublev(GL_PROJECTION_MATRIX, m_projection);

	// Set debug string
	m_debugString = std::string("Camera Pos: ") + boost::str(boost::format("%.1lf, %.1lf, %.1lf") % camera_position.x % camera_position.y % camera_position.z).c_str();
	m_debugString += std::string(" | Mouse X, Y: ") + boost::str(boost::format("%.3lf, %.3lf") % m_clickPosX % m_clickPosY).c_str();

	// Apply world translation
	//glTranslated(l_worldTranslation.x, l_worldTranslation.y, l_worldTranslation.z);

	// Do all the rendering
//	RenderVessel();
//	RenderSeaGrid();

	//// draw a rectangle using a quad
	//{
	//	glColor3f(1.0f, 0.0f, 0.0f); // Red color

	//	// Draw a quad (rectangle)
	//	float offset = 2.0f;

	//	glBegin(GL_QUADS);
	//		 glVertex2f(-0.5f + offset, -0.5f); // Bottom-left
	//		 glVertex2f( 0.5f + offset, -0.5f); // Bottom-right
	//		 glVertex2f( 0.5f + offset,  0.5f); // Top-right
	//		 glVertex2f(-0.5f + offset,  0.5f); // Top-left
	//	glEnd();
	//}

	//{
	//	glColor4ub(255, 255, 255, 255);
	//	glRectf(-0.5f, -0.5f, 0.5f, 0.5f);
	//}

	//{
	//	OpenGLHelper::DrawSolidRectangle(0.5f, 0.5f, 1.5f, 1.5f, 255, 200, 200);
	//}

	//{
	//	float fontScale = 1.0f;

	//	glColor3ub(127, 127, 127);		// White
	//	OpenGLHelper::DrawText(0, 0, 0, m_openglFont, 0.01f, "This is some text");
	////	OpenGLHelper::DrawText(x-4, y, 0, m_openglMapFont, 1.0f, "N");
	//}

	DrawTimelineEventDataList();

#ifdef DRAW_DEBUG_AXIS
	DEBUG_DrawAxis();
#endif

	// Flush
//	glFlush();

	// Swap
	SwapBuffers();
}

void TimelineGLCanvas::CheckForSelection()
{
	float		y_spacing			= l_timeline_y_gap_event * m_cameraMatrix.GetPositionZ();
	float		events_start_y		= (l_timeline_y_total_height / 2.0) * m_cameraMatrix.GetPositionZ();
	float		text_height			= l_timeline_y_event_text_height * m_cameraMatrix.GetPositionZ();
	float		y1_pos				= events_start_y;
	float		y2_pos				= y1_pos - text_height;

	int line_number				= 0;
	m_selectedId					= -1;

	for (TimeLineEventListIter iter = m_appData->eventList.begin(); iter != m_appData->eventList.end(); iter++)
	{
		if (SVS::Math::InRange(m_clickPosX, iter->startDate, iter->endDate) &&
			 SVS::Math::InRange(m_clickPosY, (double)y2_pos, (double)y1_pos))
		{
			m_selectedId = iter->id;
			break;
		}

		line_number++;

		if (line_number == l_numLines)
		{
			line_number = 0;
			y1_pos		= events_start_y;
			y2_pos		= y1_pos - text_height;
		}
		else
		{
			y1_pos -= y_spacing;
			y2_pos -= y_spacing;
		}
	}
}

void TimelineGLCanvas::DrawTimelineEventDataList()
{
	SVS::Drawing::RGB color;
	int colorIndex = 0;

	float		y_spacing			= l_timeline_y_gap_event * m_cameraMatrix.GetPositionZ();
	float		events_start_y		= (l_timeline_y_total_height / 2.0) * m_cameraMatrix.GetPositionZ();
	float		events_end_y		= events_start_y - (((double)l_numLines) * y_spacing);
	float		font_scale			= l_timeline_y_event_font_scale * m_cameraMatrix.GetPositionZ();
	float		date_font_scale	= l_timeline_y_date_axis_font_scale * m_cameraMatrix.GetPositionZ();
	float		text_height			= l_timeline_y_event_text_height * m_cameraMatrix.GetPositionZ();
	float		y1_pos				= events_start_y;
	float		y2_pos				= y1_pos - text_height;

	DrawTimelineBackground(date_font_scale, events_start_y, events_end_y);

	int line_number	= 0;
	bool selected		= false;

	for (TimeLineEventListIter iter = m_appData->eventList.begin(); iter != m_appData->eventList.end(); iter++)
	{
		color = l_displayColors[colorIndex++];

		//selected = SVS::Math::InRange(m_clickPosX, iter->startDate, iter->endDate) &&
		//			  SVS::Math::InRange(m_clickPosY, (double)y2_pos, (double)y1_pos);
		selected = (iter->id == m_selectedId);

		DrawTimelineEvent(*iter, selected, font_scale, y1_pos, y2_pos, color.r, color.g, color.b);

		if (colorIndex == l_numDisplayColors)
		{
			colorIndex = 0;
		}

		line_number++;

		if (line_number == l_numLines)
		{
			line_number = 0;
			y1_pos		= events_start_y;
			y2_pos		= y1_pos - text_height;
		}
		else
		{
			y1_pos -= y_spacing;
			y2_pos -= y_spacing;
		}
	}
}

void TimelineGLCanvas::DrawTimelineBackground(float font_scale, float events_start_y, float events_end_y)
{
	float		date_axis_y	= events_end_y - (l_timeline_y_gap_date_axis * m_cameraMatrix.GetPositionZ());
	double	x1				= m_cameraMatrix.GetPositionX() - m_cameraMatrix.GetPositionZ() * l_datesAxisMultiplier;
	double	x2				= m_cameraMatrix.GetPositionX() + m_cameraMatrix.GetPositionZ() * l_datesAxisMultiplier;

	//// debug draw timeline start and end lines
	//{
	//	float yy = events_start_y;
	//	OpenGLHelper::DrawLine(x1, yy, x2, yy, 255, 0, 0);
	//	yy = yy - (l_timeline_y_total_height * m_cameraMatrix.GetPositionZ());
	//	OpenGLHelper::DrawLine(x1, yy, x2, yy, 255, 0, 0);
	//}

	// Horizontal date axis
	{
		OpenGLHelper::DrawLine(x1, date_axis_y, x2, date_axis_y, 0, 0, 0);
	}

	// Dates
	{
		float	x_spacing	= 1.0;
		float	tick_height	= l_timeline_y_date_axis_tick_height * m_cameraMatrix.GetPositionZ();
		float	z				= m_cameraMatrix.GetPositionZ();

		if (z <= 5.0)
			x_spacing = 1.0;
		else if (z <= 10.0)
			x_spacing = 2.0;
		else if (z <= 20.0)
			x_spacing = 5.0;
		else if (z <= 50.0)
			x_spacing = 10.0;
		else if (z <= 100.0)
			x_spacing = 20.0;
		else if (z <= 200.0)
			x_spacing = 50.0;
		else if (z <= 500.0)
			x_spacing = 100.0;
		else if (z <= 1000.0)
			x_spacing = 200.0;
		else if (z <= 2000.0)
			x_spacing = 500.0;
		else if (z <= 5000.0)
			x_spacing = 1000.0;
		else if (z <= 10000.0)
			x_spacing = 2000.0;
		else if (z <= 20000.0)
			x_spacing = 5000.0;

		double	x			= x1 - std::fmod(x1, x_spacing);
		float		tick_y1	= date_axis_y;
		float		tick_y2	= date_axis_y - tick_height;
		float		text_y	= tick_y2 - (l_timeline_y_gap_date_axis_text * m_cameraMatrix.GetPositionZ());
		std::string date_str;

		while (x < x2)
		{
			// Vertical lines
			OpenGLHelper::DrawLine(x, tick_y1, x, events_start_y, 210, 210, 210, 1.0f, 1, 0x0F0F);	// Grey, dashed

			// Ticks
			OpenGLHelper::DrawLine(x, tick_y1, x, tick_y2, 0, 0, 0);											// Black

#ifdef DISPLAY_COMMON_ERA
			date_str = DateToString(x);
#else
			date_str = std::to_string((int64_t)x);
#endif

	      //glColor3ub(0, 0, 0);

			// Dates
			OpenGLHelper::DrawText(x, (double)text_y, 0.0, m_openglFont, font_scale, date_str);

			x += x_spacing;
		}

		// Centre date

		OpenGLHelper::DrawLine(m_cameraMatrix.GetPositionX(), tick_y1, m_cameraMatrix.GetPositionX(), events_start_y, 255, 0, 0, 1.0f, 1, 0x0F0F);	// Red, dashed
	}
}

void TimelineGLCanvas::DrawTimelineEvent(const TimelineEventData& eventData, bool selected, float font_scale, float y1_pos, float y2_pos, uint8_t red, uint8_t green, uint8_t blue)
{
	if (selected == true)
	{
		float surround_spacing = l_timeline_y_selection * m_cameraMatrix.GetPositionZ();
		OpenGLHelper::DrawSolidRectangle(eventData.startDate - surround_spacing, y1_pos + surround_spacing, eventData.endDate + surround_spacing, y2_pos - surround_spacing, 0x00, 0xA7, 0xC3); // selection blue
	}

	// Draw event period bar

	OpenGLHelper::DrawSolidRectangle(eventData.startDate, y1_pos, eventData.endDate, y2_pos, red, green, blue);

	// Draw event name

//	if (m_cameraMatrix.GetPositionZ() <= 75.0)
	{
		glColor3ub(0, 0, 0);		// Black
		OpenGLHelper::DrawText(eventData.startDate, y1_pos, 0.0f, m_openglFont, font_scale, eventData.name);
	}
}

std::string TimelineGLCanvas::DateToString(double date)
{
	std::string str;

	if (date < 0.0)
	{
		date *= -1.0;
		str = std::to_string((int64_t)date) + " BCE";	// BC
	}
	else
	{
		str = std::to_string((int64_t)date) + " CE";	// AD
	}

	return str;
}

void TimelineGLCanvas::DEBUG_DrawAxis()
{
//	glDisable(GL_LIGHTING);

	glLineWidth(1.0);

	// x, red
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(l_debugAxisLength, 0.0f, 0.0f);
	glEnd();

	// y, green
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, l_debugAxisLength, 0.0f);
	glEnd();

	// z, blue
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, l_debugAxisLength);
	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);

//	glEnable(GL_LIGHTING);
}

//void TimelineGLCanvas::OnSize(wxSizeEvent& WXUNUSED(event))
void TimelineGLCanvas::OnSize(wxSizeEvent& event)
{
	// Reset the OpenGL view aspect.
	// This is OK only because there is only one canvas that uses the context.
	// See the cube sample for that case that multiple canvases are made current with one context.
	ResetProjectionMode();

	event.Skip();
}

void TimelineGLCanvas::OnEraseBackground(wxEraseEvent& WXUNUSED(event))
{
	// Do nothing, to avoid flashing on MSW
}

void TimelineGLCanvas::SetDate(double date)
{
	m_cameraMatrix.SetPositionX(date);
}

double TimelineGLCanvas::GetDate()
{
	return m_cameraMatrix.GetPositionX();
}

void TimelineGLCanvas::SetZoom(double percentage)
{
	double z = SVS::MiscMath::LinearInterpolate(l_nearClipPlane, l_farClipPlane, percentage);
	m_cameraMatrix.SetPositionZ(z);
}

void TimelineGLCanvas::OnKeyDown(wxKeyEvent& event)
{
	// https://theasciicode.com.ar/

	float step = (event.ShiftDown() == true) ? 10.0f : 1.0f;

	int keyCode = event.GetKeyCode();

	switch (keyCode)
	{
		case 'A':
			SetDate(GetDate() - step);
//			m_dateTextCtrl->SetValue(boost::str(boost::format("%.2lfº") % wbd.pitch.AsDouble()).c_str());
			break;
		case 'D':
			SetDate(GetDate() + step);
			break;
		case 'W':
			m_cameraMatrix.SetPositionZ((std::max)(l_nearClipPlane, m_cameraMatrix.GetPositionZ() - step));
			break;
		case 'S':
			m_cameraMatrix.SetPositionZ((std::min)(l_farClipPlane, m_cameraMatrix.GetPositionZ() + step));
			break;
	//	case 306:	m_cameraSpeedScale = 2.0;														break;	// Shift key
	//	case WXK_ESCAPE:
	//	case 'V':	m_inputData.keyData.exitKeyDown = true;									break;
		default: break;
	}

	event.Skip();
}

void TimelineGLCanvas::OnKeyUp(wxKeyEvent& event)
{
	int keyCode = event.GetKeyCode();

	//switch (keyCode)
	//{
	//	case 'A':	m_inputData.keyData.leftKeyDown = false;			break;
	//	case 'D':	m_inputData.keyData.rightKeyDown = false;			break;
	//	case 'W':	m_inputData.keyData.forwardKeyDown = false;		break;
	//	case 'S':	m_inputData.keyData.backKeyDown = false;			break;
	//	case 306:	m_cameraSpeedScale = 0.5;							break;	// Shift key
	//	case 'I':	m_inputData.keyData.noseDownKeyDown = false;		break;
	//	case 'K':	m_inputData.keyData.noseUpKeyDown = false;		break;
	//	case 'J':	m_inputData.keyData.turnLeftKeyDown = false;		break;
	//	case 'L':	m_inputData.keyData.turnRightKeyDown = false;	break;
	//	case 'U':	m_inputData.keyData.rollLeftKeyDown = false;		break;
	//	case 'O':	m_inputData.keyData.rollRightKeyDown = false;	break;
	//	case 'P':	m_inputData.keyData.speedUpKeyDown = false;		break;
	//	case ';':	m_inputData.keyData.speedDownKeyDown = false;	break;
	//	default: break;
	//}

	event.Skip();
}

//bool TimelineGLCanvas::IsGraphShowing()
//{
//	return (m_graphDialog->IsShownOnScreen() == true);
//}

//void TimelineGLCanvas::OnMouse(wxMouseEvent& event)
//{
//#ifdef MAKE_ACTIVE_WHEN_MOUSE_MOVES_OVER_CANVAS
//	if (HasFocus() == false)
//	{
//		SetFocus();
//	}
//#endif
//
//	if (event.LeftDown() || event.RightDown())
//	{
//		m_mouseData.lastX = event.GetX();
//		m_mouseData.lastY = event.GetY();
//	}
//	else if (event.Dragging())
//	{
//		const SVS::Vector3Dd worldYAxis(0.0, 1.0, 0.0);
//		const double rotationScale = 2.0;
//
//		int deltaX = (m_mouseData.lastX - event.GetX()) / rotationScale;
//		int deltaY = (m_mouseData.lastY - event.GetY()) / rotationScale;
//
//		double deltaXFiltered = m_mouseData.deltaXFilter.Filter(deltaX);
//		double deltaYFiltered = m_mouseData.deltaYFilter.Filter(deltaY);
//
//		m_cameraMatrix.Rotate(worldYAxis, SVS::Angle::Degrees(deltaXFiltered));
//		m_cameraMatrix.RotateRelativeXAxis(SVS::Angle::Degrees(-deltaYFiltered));
//
//		m_mouseData.lastX = event.GetX();
//		m_mouseData.lastY = event.GetY();
//	}
//
//	event.Skip();
//}

void TimelineGLCanvas::OnLeftDown(wxMouseEvent& event)
{
	MouseToOpenGLPlane(event, this, m_clickPosX, m_clickPosY);
	CheckForSelection();

	event.Skip();
}

void TimelineGLCanvas::MouseToOpenGLPlane(wxMouseEvent& event, wxGLCanvas* canvas, double& outX, double& outY)
{
	// 1. Get viewport and matrix data
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];

	// Only want to call these occasionally as they stall the render pipeline
	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

//SVS::Matrix4d m1, m_cameraMatrixTransposed;
//m_cameraMatrixTransposed = m_cameraMatrix;
////m_cameraMatrixTransposed.Rotate(SVS::Vector3Dd(0.0, 1.0, 0.0), SVS::Angle::Degrees(180.0));
////m_cameraMatrixTransposed.SetRight(-m_cameraMatrixTransposed.GetRight());
////m_cameraMatrixTransposed.RotateRelativeYAxis(SVS::Angle::Degrees(180.0));
////m_cameraMatrixTransposed.SetPositionZ(-m_cameraMatrixTransposed.GetPositionZ());
//m_cameraMatrixTransposed = m_cameraMatrixTransposed.Transpose();
//m_cameraMatrixTransposed.Inverse(m1);
//m_cameraMatrixTransposed = m1;

//// correct values but signs wrong
//SVS::Matrix4d m_cameraMatrixTransposed;
//m_cameraMatrix.Inverse(m_cameraMatrixTransposed);
//m_cameraMatrixTransposed = m_cameraMatrixTransposed.Transpose();
//m_cameraMatrixTransposed.SetRightX(-m_cameraMatrixTransposed.GetRightX());	// 0
//m_cameraMatrixTransposed.SetAtX(-m_cameraMatrixTransposed.GetAtX());	// 2
//m_cameraMatrixTransposed.SetPositionY(-m_cameraMatrixTransposed.GetPositionY());	// 7, 0 goes to -0 so probably not necessary
//m_cameraMatrixTransposed.SetRightZ(-m_cameraMatrixTransposed.GetRightZ());	// 8
//m_cameraMatrixTransposed.SetAtZ(-m_cameraMatrixTransposed.GetAtZ());	// 10
//m_cameraMatrixTransposed.Set12(-m_cameraMatrixTransposed.Get12());	// 12
//m_cameraMatrixTransposed.Set14(-m_cameraMatrixTransposed.Get14());	// 14


//// Transposes a 4x4 matrix in-place
//void transpose4x4(float mat[4][4]) {
//    for (int i = 0; i < 4; ++i) {
//        for (int j = i + 1; j < 4; ++j) {
//            std::swap(mat[i][j], mat[j][i]);
//        }
//    }
//}
//gluInvertMatrix


   //inline T GetRightX()    { return m_matrix[0];   }
   //inline T GetRightY()    { return m_matrix[4];   }
   //inline T GetRightZ()    { return m_matrix[8];   }
   //inline T GetUpX()       { return m_matrix[1];   }
   //inline T GetUpY()       { return m_matrix[5];   }
   //inline T GetUpZ()       { return m_matrix[9];   }
   //inline T GetAtX()       { return m_matrix[2];   }
   //inline T GetAtY()       { return m_matrix[6];   }
   //inline T GetAtZ()       { return m_matrix[10];  }
   //inline T GetPositionX() { return m_matrix[3];   }
   //inline T GetPositionY() { return m_matrix[7];   }
   //inline T GetPositionZ() { return m_matrix[11];  }

//SVS::Matrix4d m1, m_cameraMatrixTransposed;
//m1 = m_cameraMatrix.Transpose();
//m1.Inverse(m_cameraMatrixTransposed);

	// 2. Correct the Y-coordinate (wxWidgets top-left is 0, OpenGL bottom-left is 0)
	double mouseX = event.GetX();
	double mouseY = viewport[3] - event.GetY(); 
//	double mouseY = viewport[3] - event.GetY() - 1; 

	// 3. Unproject onto the near plane (Z = 0.0)
	double nearX, nearY, nearZ;
	gluUnProject(mouseX, mouseY, 0.0, modelview, projection, viewport, &nearX, &nearY, &nearZ);

	// 4. Unproject onto the far plane (Z = 1.0)
	double farX, farY, farZ;
	gluUnProject(mouseX, mouseY, 1.0, modelview, projection, viewport, &farX, &farY, &farZ);

	// 5. Calculate intersection with the Z = 0 plane using linear interpolation
	double t = -nearZ / (farZ - nearZ);
	outX = nearX + t * (farX - nearX);
	outY = nearY + t * (farY - nearY);
}

/*
// code to replicate glulookat

#include <cmath>
#include <array>

struct Vec3 { float x, y, z; };

Vec3 normalize(Vec3 v) {
    float len = std::sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
    return { v.x / len, v.y / len, v.z / len };
}

Vec3 cross(Vec3 a, Vec3 b) {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

float dot(Vec3 a, Vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

std::array<float, 16> replicate_gluLookAt(Vec3 eye, Vec3 center, Vec3 up) {
    // 1. Forward Vector
    Vec3 f = normalize({ center.x - eye.x, center.y - eye.y, center.z - eye.z });
    
    // 2. Side (Right) Vector
    Vec3 s = normalize(cross(f, up));
    
    // 3. True Up Vector
    Vec3 u = cross(s, f);

    // 4. Combined Rotation and Translation Matrix (PP. row-major array. probably want to remap to column major)
    std::array<float, 16> viewMatrix = {{
         s.x,  s.y,  s.z, -dot(s, eye),
         u.x,  u.y,  u.z, -dot(u, eye),
        -f.x, -f.y, -f.z,  dot(f, eye),
         0.0f, 0.0f, 0.0f, 1.0f
    }};

    return viewMatrix;
}
*/