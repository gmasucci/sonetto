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

#include "SonettoRARC.h"
#include "zlib.h"

namespace Sonetto {
    //-----------------------------------------------------------------------------
    // Sonetto::ResourceArchive Implementation
    //-----------------------------------------------------------------------------
    ResourceArchive::ResourceArchive(const Ogre::String& name, const Ogre::String& archType)
        : Ogre::Archive(name, archType), mDataStream(0)
    {
    }
    //-----------------------------------------------------------------------------
    ResourceArchive::~ResourceArchive()
    {
        unload();
    }
    //-----------------------------------------------------------------------------
    void ResourceArchive::load()
    {
        // Use filesystem to determine size
        // (quicker than streaming to the end and back)
        struct stat tagStat;
        int ret = stat(mName.c_str(), &tagStat);
        assert(ret == 0 && "Problem getting file size" );

        // Create a pointer to a file stream and open it always in binary mode
        std::ifstream * srcStream = new std::ifstream();
        srcStream->open(mName.c_str(), std::ios::in | std::ios::binary);

        // Check to see if the file was opened properly
        if (srcStream->fail())
        {
            delete srcStream;
            assert("Problem while trying to open file..." );
        }

        char tmpHead[4];
        // Temporary Buffer to store compressed data to processing
        char * tempData = new char[tagStat.st_size];
        // Open the file
        std::ifstream * fdata = new std::ifstream(mName.c_str(), std::ios_base::in | std::ios_base::binary);
        // Read the header
        fdata->read((char*)&tmpHead, sizeof(char)*4);
        // Read the data to temporary buffer
        fdata->seekg(0, std::ios_base::beg);
        fdata->read(tempData, tagStat.st_size);

        switch(checkHeader((char*)&tmpHead))
        {
            case 1:
            {
                // decompress the file before copying it to the Data Stream
                size_t uncSize, compSize = 0;
                readSize(uncSize, tempData+4);
                readSize(compSize, tempData+4+sizeof(size_t));
                char * uncmpTempData = new char[uncSize];

                int ret = _decompressData(tempData+4+(sizeof(size_t)*2), compSize, uncmpTempData, uncSize);

                mDataStream = new Ogre::MemoryDataStream(mName, uncmpTempData, uncSize, true);

                delete[] tempData;
            }
            break;
            case 0:
            {
                // Construct the file stream, tell it to delete on destroy
                mDataStream = new Ogre::MemoryDataStream(mName, tempData, tagStat.st_size, true);
            }
            break;
            default:
            {
                delete[] tempData;
                tempData = 0;
                fdata->close();
                assert("Invalid file type");
                return;
            }
            break;
        }

        // Skip the header and the two dummies, we've read it already.
        mDataStream->seek(4 + (sizeof(size_t)*2));
        // Read the number of files contained in this archive.
        size_t numFiles = 0;
        mDataStream->read(&numFiles, sizeof(size_t));
        for(size_t i = 0; i < numFiles; ++i)
        {
            RARCFileEntry tmpFileEntry;
            RARCFileInfo tmpFileInfo;
            mDataStream->read(&tmpFileEntry, sizeof(RARCFileEntry));
            // Save the current cursor position.
            size_t sCurPos = mDataStream->tell();
            mDataStream->seek(tmpFileEntry.name_ptr);
            // Now try to read the file name
            Ogre::String tmpFileName;
            Ogre::String tmpFilePath;
            Ogre::String tmpFileBaseName;
            // Copy the string from the stream.
            tmpFileName.append((char*)mDataStream->getCurrentPtr(), tmpFileEntry.name_size);
            // Return to the original position
            mDataStream->seek(sCurPos);
            // Now copy everything to File Info
            tmpFileInfo.archive = this;
            tmpFileInfo.filename = tmpFileName;
            Ogre::StringUtil::splitFilename(tmpFileName, tmpFilePath, tmpFileBaseName);
            tmpFileInfo.path = tmpFilePath;
            tmpFileInfo.basename = tmpFileBaseName;
            tmpFileInfo.compressedSize = tmpFileEntry.data_size;
            tmpFileInfo.uncompressedSize = tmpFileEntry.data_size;
            tmpFileInfo.positionOffset = tmpFileEntry.data_ptr;
            // And add it to the file list
            mFileList.push_back(tmpFileInfo);
        }

    }
    //-----------------------------------------------------------------------------
    void ResourceArchive::unload()
    {
        // Close the stream, deleting the data stored in it.
        mDataStream->close();
    }
    //-----------------------------------------------------------------------------
    Ogre::DataStreamPtr ResourceArchive::open(const Ogre::String& filename) const
    {
        //create a iterator
        std::vector<RARCFileInfo>::const_iterator i, iend;
        iend = mFileList.end();
        // loop and stop if the file is found
        for (i = mFileList.begin(); i != iend; ++i)
        {
            if(i->filename == filename)
                break;
        }
        // Create a new Memory Data Stream and point it to the file.
        Ogre::MemoryDataStream * mds = new Ogre::MemoryDataStream(filename, mDataStream->getPtr() + i->positionOffset, i->uncompressedSize, false);
        // return it as an DataStreamPtr
        return Ogre::DataStreamPtr(mds);
    }
    //-----------------------------------------------------------------------------
    Ogre::StringVectorPtr ResourceArchive::list(bool recursive, bool dirs)
    {
        // Create a String Vector to return
        Ogre::StringVectorPtr ret = Ogre::StringVectorPtr(new Ogre::StringVector());
        std::vector<RARCFileInfo>::const_iterator itr;
        // Iterate through all files
        for(itr = mFileList.begin(); itr != mFileList.end(); ++itr)
        {
            // put all filenames in the String Vector.
            ret->push_back(itr->filename);
        }
        // Return the StringVectorPtr.
        return ret;
    }
    //-----------------------------------------------------------------------------
    Ogre::FileInfoListPtr ResourceArchive::listFileInfo(bool recursive, bool dirs)
    {
        // Create an file info list
        Ogre::FileInfoList* fil = new Ogre::FileInfoList();
        // Create the iterator
        std::vector<RARCFileInfo>::const_iterator i, iend;
        iend = mFileList.end();
        // Loop through all files
        for (i = mFileList.begin(); i != iend; ++i)
        {
            // manually convert from RARCFileInfo to FileInfo
            Ogre::FileInfo ret;
            ret.filename = i->filename;
            ret.path = i->path;
            ret.basename = i->basename;
            ret.compressedSize = i->compressedSize;
            ret.uncompressedSize = i->uncompressedSize;
            // put the FileInfo in the list
            fil->push_back(ret);
        }
        // Return it as an FileInfoListPtr.
        return Ogre::FileInfoListPtr(fil);
    }
    //-----------------------------------------------------------------------------
    Ogre::StringVectorPtr ResourceArchive::find(const Ogre::String& pattern, bool recursive, bool dirs)
    {
        // Create a String Vector to return
        Ogre::StringVectorPtr ret = Ogre::StringVectorPtr(new Ogre::StringVector());
        std::vector<RARCFileInfo>::const_iterator itr;
        // Iterate through all files
        for(itr = mFileList.begin(); itr != mFileList.end(); ++itr)
        {
            // Put all files wich contains the pattern in the list
            if(itr->filename == pattern)
                ret->push_back(itr->filename);
        }
        // Return the list
        return ret;
    }
    //-----------------------------------------------------------------------------
    Ogre::FileInfoListPtr ResourceArchive::findFileInfo(const Ogre::String& pattern, bool recursive, bool dirs)
    {
        // Create an file info list
        Ogre::FileInfoList * fil = new Ogre::FileInfoList();
        // Create the iterator
        std::vector<RARCFileInfo>::const_iterator i, iend;
        iend = mFileList.end();
        // Iterate through all files
        for (i = mFileList.begin(); i != iend; ++i)
        {
            // Get all files wich contains the pattern
            if(i->filename == pattern)
            {
                // manually convert from RARCFileInfo to FileInfo.
                Ogre::FileInfo ret;
                ret.filename = i->filename;
                ret.path = i->path;
                ret.basename = i->basename;
                ret.compressedSize = i->compressedSize;
                ret.uncompressedSize = i->uncompressedSize;
                // put the FileInfo in the list.
                fil->push_back(ret);
            }
        }
        return Ogre::FileInfoListPtr(fil);
    }
    //-----------------------------------------------------------------------------
    bool ResourceArchive::exists(const Ogre::String& filename)
    {
        // Create an iterator
        std::vector<RARCFileInfo>::const_iterator i, iend;
        iend = mFileList.end();
        // Loop through all files
        for (i = mFileList.begin(); i != iend; ++i)
        {
            // return true if a file with this name is found
            if(i->filename == filename)
                return true;
        }
        // if the loop reach the end and no file is found, return false.
        return false;
    }
    //-----------------------------------------------------------------------------
    void ResourceArchive::readSize(size_t &dest, void * buffer)
    {
        // simply copy from the buffer to the destiny.
        memcpy(&dest, buffer, sizeof(size_t));
    }
    //-----------------------------------------------------------------------------
    int ResourceArchive::_decompressData(void * inbuffer, size_t insize, void * outbuffer, size_t outsize)
    {
        // ret: zlib return codes
        int ret;
        // strm structure is used to pass information to and from the zlib routines
        z_stream strm;
        // Allocate the default state
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        strm.avail_in = insize;
        strm.next_in = static_cast<Bytef*>(inbuffer);
        // Initiate the inflate process
        ret = inflateInit(&strm);
        // check to se if everything is allright, if not, then return the error
        if (ret != Z_OK)
            return ret;

        // set the output
        strm.avail_out = outsize;
        strm.next_out = static_cast<Bytef*>(outbuffer);

        // decompress the data
        ret = inflate(&strm, Z_FINISH);

        // check to see if there is any error
        assert(ret != Z_STREAM_ERROR);
        switch (ret) {
            case Z_NEED_DICT:
                ret = Z_DATA_ERROR;     // and fall through
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                (void)inflateEnd(&strm); // Finish the inflate process
                return ret; // Return the status
            case Z_STREAM_END:
                ret = inflateEnd(&strm); // Finish the inflate process
                return ret; // Return the status
        }

    }
    //-----------------------------------------------------------------------------
    int ResourceArchive::checkHeader(char * head)
    {
        // Check if the header is a RARC file
        if( head[0] == 'R' &&  head[1] == 'A' &&  head[2] == 'R' &&  head[3] == 'C' )
        {
            // Return 1 if its true
            return 1;
        }
        // Otherwise check to see if its an CARC file
        else if ( head[0] == 'C' &&  head[1] == 'A' &&  head[2] == 'R' &&  head[3] == 'C' )
        {
            // Return 0 if its true
            return 0;
        }
        // If the file is not an RARC nor an CARC, return an error (unknown file type)
        else
        {
            // Return -1 for an error
            return -1;
        }
    }
    //-----------------------------------------------------------------------------
    // ResourceArchiveFactory Implementation
    //-----------------------------------------------------------------------------
    const Ogre::String& ResourceArchiveFactory::getType(void) const
    {
        // Return the filetype.
        static Ogre::String name = "RARC";
        return name;
    }
}; // namespace Sonetto
