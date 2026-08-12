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
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/timer.h>
#include <wx/menu.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

namespace History
{
	
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
			wxTextCtrl* m_textCtrl1;
			wxStaticText* m_staticText11;
			wxTextCtrl* m_textCtrl11;
			wxStaticText* m_staticText12;
			wxTextCtrl* m_textCtrl12;
			wxStaticText* m_staticText13;
			wxTextCtrl* m_textCtrl13;
			wxStaticText* m_staticText14;
			wxTextCtrl* m_textCtrl14;
			wxStaticText* m_staticText15;
			wxTextCtrl* m_textCtrl15;
			wxStaticText* m_staticText16;
			wxTextCtrl* m_textCtrl16;
			wxStaticText* m_staticText17;
			wxTextCtrl* m_textCtrl17;
			wxStaticText* m_staticText18;
			wxTextCtrl* m_textCtrl18;
			wxPanel* m_bottomPanel;
			wxStaticText* m_staticText2;
			wxButton* m_exitButton;
			wxTimer m_guiTimer;
			wxMenuBar* m_menuBar;
			
			// Virtual event handlers, overide them in your derived class
			virtual void OnExitButtonClick( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnGuiTimer( wxTimerEvent& event ) { event.Skip(); }
			
		
		public:
			
			MainForm( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Waveblade App"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1000,700 ), long style = wxCAPTION|wxCLOSE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU|wxTAB_TRAVERSAL );
			
			~MainForm();
		
	};
	
} // namespace History

#endif //__HISTORY_H__
