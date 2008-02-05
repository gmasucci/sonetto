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

#ifndef __SONETTO_INPUTMANAGER_H_
#define __SONETTO_INPUTMANAGER_H_

//Use this define to signify OIS will be used as a DLL
//(so that dll import/export macros are in effect)
#define OIS_DYNAMIC_LIB
#include <Ogre.h>
#include <OIS/OIS.h>

namespace Sonetto {
    const Ogre::uint8 MAX_JOY = 4;

    enum BUTTON {
        BTN_TRIANGLE    = 0x00,
        BTN_CIRCLE      = 0x01,
        BTN_CROSS       = 0x02,
        BTN_SQUARE      = 0x03,
        BTN_L2          = 0x04,
        BTN_R2          = 0x05,
        BTN_L1          = 0x06,
        BTN_R1          = 0x07,
        BTN_START       = 0x08,
        BTN_SELECT      = 0x09,
        BTN_L3          = 0x0A,
        BTN_R3          = 0x0B,
        BTN_DPAD_UP     = 0x0C,
        BTN_DPAD_RIGHT  = 0x0D,
        BTN_DPAD_DOWN   = 0x0E,
        BTN_DPAD_LEFT   = 0x0F
    };

    enum AXIS {
        AX_LEFT         = 0x00,
        AX_RIGHT        = 0x02
    };

    enum KEYSTATE {
        KS_NONE     = 0x00, // 0000
        KS_PRESS    = 0x01, // 0001
        KS_RELEASE  = 0x02, // 0010
        KS_HOLD     = 0x03  // 0011
    };

    struct KeyStates {
        float    axes[4];       // Controller axis
        KEYSTATE buttons[16];   // Common buttons
    };

    struct KeyConfig {
        char inputDevice;

        char analogCount;
        bool enableRumble;

        Ogre::uint16 axes[8];       // Controller axes
        Ogre::uint16 buttons[16];   // Common buttons
    };

    class InputManager {
    public:
        InputManager() : mInputManager(0), mInitialised(0) {}

        ~InputManager() {}

        bool           initialise(Ogre::RenderWindow *win,KeyConfig *kc);
        bool           deinitialise();

        void           updateInput();

        void           setKeyConfig(Ogre::uint8 playerID, KeyConfig *kc);
        KeyConfig     *getKeyConfig(Ogre::uint8 playerID);

        bool           playerExists(Ogre::uint8 playerID);
        KEYSTATE       getButtonState(Ogre::uint8 playerID, BUTTON btn);
        Ogre::Vector2  getAxis(Ogre::uint8 playerID, AXIS axis);

    protected:
        // OIS devices (mouse is evil from hell)
        OIS::InputManager               *mInputManager;
        OIS::Keyboard                   *mKeyboard;
        OIS::JoyStick                   *mJoy[MAX_JOY];

        bool mInitialised;

        KeyStates mKeyStates[MAX_JOY];
        KeyConfig mKeyConfig[MAX_JOY];
    };
}
; // namespace

#endif
