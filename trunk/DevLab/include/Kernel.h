/*-----------------------------------------------------------------------------
This source file is part of Sonetto RPG Engine.

Copyright (C) 2007,2008 Arthur Carvalho de Souza Lima, Guilherme PrÃ¡ Vieira


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

#include <Ogre.h>
#include <stack>

namespace Sonetto {
    class Kernel;

    extern Kernel *KERNEL;
};

#include "Module.h"

namespace Sonetto {
    class Kernel {
        friend class Module;
    public:
        Kernel() {}
        ~Kernel() {}

        bool initialise();

        bool run();

        void pushModule(Module * pModule,bool haltMode = false);

        void popModule();

    public: // Change to protected or private later

        /// Module Stack
        std::stack<Module*> mModuleList;

        Ogre::Root              *mRoot;
        Ogre::RenderWindow      *mWindow;
        Ogre::Viewport          *mViewport;
        Ogre::OverlayManager    *mOverlayMan;

        bool hasEnded;

    };
}; // namespace

#endif
