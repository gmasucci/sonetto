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

#include "SonettoPrerequisites.h"
#include "SonettoDatabase.h"
#include "SonettoMusic.h"

namespace Sonetto
{
    //--------------------------------------------------------------------------
    // Sonetto::Database implementation.
    //--------------------------------------------------------------------------
    SONETTO_SINGLETON_IMPLEMENT(Database);
    //--------------------------------------------------------------------------
    Database::~Database()
    {
    }
    //--------------------------------------------------------------------------
    void Database::initialize()
    {
        musics.push_back(Music("music.ogg",0));

        sounds.push_back(SoundDef("rmsystem_accept.ogg"));
        sounds.push_back(SoundDef("sound.ogg"));
        sounds.push_back(SoundDef("rmsystem_cancel.ogg"));

        SoundSet set;
        set.getSounds().push_back(1);
        set.getSounds().push_back(3);
        set.getSounds().push_back(2);
        soundSets.push_back(set);

        GroundType ground;
        ground.defaultFootsteps.push_back(2);
        ground.defaultFootsteps.push_back(3);
        groundTypes.push_back(ground);

        FootwearSounds footwear;
        /*IDVector &sounds = footwear[1];
        sounds.push_back(1);
        sounds.push_back(3);*/
        footwears.push_back(footwear);
    }
} // namespace
