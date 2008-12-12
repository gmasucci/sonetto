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
        mOgre = new Ogre::Root("","","game.log");
#else
        mOgre = new Ogre::Root("","","");
#endif

#ifdef WINDOWS
        wndParamList["externalWindowHandle"] = Ogre::StringConverter::toString((unsigned long)wmInfo.window);
#else
        // TODO
#endif

        // Load and configure sonetto.
        // <todo> Store and load Sonetto Configuration from User directory.
        loadConfig("config.ini",wndParamList);

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
        Ogre::ConfigFile config;
        //config.loadDirect(fname, "\t:=", false);
        config.load(fname);

        // Video Configuration Section
        Ogre::String videoSectName = "video";
        mGameTitle = config.getSetting("projectTitle",videoSectName);

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
    // ----------------------------------------------------------------------
    void Kernel::run() {
    }
} // namespace
