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

#ifndef SONETTO_KERNEL_H
#define SONETTO_KERNEL_H

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
        Kernel() : mInitialised(false) {}

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

    private:
        /// Loads configuration from file and configures Sonetto
        void loadConfig(std::string file);

        /// Ogre::Root instance
        Ogre::Root *mOgreRoot;

        /// Whether initialise() was called or not
        bool mInitialised;
    };
} // namespace

#endif
