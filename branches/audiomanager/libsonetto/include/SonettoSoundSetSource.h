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

#ifndef SONETTO_SOUNDSETSOURCE_H
#define SONETTO_SOUNDSETSOURCE_H

#include <OgreSharedPtr.h>
#include "SonettoPrerequisites.h"
#include "SonettoSoundSource.h"

namespace Sonetto
{
    class SONETTO_API SoundSetSource : public SoundSource
    {
    public:
        SoundSetSource() : SoundSource(),mSetID(0) {}
        virtual ~SoundSetSource() {}

        virtual inline void setSoundSetID(uint32 id) { mSetID = id; }
        virtual inline uint32 getSoundSetID() const { return mSetID; }

        virtual void setSoundID(uint32 id);

        virtual uint32 getSoundNum() const;

    protected:
        uint32 mSetID;
    };

    typedef Ogre::SharedPtr<SoundSetSource> SoundSetSourcePtr;
} // namespace

#endif
