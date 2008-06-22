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

#include <SDL/SDL.h>
#include <SDL/SDL_syswm.h>
#include "SonettoKernel.h"

using namespace std;
using namespace Ogre;

namespace Sonetto {
    //-----------------------------------------------------------------------------
    // Sonetto::Kernel implementation.
    //-----------------------------------------------------------------------------
    Kernel *Kernel::mSingleton = NULL;
    //-----------------------------------------------------------------------------
    Kernel::Kernel() : mShutdown(false), mScreen(NULL),
    mRoot(NULL), mWindow(NULL), mViewport(NULL), mLogMan(NULL),
    mOverlayMan(NULL), mResourceMan(NULL),mResMan(NULL)
    #ifdef _DEBUG
    , mDebugOverlay(NULL)
    #endif
    {
        NameValuePairList  wndParamList; // Needed for Ogre to use SDL rendering window
        SDL_SysWMinfo      wmInfo;       // Structure holding SDL window information

        // Initialises SDL video and joystick subsystems
        if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK) == -1)
        {
            SONETTO_THROW("Could not initialise SDL");
        }

        // Create SDL rendering window
        // <todo> Load resolution from configuration file
        mScreen = SDL_SetVideoMode(640,480,0,0);
        if (!mScreen)
        {
            SONETTO_THROW("Could not create SDL window");
        }

        // RPGs don't use mouse, bwagahgah
        SDL_ShowCursor(SDL_DISABLE);
        SDL_WM_SetCaption("Sonetto Game Runtime","Sonetto Game Runtime");

        // <todo> Add some loading imagery here
        SDL_Flip(mScreen);

        // Get window info to attach Ogre at it
        SDL_VERSION(&wmInfo.version);
        SDL_GetWMInfo(&wmInfo);

        #ifdef _WINDOWS
            // Under Windows we just need to pass the window handle(HWND) as a string to
            // Ogre::Root::createRenderWindow() for Ogre to use such window for its drawings
            wndParamList["externalWindowHandle"] = StringConverter::toString((unsigned long)wmInfo.window);
        #else
            // Under *nixes, Ogre uses GLX for rendering. The syntax of
            // externalWindowHandle in this case is a bit different, but it
            // does essentially the same thing.
            string         wndHandleStr;
            string         dsp(&(DisplayString(info.info.x11.display)[1]));
            vector<String> tokens = StringUtil::split(dsp,".");

            wndHandleStr = StringConverter::toString((long)info.info.x11.display)+
                        ":"+tokens[1]+":"+
                        StringConverter::toString((long)info.info.x11.window);

            wndParamList["externalWindowHandle"] = wndHandleStr;
        #endif

        // Creates and configures Ogre root object, based on configuration
        // took from the configuration file
        mRoot = new Root("plugins.dlc","devlab.dlc","devlab.log");
        if (mRoot->showConfigDialog()) {
            // If returned true, the user clicked OK, so initialise
            mRoot->initialise(false);
            mWindow = mRoot->createRenderWindow("",640,480,false,&wndParamList);
        } else {
            delete mRoot;
            mRoot     = NULL;
            mShutdown = true;

            return;
        }

        // Get Ogre singletons for easier use
        mLogMan      = LogManager::getSingletonPtr();
        mOverlayMan  = OverlayManager::getSingletonPtr();
        mResourceMan = ResourceGroupManager::getSingletonPtr();

        // Create Audio Manager
        mAudioMan = new AudioManager();
        mAudioMan->initialise();

        // Create Input Manager
        mInputMan = new InputManager(4,mWindow);
    }

    Kernel::~Kernel()
    {
        // Deinitialise InputManager
        if (mInputMan) {
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

        SDL_Quit();
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
            SDL_Event evt; // Check for quit events

            // Small error check
            if (mModuleList.size() == 0)
                SONETTO_THROW("Module stack is empty");

            // Update game window (Message Pump)
            WindowEventUtilities::messagePump();

            // First update input
            mInputMan->update();

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
    Viewport *Kernel::getViewport()
    {
        return mViewport;
    }
    //-----------------------------------------------------------------------------
    void Kernel::setViewport(Viewport *viewport)
    {
        mViewport = viewport;
    }
    //-----------------------------------------------------------------------------
    RenderWindow *Kernel::getRenderWindow()
    {
        return mWindow;
    }
    //-----------------------------------------------------------------------------
    Root *Kernel::getOgreRoot()
    {
        return mRoot;
    }
    //-----------------------------------------------------------------------------
    InputManager *Kernel::getInputMan()
    {
        return mInputMan;
    }
    //-----------------------------------------------------------------------------
    AudioManager *Kernel::getAudioMan()
    {
        return mAudioMan;
    }
    //-----------------------------------------------------------------------------
}; // namespace Sonetto
