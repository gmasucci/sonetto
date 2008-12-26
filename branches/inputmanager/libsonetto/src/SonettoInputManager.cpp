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

#ifdef WINDOWS
#   include <windows.h>
#   include <dinput.h>
#endif
#include <algorithm>
#include <vector>
#include <OgreVector2.h>
#include "SonettoKernel.h"
#include "SonettoInputManager.h"
#include "SonettoPlayerInput.h"
#include "SonettoJoystick.h"
#include "SonettoJoystickHardwareData.h"

namespace Sonetto
{
    // ----------------------------------------------------------------------
    // Sonetto::InputManager implementation
    // ----------------------------------------------------------------------
    #ifdef WINDOWS
    struct GuidCount
    {
        GuidCount(GUID aGuid,size_t aCount) : guid(aGuid),count(aCount) {}

        GUID guid;
        size_t count;
    };

    typedef std::vector<GuidCount> GuidCountVector;
    #endif
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
    #ifdef WINDOWS
    void InputManager::initialize(HWND hWnd)
    #else
    void InputManager::initialize()
    #endif
    {
        #ifdef WINDOWS
        {
            // Creates DirectInput object
            if ( FAILED(DirectInput8Create(GetModuleHandle(NULL),
                    DIRECTINPUT_VERSION,IID_IDirectInput8,
                    (void **)(&mDirectInput),NULL)) )
            {
                SONETTO_THROW("Could not initialize DirectInput");
            }

            // Creates DirectInput keyboard device
            if ( FAILED(mDirectInput->CreateDevice(GUID_SysKeyboard,
                    &mKeyboard,NULL)) )
            {
                SONETTO_THROW("Could not create DirectInput keyboard "
                              "device");
            }

            // Sets keyboard device data format
            if ( FAILED(mKeyboard->SetDataFormat(&c_dfDIKeyboard)) )
            {
                SONETTO_THROW("Could not set DirectInput keyboard device "
                              "data format");
            }

            // Sets keyboard cooperative level
            if ( FAILED(mKeyboard->SetCooperativeLevel(hWnd,
                    DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)) )
            {
                SONETTO_THROW("Could not set DirectInput keyboard "
                              "cooperative level");
            }

            // Acquires keyboard
            if ( FAILED(mKeyboard->Acquire()) )
            {
                SONETTO_THROW("Could not acquire DirectInput keyboard");
            }

            // Enumerates joysticks for use
            updateJoysticks();
        }
        #endif

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
            bool rawState = getRawKeyState(i);

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

        // Updates joystick states
        updateJoysticks();
        /*for (size_t i = 0;i < mJoysticks.size();++i)
        {
            if (mJoysticks[i].unique()) {
                // Deletes joystick not in use anymore
                mJoysticks.erase(mJoysticks.begin() + i);

                // Rewinds one because we have just
                // deleted an mJoysticks entry
                --i;
            } else {
                // Updates if joystick is still in use
                mJoysticks[i]->update();
            }
        }*/

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
        // ID 0 is never attached
        if (id == 0)
        {
            return false;
        }

        // Searches vector of player inputs for a joystick
        // that has the provided ID
        for (size_t i = 0;i < mPlayers.size();++i)
        {
            if (mPlayers[i]->getJoystick() == id)
            {
                // Returns true if one is found...
                return true;
            }
        }

        // ...or false otherwise
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
    bool InputManager::getRawKeyState(uint32 key)
    {
        #ifdef WINDOWS
        uint8 keys[256];
        HRESULT hr;

        // Gets device state and checks for errors
        hr = mKeyboard->GetDeviceState(sizeof(keys),(void *)keys);
        if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED) {
            // If the keyboard was lost (probably due to the window losing
            // focus), tries do reacquire it and returns false
            mKeyboard->Acquire();
            return false;
        } else
        if ( FAILED(hr) ) {
            // If an error has occurred, throws exception
            SONETTO_THROW("Could not get DirectInput keyboard device "
                          "state");
        }

        // Returns whether the high bit is 1, meaning the key is down
        // (who understands Windows programming?)
        return (keys[key] & 0x80);
        #endif
    }
    // ----------------------------------------------------------------------
    void InputManager::updateJoysticks()
    {
        #ifdef WINDOWS
        // This GUID counting vector is used to reference
        // devices under the same product GUID
        GuidCountVector guidCounts;
        mDirectInput->EnumDevices(DI8DEVCLASS_GAMECTRL,joystickEnumCallback,
                &guidCounts,DIEDFL_ATTACHEDONLY);
        #endif
    }
    // ----------------------------------------------------------------------
    #ifdef WINDOWS
    BOOL CALLBACK InputManager::joystickEnumCallback(
                LPCDIDEVICEINSTANCE dev,void *aGuidCounts)
    {
        InputManager &inputMan = InputManager::getSingleton();

        size_t guidIndex = 0;
        GuidCountVector *guidCounts = (GuidCountVector *)aGuidCounts;
        GuidCountVector::iterator iter;
        GUID guid = dev->guidProduct;

        for (size_t i = 0;i < guidCounts->size();++i)
        {
            if ((*guidCounts)[i].guid == guid)
            {
                guidIndex = (*guidCounts)[i].count++;
            }
        }

        if (guidIndex == 0)
        {
            guidCounts->push_back(GuidCount(guid,1));
        }

        // Searches for joysticks already assigned to this device
        // Updates their structures if this device was assigned
        // to one of them; this way, joystick IDs are always kept
        for (size_t i = 0;i < inputMan.mJoysticks.size();++i)
        {
            const JoystickHardwareData &hwdata = inputMan.mJoysticks[i]->
                    getHardwareData();

            if (dev == hwdata.device)
            {
                // Device already detected; skip
                return true;
            }

            GUID iGuid = hwdata.device->guidProduct;

            if (guid.Data1 == iGuid.Data1 &&
                guid.Data2 == iGuid.Data2 &&
                guid.Data3 == iGuid.Data3 &&
                memcmp(guid.Data4,iGuid.Data4,sizeof(guid.Data4)) &&
                hwdata.guidIndex == guidIndex)
            {
                // Updates joysticks' old hwdata to new one
                inputMan.mJoysticks[i]->setHardwareData(
                        JoystickHardwareData(dev,guidIndex));

                std::cout << "Reassigned\n";

                // Device detected and reassigned; return
                return true;
            }
        }

        // If no joystick was already assigned to this device, creates one
        inputMan.mJoysticks.push_back(JoystickPtr(new Joystick(
                JoystickHardwareData(dev,guidIndex))));

        std::cout << "Created\n";

        return true;
    }
    #endif
};
