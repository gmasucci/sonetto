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

#ifndef SONETTO_JOYSTICK_H
#define SONETTO_JOYSTICK_H

#include <OgreSharedPtr.h>
#include <OgreVector2.h>
#include <SDL/SDL_joystick.h>
#include "SonettoPrerequisites.h"
#include "SonettoInputSource.h"

namespace Sonetto
{
    /// Reference counted Joystick shared pointer type
    typedef Ogre::SharedPtr<Joystick> JoystickPtr;

    class SONETTO_API Joystick
    {
    public:
        enum RawButton
        {
            RWB_NONE,
            RWB_1,
            RWB_2,
            RWB_3,
            RWB_4,
            RWB_5,
            RWB_6,
            RWB_7,
            RWB_8,
            RWB_9,
            RWB_10,
            RWB_11,
            RWB_12,
            RWB_13,
            RWB_14,
            RWB_15,
            RWB_16,
            RWB_FIRST_HAT,
            RWB_HAT_UP = RWB_FIRST_HAT,
            RWB_HAT_RIGHT,
            RWB_HAT_DOWN,
            RWB_HAT_LEFT
        };

        enum RawAnalog
        {
            RWA_NONE,
            RWA_1,
            RWA_2
        };

        Joystick(uint32 id) : mID(id),mJoy(NULL) {}

        virtual ~Joystick();

        // Winmm hack
        bool isPlugged();

        inline uint32 getID() const { return mID; }

        inline bool isEnabled() const { return (mJoy != NULL); }

        void setEnabled(bool enable);

        bool getRawButtonState(RawButton button);

        Ogre::Vector2 getRawAnalogState(RawAnalog analog);

    protected:
        uint32 mID;

        SDL_Joystick *mJoy;
    };
} // namespace

#endif

