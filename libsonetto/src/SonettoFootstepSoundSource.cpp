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

#include <cstdlib>
#include "SonettoKernel.h"
#include "SonettoDatabase.h"
#include "SonettoMath.h"
#include "SonettoFootstepSoundSource.h"

namespace Sonetto
{
    //-----------------------------------------------------------------------------
    // Sonetto::FootstepSoundSource implementation.
    //-----------------------------------------------------------------------------
    void FootstepSoundSource::setGround(uint32 ground)
    {
        mGround = ground;
        SoundSource::setSoundID(0);

        if (ground == 0)
        {
            return;
        }

        if (ground > Database::getSingleton().groundTypes.size())
        {
            SONETTO_THROW("Unknown ground type");
        }
    }
    //-----------------------------------------------------------------------------
    void FootstepSoundSource::setFootwear(uint32 footwear)
    {
        mFootwear = footwear;
        SoundSource::setSoundID(0);

        if (footwear == 0)
        {
            return;
        }

        if (footwear > Database::getSingleton().footwears.size())
        {
            SONETTO_THROW("Unknown footwear");
        }
    }
    //-----------------------------------------------------------------------------
    void FootstepSoundSource::play()
    {
        if (mGround != 0 && mFootwear != 0)
        {
            Database &database = Database::getSingleton();
            FootwearSounds &footwear = database.footwears[mFootwear - 1];
            FootwearSounds::iterator iter = footwear.find(mGround);
            IDVector *footsteps;

            if (iter != footwear.end()) {
                footsteps = &iter->second;
            } else {
                footsteps = &database.groundTypes[mGround - 1].defaultFootsteps;
            }

            if (footsteps->size() > 0)
            {
                SoundSource::setSoundID(
                        (*footsteps)[Math::irand(0,footsteps->size() - 1)]);
                SoundSource::play();
            }
        }
    }
} // namespace
