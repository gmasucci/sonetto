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

#include <Ogre.h>
#include "Kernel.h"

namespace Sonetto {
    Kernel *KERNEL = new Kernel();
    
    bool Kernel::initialise() {
        if (!mHasInitialised) {
            mHasEnded = false;
            mRoot = new Ogre::Root("plugins.dlc","devlab.dlc","DevLab.log");
            if (mRoot->showConfigDialog()) {
                // If returned true, user clicked OK so initialise
                // Here we choose to let the system create a default rendering window by passing 'true'
                mWindow = mRoot->initialise(true, "Sonetto Development Laboratory");
            } else {
                return false;
            }

            // OIS initialisation
            OIS::ParamList pl;
            size_t windowHnd = 0;
            std::ostringstream windowHndStr;
            mWindow->getCustomAttribute("WINDOW", &windowHnd);
            windowHndStr << windowHnd;
            pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
            mInputManager = OIS::InputManager::createInputSystem( pl );
            mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, false ));
            try {
                mJoy = static_cast<OIS::JoyStick*>(mInputManager->createInputObject( OIS::OISJoyStick, false ));
            } catch (...) {
                mJoy = 0;
            }
            
            // We'll configure the input now.
            mKeyConfig.inputDevice = 1; // Joystick ID (0: No Joystick)
            
            mKeyConfig.hasAnalog = 2;
            
            mKeyConfig.enableRumble = 0; // Let's disable it for now.
            
            //mKeyConfig.axis[0] = 0x00C8;
            //mKeyConfig.axis[1] = 0x00CD;
            //mKeyConfig.axis[2] = 0x00D0;
            //mKeyConfig.axis[3] = 0x00CB;
            // Versão dos analógicos
            mKeyConfig.axis[0] = 0x1000;
            mKeyConfig.axis[1] = 0x2000;
            mKeyConfig.axis[2] = 0x0000;
            mKeyConfig.axis[3] = 0x0000;
            
            mKeyConfig.axis[4] = 0x4000;
            mKeyConfig.axis[5] = 0x3000;
            mKeyConfig.axis[6] = 0x0000;
            mKeyConfig.axis[7] = 0x0000;
            
            mKeyConfig.buttons[0x0] = 0x00FF;
            mKeyConfig.buttons[0x1] = 0x0100;
            mKeyConfig.buttons[0x2] = 0x0200;
            mKeyConfig.buttons[0x3] = 0x0300;
            
            mKeyConfig.buttons[0x4] = 0x0400;
            mKeyConfig.buttons[0x5] = 0x0500;
            mKeyConfig.buttons[0x6] = 0x0600;
            mKeyConfig.buttons[0x7] = 0x0700;
            
            mKeyConfig.buttons[0x8] = 0x0800;
            mKeyConfig.buttons[0x9] = 0x0900;
            
            mKeyConfig.buttons[0xA] = 0x0A00;
            mKeyConfig.buttons[0xB] = 0x0B00;
            mKeyConfig.buttons[0xC] = 0x0C00;
            mKeyConfig.buttons[0xD] = 0x0D00;
            mKeyConfig.buttons[0xE] = 0x0E00;
            mKeyConfig.buttons[0xF] = 0x0F00;
            
            // Initialise the ogre pointers (get their respective singletons)
            mOverlayMan = Ogre::OverlayManager::getSingletonPtr();
            mResourceMan = Ogre::ResourceGroupManager::getSingletonPtr();
            
            mFontMan = new FontManager();
            
            mTextElementFactory = new TextElementFactory();
            mOverlayMan->addOverlayElementFactory(mTextElementFactory);
            
            // Temporary
            mResourceMan->addResourceLocation("basic_font.ctf","Zip","KERNEL",false);
            mResourceMan->loadResourceGroup("KERNEL",true,false);
            mMainFont = mFontMan->load("BaseFont.fnt","KERNEL");
            
            // Create the text overlay
            mDebugOverlay = mOverlayMan->create("DEBUG_OVERLAY");
            
            mDebugOverlayContainer = static_cast<Ogre::OverlayContainer*>
            (mOverlayMan->createOverlayElement("Panel","DEBUG_PANEL"));
            
            mDebugOverlayContainer->setDimensions(1.0, 1.0);
            mDebugOverlayContainer->setPosition(0.0f,0.0f);
            
            mDebugOverlay->add2D(mDebugOverlayContainer);
            
            
            mDebugText = mOverlayMan->createOverlayElement("Text","DEBUG_TEXT");
            static_cast<TextElement*>(mDebugText)->setFont(mMainFont);
            mDebugOverlayContainer->addChild(mDebugText);
            mDebugText->setDimensions(1.0, 1.0);
            mDebugText->setPosition(0.0f,0.0f);
            static_cast<TextElement*>(mDebugText)->setTextSize(0.05f);
            static_cast<TextElement*>(mDebugText)->setAnimSettings(false, false, false, 0.0f, 0.0f);
            static_cast<TextElement*>(mDebugText)->forceAnimReset();
            static_cast<TextElement*>(mDebugText)->setMessage("Testing Overlay Stuff");
            
            mDebugOverlay->show();
            mDebugOverlayContainer->show();
            mDebugText->show();
            
            
            
            
            
            // We're all set
            mHasInitialised = true;

            return true;
        }
        return false;
    }
    
    bool Kernel::deinitialise()
    {
        if(mHasInitialised) {
            // Deinitialise OIS
            if ( mInputManager ) {
                mInputManager->destroyInputObject( mKeyboard );
                mInputManager->destroyInputObject( mJoy );
                OIS::InputManager::destroyInputSystem(mInputManager);
                mInputManager = 0;
            }
            
            // At last, delete the Ogre::Root
            if (mRoot)
                delete mRoot;
            
            return true;
        }
        
        return false;
    }
    
    bool Kernel::run() {
        if (mHasInitialised) {
            while (!mHasEnded) {
                // Update the window (Message Pump)
                Ogre::WindowEventUtilities::messagePump();

                // Get the input first
                updateInput();

                // The Kernel must have something to execute
                assert(!mModuleList.empty());

                // Update the stack top Module
                mModuleList.top()->update(1.0f);

                mRoot->renderOneFrame();
            }

            deinitialise();
            return true;
        }
        
        return false;
    }
    
    void Kernel::updateInput() {
        // Capture the input first
        if ( mKeyboard ) mKeyboard->capture();
        if ( mJoy ) mJoy->capture();
        Ogre::String test;
        for(char i = 0;i < 16;++i) {
            bool oldState,newState = false;
            
            oldState = (mKeyStates.buttons[i] & 0x02);
            if(mKeyConfig.buttons[i] >= 0x00FF && mKeyConfig.buttons[i] <= 0x0F00)
                newState = mJoy->getJoyStickState().mButtons[(mKeyConfig.buttons[i]) >> 8];
            if(mKeyConfig.buttons[i] > 0x0000 && mKeyConfig.buttons[i] < 0x00FF)
                newState = mKeyboard->isKeyDown((OIS::KeyCode)(mKeyConfig.buttons[i]));
            
            mKeyStates.buttons[i] = (KEYSTATE)((oldState << 1) | newState);
            test += Ogre::StringConverter::toString(mKeyStates.buttons[i]);
        }

        Ogre::String analogtest[8];
        for(char i = 0;i < 8;++i) {
            if(mKeyConfig.axis[i] > 0x0FFF && mKeyConfig.axis[i] <= 0xF000) {
                mKeyStates.axis[i] = (float) mJoy->getJoyStickState().mAxes[((mKeyConfig.axis[i]) >> 12)-1].abs/32767.0f;
            } else {
                float value = 0.0f;
                
                if(mKeyConfig.axis[i] >= 0x00FF && mKeyConfig.axis[i] <= 0x0F00)
                    value = (float) mJoy->getJoyStickState().mButtons[(mKeyConfig.axis[i]) >> 8];
                if(mKeyConfig.axis[i] > 0x0000 && mKeyConfig.axis[i] < 0x00FF)
                    value = (float) mKeyboard->isKeyDown((OIS::KeyCode)(mKeyConfig.axis[i])); 
                
                switch(i) {
                    case 0x00: mKeyStates.axis[0] = -value; break;
                    case 0x01: mKeyStates.axis[1] = value; break;
                    case 0x02: mKeyStates.axis[0] = value; break;
                    case 0x03: mKeyStates.axis[1] = -value; break;
                    case 0x04: mKeyStates.axis[2] = -value; break;
                    case 0x05: mKeyStates.axis[3] = value; break;
                    case 0x06: mKeyStates.axis[2] = value; break;
                    case 0x07: mKeyStates.axis[3] = -value; break;
                    default: break;
                }
            }
           analogtest[i] += Ogre::StringConverter::toString(mKeyStates.axis[i]);
           
           
        }
        static_cast<TextElement*>(mDebugText)->setMessage("Controller Input:\r"+test+
        "\rAnalog[0]: "+analogtest[0]+
        "\rAnalog[1]: "+analogtest[1]+
        "\rAnalog[2]: "+analogtest[2]+
        "\rAnalog[3]: "+analogtest[3]+
        "\rAnalog[4]: "+analogtest[4]+
        "\rAnalog[5]: "+analogtest[5]+
        "\rAnalog[6]: "+analogtest[6]+
        "\rAnalog[7]: "+analogtest[7]);
    }
    
    void Kernel::pushModule(Module * pModule,bool haltMode) {
        assert(pModule);
        
        if(!mModuleList.empty()) {
            if(haltMode) {
                mModuleList.top()->halt();
            } else {
                delete mModuleList.top();
                mModuleList.pop();
            }
        }
        
        mModuleList.push(pModule);
        pModule->enter();
    }
    
    void Kernel::popModule() {
        assert(mModuleList.size() >= 2); // Ensure the stack wont be empty. A Kernel
                                      // must have at least one Module.
        
        mModuleList.top()->exit(); // Cleanup
        delete mModuleList.top();
        mModuleList.pop();
        
        mModuleList.top()->wakeup();
    }
}; // namespace

