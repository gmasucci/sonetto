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

#ifndef SONETTO_MODULE_H
#define SONETTO_MODULE_H

#include <stack>
#include <Ogre.h>
#include "SonettoPrerequisites.h"

namespace Sonetto
{
    class SONETTO_API Module
    {
    public:
        enum ModuleType
        {
            MT_NONE,
            MT_BOOT,
            MT_TITLE,
            MT_MAP,
            MT_MENU,
            MT_WORLD,
            MT_BATTLE
        };

        Module(){}
        virtual ~Module() {}

        virtual void initialize();
        virtual void update();
        virtual void deinitialize();

        virtual void halt();
        virtual void resume();

        /** Change the viewport background color */
        void setBgColor(const Ogre::ColourValue &col);

        /// Pointer to the scene manager for this module.
        Ogre::SceneManager * mSceneMan;

        /// Pointer to the overlay for this module.
        Ogre::Overlay * mOverlay;

        /// Pointer to this module's camera.
        Ogre::Camera * mCamera;

        /// Pointer to the module viewport.
        Ogre::Viewport * mViewport;

        /// String containing the Overlay name for this module.
        std::string mOverlayName;

        /// Current background color for this module's viewport.
        Ogre::ColourValue mBgColor;
    };

    typedef std::stack<Module *> ModuleStack;
} // namespace

#endif
