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

#ifndef __AppForms__
#define __AppForms__

#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/statbmp.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/choice.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/checkbox.h>
#include <wx/panel.h>
#include <wx/choicebk.h>
#include <wx/statbox.h>
#include <wx/notebook.h>
#include <wx/slider.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

namespace ConfigApplication
{
	///////////////////////////////////////////////////////////////////////////////
	/// Class ConfigWindow
	///////////////////////////////////////////////////////////////////////////////
	class ConfigWindow : public wxDialog
	{
		DECLARE_EVENT_TABLE()
		private:

			// Private event handlers
			void _wxFB_onRsysChange( wxChoicebookEvent& event ){ onRsysChange( event ); }
			void _wxFB_onRsysChanging( wxChoicebookEvent& event ){ onRsysChanging( event ); }
			void _wxFB_onChoiceD3D9Res( wxCommandEvent& event ){ onChoiceD3D9Res( event ); }
			void _wxFB_onCheckD3D9FullScreen( wxCommandEvent& event ){ onCheckD3D9FullScreen( event ); }
			void _wxFB_onCheckD3D9VSync( wxCommandEvent& event ){ onCheckD3D9VSync( event ); }
			void _wxFB_onChoiceOGLRes( wxCommandEvent& event ){ onChoiceOGLRes( event ); }
			void _wxFB_onChoiceOGLColDepth( wxCommandEvent& event ){ onChoiceOGLColDepth( event ); }
			void _wxFB_onChoiceOGLFreq( wxCommandEvent& event ){ onChoiceOGLFreq( event ); }
			void _wxFB_onChoiceOGLFSAA( wxCommandEvent& event ){ onChoiceOGLFSAA( event ); }
			void _wxFB_onCheckOGLFullScreen( wxCommandEvent& event ){ onCheckOGLFullScreen( event ); }
			void _wxFB_onCheckOGLVSync( wxCommandEvent& event ){ onCheckOGLVSync( event ); }
			void _wxFB_onDeviceChoice( wxCommandEvent& event ){ onDeviceChoice( event ); }
			void _wxFB_OnClickApply( wxCommandEvent& event ){ OnClickApply( event ); }
			void _wxFB_OnClickCancel( wxCommandEvent& event ){ OnClickCancel( event ); }
			void _wxFB_OnClickOk( wxCommandEvent& event ){ OnClickOk( event ); }


		public:
			enum
			{
				ID_SECTION_TABS = 1000,
				ID_RSYS_CBOOK,
				ID_D3D9_PANEL,
				ID_D3D9_RES,
				ID_D3D9_AA,
				ID_D3D9_FSCR,
				ID_D3D9_VSYNC,
				ID_OGL_PANEL,
				ID_OGL_RES,
				ID_OGL_CDEPTH,
				ID_OGL_FREQ,
				ID_OGL_FSAA,
				ID_OGL_FSCR,
				ID_OGL_VSYNC,
				ID_OAL_DEVICE,
				ID_OAL_MVOL,
				ID_OAL_EVOL,
				ID_OAL_VVOL,
				ID_PLAYERS_TABS,
				ID_PLAYER_TAB,
				ID_INPUT_DEVICE,
				ID_CTRL_PLUGGED,
				ID_CTRL_FFBACK,
				ID_BTNCONFIG,
			};

        protected:
            wxStaticBitmap* Logo;
			wxNotebook* SectionTabs;
			wxPanel* VideoTab;
			wxNotebook* m_notebook3;
			wxPanel* m_panel9;
			wxChoicebook* m_choicebook3;
			wxPanel* mD3D9Panel;
			wxStaticText* m_staticText5;
			wxChoice* m_choice_d3d9_res;
			wxStaticText* m_staticText8;
			wxChoice* m_choice_d3d9_aa;
			wxStaticLine* m_staticline2;
			wxCheckBox* m_checkBox_d3d9_fscr;
			wxCheckBox* m_checkBox_d3d9_vsync;
			wxPanel* mOGLPanel;
			wxStaticText* m_staticText11;
			wxChoice* m_choice_ogl_res;
			wxStaticText* m_staticText12;
			wxChoice* m_choice_ogl_cdepth;
			wxStaticText* m_staticText13;
			wxChoice* m_choice_ogl_freq;
			wxStaticText* m_staticText14;
			wxChoice* m_choice_ogl_fsaa;
			wxStaticLine* m_staticline3;
			wxCheckBox* m_checkBox_ogl_fscr;
			wxCheckBox* m_checkBox_ogl_vsync;
			wxPanel* m_panel10;
			wxPanel* AudioTab;
			wxChoice* m_choice24;
			wxSlider* m_slider4;
			wxSlider* m_slider41;
			wxSlider* m_slider42;

			wxPanel* InputTab;
			wxNotebook* m_notebook4;
			wxPanel* m_panel131;
			wxStaticText* m_staticText101;
			wxChoice* m_choice101;
			wxCheckBox* m_checkBox51;
			wxCheckBox* m_checkBox61;
			wxStaticLine* m_staticline311;
			wxStaticBitmap* m_bitmap31;
			wxTextCtrl* m_textCtrl11;
			wxStaticBitmap* m_bitmap41;
			wxTextCtrl* m_textCtrl21;
			wxStaticBitmap* m_bitmap51;
			wxTextCtrl* m_textCtrl31;
			wxStaticBitmap* m_bitmap61;
			wxTextCtrl* m_textCtrl41;
			wxStaticBitmap* m_bitmap75;
			wxTextCtrl* m_textCtrl55;
			wxStaticBitmap* m_bitmap85;
			wxTextCtrl* m_textCtrl65;
			wxStaticBitmap* m_bitmap95;
			wxTextCtrl* m_textCtrl75;
			wxStaticBitmap* m_bitmap105;
			wxTextCtrl* m_textCtrl85;
			wxStaticBitmap* m_bitmap711;
			wxTextCtrl* m_textCtrl511;
			wxStaticBitmap* m_bitmap811;
			wxTextCtrl* m_textCtrl611;
			wxStaticBitmap* m_bitmap911;
			wxTextCtrl* m_textCtrl711;
			wxStaticBitmap* m_bitmap1011;
			wxTextCtrl* m_textCtrl811;
			wxStaticBitmap* m_bitmap721;
			wxTextCtrl* m_textCtrl521;
			wxStaticBitmap* m_bitmap821;
			wxTextCtrl* m_textCtrl621;
			wxStaticBitmap* m_bitmap921;
			wxTextCtrl* m_textCtrl721;
			wxStaticBitmap* m_bitmap1021;
			wxTextCtrl* m_textCtrl821;
			wxStaticBitmap* m_bitmap731;
			wxTextCtrl* m_textCtrl531;
			wxStaticBitmap* m_bitmap831;
			wxTextCtrl* m_textCtrl631;
			wxStaticBitmap* m_bitmap931;
			wxTextCtrl* m_textCtrl731;
			wxStaticBitmap* m_bitmap1031;
			wxTextCtrl* m_textCtrl831;
			wxStaticBitmap* m_bitmap741;
			wxTextCtrl* m_textCtrl541;
			wxStaticBitmap* m_bitmap841;
			wxTextCtrl* m_textCtrl641;
			wxStaticBitmap* m_bitmap941;
			wxTextCtrl* m_textCtrl741;
			wxStaticBitmap* m_bitmap1041;
			wxTextCtrl* m_textCtrl841;
			wxPanel* m_panel1311;
			wxStaticText* m_staticText1011;
			wxChoice* m_choice1011;
			wxCheckBox* m_checkBox511;
			wxCheckBox* m_checkBox611;
			wxStaticLine* m_staticline3111;
			wxStaticBitmap* m_bitmap311;
			wxTextCtrl* m_textCtrl111;
			wxStaticBitmap* m_bitmap411;
			wxTextCtrl* m_textCtrl211;
			wxStaticBitmap* m_bitmap511;
			wxTextCtrl* m_textCtrl311;
			wxStaticBitmap* m_bitmap611;
			wxTextCtrl* m_textCtrl411;
			wxStaticBitmap* m_bitmap751;
			wxTextCtrl* m_textCtrl551;
			wxStaticBitmap* m_bitmap851;
			wxTextCtrl* m_textCtrl651;
			wxStaticBitmap* m_bitmap951;
			wxTextCtrl* m_textCtrl751;
			wxStaticBitmap* m_bitmap1051;
			wxTextCtrl* m_textCtrl851;
			wxStaticBitmap* m_bitmap7111;
			wxTextCtrl* m_textCtrl5111;
			wxStaticBitmap* m_bitmap8111;
			wxTextCtrl* m_textCtrl6111;
			wxStaticBitmap* m_bitmap9111;
			wxTextCtrl* m_textCtrl7111;
			wxStaticBitmap* m_bitmap10111;
			wxTextCtrl* m_textCtrl8111;
			wxStaticBitmap* m_bitmap7211;
			wxTextCtrl* m_textCtrl5211;
			wxStaticBitmap* m_bitmap8211;
			wxTextCtrl* m_textCtrl6211;
			wxStaticBitmap* m_bitmap9211;
			wxTextCtrl* m_textCtrl7211;
			wxStaticBitmap* m_bitmap10211;
			wxTextCtrl* m_textCtrl8211;
			wxStaticBitmap* m_bitmap7311;
			wxTextCtrl* m_textCtrl5311;
			wxStaticBitmap* m_bitmap8311;
			wxTextCtrl* m_textCtrl6311;
			wxStaticBitmap* m_bitmap9311;
			wxTextCtrl* m_textCtrl7311;
			wxStaticBitmap* m_bitmap10311;
			wxTextCtrl* m_textCtrl8311;
			wxStaticBitmap* m_bitmap7411;
			wxTextCtrl* m_textCtrl5411;
			wxStaticBitmap* m_bitmap8411;
			wxTextCtrl* m_textCtrl6411;
			wxStaticBitmap* m_bitmap9411;
			wxTextCtrl* m_textCtrl7411;
			wxStaticBitmap* m_bitmap10411;
			wxTextCtrl* m_textCtrl8411;
			wxPanel* m_panel13111;
			wxStaticText* m_staticText10111;
			wxChoice* m_choice10111;
			wxCheckBox* m_checkBox5111;
			wxCheckBox* m_checkBox6111;
			wxStaticLine* m_staticline31111;
			wxStaticBitmap* m_bitmap3111;
			wxTextCtrl* m_textCtrl1111;
			wxStaticBitmap* m_bitmap4111;
			wxTextCtrl* m_textCtrl2111;
			wxStaticBitmap* m_bitmap5111;
			wxTextCtrl* m_textCtrl3111;
			wxStaticBitmap* m_bitmap6111;
			wxTextCtrl* m_textCtrl4111;
			wxStaticBitmap* m_bitmap7511;
			wxTextCtrl* m_textCtrl5511;
			wxStaticBitmap* m_bitmap8511;
			wxTextCtrl* m_textCtrl6511;
			wxStaticBitmap* m_bitmap9511;
			wxTextCtrl* m_textCtrl7511;
			wxStaticBitmap* m_bitmap10511;
			wxTextCtrl* m_textCtrl8511;
			wxStaticBitmap* m_bitmap71111;
			wxTextCtrl* m_textCtrl51111;
			wxStaticBitmap* m_bitmap81111;
			wxTextCtrl* m_textCtrl61111;
			wxStaticBitmap* m_bitmap91111;
			wxTextCtrl* m_textCtrl71111;
			wxStaticBitmap* m_bitmap101111;
			wxTextCtrl* m_textCtrl81111;
			wxStaticBitmap* m_bitmap72111;
			wxTextCtrl* m_textCtrl52111;
			wxStaticBitmap* m_bitmap82111;
			wxTextCtrl* m_textCtrl62111;
			wxStaticBitmap* m_bitmap92111;
			wxTextCtrl* m_textCtrl72111;
			wxStaticBitmap* m_bitmap102111;
			wxTextCtrl* m_textCtrl82111;
			wxStaticBitmap* m_bitmap73111;
			wxTextCtrl* m_textCtrl53111;
			wxStaticBitmap* m_bitmap83111;
			wxTextCtrl* m_textCtrl63111;
			wxStaticBitmap* m_bitmap93111;
			wxTextCtrl* m_textCtrl73111;
			wxStaticBitmap* m_bitmap103111;
			wxTextCtrl* m_textCtrl83111;
			wxStaticBitmap* m_bitmap74111;
			wxTextCtrl* m_textCtrl54111;
			wxStaticBitmap* m_bitmap84111;
			wxTextCtrl* m_textCtrl64111;
			wxStaticBitmap* m_bitmap94111;
			wxTextCtrl* m_textCtrl74111;
			wxStaticBitmap* m_bitmap104111;
			wxTextCtrl* m_textCtrl84111;
			wxPanel* m_panel13112;
			wxStaticText* m_staticText10112;
			wxChoice* m_choice10112;
			wxCheckBox* m_checkBox5112;
			wxCheckBox* m_checkBox6112;
			wxStaticLine* m_staticline31112;
			wxStaticBitmap* m_bitmap3112;
			wxTextCtrl* m_textCtrl1112;
			wxStaticBitmap* m_bitmap4112;
			wxTextCtrl* m_textCtrl2112;
			wxStaticBitmap* m_bitmap5112;
			wxTextCtrl* m_textCtrl3112;
			wxStaticBitmap* m_bitmap6112;
			wxTextCtrl* m_textCtrl4112;
			wxStaticBitmap* m_bitmap7512;
			wxTextCtrl* m_textCtrl5512;
			wxStaticBitmap* m_bitmap8512;
			wxTextCtrl* m_textCtrl6512;
			wxStaticBitmap* m_bitmap9512;
			wxTextCtrl* m_textCtrl7512;
			wxStaticBitmap* m_bitmap10512;
			wxTextCtrl* m_textCtrl8512;
			wxStaticBitmap* m_bitmap71112;
			wxTextCtrl* m_textCtrl51112;
			wxStaticBitmap* m_bitmap81112;
			wxTextCtrl* m_textCtrl61112;
			wxStaticBitmap* m_bitmap91112;
			wxTextCtrl* m_textCtrl71112;
			wxStaticBitmap* m_bitmap101112;
			wxTextCtrl* m_textCtrl81112;
			wxStaticBitmap* m_bitmap72112;
			wxTextCtrl* m_textCtrl52112;
			wxStaticBitmap* m_bitmap82112;
			wxTextCtrl* m_textCtrl62112;
			wxStaticBitmap* m_bitmap92112;
			wxTextCtrl* m_textCtrl72112;
			wxStaticBitmap* m_bitmap102112;
			wxTextCtrl* m_textCtrl82112;
			wxStaticBitmap* m_bitmap73112;
			wxTextCtrl* m_textCtrl53112;
			wxStaticBitmap* m_bitmap83112;
			wxTextCtrl* m_textCtrl63112;
			wxStaticBitmap* m_bitmap93112;
			wxTextCtrl* m_textCtrl73112;
			wxStaticBitmap* m_bitmap103112;
			wxTextCtrl* m_textCtrl83112;
			wxStaticBitmap* m_bitmap74112;
			wxTextCtrl* m_textCtrl54112;
			wxStaticBitmap* m_bitmap84112;
			wxTextCtrl* m_textCtrl64112;
			wxStaticBitmap* m_bitmap94112;
			wxTextCtrl* m_textCtrl74112;
			wxStaticBitmap* m_bitmap104112;
			wxTextCtrl* m_textCtrl84112;
			wxStdDialogButtonSizer* DialogButtons;
			wxButton* DialogButtonsOK;
			wxButton* DialogButtonsApply;
			wxButton* DialogButtonsCancel;

			// Virtual event handlers, overide them in your derived class
			virtual void onRsysChange( wxChoicebookEvent& event ){ event.Skip(); }
			virtual void onRsysChanging( wxChoicebookEvent& event ){ event.Skip(); }
			virtual void onChoiceD3D9Res( wxCommandEvent& event ){ event.Skip(); }
			virtual void onCheckD3D9FullScreen( wxCommandEvent& event ){ event.Skip(); }
			virtual void onCheckD3D9VSync( wxCommandEvent& event ){ event.Skip(); }
			virtual void onChoiceOGLRes( wxCommandEvent& event ){ event.Skip(); }
			virtual void onChoiceOGLColDepth( wxCommandEvent& event ){ event.Skip(); }
			virtual void onChoiceOGLFreq( wxCommandEvent& event ){ event.Skip(); }
			virtual void onChoiceOGLFSAA( wxCommandEvent& event ){ event.Skip(); }
			virtual void onCheckOGLFullScreen( wxCommandEvent& event ){ event.Skip(); }
			virtual void onCheckOGLVSync( wxCommandEvent& event ){ event.Skip(); }
			virtual void onDeviceChoice( wxCommandEvent& event ){ event.Skip(); }
			virtual void OnClickApply( wxCommandEvent& event ){ event.Skip(); }
			virtual void OnClickCancel( wxCommandEvent& event ){ event.Skip(); }
			virtual void OnClickOk( wxCommandEvent& event ){ event.Skip(); }


		public:
			ConfigWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Game Settings"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,420 ), long style = wxDEFAULT_DIALOG_STYLE|wxMINIMIZE_BOX );
			~ConfigWindow();

	};

} // namespace ConfigApplication

#endif //__AppForms__
