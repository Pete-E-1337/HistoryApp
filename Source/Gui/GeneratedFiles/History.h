///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __HISTORY_H__
#define __HISTORY_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/choice.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/statbmp.h>
#include <wx/scrolbar.h>
#include <wx/textctrl.h>
#include <wx/valtext.h>
#include <wx/spinbutt.h>
#include <wx/slider.h>
#include <wx/splitter.h>
#include <wx/button.h>
#include <wx/timer.h>
#include <wx/menu.h>
#include <wx/frame.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

namespace History
{
	#define MAIN_GUI_TIMER 1000
	#define MAIN_RENDER_TIMER 1001
	
	///////////////////////////////////////////////////////////////////////////////
	/// Class MainForm
	///////////////////////////////////////////////////////////////////////////////
	class MainForm : public wxFrame 
	{
		private:
		
		protected:
			wxSplitterWindow* m_mainSplitter;
			wxPanel* m_topPanel;
			wxPanel* m_panel2;
			wxPanel* m_panel10;
			wxStaticText* m_staticText1;
			wxChoice* m_categoryChoice;
			wxPanel* m_bitmapPanel;
			wxStaticBitmap* m_bitmap;
			wxPanel* m_panel14;
			wxPanel* m_timelineBasePanel;
			wxPanel* m_timelinePanel;
			wxPanel* m_panel8;
			wxScrollBar* m_timelineZoomScrollBar;
			wxPanel* m_panel6;
			wxScrollBar* m_timelineDateScrollBar;
			wxPanel* m_panel9;
			wxPanel* m_panel7;
			wxStaticText* m_staticText3;
			wxTextCtrl* m_dateTextCtrl;
			wxSpinButton* m_dateSpinBtn;
			wxStaticText* m_staticText4;
			wxSlider* m_timelineZoomSlider;
			wxTextCtrl* m_zoomTextCtrl;
			wxPanel* m_bottomPanel;
			wxTextCtrl* m_debugTextCtrl;
			wxButton* m_exitButton;
			wxTimer m_guiTimer;
			wxTimer m_renderTickTimer;
			wxMenuBar* m_menuBar;
			
			// Virtual event handlers, overide them in your derived class
			virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
			virtual void OnIdle( wxIdleEvent& event ) { event.Skip(); }
			virtual void OnMainFormKeyDown( wxKeyEvent& event ) { event.Skip(); }
			virtual void OnMainSplitterSplitterSashPosChanged( wxSplitterEvent& event ) { event.Skip(); }
			virtual void OnBitmapLeftDown( wxMouseEvent& event ) { event.Skip(); }
			virtual void OnTimelineZoomScrollBarScroll( wxScrollEvent& event ) { event.Skip(); }
			virtual void OnTimelineDateScrollBarScroll( wxScrollEvent& event ) { event.Skip(); }
			virtual void OnDateTextCtrlLeftDown( wxMouseEvent& event ) { event.Skip(); }
			virtual void OnDateTextCtrlOnText( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnDateTextCtrlTextEnter( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnDateSpinBtnSpinDown( wxSpinEvent& event ) { event.Skip(); }
			virtual void OnDateSpinBtnSpinUp( wxSpinEvent& event ) { event.Skip(); }
			virtual void OnTimelineZoomSliderScroll( wxScrollEvent& event ) { event.Skip(); }
			virtual void OnExitButtonClick( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnGuiTimer( wxTimerEvent& event ) { event.Skip(); }
			virtual void OnRenderTickTimer( wxTimerEvent& event ) { event.Skip(); }
			
		
		public:
			
			MainForm( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("History App"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1000,700 ), long style = wxCAPTION|wxCLOSE_BOX|wxMAXIMIZE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU|wxTAB_TRAVERSAL );
			
			~MainForm();
		
	};
	
	///////////////////////////////////////////////////////////////////////////////
	/// Class ImageDialog
	///////////////////////////////////////////////////////////////////////////////
	class ImageDialog : public wxDialog 
	{
		private:
		
		protected:
			wxPanel* m_topPanel;
			wxStaticBitmap* m_imageBitmap;
			wxPanel* m_bottomPanel;
			wxStdDialogButtonSizer* ImageSdbSizer;
			wxButton* ImageSdbSizerOK;
			
			// Virtual event handlers, overide them in your derived class
			virtual void OnImageDialogSize( wxSizeEvent& event ) { event.Skip(); }
			virtual void OnImageBitmapSize( wxSizeEvent& event ) { event.Skip(); }
			virtual void OnImageSdbSizerOKButtonClick( wxCommandEvent& event ) { event.Skip(); }
			
		
		public:
			
			ImageDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 813,679 ), long style = wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxRESIZE_BORDER ); 
			~ImageDialog();
		
	};
	
} // namespace History

#endif //__HISTORY_H__
