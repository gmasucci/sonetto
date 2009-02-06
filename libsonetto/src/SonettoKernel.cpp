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

#include <cstdlib>
#ifndef WINDOWS
#   include <sys/stat.h>
#   include <dirent.h>
#endif
#include "SonettoKernel.h"
#include "SonettoDatabase.h"
#include "SonettoInputManager.h"
#include "SonettoScriptManager.h"
#include "SonettoStaticTextElement.h"

namespace Sonetto
{
    // ----------------------------------------------------------------------
    // Sonetto::Kernel implementation
    // ----------------------------------------------------------------------
    SONETTO_SINGLETON_IMPLEMENT(Kernel);
    // ----------------------------------------------------------------------
    void Kernel::initialize()
    {
        if (mInitialized)
        {
            SONETTO_THROW("Kernel was already initialized");
        }

        Ogre::NameValuePairList wndParamList;
        SDL_SysWMinfo wmInfo;
        const char *defaultCfgName = "defaultcfg.dat";
        mScreenWidth = DEFAULT_SCREEN_WIDTH;
        mScreenHeight = DEFAULT_SCREEN_HEIGHT;

        // Checks if wasn't initialized yet
        if (mInitialized)
        {
            SONETTO_THROW("Kernel is already initialized");
        }

        // Reads Sonetto Project File
        readSPF();

        // ------------------
        // SDL Initialisation
        // ------------------
        // Initializes SDL video and joystick subsystems
        if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK) == -1)
        {
            SONETTO_THROW("Could not initialize SDL");
        }

        // Creates SDL rendering window with default resolutions
        // (resize later to the desired resolution)
        mWindow = SDL_SetVideoMode(mScreenWidth,mScreenHeight,32,0);
        if (!mWindow)
        {
            SONETTO_THROW("Could not create SDL window");
        }

        // Disable cursor, Sonetto does not support mouse for now.
        SDL_ShowCursor(SDL_DISABLE);
        SDL_WM_SetCaption("Now Loading...","Now Loading...");

        /*if (mLoadingImg.size() > 0)
        {*/
            SDL_Surface *loading;
            SDL_Rect src,dest;

            // Fills the screen with the background colour
            // taken from the SPF file
            SDL_FillRect(mWindow,NULL,SDL_MapRGB(mWindow->format,
                    mLoadingBGR,mLoadingBGG,mLoadingBGB));

            // Loads loading image and checks for errors
            loading = SDL_LoadBMP(mLoadingImg.c_str());
            if (!loading)
            {
                SONETTO_THROW("Unable to find " + mLoadingImg);
            }

            // Source rectangle: Full image
            src.x = src.y = 0;
            src.w = loading->w;
            src.h = loading->h;

            // Destination rectangle: Position took from SPF file
            dest.x = mLoadingImgLeft;
            dest.y = mLoadingImgTop;

            // Blits loading image into screen buffer and flips it
            SDL_BlitSurface(loading,&src,mWindow,&dest);
            SDL_Flip(mWindow);

            // Frees loaded loading image
            //SDL_BlitSurface(loading,&src,mWindow,&dest);
            //SDL_FreeSurface(loading);
        /*}*/

        // Get window info to attach Ogre at it
        SDL_VERSION(&wmInfo.version);
        SDL_GetWMInfo(&wmInfo);

        #ifdef WINDOWS
        {
            // Get the User Application Data directory.
            mGameData = getenv("APPDATA");
            // Append the Sonetto Directory to the end of it;
            mGameData += "\\Sonetto\\";
            // Verify the existence of the directory
            if((_access(mGameData.c_str(),0)) == -1)
            {
                // If it does not exist, create it
                mkdir(mGameData.c_str());
            }
            mGameDataPath = mGameData + mGameIdentifier + "\\";
            if((_access(mGameDataPath.c_str(),0)) == -1)
            {
                // If it does not exist, create it
                mkdir(mGameDataPath.c_str());
            }
            // Check if the configuration file for this project exists.
            std::string configfile = mGameDataPath + mGameIdentifier + ".INI";
            if((_access(configfile.c_str(),0)) == -1)
            {
                if(_access(defaultCfgName,0) == -1)
                {
                    SONETTO_THROW("Missing default configuration file");
                }

                // If not, then copy the default config file from the
                // game directory
                if(!CopyFile(defaultCfgName,configfile.c_str(),true))
                {
                    SONETTO_THROW("Unable to copy configuration file");
                }
            }
        }
        #else
        {
            DIR *dir;          // Used to check directory existence
            struct stat fstat; // Used to check file existence

            // Gets user home directory
            mGameData = std::string("/home/") + getenv("PWD");

            // Appends Sonetto directory to the end of it
            mGameData += "/.sonetto/";

            // Verifies existence of directory
            dir = opendir(mGameData.c_str(),0);
            if (!dir) {
                // If it does not exist, creates it
                mkdir(mGameData.c_str(),S_IRWXU);
            } else {
                // If openned successfuly, closes it
                closedir(dir);
            }

            // Creates path for the actual game data folder
            mGameDataPath = mGameData + mGameIdentifier + "/";

            // Verifies existence of directory
            dir = opendir(mGameDataPath.c_str(),0);
            if (!dir) {
                // If it does not exist, creates it
                mkdir(mGameDataPath.c_str(),S_IRWXU);
            } else {
                // If openned successfuly, closes it
                closedir(dir);
            }

            // Check if the configuration file for this project exists.
            std::string configfile = mGameDataPath + mGameIdentifier + ".INI";
            if (stat(configfile.c_str(),&fstat) < 0)
            {
                ifstream src;
                ofstream dest;

                // Opens default configuration file
                src.open(defaultCfgName);
                if (!src.is_open())
                {
                    SONETTO_THROW("Missing default configuration file");
                }

                // Opens local user configuration file
                dest.open(configfile.c_str());
                if (!dest.is_open())
                {
                    SONETTO_THROW("Unable to copy configuration file");
                }

                // Copies contents from default to local user config file
                dest << src.rdbuf();

                // Closes handles
                src.close();
                dest.close();
            }
        }
        #endif

        // -------------------
        // Ogre Initialisation
        // -------------------
        // Produce logs only on debug compilations
#ifdef DEBUG
        mOgre = new Ogre::Root("","",mGameDataPath+"game.log");
#else
        mOgre = new Ogre::Root("","","");
#endif

        // Flips loading screen (temporary)
        SDL_Flip(mWindow);

#ifdef WINDOWS
        wndParamList["externalWindowHandle"] =
                Ogre::StringConverter::toString((uint32)wmInfo.window);
#else
        #error External window handle is not yet implemented on non Windows \
               applications.
#endif

        // Load and configure Sonetto
        loadConfig(mGameDataPath+mGameIdentifier+".INI",wndParamList);

        int sdlwindowcfg = 0;

        // Resets video mode to loaded configurations
        if(mIsFullScreen)
        {
            sdlwindowcfg = SDL_FULLSCREEN;
        } else {
            sdlwindowcfg = 0;
        }
        mWindow = SDL_SetVideoMode(640,480, 32,sdlwindowcfg);

        SDL_FillRect(mWindow,NULL,SDL_MapRGB(mWindow->format,
                    mLoadingBGR,mLoadingBGG,mLoadingBGB));
        SDL_BlitSurface(loading,&src,mWindow,&dest);
        SDL_FreeSurface(loading);
        SDL_Flip(mWindow);

        // Initialize Ogre Root
        mOgre->initialise(false);

        // Get ogre managers and copy them to pointers for easy access.
        mOverlayMan  = Ogre::OverlayManager::getSingletonPtr();

        // Flips loading screen (temporary)
        SDL_Flip(mWindow);

        mScriptMan = new ScriptManager();

        mDatabase = new Database();
        mDatabase->initialize();

        // Initializes input manager
        mInputMan = new InputManager(4);
        mInputMan->initialize();
        mFontMan = new FontManager();

        // Initialize Objects/Elements;
        StaticTextElementFactory * mTextElementFactory = new StaticTextElementFactory();
        mOverlayMan->addOverlayElementFactory(mTextElementFactory);

        // Resets video mode to loaded configurations
        mWindow = SDL_SetVideoMode(mScreenWidth,mScreenHeight,
                Ogre::StringConverter::parseUnsignedInt(
                wndParamList["colourDepth"]),sdlwindowcfg);

        // Create the Ogre Render Window
        mRenderWindow = mOgre->createRenderWindow("",mScreenWidth,
                mScreenHeight,mIsFullScreen,&wndParamList);

        // Resets caption to real game window caption
        SDL_WM_SetCaption(mGameTitle.c_str(),mGameTitle.c_str());

        // Creates a Boot Module and activates it
        pushModule(Module::MT_BOOT,MA_CHANGE);

        // Flags we have initialized
        mInitialized = true;
    }
    // ----------------------------------------------------------------------
    Kernel::~Kernel()
    {
        // Deinitialize if initialized
        if (mInitialized)
        {
            // Deinitializes and deletes instantiated modules
            while (!mModuleStack.empty())
            {
                Module *module = mModuleStack.top();

                // Deinitializes, deletes and removes from stack
                module->deinitialize();
                delete mModuleStack.top();
                mModuleStack.pop();
            }

            // Deletes input manager
            delete mInputMan;

            delete mDatabase;

            // Remove and delete all Sonetto Resources.
            delete mFontMan;
            delete mScriptMan;

            // Deletes Ogre
            delete mOgre;

            // Deinitialize SDL
            SDL_Quit();
        }
    }
    // ----------------------------------------------------------------------
    void Kernel::run()
    {
        bool running = true;

        while (running)
        {
            SDL_Event evt;

            // Pump events
            while (SDL_PollEvent(&evt))
            {
                if (evt.type == SDL_QUIT)
                {
                    // Shutdowns the game when asked to
                    mKernelAction = KA_SHUTDOWN;
                }
            }

            // Stops game while window is deactivated (minimised)
            if (!(SDL_GetAppState() & SDL_APPACTIVE))
            {
                // Loop until the window gets activated again
                while (SDL_WaitEvent(&evt))
                {
                    if (SDL_GetAppState() & SDL_APPACTIVE)
                    {
                        // Window activated; break the loop and continue the game
                        break;
                    }
                }
            }

            switch (mKernelAction)
            {
                case KA_CHANGE_MODULE:
                    if (mModuleAction == MA_RETURN) {
                        // Pops current module, returning to the previous one
                        popModule();
                    } else {
                        // Pushes desired module with the desired action
                        // (if action is MA_CHANGE, pushModule() will remove
                        // the current one by itself)
                        pushModule(mNextModuleType,mModuleAction);
                    }

                    // Resets action parameters
                    mKernelAction   = KA_NONE;
                    mModuleAction   = MA_NONE;
                    mNextModuleType = Module::MT_NONE;
                break;

                case KA_SHUTDOWN:
                    // Stops running
                    running = false;
                break;

                default: break;
            }

            // Updates input manager
            mInputMan->update();

            // Checks whether the stack is empty
            if (mModuleStack.empty())
            {
                SONETTO_THROW("The module stack is empty");
            }

            // Updates active module
            mModuleStack.top()->update();

            // Renders one frame
            mOgre->renderOneFrame();
        }
    }
    // ----------------------------------------------------------------------
    void Kernel::setAction(KernelAction kact,ModuleAction mact,
            Module::ModuleType modtype)
    {
        switch (kact)
        {
            case KA_NONE:
                // Makes sure parameters are valid
                assert(mact == MA_NONE && modtype == Module::MT_NONE);

                // Sets action parameters
                mKernelAction   = KA_NONE;
                mModuleAction   = MA_NONE;
                mNextModuleType = Module::MT_NONE;
            break;

            case KA_CHANGE_MODULE:
                // Makes sure parameters are valid
                assert(mact != MA_NONE && modtype != Module::MT_NONE);

                // Sets action parameters
                mKernelAction   = KA_CHANGE_MODULE;
                mModuleAction   = mact;
                mNextModuleType = modtype;
            break;

            case KA_SHUTDOWN:
                // Makes sure parameters are valid
                assert(mact == MA_NONE && modtype == Module::MT_NONE);

                // Sets action parameters
                mKernelAction   = KA_SHUTDOWN;
                mModuleAction   = MA_NONE;
                mNextModuleType = Module::MT_NONE;
            break;
        }
    }
    // ----------------------------------------------------------------------
    Ogre::RenderWindow * Kernel::getRenderWindow()
    {
        return mRenderWindow;
    }
    // ----------------------------------------------------------------------
    void Kernel::loadConfig(const std::string &fname,
            Ogre::NameValuePairList &wndParamList)
    {
        // Checks for config file existence
        {
            const std::string cannotLoadMsg = "Unable to load " + fname +
                    ",\nthe file does not exist";

        #ifdef WINDOWS
            if((_access(fname.c_str(),0)) == -1)
            {
                SONETTO_THROW(cannotLoadMsg);
            }
        #else
            struct stat fstat;
            if (stat(fname.c_str(),&fstat) < 0)
            {
                SONETTO_THROW(cannotLoadMsg);
            }
        #endif
        }

        Ogre::ConfigFile        config;
        Ogre::RenderSystemList *renderers;

        // Loads file into Ogre::ConfigFile
        config.load(fname);

        // Video configuration section name
        const char *videoSectName = "video";

        // Loads the desired render system plugin
        mOgre->loadPlugin(config.getSetting("renderSystem",
                videoSectName));

        // Gets list of loaded render systems and makes sure it's not empty
        renderers = mOgre->getAvailableRenderers();
        assert(!renderers->empty());

        // Sets the render system we've just loaded as active
        mOgre->setRenderSystem(renderers->back());

        // Gets resolution config string
        std::string resolution = config.getSetting("screenResolution",
                videoSectName);

        // Explodes resolution string on the "x" and validates it
        std::vector<Ogre::String> res = Ogre::StringUtil::split(resolution,"x",1);
        if (res.size() != 2)
        {
            SONETTO_THROW("Invalid screen resolution set in configuration "
                          "file");
        }

        // Parses width and height took from resolution
        // string as integers
        mScreenWidth = Ogre::StringConverter::parseUnsignedInt(res[0]);
        mScreenHeight = Ogre::StringConverter::parseUnsignedInt(res[1]);

        // Gets fullscreen config string
        Ogre::String fullscreen = config.getSetting("fullScreen",
                videoSectName);

        // Checks its value
        if(fullscreen == "true")
        {
            mIsFullScreen = true;

        } else
        if(fullscreen == "false")
        {
            mIsFullScreen = false;
        } else {
            SONETTO_THROW("Invalid fullscreen value set in configuration "
                          "file");
        }

        // Gets other config strings and set them as
        // window parameters accordingly
        wndParamList["vsync"] = config.getSetting("vsync",videoSectName);
        wndParamList["displayFrequency"] = config.
                getSetting("displayFrequency",videoSectName);
        wndParamList["colourDepth"] = config.
                getSetting("colourDepth",videoSectName);
        wndParamList["FSAA"] = config.getSetting("FSAA",videoSectName);
    }
    // ----------------------------------------------------------------------
    void Kernel::pushModule(Module::ModuleType modtype,ModuleAction mact)
    {
        Module *newmod,*curmod = NULL;

        // Makes sure parameters are valid
        assert(modtype != Module::MT_NONE && mact != MA_NONE &&
                mact != MA_RETURN);

        // Instantiates new module
        newmod = mModuleFactory->createModule(modtype);

        // Gets current active module (if any)
        if (!mModuleStack.empty())
        {
            curmod = mModuleStack.top();

            if (mact == MA_CHANGE) {
                // Deinitializes, deletes and removes the
                // current module from the stack
                curmod->deinitialize();
                delete curmod;
                mModuleStack.pop();
            } else {
                // Halts current module
                curmod->halt();
            }
        }

        // Pushes new module into stack and initializes it
        mModuleStack.push(newmod);
        newmod->initialize();
    }
    // ----------------------------------------------------------------------
    void Kernel::popModule()
    {
        Module *module;

        // Makes sure this won't leave the stack empty
        if (mModuleStack.size() < 2)
        {
            SONETTO_THROW("Cannot empty module stack");
        }

        // Gets current active module, deinitializes and deletes it
        module = mModuleStack.top();
        module->deinitialize();
        delete module;

        // Gets new top module and resume its execution
        mModuleStack.top()->resume();
    }
    // ----------------------------------------------------------------------
    void Kernel::readSPF()
    {
        uint32 spffourcc = MKFOURCC('S','P','F','0');
        uint32 filefourcc = 0;

        // Opens SPF file
        std::ifstream gamespf;
        gamespf.open("game.spf", std::ios_base::in | std::ios_base::binary);

        // Checks if it was opened successfuly
        if (!gamespf.is_open())
        {
            SONETTO_THROW("Unable to find game.spf or the file is in "
                          "use by another proccess");
        }

        // Reads and checks file's fourcc identification
        gamespf.read((char*)&filefourcc,sizeof(filefourcc));
        if(filefourcc != spffourcc)
        {
            SONETTO_THROW("Invalid SPF file");
        }

        // Skips the last modification timestamp
        gamespf.seekg(4,std::ios_base::cur);

        // Reads information from file
        mGameTitle = readString(gamespf);
        mGameIdentifier = readString(gamespf);
        mGameAuthor = readString(gamespf);
        mLoadingImg = readString(gamespf);
        gamespf.read((char *)&mLoadingImgLeft,sizeof(mLoadingImgLeft));
        gamespf.read((char *)&mLoadingImgTop,sizeof(mLoadingImgTop));
        gamespf.read((char *)&mLoadingBGR,sizeof(mLoadingBGR));
        gamespf.read((char *)&mLoadingBGG,sizeof(mLoadingBGG));
        gamespf.read((char *)&mLoadingBGB,sizeof(mLoadingBGB));

        // Closes file handle
        gamespf.close();
    }
    // ----------------------------------------------------------------------
    std::string Kernel::readString(std::ifstream &stream)
    {
        uint16 strsize;
        stream.read((char*)&strsize, sizeof(strsize));
        char * stringbuffer = new char[strsize+1];
        stream.read((char*)stringbuffer, strsize);
        stringbuffer[strsize] = '\0';
        std::string rstring = stringbuffer;
        delete[] stringbuffer;
        return rstring;
    }
} // namespace
