/*-----------------------------------------------------------------------------
This source file is part of Sonetto RPG Engine.

Copyright (C) 2007,2008 Arthur Carvalho de Souza Lima, Guilherme PrÃ¡ Vieira


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

#ifndef __SONETTO_KERNEL_H_
#define __SONETTO_KERNEL_H_

// Forward declarations
namespace Sonetto {
    class Kernel;
    
    extern Kernel *KERNEL;
};

#include <Ogre.h>
#include <stack>
//Use this define to signify OIS will be used as a DLL
//(so that dll import/export macros are in effect)
#define OIS_DYNAMIC_LIB
#include <OIS/OIS.h>

#include "Module.h"

#include "SonettoFontSerializer.h"
#include "SonettoFontManager.h"
#include "SonettoTextFactory.h"



namespace Sonetto {
    enum BUTTON {
        TRIANGLE = 0x0,
        CIRCLE = 0x1,
        CROSS = 0x2,
        SQUARE = 0x3,
        L2 = 0x4,
        R2 = 0x5,
        L1 = 0x6,
        R1 = 0x7,
        START = 0x8,
        SELECT = 0x9,
        L3 = 0xA,
        R3 = 0xB,
        DPAD_UP = 0xC,
        DPAD_RIGHT = 0xD,
        DPAD_BOTTOM = 0xE,
        DPAD_LEFT = 0xF
    };

    enum KEYSTATE {
        NONE     = 0x00, // 0000
        PRESS    = 0x01, // 0001
        RELEASE  = 0x02, // 0010
        HOLD     = 0x03  // 0011
    };

    struct KeyStates {
        float    axis[4];       // Controller axis
        KEYSTATE buttons[16];   // Common buttons
    };
    
    struct KeyConfig {
        char inputDevice;
        
        char hasAnalog;
        
        bool enableRumble;
        
        Ogre::uint16 axis[8];       // Controller axis
        Ogre::uint16 buttons[16];   // Common buttons
    };
    
    class Kernel {
        friend class Module;
    public:
        /// Default constructor.
        Kernel() : mRoot(0), mWindow(0), mViewport(0), mOverlayMan(0),
        mInputManager(0), mKeyboard(0), mJoy(0) {}
        
        /// Default destructor.
        ~Kernel() {}
        
        /** @brief Initialise Ogre, OIS, and other Sonetto dependencies
         *  @return Wheter it was successful or not.
         */
        bool initialise();
        
        /** @brief Deinitialise everything initialised before by initialise().
         */
        bool deinitialise();
        
        bool run();
        
        void updateInput();
        
        void pushModule(Module * pModule,bool haltMode = false);
        
        void popModule();
        
    public: // Change to protected or private later
        
        /// Module Stack
        std::stack<Module*> mModuleList;
        
        Ogre::Root              *mRoot;
        Ogre::RenderWindow      *mWindow;
        Ogre::Viewport          *mViewport;
        Ogre::OverlayManager    *mOverlayMan;
        Ogre::ResourceGroupManager   *mResourceMan;
        
        // OIS devices (mouse is evil from hell)
        OIS::InputManager* mInputManager;
        OIS::Keyboard* mKeyboard;
        OIS::JoyStick* mJoy;
        
        bool mHasEnded;
        bool mHasInitialised;
        
        KeyStates mKeyStates;
        KeyConfig mKeyConfig;
        
        // Sonetto Stuff
        FontManager *mFontMan;
        TextElementFactory* mTextElementFactory;
        
        //Temporary stuff
        FontPtr mMainFont;
        Ogre::Overlay * mDebugOverlay;
        Ogre::OverlayContainer * mDebugOverlayContainer;
        Ogre::OverlayElement * mDebugText;
        
    };
}; // namespace

#endif
