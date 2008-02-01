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

#include <Ogre.h>
#include "Kernel.h"

namespace Sonetto {
    Kernel *KERNEL = new Kernel();
    
    bool Kernel::initialise()
    {
        hasEnded=false;
        mRoot = new Ogre::Root("plugins.dlc","devlab.dlc","DevLab.log");
        if (mRoot->showConfigDialog()) {
            // If returned true, user clicked OK so initialise
            // Here we choose to let the system create a default rendering window by passing 'true'
            mWindow = mRoot->initialise(true, "Sonetto Development Laboratory");
        }
        else {
            return false;
        }
        
        return true;
    }
    
    bool Kernel::run() {
        bool hasEnded = false;
        
        while(!hasEnded) {
            // Update the window (Message Pump)
            Ogre::WindowEventUtilities::messagePump();
            
            // Update the stack top Module
            mModuleList.top()->update(1.0f);
            
            mRoot->renderOneFrame();
        }
        
        return true;
    }
    
    void Kernel::pushModule(Module * pModule,bool haltMode) {
        assert(pModule);
        
        if(!mModuleList.empty()) {
            if(haltMode) {
                mModuleList.top()->halt();
            } else {
                delete mModuleList.top();
                mModuleList.pop();
            }
        }
        
        mModuleList.push(pModule);
        pModule->enter();
    }
    
    void Kernel::popModule() {
        assert(mModuleList.size() >= 2); // Ensure the stack wont be empty. A Kernel
                                      // must have at least one Module.
        
        mModuleList.top()->exit(); // Cleanup
        delete mModuleList.top();
        mModuleList.pop();
        
        mModuleList.top()->wakeup();
    }
}; // namespace

