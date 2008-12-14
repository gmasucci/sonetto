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
#ifdef WINDOWS
#   include <windows.h>
#else
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
                // If not, then copy the default config file from the game directory.
                CopyFile("defaultcfg.dat",configfile.c_str(),true);
            }
        }
        #else
        {
            DIR *dir;          // Used to check directory existence
            struct stat fstat; // Used to check file existence

            // Gets user home directory
            mGameData = std::string("/home/") + getenv("PWD");

            // Appends Sonetto directory to the end of it
            mGameData += "/Sonetto/";

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
                src.open("defaultcfg.dat");
                if (!src.is_open())
                {
                    SONETTO_THROW("Missing default configuration file");
                }

                // Opens local user configuration file
                dest.open(configfile.c_str());
                if (!dest.is_open())
                {
                    SONETTO_THROW("Could not open configuration file for writing");
                }

                // Copies contents from default to local user config file
                dest << src.rdbuf();

                // Closes handles
                src.close();
                dest.close();
            }
        }
        #endif

        Ogre::NameValuePairList  wndParamList; // Needed for Ogre to use SDL rendering window
        SDL_SysWMinfo      wmInfo;       // Structure holding SDL window information

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

        // Create SDL rendering window (resize later to the desired resolution)
        mWindow = SDL_SetVideoMode(640,480,0,0);
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
        wndParamList["externalWindowHandle"] = Ogre::StringConverter::toString((unsigned long)wmInfo.window);
#else
        #error External window handle is not yet implemented on non Win32 Applications.
#endif

        // Load and configure sonetto.
        // <todo> Store and load Sonetto Configuration from User directory.
        loadConfig(mGameDataPath+mGameIdentifier+".INI",wndParamList);

        // Initialise Ogre Root
        mOgre->initialise(false);
        // Create the Ogre Render Window.
        mRenderWindow = mOgre->createRenderWindow("", mScreenWidth, mScreenHeight, mIsFullScreen,&wndParamList);

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
    bool Kernel::loadConfig(const Ogre::String& fname,Ogre::NameValuePairList &wndParamList)
    {
        #ifdef WINDOWS
        if((_access(fname.c_str(),0)) == -1)
            SONETTO_THROW("Unable to load "+fname+",\nthe file does not exist");
        #endif
        Ogre::ConfigFile config;
        //config.loadDirect(fname, "\t:=", false);
        config.load(fname);

        // Video Configuration Section
        Ogre::String videoSectName = "video";

        mOgre->loadPlugin(config.getSetting("renderSystemPlugin",videoSectName));
        Ogre::RenderSystem* rs = mOgre->getRenderSystemByName(config.getSetting("renderSystem",videoSectName));

        Ogre::String resolution = config.getSetting("screenResolution",videoSectName);

        size_t divpos = resolution.find_first_of('x');

        std::vector< Ogre::String > res = Ogre::StringUtil::split(resolution, "x", 1);

        mScreenWidth = Ogre::StringConverter::parseUnsignedInt(res[0]);
        mScreenHeight = Ogre::StringConverter::parseUnsignedInt(res[1]);

        Ogre::String fullscreen = config.getSetting("fullScreen",videoSectName);

        if(fullscreen == "true")
        {
            mIsFullScreen = true;
        }
        if(fullscreen == "false")
        {
            mIsFullScreen = false;
        }

        wndParamList["vsync"] = config.getSetting("vsync",videoSectName);
        wndParamList["displayFrequency"] = config.getSetting("displayFrequency",videoSectName);
        wndParamList["colourDepth"] = config.getSetting("colourDepth",videoSectName);
        wndParamList["FSAA"] = config.getSetting("FSAA",videoSectName);
        mWindow = SDL_SetVideoMode(mScreenWidth, mScreenHeight, Ogre::StringConverter::parseUnsignedInt(config.getSetting("colourDepth")),0);
        if (!rs)
        {
            // Unrecognised render system
            return false;
        }

        mOgre->setRenderSystem(rs);

        return true;
    }
    bool Kernel::readSPF()
    {
        uint32 spffourcc = MKFOURCC('S','P','F','0');
        uint32 filefourcc = 0;
        #ifdef WINDOWS
        if(_access("game.spf",0) == -1)
        {
            SONETTO_THROW("Unable to find game.spf");
        }
        #endif
        std::ifstream gamespf;
        gamespf.open("game.spf", std::ios_base::in | std::ios_base::binary);

        std::cout<<"FourCC Value: "<<spffourcc<<"\nFourCC from file: ";
        // Read the fourcc
        gamespf.read((char*)&filefourcc, sizeof(filefourcc));

        std::cout<<filefourcc<<"\n";

        if(filefourcc != spffourcc)
        {
            SONETTO_THROW("Invalid SPF file");
        }

        gamespf.seekg(4, std::ios_base::cur);

        mGameTitle = readString(gamespf);
        mGameIdentifier = readString(gamespf);
        mGameAuthor = readString(gamespf);

        gamespf.close();

        return true;
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
    void Kernel::run() {
    }
} // namespace
