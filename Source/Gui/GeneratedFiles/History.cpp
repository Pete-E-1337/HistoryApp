///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "History.h"

///////////////////////////////////////////////////////////////////////////
using namespace History;

MainForm::MainForm( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer1->AddGrowableCol( 0 );
	fgSizer1->AddGrowableRow( 1 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_mainPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_panel2 = new wxPanel( m_mainPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 0, 6, 0, 0 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText1 = new wxStaticText( m_panel2, wxID_ANY, wxT("Category"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	fgSizer3->Add( m_staticText1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxString m_categoryChoiceChoices[] = { wxT("Ages/Eras"), wxT("Battles"), wxT("Ages") };
	int m_categoryChoiceNChoices = sizeof( m_categoryChoiceChoices ) / sizeof( wxString );
	m_categoryChoice = new wxChoice( m_panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_categoryChoiceNChoices, m_categoryChoiceChoices, 0 );
	m_categoryChoice->SetSelection( 0 );
	fgSizer3->Add( m_categoryChoice, 0, wxALL, 5 );
	
	
	m_panel2->SetSizer( fgSizer3 );
	m_panel2->Layout();
	fgSizer3->Fit( m_panel2 );
	fgSizer2->Add( m_panel2, 1, wxALL, 5 );
	
	
	m_mainPanel->SetSizer( fgSizer2 );
	m_mainPanel->Layout();
	fgSizer2->Fit( m_mainPanel );
	fgSizer1->Add( m_mainPanel, 1, wxALL|wxEXPAND, 0 );
	
	m_timelinePanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
	m_timelinePanel->SetBackgroundColour( wxColour( 255, 255, 255 ) );
	
	wxFlexGridSizer* fgSizer5;
	fgSizer5 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer5->AddGrowableCol( 0 );
	fgSizer5->AddGrowableRow( 0 );
	fgSizer5->SetFlexibleDirection( wxBOTH );
	fgSizer5->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	
	m_timelinePanel->SetSizer( fgSizer5 );
	m_timelinePanel->Layout();
	fgSizer5->Fit( m_timelinePanel );
	fgSizer1->Add( m_timelinePanel, 1, wxEXPAND | wxALL, 5 );
	
	m_panel7 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer6;
	fgSizer6 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer6->SetFlexibleDirection( wxBOTH );
	fgSizer6->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_slider1 = new wxSlider( m_panel7, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	fgSizer6->Add( m_slider1, 0, wxALL, 5 );
	
	m_scrollBar1 = new wxScrollBar( m_panel7, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSB_HORIZONTAL );
	fgSizer6->Add( m_scrollBar1, 0, wxALL, 5 );
	
	
	m_panel7->SetSizer( fgSizer6 );
	m_panel7->Layout();
	fgSizer6->Fit( m_panel7 );
	fgSizer1->Add( m_panel7, 1, wxEXPAND | wxALL, 5 );
	
	m_bottomPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer4;
	fgSizer4 = new wxFlexGridSizer( 1, 0, 0, 0 );
	fgSizer4->AddGrowableCol( 0 );
	fgSizer4->AddGrowableRow( 0 );
	fgSizer4->SetFlexibleDirection( wxBOTH );
	fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText2 = new wxStaticText( m_bottomPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	fgSizer4->Add( m_staticText2, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
	fgSizer4->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_exitButton = new wxButton( m_bottomPanel, wxID_ANY, wxT("Exit"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( m_exitButton, 0, wxALL, 5 );
	
	
	m_bottomPanel->SetSizer( fgSizer4 );
	m_bottomPanel->Layout();
	fgSizer4->Fit( m_bottomPanel );
	fgSizer1->Add( m_bottomPanel, 1, wxEXPAND | wxALL, 0 );
	
	
	this->SetSizer( fgSizer1 );
	this->Layout();
	m_guiTimer.SetOwner( this, MAIN_GUI_TIMER );
	m_renderTickTimer.SetOwner( this, MAIN_RENDER_TIMER );
	m_menuBar = new wxMenuBar( 0 );
	this->SetMenuBar( m_menuBar );
	
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_exitButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainForm::OnExitButtonClick ), NULL, this );
	this->Connect( MAIN_GUI_TIMER, wxEVT_TIMER, wxTimerEventHandler( MainForm::OnGuiTimer ) );
	this->Connect( MAIN_RENDER_TIMER, wxEVT_TIMER, wxTimerEventHandler( MainForm::OnRenderTickTimer ) );
}

MainForm::~MainForm()
{
	// Disconnect Events
	m_exitButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainForm::OnExitButtonClick ), NULL, this );
	this->Disconnect( MAIN_GUI_TIMER, wxEVT_TIMER, wxTimerEventHandler( MainForm::OnGuiTimer ) );
	this->Disconnect( MAIN_RENDER_TIMER, wxEVT_TIMER, wxTimerEventHandler( MainForm::OnRenderTickTimer ) );
	
}
