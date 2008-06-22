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
#include <Ogre.h>
#include <SDL/SDL.h>
#include "SonettoKernel.h"
#include "SonettoInputManager.h"

using namespace std;

namespace Sonetto
{
    PlayerInput::PlayerInput(bool enabled,bool rumbleEnabled,SDL_Joystick *joy)
            : mEnabled(enabled), mRumbleEnabled(rumbleEnabled), mJoy(joy)
    {
        memset(mBtnStates,0x00,sizeof(mBtnStates));
        memset(mAxesValues,0x00,sizeof(mAxesValues));
    }

    void PlayerInput::update()
    {
        // <todo> Remember to clean button states before disabling a PlayerInput
        /*if (mEnabled)
        {
            if (mJoy)
            {
                mJoy->capture();
            }

            for (size_t i = 0;i < INPUT_SRC_NUM;++i)
            {
                if (i < 16) { // Buttons
                    KEYSTATE state = KS_NONE;

                    switch (mInputCfg[i].type) {
                        case IST_KEY:
                            state = (Kernel::get()->getInputMan()->getKeyboard()->
                                    isKeyDown((OIS::KeyCode)(mInputCfg[i].value))) ? KS_PRESS : KS_NONE;
                        break;

                        case IST_BUTTON:
                            if (mJoy)
                            {
                                state = (mJoy->getJoyStickState().
                                        mButtons[mInputCfg[i].value]) ? KS_PRESS : KS_NONE;
                            }
                        break;

                        case IST_AXIS:
                            if (mJoy)
                            {
                                float value = (float)(mJoy->getJoyStickState().
                                        mAxes[mInputCfg[i].value].abs);

                                value /= ((value > 0) ? 32767.0f : 32768.0f);
                                state = (fabs(value) > 0.5f) ? KS_PRESS : KS_NONE;
                            }
                        break;
                    }

                    switch (mBtnStates[i])
                    {
                        case KS_NONE:
                            if (state == KS_PRESS)
                            {
                                mBtnStates[i] = KS_PRESS;
                            }
                        break;

                        case KS_PRESS:
                            if (state == KS_PRESS) {
                                mBtnStates[i] = KS_HOLD;
                            } else {
                                mBtnStates[i] = KS_RELEASE;
                            }
                        break;

                        case KS_RELEASE:
                            if (state == KS_PRESS) {
                                mBtnStates[i] = KS_PRESS;
                            } else {
                                mBtnStates[i] = KS_NONE;
                            }
                        break;

                        case KS_HOLD:
                            if (state != KS_PRESS)
                            {
                                mBtnStates[i] = KS_RELEASE;
                            }
                        break;
                    }
                } else {                   // Axes
                    float value;

                    if (mInputCfg[i].isKey) {

                    } else {

                    }
                }
            }
        }*/
    }

    PlayerInput *InputManager::getPlayer(size_t num) const
    {
        // Checks bounds
        if (num >= mPlayers.size())
        {
            return NULL;
        }

        // Returns requested PlayerInput
        return mPlayers[num];
    }

    void PlayerInput::configureAll(const InputSource input[INPUT_SRC_NUM])
    {
        // Just copies new info over the old ones
        memcpy(mInputCfg,input,sizeof(mInputCfg));
    }

    InputManager::InputManager(size_t players,Ogre::RenderWindow *win)
    {
        // Creates desired number of unplugged PlayerInput structures
        // For them to work, you must configure it's buttons and attach
        // a joystick to it (if you're going to use any at all)
        for (size_t i = 0;i < players;++i)
        {
            mPlayers.push_back(new PlayerInput(false,false,NULL));
        }
    }

    void InputManager::update()
    {
        // Updates all connected joysticks' states
        SDL_JoystickUpdate();

        if (SDL_GetKeyState(NULL)[SDLK_DOWN])
        {
            cout << "The console is going doooooooooownnn...\n";
        }

        /*mKeyboard->capture();

        for (size_t i = 0;i < mPlayers.size();++i)
        {
            mPlayers[i]->update();
        }*/
    }
};
