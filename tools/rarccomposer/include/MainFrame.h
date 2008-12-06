///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb  1 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __rarc_composer__
#define __rarc_composer__

#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/toolbar.h>
#include <wx/statusbr.h>
#include <wx/treectrl.h>
#include <wx/sizer.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class MainFrame
///////////////////////////////////////////////////////////////////////////////
class MainFrame : public wxFrame
{
	DECLARE_EVENT_TABLE()
	private:

		// Private event handlers
		void _wxFB_onExit( wxCloseEvent& event ){ onExit( event ); }
		void _wxFB_onCreateNew( wxCommandEvent& event ){ onCreateNew( event ); }
		void _wxFB_onOpenFile( wxCommandEvent& event ){ onOpenFile( event ); }
		void _wxFB_onSaveFile( wxCommandEvent& event ){ onSaveFile( event ); }
		void _wxFB_onSaveFileAs( wxCommandEvent& event ){ onSaveFileAs( event ); }
		void _wxFB_onClickExit( wxCommandEvent& event ){ onClickExit( event ); }
		void _wxFB_onAddFile( wxCommandEvent& event ){ onAddFile( event ); }
		void _wxFB_onExportFile( wxCommandEvent& event ){ onExportFile( event ); }
		void _wxFB_onCreateFolder( wxCommandEvent& event ){ onCreateFolder( event ); }
		void _wxFB_onRemoveFile( wxCommandEvent& event ){ onRemoveFile( event ); }
		void _wxFB_onMoveUp( wxCommandEvent& event ){ onMoveUp( event ); }
		void _wxFB_onMoveDown( wxCommandEvent& event ){ onMoveDown( event ); }
		void _wxFB_onOrganize( wxCommandEvent& event ){ onOrganize( event ); }
		void _wxFB_onClickAbout( wxCommandEvent& event ){ onClickAbout( event ); }
		void _wxFB_updateItemList( wxTreeEvent& event ){ updateItemList( event ); }
		void _wxFB_renameEntry( wxTreeEvent& event ){ renameEntry( event ); }


	public:
		enum
		{
			ID_FILE_ADD = 1000,
			ID_FILE_EXPORT,
			ID_NEW_FOLDER,
			ID_FILE_REMOVE,
			ID_T_ADD,
			ID_T_EXPORT,
			ID_T_NEW_FOLDER,
			ID_T_REMOVE,
			ID_FILE_DISPLAY,
		};

		wxMenuBar* main_menubar;
		wxMenu* m_file;
		wxMenu* m_action;
		wxMenu* m_about;
		wxToolBar* m_tool_bar;
		wxStatusBar* m_status_bar;
		wxTreeCtrl* m_file_ctrl;

		// Virtual event handlers, overide them in your derived class
		virtual void onExit( wxCloseEvent& event ){ event.Skip(); }
		virtual void onCreateNew( wxCommandEvent& event ){ event.Skip(); }
		virtual void onOpenFile( wxCommandEvent& event ){ event.Skip(); }
		virtual void onSaveFile( wxCommandEvent& event ){ event.Skip(); }
		virtual void onSaveFileAs( wxCommandEvent& event ){ event.Skip(); }
		virtual void onClickExit( wxCommandEvent& event ){ event.Skip(); }
		virtual void onAddFile( wxCommandEvent& event ){ event.Skip(); }
		virtual void onExportFile( wxCommandEvent& event ){ event.Skip(); }
		virtual void onCreateFolder( wxCommandEvent& event ){ event.Skip(); }
		virtual void onRemoveFile( wxCommandEvent& event ){ event.Skip(); }
		virtual void onMoveUp( wxCommandEvent& event ){ event.Skip(); }
		virtual void onMoveDown( wxCommandEvent& event ){ event.Skip(); }
		virtual void onOrganize( wxCommandEvent& event ){ event.Skip(); }
		virtual void onClickAbout( wxCommandEvent& event ){ event.Skip(); }
		virtual void updateItemList( wxTreeEvent& event ){ event.Skip(); }
		virtual void renameEntry( wxTreeEvent& event ){ event.Skip(); }


	public:
		MainFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("RARC Composer"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 560,450 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~MainFrame();

};

#endif //__rarc_composer__
