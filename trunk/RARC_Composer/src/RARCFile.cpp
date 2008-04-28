#include <iostream>
#include <wx/log.h>
#include "zlib.h"
#include "RARCFile.h"

using namespace std;

RARCFile::RARCFile(char compTag[4],char uncompTag[4]) {
    // Hacky way to initialise archive HEADER
    memcpy(&mRARCHeader[0],compTag,4);
    memset(&mHeader,0x00,sizeof(mHeader));
    memcpy(&mHeader,uncompTag,4);
}

RARCFile::~RARCFile() {
    // <todo>
}

void RARCFile::addFileEntry(wxString name,size_t namelen,const char *data,size_t datalen) {
    rarc_file_entry fileEntry; // New file entry

    // Creates a file entry based on supplied arguments
    fileEntry.name      = name;
    fileEntry.name_size = namelen;
    fileEntry.data_ptr  = data;
    fileEntry.data_size = datalen;

    // Adds to file entry list and updates header information
    mFileEntry.push_back(fileEntry);
    mHeader.file_count = mFileEntry.size();
}

void RARCFile::getFileEntry() {
    // <todo>
}

bool RARCFile::writeData(wxString filename,bool compressed) {
    if(!mFileEntry.size()) {
        wxLogError("This file is empty!\nYou can't create an empty file.");
        return false;
    }

    wxFile *file = new wxFile(filename,wxFile::write);
    if (!file->IsOpened()) {
        delete file;
        return false;
    }

    writePlainData(file);
    if (compressed) {
        size_t  outBufferSize;
        size_t oOutBufferSize = getBufferSize();
        char   *outBuffer = compressData(outBufferSize);

        // Checks for errors
        if (!outBuffer)
            return false;

        // Writes RARC header (<todo> shouldn't this be done in _writePlainData()?)
        file->Write(mRARCHeader,sizeof(char)*4);
        file->Write(&oOutBufferSize, sizeof(size_t));
        file->Write(&outBufferSize, sizeof(size_t));
        std::cout << "Out Size: "   << outBufferSize     << "\n";
        std::cout << "Out Buffer: " << (void *)outBuffer << "\n";

        // Writes compressed data and frees buffer
        file->Write(outBuffer,outBufferSize);
        delete outBuffer;
    } else {
        // Writes current data buffer "as is" (without compressing)
        file->Write(getBufferPtr(),getBufferSize());
    }

    file->Close();
    return true;
}

bool RARCFile::readData(wxString filename,bool compressed) {
    // <todo>
    return false;
}

bool RARCFile::writePlainData(wxFile *file) {
    // Sets the file_count to ensure we got the correct number of files
    mHeader.file_count = mFileEntry.size();

    // Writes the header
    write(&mHeader,sizeof(rarc_header));

    // Creates an iterator to iterate through the File Entry Vector
    std::vector<rarc_file_entry>::iterator fileItr;

    // File Entries that contains "pointers" to the data in file
    std::vector<rarc_file_entry_out>           fileEntryOut;
    std::vector<rarc_file_entry_out>::iterator outItr;

    // We'll handle the String Pointer and Data Pointer separately
    std::vector<size_t> stringPointer;
    std::vector<size_t> dataPointer;

    // Sets variables' reference offset
    // This is the String Table Offset, the position in file where the strings are stored.
    size_t strTableOffset = 0;

    // This is the Data Offset, where all data will be stored
    size_t dataOffset = 0;

    // Archive structure:
    // [ Header ]
    // [ rarc_file_entry_outs ]
    // [ String Table ]
    // [ Data ]

    // Adds the header length to the String Table Offset
    strTableOffset += sizeof(rarc_header);

    // Now add the file entry value multiplied by the number of file entries
    strTableOffset += sizeof(rarc_file_entry_out)*mFileEntry.size();

    // Temporary string offset
    // This variable represents the position of a given string in the file
    // Every string begins at the beginning of the String Table Offset,
    // so we set it this way here
    size_t sof = strTableOffset;

    // Iterate and set the pointers for the string table
    for(fileItr = mFileEntry.begin();fileItr < mFileEntry.end();++fileItr) {
        // Adds string offset to vector
        stringPointer.push_back(sof);
        std::cout << "PTR 0: " << sof << "\n";

        // Increments by the length of the string it represents,
        // so that on the next iteration, it will be a pointer
        // to the next string
        sof += fileItr->name_size;
        std::cout << "PTR 1: " << sof << "\n";
    }

    // Sets the data offset
    // The data offset is just below the string table, so we set
    // it to our last string offset
    dataOffset = sof;

    // And creates a temporary variable for each data in the data offset
    // (just like we did with `strTableOffset')
    size_t dof = dataOffset;

    // Now itearte to set the Data Pointers
    for(fileItr = mFileEntry.begin(); fileItr < mFileEntry.end(); ++fileItr) {
        // Adds data offset to vector
        dataPointer.push_back(dof);

        // Increments by the length of the data it points to
        dof += fileItr->data_size;
    }

    // Now that we've got the String Table pointers and Data pointers, we have to
    // setup the File Entry Outputs (structures that hold these informations)
    for(size_t i = 0; i < mFileEntry.size(); ++i) {
        rarc_file_entry_out outFE;

        outFE.name_ptr = stringPointer[i];
        std::cout << "NAME PTR: "  << outFE.name_ptr  << "\n";
        outFE.name_size = mFileEntry[i].name_size;
        std::cout << "NAME SIZE: " << outFE.name_size << "\n";
        outFE.data_ptr = dataPointer[i];
        std::cout << "DATA PTR: "  << outFE.data_ptr  << "\n";
        outFE.data_size = mFileEntry[i].data_size;
        std::cout << "DATA SIZE: " << outFE.data_size << "\n";

        fileEntryOut.push_back(outFE);
    }

    // Now we finally start writing the file entries to our uncompressed buffer
    for(outItr = fileEntryOut.begin();outItr < fileEntryOut.end();++outItr)
        write(&(*outItr),sizeof(rarc_file_entry_out));

    // And the String Table
    for(fileItr = mFileEntry.begin();fileItr < mFileEntry.end();++fileItr)
        write(fileItr->name.c_str(),fileItr->name_size);

    // And the actual data
    for(fileItr = mFileEntry.begin(); fileItr < mFileEntry.end(); ++fileItr)
        write(fileItr->data_ptr,fileItr->data_size);

    // We're finished
    return true;
}

bool RARCFile::readPlainData(wxFile *file) {
    // <todo>
    return false;
}

char *RARCFile::compressData(size_t &size) {
    char     *buffer; // Compressed data buffer
    z_stream  strm;   // strm structure is used to pass information to and from the zlib routines

    // Allocates default state
    strm.zalloc = Z_NULL;
    strm.zfree  = Z_NULL;
    strm.opaque = Z_NULL;

    // Feeds stream with data to be compressed
    strm.next_in  = (Bytef *)(getBufferPtr());
    strm.avail_in = getBufferSize();

    // Creates a temporary buffer to hold compressed data
    // The size of compressed data will never be greater than uncompressed size
    buffer = new char[getBufferSize()];

    // Lets zlib stream know where to put uncompressed data,
    // and how much room there is available for writing
    strm.next_out  = (Bytef *)buffer;
    strm.avail_out = getBufferSize();

    // Initialises deflate process
    if (deflateInit(&strm,-1) != Z_OK) {
        // Cleanup if something bad happens
        delete buffer;
        return NULL;
    }

    // Deflate
    deflate(&strm,Z_FINISH);
    size = strm.total_out;
    deflateEnd(&strm);

    return buffer;
}

bool RARCFile::write(const void *buffer,size_t size) {
    size_t last = mFileBuffer.size();

    // Tries to allocate memory for the buffer to be written
    // A bad_alloc exception is thrown if we run out of memory
    try {
        mFileBuffer.resize(last+size);
    } catch(exception &e) {             // bad_alloc
        return false;
    }

    // Writes buffer
    memcpy(&mFileBuffer[last],buffer,size);
    return true;
}
