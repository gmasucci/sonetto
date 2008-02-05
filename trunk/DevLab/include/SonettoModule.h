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

#ifndef __SONETTO_MODULE_H_
#define __SONETTO_MODULE_H_

// Forward declarations
namespace Sonetto {
    class Module;
};

#include "SonettoKernel.h"

namespace Sonetto {
    class Module {
    public:
        Module() :
        mSceneMan     (NULL),
        mOverlay      (NULL),
        mCamera       (NULL) {
            mKernel = Sonetto::KERNEL;
        }

        virtual ~Module() {}

        /** @brief Module initialization
         *
         *  This function gets called by Kernel when it's time to initialize the
         *  module.
         */
        virtual void enter();

        /** @brief Module update
         *
         *  Gets called by Kernel on each frame update.
         *  @param deltatime Time since last frame.
         */
        virtual void update(Ogre::Real deltatime) = 0;

        /** @brief Module cleanup
         *
         *  Gets called by Kernel prior to a deletion. Should delete it's
         *  Entities, stop sounds/BGM, etc.
         */
        virtual void exit();

        /** @brief Module halt
         *
         *  Gets called by Kernel when another Module has taken the top of the
         *  module stack.
         */
        virtual void halt();

        /** @brief Module wakeup
         *
         *  Gets called by Kernel when the top Module has been popped and this
         *  module will start receiving update()'s again.
         */
        virtual void wakeup();

        friend class Kernel;

    public:
        /// Use this in place of Kernel::getSingletonPtr().
        Kernel              *mKernel;
        /// Module Scene Manager
        Ogre::SceneManager  *mSceneMan;
        /// Module Overlay
        Ogre::Overlay       *mOverlay;
        /// Module Camera
        Ogre::Camera        *mCamera;
    };
}; // namespace

#endif
