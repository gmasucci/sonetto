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

// If it's the client, then import the library from the dll, export otherwise.
#if defined( WIN32 )
#   if defined( MODULE_DLL )
#        define MODULE_EXPORT __declspec( dllexport )
#   else
#       define MODULE_EXPORT __declspec( dllimport )
#   endif
#endif

#include <OgreSerializer.h>
#include "TESTMapFile.h"

namespace Sonetto{

    class MapFile; // forward declaration

    class MODULE_EXPORT MapFileSerializer : public Ogre::Serializer
    {
    public:
        MapFileSerializer();
        virtual ~MapFileSerializer();

        void exportMapFile(const MapFile *pText, const Ogre::String &fileName);
        void importMapFile(Ogre::DataStreamPtr &stream, MapFile *pDest);

        void writeMapLayer(MapLayerFile &maplayer);
        void readMapLayer(Ogre::DataStreamPtr& stream,MapLayerFile &pDest);
    protected:
        static Ogre::String msCurrentVersion;
    };

}; // namespace.

#endif // TEST_MAPFILESERIALIZER_H
