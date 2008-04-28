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

#ifndef TREEITERATOR_H
#define TREEITERATOR_H

#include "RARCTreeData.h"

struct TreeItem;

/** @brief wxTreeCtrl iterator
 *
 *  Used to iterate through wxTreeCtrl's. It could be made inline,
 *  but this class makes the code cleaner.
 */
class TreeIterator {
    public:
        /** Constructor
         *
         *  @param tree wxTreeCtrl to be iterated.
         */
        TreeIterator(wxTreeCtrl *tree);
        TreeIterator(wxTreeCtrl *tree,wxTreeItemId wxIter);
        TreeIterator(wxTreeCtrl *tree,wxTreeItemId wxIter,wxTreeItemIdValue wxCookie);
        ~TreeIterator() {}

        RARCTreeData::EntryType getType() const;
        TreeItem                getItem() const;
        TreeItem                getFirstChild();
        TreeItem                getNextSibling();
        TreeItem                getNextChild();

    private:
        wxTreeCtrl        *mTree;
        wxTreeItemId       mID;
        wxTreeItemIdValue  mCookie;
};

struct TreeItem {
    bool                     isOk;
    RARCTreeData::EntryType  type;
    wxString                 text;
    RARCTreeData            *data;
};

#endif
