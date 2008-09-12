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
using namespace Ogre;

namespace Sonetto
{
    PlayerInput::PlayerInput(bool enabled,bool rumbleEnabled,SDL_Joystick *joy)
            : mEnabled(enabled), mRumbleEnabled(rumbleEnabled), mJoy(joy)
    {
        memset(mBtnStates,0x00,sizeof(mBtnStates));
        memset(mAxesValues,0x00,sizeof(mAxesValues));
    }

    PlayerInput::~PlayerInput()
    {
        if (mJoy)
        {
            SDL_JoystickClose(mJoy);
        }
    }

    void PlayerInput::update()
    {
        // <todo> Remember to clean button states before disabling a PlayerInput
        if (mEnabled && Kernel::get()->isWndFocused())
        {
            Uint8 *keystates = SDL_GetKeyState(NULL);

            mAxesValues[0].x = mAxesValues[0].y = 0.0f;
            mAxesValues[1].x = mAxesValues[1].y = 0.0f;

            for (size_t i = 0;i < INPUT_SRC_NUM;++i)
            {
                if (i < 16) { // Buttons
                    KEYSTATE state = KS_NONE;

                    switch (mInputCfg[i].type) {
                        case InputSource::IST_KEY:
                            state = (keystates[mInputCfg[i].value]) ? KS_PRESS : KS_NONE;
                        break;

                        case InputSource::IST_BUTTON:
                            if (mJoy)
                            {
                                state = (SDL_JoystickGetButton(mJoy,
                                         mInputCfg[i].value)) ? KS_PRESS : KS_NONE;
                            }
                        break;

                        case InputSource::IST_AXIS:
                            if (mJoy)
                            {
                                float  value;

                                switch (mInputCfg[i].value)
                                {
                                    case AXE_LEFT_LEFT:
                                    case AXE_LEFT_RIGHT:
                                        value = SDL_JoystickGetAxis(mJoy,0);
                                        value /= ((value > 0) ? 32767.0f : 32768.0f);

                                        if (mInputCfg[i].value == AXE_LEFT_LEFT) {
                                            state = (value <= -0.5f) ? KS_PRESS : KS_NONE;
                                        } else {
                                            state = (value >= +0.5f) ? KS_PRESS : KS_NONE;
                                        }
                                    break;

                                    case AXE_LEFT_UP:
                                    case AXE_LEFT_DOWN:
                                        value = SDL_JoystickGetAxis(mJoy,1);
                                        value /= ((value > 0) ? 32767.0f : 32768.0f);

                                        if (mInputCfg[i].value == AXE_LEFT_UP) {
                                            state = (value <= -0.5f) ? KS_PRESS : KS_NONE;
                                        } else {
                                            state = (value >= +0.5f) ? KS_PRESS : KS_NONE;
                                        }
                                    break;

                                    case AXE_RIGHT_UP:
                                    case AXE_RIGHT_DOWN:
                                        value = SDL_JoystickGetAxis(mJoy,2);
                                        value /= ((value > 0) ? 32767.0f : 32768.0f);

                                        if (mInputCfg[i].value == AXE_RIGHT_UP) {
                                            state = (value <= -0.5f) ? KS_PRESS : KS_NONE;
                                        } else {
                                            state = (value >= +0.5f) ? KS_PRESS : KS_NONE;
                                        }
                                    break;

                                    case AXE_RIGHT_LEFT:
                                    case AXE_RIGHT_RIGHT:
                                        value = SDL_JoystickGetAxis(mJoy,3);
                                        value /= ((value > 0) ? 32767.0f : 32768.0f);

                                        if (mInputCfg[i].value == AXE_LEFT_LEFT) {
                                            state = (value <= -0.5f) ? KS_PRESS : KS_NONE;
                                        } else {
                                            state = (value >= +0.5f) ? KS_PRESS : KS_NONE;
                                        }
                                    break;

                                    default: state = KS_NONE; break;
                                }
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
                    float value = 0.0f;
                    int   axis = (i-16 >= 0 && i-16 <= 3) ? 0 : 1;
                    char  dir  = (i-16)%4;

                    if (dir == 0 || dir == 2) { // Vertical axis (dir == UP || dir == DOWN)
                        value = mAxesValues[axis].y;
                    } else {                    // Horizontal axis (dir != UP && dir != DOWN)
                        value = mAxesValues[axis].x;
                    }

                    switch (mInputCfg[i].type)
                    {
                        case InputSource::IST_KEY:
                            switch (dir)
                            {
                                case 0: // Up
                                    if (keystates[mInputCfg[i].value])
                                    {
                                        value = (mAxesValues[axis].y != 0.0f) ? 0.0f : -1.0f;
                                    }
                                break;

                                case 1: // Right
                                    if (keystates[mInputCfg[i].value])
                                    {
                                        value = (mAxesValues[axis].x != 0.0f) ? 0.0f : +1.0f;
                                    }
                                break;

                                case 2: // Down
                                    if (keystates[mInputCfg[i].value])
                                    {
                                        value = (mAxesValues[axis].y != 0.0f) ? 0.0f : +1.0f;
                                    }
                                break;

                                case 3: // Left
                                    if (keystates[mInputCfg[i].value])
                                    {
                                        value = (mAxesValues[axis].x != 0.0f) ? 0.0f : -1.0f;
                                    }
                                break;
                            }
                        break;

                        case InputSource::IST_BUTTON:
                            if (mJoy)
                            {
                                switch (dir)
                                {
                                    case 0: // Up
                                        if (SDL_JoystickGetButton(mJoy,mInputCfg[i].value))
                                        {
                                            value = (mAxesValues[axis].y != 0.0f) ? 0.0f : -1.0f;
                                        }
                                    break;

                                    case 1: // Right
                                        if (SDL_JoystickGetButton(mJoy,mInputCfg[i].value))
                                        {
                                            value = (mAxesValues[axis].x != 0.0f) ? 0.0f : +1.0f;
                                        }
                                    break;

                                    case 2: // Down
                                        if (SDL_JoystickGetButton(mJoy,mInputCfg[i].value))
                                        {
                                            value = (mAxesValues[axis].y != 0.0f) ? 0.0f : +1.0f;
                                        }
                                    break;

                                    case 3: // Left
                                        if (SDL_JoystickGetButton(mJoy,mInputCfg[i].value))
                                        {
                                            value = (mAxesValues[axis].x != 0.0f) ? 0.0f : -1.0f;
                                        }
                                    break;
                                }
                            }
                        break;

                        case InputSource::IST_AXIS:
                            if (dir == 0 || dir == 4) {
                                if (mJoy)
                                {
                                    int cfgAxis;

                                    if (mInputCfg[i].value == AX_LEFT) {
                                        cfgAxis = 0;
                                    } else
                                    if (mInputCfg[i].value == AX_RIGHT) {
                                        cfgAxis = 2;
                                    } else {
                                        break;
                                    }

                                    mAxesValues[axis].x = SDL_JoystickGetAxis(mJoy,cfgAxis);   // Horizontal
                                    mAxesValues[axis].y = SDL_JoystickGetAxis(mJoy,cfgAxis+1); // Vertical
                                    mAxesValues[axis].x /= ((mAxesValues[axis].x > 0) ? 32767.0f : 32768.0f);
                                    mAxesValues[axis].y /= ((mAxesValues[axis].y > 0) ? 32767.0f : 32768.0f);

                                    // Inverts axes if needed
                                    if (mInputCfg[i].invert & InputSource::INV_ORDER)
                                    {
                                        float tmp = mAxesValues[axis].x;
                                        mAxesValues[axis].x = mAxesValues[axis].y;
                                        mAxesValues[axis].y = tmp;
                                    }

                                    // Inverts polarity if needed
                                    if (mInputCfg[i].invert & InputSource::INV_X_POLARITY)
                                    {
                                        mAxesValues[axis].x = -mAxesValues[axis].x;
                                    }

                                    if (mInputCfg[i].invert & InputSource::INV_Y_POLARITY)
                                    {
                                        mAxesValues[axis].y = -mAxesValues[axis].y;
                                    }



                                    i += 3;
                                    continue;
                                }

                                i += 3;
                            }
                        break;
                    }

                    if (dir == 0 || dir == 2) { // Vertical axis (dir == UP || dir == DOWN)
                        mAxesValues[axis].y = value;
                    } else {                    // Horizontal axis (dir != UP && dir != DOWN)
                        mAxesValues[axis].x = value;
                    }
                }
            }
        }
    }

    void PlayerInput::configAxis(AXIS axs,const InputSource &input)
    {
        switch (axs)
        {
            case AX_LEFT:
                mInputCfg[16] = input;
            break;

            case AXE_LEFT_UP:
            case AXE_LEFT_RIGHT:
            case AXE_LEFT_DOWN:
            case AXE_LEFT_LEFT:
                mInputCfg[16+(axs-1)] = input;
            break;

            case AX_RIGHT:
                mInputCfg[20] = input;
            break;

            case AXE_RIGHT_UP:
            case AXE_RIGHT_RIGHT:
            case AXE_RIGHT_DOWN:
            case AXE_RIGHT_LEFT:
                mInputCfg[20+(axs-6)] = input;
            break;
        }
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

    void PlayerInput::config(const InputSource input[INPUT_SRC_NUM])
    {
        // Just copies new info over the old ones
        memcpy(mInputCfg,input,sizeof(mInputCfg));
    }

    Vector2 PlayerInput::getAxisValue(AXIS axs)
    {
        switch (axs)
        {
            case AX_LEFT:  return mAxesValues[0]; break;
            case AX_RIGHT: return mAxesValues[1]; break;

            default: return Vector2(0.0f,0.0f); break;
        }
    }

    void InputManager::update()
    {
        // Updates all connected joysticks' states
        SDL_JoystickUpdate();

        // And updates PlayerInput states
        for (size_t i = 0;i < mPlayers.size();++i)
        {
            mPlayers[i]->update();
        }
    }

    SDL_Joystick *InputManager::getJoystick(size_t index) const
    {
         return SDL_JoystickOpen(index);
    }

    size_t InputManager::getJoystickNum() const
    {
        return (size_t)SDL_NumJoysticks();
    }

    bool InputManager::joystickAttached(size_t index)
    {
        for (size_t i = 0;i < mPlayers.size();++i)
        {
            if (mPlayers[i]->getJoystick() &&
                    SDL_JoystickIndex(mPlayers[i]->getJoystick()) == (int)index)
            {
                return true;
            }
        }

        return false;
    }

    InputManager::InputManager(size_t players)
    {
        // Creates desired number of unplugged PlayerInput structures
        // For them to work, you must configure it's buttons and attach
        // a joystick to it (if you're going to use any at all)
        for (size_t i = 0;i < players;++i)
        {
            mPlayers.push_back(new PlayerInput(false,false,NULL));
        }
    }

    InputManager::~InputManager()
    {
        for (size_t i = 0;i < mPlayers.size();++i)
        {
            delete mPlayers[i];
        }
    }
};
