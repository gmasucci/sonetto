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
#ifndef _SONETTO_SAVEMAP_H_
#define _SONETTO_SAVEMAP_H_

#include "SonettoMain.h"

namespace Sonetto {
    /** @brief MemoryFrame for use by the SaveMap,
        it stores each SaveMap object's pointer and size.
        (Note that it stores a local pointer)
    */
    struct SONETTO_EXPORT MemoryFrame
    {
        /// @brief Local pointer to the object inside a SaveMap.
        size_t mDataPtr;
        /// @brief The size of the object inside a SaveMap.
        size_t mDataSize;
    };
    /** @brief Save memory block

        The SaveMap is a static memory block that must be initialized
        before using, also note that you can't change it's structure or
        size after the initialization.

        The SaveMap uses a MemoryFrame to identify every object inside
        it.
    */
    class SONETTO_EXPORT SaveMap
    {
    public:
        /// @brief Constructor.
        SaveMap();
        /// @brief Destructor.
        ~SaveMap();
        /** @brief Initialize the SaveMap, after the save map is initialized
            it will not be possible to modify the structure!
        */
        void initialize();
        /// @brief Return a pointer to the start of the memory frame.
        void * getMemoryFramePtr();
        /// @brief Return the Memory Frame size.
        size_t getMemoryFrameSize();
        /// @brief Return a pointer to the start of the memory block.
        void * getMemoryPtr();
        /// @brief Return the Memory Block size.
        size_t getMemorySize();
        /** @brief Register a object in the memory module

            It returns its ID. Use it to get the pointer
            after the SaveMap is initialized.
            This MUST be done before initializing!
        */
        size_t registerObject(size_t objSize);
        /// @brief Get a pointer to the object inside the memory frame
        void * getObject(size_t objectID);
        /// @brief Return the object size.
        size_t getObjectSize(size_t objectID);
        /// @brief Return the number of objects.
        size_t getNumObjects();
        /** @brief Reset the SaveMap data,
            Only works when it is already initialized,
            It will empty all the SaveMap, but will not
            reset the MemoryFrame.
        */
        void resetSaveMap();
        /** @brief Write data to a object in SaveMap.
            If the original object is bigger than the destiny object
            it will crop everything bigger, if it's smaller, it will
            fill the space with zeroes.
        */
        void writeToObject(void * ptr, size_t size, size_t objID);
        /** @brief Overwrite data to a object in SaveMap.
            Similar to the writeToMemory(), it will crop data bigger than
            the destiny object, but if the object is smaller, it will
            leave the original contents and only overwrite part of it.
        */
        void overwriteToObject(void * ptr, size_t size, size_t objID);
        /// @brief Reset the specified object to zero.
        void SaveMap::resetObject(size_t objID);
    private:
        /// @brief Check if it's initialized.
        bool mInitialized;
        /// @brief A pointer to the beginning of the memory frame.
        void * mMemoryFrameStart;
        /// @brief The total size of the memory block.
        size_t mMemoryFrameSize;
        /// @brief Total number of elements in the SaveMap.
        size_t mMemoryFrameNum;
        /// @brief The total size of the memory block.
        size_t mMemorySize;
        /// @brief Stores the MemoryFrame.
        std::vector<MemoryFrame> mMemoryFrame;
        /// @brief Stores the actual SaveMap data.
        char * mMemoryData;
    };

}; // namespace Sonetto

#endif // _SONETTO_SAVEMAP_H_
