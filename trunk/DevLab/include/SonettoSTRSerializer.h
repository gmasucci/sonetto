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
#ifndef SONETTOSTRSERIALIZER_H_INCLUDED
#define SONETTOSTRSERIALIZER_H_INCLUDED

#include <OgreSerializer.h>
// Side note: All this class has been coded under the sound track of Goof Troop - Capcom - SNES (1993)
namespace Sonetto
{
    class STRData; // Forward declaration

    class STRSerializer : public Ogre::Serializer
    {
    public:
        STRSerializer();
        ~STRSerializer();

        void exportSTR(const STRData *pSTR, const Ogre::String &fileName, Ogre::Serializer::Endian endianMode = Ogre::Serializer::ENDIAN_NATIVE);
        void importSTR(Ogre::DataStreamPtr &stream, STRData *pDest);
    };

} // namespace
#endif // SONETTOSTRSERIALIZER_H_INCLUDED
