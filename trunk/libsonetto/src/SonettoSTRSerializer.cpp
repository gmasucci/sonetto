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
#include "SonettoSTRSerializer.h"
#include "SonettoSTR.h"
#include <OgreException.h>

namespace Sonetto
{
    //-----------------------------------------------------------------------------
    // Sonetto::STRSerializer implementation.
    //-----------------------------------------------------------------------------
    const long STREAM_OVERHEAD_SIZE = sizeof(Ogre::uint16) + sizeof(Ogre::uint32);
    //-----------------------------------------------------------------------------
    STRSerializer::STRSerializer()
    {
        // Version number
        mVersion = "STR0";
    }
    //-----------------------------------------------------------------------------
    STRSerializer::~STRSerializer()
    {

    }
    //-----------------------------------------------------------------------------
    void STRSerializer::exportSTR(const STRData *pSTR, const Ogre::String &fileName, Ogre::Serializer::Endian endianMode)
    {
        // Decide on endian mode
		Serializer::determineEndianness(endianMode);
        // Open/create the file
		mpfFile = fopen(fileName.c_str(), "wb");
		if (!mpfFile)
		{
		    // Throw an error if the file was not found, or was not possible to read
		    SONETTO_THROW("A file was not found!");
		}
        // Start the file header
		writeFileHeader();
		// Get the number of strings.
		unsigned int totalnstrings = pSTR->mMessageList.size();
		// Write the number of strings in the file.
		writeInts(&totalnstrings,1);

        // Now write every string to the file.
		for (size_t i = 0; i < pSTR->mMessageList.size()-1; ++i)
        {
            writeStr(pSTR->mMessageList[i]);
        }

        fclose(mpfFile);

    }
    //-----------------------------------------------------------------------------
    void STRSerializer::importSTR(Ogre::DataStreamPtr &stream, STRData *pDest)
    {
        // Determine endianness (must be the first thing we do!)
        std::cout<<"STREAM TELL(): "<<stream->tell()<<"\nEntering on determineEndianness(stream);\n";
		determineEndianness(stream);

		// Check header
        readFileHeader(stream);
        // Get the number of strings
        unsigned int numStrs;
        readInts(stream, &numStrs, 1);
        // Loop through the file reading all the strings.
        for(unsigned int i = 0; i < numStrs; ++i)
        {
            pDest->mMessageList.push_back(readStr(stream));
        }
    }
    //-----------------------------------------------------------------------------
    void STRSerializer::writeStr(const Ogre::String& string)
    {
        // Get the string size
        unsigned int strsize = string.size();
        // Write the string size to the file.
        writeInts(&strsize,1);
        // Now write the actual string.
        fputs(string.c_str(), mpfFile);
        // Add the zero at the end of every string.
        fputc(0x0, mpfFile);
    }
    //-----------------------------------------------------------------------------
    Ogre::String STRSerializer::readStr(Ogre::DataStreamPtr& stream)
    {
        // Variable to store the string size.
        unsigned int strsize;
        // Read the string size from the file.
        readInts(stream, &strsize, 1);
        // Now read the string.
        return readString(stream, strsize);
    }
    //-----------------------------------------------------------------------------
} // namespace
