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

#include <SDL/SDL.h>
#include "SonettoPrerequisites.h"
#include "SonettoInputManager.h"
#include "SonettoAudioManager.h"
#include "SonettoSoundSource.h"
#include "GenericBootModule.h"

namespace BootModule
{
    // ----------------------------------------------------------------------
    // BootModule::GenericBootModule implementation
    // ----------------------------------------------------------------------
    GenericBootModule::GenericBootModule() {}
    // ----------------------------------------------------------------------
    GenericBootModule::~GenericBootModule() {}
    // ----------------------------------------------------------------------
    void GenericBootModule::initialize()
    {
        Sonetto::AudioManager::getSingleton().loadSoundSet(1);
        mSource = Sonetto::AudioManager::getSingleton().
                createSound<Sonetto::FootstepSoundSource>();
        mSource->setGround(1);
        mSource->setFootwear(1);

        Sonetto::InputManager::getSingleton().getPlayer(0)->configBtn(
                Sonetto::BTN_CROSS,Sonetto::InputSource(true,
                Sonetto::InputSource::IST_KEY,SDLK_x));
        Sonetto::InputManager::getSingleton().getPlayer(0)->configBtn(
                Sonetto::BTN_TRIANGLE,Sonetto::InputSource(true,
                Sonetto::InputSource::IST_KEY,SDLK_z));
        Sonetto::InputManager::getSingleton().getPlayer(0)->setEnabled(true);
    }
    // ----------------------------------------------------------------------
    void GenericBootModule::update()
    {
        Sonetto::PlayerInput *p0 = Sonetto::InputManager::getSingleton().
                getPlayer(0);

        if (p0->getBtnState(Sonetto::BTN_CROSS) == Sonetto::KS_PRESS)
        {
            mSource->play();
        }
    }
    // ----------------------------------------------------------------------
    void GenericBootModule::deinitialize() {}
    // ----------------------------------------------------------------------
    void GenericBootModule::halt() {}
    // ----------------------------------------------------------------------
    void GenericBootModule::resume() {}
} // namespace
