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

#include <sstream>
#include <iostream>
#include <vector>
#include <deque>
#include "wx/wx.h"
#include "wx/file.h"
#include "wx/treebase.h"
#include "TreeIterator.h"
#include "RARCTreeData.h"
#include "RARCFile.h"
#include "MainFrameImpl.h"

MainFrameImpl::MainFrameImpl(wxWindow *parent) : MainFrame(parent) {
    // Initialises members
    mAppName     = "RARC Composer";
    mModSymbol   = "*";
    mArcName     = "Untitled.rarc";
    mFullPath    = "";
    mCompressed  = true;
    mRootName    = "root/";
    mWildCards   = "Compressed Resource Archive (*.rarc)|*.rarc|Uncompressed Compound Archive (*.carc)|*.carc|All Files (*.*)|*.*";
    mModified    = true;
    mCloseOnSave = false;

    // Updates title bar (will be "Untitled - RARC Composer")
    updateTitleBar();

    // Creates and fills an image list for holding m_file_ctrl icons
    mImageList = new wxImageList(16,16,false,3);
    mImageList->Add(wxBitmap(wxT("icn/package_w.png"),wxBITMAP_TYPE_ANY));
    mImageList->Add(wxBitmap(wxT("icn/folder_w.png") ,wxBITMAP_TYPE_ANY));
    mImageList->Add(wxBitmap(wxT("icn/page_w.png")   ,wxBITMAP_TYPE_ANY));
    m_file_ctrl->SetImageList(mImageList);

    // Creates root object
    addFile(RARCTreeData::ET_ROOT,mRootName);
}

void MainFrameImpl::onExit(wxCloseEvent &event) {
    // If our archive was modified, we must ask for saving it
    if (mModified) {
        // The 'Cancel' option will only be available if the event can be veto'ed
        int veto = (event.CanVeto()) ? wxCANCEL : 0;
        int a = wxMessageBox("The file has been modified.\nDo you want to save the changes?","Save File",
                             wxYES_NO|veto|wxICON_QUESTION);

        if (a == wxYES) {
            // When there is no Full Path, it means the file was never saved before, and thus
            // we must ask for a place where to save it
            if (mFullPath.empty()) {
                // Loop to ensure saving when the event cannot be veto'ed
                while (true) {
                    wxCommandEvent saveAsEvt(wxEVT_COMMAND_MENU_SELECTED,wxID_SAVEAS);
                    saveAsEvt.SetEventObject(this);

                    // Asks for a save as dialog
                    ProcessEvent(saveAsEvt);

                    // If after the 'Save As' event being processed the archive is still modified,
                    // it means it was not saved, and so we must veto the close event, or try again
                    // if we can't
                    if (mModified) {
                        if (event.CanVeto()) {
                            event.Veto();
                            return;
                        }
                    } else {
                        // It was saved, so we break this loop
                        break;
                    }
                }
            } else {
                // If the file was already saved once before, we save it using that old path
                // and compression option (enabled or not)
                if (!save(mFullPath,mCompressed)) {
                    // Veto event on errors
                    if (event.CanVeto()) {
                        event.Veto();
                        return;
                    } else {
                        wxLogError("Oops! The file could not be saved and the application closing could "
                                   "not be aborted. Changes will be lost, sorry.");
                    }
                }
            }
        } else {
            if (a == wxCANCEL) {
                event.Veto();
                return;
            }
        }
    }

    // Destroys window, thus closing the application
    Destroy();
}

void MainFrameImpl::onCreateNew(wxCommandEvent &event) {
    // Asks for a filename
    wxTextEntryDialog *textDialog = new wxTextEntryDialog(this,"Create New File","File Name","untitled.rarc",
                                    wxOK|wxCANCEL|wxCENTRE,wxDefaultPosition);

    // If the dialog is canceled, nothing will be done
    if (textDialog->ShowModal() == wxID_OK) {
        // Sets new archive name and resets file list
        mArcName = textDialog->GetValue();
        m_file_ctrl->DeleteAllItems();
        addFile(RARCTreeData::ET_ROOT,mRootName);

        // Updates title bar
        updateTitleBar();
    }

    // Closes dialog
    textDialog->Close();
}

void MainFrameImpl::onOpenFile(wxCommandEvent &event) {
    wxMessageBox(wxT("Not implemented"),wxT(""));
}

void MainFrameImpl::onSaveFile(wxCommandEvent &event) {
    if (mModified) {
        if (mFullPath.empty()) {
            wxCommandEvent saveAsEvt(wxEVT_COMMAND_MENU_SELECTED,wxID_SAVEAS);
            saveAsEvt.SetEventObject(this);
            ProcessEvent(saveAsEvt);
        } else {
            if (save(mFullPath,mCompressed)) {
                mModified = false;
                updateTitleBar();
            }
        }
    }
}

void MainFrameImpl::onSaveFileAs(wxCommandEvent &event) {
    wxFileDialog *openDialog = new wxFileDialog(this,_T("Save File As..."),_(""),_(mArcName),mWildCards,
                               wxSAVE|wxFD_OVERWRITE_PROMPT);

    if (openDialog->ShowModal() == wxID_OK) {
        wxString  outFilePath = openDialog->GetPath();
        wxString  filename    = openDialog->GetFilename();

        // Checks if the extension is .rarc
        // If so, the file should be compressed
        bool compress = (filename.rfind(".rarc") != wxString::npos);

        // If it is not rarc, but also is not carc, we ask to know if the user
        // wants to the archive to be compressed or not
        if (!compress && filename.rfind(".carc") == wxString::npos) {
            int a = wxMessageBox("The file extension was not identified.\nDo you want to use compression?",
                                 "Compress Output",wxYES_NO|wxICON_QUESTION);

            // Uses compression if the answer is yes
            compress = (a == wxYES);
        }

        // Tries to save and updates titlebar on success
        if (save(outFilePath,compress)) {
            mArcName    = filename;
            mFullPath   = outFilePath;
            mCompressed = compress;
            mModified   = false;
            updateTitleBar();

            if (mCloseOnSave)
                Close();
        } else {
            mCloseOnSave = false;
        }
    }

    // Closes file dialog
    openDialog->Close();
}

void MainFrameImpl::onAddFile(wxCommandEvent &event) {
    wxFileDialog *openDialog = new wxFileDialog(this,_T("Add File"),_(""),_(""),_("All Files (*.*)|*.*"),
                               wxOPEN|wxFD_FILE_MUST_EXIST);

    if (openDialog->ShowModal() == wxID_OK) {
        // Opens a file
        wxFile *fp   = new wxFile(openDialog->GetPath(),wxFile::read);

        // Allocates memory to hold it
        size_t  flen = fp->Length();
        char   *data = new char[flen];

        // Reads it into memory and add to file list
        fp->Read(data,flen);
        if (!addFile(RARCTreeData::ET_FILE,openDialog->GetFilename(),data,flen)) {
            delete data;
            wxMessageBox("A file with the same name already exists in the current folder.",
                         "File addition failure",wxICON_ERROR);
        }
    }

    // Closes file dialog
    openDialog->Close();
}

void MainFrameImpl::onExportFile(wxCommandEvent &event) {
    wxTreeItemId  exportItem; // Selected item
    RARCTreeData *exportData; // Selected item data

    // Checks if the user asked to export the root object
    // (todo: we should make this export the whole archive!)
    if (m_file_ctrl->GetSelection() == m_file_ctrl->GetRootItem()) {
        wxLogError("Folder exportion not implemented"); // <todo>
        return;
    }

    exportItem = m_file_ctrl->GetSelection();
    exportData = static_cast<RARCTreeData*>(m_file_ctrl->GetItemData(exportItem));

    // Checks if the user asked to export a folder
    // (todo: we should make this export the whole folder!)
    if (exportData->getType() == RARCTreeData::ET_FOLDER) {
        wxLogError("Folder exportion not implemented"); // <todo>
        return;
    }

    wxFileDialog *openDialog = new wxFileDialog(this,_T("Export File"),_(""),_(m_file_ctrl->
                               GetItemText(exportItem)),_("All Files (*.*)|*.*"),
                               wxSAVE|wxFD_OVERWRITE_PROMPT);

    if (openDialog->ShowModal() == wxID_OK) {
        // Opens selected file
        wxFile *fp = new wxFile(openDialog->GetPath(),wxFile::write);
        std::cout << "Export File: " << openDialog->GetPath() << "\nBuffer Size: " <<
        exportData->getBufferSize() << "\n";

        // Write entry's data buffer to file
        fp->Write(exportData->getBuffer(),exportData->getBufferSize());
        delete fp;
    }

    openDialog->Close();
}

void MainFrameImpl::onRemoveFile(wxCommandEvent &event) {
    int           a;
    wxTreeItemId  selection = m_file_ctrl->GetSelection();
    RARCTreeData *selData   = static_cast<RARCTreeData *>(m_file_ctrl->GetItemData(selection));

    // One cannot delete the root object
    if (selData->getType() == RARCTreeData::ET_ROOT)
        return;

    a = wxMessageBox("Are you sure you want to remove this item?","Remove Item",wxYES_NO|wxICON_QUESTION);
    if (a == wxYES) {
        m_file_ctrl->Delete(m_file_ctrl->GetSelection());
        mModified = true;
        updateTitleBar();
    }
}

void MainFrameImpl::onMoveUp(wxCommandEvent &event) {
    // TODO: Implement onMoveUp
}

void MainFrameImpl::onMoveDown(wxCommandEvent &event) {
    // TODO: Implement onMoveDown
}

void MainFrameImpl::onCreateFolder(wxCommandEvent &event) {
    wxTextEntryDialog *textDialog = new wxTextEntryDialog(this,"Create New Folder","Folder Name","",
                                    wxOK|wxCANCEL|wxCENTRE,wxDefaultPosition);

    if (textDialog->ShowModal() == wxID_OK) {
        wxString newFolderName = textDialog->GetValue();

        // Filters inputed folder name
        if (newFolderName.size() == 0 || newFolderName.find("/") != wxString::npos ||
                newFolderName.find("\\")!= wxString::npos) {
            wxLogError("Invalid name:\nFolder names can't be blank, contain '/' nor '\\'.");
            return;
        }

        // Add new folder
        if (!addFile(RARCTreeData::ET_FOLDER,newFolderName)) {
            wxMessageBox("A folder with the same name already exists in the current folder.",
                         "Folder creation failure",wxICON_ERROR);
        }
    }

    textDialog->Close(); // Or OpenDialog->Destroy() ?
}

void MainFrameImpl::onClickExit(wxCommandEvent &event) {
    // TODO: Implement onClickAbout
}

void MainFrameImpl::onClickAbout(wxCommandEvent &event) {
    wxLogMessage("RARC Composer - v0.0.1\nCoded by: Arthur ''Yoasakura'' Carvalho de Souza Lima\n");
}

void MainFrameImpl::updateItemList(wxTreeEvent &event) {
    // TODO: Implement updateItemList
}

void MainFrameImpl::renameEntry(wxTreeEvent &event) {
    // TODO: Implement renameEntry
}

void MainFrameImpl::updateTitleBar() {
    wxString title = mArcName;
    if (mModified)
        title += mModSymbol;

    title += " - "+mAppName;
    SetLabel(title);
}

bool MainFrameImpl::addFile(RARCTreeData::EntryType type,wxString name,char *ptr,size_t size) {
    size_t        icon       = 2; // File icon
    wxTreeItemId  prevFile   = m_file_ctrl->GetSelection();
    wxTreeItemId  parentFile = m_file_ctrl->GetItemParent(prevFile);
    RARCTreeData *prevData   = static_cast<RARCTreeData*>(m_file_ctrl->GetItemData(prevFile));

    switch (type) {
    case RARCTreeData::ET_ROOT:
        if (!m_file_ctrl->IsEmpty())
            return false;

        icon = 0; // Root icon
        m_file_ctrl->AddRoot(mRootName,icon,-1,new RARCTreeData(RARCTreeData::ET_ROOT));
        break;

    case RARCTreeData::ET_FOLDER:
        name += "/"; // Appends "/" to folder name
        icon = 1;    // Use folder icon

    case RARCTreeData::ET_FILE: {
        TreeIterator iter(m_file_ctrl);

        // If the selection is a file, we need to explore its parent folder
        // If it's a folder, we need to explore the folder
        if (prevData->getType() == RARCTreeData::ET_FILE) {
            iter = TreeIterator(m_file_ctrl,parentFile);
        } else {
            iter = TreeIterator(m_file_ctrl,prevFile);
        }

        // Checks for existence
        while (iter.getItem().isOk) {
            if (iter.getItem().text == name)
                return false;

            iter.getNextChild();
        }

        // The default icon is a file one, so we don't have to set it again
        if (prevFile == m_file_ctrl->GetRootItem()) {
            // If the selected entry is the root object, we only need to append
            // this new item as its child
            m_file_ctrl->AppendItem(m_file_ctrl->GetRootItem(),name,icon,-1,
                                    new RARCTreeData(type,ptr,size));
        } else
            if (prevData->getType() == RARCTreeData::ET_FILE) {
                // If one tries to add a file to a file entry, we have insert it to
                // the selected file entry's parent folder
                m_file_ctrl->InsertItem(parentFile,prevFile,name,icon,-1,
                                        new RARCTreeData(type,ptr,size));
            } else {
                // If the selected entry is a folder, we only need to append this new
                // item as its child
                m_file_ctrl->AppendItem(prevFile,name,icon,-1,new RARCTreeData(type,ptr,size));
            }
    }
    break;

    default:
        break;
    }

    // Hack for wxWidgets
    // It wasn't redrawing when a folder was added within an empty one,
    // so I'm forcing it to do so here
    m_file_ctrl->Refresh(false);

    // Updates modified status
    mModified = true;
    updateTitleBar();

    return true;
}

bool MainFrameImpl::save(wxString filepath,bool compressed) {
    // In this deque, the current iterator exploring new files is took
    // using iters.back(). The old ones are stored with push_front(), so when
    // the current one drops, the old one takes place and continue exploring
    // the tree
    std::deque<TreeIterator *> iters;

    // Exportion interface
    RARCFile *fileExport  = new RARCFile("RARC","CARC");

    // Creates an iterator and push it in the deque
    iters.push_back(new TreeIterator(m_file_ctrl));

    while (!iters.empty()) {
        TreeItem      item;               // Current item information
        TreeIterator *cur = iters.back(); // Current iterator

        // Get information about current iterator
        // If it is not valid, we must delete and drop it, so that
        // the old one becomes current, or the deque becomes empty
        // The loop ends when the deque gets empty
        item = cur->getItem();
        if (!item.isOk) {
            delete cur;
            iters.pop_back();

            // Skips the item we have already scanned
            if (!iters.empty())
                iters.back()->getNextSibling();

            continue;
        }

        switch (item.type) {
        case RARCTreeData::ET_FOLDER:
            // Place a backup right before the current iterator
            iters.pop_back();
            iters.push_back(new TreeIterator(*cur));
            iters.push_back(cur);

            // Tries to get the folder's first child (this will
            // cause the iterator to be dropped in the beginning
            // of the loop if there is no child)
            cur->getFirstChild();
            break;

        case RARCTreeData::ET_FILE: {
            wxString str; // String formatter

            // Builds a path to the file, iterating through the deque
            for (size_t i = 0;i < iters.size();++i) {
                TreeItem item = iters[i]->getItem();

                // Skips files (I wonder if this is needed. It doesn't hurt, though..)
                if (item.type == RARCTreeData::ET_FOLDER)
                    str += item.text;
            }

            // Appends the filename
            str += item.text;

            // Add file entry to file export class
            fileExport->addFileEntry(str,str.size(),item.data->getBuffer(),item.data->getBufferSize());

            // Tries to get the file's next sibling (this will
            // cause the iterator to be dropped in the beginning
            // of the loop if there is no sibling)
            cur->getNextSibling();
        }
        break;

        default:
            break;
        }
    }

    // Write data to the file
    if (!fileExport->writeData(filepath,compressed)) {
        delete fileExport;
        return false;
    }

    delete fileExport;
    return true;
}
