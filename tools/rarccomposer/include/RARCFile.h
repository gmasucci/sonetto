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

#ifndef RARCFILE_H
#define RARCFILE_H

#include <wx/file.h>
#include <wx/wfstream.h>
#include <vector>

#define ZLIB_CHUNK 16384

/// File Header Structure
struct rarc_header {
    char         byte4[4];
    unsigned int dummy0;
    unsigned int dummy1;
    unsigned int file_count;
};

/// File Entry Data
struct rarc_file_entry {
    wxString    name;
    size_t      name_size;
    const char *data_ptr;
    size_t      data_size;
};

struct rarc_file_entry_out {
    size_t name_ptr;
    size_t name_size;
    size_t data_ptr;
    size_t data_size;
};

class RARCFile {
public:
    /// Default constructor
    RARCFile(char compTag[4],char uncompTag[4]);

    /// Default destructor
    ~RARCFile();

    /// Add a file entry to archive
    void addFileEntry(wxString name,size_t namelen,const char *data,size_t datalen);

    /// Get a file entry from archive
    void getFileEntry(); // <todo>

    /// Write data interface
    bool writeData(wxString filename,bool compressed = true);

    /// Read data interface
    bool readData(wxString filename,bool compressed = true); // <todo>

private:
    /// Write data to the buffer
    bool write(const void *buffer,size_t size);

    inline char   *getBufferPtr()        { return &mFileBuffer[0];     }
    inline size_t  getBufferSize() const { return  mFileBuffer.size(); }

    bool  writePlainData(wxFile *file);
    bool  readPlainData(wxFile *file);

    char *compressData(size_t &size);

    /// RARC header
    char mRARCHeader[4];

    /// File Header
    rarc_header mHeader;

    /// File Entry Vector
    std::vector<rarc_file_entry> mFileEntry;

    /// File Output Buffer
    std::vector<char> mFileBuffer;
};

#endif // RARCFILE_H
