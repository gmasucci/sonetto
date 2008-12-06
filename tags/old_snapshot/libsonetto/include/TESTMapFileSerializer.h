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
#ifndef TEST_MAPFILESERIALIZER_H
#define TEST_MAPFILESERIALIZER_H

#include "TESTMapFile.h"
#include <OgreSerializer.h>

// If it's the client, then import the library from the dll, export otherwise.
#if defined( WIN32 )
#   if defined( SONETTO_DLL_BUILD )
#        define SONETTO_EXPORT __declspec( dllexport )
#   else
#       define SONETTO_EXPORT __declspec( dllimport )
#   endif
#else
#   define SONETTO_EXPORT
#endif

namespace Sonetto {

        class MapFile; // forward declaration

        class SONETTO_EXPORT MapFileSerializer : public Ogre::Serializer
            {

                public:
                    MapFileSerializer();
                    virtual ~MapFileSerializer();

                    void exportMapFile ( const MapFile *pMap, const Ogre::String &fileName, Ogre::Serializer::Endian endianMode = ENDIAN_NATIVE );
                    void importMapFile ( Ogre::DataStreamPtr &stream, MapFile *pDest );

                    void writeMapLayer ( const MapLayerFile &maplayer );
                    void readMapLayer ( Ogre::DataStreamPtr& stream, MapLayerFile &pDest );
            };

    }; // namespace.

#endif // TEST_MAPFILESERIALIZER_H
