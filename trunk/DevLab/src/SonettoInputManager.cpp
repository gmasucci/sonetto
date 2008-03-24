/*-----------------------------------------------------------------------------
This source file is part of Sonetto RPG Engine.

Copyright (C) 2007,2008 Arthur Carvalho de Souza Lima, Guilherme Prá Vieira


Sonetto RPG Engine is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Sonetto RPG Engine is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even http://a.scarywater.net/afk/the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this library; if not, write to the Free Software Foundation,
Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA or go to
http://www.gnu.org/copyleft/lesser.txt
-----------------------------------------------------------------------------*/

#include <vector>
#include "SonettoKernel.h"
#include "SonettoInputManager.h"

using namespace std;

namespace Sonetto {
    bool InputManager::initialise(Ogre::RenderWindow *win,KeyConfig kc[MAX_PLAYERS]) {
        // Fails if we already initialised
        if (mInitialised) {
            Kernel::getSingleton()->mLogMan->
                                logMessage("( InputManager::initialise() ) InputManager was asked to be "
                                "initialised twice. This will probably cause errors.\n",
                                Ogre::LML_CRITICAL);

            return false;
        }

        // Following bunch of lines sets an OIS Input System for our
        // window handle. Don't be scared. ;-)
        OIS::ParamList pl;
        size_t windowHnd = 0;
        std::ostringstream windowHndStr;
        win->getCustomAttribute("WINDOW",&windowHnd);
        windowHndStr << windowHnd;
        pl.insert(std::make_pair(std::string("WINDOW"),windowHndStr.str()));
        mInputManager = OIS::InputManager::createInputSystem(pl);

        // Creates a keyboard handler and a joystick handler for each joystick plugged in
        mKeyboard = static_cast<OIS::Keyboard *>(mInputManager->
                    createInputObject(OIS::OISKeyboard,false));

        for (int i = 0 ;i < mInputManager->getNumberOfDevices(OIS::OISJoyStick);++i) {
            try {
                // Try to open an OISJoyStick input device
                // This static_cast just casts the general purpose createInputObject()'s return object to
                // our desired JoyStick
                mJoy.push_back(static_cast<OIS::JoyStick *>(mInputManager->
                               createInputObject(OIS::OISJoyStick,false)));
            } catch (...) {
                // Or NULL'ifies slot
                mJoy.push_back(NULL);
            }
        }

        mJoyItrEnd = mJoy.end();

        // Copies the supplied key configuration
        memcpy(mKeyConfig,kc,sizeof(KeyConfig)*MAX_PLAYERS);

        // We're set
        mInitialised = true;
        return true;
    }

    bool InputManager::deinitialise() {
        if (!mInitialised) {
            Kernel::getSingleton()->mLogMan->
                                logMessage("[InputManager::deinitialise()] Trying to deinitialise a non-"
                                "initialised InputManager.\n",Ogre::LML_CRITICAL);

            return false;
        }

        // Deinitialise OIS
        if (mInputManager) {
            // Destroy the Keyboard handler and every Joystick handler
            mInputManager->destroyInputObject(mKeyboard);
            for (mJoyItr = mJoy.begin();mJoyItr != mJoyItrEnd;++mJoyItr)
                mInputManager->destroyInputObject(*mJoyItr);

            // Destroy OIS
            OIS::InputManager::destroyInputSystem(mInputManager);
            mInputManager = NULL;
        }

        // We're uninitialised
        mInitialised = false;
        return true;
    }

    KEYSTATE InputManager::getButtonState(Ogre::uint8 playerID,BUTTON btn) {
        // Makes sure we aren't going to access a non-existent player
        if(playerID >= MAX_PLAYERS) {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                        "PlayerID outbounds MAX_PLAYERS",
                        "InputManager::playerPlugged()");
        }

        return mKeyStates[playerID].buttons[btn];
    }

    Ogre::Vector2 InputManager::getAxis(Ogre::uint8 playerID,AXIS axis) {
        // Makes sure we aren't going to access a non-existent player
        if(playerID >= MAX_PLAYERS) {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                        "PlayerID outbounds MAX_PLAYERS",
                        "InputManager::playerPlugged()");
        }

        return Ogre::Vector2(mKeyStates[playerID].axes[axis+1], // X
                             mKeyStates[playerID].axes[axis]);  // Y
    }


    void InputManager::updateInput() {
        // Capture Input
        for (Ogre::uint8 playerID = 0;playerID < MAX_PLAYERS;++playerID) {
            if (mKeyConfig[playerID].inputDevice > 0 && !playerPlugged(playerID))
                continue;

            if (mKeyboard)
                mKeyboard->capture();

            if (mKeyConfig[playerID].inputDevice > 0)
                mJoy[mKeyConfig[playerID].inputDevice-1]->capture();

            // Update button states
            for (int i = 0;i < 16;++i) {
                bool     newState = false;
                KEYSTATE oldState = mKeyStates[playerID].buttons[i];

                // 0x0F00: Joystick button mask (Only works if inputDevice is greater than 0)
                if (mKeyConfig[playerID].inputDevice > 0 && mKeyConfig[playerID].buttons[i] >= 0x00FF && mKeyConfig[playerID].buttons[i] <= 0x0F00)
                    newState = mJoy[mKeyConfig[playerID].inputDevice-1]->getJoyStickState().mButtons[(mKeyConfig[playerID].buttons[i]) >> 8];

                // 0x00FF: Keyboard key mask
                if (mKeyConfig[playerID].buttons[i] > 0x0000 && mKeyConfig[playerID].buttons[i] < 0x00FF)
                    newState = mKeyboard->isKeyDown((OIS::KeyCode)(mKeyConfig[playerID].buttons[i]));

                if (mKeyConfig[playerID].inputDevice > 0 && mKeyConfig[playerID].buttons[i] >= 0x00FF && mKeyConfig[playerID].buttons[i] <= 0x0F00)
                    newState = mJoy[mKeyConfig[playerID].inputDevice-1]->getJoyStickState().mButtons[(mKeyConfig[playerID].buttons[i]) >> 8];

                // Compare old and new states, and replace to the current one
                switch (oldState) {
                case KS_NONE:
                    if (newState)
                        mKeyStates[playerID].buttons[i] = KS_PRESS;
                    break;

                case KS_PRESS:
                    if (newState) {
                        mKeyStates[playerID].buttons[i] = KS_HOLD;
                    } else {
                        mKeyStates[playerID].buttons[i] = KS_RELEASE;
                    }
                    break;

                case KS_RELEASE:
                    if (newState) {
                        mKeyStates[playerID].buttons[i] = KS_PRESS;
                    } else {
                        mKeyStates[playerID].buttons[i] = KS_NONE;
                    }
                    break;

                case KS_HOLD:
                    if (!newState)
                        mKeyStates[playerID].buttons[i] = KS_RELEASE;
                    break;
                }
            }

            // Update axes
            // Clean axes before checking
            for (int i = 0;i < 4;++i)
                mKeyStates[playerID].axes[i] = 0.0f;

            // Get new axes values
            for (int i = 0;i < 8;++i) {
                // 0xF000: Joystick axes mask (Only works if inputDevice is greater than 0)
                if (mKeyConfig[playerID].inputDevice > 0 && mKeyConfig[playerID].axes[i] > 0x0FFF && mKeyConfig[playerID].axes[i] <= 0xF000) {
                    mKeyStates[playerID].axes[i] = (float) mJoy[mKeyConfig[playerID].inputDevice-1]->
                                                   getJoyStickState().mAxes[((mKeyConfig[playerID].axes[i]) >> 12)-1].abs;

                    // Minimal value is -32768, maximal value is +32767.
                    // Divides respectively to reach a -1.0f to +1.0f value.
                    mKeyStates[playerID].axes[i] /= ((mKeyStates[playerID].axes[i] > 0) ? 32767.0f : 32768.0f);

                    // Small ugly hack: If we already got input from all axes, we
                    // should skip the emulation part
                    if (i >= 3) {
                        // Trigger a loop end, conditioning it to false
                        i = 8;

                        // And making it recheck
                        continue;
                    }
                } else {
                    float value = 0.0f;

                    // 0x0F00: Joystick buttons mask
                    if (mKeyConfig[playerID].axes[i] >= 0x00FF && mKeyConfig[playerID].axes[i] <= 0x0F00)
                        value = (float) mJoy[mKeyConfig[playerID].inputDevice-1]->getJoyStickState().mButtons[(mKeyConfig[playerID].axes[i]) >> 8];

                    // 0x00FF: Keyboard keys mask
                    if (mKeyConfig[playerID].axes[i] > 0x0000 && mKeyConfig[playerID].axes[i] < 0x00FF)
                        value = (float) mKeyboard->isKeyDown((OIS::KeyCode)(mKeyConfig[playerID].axes[i]));

                    // Emulate axes
                    switch (i) {
                        // First two buttons: Write it's value anyway
                    case 0x00:
                        mKeyStates[playerID].axes[0] = -value;
                        break;
                    case 0x01:
                        mKeyStates[playerID].axes[1] = value;
                        break;

                    case 0x02:
                        // Opposite button: Don't override old value
                        if (mKeyStates[playerID].axes[0] == 0.0f)
                            mKeyStates[playerID].axes[0] = -value;
                        break;

                    case 0x03:
                        // Opposite button: Don't override old value
                        if (mKeyStates[playerID].axes[1] == 0.0f)
                            mKeyStates[playerID].axes[1] = -value;
                        break;

                        // First two buttons: Write it's value anyway
                    case 0x04:
                        mKeyStates[playerID].axes[2] = -value;
                        break;
                    case 0x05:
                        mKeyStates[playerID].axes[3] = value;
                        break;

                    case 0x06:
                        // Opposite button: Don't override old value
                        if (mKeyStates[playerID].axes[2] == 0.0f)
                            mKeyStates[playerID].axes[2] = value;
                        break;

                    case 0x07:
                        // Opposite button: Don't override old value
                        if (mKeyStates[playerID].axes[3] == 0.0f)
                            mKeyStates[playerID].axes[3] = -value;
                        break;

                    default:
                        break;
                    } // S
                } // l
            } // o
        } // p
    } // e !!      :-)

    void InputManager::setKeyConfig(Ogre::uint8 playerID,KeyConfig *kc) {
        // Makes sure we aren't going to access a non-existent player
        if(playerID >= MAX_PLAYERS) {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                        "PlayerID outbounds MAX_PLAYERS",
                        "InputManager::playerPlugged()");
        }

        assert(kc && "Supplied KeyConfig is NULL");

        // Rawly copies new configuration
        memcpy(&mKeyConfig[playerID],kc,sizeof(mKeyConfig[playerID]));
    }

    KeyConfig *InputManager::getKeyConfig(Ogre::uint8 playerID) {
        // Makes sure we aren't going to access a non-existent player
        if(playerID >= MAX_PLAYERS) {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                        "PlayerID outbounds MAX_PLAYERS",
                        "InputManager::playerPlugged()");
        }

        return &mKeyConfig[playerID];
    }

    bool InputManager::playerPlugged(Ogre::uint8 playerID) {
        // Makes sure we aren't going to access a non-existent player
        if(playerID >= MAX_PLAYERS) {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                        "PlayerID outbounds MAX_PLAYERS",
                        "InputManager::playerPlugged()");
        }

        // Device 0 is a Keyboard
        if (mKeyConfig[playerID].inputDevice == 0)
            return (mKeyboard != NULL);

        // If out of controller vector bounds, the controller is not plugged
        if (mKeyConfig[playerID].inputDevice-1 >= (int)(mJoy.size()))
            return false;

        return (mJoy[mKeyConfig[playerID].inputDevice-1] != NULL);
    }
};
