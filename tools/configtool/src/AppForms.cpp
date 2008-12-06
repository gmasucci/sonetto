///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb  1 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "AppForms.h"

///////////////////////////////////////////////////////////////////////////
using namespace ConfigApplication;

BEGIN_EVENT_TABLE( ConfigWindow, wxDialog )
	EVT_CHOICEBOOK_PAGE_CHANGED( ID_RSYS_CBOOK, ConfigWindow::_wxFB_onRsysChange )
	EVT_CHOICEBOOK_PAGE_CHANGING( ID_RSYS_CBOOK, ConfigWindow::_wxFB_onRsysChanging )
	EVT_CHOICE( ID_D3D9_RES, ConfigWindow::_wxFB_onChoiceD3D9Res )
	EVT_CHECKBOX( ID_D3D9_FSCR, ConfigWindow::_wxFB_onCheckD3D9FullScreen )
	EVT_CHECKBOX( ID_D3D9_VSYNC, ConfigWindow::_wxFB_onCheckD3D9VSync )
	EVT_CHOICE( ID_OGL_RES, ConfigWindow::_wxFB_onChoiceOGLRes )
	EVT_CHOICE( ID_OGL_CDEPTH, ConfigWindow::_wxFB_onChoiceOGLColDepth )
	EVT_CHOICE( ID_OGL_FREQ, ConfigWindow::_wxFB_onChoiceOGLFreq )
	EVT_CHOICE( ID_OGL_FSAA, ConfigWindow::_wxFB_onChoiceOGLFSAA )
	EVT_CHECKBOX( ID_OGL_FSCR, ConfigWindow::_wxFB_onCheckOGLFullScreen )
	EVT_CHECKBOX( ID_OGL_VSYNC, ConfigWindow::_wxFB_onCheckOGLVSync )
	EVT_CHOICE( ID_INPUT_DEVICE, ConfigWindow::_wxFB_onDeviceChoice )
	EVT_CHOICE( ID_INPUT_DEVICE, ConfigWindow::_wxFB_onDeviceChoice )
	EVT_CHOICE( ID_INPUT_DEVICE, ConfigWindow::_wxFB_onDeviceChoice )
	EVT_CHOICE( ID_INPUT_DEVICE, ConfigWindow::_wxFB_onDeviceChoice )
	EVT_BUTTON( wxID_APPLY, ConfigWindow::_wxFB_OnClickApply )
	EVT_BUTTON( wxID_CANCEL, ConfigWindow::_wxFB_OnClickCancel )
	EVT_BUTTON( wxID_OK, ConfigWindow::_wxFB_OnClickOk )
END_EVENT_TABLE()

ConfigWindow::ConfigWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxFlexGridSizer* BaseSizer;
	BaseSizer = new wxFlexGridSizer( 3, 1, 0, 0 );
	BaseSizer->SetFlexibleDirection( wxBOTH );
	BaseSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	Logo = new wxStaticBitmap( this, wxID_ANY, wxBitmap( wxT("logo.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxSize( 500,50 ), 0 );
	BaseSizer->Add( Logo, 0, wxALIGN_CENTER|wxALIGN_TOP, 0 );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );

	bSizer2->SetMinSize( wxSize( -1,295 ) );
	SectionTabs = new wxNotebook( this, ID_SECTION_TABS, wxDefaultPosition, wxDefaultSize, 0 );
	VideoTab = new wxPanel( SectionTabs, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );

	m_notebook3 = new wxNotebook( VideoTab, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel9 = new wxPanel( m_notebook3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxStaticBoxSizer* sbSizer28;
	sbSizer28 = new wxStaticBoxSizer( new wxStaticBox( m_panel9, wxID_ANY, wxT("Render System") ), wxVERTICAL );

	m_choicebook3 = new wxChoicebook( m_panel9, ID_RSYS_CBOOK, wxDefaultPosition, wxDefaultSize, wxCHB_DEFAULT );
	mD3D9Panel = new wxPanel( m_choicebook3, ID_D3D9_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	mD3D9Panel->Hide();

	wxFlexGridSizer* fgSizer16;
	fgSizer16 = new wxFlexGridSizer( 4, 1, 0, 0 );
	fgSizer16->SetFlexibleDirection( wxBOTH );
	fgSizer16->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxGridSizer* gSizer15;
	gSizer15 = new wxGridSizer( 1, 2, 0, 0 );

	m_staticText5 = new wxStaticText( mD3D9Panel, wxID_ANY, wxT("Display Mode:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	gSizer15->Add( m_staticText5, 0, wxALL, 9 );

	wxArrayString m_choice_d3d9_resChoices;
	m_choice_d3d9_res = new wxChoice( mD3D9Panel, ID_D3D9_RES, wxDefaultPosition, wxSize( 200,-1 ), m_choice_d3d9_resChoices, 0 );
	m_choice_d3d9_res->SetSelection( 0 );
	gSizer15->Add( m_choice_d3d9_res, 0, wxALL, 5 );

	fgSizer16->Add( gSizer15, 1, wxEXPAND, 5 );

	wxGridSizer* gSizer9;
	gSizer9 = new wxGridSizer( 1, 2, 0, 0 );

	m_staticText8 = new wxStaticText( mD3D9Panel, wxID_ANY, wxT("Anti-Aliasing:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	gSizer9->Add( m_staticText8, 0, wxALL, 9 );

	wxArrayString m_choice_d3d9_aaChoices;
	m_choice_d3d9_aa = new wxChoice( mD3D9Panel, ID_D3D9_AA, wxDefaultPosition, wxSize( 200,-1 ), m_choice_d3d9_aaChoices, 0 );
	m_choice_d3d9_aa->SetSelection( 0 );
	gSizer9->Add( m_choice_d3d9_aa, 0, wxALL, 5 );

	fgSizer16->Add( gSizer9, 1, wxEXPAND, 5 );

	m_staticline2 = new wxStaticLine( mD3D9Panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer16->Add( m_staticline2, 0, wxEXPAND | wxALL, 5 );

	m_checkBox_d3d9_fscr = new wxCheckBox( mD3D9Panel, ID_D3D9_FSCR, wxT("Fullscreen"), wxDefaultPosition, wxDefaultSize, 0 );
	m_checkBox_d3d9_fscr->SetValue(true);

	fgSizer16->Add( m_checkBox_d3d9_fscr, 0, wxALL, 5 );

	m_checkBox_d3d9_vsync = new wxCheckBox( mD3D9Panel, ID_D3D9_VSYNC, wxT("VSync"), wxDefaultPosition, wxDefaultSize, 0 );
	m_checkBox_d3d9_vsync->SetValue(true);

	fgSizer16->Add( m_checkBox_d3d9_vsync, 0, wxALL, 5 );

	mD3D9Panel->SetSizer( fgSizer16 );
	mD3D9Panel->Layout();
	fgSizer16->Fit( mD3D9Panel );
	m_choicebook3->AddPage( mD3D9Panel, wxT("Direct3D 9"), true );
	mOGLPanel = new wxPanel( m_choicebook3, ID_OGL_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer17;
	fgSizer17 = new wxFlexGridSizer( 6, 1, 0, 0 );
	fgSizer17->SetFlexibleDirection( wxBOTH );
	fgSizer17->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxGridSizer* gSizer19;
	gSizer19 = new wxGridSizer( 1, 2, 0, 0 );

	m_staticText11 = new wxStaticText( mOGLPanel, wxID_ANY, wxT("Resolution:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	gSizer19->Add( m_staticText11, 0, wxALL, 9 );

	wxArrayString m_choice_ogl_resChoices;
	m_choice_ogl_res = new wxChoice( mOGLPanel, ID_OGL_RES, wxDefaultPosition, wxSize( 200,-1 ), m_choice_ogl_resChoices, 0 );
	m_choice_ogl_res->SetSelection( 0 );
	gSizer19->Add( m_choice_ogl_res, 0, wxALL, 5 );

	fgSizer17->Add( gSizer19, 1, wxEXPAND, 5 );

	wxGridSizer* gSizer20;
	gSizer20 = new wxGridSizer( 1, 2, 0, 0 );

	m_staticText12 = new wxStaticText( mOGLPanel, wxID_ANY, wxT("Color Depth:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	gSizer20->Add( m_staticText12, 0, wxALL, 9 );

	wxArrayString m_choice_ogl_cdepthChoices;
	m_choice_ogl_cdepth = new wxChoice( mOGLPanel, ID_OGL_CDEPTH, wxDefaultPosition, wxSize( 200,-1 ), m_choice_ogl_cdepthChoices, 0 );
	m_choice_ogl_cdepth->SetSelection( 0 );
	gSizer20->Add( m_choice_ogl_cdepth, 0, wxALL, 5 );

	fgSizer17->Add( gSizer20, 1, wxEXPAND, 5 );

	wxGridSizer* gSizer21;
	gSizer21 = new wxGridSizer( 1, 2, 0, 0 );

	m_staticText13 = new wxStaticText( mOGLPanel, wxID_ANY, wxT("Monitor Frequency:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	gSizer21->Add( m_staticText13, 0, wxALL, 9 );

	wxArrayString m_choice_ogl_freqChoices;
	m_choice_ogl_freq = new wxChoice( mOGLPanel, ID_OGL_FREQ, wxDefaultPosition, wxSize( 200,-1 ), m_choice_ogl_freqChoices, 0 );
	m_choice_ogl_freq->SetSelection( 0 );
	gSizer21->Add( m_choice_ogl_freq, 0, wxALL, 5 );

	fgSizer17->Add( gSizer21, 1, wxEXPAND, 5 );

	wxGridSizer* gSizer22;
	gSizer22 = new wxGridSizer( 1, 2, 0, 0 );

	m_staticText14 = new wxStaticText( mOGLPanel, wxID_ANY, wxT("Anti-Aliasing:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14->Wrap( -1 );
	gSizer22->Add( m_staticText14, 0, wxALL, 9 );

	wxArrayString m_choice_ogl_fsaaChoices;
	m_choice_ogl_fsaa = new wxChoice( mOGLPanel, ID_OGL_FSAA, wxDefaultPosition, wxSize( 200,-1 ), m_choice_ogl_fsaaChoices, 0 );
	m_choice_ogl_fsaa->SetSelection( 0 );
	gSizer22->Add( m_choice_ogl_fsaa, 0, wxALL, 5 );

	fgSizer17->Add( gSizer22, 1, wxEXPAND, 5 );

	m_staticline3 = new wxStaticLine( mOGLPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer17->Add( m_staticline3, 0, wxEXPAND | wxALL, 5 );

	m_checkBox_ogl_fscr = new wxCheckBox( mOGLPanel, ID_OGL_FSCR, wxT("Fullscreen"), wxDefaultPosition, wxDefaultSize, 0 );
	m_checkBox_ogl_fscr->SetValue(true);

	fgSizer17->Add( m_checkBox_ogl_fscr, 0, wxALL, 5 );

	m_checkBox_ogl_vsync = new wxCheckBox( mOGLPanel, ID_OGL_VSYNC, wxT("VSync"), wxDefaultPosition, wxDefaultSize, 0 );
	m_checkBox_ogl_vsync->SetValue(true);

	fgSizer17->Add( m_checkBox_ogl_vsync, 0, wxALL, 5 );

	mOGLPanel->SetSizer( fgSizer17 );
	mOGLPanel->Layout();
	fgSizer17->Fit( mOGLPanel );
	m_choicebook3->AddPage( mOGLPanel, wxT("OpenGL"), false );
	sbSizer28->Add( m_choicebook3, 1, wxEXPAND | wxALL, 5 );

	m_panel9->SetSizer( sbSizer28 );
	m_panel9->Layout();
	sbSizer28->Fit( m_panel9 );
	m_notebook3->AddPage( m_panel9, wxT("Display Settings"), true );
	m_panel10 = new wxPanel( m_notebook3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxVERTICAL );

	m_panel10->SetSizer( bSizer12 );
	m_panel10->Layout();
	bSizer12->Fit( m_panel10 );
	m_notebook3->AddPage( m_panel10, wxT("Advanced"), false );

	bSizer4->Add( m_notebook3, 1, wxEXPAND | wxALL, 5 );

	VideoTab->SetSizer( bSizer4 );
	VideoTab->Layout();
	bSizer4->Fit( VideoTab );
	SectionTabs->AddPage( VideoTab, wxT("Video"), false );
	AudioTab = new wxPanel( SectionTabs, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxGridSizer* gSizer10;
	gSizer10 = new wxGridSizer( 1, 2, 0, 0 );

	wxFlexGridSizer* fgSizer11;
	fgSizer11 = new wxFlexGridSizer( 2, 1, 0, 0 );
	fgSizer11->SetFlexibleDirection( wxBOTH );
	fgSizer11->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxStaticBoxSizer* sbSizer30;
	sbSizer30 = new wxStaticBoxSizer( new wxStaticBox( AudioTab, wxID_ANY, wxT("Audio Device") ), wxVERTICAL );

	wxString m_choice24Choices[] = { wxT("Default"), wxT("Generic Software"), wxT("Generic Hardware") };
	int m_choice24NChoices = sizeof( m_choice24Choices ) / sizeof( wxString );
	m_choice24 = new wxChoice( AudioTab, ID_OAL_DEVICE, wxDefaultPosition, wxSize( 215,-1 ), m_choice24NChoices, m_choice24Choices, 0 );
	m_choice24->SetSelection( 0 );
	sbSizer30->Add( m_choice24, 0, wxALL, 5 );

	fgSizer11->Add( sbSizer30, 1, wxEXPAND, 5 );

	gSizer10->Add( fgSizer11, 1, wxEXPAND, 5 );

	wxGridSizer* gSizer11;
	gSizer11 = new wxGridSizer( 4, 1, 0, 0 );

	wxStaticBoxSizer* sbSizer25;
	sbSizer25 = new wxStaticBoxSizer( new wxStaticBox( AudioTab, wxID_ANY, wxT("Music Volume") ), wxVERTICAL );

	m_slider4 = new wxSlider( AudioTab, ID_OAL_MVOL, 1000, 0, 1000, wxDefaultPosition, wxSize( 216,-1 ), wxSL_HORIZONTAL );
	sbSizer25->Add( m_slider4, 0, wxALL, 5 );

	gSizer11->Add( sbSizer25, 1, 0, 5 );

	wxStaticBoxSizer* sbSizer26;
	sbSizer26 = new wxStaticBoxSizer( new wxStaticBox( AudioTab, wxID_ANY, wxT("Effect Volume") ), wxVERTICAL );

	m_slider41 = new wxSlider( AudioTab, ID_OAL_EVOL, 1000, 0, 1000, wxDefaultPosition, wxSize( 216,-1 ), wxSL_HORIZONTAL );
	sbSizer26->Add( m_slider41, 0, wxALL, 5 );

	gSizer11->Add( sbSizer26, 1, 0, 5 );

	wxStaticBoxSizer* sbSizer27;
	sbSizer27 = new wxStaticBoxSizer( new wxStaticBox( AudioTab, wxID_ANY, wxT("Voiceover Volume") ), wxVERTICAL );

	m_slider42 = new wxSlider( AudioTab, ID_OAL_VVOL, 1000, 0, 1000, wxDefaultPosition, wxSize( 216,-1 ), wxSL_HORIZONTAL );
	sbSizer27->Add( m_slider42, 0, wxALL, 5 );

	gSizer11->Add( sbSizer27, 1, 0, 5 );


	gSizer11->Add( 0, 0, 1, wxEXPAND, 5 );

	gSizer10->Add( gSizer11, 1, wxEXPAND, 5 );

	AudioTab->SetSizer( gSizer10 );
	AudioTab->Layout();
	gSizer10->Fit( AudioTab );
	SectionTabs->AddPage( AudioTab, wxT("Audio"), false );
	InputTab = new wxPanel( SectionTabs, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* InputBase_sizer;
	InputBase_sizer = new wxBoxSizer( wxVERTICAL );

	m_notebook4 = new wxNotebook( InputTab, ID_PLAYERS_TABS, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel131 = new wxPanel( m_notebook4, ID_PLAYER_TAB, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer91;
	bSizer91 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer101;
	bSizer101 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText101 = new wxStaticText( m_panel131, wxID_ANY, wxT("Controller Device: "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText101->Wrap( -1 );
	bSizer101->Add( m_staticText101, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxArrayString m_choice101Choices;
	m_choice101 = new wxChoice( m_panel131, ID_INPUT_DEVICE, wxDefaultPosition, wxDefaultSize, m_choice101Choices, 0 );
	m_choice101->SetSelection( 0 );
	bSizer101->Add( m_choice101, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxBoxSizer* bSizer111;
	bSizer111 = new wxBoxSizer( wxVERTICAL );

	m_checkBox51 = new wxCheckBox( m_panel131, ID_CTRL_PLUGGED, wxT("Controllher Plugged"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_checkBox51->SetValue(true);

	bSizer111->Add( m_checkBox51, 0, wxALL|wxALIGN_RIGHT, 1 );

	m_checkBox61 = new wxCheckBox( m_panel131, ID_CTRL_FFBACK, wxT("Enabled Force Feedback"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );

	bSizer111->Add( m_checkBox61, 0, wxALL|wxALIGN_RIGHT, 1 );

	bSizer101->Add( bSizer111, 1, wxEXPAND, 5 );

	bSizer91->Add( bSizer101, 0, wxEXPAND, 5 );

	m_staticline311 = new wxStaticLine( m_panel131, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer91->Add( m_staticline311, 0, wxEXPAND | wxALL, 5 );

	wxGridSizer* gSizer1011;
	gSizer1011 = new wxGridSizer( 1, 6, 0, 0 );

	wxBoxSizer* bSizer131;
	bSizer131 = new wxBoxSizer( wxVERTICAL );

	m_bitmap31 = new wxStaticBitmap( m_panel131, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer131->Add( m_bitmap31, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl61119 = new wxTextCtrl( m_panel131, ID_AXIS_LH1, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer131->Add( m_textCtrl61119, 0, wxALL|wxEXPAND, 2 );

	m_bitmap41 = new wxStaticBitmap( m_panel131, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer131->Add( m_bitmap41, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl61120 = new wxTextCtrl( m_panel131, ID_AXIS_LH2, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer131->Add( m_textCtrl61120, 0, wxALL|wxEXPAND, 2 );

	m_bitmap51 = new wxStaticBitmap( m_panel131, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer131->Add( m_bitmap51, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl61121 = new wxTextCtrl( m_panel131, ID_AXIS_LV1, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer131->Add( m_textCtrl61121, 0, wxALL|wxEXPAND, 2 );

	m_bitmap61 = new wxStaticBitmap( m_panel131, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer131->Add( m_bitmap61, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl61122 = new wxTextCtrl( m_panel131, ID_AXIS_LV2, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer131->Add( m_textCtrl61122, 0, wxALL|wxEXPAND, 2 );

	gSizer1011->Add( bSizer131, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer155;
	bSizer155 = new wxBoxSizer( wxVERTICAL );

	m_bitmap75 = new wxStaticBitmap( m_panel131, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer155->Add( m_bitmap75, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl61123 = new wxTextCtrl( m_panel131, ID_AXIS_RH1, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer155->Add( m_textCtrl61123, 0, wxALL|wxEXPAND, 2 );

	m_bitmap85 = new wxStaticBitmap( m_panel131, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer155->Add( m_bitmap85, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl61124 = new wxTextCtrl( m_panel131, ID_AXIS_RH2, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer155->Add( m_textCtrl61124, 0, wxALL|wxEXPAND, 2 );

	m_bitmap95 = new wxStaticBitmap( m_panel131, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer155->Add( m_bitmap95, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl61125 = new wxTextCtrl( m_panel131, ID_AXIS_RV1, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer155->Add( m_textCtrl61125, 0, wxALL|wxEXPAND, 2 );

	m_bitmap105 = new wxStaticBitmap( m_panel131, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer155->Add( m_bitmap105, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl61126 = new wxTextCtrl( m_panel131, ID_AXIS_RV2, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer155->Add( m_textCtrl61126, 0, wxALL|wxEXPAND, 2 );

	gSizer1011->Add( bSizer155, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer1511;
	bSizer1511 = new wxBoxSizer( wxVERTICAL );

	m_bitmap711 = new wxStaticBitmap( m_panel131, wxID_ANY, wxBitmap( wxT("triangle.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1511->Add( m_bitmap711, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl6112 = new wxTextCtrl( m_panel131, ID_BTN_TRIANGLE, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1511->Add( m_textCtrl6112, 0, wxALL|wxEXPAND, 2 );

	m_bitmap811 = new wxStaticBitmap( m_panel131, wxID_ANY, wxBitmap( wxT("L2.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1511->Add( m_bitmap811, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl6115 = new wxTextCtrl( m_panel131, ID_BTN_L2, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1511->Add( m_textCtrl6115, 0, wxALL|wxEXPAND, 2 );

	m_bitmap911 = new wxStaticBitmap( m_panel131, wxID_ANY, wxBitmap( wxT("select.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1511->Add( m_bitmap911, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl6119 = new wxTextCtrl( m_panel131, ID_BTN_SELECT, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1511->Add( m_textCtrl6119, 0, wxALL|wxEXPAND, 2 );

	m_bitmap1011 = new wxStaticBitmap( m_panel131, wxID_ANY, wxBitmap( wxT("dpad_up.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1511->Add( m_bitmap1011, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl61115 = new wxTextCtrl( m_panel131, ID_BTN_DUP, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1511->Add( m_textCtrl61115, 0, wxALL|wxEXPAND, 2 );

	gSizer1011->Add( bSizer1511, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer1521;
	bSizer1521 = new wxBoxSizer( wxVERTICAL );

	m_bitmap721 = new wxStaticBitmap( m_panel131, wxID_ANY, wxBitmap( wxT("circle.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1521->Add( m_bitmap721, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl611 = new wxTextCtrl( m_panel131, ID_BTN_CIRCLE, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1521->Add( m_textCtrl611, 0, wxALL|wxEXPAND, 2 );

	m_bitmap821 = new wxStaticBitmap( m_panel131, wxID_ANY, wxBitmap( wxT("R2.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1521->Add( m_bitmap821, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl6116 = new wxTextCtrl( m_panel131, ID_BTN_R2, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1521->Add( m_textCtrl6116, 0, wxALL|wxEXPAND, 2 );

	m_bitmap921 = new wxStaticBitmap( m_panel131, wxID_ANY, wxBitmap( wxT("start.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1521->Add( m_bitmap921, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl61110 = new wxTextCtrl( m_panel131, ID_BTN_START, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1521->Add( m_textCtrl61110, 0, wxALL|wxEXPAND, 2 );

	m_bitmap1021 = new wxStaticBitmap( m_panel131, wxID_ANY, wxBitmap( wxT("dpad_right.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1521->Add( m_bitmap1021, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl61116 = new wxTextCtrl( m_panel131, ID_BTN_DRIGHT, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1521->Add( m_textCtrl61116, 0, wxALL|wxEXPAND, 2 );

	gSizer1011->Add( bSizer1521, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer1531;
	bSizer1531 = new wxBoxSizer( wxVERTICAL );

	m_bitmap731 = new wxStaticBitmap( m_panel131, wxID_ANY, wxBitmap( wxT("cross.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1531->Add( m_bitmap731, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl6113 = new wxTextCtrl( m_panel131, ID_BTN_CROSS, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1531->Add( m_textCtrl6113, 0, wxALL|wxEXPAND, 2 );

	m_bitmap831 = new wxStaticBitmap( m_panel131, wxID_ANY, wxBitmap( wxT("L1.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1531->Add( m_bitmap831, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl6117 = new wxTextCtrl( m_panel131, ID_BTN_L1, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1531->Add( m_textCtrl6117, 0, wxALL|wxEXPAND, 2 );

	m_bitmap931 = new wxStaticBitmap( m_panel131, wxID_ANY, wxBitmap( wxT("L3.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1531->Add( m_bitmap931, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl61113 = new wxTextCtrl( m_panel131, ID_BTN_L3, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1531->Add( m_textCtrl61113, 0, wxALL|wxEXPAND, 2 );

	m_bitmap1031 = new wxStaticBitmap( m_panel131, wxID_ANY, wxBitmap( wxT("dpad_down.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1531->Add( m_bitmap1031, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl61117 = new wxTextCtrl( m_panel131, ID_BTN_DDOWN, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1531->Add( m_textCtrl61117, 0, wxALL|wxEXPAND, 2 );

	gSizer1011->Add( bSizer1531, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer1541;
	bSizer1541 = new wxBoxSizer( wxVERTICAL );

	m_bitmap741 = new wxStaticBitmap( m_panel131, wxID_ANY, wxBitmap( wxT("square.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1541->Add( m_bitmap741, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl6114 = new wxTextCtrl( m_panel131, ID_BTN_SQUARE, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1541->Add( m_textCtrl6114, 0, wxALL|wxEXPAND, 2 );

	m_bitmap841 = new wxStaticBitmap( m_panel131, wxID_ANY, wxBitmap( wxT("R1.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1541->Add( m_bitmap841, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl6118 = new wxTextCtrl( m_panel131, ID_BTN_R1, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1541->Add( m_textCtrl6118, 0, wxALL|wxEXPAND, 2 );

	m_bitmap941 = new wxStaticBitmap( m_panel131, wxID_ANY, wxBitmap( wxT("R3.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1541->Add( m_bitmap941, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl61114 = new wxTextCtrl( m_panel131, ID_BTN_R3, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1541->Add( m_textCtrl61114, 0, wxALL|wxEXPAND, 2 );

	m_bitmap1041 = new wxStaticBitmap( m_panel131, wxID_ANY, wxBitmap( wxT("dpad_left.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1541->Add( m_bitmap1041, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl61118 = new wxTextCtrl( m_panel131, ID_BTN_DLEFT, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1541->Add( m_textCtrl61118, 0, wxALL|wxEXPAND, 2 );

	gSizer1011->Add( bSizer1541, 1, wxEXPAND, 5 );

	bSizer91->Add( gSizer1011, 1, wxEXPAND, 5 );

	m_panel131->SetSizer( bSizer91 );
	m_panel131->Layout();
	bSizer91->Fit( m_panel131 );
	m_notebook4->AddPage( m_panel131, wxT("Player 1"), false );
	m_panel1311 = new wxPanel( m_notebook4, ID_PLAYER_TAB, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer911;
	bSizer911 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer1011;
	bSizer1011 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText1011 = new wxStaticText( m_panel1311, wxID_ANY, wxT("Controller Device: "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1011->Wrap( -1 );
	bSizer1011->Add( m_staticText1011, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxArrayString m_choice1011Choices;
	m_choice1011 = new wxChoice( m_panel1311, ID_INPUT_DEVICE, wxDefaultPosition, wxDefaultSize, m_choice1011Choices, 0 );
	m_choice1011->SetSelection( 0 );
	bSizer1011->Add( m_choice1011, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxBoxSizer* bSizer1111;
	bSizer1111 = new wxBoxSizer( wxVERTICAL );

	m_checkBox511 = new wxCheckBox( m_panel1311, ID_CTRL_PLUGGED, wxT("Controllher Plugged"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );

	bSizer1111->Add( m_checkBox511, 0, wxALL|wxALIGN_RIGHT, 1 );

	m_checkBox611 = new wxCheckBox( m_panel1311, ID_CTRL_FFBACK, wxT("Enabled Force Feedback"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );

	bSizer1111->Add( m_checkBox611, 0, wxALL|wxALIGN_RIGHT, 1 );

	bSizer1011->Add( bSizer1111, 1, wxEXPAND, 5 );

	bSizer911->Add( bSizer1011, 0, wxEXPAND, 5 );

	m_staticline3111 = new wxStaticLine( m_panel1311, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer911->Add( m_staticline3111, 0, wxEXPAND | wxALL, 5 );

	wxGridSizer* gSizer10111;
	gSizer10111 = new wxGridSizer( 1, 6, 0, 0 );

	wxBoxSizer* bSizer1311;
	bSizer1311 = new wxBoxSizer( wxVERTICAL );

	m_bitmap311 = new wxStaticBitmap( m_panel1311, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1311->Add( m_bitmap311, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl611191 = new wxTextCtrl( m_panel1311, ID_AXIS_LH1, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1311->Add( m_textCtrl611191, 0, wxALL|wxEXPAND, 2 );

	m_bitmap411 = new wxStaticBitmap( m_panel1311, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1311->Add( m_bitmap411, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl611201 = new wxTextCtrl( m_panel1311, ID_AXIS_LH2, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1311->Add( m_textCtrl611201, 0, wxALL|wxEXPAND, 2 );

	m_bitmap511 = new wxStaticBitmap( m_panel1311, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1311->Add( m_bitmap511, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl611211 = new wxTextCtrl( m_panel1311, ID_AXIS_LV1, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1311->Add( m_textCtrl611211, 0, wxALL|wxEXPAND, 2 );

	m_bitmap611 = new wxStaticBitmap( m_panel1311, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1311->Add( m_bitmap611, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl611221 = new wxTextCtrl( m_panel1311, ID_AXIS_LV2, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1311->Add( m_textCtrl611221, 0, wxALL|wxEXPAND, 2 );

	gSizer10111->Add( bSizer1311, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer1551;
	bSizer1551 = new wxBoxSizer( wxVERTICAL );

	m_bitmap751 = new wxStaticBitmap( m_panel1311, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1551->Add( m_bitmap751, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl611231 = new wxTextCtrl( m_panel1311, ID_AXIS_RH1, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1551->Add( m_textCtrl611231, 0, wxALL|wxEXPAND, 2 );

	m_bitmap851 = new wxStaticBitmap( m_panel1311, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1551->Add( m_bitmap851, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl611241 = new wxTextCtrl( m_panel1311, ID_AXIS_RH2, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1551->Add( m_textCtrl611241, 0, wxALL|wxEXPAND, 2 );

	m_bitmap951 = new wxStaticBitmap( m_panel1311, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1551->Add( m_bitmap951, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl611251 = new wxTextCtrl( m_panel1311, ID_AXIS_RV1, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1551->Add( m_textCtrl611251, 0, wxALL|wxEXPAND, 2 );

	m_bitmap1051 = new wxStaticBitmap( m_panel1311, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1551->Add( m_bitmap1051, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl611261 = new wxTextCtrl( m_panel1311, ID_AXIS_RV2, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1551->Add( m_textCtrl611261, 0, wxALL|wxEXPAND, 2 );

	gSizer10111->Add( bSizer1551, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer15111;
	bSizer15111 = new wxBoxSizer( wxVERTICAL );

	m_bitmap7111 = new wxStaticBitmap( m_panel1311, wxID_ANY, wxBitmap( wxT("triangle.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15111->Add( m_bitmap7111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl61127 = new wxTextCtrl( m_panel1311, ID_BTN_TRIANGLE, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer15111->Add( m_textCtrl61127, 0, wxALL|wxEXPAND, 2 );

	m_bitmap8111 = new wxStaticBitmap( m_panel1311, wxID_ANY, wxBitmap( wxT("L2.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15111->Add( m_bitmap8111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl61151 = new wxTextCtrl( m_panel1311, ID_BTN_L2, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer15111->Add( m_textCtrl61151, 0, wxALL|wxEXPAND, 2 );

	m_bitmap9111 = new wxStaticBitmap( m_panel1311, wxID_ANY, wxBitmap( wxT("select.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15111->Add( m_bitmap9111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl61191 = new wxTextCtrl( m_panel1311, ID_BTN_SELECT, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer15111->Add( m_textCtrl61191, 0, wxALL|wxEXPAND, 2 );

	m_bitmap10111 = new wxStaticBitmap( m_panel1311, wxID_ANY, wxBitmap( wxT("dpad_up.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15111->Add( m_bitmap10111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl611151 = new wxTextCtrl( m_panel1311, ID_BTN_DUP, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer15111->Add( m_textCtrl611151, 0, wxALL|wxEXPAND, 2 );

	gSizer10111->Add( bSizer15111, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer15211;
	bSizer15211 = new wxBoxSizer( wxVERTICAL );

	m_bitmap7211 = new wxStaticBitmap( m_panel1311, wxID_ANY, wxBitmap( wxT("circle.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15211->Add( m_bitmap7211, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl6111 = new wxTextCtrl( m_panel1311, ID_BTN_CIRCLE, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer15211->Add( m_textCtrl6111, 0, wxALL|wxEXPAND, 2 );

	m_bitmap8211 = new wxStaticBitmap( m_panel1311, wxID_ANY, wxBitmap( wxT("R2.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15211->Add( m_bitmap8211, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl61161 = new wxTextCtrl( m_panel1311, ID_BTN_R2, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer15211->Add( m_textCtrl61161, 0, wxALL|wxEXPAND, 2 );

	m_bitmap9211 = new wxStaticBitmap( m_panel1311, wxID_ANY, wxBitmap( wxT("start.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15211->Add( m_bitmap9211, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl611101 = new wxTextCtrl( m_panel1311, ID_BTN_START, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer15211->Add( m_textCtrl611101, 0, wxALL|wxEXPAND, 2 );

	m_bitmap10211 = new wxStaticBitmap( m_panel1311, wxID_ANY, wxBitmap( wxT("dpad_right.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15211->Add( m_bitmap10211, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl611161 = new wxTextCtrl( m_panel1311, ID_BTN_DRIGHT, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer15211->Add( m_textCtrl611161, 0, wxALL|wxEXPAND, 2 );

	gSizer10111->Add( bSizer15211, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer15311;
	bSizer15311 = new wxBoxSizer( wxVERTICAL );

	m_bitmap7311 = new wxStaticBitmap( m_panel1311, wxID_ANY, wxBitmap( wxT("cross.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15311->Add( m_bitmap7311, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl61131 = new wxTextCtrl( m_panel1311, ID_BTN_CROSS, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer15311->Add( m_textCtrl61131, 0, wxALL|wxEXPAND, 2 );

	m_bitmap8311 = new wxStaticBitmap( m_panel1311, wxID_ANY, wxBitmap( wxT("L1.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15311->Add( m_bitmap8311, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl61171 = new wxTextCtrl( m_panel1311, ID_BTN_L1, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer15311->Add( m_textCtrl61171, 0, wxALL|wxEXPAND, 2 );

	m_bitmap9311 = new wxStaticBitmap( m_panel1311, wxID_ANY, wxBitmap( wxT("L3.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15311->Add( m_bitmap9311, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl611131 = new wxTextCtrl( m_panel1311, ID_BTN_L3, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer15311->Add( m_textCtrl611131, 0, wxALL|wxEXPAND, 2 );

	m_bitmap10311 = new wxStaticBitmap( m_panel1311, wxID_ANY, wxBitmap( wxT("dpad_down.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15311->Add( m_bitmap10311, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl611171 = new wxTextCtrl( m_panel1311, ID_BTN_DDOWN, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer15311->Add( m_textCtrl611171, 0, wxALL|wxEXPAND, 2 );

	gSizer10111->Add( bSizer15311, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer15411;
	bSizer15411 = new wxBoxSizer( wxVERTICAL );

	m_bitmap7411 = new wxStaticBitmap( m_panel1311, wxID_ANY, wxBitmap( wxT("square.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15411->Add( m_bitmap7411, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl61141 = new wxTextCtrl( m_panel1311, ID_BTN_SQUARE, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer15411->Add( m_textCtrl61141, 0, wxALL|wxEXPAND, 2 );

	m_bitmap8411 = new wxStaticBitmap( m_panel1311, wxID_ANY, wxBitmap( wxT("R1.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15411->Add( m_bitmap8411, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl61181 = new wxTextCtrl( m_panel1311, ID_BTN_R1, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer15411->Add( m_textCtrl61181, 0, wxALL|wxEXPAND, 2 );

	m_bitmap9411 = new wxStaticBitmap( m_panel1311, wxID_ANY, wxBitmap( wxT("R3.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15411->Add( m_bitmap9411, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl611141 = new wxTextCtrl( m_panel1311, ID_BTN_R3, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer15411->Add( m_textCtrl611141, 0, wxALL|wxEXPAND, 2 );

	m_bitmap10411 = new wxStaticBitmap( m_panel1311, wxID_ANY, wxBitmap( wxT("dpad_left.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15411->Add( m_bitmap10411, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl611181 = new wxTextCtrl( m_panel1311, ID_BTN_DLEFT, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer15411->Add( m_textCtrl611181, 0, wxALL|wxEXPAND, 2 );

	gSizer10111->Add( bSizer15411, 1, wxEXPAND, 5 );

	bSizer911->Add( gSizer10111, 1, wxEXPAND, 5 );

	m_panel1311->SetSizer( bSizer911 );
	m_panel1311->Layout();
	bSizer911->Fit( m_panel1311 );
	m_notebook4->AddPage( m_panel1311, wxT("Player 2"), false );
	m_panel13111 = new wxPanel( m_notebook4, ID_PLAYER_TAB, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer9111;
	bSizer9111 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer10111;
	bSizer10111 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText10111 = new wxStaticText( m_panel13111, wxID_ANY, wxT("Controller Device: "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10111->Wrap( -1 );
	bSizer10111->Add( m_staticText10111, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxArrayString m_choice10111Choices;
	m_choice10111 = new wxChoice( m_panel13111, ID_INPUT_DEVICE, wxDefaultPosition, wxDefaultSize, m_choice10111Choices, 0 );
	m_choice10111->SetSelection( 0 );
	bSizer10111->Add( m_choice10111, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxBoxSizer* bSizer11111;
	bSizer11111 = new wxBoxSizer( wxVERTICAL );

	m_checkBox5111 = new wxCheckBox( m_panel13111, ID_CTRL_PLUGGED, wxT("Controllher Plugged"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );

	bSizer11111->Add( m_checkBox5111, 0, wxALL|wxALIGN_RIGHT, 1 );

	m_checkBox6111 = new wxCheckBox( m_panel13111, ID_CTRL_FFBACK, wxT("Enabled Force Feedback"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );

	bSizer11111->Add( m_checkBox6111, 0, wxALL|wxALIGN_RIGHT, 1 );

	bSizer10111->Add( bSizer11111, 1, wxEXPAND, 5 );

	bSizer9111->Add( bSizer10111, 0, wxEXPAND, 5 );

	m_staticline31111 = new wxStaticLine( m_panel13111, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer9111->Add( m_staticline31111, 0, wxEXPAND | wxALL, 5 );

	wxGridSizer* gSizer101111;
	gSizer101111 = new wxGridSizer( 1, 6, 0, 0 );

	wxBoxSizer* bSizer13111;
	bSizer13111 = new wxBoxSizer( wxVERTICAL );

	m_bitmap3111 = new wxStaticBitmap( m_panel13111, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13111->Add( m_bitmap3111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl6111911 = new wxTextCtrl( m_panel13111, ID_AXIS_LH1, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer13111->Add( m_textCtrl6111911, 0, wxALL|wxEXPAND, 2 );

	m_bitmap4111 = new wxStaticBitmap( m_panel13111, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13111->Add( m_bitmap4111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl6112011 = new wxTextCtrl( m_panel13111, ID_AXIS_LH2, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer13111->Add( m_textCtrl6112011, 0, wxALL|wxEXPAND, 2 );

	m_bitmap5111 = new wxStaticBitmap( m_panel13111, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13111->Add( m_bitmap5111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl6112111 = new wxTextCtrl( m_panel13111, ID_AXIS_LV1, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer13111->Add( m_textCtrl6112111, 0, wxALL|wxEXPAND, 2 );

	m_bitmap6111 = new wxStaticBitmap( m_panel13111, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13111->Add( m_bitmap6111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl6112211 = new wxTextCtrl( m_panel13111, ID_AXIS_LV2, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer13111->Add( m_textCtrl6112211, 0, wxALL|wxEXPAND, 2 );

	gSizer101111->Add( bSizer13111, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer15511;
	bSizer15511 = new wxBoxSizer( wxVERTICAL );

	m_bitmap7511 = new wxStaticBitmap( m_panel13111, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15511->Add( m_bitmap7511, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl6112311 = new wxTextCtrl( m_panel13111, ID_AXIS_RH1, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer15511->Add( m_textCtrl6112311, 0, wxALL|wxEXPAND, 2 );

	m_bitmap8511 = new wxStaticBitmap( m_panel13111, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15511->Add( m_bitmap8511, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl6112411 = new wxTextCtrl( m_panel13111, ID_AXIS_RH2, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer15511->Add( m_textCtrl6112411, 0, wxALL|wxEXPAND, 2 );

	m_bitmap9511 = new wxStaticBitmap( m_panel13111, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15511->Add( m_bitmap9511, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl6112511 = new wxTextCtrl( m_panel13111, ID_AXIS_RV1, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer15511->Add( m_textCtrl6112511, 0, wxALL|wxEXPAND, 2 );

	m_bitmap10511 = new wxStaticBitmap( m_panel13111, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15511->Add( m_bitmap10511, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl6112611 = new wxTextCtrl( m_panel13111, ID_AXIS_RV2, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer15511->Add( m_textCtrl6112611, 0, wxALL|wxEXPAND, 2 );

	gSizer101111->Add( bSizer15511, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer151111;
	bSizer151111 = new wxBoxSizer( wxVERTICAL );

	m_bitmap71111 = new wxStaticBitmap( m_panel13111, wxID_ANY, wxBitmap( wxT("triangle.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer151111->Add( m_bitmap71111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl611271 = new wxTextCtrl( m_panel13111, ID_BTN_TRIANGLE, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer151111->Add( m_textCtrl611271, 0, wxALL|wxEXPAND, 2 );

	m_bitmap81111 = new wxStaticBitmap( m_panel13111, wxID_ANY, wxBitmap( wxT("L2.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer151111->Add( m_bitmap81111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl611511 = new wxTextCtrl( m_panel13111, ID_BTN_L2, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer151111->Add( m_textCtrl611511, 0, wxALL|wxEXPAND, 2 );

	m_bitmap91111 = new wxStaticBitmap( m_panel13111, wxID_ANY, wxBitmap( wxT("select.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer151111->Add( m_bitmap91111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl611911 = new wxTextCtrl( m_panel13111, ID_BTN_SELECT, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer151111->Add( m_textCtrl611911, 0, wxALL|wxEXPAND, 2 );

	m_bitmap101111 = new wxStaticBitmap( m_panel13111, wxID_ANY, wxBitmap( wxT("dpad_up.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer151111->Add( m_bitmap101111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl6111511 = new wxTextCtrl( m_panel13111, ID_BTN_DUP, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer151111->Add( m_textCtrl6111511, 0, wxALL|wxEXPAND, 2 );

	gSizer101111->Add( bSizer151111, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer152111;
	bSizer152111 = new wxBoxSizer( wxVERTICAL );

	m_bitmap72111 = new wxStaticBitmap( m_panel13111, wxID_ANY, wxBitmap( wxT("circle.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer152111->Add( m_bitmap72111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl61111 = new wxTextCtrl( m_panel13111, ID_BTN_CIRCLE, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer152111->Add( m_textCtrl61111, 0, wxALL|wxEXPAND, 2 );

	m_bitmap82111 = new wxStaticBitmap( m_panel13111, wxID_ANY, wxBitmap( wxT("R2.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer152111->Add( m_bitmap82111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl611611 = new wxTextCtrl( m_panel13111, ID_BTN_R2, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer152111->Add( m_textCtrl611611, 0, wxALL|wxEXPAND, 2 );

	m_bitmap92111 = new wxStaticBitmap( m_panel13111, wxID_ANY, wxBitmap( wxT("start.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer152111->Add( m_bitmap92111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl6111011 = new wxTextCtrl( m_panel13111, ID_BTN_START, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer152111->Add( m_textCtrl6111011, 0, wxALL|wxEXPAND, 2 );

	m_bitmap102111 = new wxStaticBitmap( m_panel13111, wxID_ANY, wxBitmap( wxT("dpad_right.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer152111->Add( m_bitmap102111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl6111611 = new wxTextCtrl( m_panel13111, ID_BTN_DRIGHT, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer152111->Add( m_textCtrl6111611, 0, wxALL|wxEXPAND, 2 );

	gSizer101111->Add( bSizer152111, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer153111;
	bSizer153111 = new wxBoxSizer( wxVERTICAL );

	m_bitmap73111 = new wxStaticBitmap( m_panel13111, wxID_ANY, wxBitmap( wxT("cross.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer153111->Add( m_bitmap73111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl611311 = new wxTextCtrl( m_panel13111, ID_BTN_CROSS, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer153111->Add( m_textCtrl611311, 0, wxALL|wxEXPAND, 2 );

	m_bitmap83111 = new wxStaticBitmap( m_panel13111, wxID_ANY, wxBitmap( wxT("L1.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer153111->Add( m_bitmap83111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl611711 = new wxTextCtrl( m_panel13111, ID_BTN_L1, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer153111->Add( m_textCtrl611711, 0, wxALL|wxEXPAND, 2 );

	m_bitmap93111 = new wxStaticBitmap( m_panel13111, wxID_ANY, wxBitmap( wxT("L3.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer153111->Add( m_bitmap93111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl6111311 = new wxTextCtrl( m_panel13111, ID_BTN_L3, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer153111->Add( m_textCtrl6111311, 0, wxALL|wxEXPAND, 2 );

	m_bitmap103111 = new wxStaticBitmap( m_panel13111, wxID_ANY, wxBitmap( wxT("dpad_down.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer153111->Add( m_bitmap103111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl6111711 = new wxTextCtrl( m_panel13111, ID_BTN_DDOWN, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer153111->Add( m_textCtrl6111711, 0, wxALL|wxEXPAND, 2 );

	gSizer101111->Add( bSizer153111, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer154111;
	bSizer154111 = new wxBoxSizer( wxVERTICAL );

	m_bitmap74111 = new wxStaticBitmap( m_panel13111, wxID_ANY, wxBitmap( wxT("square.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer154111->Add( m_bitmap74111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl611411 = new wxTextCtrl( m_panel13111, ID_BTN_SQUARE, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer154111->Add( m_textCtrl611411, 0, wxALL|wxEXPAND, 2 );

	m_bitmap84111 = new wxStaticBitmap( m_panel13111, wxID_ANY, wxBitmap( wxT("R1.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer154111->Add( m_bitmap84111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl611811 = new wxTextCtrl( m_panel13111, ID_BTN_R1, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer154111->Add( m_textCtrl611811, 0, wxALL|wxEXPAND, 2 );

	m_bitmap94111 = new wxStaticBitmap( m_panel13111, wxID_ANY, wxBitmap( wxT("R3.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer154111->Add( m_bitmap94111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl6111411 = new wxTextCtrl( m_panel13111, ID_BTN_R3, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer154111->Add( m_textCtrl6111411, 0, wxALL|wxEXPAND, 2 );

	m_bitmap104111 = new wxStaticBitmap( m_panel13111, wxID_ANY, wxBitmap( wxT("dpad_left.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer154111->Add( m_bitmap104111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl6111811 = new wxTextCtrl( m_panel13111, ID_BTN_DLEFT, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer154111->Add( m_textCtrl6111811, 0, wxALL|wxEXPAND, 2 );

	gSizer101111->Add( bSizer154111, 1, wxEXPAND, 5 );

	bSizer9111->Add( gSizer101111, 1, wxEXPAND, 5 );

	m_panel13111->SetSizer( bSizer9111 );
	m_panel13111->Layout();
	bSizer9111->Fit( m_panel13111 );
	m_notebook4->AddPage( m_panel13111, wxT("Player 3"), false );
	m_panel131111 = new wxPanel( m_notebook4, ID_PLAYER_TAB, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer91111;
	bSizer91111 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer101111;
	bSizer101111 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText101111 = new wxStaticText( m_panel131111, wxID_ANY, wxT("Controller Device: "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText101111->Wrap( -1 );
	bSizer101111->Add( m_staticText101111, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxArrayString m_choice101111Choices;
	m_choice101111 = new wxChoice( m_panel131111, ID_INPUT_DEVICE, wxDefaultPosition, wxDefaultSize, m_choice101111Choices, 0 );
	m_choice101111->SetSelection( 0 );
	bSizer101111->Add( m_choice101111, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxBoxSizer* bSizer111111;
	bSizer111111 = new wxBoxSizer( wxVERTICAL );

	m_checkBox51111 = new wxCheckBox( m_panel131111, ID_CTRL_PLUGGED, wxT("Controllher Plugged"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );

	bSizer111111->Add( m_checkBox51111, 0, wxALL|wxALIGN_RIGHT, 1 );

	m_checkBox61111 = new wxCheckBox( m_panel131111, ID_CTRL_FFBACK, wxT("Enabled Force Feedback"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );

	bSizer111111->Add( m_checkBox61111, 0, wxALL|wxALIGN_RIGHT, 1 );

	bSizer101111->Add( bSizer111111, 1, wxEXPAND, 5 );

	bSizer91111->Add( bSizer101111, 0, wxEXPAND, 5 );

	m_staticline311111 = new wxStaticLine( m_panel131111, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer91111->Add( m_staticline311111, 0, wxEXPAND | wxALL, 5 );

	wxGridSizer* gSizer1011111;
	gSizer1011111 = new wxGridSizer( 1, 6, 0, 0 );

	wxBoxSizer* bSizer131111;
	bSizer131111 = new wxBoxSizer( wxVERTICAL );

	m_bitmap31111 = new wxStaticBitmap( m_panel131111, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer131111->Add( m_bitmap31111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl61119111 = new wxTextCtrl( m_panel131111, ID_AXIS_LH1, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer131111->Add( m_textCtrl61119111, 0, wxALL|wxEXPAND, 2 );

	m_bitmap41111 = new wxStaticBitmap( m_panel131111, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer131111->Add( m_bitmap41111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl61120111 = new wxTextCtrl( m_panel131111, ID_AXIS_LH2, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer131111->Add( m_textCtrl61120111, 0, wxALL|wxEXPAND, 2 );

	m_bitmap51111 = new wxStaticBitmap( m_panel131111, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer131111->Add( m_bitmap51111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl61121111 = new wxTextCtrl( m_panel131111, ID_AXIS_LV1, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer131111->Add( m_textCtrl61121111, 0, wxALL|wxEXPAND, 2 );

	m_bitmap61111 = new wxStaticBitmap( m_panel131111, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer131111->Add( m_bitmap61111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl61122111 = new wxTextCtrl( m_panel131111, ID_AXIS_LV2, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer131111->Add( m_textCtrl61122111, 0, wxALL|wxEXPAND, 2 );

	gSizer1011111->Add( bSizer131111, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer155111;
	bSizer155111 = new wxBoxSizer( wxVERTICAL );

	m_bitmap75111 = new wxStaticBitmap( m_panel131111, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer155111->Add( m_bitmap75111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl61123111 = new wxTextCtrl( m_panel131111, ID_AXIS_RH1, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer155111->Add( m_textCtrl61123111, 0, wxALL|wxEXPAND, 2 );

	m_bitmap85111 = new wxStaticBitmap( m_panel131111, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer155111->Add( m_bitmap85111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl61124111 = new wxTextCtrl( m_panel131111, ID_AXIS_RH2, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer155111->Add( m_textCtrl61124111, 0, wxALL|wxEXPAND, 2 );

	m_bitmap95111 = new wxStaticBitmap( m_panel131111, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer155111->Add( m_bitmap95111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl61125111 = new wxTextCtrl( m_panel131111, ID_AXIS_RV1, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer155111->Add( m_textCtrl61125111, 0, wxALL|wxEXPAND, 2 );

	m_bitmap105111 = new wxStaticBitmap( m_panel131111, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer155111->Add( m_bitmap105111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl61126111 = new wxTextCtrl( m_panel131111, ID_AXIS_RV2, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer155111->Add( m_textCtrl61126111, 0, wxALL|wxEXPAND, 2 );

	gSizer1011111->Add( bSizer155111, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer1511111;
	bSizer1511111 = new wxBoxSizer( wxVERTICAL );

	m_bitmap711111 = new wxStaticBitmap( m_panel131111, wxID_ANY, wxBitmap( wxT("triangle.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1511111->Add( m_bitmap711111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl6112711 = new wxTextCtrl( m_panel131111, ID_BTN_TRIANGLE, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1511111->Add( m_textCtrl6112711, 0, wxALL|wxEXPAND, 2 );

	m_bitmap811111 = new wxStaticBitmap( m_panel131111, wxID_ANY, wxBitmap( wxT("L2.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1511111->Add( m_bitmap811111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl6115111 = new wxTextCtrl( m_panel131111, ID_BTN_L2, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1511111->Add( m_textCtrl6115111, 0, wxALL|wxEXPAND, 2 );

	m_bitmap911111 = new wxStaticBitmap( m_panel131111, wxID_ANY, wxBitmap( wxT("select.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1511111->Add( m_bitmap911111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl6119111 = new wxTextCtrl( m_panel131111, ID_BTN_SELECT, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1511111->Add( m_textCtrl6119111, 0, wxALL|wxEXPAND, 2 );

	m_bitmap1011111 = new wxStaticBitmap( m_panel131111, wxID_ANY, wxBitmap( wxT("dpad_up.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1511111->Add( m_bitmap1011111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl61115111 = new wxTextCtrl( m_panel131111, ID_BTN_DUP, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1511111->Add( m_textCtrl61115111, 0, wxALL|wxEXPAND, 2 );

	gSizer1011111->Add( bSizer1511111, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer1521111;
	bSizer1521111 = new wxBoxSizer( wxVERTICAL );

	m_bitmap721111 = new wxStaticBitmap( m_panel131111, wxID_ANY, wxBitmap( wxT("circle.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1521111->Add( m_bitmap721111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl611111 = new wxTextCtrl( m_panel131111, ID_BTN_CIRCLE, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1521111->Add( m_textCtrl611111, 0, wxALL|wxEXPAND, 2 );

	m_bitmap821111 = new wxStaticBitmap( m_panel131111, wxID_ANY, wxBitmap( wxT("R2.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1521111->Add( m_bitmap821111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl6116111 = new wxTextCtrl( m_panel131111, ID_BTN_R2, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1521111->Add( m_textCtrl6116111, 0, wxALL|wxEXPAND, 2 );

	m_bitmap921111 = new wxStaticBitmap( m_panel131111, wxID_ANY, wxBitmap( wxT("start.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1521111->Add( m_bitmap921111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl61110111 = new wxTextCtrl( m_panel131111, ID_BTN_START, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1521111->Add( m_textCtrl61110111, 0, wxALL|wxEXPAND, 2 );

	m_bitmap1021111 = new wxStaticBitmap( m_panel131111, wxID_ANY, wxBitmap( wxT("dpad_right.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1521111->Add( m_bitmap1021111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl61116111 = new wxTextCtrl( m_panel131111, ID_BTN_DRIGHT, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1521111->Add( m_textCtrl61116111, 0, wxALL|wxEXPAND, 2 );

	gSizer1011111->Add( bSizer1521111, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer1531111;
	bSizer1531111 = new wxBoxSizer( wxVERTICAL );

	m_bitmap731111 = new wxStaticBitmap( m_panel131111, wxID_ANY, wxBitmap( wxT("cross.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1531111->Add( m_bitmap731111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl6113111 = new wxTextCtrl( m_panel131111, ID_BTN_CROSS, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1531111->Add( m_textCtrl6113111, 0, wxALL|wxEXPAND, 2 );

	m_bitmap831111 = new wxStaticBitmap( m_panel131111, wxID_ANY, wxBitmap( wxT("L1.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1531111->Add( m_bitmap831111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl6117111 = new wxTextCtrl( m_panel131111, ID_BTN_L1, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1531111->Add( m_textCtrl6117111, 0, wxALL|wxEXPAND, 2 );

	m_bitmap931111 = new wxStaticBitmap( m_panel131111, wxID_ANY, wxBitmap( wxT("L3.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1531111->Add( m_bitmap931111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl61113111 = new wxTextCtrl( m_panel131111, ID_BTN_L3, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1531111->Add( m_textCtrl61113111, 0, wxALL|wxEXPAND, 2 );

	m_bitmap1031111 = new wxStaticBitmap( m_panel131111, wxID_ANY, wxBitmap( wxT("dpad_down.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1531111->Add( m_bitmap1031111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl61117111 = new wxTextCtrl( m_panel131111, ID_BTN_DDOWN, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1531111->Add( m_textCtrl61117111, 0, wxALL|wxEXPAND, 2 );

	gSizer1011111->Add( bSizer1531111, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer1541111;
	bSizer1541111 = new wxBoxSizer( wxVERTICAL );

	m_bitmap741111 = new wxStaticBitmap( m_panel131111, wxID_ANY, wxBitmap( wxT("square.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1541111->Add( m_bitmap741111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl6114111 = new wxTextCtrl( m_panel131111, ID_BTN_SQUARE, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1541111->Add( m_textCtrl6114111, 0, wxALL|wxEXPAND, 2 );

	m_bitmap841111 = new wxStaticBitmap( m_panel131111, wxID_ANY, wxBitmap( wxT("R1.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1541111->Add( m_bitmap841111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl6118111 = new wxTextCtrl( m_panel131111, ID_BTN_R1, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1541111->Add( m_textCtrl6118111, 0, wxALL|wxEXPAND, 2 );

	m_bitmap941111 = new wxStaticBitmap( m_panel131111, wxID_ANY, wxBitmap( wxT("R3.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1541111->Add( m_bitmap941111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl61114111 = new wxTextCtrl( m_panel131111, ID_BTN_R3, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1541111->Add( m_textCtrl61114111, 0, wxALL|wxEXPAND, 2 );

	m_bitmap1041111 = new wxStaticBitmap( m_panel131111, wxID_ANY, wxBitmap( wxT("dpad_left.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1541111->Add( m_bitmap1041111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );

	m_textCtrl61118111 = new wxTextCtrl( m_panel131111, ID_BTN_DLEFT, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1541111->Add( m_textCtrl61118111, 0, wxALL|wxEXPAND, 2 );

	gSizer1011111->Add( bSizer1541111, 1, wxEXPAND, 5 );

	bSizer91111->Add( gSizer1011111, 1, wxEXPAND, 5 );

	m_panel131111->SetSizer( bSizer91111 );
	m_panel131111->Layout();
	bSizer91111->Fit( m_panel131111 );
	m_notebook4->AddPage( m_panel131111, wxT("Player 4"), false );

	InputBase_sizer->Add( m_notebook4, 1, wxEXPAND | wxALL, 5 );

	InputTab->SetSizer( InputBase_sizer );
	InputTab->Layout();
	InputBase_sizer->Fit( InputTab );
	SectionTabs->AddPage( InputTab, wxT("Input"), true );

	bSizer2->Add( SectionTabs, 1, wxALL|wxEXPAND, 5 );

	BaseSizer->Add( bSizer2, 1, wxEXPAND, 5 );

	DialogButtons = new wxStdDialogButtonSizer();
	DialogButtonsOK = new wxButton( this, wxID_OK );
	DialogButtons->AddButton( DialogButtonsOK );
	DialogButtonsApply = new wxButton( this, wxID_APPLY );
	DialogButtons->AddButton( DialogButtonsApply );
	DialogButtonsCancel = new wxButton( this, wxID_CANCEL );
	DialogButtons->AddButton( DialogButtonsCancel );
	DialogButtons->Realize();
	BaseSizer->Add( DialogButtons, 1, wxALIGN_BOTTOM|wxALIGN_RIGHT|wxALL, 5 );

	this->SetSizer( BaseSizer );
	this->Layout();

	this->Centre( wxBOTH );
}

ConfigWindow::~ConfigWindow()
{
}
