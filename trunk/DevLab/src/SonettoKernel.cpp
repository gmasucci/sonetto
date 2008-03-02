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

#include <Ogre.h>
#include "SonettoKernel.h"

namespace Sonetto {
    Kernel *KERNEL;

    bool Kernel::initialise() {
        KeyConfig kc[4]; // Temporary key configuration for the InputManager

        if (!mInitialised) {
            mShutDown = false;
            mRoot = new Ogre::Root("plugins.dlc","devlab.dlc","");
            if (mRoot->showConfigDialog()) {
                // If returned true, user clicked OK so initialise
                // Here we choose to let the system create a default rendering window by passing 'true'
                mWindow = mRoot->initialise(true, "Sonetto Development Laboratory");
            } else {
                return false;
            }

            memset(kc,0x00,sizeof(kc));

            // Temporary input configurations (debugging purposes)
            kc[0].inputDevice  = 1;     // Joystick ID (0: No Joystick)
            kc[0].analogCount  = 2;     // Two analog sticks or emulated buttons
            kc[0].enableRumble = false; // Let's disable it for now.

            // Analog Axes
            kc[0].axes[0]                 = 0x1000;
            kc[0].axes[1]                 = 0x2000;
            kc[0].axes[2]                 = 0x4000;
            kc[0].axes[3]                 = 0x3000;

            // Unused (used when the analog input is emulated using buttons)
            kc[0].axes[4]                 = 0x0000;
            kc[0].axes[5]                 = 0x0000;
            kc[0].axes[6]                 = 0x0000;
            kc[0].axes[7]                 = 0x0000;

            kc[0].buttons[BTN_TRIANGLE]   = 0x00FF;
            kc[0].buttons[BTN_CIRCLE]     = 0x0100;
            kc[0].buttons[BTN_CROSS]      = 0x0200;
            kc[0].buttons[BTN_SQUARE]     = 0x0300;

            kc[0].buttons[BTN_L2]         = 0x0400;
            kc[0].buttons[BTN_R2]         = 0x0500;
            kc[0].buttons[BTN_L1]         = 0x0600;
            kc[0].buttons[BTN_R1]         = 0x0700;

            kc[0].buttons[BTN_SELECT]     = 0x0800;
            kc[0].buttons[BTN_START]      = 0x0900;

            kc[0].buttons[BTN_L3]         = 0x0A00;
            kc[0].buttons[BTN_R3]         = 0x0B00;

            kc[0].buttons[BTN_DPAD_UP]    = 0x0C00;
            kc[0].buttons[BTN_DPAD_RIGHT] = 0x0D00;
            kc[0].buttons[BTN_DPAD_DOWN]  = 0x0E00;
            kc[0].buttons[BTN_DPAD_LEFT]  = 0x0F00;

            kc[1].inputDevice  = 2;     // Joystick ID (0: No Joystick)
            kc[1].analogCount  = 2;     // Two analog sticks or emulated buttons
            kc[1].enableRumble = false; // Let's disable it for now.

            // Analog Axes
            kc[1].axes[0]                 = 0x1000;
            kc[1].axes[1]                 = 0x2000;
            kc[1].axes[2]                 = 0x4000;
            kc[1].axes[3]                 = 0x3000;

            // Unused (used when the analog input is emulated using buttons)
            kc[1].axes[4]                 = 0x0000;
            kc[1].axes[5]                 = 0x0000;
            kc[1].axes[6]                 = 0x0000;
            kc[1].axes[7]                 = 0x0000;

            kc[1].buttons[BTN_TRIANGLE]   = 0x00FF;
            kc[1].buttons[BTN_CIRCLE]     = 0x0100;
            kc[1].buttons[BTN_CROSS]      = 0x0200;
            kc[1].buttons[BTN_SQUARE]     = 0x0300;

            kc[1].buttons[BTN_L2]         = 0x0400;
            kc[1].buttons[BTN_R2]         = 0x0500;
            kc[1].buttons[BTN_L1]         = 0x0600;
            kc[1].buttons[BTN_R1]         = 0x0700;

            kc[1].buttons[BTN_SELECT]     = 0x0800;
            kc[1].buttons[BTN_START]      = 0x0900;

            kc[1].buttons[BTN_L3]         = 0x0A00;
            kc[1].buttons[BTN_R3]         = 0x0B00;

            kc[1].buttons[BTN_DPAD_UP]    = 0x0C00;
            kc[1].buttons[BTN_DPAD_RIGHT] = 0x0D00;
            kc[1].buttons[BTN_DPAD_DOWN]  = 0x0E00;
            kc[1].buttons[BTN_DPAD_LEFT]  = 0x0F00;

            // Get Ogre singletons for easier use
            mLogMan      = Ogre::LogManager::getSingletonPtr();
            mOverlayMan  = Ogre::OverlayManager::getSingletonPtr();
            mResourceMan = Ogre::ResourceGroupManager::getSingletonPtr();

            // Create managers
            mAudioMan           = new AudioManager();
            mInputMan           = new InputManager();
            mFontMan            = new FontManager();

            mAudioMan->initialise();
            mInputMan->initialise(mWindow,kc);

            mTextElementFactory = new TextElementFactory();
            mOverlayMan->addOverlayElementFactory(mTextElementFactory);

            // Temporary resource loading (debugging purposes)
            mResourceMan->addResourceLocation("basic_font.ctf","Zip","KERNEL",false);
            mResourceMan->loadResourceGroup("KERNEL",true,false);
            mMainFont = mFontMan->load("BaseFont.fnt","KERNEL");

            // Create the text overlay
            mDebugOverlay = mOverlayMan->create("DEBUG_OVERLAY");
            mDebugOverlayContainer = static_cast<Ogre::OverlayContainer *>(mOverlayMan->createOverlayElement("Panel","DEBUG_PANEL"));

            mDebugOverlayContainer->setDimensions(1.0f,1.0f);
            mDebugOverlayContainer->setPosition(0.0f,0.0f);

            mDebugOverlay->add2D(mDebugOverlayContainer);

            mDebugText = static_cast<TextElement *>(mOverlayMan->createOverlayElement("Text","DEBUG_TEXT"));
            mDebugOverlayContainer->addChild(mDebugText);

            mDebugText->setFont(mMainFont);
            mDebugText->setMessage("Testing Sonetto::InputManager !");
            mDebugText->setDimensions(1.0, 1.0);
            mDebugText->setPosition(0.0f,0.0f);
            mDebugText->setTextSize(0.05f);
            mDebugText->setAnimSettings(false,false,false,0.0f,0.0f);
            mDebugText->forceAnimReset();

            mDebugOverlay->show();
            mDebugOverlayContainer->show();
            mDebugText->show();

            // No controller plugged in
            if (!mInputMan->playerPlugged(0)) {
                // Create
                mDebugErrorText = static_cast<TextElement*>(mOverlayMan->createOverlayElement("Text","DEBUG_NOCONTROLLER_TEXT"));
                mDebugOverlayContainer->addChild(mDebugErrorText);

                // Cofigure
                mDebugErrorText->setFont(mMainFont);
                mDebugErrorText->setMessage("No controller detected.");
                mDebugErrorText->setDimensions(1.0f,1.0f);
                mDebugErrorText->setPosition(0.1f,0.45f);
                mDebugErrorText->setTextSize(0.10f);
                mDebugErrorText->setAnimSettings(true,true,false,10.0f,8.0f);
                mDebugErrorText->forceAnimReset();

                // Fire
                mDebugErrorText->show();
            }

            // We're all set
            mInitialised = true;

            return true;
        }
        return false;
    }

    bool Kernel::deinitialise() {
        if (mInitialised) {
            // Deinitialise OIS
            if (mInputMan) {
                mInputMan->deinitialise();

                delete mInputMan;
                mInputMan = NULL;
            }

            if(mAudioMan)
                delete mAudioMan;

            // At last, delete the Ogre::Root
            if (mRoot)
                delete mRoot;

            return true;
        }

        return false;
    }

    bool Kernel::run() {
        if (mInitialised) {
            while (!mShutDown) {
                // Update the window (Message Pump)
                Ogre::WindowEventUtilities::messagePump();

                // Get the input first
                mInputMan->updateInput();

                mAudioMan->update();

                // The Kernel must have something to execute
                assert(!mModuleList.empty());

                // Update the stack top Module
                mModuleList.top()->update(1.0f);

                //mDebugText->setMessage(Ogre::StringConverter::toString(mInputMan->getAxis(0,AX_LEFT))+" "+
                //                       Ogre::StringConverter::toString(mInputMan->getAxis(1,AX_RIGHT)));

                mRoot->renderOneFrame();
            }
            // Remove everything from the memory.
            deinitialise();
            return true;
        }

        return false;
    }


    void Kernel::pushModule(Module * pModule,bool haltMode) {
        assert(pModule); // We can't insert a NULL module!

        if(!mModuleList.empty()) {
            if(haltMode) {
                mModuleList.top()->halt(); // Put the current module to sleep
            } else {
                delete mModuleList.top();  // If it's going to change for real,
                                           // delete the current one.
                mModuleList.pop();         // Then remove it from the stack
            }
        }

        mModuleList.push(pModule);  // Insert the new module on the stack's top
        pModule->enter();           // Enter the new module
    }

    void Kernel::popModule() {
        assert(mModuleList.size() >= 2); // Ensure the stack wont be empty. A Kernel
                                         // must have at least one Module.

        mModuleList.top()->exit(); // Cleanup
        delete mModuleList.top();  // Delete the module list
        mModuleList.pop();         // Then remove it from the stack

        mModuleList.top()->wakeup(); // Now wake up the previous module
    }
}; // namespace

