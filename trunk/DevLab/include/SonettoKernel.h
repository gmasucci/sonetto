/*-----------------------------------------------------------------------------
This source file is part of Sonetto RPG Engine.

Copyright (C) 2007,2008 Arthur Carvalho de Souza Lima, Guilherme Prá¡ Vieira


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

#ifndef __SONETTO_KERNEL_H_
#define __SONETTO_KERNEL_H_

// Forward declarations
namespace Sonetto {
    class Kernel;

    extern Kernel *KERNEL;
};

#include <Ogre.h>
#include <OgreVector2.h>
#include <stack>

#include "SonettoInputManager.h"
#include "SonettoModule.h"

#include "SonettoFontSerializer.h"
#include "SonettoFontManager.h"
#include "SonettoTextFactory.h"

namespace Sonetto {
    class Kernel {
    public:
        /// Default constructor.
        Kernel() : mRoot(0), mWindow(0), mViewport(0), mOverlayMan(0), mInitialised(0) {}

        /// Default destructor.
        ~Kernel() {}

        /** @brief Initialise Ogre, OIS, and other Sonetto dependencies
         *  @return Wheter it was successful or not.
         */
        bool initialise();

        /** @brief Deinitialise everything initialised before by initialise().
         */
        bool deinitialise();

        bool run();

        void pushModule(Module *pModule,bool haltMode = false);

        void popModule();

    public: // Change to other after...
        /// Module Stack
        std::stack<Module *> mModuleList;

        Ogre::Root                   *mRoot;
        Ogre::RenderWindow           *mWindow;
        Ogre::Viewport               *mViewport;
        Ogre::OverlayManager         *mOverlayMan;
        Ogre::ResourceGroupManager   *mResourceMan;

        bool mShutDown;
        bool mInitialised;

        // Sonetto Stuff
        InputManager       *mInputMan;
        FontManager        *mFontMan;
        TextElementFactory *mTextElementFactory;

        //Temporary stuff
        FontPtr mMainFont;
        Ogre::Overlay          *mDebugOverlay;
        Ogre::OverlayContainer *mDebugOverlayContainer;
        TextElement            *mDebugText;
        TextElement            *mDebugErrorText;

    };
}
; // namespace

#endif