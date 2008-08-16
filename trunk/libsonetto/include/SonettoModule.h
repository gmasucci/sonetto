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

#include "SonettoMain.h"

// Forward declarations
namespace Sonetto {
    class Kernel;
    class Module;
};

#include <OGRE/Ogre.h>
#include "SonettoKernel.h"

namespace Sonetto {
    /** @brief Game Module interface

        This class is used as an interface to abstract different modules
        for the Kernel, in such a way that a Map Module can be treated the
        same way as a Battle Module. Also, there can be additional modules
        created by you that, of course, are unknown at the point of compiling
        libSonetto. Such modules must inherit from Sonetto::Module and implement
        at least the pure virtual method Sonetto::Module::update(). The other
        methods have useful code that are common to any Module (like creating
        an Ogre::SceneManager and Ogre::Camera to them). If you happen to make a
        base call to any of these methods, make sure you do so to ALL of them, as
        each one expects each other's deeds in the class.
    @see
        http://code.google.com/p/sonetto/source/browse/trunk/libsonetto/src/SonettoModule.cpp
    */
    class SONETTO_EXPORT Module {
    public:
        Module();
        virtual ~Module() {}

        /** @brief Module initialization

            This function gets called by Kernel when it's time to initialise the
            module.
        */
        virtual void enter();

        /** @brief Module update

            Gets called by Kernel on each frame update.
        @param
            deltatime Time since last frame.
        */
        virtual void update(Ogre::Real deltatime) = 0;

        /** @brief Module cleanup

            Gets called by Kernel prior to a deletion. Should delete it's
            Entities, stop sounds/BGM, etc.
        */
        virtual void exit();

        /** @brief Module halt

            Gets called by Kernel when another Module has taken the top of the
            module stack.
        */
        virtual void halt();

        /** @brief Module wakeup

            Gets called by Kernel when the top Module has been popped and this
            module will start receiving update()'s again.
        */
        virtual void wakeup();

    public:
        /// @brief Use this in place of Kernel::get() for the sake of simplicity
        Kernel *mKernel;

        /// @brief Module Scene Manager
        Ogre::SceneManager *mSceneMan;

        /// @brief Module Overlay
        Ogre::Overlay *mOverlay;

        /// @brief Module Camera
        Ogre::Camera *mCamera;
    };
}; // namespace

#endif
