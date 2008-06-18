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
#ifndef __SONETTO_KERNEL__
#define __SONETTO_KERNEL__

#include "SonettoMain.h"

#include <fstream>
#include <stack>
#include <Ogre.h>
#include <OgreVector2.h>

#include "SonettoAudioManager.h"
#include "SonettoInputManager.h"
#include "SonettoModule.h"
#include "SonettoRARC.h"
#include "SonettoFontSerializer.h"
#include "SonettoFontManager.h"
#include "SonettoTextFactory.h"
#include "SonettoWindowSkinSerializer.h"
#include "SonettoWindowSkinManager.h"
#include "SonettoWindowFactory.h"
#include "SonettoGauge.h"
#include "SonettoCounter.h"

namespace Sonetto {

    class SONETTO_EXPORT Kernel
    {
        friend class SONETTO_EXPORT InputManager;
        friend class SONETTO_EXPORT AudioManager;
        friend class SONETTO_EXPORT AudioManager;
    public:
        /// Return the singleton pointer to this class SONETTO_EXPORT.
        inline static Kernel *getSingleton()  { return mSingleton; }
        /// Check if the kernel is initialised.
        inline static bool    isInitialised() { return mSingleton; }
        /** Initialise Ogre, OIS, and other Sonetto dependencies
        @return
            Wheter it was successful or not.
        */
        static bool initialise();
        /// Deinitialise everything initialised before by initialise().
        static bool deinitialise();
        /// Start the game's Main Loop
        int run();
        /** Add a module in the module stack
        @remarks
            The module in the top of the stack is
            Allways the active module, basically,
            if you put a new module in the stack,
            It'll become the active module.
        @param
            pModule the Module pointer to add in the stack
        @param
            haldMode if it's true, the old module will be kept in the memory
            and the new module will become the active one, if it's false, then
            the old module will be deleted and the new module will become the active module.
        */
        void pushModule(Module *pModule,bool haltMode = false);
        /// Remove a module from the top of the stack.
        void popModule();
        /// Shutdown / Close the game.
        void shutdown() { mShutDown = true; }

        Ogre::Viewport * getViewport();
        void setViewport(Ogre::Viewport * viewport);
        Ogre::RenderWindow * getRenderWindow();
        Ogre::Root * getOgreRoot();

        InputManager * getInputMan();
        AudioManager * getAudioMan();

        /// Load and parse the configuration file.
        std::map<std::string,std::string> Kernel::loadConfig(const char *fname);
    private:
        bool _initialise();
        bool _deinitialise();
        /// Default construct SONETTO_EXPORTor.
        Kernel();
        /// Default destruct SONETTO_EXPORTor.
        ~Kernel() {}
    public:
        /// Flag indicating the program will be closed.
        bool mShutDown;
        /// Static pointer to this class SONETTO_EXPORT.
        static Kernel *mSingleton;
        /// Module List stack (holds the active modules).
        std::stack<Module *>          mModuleList;
        /// Ogre pointers.
        Ogre::Root                   *mRoot; // Ogre Root pointer.
        Ogre::RenderWindow           *mWindow; // Ogre Render Window.
        Ogre::Viewport               *mViewport; // Over Viewport.
        Ogre::LogManager             *mLogMan; // Ogre Log Manager.
        Ogre::OverlayManager         *mOverlayMan; // Ogre Overlay Manager.
        Ogre::ResourceGroupManager   *mResourceMan; // Ogre Resource Group Manager.
        Ogre::ResourceManager        *mResMan; // Ogre Resource Manager.
        // Debug Stuff.
        #ifdef _DEBUG
        Ogre::Overlay                *mDebugOverlay; // Sonetto Debug Overlay.
        #endif
        /// Sonetto Pointers.
        AudioManager                 *mAudioMan; // Sonetto Audio Manager.
        InputManager                 *mInputMan; // Sonetto Input Manager.

    };

}; // namespace Sonetto

#endif // __SONETTO_KERNEL__
