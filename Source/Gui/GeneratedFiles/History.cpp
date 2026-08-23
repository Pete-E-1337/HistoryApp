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
	this->SetBackgroundColour( wxColour( 208, 208, 208 ) );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer1->AddGrowableCol( 0 );
	fgSizer1->AddGrowableRow( 1 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_mainPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_mainPanel->SetBackgroundColour( wxColour( 208, 208, 208 ) );
	
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer2->AddGrowableCol( 0 );
	fgSizer2->AddGrowableRow( 0 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_panel2 = new wxPanel( m_mainPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer3->AddGrowableCol( 0 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_panel10 = new wxPanel( m_panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer12;
	fgSizer12 = new wxFlexGridSizer( 0, 0, 0, 0 );
	fgSizer12->AddGrowableCol( 2 );
	fgSizer12->SetFlexibleDirection( wxBOTH );
	fgSizer12->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText1 = new wxStaticText( m_panel10, wxID_ANY, wxT("Category"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	fgSizer12->Add( m_staticText1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxString m_categoryChoiceChoices[] = { wxT("Ages/Eras"), wxT("Battles"), wxT("Ages") };
	int m_categoryChoiceNChoices = sizeof( m_categoryChoiceChoices ) / sizeof( wxString );
	m_categoryChoice = new wxChoice( m_panel10, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_categoryChoiceNChoices, m_categoryChoiceChoices, 0 );
	m_categoryChoice->SetSelection( 0 );
	fgSizer12->Add( m_categoryChoice, 0, wxALL, 5 );
	
	m_debugTextCtrl = new wxTextCtrl( m_panel10, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), wxTE_READONLY );
	fgSizer12->Add( m_debugTextCtrl, 0, wxALL|wxEXPAND, 5 );
	
	
	m_panel10->SetSizer( fgSizer12 );
	m_panel10->Layout();
	fgSizer12->Fit( m_panel10 );
	fgSizer3->Add( m_panel10, 1, wxEXPAND | wxALL, 5 );
	
	m_panel11 = new wxPanel( m_panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel11->SetBackgroundColour( wxColour( 255, 255, 255 ) );
	
	wxFlexGridSizer* fgSizer13;
	fgSizer13 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer13->SetFlexibleDirection( wxBOTH );
	fgSizer13->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_bitmap = new wxStaticBitmap( m_panel11, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 200,200 ), 0 );
	fgSizer13->Add( m_bitmap, 0, wxALL, 5 );
	
	
	m_panel11->SetSizer( fgSizer13 );
	m_panel11->Layout();
	fgSizer13->Fit( m_panel11 );
	fgSizer3->Add( m_panel11, 1, wxEXPAND|wxRIGHT|wxTOP, 5 );
	
	
	m_panel2->SetSizer( fgSizer3 );
	m_panel2->Layout();
	fgSizer3->Fit( m_panel2 );
	fgSizer2->Add( m_panel2, 1, wxALL|wxEXPAND, 5 );
	
	
	m_mainPanel->SetSizer( fgSizer2 );
	m_mainPanel->Layout();
	fgSizer2->Fit( m_mainPanel );
	fgSizer1->Add( m_mainPanel, 1, wxALL|wxEXPAND, 0 );
	
	m_timelineBasePanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_timelineBasePanel->SetBackgroundColour( wxColour( 208, 208, 208 ) );
	
	wxFlexGridSizer* fgSizer5;
	fgSizer5 = new wxFlexGridSizer( 1, 0, 0, 0 );
	fgSizer5->AddGrowableCol( 0 );
	fgSizer5->AddGrowableRow( 0 );
	fgSizer5->SetFlexibleDirection( wxBOTH );
	fgSizer5->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_timelinePanel = new wxPanel( m_timelineBasePanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
	m_timelinePanel->SetBackgroundColour( wxColour( 255, 255, 255 ) );
	
	wxFlexGridSizer* fgSizer10;
	fgSizer10 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer10->AddGrowableCol( 0 );
	fgSizer10->AddGrowableRow( 0 );
	fgSizer10->SetFlexibleDirection( wxBOTH );
	fgSizer10->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	
	m_timelinePanel->SetSizer( fgSizer10 );
	m_timelinePanel->Layout();
	fgSizer10->Fit( m_timelinePanel );
	fgSizer5->Add( m_timelinePanel, 1, wxEXPAND|wxLEFT|wxTOP, 5 );
	
	m_panel8 = new wxPanel( m_timelineBasePanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer11;
	fgSizer11 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer11->AddGrowableRow( 0 );
	fgSizer11->SetFlexibleDirection( wxBOTH );
	fgSizer11->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_timelineZoomScrollBar = new wxScrollBar( m_panel8, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSB_VERTICAL );
	fgSizer11->Add( m_timelineZoomScrollBar, 0, wxEXPAND|wxLEFT|wxTOP, 5 );
	
	
	m_panel8->SetSizer( fgSizer11 );
	m_panel8->Layout();
	fgSizer11->Fit( m_panel8 );
	fgSizer5->Add( m_panel8, 1, wxEXPAND|wxRIGHT, 5 );
	
	
	m_timelineBasePanel->SetSizer( fgSizer5 );
	m_timelineBasePanel->Layout();
	fgSizer5->Fit( m_timelineBasePanel );
	fgSizer1->Add( m_timelineBasePanel, 1, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 5 );
	
	m_panel6 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer61;
	fgSizer61 = new wxFlexGridSizer( 1, 0, 0, 0 );
	fgSizer61->AddGrowableCol( 0 );
	fgSizer61->AddGrowableRow( 0 );
	fgSizer61->SetFlexibleDirection( wxBOTH );
	fgSizer61->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_timelineDateScrollBar = new wxScrollBar( m_panel6, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSB_HORIZONTAL );
	fgSizer61->Add( m_timelineDateScrollBar, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxEXPAND|wxLEFT|wxTOP, 5 );
	
	m_panel9 = new wxPanel( m_panel6, wxID_ANY, wxDefaultPosition, wxSize( 22,-1 ), wxTAB_TRAVERSAL );
	fgSizer61->Add( m_panel9, 1, wxEXPAND | wxALL, 5 );
	
	
	m_panel6->SetSizer( fgSizer61 );
	m_panel6->Layout();
	fgSizer61->Fit( m_panel6 );
	fgSizer1->Add( m_panel6, 1, wxEXPAND|wxLEFT, 5 );
	
	m_panel7 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel7->SetBackgroundColour( wxColour( 208, 208, 208 ) );
	
	wxFlexGridSizer* fgSizer6;
	fgSizer6 = new wxFlexGridSizer( 1, 0, 0, 0 );
	fgSizer6->AddGrowableCol( 0 );
	fgSizer6->AddGrowableCol( 4 );
	fgSizer6->SetFlexibleDirection( wxBOTH );
	fgSizer6->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	
	fgSizer6->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText3 = new wxStaticText( m_panel7, wxID_ANY, wxT("Date"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	fgSizer6->Add( m_staticText3, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5 );
	
	m_dateTextCtrl = new wxTextCtrl( m_panel7, wxID_ANY, wxT("4540000000 BCE"), wxDefaultPosition, wxSize( 100,-1 ), wxTE_CENTRE|wxTE_PROCESS_ENTER );
	fgSizer6->Add( m_dateTextCtrl, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_dateSpinBtn = new wxSpinButton( m_panel7, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_dateSpinBtn, 0, wxEXPAND, 5 );
	
	
	fgSizer6->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	m_panel7->SetSizer( fgSizer6 );
	m_panel7->Layout();
	fgSizer6->Fit( m_panel7 );
	fgSizer1->Add( m_panel7, 1, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	m_bottomPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_bottomPanel->SetBackgroundColour( wxColour( 208, 208, 208 ) );
	
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
	fgSizer1->Add( m_bottomPanel, 1, wxALL|wxEXPAND, 0 );
	
	
	this->SetSizer( fgSizer1 );
	this->Layout();
	m_guiTimer.SetOwner( this, MAIN_GUI_TIMER );
	m_renderTickTimer.SetOwner( this, MAIN_RENDER_TIMER );
	m_menuBar = new wxMenuBar( 0 );
	this->SetMenuBar( m_menuBar );
	
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_IDLE, wxIdleEventHandler( MainForm::OnIdle ) );
	this->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( MainForm::OnMainFormKeyDown ) );
	m_bitmap->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( MainForm::OnBitmapLeftDown ), NULL, this );
	m_timelineZoomScrollBar->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( MainForm::OnTimelineZoomScrollBarScroll ), NULL, this );
	m_timelineZoomScrollBar->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( MainForm::OnTimelineZoomScrollBarScroll ), NULL, this );
	m_timelineZoomScrollBar->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( MainForm::OnTimelineZoomScrollBarScroll ), NULL, this );
	m_timelineZoomScrollBar->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( MainForm::OnTimelineZoomScrollBarScroll ), NULL, this );
	m_timelineZoomScrollBar->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( MainForm::OnTimelineZoomScrollBarScroll ), NULL, this );
	m_timelineZoomScrollBar->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( MainForm::OnTimelineZoomScrollBarScroll ), NULL, this );
	m_timelineZoomScrollBar->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( MainForm::OnTimelineZoomScrollBarScroll ), NULL, this );
	m_timelineZoomScrollBar->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( MainForm::OnTimelineZoomScrollBarScroll ), NULL, this );
	m_timelineZoomScrollBar->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( MainForm::OnTimelineZoomScrollBarScroll ), NULL, this );
	m_timelineDateScrollBar->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( MainForm::OnTimelineDateScrollBarScroll ), NULL, this );
	m_timelineDateScrollBar->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( MainForm::OnTimelineDateScrollBarScroll ), NULL, this );
	m_timelineDateScrollBar->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( MainForm::OnTimelineDateScrollBarScroll ), NULL, this );
	m_timelineDateScrollBar->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( MainForm::OnTimelineDateScrollBarScroll ), NULL, this );
	m_timelineDateScrollBar->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( MainForm::OnTimelineDateScrollBarScroll ), NULL, this );
	m_timelineDateScrollBar->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( MainForm::OnTimelineDateScrollBarScroll ), NULL, this );
	m_timelineDateScrollBar->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( MainForm::OnTimelineDateScrollBarScroll ), NULL, this );
	m_timelineDateScrollBar->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( MainForm::OnTimelineDateScrollBarScroll ), NULL, this );
	m_timelineDateScrollBar->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( MainForm::OnTimelineDateScrollBarScroll ), NULL, this );
	m_dateTextCtrl->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( MainForm::OnDateTextCtrlLeftDown ), NULL, this );
	m_dateTextCtrl->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( MainForm::OnDateTextCtrlOnText ), NULL, this );
	m_dateTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( MainForm::OnDateTextCtrlTextEnter ), NULL, this );
	m_dateSpinBtn->Connect( wxEVT_SCROLL_LINEDOWN, wxSpinEventHandler( MainForm::OnDateSpinBtnSpinDown ), NULL, this );
	m_dateSpinBtn->Connect( wxEVT_SCROLL_LINEUP, wxSpinEventHandler( MainForm::OnDateSpinBtnSpinUp ), NULL, this );
	m_exitButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainForm::OnExitButtonClick ), NULL, this );
	this->Connect( MAIN_GUI_TIMER, wxEVT_TIMER, wxTimerEventHandler( MainForm::OnGuiTimer ) );
	this->Connect( MAIN_RENDER_TIMER, wxEVT_TIMER, wxTimerEventHandler( MainForm::OnRenderTickTimer ) );
}

MainForm::~MainForm()
{
	// Disconnect Events
	this->Disconnect( wxEVT_IDLE, wxIdleEventHandler( MainForm::OnIdle ) );
	this->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( MainForm::OnMainFormKeyDown ) );
	m_bitmap->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( MainForm::OnBitmapLeftDown ), NULL, this );
	m_timelineZoomScrollBar->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( MainForm::OnTimelineZoomScrollBarScroll ), NULL, this );
	m_timelineZoomScrollBar->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( MainForm::OnTimelineZoomScrollBarScroll ), NULL, this );
	m_timelineZoomScrollBar->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( MainForm::OnTimelineZoomScrollBarScroll ), NULL, this );
	m_timelineZoomScrollBar->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( MainForm::OnTimelineZoomScrollBarScroll ), NULL, this );
	m_timelineZoomScrollBar->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( MainForm::OnTimelineZoomScrollBarScroll ), NULL, this );
	m_timelineZoomScrollBar->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( MainForm::OnTimelineZoomScrollBarScroll ), NULL, this );
	m_timelineZoomScrollBar->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( MainForm::OnTimelineZoomScrollBarScroll ), NULL, this );
	m_timelineZoomScrollBar->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( MainForm::OnTimelineZoomScrollBarScroll ), NULL, this );
	m_timelineZoomScrollBar->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( MainForm::OnTimelineZoomScrollBarScroll ), NULL, this );
	m_timelineDateScrollBar->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( MainForm::OnTimelineDateScrollBarScroll ), NULL, this );
	m_timelineDateScrollBar->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( MainForm::OnTimelineDateScrollBarScroll ), NULL, this );
	m_timelineDateScrollBar->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( MainForm::OnTimelineDateScrollBarScroll ), NULL, this );
	m_timelineDateScrollBar->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( MainForm::OnTimelineDateScrollBarScroll ), NULL, this );
	m_timelineDateScrollBar->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( MainForm::OnTimelineDateScrollBarScroll ), NULL, this );
	m_timelineDateScrollBar->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( MainForm::OnTimelineDateScrollBarScroll ), NULL, this );
	m_timelineDateScrollBar->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( MainForm::OnTimelineDateScrollBarScroll ), NULL, this );
	m_timelineDateScrollBar->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( MainForm::OnTimelineDateScrollBarScroll ), NULL, this );
	m_timelineDateScrollBar->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( MainForm::OnTimelineDateScrollBarScroll ), NULL, this );
	m_dateTextCtrl->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( MainForm::OnDateTextCtrlLeftDown ), NULL, this );
	m_dateTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( MainForm::OnDateTextCtrlOnText ), NULL, this );
	m_dateTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( MainForm::OnDateTextCtrlTextEnter ), NULL, this );
	m_dateSpinBtn->Disconnect( wxEVT_SCROLL_LINEDOWN, wxSpinEventHandler( MainForm::OnDateSpinBtnSpinDown ), NULL, this );
	m_dateSpinBtn->Disconnect( wxEVT_SCROLL_LINEUP, wxSpinEventHandler( MainForm::OnDateSpinBtnSpinUp ), NULL, this );
	m_exitButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainForm::OnExitButtonClick ), NULL, this );
	this->Disconnect( MAIN_GUI_TIMER, wxEVT_TIMER, wxTimerEventHandler( MainForm::OnGuiTimer ) );
	this->Disconnect( MAIN_RENDER_TIMER, wxEVT_TIMER, wxTimerEventHandler( MainForm::OnRenderTickTimer ) );
	
}

ImageDialog::ImageDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer14;
	fgSizer14 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer14->AddGrowableCol( 0 );
	fgSizer14->AddGrowableRow( 0 );
	fgSizer14->SetFlexibleDirection( wxBOTH );
	fgSizer14->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_topPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer15;
	fgSizer15 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer15->AddGrowableCol( 0 );
	fgSizer15->AddGrowableRow( 0 );
	fgSizer15->SetFlexibleDirection( wxBOTH );
	fgSizer15->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_imageBitmap = new wxStaticBitmap( m_topPanel, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer15->Add( m_imageBitmap, 0, wxALL|wxEXPAND, 5 );
	
	
	m_topPanel->SetSizer( fgSizer15 );
	m_topPanel->Layout();
	fgSizer15->Fit( m_topPanel );
	fgSizer14->Add( m_topPanel, 1, wxEXPAND | wxALL, 5 );
	
	m_bottomPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer16;
	fgSizer16 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer16->AddGrowableCol( 0 );
	fgSizer16->AddGrowableRow( 0 );
	fgSizer16->SetFlexibleDirection( wxBOTH );
	fgSizer16->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	
	fgSizer16->Add( 0, 0, 1, wxEXPAND, 5 );
	
	ImageSdbSizer = new wxStdDialogButtonSizer();
	ImageSdbSizerOK = new wxButton( m_bottomPanel, wxID_OK );
	ImageSdbSizer->AddButton( ImageSdbSizerOK );
	ImageSdbSizer->Realize();
	
	fgSizer16->Add( ImageSdbSizer, 1, 0, 5 );
	
	
	m_bottomPanel->SetSizer( fgSizer16 );
	m_bottomPanel->Layout();
	fgSizer16->Fit( m_bottomPanel );
	fgSizer14->Add( m_bottomPanel, 1, wxALL|wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer14 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_SIZE, wxSizeEventHandler( ImageDialog::OnImageDialogSize ) );
	ImageSdbSizerOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ImageDialog::OnImageSdbSizerOKButtonClick ), NULL, this );
}

ImageDialog::~ImageDialog()
{
	// Disconnect Events
	this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( ImageDialog::OnImageDialogSize ) );
	ImageSdbSizerOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ImageDialog::OnImageSdbSizerOKButtonClick ), NULL, this );
	
}
