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
#include "SonettoSaveMap.h"
#include <iostream>
namespace Sonetto {
    //-----------------------------------------------------------------------------
    // Sonetto::SaveMap implementation.
    //-----------------------------------------------------------------------------
    SaveMap::SaveMap() : mInitialized(false),
    mMemoryFrameStart(NULL),
    mMemoryFrameSize(NULL),
    mMemoryData(NULL),
    mMemorySize(NULL),
    mMemoryFrameNum(0)
    {
        mMemoryFrameStart = &mMemoryFrame[0];
    }
    //-----------------------------------------------------------------------------
    SaveMap::~SaveMap()
    {
        // Delete the SaveMap data.
        if(mMemoryData)
            delete[] mMemoryData;
    }
    //-----------------------------------------------------------------------------
    void SaveMap::initialize()
    {
        // Check to see if it's not initialized yet.
        if(!mInitialized)
        {
            mMemoryData = new char[mMemorySize];
            mMemoryFrameStart = &mMemoryFrame[0];
            // Now set the initialized flag on.
            mInitialized = true;
        }
    }
    //-----------------------------------------------------------------------------
    void * SaveMap::getMemoryFramePtr()
    {
        return &mMemoryFrame[0];
    }
    //-----------------------------------------------------------------------------
    size_t SaveMap::getMemoryFrameSize()
    {
        return mMemoryFrameSize;
    }
    //-----------------------------------------------------------------------------
    void * SaveMap::getMemoryPtr()
    {
        return mMemoryData;
    }
    //-----------------------------------------------------------------------------
    size_t SaveMap::getMemorySize()
    {
        return mMemorySize;
    }
    //-----------------------------------------------------------------------------
    size_t SaveMap::registerObject(size_t objSize)
    {
        if(!mInitialized)
        {
            // Create a temporary memory frame object.
            MemoryFrame add;
            // Add the actual memory size.
            add.mDataPtr = mMemorySize;
            // Add the object size.
            add.mDataSize = objSize;
            // And add the object size to the total size.
            mMemorySize += objSize;
            // Add the frame to the MemoryFrame list.
            mMemoryFrame.push_back(add);
            mMemoryFrameSize += sizeof(MemoryFrame);
            // Now return a pointer, that is the memory block pointer plus the offset.
            return mMemoryFrameNum++;
        }
        return NULL;
    }
    //-----------------------------------------------------------------------------
    void * SaveMap::getObject(size_t objectID)
    {
        // Check if the object exists.
        if(objectID > mMemoryFrame.size())
            // If not, then return null.
            return NULL;
        // Now return the pointer to the object.
        return (void *)((mMemoryData)+mMemoryFrame[objectID].mDataPtr);
    }
    //-----------------------------------------------------------------------------
    size_t SaveMap::getObjectSize(size_t objectID)
    {
        // Check if the object exists.
        if(objectID > mMemoryFrame.size())
            // If not, then return null.
            return NULL;
        // Now return the pointer to the object.
        return mMemoryFrame[objectID].mDataSize;
    }
    size_t SaveMap::getNumObjects()
    {
        return mMemoryFrameNum;
    }
    //-----------------------------------------------------------------------------
    void SaveMap::resetSaveMap()
    {
        if(mInitialized)
        {
            memset(mMemoryData, 0, mMemorySize);
        }
    }
    //-----------------------------------------------------------------------------
    void SaveMap::writeToObject(void * ptr, size_t size, size_t objID)
    {
        if(mInitialized)
        {
            if(mMemoryFrame[objID].mDataSize > objID)
            {
                memcpy ( (void *)(mMemoryData+mMemoryFrame[objID].mDataPtr), ptr, size );
                memset( (void *)(mMemoryData+mMemoryFrame[objID].mDataPtr + size), 0, mMemoryFrame[objID].mDataSize - size);
            } else {
                memcpy ( (void *)(mMemoryData+mMemoryFrame[objID].mDataPtr), ptr, mMemoryFrame[objID].mDataSize );
            }
        }
    }
    //-----------------------------------------------------------------------------
    void SaveMap::overwriteToObject(void * ptr, size_t size, size_t objID)
    {
        if(mInitialized)
        {
            if(mMemoryFrame[objID].mDataSize > objID)
            {
                memcpy ( (void *)(mMemoryData+mMemoryFrame[objID].mDataPtr), ptr, size );
            } else {
                memcpy ( (void *)(mMemoryData+mMemoryFrame[objID].mDataPtr), ptr, mMemoryFrame[objID].mDataSize );
            }
        }
    }
    //-----------------------------------------------------------------------------
    void SaveMap::resetObject(size_t objID)
    {
        if(mInitialized)
        {
            memset ( (void *)(mMemoryData+mMemoryFrame[objID].mDataPtr), 0, mMemoryFrame[objID].mDataSize );
        }
    }
};
