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
        mVersion = "SntSTR000";
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

		mpfFile = fopen(fileName.c_str(), "wb");
		if (!mpfFile)
		{
		    using namespace Ogre;
			OGRE_EXCEPT(Ogre::Exception::ERR_CANNOT_WRITE_TO_FILE,
				"Unable to open file " + fileName + " for writing",
				"STRData::exportSTR");
		}

		writeFileHeader();

		for (size_t i = 0; i < pSTR->mMessageList.size()-1; ++i)
        {
            writeChunkHeader(0xFFFF, STREAM_OVERHEAD_SIZE + pSTR->mMessageList[i].size());
            writeString(pSTR->mMessageList[i]);
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

        unsigned short streamID;
        while(!stream->eof())
        {
            streamID = readChunk(stream);
            switch (streamID)
            {
                case 0xFFFF:
                    pDest->mMessageList.push_back(readString(stream));
                    std::cout<<"Message:\n"<<pDest->mMessageList.back()<<"\n";
                break;
            }
        }
    }
    //-----------------------------------------------------------------------------
} // namespace
