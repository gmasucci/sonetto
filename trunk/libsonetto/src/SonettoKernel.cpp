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
#include "SonettoKernel.h"

namespace Sonetto {
    //-----------------------------------------------------------------------------
    // Sonetto::Kernel implementation.
    //-----------------------------------------------------------------------------
    Kernel *Kernel::mSingleton = NULL;
    //-----------------------------------------------------------------------------
    Kernel::Kernel() : mShutdown(false),
    mRoot(NULL), mWindow(NULL), mViewport(NULL), mLogMan(NULL),
    mOverlayMan(NULL), mResourceMan(NULL),mResMan(NULL)
    #ifdef _DEBUG
    , mDebugOverlay(NULL)
    #endif
    {
        KeyConfig kc[4]; // Temporary key configuration for the InputManager

        // Creates and configures Ogre root object, based on configuration
        // took from the configuration file
        mRoot = new Ogre::Root("plugins.dlc","devlab.dlc","devlab.log");
        if (mRoot->showConfigDialog()) {
            // If returned true, the user clicked OK, so initialise
            mRoot->initialise(false);
            mWindow = mRoot->createRenderWindow("Sonetto Development Laboratory",640,480,false,0);
        } else {
            delete mRoot;
            mRoot     = NULL;
            mShutdown = true;

            return;
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

        // Create Audio Manager.
        mAudioMan = new AudioManager();
        mAudioMan->initialise();
        // Create Input Manager.
        mInputMan = new InputManager();
        mInputMan->initialise(mWindow,kc);
    }

    Kernel::~Kernel()
    {
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
    }

    //-----------------------------------------------------------------------------
    bool Kernel::create()
    {
        if (isInitialised())
            SONETTO_THROW("Trying to create Kernel Singleton twice");

        mSingleton = new Kernel();
        if (mSingleton->mShutdown)
        {
            delete mSingleton;
            mSingleton = NULL;

            return false;
        }

        return true;
    }
    //-----------------------------------------------------------------------------
    bool Kernel::destroy()
    {
        if (!isInitialised())
            SONETTO_THROW("Trying to destroy unexistent Kernel Singleton");

        delete mSingleton;
        mSingleton = NULL;

        return true;
    }
    //-----------------------------------------------------------------------------
    int Kernel::run()
    {
        while (!mShutdown) {
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

        return 0;
    }
    //-----------------------------------------------------------------------------
    void Kernel::pushModule(Module *module,bool haltMode)
    {
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
    //-----------------------------------------------------------------------------
    void Kernel::popModule() {
        if (mModuleList.size() > 0) {
            mModuleList.top()->exit();   // Cleanup
            delete mModuleList.top();    // Delete the module list
            mModuleList.pop();           // Then remove it from the stack

            mModuleList.top()->wakeup(); // Now wake up the previous module
        }
    }
    //-----------------------------------------------------------------------------
    std::map<std::string,std::string> Kernel::loadConfig(const char *fname)
    {
        std::ifstream           file;
        std::map<std::string,std::string> entries;

        // Tries to open specified file
        file.open(fname,std::ios::binary);
        if (!file.is_open())
            return std::map<std::string,std::string>();

        // Reads the file line by line
        while (!file.eof())
        {
            size_t separator;
            std::string line,entry,value;

            // Reads a line
            getline(file,line);

            // Removes eventual carriedge returns inserted by Windows text editors
            while (line.find('\r') != std::string::npos)
                line.replace(line.find('\r'),1,"");

            // Skips empty lines
            if (line.length() == 0)
                continue;

            // Skips first tabs and spaces
            line = line.substr(line.find_first_not_of(" \t"));

            // Skips comment lines
            if (line[0] == '#')
                continue;

            // Each entry line should follow the syntax: Entry=Value
            separator = line.find("=");
            if (count(line.begin(),line.end(),'=') != 1)
                return std::map<std::string,std::string>();

            entry = line.substr(0,separator);
            value = line.substr(separator+1);

            // Aborts if the entry already exists
            if (entries.find(entry) != entries.end())
                return std::map<std::string,std::string>();

            // Adds to entry map
            entries[entry] = value;
        }

        file.close();
        return entries;
    }
    //-----------------------------------------------------------------------------
    Ogre::Viewport * Kernel::getViewport()
    {
        return mViewport;
    }
    //-----------------------------------------------------------------------------
    void Kernel::setViewport(Ogre::Viewport * viewport)
    {
        mViewport = viewport;
    }
    //-----------------------------------------------------------------------------
    Ogre::RenderWindow * Kernel::getRenderWindow()
    {
        return mWindow;
    }
    //-----------------------------------------------------------------------------
    Ogre::Root * Kernel::getOgreRoot()
    {
        return mRoot;
    }
    //-----------------------------------------------------------------------------
    InputManager * Kernel::getInputMan()
    {
        return mInputMan;
    }
    //-----------------------------------------------------------------------------
    AudioManager * Kernel::getAudioMan()
    {
        return mAudioMan;
    }
    //-----------------------------------------------------------------------------
}; // namespace Sonetto
