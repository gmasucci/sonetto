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

#include "SonettoMain.h"

#include <OGRE/OgreArchive.h>
#include <OGRE/OgreArchiveFactory.h>


namespace Sonetto {

    /** @brief RARC File Info structure used to store the file name, position and size
    @remarks
        These are uncompressed files.
    */
    struct SONETTO_EXPORT RARCFileInfo

    {
        /** @brief The archive in which the file has been found

            For info when performing multi-archive searches. Note that
            you should still open through Ogre::ResourceGroupManager.
		*/
		Ogre::Archive* archive;
        /// @brief File's fully qualified name
        Ogre::String filename;
        /// @brief Path name separated by '/' and ending with '/'
        Ogre::String path;
        /// @brief Base filename
        Ogre::String basename;
        /// @brief Compressed size
        size_t compressedSize;
        /// @brief Uncompressed size
        size_t uncompressedSize;
        /// @brief Data position offset inside CARC file
        size_t positionOffset;
    };

    struct SONETTO_EXPORT RARCFileEntry
    {
        size_t name_ptr;
        size_t name_size;
        size_t data_ptr;
        size_t data_size;
    };

    /** @brief Specialisation of the Archive class to allow reading of files from a rarc
        or carc format source archive.
    @remarks
        This archive format supports compressed and uncompressed archives in RARC and CARC formats.
    */
    class SONETTO_EXPORT ResourceArchive : public Ogre::Archive
    {
    public:
        /// @brief Constructor
        ResourceArchive(const Ogre::String& name, const Ogre::String& archType);
        /// @brief Destructor
        ~ResourceArchive();
        /// @brief Returns whether this archive is case sensitive in the way it matches files
        bool isCaseSensitive(void) const { return false; }

        /** @brief Loads the archive.
        @remarks
            This initializes all the internal data of the class.
        @warning
            Do not call this function directly, it is ment to be used
            only by the ArchiveManager class.
        */
        void load();

        /** @brief Unloads the archive.
        @warning
            Do not call this function directly, it is ment to be used
            only by the ArchiveManager class.
        */
        void unload();

        /** @brief Open a stream on a given file.
        @note
            There is no equivalent 'close' method; the returned stream
            controls the lifecycle of this file operation.
        @param filename The fully qualified name of the file
        @return A shared pointer to a DataStream which can be used to
            read / write the file. If the file is not present, returns a null
			shared pointer.
        */
        virtual Ogre::DataStreamPtr open(const Ogre::String& filename) const;

        /** @brief List all file names in the archive.
        @note
            This method only returns filenames, you can also retrieve other
            information using listFileInfo.
        @param recursive Whether all paths of the archive are searched (if the
            archive has a concept of that)
        @param dirs Set to true if you want the directories to be listed
            instead of files
        @return A list of filenames matching the criteria, all are fully qualified
        */
        virtual Ogre::StringVectorPtr list(bool recursive = true, bool dirs = false);

        /** @brief List all files in the archive with accompanying information.
        @param recursive Whether all paths of the archive are searched (if the
            archive has a concept of that)
        @param dirs Set to true if you want the directories to be listed
            instead of files
        @return A list of structures detailing quite a lot of information about
            all the files in the archive.
        */
        virtual Ogre::FileInfoListPtr listFileInfo(bool recursive = true, bool dirs = false);

        /** @brief Find all file or directory names matching a given pattern
            in this archive.
        @note
            This method only returns filenames, you can also retrieve other
            information using findFileInfo.
        @param pattern The pattern to search for; wildcards (*) are allowed
        @param recursive Whether all paths of the archive are searched (if the
            archive has a concept of that)
        @param dirs Set to true if you want the directories to be listed
            instead of files
        @return A list of filenames matching the criteria, all are fully qualified
        */
        virtual Ogre::StringVectorPtr find(const Ogre::String& pattern, bool recursive = true,
            bool dirs = false);

        /// @brief Find out if the named file exists (note: fully qualified filename required)
        virtual bool exists(const Ogre::String& filename);

        /** @brief Find all files or directories matching a given pattern in this
            archive and get some detailed information about them.
        @param pattern The pattern to search for; wildcards (*) are allowed
        @param recursive Whether all paths of the archive are searched (if the
        archive has a concept of that)
        @param dirs Set to true if you want the directories to be listed
            instead of files
        @return A list of file information structures for all files matching
            the criteria.
        */
        virtual Ogre::FileInfoListPtr findFileInfo(const Ogre::String& pattern,
            bool recursive = true, bool dirs = false);
    protected:
        /// @brief Just a internal function to read the sizes from the memory
        void readSize(size_t &dest, void * buffer);
        /// @brief Decompress data using Zlib (note: must be redone later)
        int _decompressData(void * inbuffer, size_t insize, void * outbuffer, size_t outsize);
        /// @brief Check 4 bytes of data to see if the file is a RARC or CARC.
        int checkHeader(char * head);
        /// @brief A list containing file entries
        std::vector<RARCFileInfo> mFileList;
        /// @brief A pointer to the uncompressed carc file in the memory.
        Ogre::MemoryDataStream * mDataStream;
    };

    /// @brief Specialisation of ArchiveFactory for RARC and CARC files.
    class SONETTO_EXPORT ResourceArchiveFactory : public Ogre::ArchiveFactory
    {
    public:
        virtual ~ResourceArchiveFactory() {}
        /** @brief Returns the factory type.
        @return
            The factory type.
        */
        const Ogre::String& getType(void) const;
        /** @brief Creates a new object.
        @param name Name of the object to create
        @return
            An object created by the factory. The type of the object depends on
            the factory.
        */
        Ogre::Archive *createInstance( const Ogre::String& name )
        {
            return new ResourceArchive(name, "RARC");
        }
        /** @brief Destroys an object which was created by this factory.
        @param ptr Pointer to the object to destroy
        */
        void destroyInstance( Ogre::Archive* arch) { delete arch; }
    };
}; // namespace Sonetto

#endif // __SONETTO_RARC__
