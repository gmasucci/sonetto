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

#ifndef SONETTO_KERNEL_H
#define SONETTO_KERNEL_H

#include <SDL/SDL.h>
#include <SDL/SDL_syswm.h>
#include <SDL/SDL_video.h>

#include <Ogre.h>

#include "SonettoPrerequisites.h"

namespace Sonetto
{
    /** Sonetto Kernel

        This singleton is the core of this library. When its time to run Sonetto,
        this singleton will start its main loop and will only return when the
        game end.
    */
    class SONETTO_API Kernel : public Ogre::Singleton<Kernel>
    {
    public:
        /** Constructor

        @remarks
            This instance will only be ready for use when initialise() is called.
        @see
            Kernel::initialise().
        */
        Kernel() :
        mScreenWidth(640),
        mScreenHeight(480),
        mIsFullScreen(false),
        mInitialised(false) {}

        /** Destructor

            If initialise() was called before this destructor, it will
            deinitialise Sonetto and the other libraries used during this
            singleton's lifetime.
        @see
            Kernel::initialise()
        */
        ~Kernel();

        /** Overrides standard Singleton retrieval

        @remarks
            Warning: These comments below were copied straight from Ogre3D code.
            Why do we do this? Well, it's because the Singleton
            implementation is in a .h file, which means it gets compiled
            into anybody who includes it. This is needed for the
            Singleton template to work, but we actually only want it
            compiled into the implementation of the class based on the
            Singleton, not all of them. If we don't change this, we get
            link errors when trying to use the Singleton-based class from
            an outside dll.
        @param
            This method just delegates to the template version anyway,
            but the implementation stays in this single compilation unit,
            preventing link errors.
        */
        static Kernel &getSingleton();

        /** Overrides standard Singleton retrieval

        @remarks
            Warning: These comments below were copied straight from Ogre3D code.
            Why do we do this? Well, it's because the Singleton
            implementation is in a .h file, which means it gets compiled
            into anybody who includes it. This is needed for the
            Singleton template to work, but we actually only want it
            compiled into the implementation of the class based on the
            Singleton, not all of them. If we don't change this, we get
            link errors when trying to use the Singleton-based class from
            an outside dll.
        @param
            This method just delegates to the template version anyway,
            but the implementation stays in this single compilation unit,
            preventing link errors.
        */
        static Kernel *getSingletonPtr();

        /** Initialises the Kernel

            This is where Sonetto and all libraries it uses are set up. After a
            call to this method, the Kernel is ready to start its main loop. See
            run().
        @see
            Kernel::run().
        */
        void initialise();

        /** Starts running the game

            This method is where the main loop of the game lives. It will only
            return when the game end. The program can then return zero, meaning
            everything went smoothly. If any fatal error occurs, Sonetto will
            throw an exception.
        */
        void run();

        /// Project / Game Title
        std::string mGameTitle;
        /// Project / Game Identifier (Save Folder)
        std::string mGameIdentifier;
        /// Project / Game Author
        std::string mGameAuthor;
        /// Game Data Path (Savegame directory)
        std::string mGameData;
        /// Game Data Path (Including Project Directory)
        std::string mGameDataPath;
        /// Current Screen Resolution (Width)
        int mScreenWidth;
        /// Current Screen Resolution (Height)
        int mScreenHeight;
        /// Screen Mode (Full / Window)
        bool mIsFullScreen;

    private:
        /// Loads configuration from file and configures Sonetto
        bool loadConfig(const std::string &fname,Ogre::NameValuePairList &wndParamList);

        /// Read the Sonetto Project File.
        bool readSPF();

        std::string readString(std::ifstream &stream);

        /// Ogre::Root instance
        Ogre::Root *mOgre;

        /// Ogre::RenderWindow instance
        Ogre::RenderWindow *mRenderWindow;

        /// SDL Surface (Window)
        SDL_Surface *mWindow;

        /// Whether initialise() was called or not
        bool mInitialised;
    };
} // namespace

#endif
