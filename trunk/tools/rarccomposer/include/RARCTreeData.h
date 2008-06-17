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

#ifndef RARCTREEDATA_H
#define RARCTREEDATA_H

#include <wx/treectrl.h>

class RARCTreeData : public wxTreeItemData {
public:
    enum EntryType {
        ET_ROOT   = 1000,
        ET_FOLDER = 2000,
        ET_FILE   = 3000,
        ET_NONE   = 4000
    };

    RARCTreeData(EntryType type,char *buffer = NULL,size_t bufSize = 0);
    ~RARCTreeData();

    inline EntryType   getType()       const { return mEntryType;      }
    inline const char *getBuffer()     const { return mFileBuffer;     }
    inline size_t      getBufferSize() const { return mFileBufferSize; }

private:
    EntryType  mEntryType;

    char      *mFileBuffer;
    size_t     mFileBufferSize;
};

#endif // RARCTREEDATA_H
