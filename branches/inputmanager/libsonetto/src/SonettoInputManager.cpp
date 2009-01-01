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

#include <algorithm>
#include <vector>
#include <OgreVector2.h>
#include "SonettoKernel.h"
#include "SonettoInputManager.h"
#include "SonettoPlayerInput.h"
#include "SonettoJoystick.h"

namespace Sonetto
{
    // ----------------------------------------------------------------------
    // Sonetto::InputManager implementation
    // ----------------------------------------------------------------------
    SONETTO_SINGLETON_IMPLEMENT(InputManager);
    // ----------------------------------------------------------------------
    InputManager::InputManager(size_t players)
            : mPlayerNum(players), mInitialized(false)
    {
        memset(mKeyboardStates,0x00,sizeof(mKeyboardStates));
    }
    // ----------------------------------------------------------------------
    InputManager::~InputManager()
    {
        if (mInitialized)
        {
            // Deletes player inputs
            // P.s.: These destructors should take care of releasing its
            // joysticks and thus freeing them
            for (size_t i = 0;i < mPlayerNum;++i)
            {
                delete mPlayers[i];
            }
        }
    }
    // ----------------------------------------------------------------------
    void InputManager::initialize()
    {
        for (size_t i = 1;i <= SDL_NumJoysticks();++i)
        {
            mJoysticks.push_back(JoystickPtr(new Joystick(i)));
        }

        // Creates desired number of empty PlayerInput structures
        for (size_t i = 0;i < mPlayerNum;++i)
        {
            mPlayers.push_back(new PlayerInput());
        }

        // Flags we've initialized successfully
        mInitialized = true;
    }
    // ----------------------------------------------------------------------
    void InputManager::update()
    {
        // Updates keyboard states
        for (size_t i = 0;i < 256;++i)
        {
            bool rawState = SDL_GetKeyState(NULL)[i];

            switch (mKeyboardStates[i])
            {
                case KS_NONE:
                    if (rawState)
                    {
                        mKeyboardStates[i] = KS_PRESS;
                    }
                break;

                case KS_PRESS:
                    if (rawState) {
                        mKeyboardStates[i] = KS_HOLD;
                    } else {
                        mKeyboardStates[i] = KS_RELEASE;
                    }
                break;

                case KS_RELEASE:
                    if (rawState) {
                        mKeyboardStates[i] = KS_PRESS;
                    } else {
                        mKeyboardStates[i] = KS_NONE;
                    }
                break;

                case KS_HOLD:
                    if (!rawState)
                    {
                        mKeyboardStates[i] = KS_RELEASE;
                    }
                break;
            }
        }

        SDL_JoystickUpdate();
        for (size_t i = 0;i < mJoysticks.size();++i)
        {
            if (mJoysticks[i].unique()) {
                mJoysticks[i]->setEnabled(false);
            } else {
                mJoysticks[i]->setEnabled(true);
            }
        }

        // And updates PlayerInputs' states
        for (size_t i = 0;i < mPlayerNum;++i)
        {
            mPlayers[i]->update();
        }
    }
    // ----------------------------------------------------------------------
    JoystickPtr InputManager::_getJoystick(uint16 id)
    {
        // Checks if a joystick with this ID exists and returns it
        if (id <= mJoysticks.size())
        {
            return mJoysticks[id - 1];
        }

        // Or NULL otherwise
        return JoystickPtr();
    }
    // ----------------------------------------------------------------------
    uint16 InputManager::getJoystickNum() const
    {
        return mJoysticks.size();
    }
    // ----------------------------------------------------------------------
    bool InputManager::joystickAttached(uint16 id) const
    {
        if (id == 0)
        {
            return false;
        }

        if (!mJoysticks[id - 1].unique())
        {
            return true;
        }

        return false;
    }
    // ----------------------------------------------------------------------
    PlayerInput *InputManager::getPlayer(size_t num)
    {
        // Checks bounds
        if (num >= mPlayerNum)
        {
            return NULL;
        }

        // Returns requested PlayerInput
        return mPlayers[num];
    }
};
