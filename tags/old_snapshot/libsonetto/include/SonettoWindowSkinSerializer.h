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
#ifndef _SONETTO_WINDOWSKIN_SERIALIZER_H_
#define _SONETTO_WINDOWSKIN_SERIALIZER_H_

#include "SonettoMain.h"
#include <OgreSerializer.h>
#include "SonettoWindowSkin.h"

namespace Sonetto {

    class SONETTO_EXPORT WindowSkin; // forward declaration
    class SONETTO_EXPORT WindowSkinSerializer : public Ogre::Serializer {
    public:
        WindowSkinSerializer();
        ~WindowSkinSerializer();

        void exportWindowSkin(const WindowSkin *pWindowSkin, const Ogre::String &fileName, Ogre::Serializer::Endian endianMode = ENDIAN_NATIVE);
        void importWindowSkin(Ogre::DataStreamPtr &stream, WindowSkin *pDest);
    protected:
        void writeTexCoords(const WindowTexCoord& vec);
        void writeVector2(const Ogre::Vector2& vec);
        void writeColorValue(const Ogre::ColourValue& vec);
        void readTexCoords(Ogre::DataStreamPtr &stream, WindowTexCoord &vec);
        void readVector2(Ogre::DataStreamPtr &stream, Ogre::Vector2 &pDest);
        void readColorValue(Ogre::DataStreamPtr &stream, Ogre::ColourValue &pDest);
    };

}; // namespace

#endif // _SONETTO_WINDOWSKIN_SERIALIZER_H_
