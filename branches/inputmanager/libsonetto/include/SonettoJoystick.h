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

#ifndef WINDOWS
#   error Sonetto::Joystick not yet implemented in Linux.
#endif

#include <SDL/SDL_joystick.h>
#include "SonettoPrerequisites.h"

namespace Sonetto
{
    #ifdef WINDOWS
    /// Windows-pertinent Joystick hardware information
    struct JoystickHardwareData
    {
        unsigned int id;

        struct
        {
            int   offset;
            float scale;
        } transaxis[6];
    };
    #endif

    /// The real SDL_Joystick structure
    class Joystick {
    public:
        Joystick() {}
        ~Joystick() {}

        /// Checks whether this joystick is plugged or not
        bool _isPlugged() const;

    private:
        uint8 id;
        const char *name;

        int    axesNum;
        int16 *axes;

        int    hatsNum;
        uint8 *hats;

        int ballsNum;
        struct
        {
            int dx;
            int dy;
        } *balls;

        int    btnsNum;
        uint8 *btns;

        JoystickHardwareData *hwData;

        int refCount;
    };
} // namespace

#endif

