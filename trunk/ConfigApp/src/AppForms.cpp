/*-----------------------------------------------------------------------------
This source file is part of Sonetto RPG Engine.

Copyright (C) 2007,2008 Arthur Carvalho de Souza Lima, Guilherme Prá Vieira


Sonetto RPG Engine is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Sonetto RPG Engine is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this library; if not, write to the Free Software Foundation,
Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA or go to
http://www.gnu.org/copyleft/lesser.txt
-----------------------------------------------------------------------------*/

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

	Logo = new wxStaticBitmap( this, wxID_ANY, wxBitmap( wxT("logo.bmp"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxSize( 500,50 ), 0 );
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
	m_choicebook3->AddPage( mD3D9Panel, wxT("Direct3D 9"), false );
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
	m_choicebook3->AddPage( mOGLPanel, wxT("OpenGL"), true );
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

	m_textCtrl11 = new wxTextCtrl( m_panel131, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer131->Add( m_textCtrl11, 0, wxALL|wxEXPAND, 2 );

	m_bitmap41 = new wxStaticBitmap( m_panel131, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer131->Add( m_bitmap41, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl21 = new wxTextCtrl( m_panel131, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer131->Add( m_textCtrl21, 0, wxALL|wxEXPAND, 2 );

	m_bitmap51 = new wxStaticBitmap( m_panel131, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer131->Add( m_bitmap51, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl31 = new wxTextCtrl( m_panel131, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer131->Add( m_textCtrl31, 0, wxALL|wxEXPAND, 2 );

	m_bitmap61 = new wxStaticBitmap( m_panel131, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer131->Add( m_bitmap61, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl41 = new wxTextCtrl( m_panel131, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer131->Add( m_textCtrl41, 0, wxALL|wxEXPAND, 2 );

	gSizer1011->Add( bSizer131, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer155;
	bSizer155 = new wxBoxSizer( wxVERTICAL );

	m_bitmap75 = new wxStaticBitmap( m_panel131, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer155->Add( m_bitmap75, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl55 = new wxTextCtrl( m_panel131, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer155->Add( m_textCtrl55, 0, wxALL|wxEXPAND, 2 );

	m_bitmap85 = new wxStaticBitmap( m_panel131, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer155->Add( m_bitmap85, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl65 = new wxTextCtrl( m_panel131, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer155->Add( m_textCtrl65, 0, wxALL|wxEXPAND, 2 );

	m_bitmap95 = new wxStaticBitmap( m_panel131, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer155->Add( m_bitmap95, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl75 = new wxTextCtrl( m_panel131, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer155->Add( m_textCtrl75, 0, wxALL|wxEXPAND, 2 );

	m_bitmap105 = new wxStaticBitmap( m_panel131, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer155->Add( m_bitmap105, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl85 = new wxTextCtrl( m_panel131, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer155->Add( m_textCtrl85, 0, wxALL|wxEXPAND, 2 );

	gSizer1011->Add( bSizer155, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer1511;
	bSizer1511 = new wxBoxSizer( wxVERTICAL );

	m_bitmap711 = new wxStaticBitmap( m_panel131, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1511->Add( m_bitmap711, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl511 = new wxTextCtrl( m_panel131, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1511->Add( m_textCtrl511, 0, wxALL|wxEXPAND, 2 );

	m_bitmap811 = new wxStaticBitmap( m_panel131, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1511->Add( m_bitmap811, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl611 = new wxTextCtrl( m_panel131, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1511->Add( m_textCtrl611, 0, wxALL|wxEXPAND, 2 );

	m_bitmap911 = new wxStaticBitmap( m_panel131, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1511->Add( m_bitmap911, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl711 = new wxTextCtrl( m_panel131, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1511->Add( m_textCtrl711, 0, wxALL|wxEXPAND, 2 );

	m_bitmap1011 = new wxStaticBitmap( m_panel131, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1511->Add( m_bitmap1011, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl811 = new wxTextCtrl( m_panel131, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1511->Add( m_textCtrl811, 0, wxALL|wxEXPAND, 2 );

	gSizer1011->Add( bSizer1511, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer1521;
	bSizer1521 = new wxBoxSizer( wxVERTICAL );

	m_bitmap721 = new wxStaticBitmap( m_panel131, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1521->Add( m_bitmap721, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl521 = new wxTextCtrl( m_panel131, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1521->Add( m_textCtrl521, 0, wxALL|wxEXPAND, 2 );

	m_bitmap821 = new wxStaticBitmap( m_panel131, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1521->Add( m_bitmap821, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl621 = new wxTextCtrl( m_panel131, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1521->Add( m_textCtrl621, 0, wxALL|wxEXPAND, 2 );

	m_bitmap921 = new wxStaticBitmap( m_panel131, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1521->Add( m_bitmap921, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl721 = new wxTextCtrl( m_panel131, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1521->Add( m_textCtrl721, 0, wxALL|wxEXPAND, 2 );

	m_bitmap1021 = new wxStaticBitmap( m_panel131, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1521->Add( m_bitmap1021, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl821 = new wxTextCtrl( m_panel131, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1521->Add( m_textCtrl821, 0, wxALL|wxEXPAND, 2 );

	gSizer1011->Add( bSizer1521, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer1531;
	bSizer1531 = new wxBoxSizer( wxVERTICAL );

	m_bitmap731 = new wxStaticBitmap( m_panel131, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1531->Add( m_bitmap731, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl531 = new wxTextCtrl( m_panel131, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1531->Add( m_textCtrl531, 0, wxALL|wxEXPAND, 2 );

	m_bitmap831 = new wxStaticBitmap( m_panel131, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1531->Add( m_bitmap831, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl631 = new wxTextCtrl( m_panel131, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1531->Add( m_textCtrl631, 0, wxALL|wxEXPAND, 2 );

	m_bitmap931 = new wxStaticBitmap( m_panel131, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1531->Add( m_bitmap931, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl731 = new wxTextCtrl( m_panel131, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1531->Add( m_textCtrl731, 0, wxALL|wxEXPAND, 2 );

	m_bitmap1031 = new wxStaticBitmap( m_panel131, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1531->Add( m_bitmap1031, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl831 = new wxTextCtrl( m_panel131, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1531->Add( m_textCtrl831, 0, wxALL|wxEXPAND, 2 );

	gSizer1011->Add( bSizer1531, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer1541;
	bSizer1541 = new wxBoxSizer( wxVERTICAL );

	m_bitmap741 = new wxStaticBitmap( m_panel131, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1541->Add( m_bitmap741, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl541 = new wxTextCtrl( m_panel131, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1541->Add( m_textCtrl541, 0, wxALL|wxEXPAND, 2 );

	m_bitmap841 = new wxStaticBitmap( m_panel131, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1541->Add( m_bitmap841, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl641 = new wxTextCtrl( m_panel131, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1541->Add( m_textCtrl641, 0, wxALL|wxEXPAND, 2 );

	m_bitmap941 = new wxStaticBitmap( m_panel131, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1541->Add( m_bitmap941, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl741 = new wxTextCtrl( m_panel131, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1541->Add( m_textCtrl741, 0, wxALL|wxEXPAND, 2 );

	m_bitmap1041 = new wxStaticBitmap( m_panel131, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1541->Add( m_bitmap1041, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl841 = new wxTextCtrl( m_panel131, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1541->Add( m_textCtrl841, 0, wxALL|wxEXPAND, 2 );

	gSizer1011->Add( bSizer1541, 1, wxEXPAND, 5 );

	bSizer91->Add( gSizer1011, 1, wxEXPAND, 5 );

	m_panel131->SetSizer( bSizer91 );
	m_panel131->Layout();
	bSizer91->Fit( m_panel131 );
	m_notebook4->AddPage( m_panel131, wxT("Player 1"), true );
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

	m_textCtrl111 = new wxTextCtrl( m_panel1311, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1311->Add( m_textCtrl111, 0, wxALL|wxEXPAND, 2 );

	m_bitmap411 = new wxStaticBitmap( m_panel1311, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1311->Add( m_bitmap411, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl211 = new wxTextCtrl( m_panel1311, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1311->Add( m_textCtrl211, 0, wxALL|wxEXPAND, 2 );

	m_bitmap511 = new wxStaticBitmap( m_panel1311, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1311->Add( m_bitmap511, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl311 = new wxTextCtrl( m_panel1311, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1311->Add( m_textCtrl311, 0, wxALL|wxEXPAND, 2 );

	m_bitmap611 = new wxStaticBitmap( m_panel1311, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1311->Add( m_bitmap611, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl411 = new wxTextCtrl( m_panel1311, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1311->Add( m_textCtrl411, 0, wxALL|wxEXPAND, 2 );

	gSizer10111->Add( bSizer1311, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer1551;
	bSizer1551 = new wxBoxSizer( wxVERTICAL );

	m_bitmap751 = new wxStaticBitmap( m_panel1311, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1551->Add( m_bitmap751, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl551 = new wxTextCtrl( m_panel1311, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1551->Add( m_textCtrl551, 0, wxALL|wxEXPAND, 2 );

	m_bitmap851 = new wxStaticBitmap( m_panel1311, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1551->Add( m_bitmap851, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl651 = new wxTextCtrl( m_panel1311, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1551->Add( m_textCtrl651, 0, wxALL|wxEXPAND, 2 );

	m_bitmap951 = new wxStaticBitmap( m_panel1311, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1551->Add( m_bitmap951, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl751 = new wxTextCtrl( m_panel1311, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1551->Add( m_textCtrl751, 0, wxALL|wxEXPAND, 2 );

	m_bitmap1051 = new wxStaticBitmap( m_panel1311, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1551->Add( m_bitmap1051, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl851 = new wxTextCtrl( m_panel1311, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer1551->Add( m_textCtrl851, 0, wxALL|wxEXPAND, 2 );

	gSizer10111->Add( bSizer1551, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer15111;
	bSizer15111 = new wxBoxSizer( wxVERTICAL );

	m_bitmap7111 = new wxStaticBitmap( m_panel1311, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15111->Add( m_bitmap7111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl5111 = new wxTextCtrl( m_panel1311, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer15111->Add( m_textCtrl5111, 0, wxALL|wxEXPAND, 2 );

	m_bitmap8111 = new wxStaticBitmap( m_panel1311, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15111->Add( m_bitmap8111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl6111 = new wxTextCtrl( m_panel1311, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer15111->Add( m_textCtrl6111, 0, wxALL|wxEXPAND, 2 );

	m_bitmap9111 = new wxStaticBitmap( m_panel1311, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15111->Add( m_bitmap9111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl7111 = new wxTextCtrl( m_panel1311, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer15111->Add( m_textCtrl7111, 0, wxALL|wxEXPAND, 2 );

	m_bitmap10111 = new wxStaticBitmap( m_panel1311, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15111->Add( m_bitmap10111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl8111 = new wxTextCtrl( m_panel1311, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer15111->Add( m_textCtrl8111, 0, wxALL|wxEXPAND, 2 );

	gSizer10111->Add( bSizer15111, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer15211;
	bSizer15211 = new wxBoxSizer( wxVERTICAL );

	m_bitmap7211 = new wxStaticBitmap( m_panel1311, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15211->Add( m_bitmap7211, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl5211 = new wxTextCtrl( m_panel1311, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer15211->Add( m_textCtrl5211, 0, wxALL|wxEXPAND, 2 );

	m_bitmap8211 = new wxStaticBitmap( m_panel1311, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15211->Add( m_bitmap8211, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl6211 = new wxTextCtrl( m_panel1311, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer15211->Add( m_textCtrl6211, 0, wxALL|wxEXPAND, 2 );

	m_bitmap9211 = new wxStaticBitmap( m_panel1311, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15211->Add( m_bitmap9211, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl7211 = new wxTextCtrl( m_panel1311, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer15211->Add( m_textCtrl7211, 0, wxALL|wxEXPAND, 2 );

	m_bitmap10211 = new wxStaticBitmap( m_panel1311, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15211->Add( m_bitmap10211, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl8211 = new wxTextCtrl( m_panel1311, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer15211->Add( m_textCtrl8211, 0, wxALL|wxEXPAND, 2 );

	gSizer10111->Add( bSizer15211, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer15311;
	bSizer15311 = new wxBoxSizer( wxVERTICAL );

	m_bitmap7311 = new wxStaticBitmap( m_panel1311, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15311->Add( m_bitmap7311, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl5311 = new wxTextCtrl( m_panel1311, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer15311->Add( m_textCtrl5311, 0, wxALL|wxEXPAND, 2 );

	m_bitmap8311 = new wxStaticBitmap( m_panel1311, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15311->Add( m_bitmap8311, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl6311 = new wxTextCtrl( m_panel1311, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer15311->Add( m_textCtrl6311, 0, wxALL|wxEXPAND, 2 );

	m_bitmap9311 = new wxStaticBitmap( m_panel1311, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15311->Add( m_bitmap9311, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl7311 = new wxTextCtrl( m_panel1311, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer15311->Add( m_textCtrl7311, 0, wxALL|wxEXPAND, 2 );

	m_bitmap10311 = new wxStaticBitmap( m_panel1311, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15311->Add( m_bitmap10311, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl8311 = new wxTextCtrl( m_panel1311, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer15311->Add( m_textCtrl8311, 0, wxALL|wxEXPAND, 2 );

	gSizer10111->Add( bSizer15311, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer15411;
	bSizer15411 = new wxBoxSizer( wxVERTICAL );

	m_bitmap7411 = new wxStaticBitmap( m_panel1311, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15411->Add( m_bitmap7411, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl5411 = new wxTextCtrl( m_panel1311, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer15411->Add( m_textCtrl5411, 0, wxALL|wxEXPAND, 2 );

	m_bitmap8411 = new wxStaticBitmap( m_panel1311, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15411->Add( m_bitmap8411, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl6411 = new wxTextCtrl( m_panel1311, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer15411->Add( m_textCtrl6411, 0, wxALL|wxEXPAND, 2 );

	m_bitmap9411 = new wxStaticBitmap( m_panel1311, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15411->Add( m_bitmap9411, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl7411 = new wxTextCtrl( m_panel1311, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer15411->Add( m_textCtrl7411, 0, wxALL|wxEXPAND, 2 );

	m_bitmap10411 = new wxStaticBitmap( m_panel1311, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15411->Add( m_bitmap10411, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl8411 = new wxTextCtrl( m_panel1311, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer15411->Add( m_textCtrl8411, 0, wxALL|wxEXPAND, 2 );

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

	m_textCtrl1111 = new wxTextCtrl( m_panel13111, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer13111->Add( m_textCtrl1111, 0, wxALL|wxEXPAND, 2 );

	m_bitmap4111 = new wxStaticBitmap( m_panel13111, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13111->Add( m_bitmap4111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl2111 = new wxTextCtrl( m_panel13111, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer13111->Add( m_textCtrl2111, 0, wxALL|wxEXPAND, 2 );

	m_bitmap5111 = new wxStaticBitmap( m_panel13111, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13111->Add( m_bitmap5111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl3111 = new wxTextCtrl( m_panel13111, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer13111->Add( m_textCtrl3111, 0, wxALL|wxEXPAND, 2 );

	m_bitmap6111 = new wxStaticBitmap( m_panel13111, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13111->Add( m_bitmap6111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl4111 = new wxTextCtrl( m_panel13111, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer13111->Add( m_textCtrl4111, 0, wxALL|wxEXPAND, 2 );

	gSizer101111->Add( bSizer13111, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer15511;
	bSizer15511 = new wxBoxSizer( wxVERTICAL );

	m_bitmap7511 = new wxStaticBitmap( m_panel13111, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15511->Add( m_bitmap7511, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl5511 = new wxTextCtrl( m_panel13111, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer15511->Add( m_textCtrl5511, 0, wxALL|wxEXPAND, 2 );

	m_bitmap8511 = new wxStaticBitmap( m_panel13111, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15511->Add( m_bitmap8511, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl6511 = new wxTextCtrl( m_panel13111, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer15511->Add( m_textCtrl6511, 0, wxALL|wxEXPAND, 2 );

	m_bitmap9511 = new wxStaticBitmap( m_panel13111, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15511->Add( m_bitmap9511, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl7511 = new wxTextCtrl( m_panel13111, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer15511->Add( m_textCtrl7511, 0, wxALL|wxEXPAND, 2 );

	m_bitmap10511 = new wxStaticBitmap( m_panel13111, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15511->Add( m_bitmap10511, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl8511 = new wxTextCtrl( m_panel13111, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer15511->Add( m_textCtrl8511, 0, wxALL|wxEXPAND, 2 );

	gSizer101111->Add( bSizer15511, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer151111;
	bSizer151111 = new wxBoxSizer( wxVERTICAL );

	m_bitmap71111 = new wxStaticBitmap( m_panel13111, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer151111->Add( m_bitmap71111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl51111 = new wxTextCtrl( m_panel13111, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer151111->Add( m_textCtrl51111, 0, wxALL|wxEXPAND, 2 );

	m_bitmap81111 = new wxStaticBitmap( m_panel13111, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer151111->Add( m_bitmap81111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl61111 = new wxTextCtrl( m_panel13111, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer151111->Add( m_textCtrl61111, 0, wxALL|wxEXPAND, 2 );

	m_bitmap91111 = new wxStaticBitmap( m_panel13111, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer151111->Add( m_bitmap91111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl71111 = new wxTextCtrl( m_panel13111, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer151111->Add( m_textCtrl71111, 0, wxALL|wxEXPAND, 2 );

	m_bitmap101111 = new wxStaticBitmap( m_panel13111, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer151111->Add( m_bitmap101111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl81111 = new wxTextCtrl( m_panel13111, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer151111->Add( m_textCtrl81111, 0, wxALL|wxEXPAND, 2 );

	gSizer101111->Add( bSizer151111, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer152111;
	bSizer152111 = new wxBoxSizer( wxVERTICAL );

	m_bitmap72111 = new wxStaticBitmap( m_panel13111, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer152111->Add( m_bitmap72111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl52111 = new wxTextCtrl( m_panel13111, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer152111->Add( m_textCtrl52111, 0, wxALL|wxEXPAND, 2 );

	m_bitmap82111 = new wxStaticBitmap( m_panel13111, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer152111->Add( m_bitmap82111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl62111 = new wxTextCtrl( m_panel13111, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer152111->Add( m_textCtrl62111, 0, wxALL|wxEXPAND, 2 );

	m_bitmap92111 = new wxStaticBitmap( m_panel13111, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer152111->Add( m_bitmap92111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl72111 = new wxTextCtrl( m_panel13111, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer152111->Add( m_textCtrl72111, 0, wxALL|wxEXPAND, 2 );

	m_bitmap102111 = new wxStaticBitmap( m_panel13111, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer152111->Add( m_bitmap102111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl82111 = new wxTextCtrl( m_panel13111, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer152111->Add( m_textCtrl82111, 0, wxALL|wxEXPAND, 2 );

	gSizer101111->Add( bSizer152111, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer153111;
	bSizer153111 = new wxBoxSizer( wxVERTICAL );

	m_bitmap73111 = new wxStaticBitmap( m_panel13111, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer153111->Add( m_bitmap73111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl53111 = new wxTextCtrl( m_panel13111, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer153111->Add( m_textCtrl53111, 0, wxALL|wxEXPAND, 2 );

	m_bitmap83111 = new wxStaticBitmap( m_panel13111, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer153111->Add( m_bitmap83111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl63111 = new wxTextCtrl( m_panel13111, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer153111->Add( m_textCtrl63111, 0, wxALL|wxEXPAND, 2 );

	m_bitmap93111 = new wxStaticBitmap( m_panel13111, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer153111->Add( m_bitmap93111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl73111 = new wxTextCtrl( m_panel13111, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer153111->Add( m_textCtrl73111, 0, wxALL|wxEXPAND, 2 );

	m_bitmap103111 = new wxStaticBitmap( m_panel13111, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer153111->Add( m_bitmap103111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl83111 = new wxTextCtrl( m_panel13111, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer153111->Add( m_textCtrl83111, 0, wxALL|wxEXPAND, 2 );

	gSizer101111->Add( bSizer153111, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer154111;
	bSizer154111 = new wxBoxSizer( wxVERTICAL );

	m_bitmap74111 = new wxStaticBitmap( m_panel13111, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer154111->Add( m_bitmap74111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl54111 = new wxTextCtrl( m_panel13111, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer154111->Add( m_textCtrl54111, 0, wxALL|wxEXPAND, 2 );

	m_bitmap84111 = new wxStaticBitmap( m_panel13111, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer154111->Add( m_bitmap84111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl64111 = new wxTextCtrl( m_panel13111, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer154111->Add( m_textCtrl64111, 0, wxALL|wxEXPAND, 2 );

	m_bitmap94111 = new wxStaticBitmap( m_panel13111, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer154111->Add( m_bitmap94111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl74111 = new wxTextCtrl( m_panel13111, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer154111->Add( m_textCtrl74111, 0, wxALL|wxEXPAND, 2 );

	m_bitmap104111 = new wxStaticBitmap( m_panel13111, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer154111->Add( m_bitmap104111, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl84111 = new wxTextCtrl( m_panel13111, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer154111->Add( m_textCtrl84111, 0, wxALL|wxEXPAND, 2 );

	gSizer101111->Add( bSizer154111, 1, wxEXPAND, 5 );

	bSizer9111->Add( gSizer101111, 1, wxEXPAND, 5 );

	m_panel13111->SetSizer( bSizer9111 );
	m_panel13111->Layout();
	bSizer9111->Fit( m_panel13111 );
	m_notebook4->AddPage( m_panel13111, wxT("Player 3"), false );
	m_panel13112 = new wxPanel( m_notebook4, ID_PLAYER_TAB, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer9112;
	bSizer9112 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer10112;
	bSizer10112 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText10112 = new wxStaticText( m_panel13112, wxID_ANY, wxT("Controller Device: "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10112->Wrap( -1 );
	bSizer10112->Add( m_staticText10112, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxArrayString m_choice10112Choices;
	m_choice10112 = new wxChoice( m_panel13112, ID_INPUT_DEVICE, wxDefaultPosition, wxDefaultSize, m_choice10112Choices, 0 );
	m_choice10112->SetSelection( 0 );
	bSizer10112->Add( m_choice10112, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxBoxSizer* bSizer11112;
	bSizer11112 = new wxBoxSizer( wxVERTICAL );

	m_checkBox5112 = new wxCheckBox( m_panel13112, ID_CTRL_PLUGGED, wxT("Controllher Plugged"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );

	bSizer11112->Add( m_checkBox5112, 0, wxALL|wxALIGN_RIGHT, 1 );

	m_checkBox6112 = new wxCheckBox( m_panel13112, ID_CTRL_FFBACK, wxT("Enabled Force Feedback"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );

	bSizer11112->Add( m_checkBox6112, 0, wxALL|wxALIGN_RIGHT, 1 );

	bSizer10112->Add( bSizer11112, 1, wxEXPAND, 5 );

	bSizer9112->Add( bSizer10112, 0, wxEXPAND, 5 );

	m_staticline31112 = new wxStaticLine( m_panel13112, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer9112->Add( m_staticline31112, 0, wxEXPAND | wxALL, 5 );

	wxGridSizer* gSizer101112;
	gSizer101112 = new wxGridSizer( 1, 6, 0, 0 );

	wxBoxSizer* bSizer13112;
	bSizer13112 = new wxBoxSizer( wxVERTICAL );

	m_bitmap3112 = new wxStaticBitmap( m_panel13112, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13112->Add( m_bitmap3112, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl1112 = new wxTextCtrl( m_panel13112, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer13112->Add( m_textCtrl1112, 0, wxALL|wxEXPAND, 2 );

	m_bitmap4112 = new wxStaticBitmap( m_panel13112, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13112->Add( m_bitmap4112, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl2112 = new wxTextCtrl( m_panel13112, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer13112->Add( m_textCtrl2112, 0, wxALL|wxEXPAND, 2 );

	m_bitmap5112 = new wxStaticBitmap( m_panel13112, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13112->Add( m_bitmap5112, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl3112 = new wxTextCtrl( m_panel13112, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer13112->Add( m_textCtrl3112, 0, wxALL|wxEXPAND, 2 );

	m_bitmap6112 = new wxStaticBitmap( m_panel13112, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13112->Add( m_bitmap6112, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl4112 = new wxTextCtrl( m_panel13112, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer13112->Add( m_textCtrl4112, 0, wxALL|wxEXPAND, 2 );

	gSizer101112->Add( bSizer13112, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer15512;
	bSizer15512 = new wxBoxSizer( wxVERTICAL );

	m_bitmap7512 = new wxStaticBitmap( m_panel13112, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15512->Add( m_bitmap7512, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl5512 = new wxTextCtrl( m_panel13112, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer15512->Add( m_textCtrl5512, 0, wxALL|wxEXPAND, 2 );

	m_bitmap8512 = new wxStaticBitmap( m_panel13112, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15512->Add( m_bitmap8512, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl6512 = new wxTextCtrl( m_panel13112, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer15512->Add( m_textCtrl6512, 0, wxALL|wxEXPAND, 2 );

	m_bitmap9512 = new wxStaticBitmap( m_panel13112, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15512->Add( m_bitmap9512, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl7512 = new wxTextCtrl( m_panel13112, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer15512->Add( m_textCtrl7512, 0, wxALL|wxEXPAND, 2 );

	m_bitmap10512 = new wxStaticBitmap( m_panel13112, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15512->Add( m_bitmap10512, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl8512 = new wxTextCtrl( m_panel13112, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer15512->Add( m_textCtrl8512, 0, wxALL|wxEXPAND, 2 );

	gSizer101112->Add( bSizer15512, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer151112;
	bSizer151112 = new wxBoxSizer( wxVERTICAL );

	m_bitmap71112 = new wxStaticBitmap( m_panel13112, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer151112->Add( m_bitmap71112, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl51112 = new wxTextCtrl( m_panel13112, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer151112->Add( m_textCtrl51112, 0, wxALL|wxEXPAND, 2 );

	m_bitmap81112 = new wxStaticBitmap( m_panel13112, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer151112->Add( m_bitmap81112, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl61112 = new wxTextCtrl( m_panel13112, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer151112->Add( m_textCtrl61112, 0, wxALL|wxEXPAND, 2 );

	m_bitmap91112 = new wxStaticBitmap( m_panel13112, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer151112->Add( m_bitmap91112, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl71112 = new wxTextCtrl( m_panel13112, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer151112->Add( m_textCtrl71112, 0, wxALL|wxEXPAND, 2 );

	m_bitmap101112 = new wxStaticBitmap( m_panel13112, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer151112->Add( m_bitmap101112, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl81112 = new wxTextCtrl( m_panel13112, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer151112->Add( m_textCtrl81112, 0, wxALL|wxEXPAND, 2 );

	gSizer101112->Add( bSizer151112, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer152112;
	bSizer152112 = new wxBoxSizer( wxVERTICAL );

	m_bitmap72112 = new wxStaticBitmap( m_panel13112, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer152112->Add( m_bitmap72112, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl52112 = new wxTextCtrl( m_panel13112, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer152112->Add( m_textCtrl52112, 0, wxALL|wxEXPAND, 2 );

	m_bitmap82112 = new wxStaticBitmap( m_panel13112, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer152112->Add( m_bitmap82112, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl62112 = new wxTextCtrl( m_panel13112, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer152112->Add( m_textCtrl62112, 0, wxALL|wxEXPAND, 2 );

	m_bitmap92112 = new wxStaticBitmap( m_panel13112, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer152112->Add( m_bitmap92112, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl72112 = new wxTextCtrl( m_panel13112, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer152112->Add( m_textCtrl72112, 0, wxALL|wxEXPAND, 2 );

	m_bitmap102112 = new wxStaticBitmap( m_panel13112, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer152112->Add( m_bitmap102112, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl82112 = new wxTextCtrl( m_panel13112, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer152112->Add( m_textCtrl82112, 0, wxALL|wxEXPAND, 2 );

	gSizer101112->Add( bSizer152112, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer153112;
	bSizer153112 = new wxBoxSizer( wxVERTICAL );

	m_bitmap73112 = new wxStaticBitmap( m_panel13112, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer153112->Add( m_bitmap73112, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl53112 = new wxTextCtrl( m_panel13112, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer153112->Add( m_textCtrl53112, 0, wxALL|wxEXPAND, 2 );

	m_bitmap83112 = new wxStaticBitmap( m_panel13112, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer153112->Add( m_bitmap83112, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl63112 = new wxTextCtrl( m_panel13112, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer153112->Add( m_textCtrl63112, 0, wxALL|wxEXPAND, 2 );

	m_bitmap93112 = new wxStaticBitmap( m_panel13112, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer153112->Add( m_bitmap93112, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl73112 = new wxTextCtrl( m_panel13112, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer153112->Add( m_textCtrl73112, 0, wxALL|wxEXPAND, 2 );

	m_bitmap103112 = new wxStaticBitmap( m_panel13112, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer153112->Add( m_bitmap103112, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl83112 = new wxTextCtrl( m_panel13112, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer153112->Add( m_textCtrl83112, 0, wxALL|wxEXPAND, 2 );

	gSizer101112->Add( bSizer153112, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer154112;
	bSizer154112 = new wxBoxSizer( wxVERTICAL );

	m_bitmap74112 = new wxStaticBitmap( m_panel13112, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer154112->Add( m_bitmap74112, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl54112 = new wxTextCtrl( m_panel13112, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer154112->Add( m_textCtrl54112, 0, wxALL|wxEXPAND, 2 );

	m_bitmap84112 = new wxStaticBitmap( m_panel13112, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer154112->Add( m_bitmap84112, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl64112 = new wxTextCtrl( m_panel13112, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer154112->Add( m_textCtrl64112, 0, wxALL|wxEXPAND, 2 );

	m_bitmap94112 = new wxStaticBitmap( m_panel13112, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer154112->Add( m_bitmap94112, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl74112 = new wxTextCtrl( m_panel13112, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer154112->Add( m_textCtrl74112, 0, wxALL|wxEXPAND, 2 );

	m_bitmap104112 = new wxStaticBitmap( m_panel13112, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer154112->Add( m_bitmap104112, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 1 );

	m_textCtrl84112 = new wxTextCtrl( m_panel13112, ID_BTNCONFIG, wxT("Unused"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	bSizer154112->Add( m_textCtrl84112, 0, wxALL|wxEXPAND, 2 );

	gSizer101112->Add( bSizer154112, 1, wxEXPAND, 5 );

	bSizer9112->Add( gSizer101112, 1, wxEXPAND, 5 );

	m_panel13112->SetSizer( bSizer9112 );
	m_panel13112->Layout();
	bSizer9112->Fit( m_panel13112 );
	m_notebook4->AddPage( m_panel13112, wxT("Player 4"), false );

	InputBase_sizer->Add( m_notebook4, 1, wxEXPAND | wxALL, 5 );

	InputTab->SetSizer( InputBase_sizer );
	InputTab->Layout();
	InputBase_sizer->Fit( InputTab );
	SectionTabs->AddPage( InputTab, wxT("Input"), false );

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
