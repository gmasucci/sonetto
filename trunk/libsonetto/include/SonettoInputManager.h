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
#include <SDL/SDL_joystick.h>

// <todo> Comment this file

namespace Sonetto
{
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

    enum KEYSTATE
    {
        KS_NONE = 0,
        KS_PRESS,
        KS_RELEASE,
        KS_HOLD
    };

    enum INPUTSOURCETYPE {
        IST_KEY = 0,
        IST_BUTTON,
        IST_AXIS
    };

    class SONETTO_EXPORT InputSource
    {
        public:
            InputSource(bool aEnabled = false,INPUTSOURCETYPE aType = IST_KEY,char aValue = 0)
                    : enabled(aEnabled), type(aType), value(aValue) {}

            bool enabled : 1;
            char type    : 2;
            char value;
    };

    class SONETTO_EXPORT PlayerInput
    {
        public:
            static const size_t INPUT_SRC_NUM = 24;

            void update();

            void configureBtn(BUTTON btn,const InputSource &input) { mInputCfg[btn] = input; }
            void configureAxis(AXIS axs,const InputSource &input);
            void configureAll(const InputSource input[INPUT_SRC_NUM]);

            inline const InputSource    &getBtnConfiguration(BUTTON btn) const { return mInputCfg[btn]; }
            const        InputSource    &getAxisConfiguration(AXIS axs);
            inline const InputSource    *getAllConfiguration()           const { return mInputCfg; }

            inline       void            setEnabled(bool enable)               { mEnabled = enable; }
            inline       bool            isEnabled()                     const { return mEnabled;   }
            inline       void            setRumbleEnabled(bool enable)         { mEnabled = enable; }
            inline       bool            isRumbleEnabled()               const { return mEnabled;   }

            inline const KEYSTATE        getButtonState(BUTTON btn)      const { return mBtnStates[btn]; }
            inline       Ogre::Vector2   getAxisValue(AXIS axs);

        private:
            PlayerInput(bool enabled,bool rumbleEnabled,SDL_Joystick *joy = NULL);
            ~PlayerInput();

            bool           mEnabled;
            bool           mRumbleEnabled;
            SDL_Joystick  *mJoy;

            InputSource mInputCfg[INPUT_SRC_NUM];
            KEYSTATE    mBtnStates[16];
            float       mAxesValues[4];

            friend class InputManager;
    };

    class SONETTO_EXPORT InputManager
    {
        public:
            void update();

            PlayerInput   *getPlayer(size_t num) const;
            inline size_t  getPlayerNum() const { return mPlayers.size(); }

        private:
            InputManager(size_t players,Ogre::RenderWindow *win = NULL);
            ~InputManager() {}

            std::vector<PlayerInput *>  mPlayers;

            friend class Kernel;
    };
} // namespace

#endif
