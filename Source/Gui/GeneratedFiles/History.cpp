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
	fgSizer1->AddGrowableRow( 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_mainSplitter = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D|wxSP_NO_XP_THEME );
	m_mainSplitter->SetSashGravity( 0 );
	
	m_mainSplitter->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	m_mainSplitter->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	
	m_topPanel = new wxPanel( m_mainSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_topPanel->SetBackgroundColour( wxColour( 208, 208, 208 ) );
	
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer2->AddGrowableCol( 0 );
	fgSizer2->AddGrowableRow( 0 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_splitter2 = new wxSplitterWindow( m_topPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_splitter2->Connect( wxEVT_IDLE, wxIdleEventHandler( MainForm::m_splitter2OnIdle ), NULL, this );
	
	m_panel2 = new wxPanel( m_splitter2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer3->AddGrowableCol( 1 );
	fgSizer3->AddGrowableRow( 0 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_panel16 = new wxPanel( m_panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel16->SetBackgroundColour( wxColour( 208, 208, 208 ) );
	
	wxFlexGridSizer* fgSizer17;
	fgSizer17 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer17->SetFlexibleDirection( wxBOTH );
	fgSizer17->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_panel10 = new wxPanel( m_panel16, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer12;
	fgSizer12 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer12->SetFlexibleDirection( wxBOTH );
	fgSizer12->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText1 = new wxStaticText( m_panel10, wxID_ANY, wxT("Category"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	fgSizer12->Add( m_staticText1, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxLEFT|wxRIGHT|wxTOP, 5 );
	
	wxString m_categoryChoiceChoices[] = { wxT("Ages/Eras"), wxT("Battles"), wxT("Ages") };
	int m_categoryChoiceNChoices = sizeof( m_categoryChoiceChoices ) / sizeof( wxString );
	m_categoryChoice = new wxChoice( m_panel10, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_categoryChoiceNChoices, m_categoryChoiceChoices, 0 );
	m_categoryChoice->SetSelection( 0 );
	fgSizer12->Add( m_categoryChoice, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );
	
	m_findButton = new wxButton( m_panel10, wxID_ANY, wxT("Find"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer12->Add( m_findButton, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 5 );
	
	m_findTextCtrl = new wxTextCtrl( m_panel10, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizer12->Add( m_findTextCtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_googleSearchButton = new wxButton( m_panel10, wxID_ANY, wxT("Google Search"), wxDefaultPosition, wxDefaultSize, 0 );
	m_googleSearchButton->Enable( false );
	
	fgSizer12->Add( m_googleSearchButton, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 5 );
	
	m_wikipediaSearchButton = new wxButton( m_panel10, wxID_ANY, wxT("Wikipedia Search"), wxDefaultPosition, wxDefaultSize, 0 );
	m_wikipediaSearchButton->Enable( false );
	
	fgSizer12->Add( m_wikipediaSearchButton, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxTOP, 5 );
	
	m_mapSearchButton = new wxButton( m_panel10, wxID_ANY, wxT("Google Map"), wxDefaultPosition, wxDefaultSize, 0 );
	m_mapSearchButton->Enable( false );
	
	fgSizer12->Add( m_mapSearchButton, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 5 );
	
	
	m_panel10->SetSizer( fgSizer12 );
	m_panel10->Layout();
	fgSizer12->Fit( m_panel10 );
	fgSizer17->Add( m_panel10, 1, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 5 );
	
	m_panel7 = new wxPanel( m_panel16, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel7->SetBackgroundColour( wxColour( 208, 208, 208 ) );
	
	wxFlexGridSizer* fgSizer6;
	fgSizer6 = new wxFlexGridSizer( 1, 0, 0, 0 );
	fgSizer6->SetFlexibleDirection( wxBOTH );
	fgSizer6->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText3 = new wxStaticText( m_panel7, wxID_ANY, wxT("Date"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	fgSizer6->Add( m_staticText3, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5 );
	
	m_dateLeftStaticText = new wxStaticText( m_panel7, wxID_ANY, wxT("◄"), wxDefaultPosition, wxDefaultSize, 0 );
	m_dateLeftStaticText->Wrap( -1 );
	m_dateLeftStaticText->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	m_dateLeftStaticText->Hide();
	
	fgSizer6->Add( m_dateLeftStaticText, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5 );
	
	m_dateLeftButton = new wxButton( m_panel7, wxID_ANY, wxT("◄"), wxDefaultPosition, wxSize( 12,-1 ), 0|wxNO_BORDER );
	m_dateLeftButton->SetFont( wxFont( 10, 70, 90, 90, false, wxEmptyString ) );
	m_dateLeftButton->SetBackgroundColour( wxColour( 208, 208, 208 ) );
	
	fgSizer6->Add( m_dateLeftButton, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5 );
	
	m_dateTextCtrl = new wxTextCtrl( m_panel7, wxID_ANY, wxT("4540000000 BCE"), wxDefaultPosition, wxSize( 100,-1 ), wxTE_CENTRE|wxTE_PROCESS_ENTER );
	fgSizer6->Add( m_dateTextCtrl, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_dateRightButton = new wxButton( m_panel7, wxID_ANY, wxT("►"), wxDefaultPosition, wxSize( 12,-1 ), 0|wxNO_BORDER );
	m_dateRightButton->SetFont( wxFont( 10, 70, 90, 90, false, wxEmptyString ) );
	m_dateRightButton->SetBackgroundColour( wxColour( 208, 208, 208 ) );
	
	fgSizer6->Add( m_dateRightButton, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5 );
	
	m_dateRightStaticText = new wxStaticText( m_panel7, wxID_ANY, wxT("►"), wxDefaultPosition, wxDefaultSize, 0 );
	m_dateRightStaticText->Wrap( -1 );
	m_dateRightStaticText->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	m_dateRightStaticText->Hide();
	
	fgSizer6->Add( m_dateRightStaticText, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5 );
	
	
	m_panel7->SetSizer( fgSizer6 );
	m_panel7->Layout();
	fgSizer6->Fit( m_panel7 );
	fgSizer17->Add( m_panel7, 1, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 5 );
	
	m_panel17 = new wxPanel( m_panel16, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer18;
	fgSizer18 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer18->AddGrowableCol( 1 );
	fgSizer18->SetFlexibleDirection( wxBOTH );
	fgSizer18->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText4 = new wxStaticText( m_panel17, wxID_ANY, wxT("Range"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_staticText4->Wrap( -1 );
	fgSizer18->Add( m_staticText4, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5 );
	
	m_timelineZoomSlider = new wxSlider( m_panel17, wxID_ANY, 0, 0, 10000, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_TOP );
	fgSizer18->Add( m_timelineZoomSlider, 0, wxEXPAND, 5 );
	
	m_zoomTextCtrl = new wxTextCtrl( m_panel17, wxID_ANY, wxT("60000 years"), wxDefaultPosition, wxSize( 70,-1 ), wxTE_CENTRE|wxTE_READONLY );
	fgSizer18->Add( m_zoomTextCtrl, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5 );
	
	
	m_panel17->SetSizer( fgSizer18 );
	m_panel17->Layout();
	fgSizer18->Fit( m_panel17 );
	fgSizer17->Add( m_panel17, 1, wxBOTTOM|wxEXPAND|wxLEFT|wxTOP, 5 );
	
	m_panel15 = new wxPanel( m_panel16, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer16;
	fgSizer16 = new wxFlexGridSizer( 1, 0, 0, 0 );
	fgSizer16->SetFlexibleDirection( wxBOTH );
	fgSizer16->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_prevButton = new wxButton( m_panel15, wxID_ANY, wxT("│◄"), wxDefaultPosition, wxSize( 18,-1 ), 0|wxNO_BORDER );
	m_prevButton->SetFont( wxFont( 10, 70, 90, 90, false, wxEmptyString ) );
	m_prevButton->SetBackgroundColour( wxColour( 208, 208, 208 ) );
	
	fgSizer16->Add( m_prevButton, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5 );
	
	m_pauseButton = new wxButton( m_panel15, wxID_ANY, wxT("ll"), wxDefaultPosition, wxSize( 18,-1 ), 0|wxNO_BORDER );
	m_pauseButton->SetFont( wxFont( 10, 70, 90, 90, false, wxEmptyString ) );
	m_pauseButton->SetBackgroundColour( wxColour( 208, 208, 208 ) );
	
	fgSizer16->Add( m_pauseButton, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5 );
	
	m_playButton = new wxButton( m_panel15, wxID_ANY, wxT("►"), wxDefaultPosition, wxSize( 18,-1 ), 0|wxNO_BORDER );
	m_playButton->SetFont( wxFont( 10, 70, 90, 90, false, wxEmptyString ) );
	m_playButton->SetBackgroundColour( wxColour( 208, 208, 208 ) );
	
	fgSizer16->Add( m_playButton, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5 );
	
	m_nextButton = new wxButton( m_panel15, wxID_ANY, wxT("►│"), wxDefaultPosition, wxSize( 18,-1 ), 0|wxNO_BORDER );
	m_nextButton->SetFont( wxFont( 10, 70, 90, 90, false, wxEmptyString ) );
	m_nextButton->SetBackgroundColour( wxColour( 208, 208, 208 ) );
	
	fgSizer16->Add( m_nextButton, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5 );
	
	m_staticText6 = new wxStaticText( m_panel15, wxID_ANY, wxT("Display Time"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	fgSizer16->Add( m_staticText6, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 14 );
	
	m_slideshowDisplayTimeSpinCtrl = new wxSpinCtrl( m_panel15, wxID_ANY, wxT("3"), wxDefaultPosition, wxSize( 40,-1 ), wxSP_ARROW_KEYS, 1, 60, 0 );
	fgSizer16->Add( m_slideshowDisplayTimeSpinCtrl, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5 );
	
	
	m_panel15->SetSizer( fgSizer16 );
	m_panel15->Layout();
	fgSizer16->Fit( m_panel15 );
	fgSizer17->Add( m_panel15, 1, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxEXPAND|wxLEFT|wxTOP, 5 );
	
	
	m_panel16->SetSizer( fgSizer17 );
	m_panel16->Layout();
	fgSizer17->Fit( m_panel16 );
	fgSizer3->Add( m_panel16, 1, wxEXPAND, 5 );
	
	m_bitmapPanel = new wxPanel( m_panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
	m_bitmapPanel->SetBackgroundColour( wxColour( 208, 208, 208 ) );
	
	wxFlexGridSizer* fgSizer13;
	fgSizer13 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer13->AddGrowableCol( 0 );
	fgSizer13->AddGrowableRow( 0 );
	fgSizer13->SetFlexibleDirection( wxBOTH );
	fgSizer13->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_bitmap = new wxStaticBitmap( m_bitmapPanel, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_bitmap->SetBackgroundColour( wxColour( 208, 208, 208 ) );
	
	fgSizer13->Add( m_bitmap, 0, wxALL|wxEXPAND, 0 );
	
	
	m_bitmapPanel->SetSizer( fgSizer13 );
	m_bitmapPanel->Layout();
	fgSizer13->Fit( m_bitmapPanel );
	fgSizer3->Add( m_bitmapPanel, 1, wxEXPAND, 5 );
	
	
	m_panel2->SetSizer( fgSizer3 );
	m_panel2->Layout();
	fgSizer3->Fit( m_panel2 );
	m_panel18 = new wxPanel( m_splitter2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxWANTS_CHARS );
	m_panel18->SetBackgroundColour( wxColour( 208, 208, 208 ) );
	
	wxFlexGridSizer* fgSizer19;
	fgSizer19 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer19->AddGrowableCol( 0 );
	fgSizer19->AddGrowableRow( 0 );
	fgSizer19->SetFlexibleDirection( wxBOTH );
	fgSizer19->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_webView = wxWebView::New(m_panel18, wxID_ANY);
	/*
	if (wxWebView::IsBackendAvailable(wxWebViewBackendEdge))
	{
	m_webView = wxWebView::New(m_panel18,
	wxID_ANY,
	"https://google.com",
	wxDefaultPosition,
	wxDefaultSize,
	wxWebViewBackendEdge // <--- Request Chromium Edge
	);
	}
	*/
	
	fgSizer19->Add( m_webView, 0, wxEXPAND, 5 );
	
	
	m_panel18->SetSizer( fgSizer19 );
	m_panel18->Layout();
	fgSizer19->Fit( m_panel18 );
	m_splitter2->SplitVertically( m_panel2, m_panel18, 990 );
	fgSizer2->Add( m_splitter2, 0, wxEXPAND, 5 );
	
	
	m_topPanel->SetSizer( fgSizer2 );
	m_topPanel->Layout();
	fgSizer2->Fit( m_topPanel );
	m_panel14 = new wxPanel( m_mainSplitter, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxTAB_TRAVERSAL );
	m_panel14->SetBackgroundColour( wxColour( 208, 208, 208 ) );
	
	wxFlexGridSizer* fgSizer15;
	fgSizer15 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer15->AddGrowableCol( 0 );
	fgSizer15->AddGrowableRow( 0 );
	fgSizer15->SetFlexibleDirection( wxBOTH );
	fgSizer15->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_timelineBasePanel = new wxPanel( m_panel14, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_timelineBasePanel->SetBackgroundColour( wxColour( 208, 208, 208 ) );
	
	wxFlexGridSizer* fgSizer5;
	fgSizer5 = new wxFlexGridSizer( 1, 0, 0, 0 );
	fgSizer5->AddGrowableCol( 0 );
	fgSizer5->AddGrowableRow( 0 );
	fgSizer5->SetFlexibleDirection( wxBOTH );
	fgSizer5->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_timelinePanel = new wxPanel( m_timelineBasePanel, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
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
	
	m_panel8 = new wxPanel( m_timelineBasePanel, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer11;
	fgSizer11 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer11->AddGrowableRow( 0 );
	fgSizer11->SetFlexibleDirection( wxBOTH );
	fgSizer11->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_timelineZoomScrollBar = new wxScrollBar( m_panel8, wxID_ANY, wxDefaultPosition, wxSize( 26,-1 ), wxSB_VERTICAL );
	m_timelineZoomScrollBar->Hide();
	
	fgSizer11->Add( m_timelineZoomScrollBar, 0, wxEXPAND|wxLEFT|wxTOP, 5 );
	
	
	fgSizer11->Add( 0, 96, 1, wxEXPAND, 5 );
	
	
	m_panel8->SetSizer( fgSizer11 );
	m_panel8->Layout();
	fgSizer11->Fit( m_panel8 );
	fgSizer5->Add( m_panel8, 1, wxEXPAND|wxRIGHT, 5 );
	
	
	m_timelineBasePanel->SetSizer( fgSizer5 );
	m_timelineBasePanel->Layout();
	fgSizer5->Fit( m_timelineBasePanel );
	fgSizer15->Add( m_timelineBasePanel, 1, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 5 );
	
	m_panel6 = new wxPanel( m_panel14, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer61;
	fgSizer61 = new wxFlexGridSizer( 1, 0, 0, 0 );
	fgSizer61->AddGrowableCol( 0 );
	fgSizer61->AddGrowableRow( 0 );
	fgSizer61->SetFlexibleDirection( wxBOTH );
	fgSizer61->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_timelineDateScrollBar = new wxScrollBar( m_panel6, wxID_ANY, wxDefaultPosition, wxSize( -1,26 ), wxSB_HORIZONTAL );
	fgSizer61->Add( m_timelineDateScrollBar, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxEXPAND|wxLEFT|wxTOP, 5 );
	
	m_panel9 = new wxPanel( m_panel6, wxID_ANY, wxDefaultPosition, wxSize( 0,-1 ), wxTAB_TRAVERSAL );
	m_panel9->SetBackgroundColour( wxColour( 208, 208, 208 ) );
	
	fgSizer61->Add( m_panel9, 1, wxEXPAND | wxALL, 5 );
	
	
	m_panel6->SetSizer( fgSizer61 );
	m_panel6->Layout();
	fgSizer61->Fit( m_panel6 );
	fgSizer15->Add( m_panel6, 1, wxEXPAND|wxLEFT, 5 );
	
	
	m_panel14->SetSizer( fgSizer15 );
	m_panel14->Layout();
	fgSizer15->Fit( m_panel14 );
	m_mainSplitter->SplitHorizontally( m_topPanel, m_panel14, -1 );
	fgSizer1->Add( m_mainSplitter, 1, wxEXPAND, 5 );
	
	m_bottomPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_bottomPanel->SetBackgroundColour( wxColour( 208, 208, 208 ) );
	
	wxFlexGridSizer* fgSizer4;
	fgSizer4 = new wxFlexGridSizer( 1, 0, 0, 0 );
	fgSizer4->AddGrowableCol( 0 );
	fgSizer4->AddGrowableRow( 0 );
	fgSizer4->SetFlexibleDirection( wxBOTH );
	fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_debugTextCtrl = new wxTextCtrl( m_bottomPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), wxTE_READONLY|wxNO_BORDER );
	m_debugTextCtrl->SetBackgroundColour( wxColour( 208, 208, 208 ) );
	
	fgSizer4->Add( m_debugTextCtrl, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 5 );
	
	
	fgSizer4->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_exitButton = new wxButton( m_bottomPanel, wxID_ANY, wxT("Exit"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( m_exitButton, 0, 0, 5 );
	
	
	m_bottomPanel->SetSizer( fgSizer4 );
	m_bottomPanel->Layout();
	fgSizer4->Fit( m_bottomPanel );
	fgSizer1->Add( m_bottomPanel, 1, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 0 );
	
	
	this->SetSizer( fgSizer1 );
	this->Layout();
	m_guiTimer.SetOwner( this, MAIN_GUI_TIMER );
	m_imageTimer.SetOwner( this, MAIN_IMAGE_TIMER );
	m_slideshowTimer.SetOwner( this, MAIN_SLIDESHOW_TIMER );
	m_renderTickTimer.SetOwner( this, MAIN_RENDER_TIMER );
	m_menuBar = new wxMenuBar( 0 );
	this->SetMenuBar( m_menuBar );
	
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainForm::OnClose ) );
	this->Connect( wxEVT_IDLE, wxIdleEventHandler( MainForm::OnIdle ) );
	m_mainSplitter->Connect( wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGED, wxSplitterEventHandler( MainForm::OnMainSplitterSplitterSashPosChanged ), NULL, this );
	m_findButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainForm::OnFindButtonButtonClick ), NULL, this );
	m_findTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( MainForm::OnFindTextCtrlTextEnter ), NULL, this );
	m_googleSearchButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainForm::OnGoogleSearchButtonButtonClick ), NULL, this );
	m_wikipediaSearchButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainForm::OnWikipediaSearchButtonButtonClick ), NULL, this );
	m_mapSearchButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainForm::OnMapSearchButtonButtonClick ), NULL, this );
	m_dateLeftStaticText->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( MainForm::OnDateLeftStaticTextLeftDown ), NULL, this );
	m_dateLeftButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainForm::OnDateLeftButtonButtonClick ), NULL, this );
	m_dateTextCtrl->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( MainForm::OnDateTextCtrlKillFocus ), NULL, this );
	m_dateTextCtrl->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( MainForm::OnDateTextCtrlLeftDown ), NULL, this );
	m_dateTextCtrl->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( MainForm::OnDateTextCtrlSetFocus ), NULL, this );
	m_dateTextCtrl->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( MainForm::OnDateTextCtrlOnText ), NULL, this );
	m_dateTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( MainForm::OnDateTextCtrlTextEnter ), NULL, this );
	m_dateRightButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainForm::OnDateRightButtonButtonClick ), NULL, this );
	m_dateRightStaticText->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( MainForm::OnDateRightStaticTextLeftDown ), NULL, this );
	m_timelineZoomSlider->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( MainForm::OnTimelineZoomSliderScroll ), NULL, this );
	m_timelineZoomSlider->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( MainForm::OnTimelineZoomSliderScroll ), NULL, this );
	m_timelineZoomSlider->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( MainForm::OnTimelineZoomSliderScroll ), NULL, this );
	m_timelineZoomSlider->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( MainForm::OnTimelineZoomSliderScroll ), NULL, this );
	m_timelineZoomSlider->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( MainForm::OnTimelineZoomSliderScroll ), NULL, this );
	m_timelineZoomSlider->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( MainForm::OnTimelineZoomSliderScroll ), NULL, this );
	m_timelineZoomSlider->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( MainForm::OnTimelineZoomSliderScroll ), NULL, this );
	m_timelineZoomSlider->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( MainForm::OnTimelineZoomSliderScroll ), NULL, this );
	m_timelineZoomSlider->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( MainForm::OnTimelineZoomSliderScroll ), NULL, this );
	m_prevButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainForm::OnPrevButtonButtonClick ), NULL, this );
	m_pauseButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainForm::OnPauseButtonButtonClick ), NULL, this );
	m_playButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainForm::OnPlayButtonButtonClick ), NULL, this );
	m_nextButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainForm::OnNextButtonButtonClick ), NULL, this );
	m_slideshowDisplayTimeSpinCtrl->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( MainForm::OnSlideshowDisplayTimeSpinCtrlSpinCtrl ), NULL, this );
	m_bitmap->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( MainForm::OnBitmapLeftDClick ), NULL, this );
	m_bitmap->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( MainForm::OnBitmapLeftDown ), NULL, this );
	m_bitmap->Connect( wxEVT_MOTION, wxMouseEventHandler( MainForm::OnBitmapMotion ), NULL, this );
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
	m_exitButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainForm::OnExitButtonClick ), NULL, this );
	this->Connect( MAIN_GUI_TIMER, wxEVT_TIMER, wxTimerEventHandler( MainForm::OnGuiTimer ) );
	this->Connect( MAIN_IMAGE_TIMER, wxEVT_TIMER, wxTimerEventHandler( MainForm::OnImageTimer ) );
	this->Connect( MAIN_SLIDESHOW_TIMER, wxEVT_TIMER, wxTimerEventHandler( MainForm::OnSlideshowTimer ) );
	this->Connect( MAIN_RENDER_TIMER, wxEVT_TIMER, wxTimerEventHandler( MainForm::OnRenderTickTimer ) );
}

MainForm::~MainForm()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainForm::OnClose ) );
	this->Disconnect( wxEVT_IDLE, wxIdleEventHandler( MainForm::OnIdle ) );
	m_mainSplitter->Disconnect( wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGED, wxSplitterEventHandler( MainForm::OnMainSplitterSplitterSashPosChanged ), NULL, this );
	m_findButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainForm::OnFindButtonButtonClick ), NULL, this );
	m_findTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( MainForm::OnFindTextCtrlTextEnter ), NULL, this );
	m_googleSearchButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainForm::OnGoogleSearchButtonButtonClick ), NULL, this );
	m_wikipediaSearchButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainForm::OnWikipediaSearchButtonButtonClick ), NULL, this );
	m_mapSearchButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainForm::OnMapSearchButtonButtonClick ), NULL, this );
	m_dateLeftStaticText->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( MainForm::OnDateLeftStaticTextLeftDown ), NULL, this );
	m_dateLeftButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainForm::OnDateLeftButtonButtonClick ), NULL, this );
	m_dateTextCtrl->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( MainForm::OnDateTextCtrlKillFocus ), NULL, this );
	m_dateTextCtrl->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( MainForm::OnDateTextCtrlLeftDown ), NULL, this );
	m_dateTextCtrl->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( MainForm::OnDateTextCtrlSetFocus ), NULL, this );
	m_dateTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( MainForm::OnDateTextCtrlOnText ), NULL, this );
	m_dateTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( MainForm::OnDateTextCtrlTextEnter ), NULL, this );
	m_dateRightButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainForm::OnDateRightButtonButtonClick ), NULL, this );
	m_dateRightStaticText->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( MainForm::OnDateRightStaticTextLeftDown ), NULL, this );
	m_timelineZoomSlider->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( MainForm::OnTimelineZoomSliderScroll ), NULL, this );
	m_timelineZoomSlider->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( MainForm::OnTimelineZoomSliderScroll ), NULL, this );
	m_timelineZoomSlider->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( MainForm::OnTimelineZoomSliderScroll ), NULL, this );
	m_timelineZoomSlider->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( MainForm::OnTimelineZoomSliderScroll ), NULL, this );
	m_timelineZoomSlider->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( MainForm::OnTimelineZoomSliderScroll ), NULL, this );
	m_timelineZoomSlider->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( MainForm::OnTimelineZoomSliderScroll ), NULL, this );
	m_timelineZoomSlider->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( MainForm::OnTimelineZoomSliderScroll ), NULL, this );
	m_timelineZoomSlider->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( MainForm::OnTimelineZoomSliderScroll ), NULL, this );
	m_timelineZoomSlider->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( MainForm::OnTimelineZoomSliderScroll ), NULL, this );
	m_prevButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainForm::OnPrevButtonButtonClick ), NULL, this );
	m_pauseButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainForm::OnPauseButtonButtonClick ), NULL, this );
	m_playButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainForm::OnPlayButtonButtonClick ), NULL, this );
	m_nextButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainForm::OnNextButtonButtonClick ), NULL, this );
	m_slideshowDisplayTimeSpinCtrl->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( MainForm::OnSlideshowDisplayTimeSpinCtrlSpinCtrl ), NULL, this );
	m_bitmap->Disconnect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( MainForm::OnBitmapLeftDClick ), NULL, this );
	m_bitmap->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( MainForm::OnBitmapLeftDown ), NULL, this );
	m_bitmap->Disconnect( wxEVT_MOTION, wxMouseEventHandler( MainForm::OnBitmapMotion ), NULL, this );
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
	m_exitButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainForm::OnExitButtonClick ), NULL, this );
	this->Disconnect( MAIN_GUI_TIMER, wxEVT_TIMER, wxTimerEventHandler( MainForm::OnGuiTimer ) );
	this->Disconnect( MAIN_IMAGE_TIMER, wxEVT_TIMER, wxTimerEventHandler( MainForm::OnImageTimer ) );
	this->Disconnect( MAIN_SLIDESHOW_TIMER, wxEVT_TIMER, wxTimerEventHandler( MainForm::OnSlideshowTimer ) );
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
	m_imageBitmap->Connect( wxEVT_SIZE, wxSizeEventHandler( ImageDialog::OnImageBitmapSize ), NULL, this );
	ImageSdbSizerOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ImageDialog::OnImageSdbSizerOKButtonClick ), NULL, this );
}

ImageDialog::~ImageDialog()
{
	// Disconnect Events
	this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( ImageDialog::OnImageDialogSize ) );
	m_imageBitmap->Disconnect( wxEVT_SIZE, wxSizeEventHandler( ImageDialog::OnImageBitmapSize ), NULL, this );
	ImageSdbSizerOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ImageDialog::OnImageSdbSizerOKButtonClick ), NULL, this );
	
}
