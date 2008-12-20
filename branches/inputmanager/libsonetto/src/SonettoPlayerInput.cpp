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

#include "SonettoPlayerInput.h"
#include "SonettoJoystick.h"

namespace Sonetto
{
    // ----------------------------------------------------------------------
    // Sonetto::PlayerInput implementation
    // ----------------------------------------------------------------------
    PlayerInput::PlayerInput(bool enabled,uint32 joyID)
            : mEnabled(enabled), mJoyID(joyID), mJoy(NULL)
    {
        // Clears arrays
        memset(mBtnStates,0x00,sizeof(mBtnStates));
        memset(mAxesValues,0x00,sizeof(mAxesValues));
        memset(mInputCfg,0x00,sizeof(mInputCfg));

        // Gets joystick pointer if a joystick ID was set
        if (joyID != 0)
        {
            mJoy = InputManager::getSingletonPtr()->_getJoystick(joyID);
        }
    }
    // ----------------------------------------------------------------------
    PlayerInput::~PlayerInput()
    {
        // Releases joystick if using a valid one
        if (mJoy)
        {
            InputManager::getSingletonPtr()->_releaseJoystick(mJoyID);
        }
    }
    // ----------------------------------------------------------------------
    void PlayerInput::setJoystick(uint32 joyID)
    {
        // Releases current joystick if using a valid one
        if (mJoy)
        {
            InputManager::getSingletonPtr()->_releaseJoystick(mJoyID);
            mJoy = NULL;
        }

        // Sets new joystick ID
        mJoyID = joyID;

        // Gets pointer to joystick if requested
        if (joyID != 0) {
            mJoy = InputManager::getSingletonPtr()->_getJoystick(joyID);
        }
    }
    // ----------------------------------------------------------------------
    bool PlayerInput::isPlugged() const
    {
        // If no valid joystick is being used, returns false
        if (!mJoy)
        {
            return false;
        }

        // Returns whether the joystick is plugged or not
        return mJoy->_isPlugged();
    }
    // ----------------------------------------------------------------------
    void PlayerInput::update()
    {
        // Resets axes' values
        mAxesValues[0].x = mAxesValues[0].y = 0.0f;
        mAxesValues[1].x = mAxesValues[1].y = 0.0f;

        if (mEnabled) {
            uint8 *keystates = SDL_GetKeyState(NULL);

            // If a joystick ID was requested but was not yet open (i.e. wasn't plugged),
            // checks again whether it can be opened
            if (mJoyID != 0 && !mJoy)
            {
                mJoy = InputManager::getSingletonPtr()->_getJoystick(mJoyID);
            }

            for (size_t i = 0;i < INPUT_SRC_NUM;++i)
            {
                if (i <= BTN_LAST) { // Buttons
                    KeyState state = KS_NONE;

                    switch (mInputCfg[i].type) {
                        case InputSource::IST_KEY:
                            state = (keystates[mInputCfg[i].value]) ? KS_PRESS : KS_NONE;
                        break;

                        case InputSource::IST_BUTTON:
                            if (mJoy)
                            {
                                state = (SDL_JoystickGetButton((SDL_Joystick *)mJoy,
                                         mInputCfg[i].value)) ? KS_PRESS : KS_NONE;
                            }
                        break;

                        case InputSource::IST_AXIS:
                            if (mJoy)
                            {
                                Axis axisCheck;
                                uint8 axisNum;
                                float value;

                                switch (mInputCfg[i].value)
                                {
                                    case AXE_LEFT_LEFT:
                                    case AXE_LEFT_RIGHT:
                                        // Inverts axis number if INV_ORDER bit is set
                                        if (mInputCfg[i].invert & InputSource::INV_ORDER) {
                                            axisNum = 1;
                                        } else {
                                            axisNum = 0;
                                        }

                                        // Gets axis value as a [-1.0f, +1.0f] ranging float
                                        value = SDL_JoystickGetAxis((SDL_Joystick *)mJoy,axisNum);
                                        value /= ((value > 0) ? 32767.0f : 32768.0f);

                                        // Inverts axis direction check if INV_Y_POLARITY bit is set
                                        if (mInputCfg[i].invert & InputSource::INV_X_POLARITY) {
                                            axisCheck = AXE_LEFT_RIGHT;
                                        } else {
                                            axisCheck = AXE_LEFT_LEFT;
                                        }

                                        // Gets button state from axis
                                        if (mInputCfg[i].value == axisCheck) {
                                            state = (value <= -mInputCfg[i].axisDeadzone) ? KS_PRESS : KS_NONE;
                                        } else {
                                            state = (value >= +mInputCfg[i].axisDeadzone) ? KS_PRESS : KS_NONE;
                                        }
                                    break;

                                    case AXE_LEFT_UP:
                                    case AXE_LEFT_DOWN:
                                        // Inverts axis number if INV_ORDER bit is set
                                        if (mInputCfg[i].invert & InputSource::INV_ORDER) {
                                            axisNum = 0;
                                        } else {
                                            axisNum = 1;
                                        }

                                        // Gets axis value as a [-1.0f, +1.0f] ranging float
                                        value = SDL_JoystickGetAxis((SDL_Joystick *)mJoy,axisNum);
                                        value /= ((value > 0) ? 32767.0f : 32768.0f);

                                        // Inverts axis direction check if INV_Y_POLARITY bit is set
                                        if (mInputCfg[i].invert & InputSource::INV_Y_POLARITY) {
                                            axisCheck = AXE_LEFT_DOWN;
                                        } else {
                                            axisCheck = AXE_LEFT_UP;
                                        }

                                        // Gets button state from axis
                                        if (mInputCfg[i].value == axisCheck) {
                                            state = (value <= -mInputCfg[i].axisDeadzone) ? KS_PRESS : KS_NONE;
                                        } else {
                                            state = (value >= +mInputCfg[i].axisDeadzone) ? KS_PRESS : KS_NONE;
                                        }
                                    break;

                                    case AXE_RIGHT_UP:
                                    case AXE_RIGHT_DOWN:
                                        // Inverts axis number if INV_ORDER bit is set
                                        if (mInputCfg[i].invert & InputSource::INV_ORDER) {
                                            axisNum = 3;
                                        } else {
                                            axisNum = 2;
                                        }

                                        // Gets axis value as a [-1.0f, +1.0f] ranging float
                                        value = SDL_JoystickGetAxis((SDL_Joystick *)mJoy,axisNum);
                                        value /= ((value > 0) ? 32767.0f : 32768.0f);

                                        // Inverts axis direction check if INV_Y_POLARITY bit is set
                                        if (mInputCfg[i].invert & InputSource::INV_Y_POLARITY) {
                                            axisCheck = AXE_RIGHT_DOWN;
                                        } else {
                                            axisCheck = AXE_RIGHT_UP;
                                        }

                                        // Gets button state from axis
                                        if (mInputCfg[i].value == axisCheck) {
                                            state = (value <= -mInputCfg[i].axisDeadzone) ? KS_PRESS : KS_NONE;
                                        } else {
                                            state = (value >= +mInputCfg[i].axisDeadzone) ? KS_PRESS : KS_NONE;
                                        }
                                    break;

                                    case AXE_RIGHT_LEFT:
                                    case AXE_RIGHT_RIGHT:
                                        // Inverts axis number if INV_ORDER bit is set
                                        if (mInputCfg[i].invert & InputSource::INV_ORDER) {
                                            axisNum = 2;
                                        } else {
                                            axisNum = 3;
                                        }

                                        // Gets axis value as a [-1.0f, +1.0f] ranging float
                                        value = SDL_JoystickGetAxis((SDL_Joystick *)mJoy,axisNum);
                                        value /= ((value > 0) ? 32767.0f : 32768.0f);

                                        // Inverts axis direction check if INV_Y_POLARITY bit is set
                                        if (mInputCfg[i].invert & InputSource::INV_X_POLARITY) {
                                            axisCheck = AXE_RIGHT_RIGHT;
                                        } else {
                                            axisCheck = AXE_RIGHT_LEFT;
                                        }

                                        // Gets button state from axis
                                        if (mInputCfg[i].value == axisCheck) {
                                            state = (value <= -mInputCfg[i].axisDeadzone) ? KS_PRESS : KS_NONE;
                                        } else {
                                            state = (value >= +mInputCfg[i].axisDeadzone) ? KS_PRESS : KS_NONE;
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
                    int   axis = (i - (BTN_LAST + 1) >= 0 && i - (BTN_LAST + 1) <= 3) ? 0 : 1;
                    char  dir  = (i - (BTN_LAST + 1)) % 4;

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
                                        if (SDL_JoystickGetButton((SDL_Joystick *)mJoy,mInputCfg[i].value))
                                        {
                                            value = (mAxesValues[axis].y != 0.0f) ? 0.0f : -1.0f;
                                        }
                                    break;

                                    case 1: // Right
                                        if (SDL_JoystickGetButton((SDL_Joystick *)mJoy,mInputCfg[i].value))
                                        {
                                            value = (mAxesValues[axis].x != 0.0f) ? 0.0f : +1.0f;
                                        }
                                    break;

                                    case 2: // Down
                                        if (SDL_JoystickGetButton((SDL_Joystick *)mJoy,mInputCfg[i].value))
                                        {
                                            value = (mAxesValues[axis].y != 0.0f) ? 0.0f : +1.0f;
                                        }
                                    break;

                                    case 3: // Left
                                        if (SDL_JoystickGetButton((SDL_Joystick *)mJoy,mInputCfg[i].value))
                                        {
                                            value = (mAxesValues[axis].x != 0.0f) ? 0.0f : -1.0f;
                                        }
                                    break;
                                }
                            }
                        break;

                        case InputSource::IST_AXIS:
                            if (dir == 0 || dir == 4)
                            {
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

                                    mAxesValues[axis].x =
                                            SDL_JoystickGetAxis((SDL_Joystick *)mJoy,cfgAxis);     // Horizontal
                                    mAxesValues[axis].y =
                                            SDL_JoystickGetAxis((SDL_Joystick *)mJoy,cfgAxis + 1); // Vertical
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

                    // Normalises vector
                    mAxesValues[axis].normalise();
                }
            }
        } else {
            // Releases buttons when this PlayerInput is disabled
            for (size_t i = 0;i < 16;++i)
            {
                switch (mBtnStates[i])
                {
                    case KS_PRESS:
                        mBtnStates[i] = KS_RELEASE;
                    break;

                    case KS_HOLD:
                        mBtnStates[i] = KS_RELEASE;
                    break;

                    case KS_RELEASE:
                        mBtnStates[i] = KS_NONE;
                    break;

                    default: break;
                }
            }
        }
    }
    // ----------------------------------------------------------------------
    void PlayerInput::configAxis(Axis axs,const InputSource &input)
    {
        switch (axs)
        {
            case AX_LEFT:
                mInputCfg[BTN_LAST + 1] = input;
            break;

            case AXE_LEFT_UP:
            case AXE_LEFT_RIGHT:
            case AXE_LEFT_DOWN:
            case AXE_LEFT_LEFT:
                mInputCfg[ (BTN_LAST + 1) + (axs - 1) ] = input;
            break;

            case AX_RIGHT:
                mInputCfg[20] = input;
            break;

            case AXE_RIGHT_UP:
            case AXE_RIGHT_RIGHT:
            case AXE_RIGHT_DOWN:
            case AXE_RIGHT_LEFT:
                mInputCfg[ (BTN_LAST + 1) + 4 + (axs - 6) ] = input;
            break;
        }
    }
} // namespace
