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
#include <wx/slider.h>
#include <wx/scrolbar.h>
#include <wx/button.h>
#include <wx/timer.h>
#include <wx/menu.h>
#include <wx/frame.h>

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
			wxPanel* m_mainPanel;
			wxPanel* m_panel2;
			wxStaticText* m_staticText1;
			wxChoice* m_categoryChoice;
			wxPanel* m_timelinePanel;
			wxPanel* m_panel7;
			wxSlider* m_slider1;
			wxScrollBar* m_scrollBar1;
			wxPanel* m_bottomPanel;
			wxStaticText* m_staticText2;
			wxButton* m_exitButton;
			wxTimer m_guiTimer;
			wxTimer m_renderTickTimer;
			wxMenuBar* m_menuBar;
			
			// Virtual event handlers, overide them in your derived class
			virtual void OnExitButtonClick( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnGuiTimer( wxTimerEvent& event ) { event.Skip(); }
			virtual void OnRenderTickTimer( wxTimerEvent& event ) { event.Skip(); }
			
		
		public:
			
			MainForm( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("History App"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1000,700 ), long style = wxCAPTION|wxCLOSE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU|wxTAB_TRAVERSAL );
			
			~MainForm();
		
	};
	
} // namespace History

#endif //__HISTORY_H__
