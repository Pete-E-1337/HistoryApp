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
	fgSizer1->AddGrowableRow( 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_mainPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer2->AddGrowableCol( 0 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_panel2 = new wxPanel( m_mainPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 0, 6, 0, 0 );
	fgSizer3->AddGrowableCol( 1 );
	fgSizer3->AddGrowableCol( 3 );
	fgSizer3->AddGrowableCol( 5 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText1 = new wxStaticText( m_panel2, wxID_ANY, wxT("Yaw"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	fgSizer3->Add( m_staticText1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_textCtrl1 = new wxTextCtrl( m_panel2, wxID_ANY, wxT("0.0"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY|wxSUNKEN_BORDER );
	m_textCtrl1->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_MENU ) );
	
	fgSizer3->Add( m_textCtrl1, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText11 = new wxStaticText( m_panel2, wxID_ANY, wxT("Pitch"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	fgSizer3->Add( m_staticText11, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_textCtrl11 = new wxTextCtrl( m_panel2, wxID_ANY, wxT("0.0"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY|wxSUNKEN_BORDER );
	m_textCtrl11->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_MENU ) );
	
	fgSizer3->Add( m_textCtrl11, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText12 = new wxStaticText( m_panel2, wxID_ANY, wxT("Roll"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	fgSizer3->Add( m_staticText12, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_textCtrl12 = new wxTextCtrl( m_panel2, wxID_ANY, wxT("0.0"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY|wxSUNKEN_BORDER );
	m_textCtrl12->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_MENU ) );
	
	fgSizer3->Add( m_textCtrl12, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText13 = new wxStaticText( m_panel2, wxID_ANY, wxT("Position"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	fgSizer3->Add( m_staticText13, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_textCtrl13 = new wxTextCtrl( m_panel2, wxID_ANY, wxT("0.0"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY|wxSUNKEN_BORDER );
	m_textCtrl13->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_MENU ) );
	
	fgSizer3->Add( m_textCtrl13, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText14 = new wxStaticText( m_panel2, wxID_ANY, wxT("Vessel Depth"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14->Wrap( -1 );
	fgSizer3->Add( m_staticText14, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_textCtrl14 = new wxTextCtrl( m_panel2, wxID_ANY, wxT("0.0"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY|wxSUNKEN_BORDER );
	m_textCtrl14->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_MENU ) );
	
	fgSizer3->Add( m_textCtrl14, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText15 = new wxStaticText( m_panel2, wxID_ANY, wxT("Water Depth"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText15->Wrap( -1 );
	fgSizer3->Add( m_staticText15, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_textCtrl15 = new wxTextCtrl( m_panel2, wxID_ANY, wxT("0.0"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY|wxSUNKEN_BORDER );
	m_textCtrl15->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_MENU ) );
	
	fgSizer3->Add( m_textCtrl15, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText16 = new wxStaticText( m_panel2, wxID_ANY, wxT("Wave Height"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText16->Wrap( -1 );
	fgSizer3->Add( m_staticText16, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_textCtrl16 = new wxTextCtrl( m_panel2, wxID_ANY, wxT("0.0"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY|wxSUNKEN_BORDER );
	m_textCtrl16->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_MENU ) );
	
	fgSizer3->Add( m_textCtrl16, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText17 = new wxStaticText( m_panel2, wxID_ANY, wxT("Fuel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText17->Wrap( -1 );
	fgSizer3->Add( m_staticText17, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_textCtrl17 = new wxTextCtrl( m_panel2, wxID_ANY, wxT("0.0"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY|wxSUNKEN_BORDER );
	m_textCtrl17->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_MENU ) );
	
	fgSizer3->Add( m_textCtrl17, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText18 = new wxStaticText( m_panel2, wxID_ANY, wxT("Battery"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText18->Wrap( -1 );
	fgSizer3->Add( m_staticText18, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_textCtrl18 = new wxTextCtrl( m_panel2, wxID_ANY, wxT("0.0"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY|wxSUNKEN_BORDER );
	m_textCtrl18->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_MENU ) );
	
	fgSizer3->Add( m_textCtrl18, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	
	m_panel2->SetSizer( fgSizer3 );
	m_panel2->Layout();
	fgSizer3->Fit( m_panel2 );
	fgSizer2->Add( m_panel2, 1, wxEXPAND | wxALL, 5 );
	
	
	m_mainPanel->SetSizer( fgSizer2 );
	m_mainPanel->Layout();
	fgSizer2->Fit( m_mainPanel );
	fgSizer1->Add( m_mainPanel, 1, wxEXPAND | wxALL, 0 );
	
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
	m_guiTimer.SetOwner( this, wxID_ANY );
	m_menuBar = new wxMenuBar( 0 );
	this->SetMenuBar( m_menuBar );
	
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_exitButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainForm::OnExitButtonClick ), NULL, this );
	this->Connect( wxID_ANY, wxEVT_TIMER, wxTimerEventHandler( MainForm::OnGuiTimer ) );
}

MainForm::~MainForm()
{
	// Disconnect Events
	m_exitButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainForm::OnExitButtonClick ), NULL, this );
	this->Disconnect( wxID_ANY, wxEVT_TIMER, wxTimerEventHandler( MainForm::OnGuiTimer ) );
	
}
