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

#include "TreeIterator.h"

TreeIterator::TreeIterator(wxTreeCtrl *tree) : mTree(tree) {
    // Begin iteration from wxTreeCtrl's first root child
    mID = mTree->GetFirstChild(mTree->GetRootItem(),mCookie);
}

TreeIterator::TreeIterator(wxTreeCtrl *tree,wxTreeItemId wxIter) :
        mTree(tree), mID(wxIter) {
    mID = mTree->GetFirstChild(wxIter,mCookie);
}

TreeIterator::TreeIterator(wxTreeCtrl *tree,wxTreeItemId wxIter,wxTreeItemIdValue wxCookie) :
        mTree(tree), mID(wxIter), mCookie(wxCookie) {
    // Invalidate supplied cookie
    wxCookie = NULL;
}

TreeItem TreeIterator::getItem() const {
    TreeItem item; // Return struct

    // If this iterator is currently pointing to a valid item,
    // we fill the struct with information. If not, it is only
    // tagged as invalid by setting isOk to false
    if (mID.IsOk()) {
        item.isOk = true;
        item.type = getType();
        item.text = mTree->GetItemText(mID);
        item.data = static_cast<RARCTreeData *>(mTree->GetItemData(mID));
    } else {
        item.isOk = false;
    }

    return item;
}

TreeItem TreeIterator::getFirstChild() {
    // Iterates
    mID = mTree->GetFirstChild(mID,mCookie);
    return getItem();
}
TreeItem TreeIterator::getNextSibling() {
    // Iterates
    mID = mTree->GetNextSibling(mID);
    return getItem();
}

TreeItem TreeIterator::getNextChild() {
    // Iterates
    mID = mTree->GetNextChild(mID,mCookie);
    return getItem();
}

RARCTreeData::EntryType TreeIterator::getType() const {
    // Get tree item data (in RARC Composer, it can only be RARCTreeData)
    // and return entry type
    return static_cast<RARCTreeData *>(mTree->GetItemData(mID))->getType();
}
