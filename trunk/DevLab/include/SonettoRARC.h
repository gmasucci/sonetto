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

#ifndef __SONETTO_RARC__
#define __SONETTO_RARC__

#include "OgreArchive.h"
#include "OgreArchiveFactory.h"

namespace Sonetto {

    /** RARC File Info Structure used to store the file name, position and size
    @remarks
        These are uncompressed files.
    */
    struct RARCFileInfo

    {
        /// The archive in which the file has been found (for info when performing
		/// multi-Archive searches, note you should still open through ResourceGroupManager)
		Ogre::Archive* archive;
        /// The file's fully qualified name
        Ogre::String filename;
        /// Path name; separated by '/' and ending with '/'
        Ogre::String path;
        /// Base filename
        Ogre::String basename;
        /// Compressed size
        size_t compressedSize;
        /// Uncompressed size
        size_t uncompressedSize;
        /// Data position offset inside the carc file
        size_t positionOffset;
    };

    struct RARCFileEntry
    {
        size_t name_ptr;
        size_t name_size;
        size_t data_ptr;
        size_t data_size;
    };

    /** Specialisation of the Archive class to allow reading of files from a rarc
        or carc format source archive.
    @remarks
        This archive format supports compressed and uncompressed archives in RARC and CARC formats.
    */
    class ResourceArchive : public Ogre::Archive
    {
    public:
        /// Constructor
        ResourceArchive(const Ogre::String& name, const Ogre::String& archType);
        /// Destructor
        ~ResourceArchive();
        /// Returns whether this archive is case sensitive in the way it matches files
        bool isCaseSensitive(void) const { return false; }

        /** Loads the archive.
        @remarks
            This initializes all the internal data of the class.
        @warning
            Do not call this function directly, it is ment to be used
            only by the ArchiveManager class.
        */
        void load();

        /** Unloads the archive.
        @warning
            Do not call this function directly, it is ment to be used
            only by the ArchiveManager class.
        */
        void unload();

        /** Open a stream on a given file.
        @note
            There is no equivalent 'close' method; the returned stream
            controls the lifecycle of this file operation.
        @param filename The fully qualified name of the file
        @returns A shared pointer to a DataStream which can be used to
            read / write the file. If the file is not present, returns a null
			shared pointer.
        */
        virtual Ogre::DataStreamPtr open(const Ogre::String& filename) const;

        /** List all file names in the archive.
        @note
            This method only returns filenames, you can also retrieve other
            information using listFileInfo.
        @param recursive Whether all paths of the archive are searched (if the
            archive has a concept of that)
        @param dirs Set to true if you want the directories to be listed
            instead of files
        @returns A list of filenames matching the criteria, all are fully qualified
        */
        virtual Ogre::StringVectorPtr list(bool recursive = true, bool dirs = false);

        /** List all files in the archive with accompanying information.
        @param recursive Whether all paths of the archive are searched (if the
            archive has a concept of that)
        @param dirs Set to true if you want the directories to be listed
            instead of files
        @returns A list of structures detailing quite a lot of information about
            all the files in the archive.
        */
        virtual Ogre::FileInfoListPtr listFileInfo(bool recursive = true, bool dirs = false);

        /** Find all file or directory names matching a given pattern
            in this archive.
        @note
            This method only returns filenames, you can also retrieve other
            information using findFileInfo.
        @param pattern The pattern to search for; wildcards (*) are allowed
        @param recursive Whether all paths of the archive are searched (if the
            archive has a concept of that)
        @param dirs Set to true if you want the directories to be listed
            instead of files
        @returns A list of filenames matching the criteria, all are fully qualified
        */
        virtual Ogre::StringVectorPtr find(const Ogre::String& pattern, bool recursive = true,
            bool dirs = false);

        /// Find out if the named file exists (note: fully qualified filename required)
        virtual bool exists(const Ogre::String& filename);

        /** Find all files or directories matching a given pattern in this
            archive and get some detailed information about them.
        @param pattern The pattern to search for; wildcards (*) are allowed
        @param recursive Whether all paths of the archive are searched (if the
        archive has a concept of that)
        @param dirs Set to true if you want the directories to be listed
            instead of files
        @returns A list of file information structures for all files matching
            the criteria.
        */
        virtual Ogre::FileInfoListPtr findFileInfo(const Ogre::String& pattern,
            bool recursive = true, bool dirs = false);
    protected:
        /// Just a internal function to read the sizes from the memory
        void readSize(size_t &dest, void * buffer);
        /// Decompress data using Zlib (note: must be redone later)
        int _decompressData(void * inbuffer, size_t insize, void * outbuffer, size_t outsize);
        /// Check 4 bytes of data to see if the file is a RARC or CARC.
        int checkHeader(char * head);
        /// A list containing file entries
        std::vector<RARCFileInfo> mFileList;
        /// A pointer to the uncompressed carc file in the memory.
        Ogre::MemoryDataStream * mDataStream;
    };

    /// Specialisation of ArchiveFactory for RARC and CARC files.
    class ResourceArchiveFactory : public Ogre::ArchiveFactory
    {
    public:
        virtual ~ResourceArchiveFactory() {}
        /** Returns the factory type.
            @return
                The factory type.
        */
        const Ogre::String& getType(void) const;
        /** Creates a new object.
        @param name Name of the object to create
        @return
            An object created by the factory. The type of the object depends on
            the factory.
        */
        Ogre::Archive *createInstance( const Ogre::String& name )
        {
            return new ResourceArchive(name, "RARC");
        }
        /** Destroys an object which was created by this factory.
        @param ptr Pointer to the object to destroy
        */
        void destroyInstance( Ogre::Archive* arch) { delete arch; }
    };
}; // namespace Sonetto

#endif // __SONETTO_RARC__
