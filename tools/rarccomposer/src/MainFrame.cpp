///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb  1 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "MainFrame.h"

///////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE( MainFrame, wxFrame )
	EVT_CLOSE( MainFrame::_wxFB_onExit )
	EVT_MENU( wxID_NEW, MainFrame::_wxFB_onCreateNew )
	EVT_MENU( wxID_OPEN, MainFrame::_wxFB_onOpenFile )
	EVT_MENU( wxID_SAVE, MainFrame::_wxFB_onSaveFile )
	EVT_MENU( wxID_SAVEAS, MainFrame::_wxFB_onSaveFileAs )
	EVT_MENU( wxID_EXIT, MainFrame::_wxFB_onClickExit )
	EVT_MENU( ID_FILE_ADD, MainFrame::_wxFB_onAddFile )
	EVT_MENU( ID_FILE_EXPORT, MainFrame::_wxFB_onExportFile )
	EVT_MENU( ID_NEW_FOLDER, MainFrame::_wxFB_onCreateFolder )
	EVT_MENU( ID_FILE_REMOVE, MainFrame::_wxFB_onRemoveFile )
	EVT_MENU( wxID_UP, MainFrame::_wxFB_onMoveUp )
	EVT_MENU( wxID_DOWN, MainFrame::_wxFB_onMoveDown )
	EVT_MENU( wxID_VIEW_SORTNAME, MainFrame::_wxFB_onOrganize )
	EVT_MENU( wxID_ABOUT, MainFrame::_wxFB_onClickAbout )
	EVT_TOOL( wxID_NEW, MainFrame::_wxFB_onCreateNew )
	EVT_TOOL( wxID_OPEN, MainFrame::_wxFB_onOpenFile )
	EVT_TOOL( wxID_SAVE, MainFrame::_wxFB_onSaveFile )
	EVT_TOOL( ID_T_ADD, MainFrame::_wxFB_onAddFile )
	EVT_TOOL( ID_T_EXPORT, MainFrame::_wxFB_onExportFile )
	EVT_TOOL( ID_T_NEW_FOLDER, MainFrame::_wxFB_onCreateFolder )
	EVT_TOOL( ID_T_REMOVE, MainFrame::_wxFB_onRemoveFile )
	EVT_TOOL( wxID_UP, MainFrame::_wxFB_onMoveUp )
	EVT_TOOL( wxID_DOWN, MainFrame::_wxFB_onMoveDown )
	EVT_TOOL( wxID_EXIT, MainFrame::_wxFB_onClickExit )
	EVT_TREE_END_DRAG( ID_FILE_DISPLAY, MainFrame::_wxFB_updateItemList )
	EVT_TREE_END_LABEL_EDIT( ID_FILE_DISPLAY, MainFrame::_wxFB_renameEntry )
END_EVENT_TABLE()

MainFrame::MainFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	main_menubar = new wxMenuBar( 0 );
	m_file = new wxMenu();
	wxMenuItem* m_item_new;
	m_item_new = new wxMenuItem( m_file, wxID_NEW, wxString( wxT("New File") ) + wxT('\t') + wxT("Ctrl-N"), wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_item_new->SetBitmaps( wxBitmap( wxT("icn/page_white.png"), wxBITMAP_TYPE_ANY ) );
	#elif defined( __WXGTK__ )
	m_item_new->SetBitmap( wxBitmap( wxT("icn/page_white.png"), wxBITMAP_TYPE_ANY ) );
	#endif
	m_file->Append( m_item_new );

	wxMenuItem* m_item_open;
	m_item_open = new wxMenuItem( m_file, wxID_OPEN, wxString( wxT("Open File...") ) + wxT('\t') + wxT("Ctrl-O"), wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_item_open->SetBitmaps( wxBitmap( wxT("icn/folder_page.png"), wxBITMAP_TYPE_ANY ) );
	#elif defined( __WXGTK__ )
	m_item_open->SetBitmap( wxBitmap( wxT("icn/folder_page.png"), wxBITMAP_TYPE_ANY ) );
	#endif
	m_file->Append( m_item_open );

	wxMenuItem* m_item_save;
	m_item_save = new wxMenuItem( m_file, wxID_SAVE, wxString( wxT("Save File") ) + wxT('\t') + wxT("Ctrl-S"), wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_item_save->SetBitmaps( wxBitmap( wxT("icn/page_save.png"), wxBITMAP_TYPE_ANY ) );
	#elif defined( __WXGTK__ )
	m_item_save->SetBitmap( wxBitmap( wxT("icn/page_save.png"), wxBITMAP_TYPE_ANY ) );
	#endif
	m_file->Append( m_item_save );

	wxMenuItem* m_item_save_as;
	m_item_save_as = new wxMenuItem( m_file, wxID_SAVEAS, wxString( wxT("Save File As...") ) + wxT('\t') + wxT("Ctrl-Shift-S"), wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_item_save_as->SetBitmaps( wxBitmap( wxT("icn/page_save.png"), wxBITMAP_TYPE_ANY ) );
	#elif defined( __WXGTK__ )
	m_item_save_as->SetBitmap( wxBitmap( wxT("icn/page_save.png"), wxBITMAP_TYPE_ANY ) );
	#endif
	m_file->Append( m_item_save_as );

	m_file->AppendSeparator();

	wxMenuItem* m_item_exit;
	m_item_exit = new wxMenuItem( m_file, wxID_EXIT, wxString( wxT("Exit") ) + wxT('\t') + wxT("Alt-F4"), wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_item_exit->SetBitmaps( wxBitmap( wxT("icn/cross.png"), wxBITMAP_TYPE_ANY ) );
	#elif defined( __WXGTK__ )
	m_item_exit->SetBitmap( wxBitmap( wxT("icn/cross.png"), wxBITMAP_TYPE_ANY ) );
	#endif
	m_file->Append( m_item_exit );

	main_menubar->Append( m_file, wxT("File") );

	m_action = new wxMenu();
	wxMenuItem* m_item_add_file;
	m_item_add_file = new wxMenuItem( m_action, ID_FILE_ADD, wxString( wxT("Add File") ) + wxT('\t') + wxT("Ctrl-I"), wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_item_add_file->SetBitmaps( wxBitmap( wxT("icn/page_add.png"), wxBITMAP_TYPE_ANY ) );
	#elif defined( __WXGTK__ )
	m_item_add_file->SetBitmap( wxBitmap( wxT("icn/page_add.png"), wxBITMAP_TYPE_ANY ) );
	#endif
	m_action->Append( m_item_add_file );

	wxMenuItem* m_item_export_file;
	m_item_export_file = new wxMenuItem( m_action, ID_FILE_EXPORT, wxString( wxT("Export File") ) + wxT('\t') + wxT("Ctrl-E"), wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_item_export_file->SetBitmaps( wxBitmap( wxT("icn/page_go.png"), wxBITMAP_TYPE_ANY ) );
	#elif defined( __WXGTK__ )
	m_item_export_file->SetBitmap( wxBitmap( wxT("icn/page_go.png"), wxBITMAP_TYPE_ANY ) );
	#endif
	m_action->Append( m_item_export_file );

	wxMenuItem* m_item_folder_new;
	m_item_folder_new = new wxMenuItem( m_action, ID_NEW_FOLDER, wxString( wxT("Create Folder") ) + wxT('\t') + wxT("Insert"), wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_item_folder_new->SetBitmaps( wxBitmap( wxT("icn/folder_add.png"), wxBITMAP_TYPE_ANY ) );
	#elif defined( __WXGTK__ )
	m_item_folder_new->SetBitmap( wxBitmap( wxT("icn/folder_add.png"), wxBITMAP_TYPE_ANY ) );
	#endif
	m_action->Append( m_item_folder_new );

	wxMenuItem* m_item_remove_file;
	m_item_remove_file = new wxMenuItem( m_action, ID_FILE_REMOVE, wxString( wxT("Remove Item") ) + wxT('\t') + wxT("Del"), wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_item_remove_file->SetBitmaps( wxBitmap( wxT("icn/delete.png"), wxBITMAP_TYPE_ANY ) );
	#elif defined( __WXGTK__ )
	m_item_remove_file->SetBitmap( wxBitmap( wxT("icn/delete.png"), wxBITMAP_TYPE_ANY ) );
	#endif
	m_action->Append( m_item_remove_file );

	m_action->AppendSeparator();

	wxMenuItem* m_item_move_up;
	m_item_move_up = new wxMenuItem( m_action, wxID_UP, wxString( wxT("Move Up") ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_item_move_up->SetBitmaps( wxBitmap( wxT("icn/arrow_up.png"), wxBITMAP_TYPE_ANY ) );
	#elif defined( __WXGTK__ )
	m_item_move_up->SetBitmap( wxBitmap( wxT("icn/arrow_up.png"), wxBITMAP_TYPE_ANY ) );
	#endif
	m_action->Append( m_item_move_up );

	wxMenuItem* m_item_move_down;
	m_item_move_down = new wxMenuItem( m_action, wxID_DOWN, wxString( wxT("Move Down") ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_item_move_down->SetBitmaps( wxBitmap( wxT("icn/arrow_down.png"), wxBITMAP_TYPE_ANY ) );
	#elif defined( __WXGTK__ )
	m_item_move_down->SetBitmap( wxBitmap( wxT("icn/arrow_down.png"), wxBITMAP_TYPE_ANY ) );
	#endif
	m_action->Append( m_item_move_down );

	m_action->AppendSeparator();

	wxMenuItem* m_item_sortname;
	m_item_sortname = new wxMenuItem( m_action, wxID_VIEW_SORTNAME, wxString( wxT("Sort by Name") ) , wxEmptyString, wxITEM_NORMAL );
	m_action->Append( m_item_sortname );

	wxMenuItem* m_item_sorttype;
	m_item_sorttype = new wxMenuItem( m_action, wxID_VIEW_SORTTYPE, wxString( wxT("Sort by Type") ) , wxEmptyString, wxITEM_NORMAL );
	m_action->Append( m_item_sorttype );

	main_menubar->Append( m_action, wxT("Action") );

	m_about = new wxMenu();
	wxMenuItem* m_item_about;
	m_item_about = new wxMenuItem( m_about, wxID_ABOUT, wxString( wxT("About") ) , wxEmptyString, wxITEM_NORMAL );
	m_about->Append( m_item_about );

	main_menubar->Append( m_about, wxT("About") );

	this->SetMenuBar( main_menubar );

	m_tool_bar = this->CreateToolBar( wxTB_HORIZONTAL, wxID_ANY );
	m_tool_bar->SetToolBitmapSize( wxSize( 16,16 ) );
	m_tool_bar->AddTool( wxID_NEW, wxT("New File"), wxBitmap( wxT("icn/page_white.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Create a new file."), wxEmptyString );
	m_tool_bar->AddTool( wxID_OPEN, wxT("Open File"), wxBitmap( wxT("icn/folder_page.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Open an existing file."), wxEmptyString );
	m_tool_bar->AddTool( wxID_SAVE, wxT("Save File"), wxBitmap( wxT("icn/page_save.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Save this file."), wxEmptyString );
	m_tool_bar->AddSeparator();
	m_tool_bar->AddTool( ID_T_ADD, wxT("Add File"), wxBitmap( wxT("icn/page_add.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Add a file to this archive."), wxEmptyString );
	m_tool_bar->AddTool( ID_T_EXPORT, wxT("tool"), wxBitmap( wxT("icn/page_go.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Export a file from this archive."), wxEmptyString );
	m_tool_bar->AddTool( ID_T_NEW_FOLDER, wxT("tool"), wxBitmap( wxT("icn/folder_add.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Add a folder to this archive."), wxEmptyString );
	m_tool_bar->AddTool( ID_T_REMOVE, wxT("Remove File"), wxBitmap( wxT("icn/delete.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Remove an item (either folder or file)"), wxEmptyString );
	m_tool_bar->AddSeparator();
	m_tool_bar->AddTool( wxID_UP, wxT("Move Up"), wxBitmap( wxT("icn/arrow_up.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Move the selected item up."), wxEmptyString );
	m_tool_bar->AddTool( wxID_DOWN, wxT("Move Down"), wxBitmap( wxT("icn/arrow_down.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Move the selected item down."), wxEmptyString );
	m_tool_bar->AddSeparator();
	m_tool_bar->AddTool( wxID_EXIT, wxT("Exit"), wxBitmap( wxT("icn/cross.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Exit this program."), wxEmptyString );
	m_tool_bar->Realize();

	m_status_bar = this->CreateStatusBar( 2, wxST_SIZEGRIP, wxID_ANY );
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );

	wxGridSizer* gSizer1;
	gSizer1 = new wxGridSizer( 1, 1, 0, 0 );

	m_file_ctrl = new wxTreeCtrl( this, ID_FILE_DISPLAY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE|wxTR_EDIT_LABELS );
	gSizer1->Add( m_file_ctrl, 0, wxEXPAND, 0 );

	bSizer2->Add( gSizer1, 1, wxEXPAND, 5 );

	this->SetSizer( bSizer2 );
	this->Layout();
}

MainFrame::~MainFrame()
{
}
