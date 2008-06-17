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

#ifndef MAINFRAMEIMPL_H
#define MAINFRAMEIMPL_H

#include "RARCTreeData.h"
#include "MainFrame.h"

class MainFrameImpl : public MainFrame {
public:
	MainFrameImpl(wxWindow *parent);

protected:
	// Handlers for MainFrame events
	void onExit( wxCloseEvent& event );
	void onCreateNew( wxCommandEvent& event );
	void onOpenFile( wxCommandEvent& event );
	void onSaveFile( wxCommandEvent& event );
	void onSaveFileAs( wxCommandEvent& event );
	void onAddFile( wxCommandEvent& event );
	void onExportFile( wxCommandEvent& event );
	void onRemoveFile( wxCommandEvent& event );
	void onMoveUp( wxCommandEvent& event );
	void onMoveDown( wxCommandEvent& event );
	void onCreateFolder ( wxCommandEvent& event );
	void onClickExit( wxCommandEvent& event );
	void onClickAbout( wxCommandEvent& event );
	void updateItemList( wxTreeEvent& event );
	void renameEntry( wxTreeEvent& event );
	bool save(wxString filepath,bool compressed);

private:
	wxString     mAppName;
    wxString     mModSymbol;
    wxString     mArcName;
    wxString     mFullPath;
    bool         mCompressed;
    wxString     mRootName;
    wxString     mWildCards;
    wxImageList *mImageList;
    bool         mModified;
    bool         mCloseOnSave;

	// Internal Functions
	void updateTitleBar();
	bool addFile(RARCTreeData::EntryType type,wxString name,char *ptr = NULL,size_t size = 0);
};

#endif // MAINFRAMEIMPL_H
