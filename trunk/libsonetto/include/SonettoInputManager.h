/*-----------------------------------------------------------------------------
This source file is part of Sonetto RPG Engine.

Copyright (C) 2007,2008 Arthur Carvalho de Souza Lima, Guilherme Prá Vieira


Sonetto RPG Engine is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Sonetto RPG Engine is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this library; if not, write to the Free Software Foundation,
Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA or go to
http://www.gnu.org/copyleft/lesser.txt
-----------------------------------------------------------------------------*/

#ifndef SONETTO_INPUTMANAGER_H
#define SONETTO_INPUTMANAGER_H

#include <vector>
#include "SonettoMain.h"

#include <Ogre.h>
#include <SDL/SDL.h>

namespace Sonetto
{
    /** @brief Describes a Sonetto virtual button

        These "virtual" buttons are going to be attached to "physical"
        keyboard keys, joystick buttons and axes.
    */
    enum BUTTON
    {
        BTN_TRIANGLE = 0,
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
        BTN_DPAD_LEFT
    };

    /** @brief Describes a Sonetto virtual axis

        These "virtual" axis are going to be attached to "physical"
        keyboard keys, joystick buttons and axes.
    */
    enum AXIS
    {
        AX_LEFT = 0,
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

    /** @brief Describes a Sonetto virtual button state

        These are the states a button can assume.
        KS_NONE means it is not pressed;
        KS_PRESS means it has just been pressed;
        KS_RELEASE means it has just been released;
        KS_HOLD means it is currently being held down;
    @see
        Sonetto::PlayerInput::getBtnState()
    */
    enum KEYSTATE
    {
        KS_NONE = 0,
        KS_PRESS,
        KS_RELEASE,
        KS_HOLD
    };

    /** @brief Physical input attachment

        Sonetto button checks are made through a call to
        Sonetto::PlayerInput::getBtnState(). Each of these buttons(Sonetto::BUTTON)
        can be configured to a physical attachment, that is, an Input Source. A Sonetto
        button can be attached to a keyboard key, joystick button or even an axis. This
        structure is attached to these virtual buttons and axis to link them to the
        physical input that is going to determine their states.
    @see
        Sonetto::InputSource::TYPE
    @see
        Sonetto::PlayerInput::configBtn()
    @see
        Sonetto::PlayerInput::configAxis()
    @see
        Sonetto::PlayerInput::config()
    */
    class SONETTO_EXPORT InputSource
    {
        public:
            /** @brief Describes a physical input attachment type

                When a button or axis is attached to a Sonetto::InputSource struct,
                this button's state will be based in a keyboard key when the struct is
                set to a type of IST_KEY, a joystick button when it's set to a type of
                IST_BUTTON, or a joystick axis when it's set to a type of IST_AXIS.
            @see
                Sonetto::InputSource
            @see
                Sonetto::PlayerInput::configBtn()
            @see
                Sonetto::PlayerInput::configAxis()
            @see
                Sonetto::PlayerInput::config()
            */
            enum TYPE
            {
                IST_KEY    = 0,
                IST_BUTTON,
                IST_AXIS
            };

            enum INVERT_INPUT
            {
                INV_ORDER    = 0x01,
                INV_X_POLARITY = 0x02,
                INV_Y_POLARITY = 0x04,
            };

            /** @brief Constructor

            @param
                aEnabled Whether the button attached to this source will be enabled or not. If not,
                the button will always be in a KS_NONE state.
            @param
                aType Defines the kind of input source
            @param
                aValue This argument depends on aType. If aType is set to IST_KEY, this value is
                an SDLKey. If it's set to IST_BUTTON, this is the button number in the joystick.
                And if it's set to IST_AXIS, this is a Sonetto::AXIS value.
            @param
                aInvert An OR'ed combination of InputSource::INVERT_INPUTs.
            @see
                http://www.libsdl.org/cgi/docwiki.cgi/SDLKey
            */
            InputSource(bool aEnabled = false,TYPE aType = IST_KEY,
                    unsigned short aValue = 0,char aInvert = 0x00)
                    : enabled(aEnabled), type(aType), value(aValue), invert(aInvert) {}

            /// @brief Whether this source is enabled or not
            bool enabled : 1;

            /// @brief Source type (Sonetto::InputSource::TYPE)
            char type : 3;

            /// @brief Order and polarity inversions (Sonetto::InputSource::INVERT_INPUT)
            char invert : 2;

            /// @brief Source value (depends on Sonetto::InputSource::type)
            unsigned short value;
    };

    /** @brief Player input class

        This class is the interface used to configure and retrieve information
        regarding a player's input. They are maintened by Sonetto::InputManager
        and are all disabled by default. To use a PlayerInput class, you first
        have to configure and enable it. You can assign buttons and axes to your
        joysticks and keyboard using configBtn(), configAxis() and
        config() methods. After you are done, you can just enable it by
        calling setEnabled(true). PlayerInput constructors and destructors are
        private, and thus you cannot create or delete them manually. Use the
        Sonetto::InputManager::getPlayer() to access them instead.
    @see
        Sonetto::InputManager
    */
    class SONETTO_EXPORT PlayerInput
    {
        public:
            /** @brief Size of input configuration arrays

            @see
                Sonetto::config()
            @see
                Sonetto::getConfig()
            */
            static const size_t INPUT_SRC_NUM = 24;

            /** @brief Update input states

                This is called by Sonetto::InputManager::update(), so you don't
                have to really care about it.
            */
            void update();

            /// @brief Configure an independent button
            inline void configBtn(BUTTON btn,const InputSource &input) { mInputCfg[btn] = input; }

            /// @brief Configure an independent axis
            void configAxis(AXIS axs,const InputSource &input);

            /** @brief Configure all input sources at once

                This method is useful for loading configuration directly
                from a file or memory region.
            @see
                Sonetto::InputSource::getConfig()
            */
            void config(const InputSource input[INPUT_SRC_NUM]);

            /// @brief Get independent button configuration
            inline const InputSource &getBtnConfig(BUTTON btn) const { return mInputCfg[btn]; }

            /// @brief Get independent axis configuration
            const InputSource &getAxisConfig(AXIS axs);

            /** @brief Get all input sources at once

                This method is useful for saving configuration directly to
                a file or memory region, as you don't have to check each
                input source independetly.
            @see
                Sonetto::InputSource::config()
            */
            inline const InputSource *getConfig() const { return mInputCfg; }

            /** @brief Set joystick from which this class will get information from

            @param
                joy SDL_Joystick pointer, which can be taken from
                Sonetto::InputManager::getJoystick(), or NULL to not use a joystick.
            */
            inline void setJoystick(SDL_Joystick *joy) { mJoy = joy; }

            /** @brief Get joystick from which this class will get information from

            @return
                SDL_Joystick pointer, or NULL when no joystick was set
            */
            inline SDL_Joystick *getJoystick() { return mJoy; }

            /// @brief Enables or disables this player's input updates
            inline void setEnabled(bool enable) { mEnabled = enable; }

            /// @brief Checks whether this PlayerInput is enabled or not
            inline bool isEnabled() const { return mEnabled; }

            /// @brief Enables or disables joystick rumble
            inline void setRumbleEnabled(bool enable) { mEnabled = enable; }

            /// @brief Checks whether rumble is enabled or not
            inline bool isRumbleEnabled() const { return mEnabled; }

            /// @brief Gets a button state
            inline const KEYSTATE getBtnState(BUTTON btn) const { return mBtnStates[btn]; }

            /// @brief Gets an axis current values
            Ogre::Vector2 getAxisValue(AXIS axs);

        private:
            /** @brief Constructor

                Can only be used by Sonetto::InputManager.
            @param
                enabled Whether this instance is enabled or not
            @param
                rumbleEnabled Whether this instance's rumble is enabled or not
            @param
                joy Joystick from which this instance will get input from
            */
            PlayerInput(bool enabled,bool rumbleEnabled,SDL_Joystick *joy = NULL);

            /// @brief Destructor
            ~PlayerInput();

            /// @brief Whether this PlayerInput is enabled or not
            bool mEnabled;

            /// @brief Whether is PlayerInput has rumble enabled or not
            bool mRumbleEnabled;

            /// @brief Joystick from which this PlayerInput will get its input from
            SDL_Joystick *mJoy;

            /// @brief Input source configurations
            InputSource mInputCfg[INPUT_SRC_NUM];

            /// @brief Button states
            KEYSTATE mBtnStates[16];

            /// @brief Axes values
            Ogre::Vector2 mAxesValues[2];

            friend class InputManager;
    };

    /** @brief Class responsible for managing input resources

        The InputManager is a class created and maintaned by Sonetto::Kernel.
        Its constructor and destructor are private, so you cannot create or delete
        them. From this class, you can get Sonetto::PlayerInput instances, which you
        can use for retrieving input from joysticks and keyboard. This class' instance
        can be accessed through a call to Sonetto::Kernel::get()->getInputMan().
    */
    class SONETTO_EXPORT InputManager
    {
        public:
            /** @brief Update all PlayerInputs.

                This is called by Sonetto::Kernel::run(), so you
                don't have to care about it.
            */
            void update();

            /** @brief Retrieves a Sonetto::PlayerInput given its index

            @remarks
                Be careful not to access an index greater than
                or equal to Sonetto::InputManager::getPlayerNum().
            */
            PlayerInput *getPlayer(size_t num) const;

            /// @brief Retrieves the number of PlayerInputs in the manager
            inline size_t getPlayerNum() const { return mPlayers.size(); }

            /** @brief Gets a joystick to be used in a Sonetto::PlayerInput::setJoystick() call

            @remarks
                Be careful not to access an index greater than
                or equal to Sonetto::InputManager::getJoystickNum().
            */
            SDL_Joystick *getJoystick(size_t index) const;

            /// @brief Returns the number of available joysticks for use
            size_t getJoystickNum() const;

            /// @brief Checks whether the joystick in the given index is being used by a PlayerInput or not
            bool joystickAttached(size_t index);

        private:
            /** @brief Constructor

                Called by Sonetto::Kernel.
            @param
                players Number of PlayerInputs to be created
            */
            InputManager(size_t players);

            /// @brief Destructor
            ~InputManager();

            /// @brief PlayerInput vector
            std::vector<PlayerInput *> mPlayers;

            friend class Kernel;
    };
} // namespace

#endif
