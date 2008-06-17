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

#include <cstddef>
#include <memory>
#include <vorbis/vorbisfile.h>
#include "SonettoVirtualFile.h"

namespace Sonetto {
    size_t VirtualFile::read(void *dest,size_t size,size_t times,VirtualFile *file) {
        size_t readSize = size*times;

        // Truncates number of bytes to be read, not allowing
        // `size' to get past the file size
        if (file->pos+readSize > file->length)
            readSize = file->length - file->pos;

        // Copies("reads") the desired memory chunk
        memcpy(dest,(char *)(file->address)+file->pos,readSize);

        // Moves cursor position in file
        file->pos += readSize;

        // Returns number of bytes read
        return readSize;
    }

    int VirtualFile::seek(VirtualFile *file,ogg_int64_t offset,int whence) {
        ogg_int64_t cur;

        // SEEK_CUR -> Seeks from current cursor position
        // SEEK_SET -> Seeks from beginning
        // SEEK_END -> Seeks from ending
        switch (whence) {
        case SEEK_CUR:
            cur = file->pos;
            break;
        case SEEK_SET:
            cur = 0;
            break;
        case SEEK_END:
            cur = file->length;
            break;

        default:
            return -1;
            break;
        }

        // Moves the cursor to desired position
        cur += offset;

        // Clamps it between the acceptable range (from beginning to the end of file)
        if (cur < 0)
            cur = 0;
        if (cur > file->length)
            cur = file->length;

        file->pos = cur;
        return 0;
    }

    int VirtualFile::close(VirtualFile *file) {
        // Simply delete class SONETTO_EXPORT
        delete file;
        return 0;
    }

    long VirtualFile::tell(const VirtualFile *file) {
        // Returns current cursor position
        return file->pos;
    }
} // namespace
