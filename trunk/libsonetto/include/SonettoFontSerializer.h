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

#ifndef FONTSERIALIZER_H_INCLUDED
#define FONTSERIALIZER_H_INCLUDED

#include <OGRE/OgreSerializer.h>

#include "SonettoMain.h"

namespace Sonetto {
    class SONETTO_EXPORT Font; // forward declaration

    class SONETTO_EXPORT FontSerializer : public Ogre::Serializer
    {
    public:
        FontSerializer();
        virtual ~FontSerializer();

        void exportFont(const Font *pFont, const Ogre::String &fileName, Ogre::Serializer::Endian endianMode = ENDIAN_NATIVE);
        void importFont(Ogre::DataStreamPtr &stream, Font *pDest);
    protected:
        void readMain(Ogre::DataStreamPtr& stream, Font* pFont);
        void readGlyph(Ogre::DataStreamPtr& stream, Font* pFont);
        void readColor(Ogre::DataStreamPtr& stream, Font* pFont);
        void writeMain(const Font* pFont);
        void writeGlyph(const Font* pFont, size_t glyph_id);
        void writeColor(const Font* pFont, size_t color_id);
        virtual size_t calcMainSize(const Font* pFont);
        virtual size_t calcGlyphSize(const Font * pFont);
        virtual size_t calcColorSize(const Font * pFont);
    };
} // namespace

#endif // FONTSERIALIZER_H_INCLUDED
