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
        // Checks if wasn't initialised yet
        if (mInitialised)
        {
            SONETTO_THROW("Kernel is already initialised");
        }

        // Creates Ogre root
        // Produce logs only on debug compilations
        #ifdef DEBUG
            mOgreRoot = new Ogre::Root("","","game.log");
        #else
            mOgreRoot = new Ogre::Root("","","");
        #endif

        // Configures Sonetto based on configuration file
        loadConfig("config.ini");

        // <todo> Get this caption from somewhere that makes more sense
        // (Sonetto Runtime, maybe?)
        mOgreRoot->initialise(true,"Game");

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
            delete mOgreRoot;
        }
    }
    // ----------------------------------------------------------------------
    void Kernel::loadConfig(std::string file) {}
    // ----------------------------------------------------------------------
    void Kernel::run() {}
} // namespace
