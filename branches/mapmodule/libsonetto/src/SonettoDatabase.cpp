/*-----------------------------------------------------------------------------
Copyright (c) 2009, Sonetto Project Developers
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1.  Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
2.  Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
3.  Neither the name of the Sonetto Project nor the names of its contributors
    may be used to endorse or promote products derived from this software
    without specific prior written permission.


THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
-----------------------------------------------------------------------------*/

#include "SonettoPrerequisites.h"
#include "SonettoDatabase.h"
#include "SonettoScriptDataHandler.h"
#include "SonettoMusic.h"

namespace Sonetto
{
    //--------------------------------------------------------------------------
    // Sonetto::Database implementation.
    //--------------------------------------------------------------------------
    SONETTO_SINGLETON_IMPLEMENT(SONETTO_API,Database);
    //--------------------------------------------------------------------------
    Database::~Database()
    {
        if (mInitialized)
        {
            mScriptDataHandler.unregisterOpcodes();
        }
    }
    //--------------------------------------------------------------------------
    void Database::initialize()
    {
        if (mInitialized)
        {
            SONETTO_THROW("Database was already initialized");
        }

        mScriptDataHandler.registerOpcodes();
        savemap.load("savegame001.lrs");

        musics.push_back(Music("music.ogg",0));

        sounds.push_back(SoundDef("rmsystem_accept.ogg"));
        sounds.push_back(SoundDef("rmsystem_move.ogg"));
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
        IDVector &sounds = footwear[1];
        sounds.push_back(1);
        sounds.push_back(3);
        footwears.push_back(footwear);

        system.startMap = 1;

        mInitialized = true;
    }
} // namespace
