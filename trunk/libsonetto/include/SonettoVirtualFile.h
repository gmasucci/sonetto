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

#ifndef SONETTO_VIRTUALFILE_H
#define SONETTO_VIRTUALFILE_H

#include "SonettoMain.h"

#include <cstddef>
#include <vorbis/vorbisfile.h>

namespace Sonetto {
    /** @brief Virtual file
     *
     *  A virtual file is a class SONETTO_EXPORT used by the AudioManager to present
     *  functions for Vorbisfile library to treat RAM memory addresses as
     *  files. This way files loaded into memory by Ogre can serve
     *  as virtual OGG Vorbis files to be decompressed.
     *  @remarks Virtual files are read-only.
     */
    class SONETTO_EXPORT VirtualFile {
    public:
        /** @brief Construct SONETTO_EXPORTor
         *
         *  @param _address Address of to the beginning of the file.
         *  @param _length  The actual file size.
         */
        VirtualFile(void *_address,size_t _length) :
                address(_address),pos(0),length(_length) {}

        /// @brief VirtualFile's std::fread() equivalent
        static size_t read(void *dest,size_t size,size_t times,VirtualFile *file);

        /// @brief VirtualFile's std::fseek() 64bits equivalent
        static int    seek(VirtualFile *file,ogg_int64_t offset,int whence);

        /// @brief VirtualFile's std::fclose() equivalent
        static int    close(VirtualFile *file);

        /// @brief VirtualFile's std::ftell() equivalent
        static long   tell(const VirtualFile *file);

    private:
        /// @brief Address of to the beginning of the file
        void        *address;

        /// @brief Current cursor position in file
        ogg_int64_t  pos;

        /// @brief The actual file size
        ogg_int64_t  length;
    };
} // namespace

#endif
