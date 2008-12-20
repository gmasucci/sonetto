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

#include <vector>
#include <OgreVector2.h>
#include <SDL/SDL.h>
#include "SonettoKernel.h"
#include "SonettoInputManager.h"

namespace Sonetto
{
    // ----------------------------------------------------------------------
    // Sonetto::InputManager implementation
    // ----------------------------------------------------------------------
    SONETTO_SINGLETON_IMPLEMENT(InputManager);
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
        // Creates desired number of unplugged PlayerInput structures
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
        for (size_t i = SDLK_FIRST;i <= SDLK_LAST;++i)
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

        // Updates all connected joysticks' states
        SDL_JoystickUpdate();

        // And updates PlayerInputs' states
        for (size_t i = 0;i < mPlayerNum;++i)
        {
            mPlayers[i]->update();
        }
    }
    // ----------------------------------------------------------------------
    Joystick *InputManager::_getJoystick(uint32 index)
    {
        Joystick *joy;

        if (mJoyRefCounts.find(index) == mJoyRefCounts.end()) {
            // Tries to open joystick
            joy = (Sonetto::Joystick *)( SDL_JoystickOpen(index - 1) );

            // If successful, creates a reference counting structure for it
            if (joy)
            {
                // Adds reference count structure to reference count vector
                JoyRefCount refCount(joy);
                ++refCount.refCount;
                mJoyRefCounts[index] = refCount;
            }
        } else {
            // Gets already open joystick and increments reference count
            joy = mJoyRefCounts[index].joyPtr;
            ++mJoyRefCounts[index].refCount;
        }

         return joy;
    }
    // ----------------------------------------------------------------------
    void InputManager::_releaseJoystick(uint32 index)
    {
        if (mJoyRefCounts.find(index) == mJoyRefCounts.end())
        {
            SONETTO_THROW("Releasing joystick that isn't open");
        }

        // Decrements reference count for the joystick being released
        JoyRefCount &refCount = mJoyRefCounts[index];
        --refCount.refCount;

        // If there are no more references to it, frees and removes it
        // from vector
        if (refCount.refCount == 0)
        {
            SDL_JoystickClose((SDL_Joystick *)refCount.joyPtr);
            mJoyRefCounts.erase(index);
        }
    }
    // ----------------------------------------------------------------------
    uint16 InputManager::getJoystickNum() const
    {
        return (uint16)SDL_NumJoysticks();
    }
    // ----------------------------------------------------------------------
    bool InputManager::joystickAttached(uint16 index) const
    {
        // Checks map of currently in use joysticks
        if (mJoyRefCounts.find(index) != mJoyRefCounts.end())
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
    // ----------------------------------------------------------------------
    Ogre::Vector2 PlayerInput::getAxisValue(Axis axs)
    {
        switch (axs)
        {
            case AX_LEFT:  return mAxesValues[0]; break;
            case AX_RIGHT: return mAxesValues[1]; break;

            default: return Ogre::Vector2(0.0f,0.0f); break;
        }
    }
};
