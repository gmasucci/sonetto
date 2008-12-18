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

namespace Sonetto
{
    // ----------------------------------------------------------------------
    // Sonetto::Kernel implementation
    // ----------------------------------------------------------------------
    SONETTO_SINGLETON_IMPLEMENT(Kernel);
    // ----------------------------------------------------------------------
    void Kernel::initialise()
    {
        const char *defaultCfgName = "defaultcfg.dat";

        // Reads Sonetto Project File
        readSPF();

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

        Ogre::NameValuePairList wndParamList; // Needed for Ogre to use SDL rendering window
        SDL_SysWMinfo wmInfo;                 // Structure holding SDL window information

        // Checks if wasn't initialised yet
        if (mInitialised)
        {
            SONETTO_THROW("Kernel is already initialised");
        }

        // ------------------
        // SDL Initialisation
        // ------------------
        // Initialises SDL video and joystick subsystems
        if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK) == -1)
        {
            SONETTO_THROW("Could not initialise SDL");
        }

        // Creates SDL rendering window with default resolutions
        // (resize later to the desired resolution)
        mWindow = SDL_SetVideoMode(mScreenWidth,mScreenHeight,0,0);
        if (!mWindow)
        {
            SONETTO_THROW("Could not create SDL window");
        }

        // Disable cursor, Sonetto does not support mouse for now.
        SDL_ShowCursor(SDL_DISABLE);
        SDL_WM_SetCaption("Now Loading...","Now Loading...");

        SDL_Flip(mWindow);

        // Get window info to attach Ogre at it
        SDL_VERSION(&wmInfo.version);
        SDL_GetWMInfo(&wmInfo);

        // -------------------
        // Ogre Initialisation
        // -------------------
        // Produce logs only on debug compilations
#ifdef DEBUG
        mOgre = new Ogre::Root("","",mGameDataPath+"game.log");
#else
        mOgre = new Ogre::Root("","","");
#endif

#ifdef WINDOWS
        wndParamList["externalWindowHandle"] =
                Ogre::StringConverter::toString((uint32)wmInfo.window);
#else
        #error External window handle is not yet implemented on non Windows \
               applications.
#endif

        // Load and configure Sonetto
        loadConfig(mGameDataPath+mGameIdentifier+".INI",wndParamList);

        // Initialise Ogre Root
        mOgre->initialise(false);

        // Create the Ogre Render Window
        mRenderWindow = mOgre->createRenderWindow("",mScreenWidth,mScreenHeight,mIsFullScreen,&wndParamList);

        // Resets caption to real game window caption
        SDL_WM_SetCaption(mGameTitle.c_str(),mGameTitle.c_str());

        // Flags we have initialised
        mInitialised = true;
    }
    // ----------------------------------------------------------------------
    Kernel::~Kernel()
    {
        // Deinitialise if initialised
        if (mInitialised)
        {
            // Deletes Ogre
            delete mOgre;

            // Deinitialise SDL
            SDL_Quit();
        }
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

        // Resets video mode to loaded configurations
        mWindow = SDL_SetVideoMode(mScreenWidth,mScreenHeight,
                Ogre::StringConverter::parseUnsignedInt(config.
                getSetting("colourDepth",videoSectName)),0);

        // Sets the render system we've just loaded above as active
        // <todo> Does this really need to be done after
        // SDL_SetVideoMode()'ing?
        mOgre->setRenderSystem(renderers->back());
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

        // Skips four bytes (<todo> What are those?)
        gamespf.seekg(4,std::ios_base::cur);

        // Reads information from file
        mGameTitle = readString(gamespf);
        mGameIdentifier = readString(gamespf);
        mGameAuthor = readString(gamespf);

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
    // ----------------------------------------------------------------------
    void Kernel::run() {}
} // namespace
