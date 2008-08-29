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
#include "TESTMapModule.h"
namespace Sonetto {
    //-----------------------------------------------------------------------------
    // Sonetto::TESTMapModule implementation.
    //-----------------------------------------------------------------------------
    TestMapModule::TestMapModule() : Module()
    {
        mModuleOverlayName = "MAP_OVERLAY";
    }
    //-----------------------------------------------------------------------------
    TestMapModule::~TestMapModule()
    {
    }
    //-----------------------------------------------------------------------------
    void TestMapModule::enter()
    {
        // Call the Module base function.
        Module::enter();
        // Create a new resource group, the MAP_DATA_RG.
        mKernel->mResourceMan->createResourceGroup("MAP_DATA_RG");
    }
    //-----------------------------------------------------------------------------
    void TestMapModule::update(Ogre::Real deltatime) // This one will be a pain...
    {
        if(mKernel->getInputMan()->getPlayer(0)->getBtnState(BTN_CROSS) == KS_PRESS)
            mKernel->popModule();
    }
    //-----------------------------------------------------------------------------
    void TestMapModule::exit()
    {
        // Unload and remove the resource group.
        mKernel->mResourceMan->destroyResourceGroup("MAP_DATA_RG");
        // Call the Module base function.
        Module::exit();
    }
    //-----------------------------------------------------------------------------
    void TestMapModule::halt()
    {
        // Call the Module base function.
        Module::halt();
    }
    //-----------------------------------------------------------------------------
    void TestMapModule::wakeup()
    {
        // Call the Module base function.
        Module::wakeup();
    }
    //-----------------------------------------------------------------------------
    void TestMapModule::changeMap()
    {
        // Start by cleaning the current map.
        cleanMap();
        // Read the map to be loaded from the database.
        std::string destMapFile = mKernel->mDatabase->mMapList[mKernel->mDatabase->mMapID];
        // Load the Map Resources to the memory.
        loadMapFile(destMapFile);
    }
    //-----------------------------------------------------------------------------
    void TestMapModule::loadMapFile(std::string filename)
    {

    }
    //-----------------------------------------------------------------------------
    void TestMapModule::cleanMap()
    {
    }
}; // namespace Sonetto
