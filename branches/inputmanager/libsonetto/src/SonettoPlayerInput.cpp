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
            : mEnabled(enabled)
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
    void PlayerInput::setJoystick(uint32 joyID)
    {
        if (joyID != 0) {
            // Gets shared pointer to joystick if requested
            mJoy = InputManager::getSingletonPtr()->_getJoystick(joyID);
        } else {
            // Releases joystick
            mJoy.setNull();
        }
    }
    // ----------------------------------------------------------------------
    bool PlayerInput::isPlugged() const
    {
        // If no joystick is being used, returns false
        if (mJoy.isNull())
        {
            return false;
        }

        // Returns whether the joystick is plugged or not
        return mJoy->isPlugged();
    }
    // ----------------------------------------------------------------------
    void PlayerInput::update()
    {
        // Resets axes' values
        mAxesValues[0].x = mAxesValues[0].y = 0.0f;
        mAxesValues[1].x = mAxesValues[1].y = 0.0f;

        if (mEnabled) {
            InputManager *inputMan = InputManager::getSingletonPtr();

            for (size_t i = 0;i < INPUT_SRC_NUM;++i)
            {
                if (i <= BTN_LAST) { // Buttons
                    KeyState state = KS_NONE;

                    switch (mInputCfg[i].type) {
                        case InputSource::IST_KEY:
                            // Gets key state from InputManager
                            if (inputMan->getDirectKeyState(mInputCfg[i].value) != KS_NONE) {
                                state = KS_PRESS;
                            } else {
                                state = KS_NONE;
                            }
                        break;

                        case InputSource::IST_BUTTON:
                            if (!mJoy.isNull())
                            {
                                state = mJoy->getRawButtonState((Joystick::RawButton)mInputCfg[i].value) ? KS_PRESS : KS_NONE;
                            }
                        break;

                        case InputSource::IST_AXIS:
                            if (!mJoy.isNull())
                            {
                                Ogre::Vector2 leftVect,rightVect;

                                leftVect = mJoy->getRawAnalogState(Joystick::RWA_1,(InputSource::InvertInput)mInputCfg[i].invert);
                                rightVect = mJoy->getRawAnalogState(Joystick::RWA_2,(InputSource::InvertInput)mInputCfg[i].invert);

                                switch (mInputCfg[i].value)
                                {
                                    case AXE_LEFT_LEFT:
                                        state = (leftVect.x < -mInputCfg[i].axisDeadzone) ? KS_PRESS : KS_NONE;
                                    break;

                                    case AXE_LEFT_RIGHT:
                                        state = (leftVect.x > mInputCfg[i].axisDeadzone) ? KS_PRESS : KS_NONE;
                                    break;

                                    case AXE_LEFT_UP:
                                        state = (leftVect.y < -mInputCfg[i].axisDeadzone) ? KS_PRESS : KS_NONE;
                                    break;

                                    case AXE_LEFT_DOWN:
                                        state = (leftVect.y > mInputCfg[i].axisDeadzone) ? KS_PRESS : KS_NONE;
                                    break;

                                    case AXE_RIGHT_LEFT:
                                        state = (rightVect.x < -mInputCfg[i].axisDeadzone) ? KS_PRESS : KS_NONE;
                                    break;

                                    case AXE_RIGHT_RIGHT:
                                        state = (rightVect.x > mInputCfg[i].axisDeadzone) ? KS_PRESS : KS_NONE;
                                    break;

                                    case AXE_RIGHT_UP:
                                        state = (rightVect.y < -mInputCfg[i].axisDeadzone) ? KS_PRESS : KS_NONE;
                                    break;

                                    case AXE_RIGHT_DOWN:
                                        state = (rightVect.y > mInputCfg[i].axisDeadzone) ? KS_PRESS : KS_NONE;
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
                    int   axis = (i - (BTN_LAST + 1) <= 3) ? 0 : 1;
                    char  dir  = (i - (BTN_LAST + 1)) % 4;

                    switch (mInputCfg[i].type)
                    {
                        case InputSource::IST_KEY:
                            switch (dir)
                            {
                                case 0:
                                    if (inputMan->getDirectKeyState(mInputCfg[i].value) != KS_NONE)
                                    {
                                        mAxesValues[axis].y -= 1.0f;
                                    }
                                break;

                                case 1:
                                    if (inputMan->getDirectKeyState(mInputCfg[i].value) != KS_NONE)
                                    {
                                        mAxesValues[axis].x += 1.0f;
                                    }
                                break;

                                case 2:
                                    if (inputMan->getDirectKeyState(mInputCfg[i].value) != KS_NONE)
                                    {
                                        mAxesValues[axis].y += 1.0f;
                                    }
                                break;

                                case 3:
                                    if (inputMan->getDirectKeyState(mInputCfg[i].value) != KS_NONE)
                                    {
                                        mAxesValues[axis].x -= 1.0f;
                                    }
                                break;
                            }
                        break;

                        case InputSource::IST_BUTTON:
                            if (!mJoy.isNull())
                            {
                                switch (dir)
                                {
                                    case 0:
                                        if (mJoy->getRawButtonState((Joystick::RawButton)mInputCfg[i].value))
                                        {
                                            mAxesValues[axis].y -= 1.0f;
                                        }
                                    break;

                                    case 1:
                                        if (mJoy->getRawButtonState((Joystick::RawButton)mInputCfg[i].value))
                                        {
                                            mAxesValues[axis].x += 1.0f;
                                        }
                                    break;

                                    case 2:
                                        if (mJoy->getRawButtonState((Joystick::RawButton)mInputCfg[i].value))
                                        {
                                            mAxesValues[axis].y += 1.0f;
                                        }
                                    break;

                                    case 3:
                                        if (mJoy->getRawButtonState((Joystick::RawButton)mInputCfg[i].value))
                                        {
                                            mAxesValues[axis].x -= 1.0f;
                                        }
                                    break;
                                }
                            }
                        break;

                        case InputSource::IST_AXIS:
                            if (!mJoy.isNull())
                            {
                                mAxesValues[axis] = mJoy->getRawAnalogState((Joystick::RawAnalog)(dir / 4),
                                        (InputSource::InvertInput)mInputCfg[i].invert);

                                i += 3;
                                continue;
                            }
                        break;
                    }
                }
            }

            mAxesValues[0].normalise();
            mAxesValues[1].normalise();
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
    Ogre::Vector2 PlayerInput::getAxisValue(Axis axs)
    {
        switch (axs)
        {
            case AX_LEFT:  return mAxesValues[0]; break;
            case AX_RIGHT: return mAxesValues[1]; break;

            default: return Ogre::Vector2(0.0f,0.0f); break;
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
