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

#ifndef SONETTO_PLAYERINPUT_H
#define SONETTO_PLAYERINPUT_H

#include <vector>
#include "SonettoPrerequisites.h"
#include "SonettoJoystick.h"

// Types needed by other headers
namespace Sonetto
{
    /// Vector of PlayerInput's
    typedef std::vector<PlayerInput *> PlayerInputVector;

    /** Describes a Sonetto virtual button

        These virtual buttons are going to be attached to physical
        keyboard keys, joystick buttons or axes using an InputSource.
    */
    enum Button
    {
        BTN_TRIANGLE,
        BTN_CIRCLE,
        BTN_CROSS,
        BTN_SQUARE,
        BTN_L2,
        BTN_R2,
        BTN_L1,
        BTN_R1,
        BTN_START,
        BTN_SELECT,
        BTN_L3,
        BTN_R3,
        BTN_DPAD_UP,
        BTN_DPAD_RIGHT,
        BTN_DPAD_DOWN,
        BTN_DPAD_LEFT,
        BTN_LAST = BTN_DPAD_LEFT
    };

    /** Describes a Sonetto virtual axis

        These virtual axes are going to be attached to physical
        keyboard keys, joystick buttons and axes using an InputSource.
    */
    enum Axis
    {
        AX_LEFT,
        AXE_LEFT_UP,
        AXE_LEFT_RIGHT,
        AXE_LEFT_DOWN,
        AXE_LEFT_LEFT,
        AX_RIGHT,
        AXE_RIGHT_UP,
        AXE_RIGHT_RIGHT,
        AXE_RIGHT_DOWN,
        AXE_RIGHT_LEFT
    };

    /** Describes a Sonetto virtual button state

        Each virtual button or keyboard key can assume one of these states.
    @see
        Sonetto::PlayerInput::getBtnState()
    @see
        Sonetto::InputManager::getDirectKeyState()
    */
    enum KeyState
    {
        /// Not pressed
        KS_NONE = 0,
        /// Has just been pressed
        KS_PRESS,
        /// Has just been release
        KS_RELEASE,
        /// Is currentlu being held down
        KS_HOLD
    };
} // namespace

#include <OgreVector2.h>
#include "SonettoInputManager.h"
#include "SonettoInputSource.h"

namespace Sonetto
{
    /** Player input class

        This class is the interface used to configure and retrieve information
        regarding one player's input. They are maintened by Sonetto::InputManager
        and are all disabled by default. To use a PlayerInput class, you first
        have to configure and enable it. You can assign buttons and axes to your
        joysticks and keyboard using configBtn(), configAxis() and
        config() methods. After you are done, you can just enable it by
        calling setEnabled(true).
    @see
        Sonetto::InputManager
    */
    class SONETTO_API PlayerInput
    {
        public:
            /** Size of input configuration arrays

            @see
                mInputCfg
            */
            static const size_t INPUT_SRC_NUM = 24;

            /** Constructor

                Should never be called directly. Use InputManager instead.
            @param
                enabled Whether this instance is enabled or not.
            @param
                joyID Joystick ID from which this instance will get input from.
                A value of zero means no joystick will be used.
            */
            PlayerInput(bool enabled = false,uint32 joyID = 0);

            /// Destructor
            ~PlayerInput() {}

            /** Updates input states

            @remarks
                This is called by InputManager::update(), so you don't
                really have to worry about it.
            */
            void update();

            /// Configures a single button
            inline void configBtn(Button btn,const InputSource &input)
                    { mInputCfg[btn] = input; }

            /// Configures a single axis
            void configAxis(Axis axs,const InputSource &input);

            /// Gets a single button input source configuration
            inline const InputSource &getBtnConfig(Button btn) const { return mInputCfg[btn]; }

            /// Gets a single axis input source configuration
            inline const InputSource &getAxisConfig(Axis axs) const
                    { return mInputCfg[ (BTN_LAST + 1) + axs ]; }

            /** Sets joystick ID from which this class will get information from

            @param
                joyID Joystick ID to be used. A value of zero means no joystick
                will be used.
            */
            void setJoystick(uint32 joyID);

            /** Gets joystick ID from which this class will get information from

            @return
                Joystick ID. A value of zero means no joystick is being used.
            */
            uint32 getJoystick() const;

            /// Enables or disables this player's input updates
            inline void setEnabled(bool enable) { mEnabled = enable; }

            /// Checks whether this PlayerInput is enabled or not
            inline bool isEnabled() const { return mEnabled; }

            /// Checks whether the attached joystick is physically plugged or not
            bool isPlugged() const;

            /// Gets a button state
            inline const KeyState getBtnState(Button btn) const { return mBtnStates[btn]; }

            /// Gets an axis current values
            Ogre::Vector2 getAxisValue(Axis axs);

        private:
            /// Whether this PlayerInput is enabled or not
            bool mEnabled;

            /// Joystick shared pointer from which this PlayerInput will get its input from
            JoystickPtr mJoy;

            /// Input source attachments
            InputSource mInputCfg[INPUT_SRC_NUM];

            /// Button states
            KeyState mBtnStates[BTN_LAST + 1];

            /// Axes' values
            Ogre::Vector2 mAxesValues[2];
    };
} // namespace

#endif
