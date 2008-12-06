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
