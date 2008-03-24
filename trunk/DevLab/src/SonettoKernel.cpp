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

#include <cstdio>
#include <Ogre.h>
#include "SonettoException.h"
#include "SonettoKernel.h"
#include "SonettoWindowSkinSerializer.h"

using namespace std;

namespace Sonetto {
    Kernel *Kernel::mSingleton = NULL;

    Kernel::Kernel() {
        mShutDown = false;

        mRoot        = NULL;
        mWindow      = NULL;
        mViewport    = NULL;
        mLogMan      = NULL;
        mOverlayMan  = NULL;
        mResourceMan = NULL;
        mResMan      = NULL;

        mAudioMan           = NULL;
        mInputMan           = NULL;
        mFontMan            = NULL;
        mTextElementFactory = NULL;
        mWindowFactory      = NULL;
        mSWindowFactory     = NULL;
        mTWindowFactory     = NULL;
        mWSkinMan           = NULL;

        mDebugOverlay          = NULL;
        mDebugOverlayContainer = NULL;
        mDebugText             = NULL;
        mDebugErrorText        = NULL;
        mTmpWinSkinLoader      = NULL;
    }

    bool Kernel::initialise() {
        if (isInitialised()) {
            cerr << "(Kernel::initialise()) Kernel was asked to be "
                    "initialised twice. This will probably cause errors.\n";
        }

        mSingleton = new Kernel();
        return mSingleton->_initialise();
    }

    bool Kernel::deinitialise() {
        if (!isInitialised()) {
            cerr << "(Kernel::deinitialise()) Kernel was asked to be "
                    "deinitialised before its initialisation. This will probably "
                    "cause errors.\n";
        }

        if (mSingleton->_deinitialise()) {
            delete mSingleton;
            mSingleton = NULL;

            return true;
        }

        return false;
    }

    bool Kernel::_initialise() {
        KeyConfig kc[4]; // Temporary key configuration for the InputManager

        mRoot = new Ogre::Root("plugins.dlc","devlab.dlc","devlab.log");
        if (mRoot->showConfigDialog()) {
            // If returned true, user clicked OK so initialise
            // Here we choose to let the system create a default rendering window by passing 'true'
            mWindow = mRoot->initialise(true,"Sonetto Development Laboratory");
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
        mWSkinMan           = new WindowSkinManager();

        mAudioMan->initialise();
        mInputMan->initialise(mWindow,kc);

        mTextElementFactory = new TextElementFactory();
        mOverlayMan->addOverlayElementFactory(mTextElementFactory);

        mWindowFactory = new Sonetto::WindowFactory();
        mOverlayMan->addOverlayElementFactory(mWindowFactory);

        mSWindowFactory = new Sonetto::SlimWindowFactory();
        mOverlayMan->addOverlayElementFactory(mSWindowFactory);

        mTWindowFactory = new Sonetto::TailedWindowFactory();
        mOverlayMan->addOverlayElementFactory(mTWindowFactory);

        // Temporary resource loading (debugging purposes)
        mResourceMan->addResourceLocation("basic_font.ctf","Zip","KERNEL",false);
        mResourceMan->loadResourceGroup("KERNEL",true,false);
        mMainFont = mFontMan->load("BaseFont.fnt","KERNEL");

        FILE *fp = fopen("sound/sound000.ogg","rb");
        fseek(fp,0,SEEK_END);
        long size = ftell(fp);
        char *data = new char[size];
        rewind(fp);
        printf("%d bytes read\n",fread(data,1,size,fp));
        fclose(fp);
        mAudioMan->addSound(new VirtualFile(data,size),0.0f,1.0f,1.0f,1.0f);

        mAudioMan->addMusic("music001.ogg",true);

        // Load the windowskin manually

        mTmpWinSkinLoader = new TempWindowSkinLoader();
        mResourceMan->createResourceGroup("KERNEL2");
        mResourceMan->addResourceLocation("windowskin/", "FileSystem","KERNEL2");
        //mResourceMan->declareResource("WndSkin", "WindowSkin", "KERNEL2", mTmpWinSkinLoader);
        mResourceMan->declareResource("windowskin.bin", "WindowSkin", "KERNEL2");
        mResourceMan->initialiseResourceGroup("KERNEL2");
        //mWSkinPtr = static_cast<WindowSkinPtr>(mResMan->create("WndSkn", "KERNEL", true, mTmpWinSkinLoader));
        mWSkinPtr = mWSkinMan->load("windowskin.bin", "KERNEL2");

        WindowSkinSerializer tmpWSkinSerializer;
        tmpWSkinSerializer.exportWindowSkin(static_cast<WindowSkin *>(mWSkinPtr.getPointer()), "windowskin.bin");

        // Create the text overlay
        mDebugOverlay = mOverlayMan->create("DEBUG_OVERLAY");
        mDebugOverlayContainer = static_cast<Ogre::OverlayContainer *>(mOverlayMan->createOverlayElement("Window","DEBUG_PANEL"));

        mDebugOverlayContainer->setDimensions(1.0f,1.0f);
        mDebugOverlayContainer->setPosition(0.0f,0.0f);

        static_cast<Window*>(mDebugOverlayContainer)->setWindowType(WT_ENEMY);
        static_cast<Window*>(mDebugOverlayContainer)->setWindowSkin(mWSkinPtr);

        mDebugOverlay->add2D(mDebugOverlayContainer);

        mDebugText = static_cast<TextElement *>(mOverlayMan->createOverlayElement("Text","DEBUG_TEXT"));
        mDebugOverlayContainer->addChild(mDebugText);

        mDebugText->setFont(mMainFont);
        mDebugText->setMessage("Testing Sonetto::AudioManager !");
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
            mDebugErrorText->setMessage("No controller detected");
            mDebugErrorText->setDimensions(1.0f,1.0f);
            mDebugErrorText->setPosition(0.1f,0.45f);
            mDebugErrorText->setTextSize(0.10f);
            mDebugErrorText->setAnimSettings(true,true,false,10.0f,8.0f);
            mDebugErrorText->forceAnimReset();

            // Fire
            mDebugErrorText->show();
        }

        return true;
    }

    bool Kernel::_deinitialise() {
        // Deinitialise InputManager
        if (mInputMan) {
            mInputMan->deinitialise();

            delete mInputMan;
            mInputMan = NULL;
        }

        // Deinitialise AudioManager
        if (mAudioMan) {
            mAudioMan->deinitialise();

            delete mAudioMan;
            mAudioMan = NULL;
        }

        // At last, delete the Ogre::Root
        if (mRoot) {
            delete mRoot;
            mRoot = NULL;
        }

        return true;
    }

    int Kernel::run() {
        while (!mShutDown) {
            // Small error check
            if (mModuleList.size() == 0)
                SONETTO_THROW("Module stack is empty");

            // Update game window (Message Pump)
            Ogre::WindowEventUtilities::messagePump();

            // First update input
            mInputMan->updateInput();

            // Update stack's top Module
            mModuleList.top()->update(1.0f);

            // Update audio manager
            if (mAudioMan)
                mAudioMan->update();

            // Render !
            mRoot->renderOneFrame();
        }

        // Remove everything from the memory and destroy singleton
        Kernel::deinitialise();
        return 0;
    }

    void Kernel::pushModule(Module *module,bool haltMode) {
        if (!mModuleList.empty()) {
            if (haltMode) {
                mModuleList.top()->halt(); // Put the current module to sleep
            } else {
                delete mModuleList.top();  // If it's going to change for real,
                                           // delete the current one.
                mModuleList.pop();         // Then remove it from the stack
            }
        }

        mModuleList.push(module);   // Insert the new module on the stack's top
        module->enter();            // Enter the new module
    }

    void Kernel::popModule() {
        if (mModuleList.size() > 0) {
            mModuleList.top()->exit();   // Cleanup
            delete mModuleList.top();    // Delete the module list
            mModuleList.pop();           // Then remove it from the stack

            mModuleList.top()->wakeup(); // Now wake up the previous module
        }
    }
} // namespace
