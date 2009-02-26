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

#ifndef GENERICMAPMODULE_MODULE_H
#define GENERICMAPMODULE_MODULE_H

#include <SonettoMapModule.h>
#include "GenericMapPrerequisites.h"

// Forward declarations
namespace GenericMapModule
{
    typedef std::map<uint32,Ogre::Entity *> EntityMap;
    typedef std::map<uint32,Ogre::BillboardSet *> BillboardSetMap;
    typedef std::map<uint32,Ogre::Billboard *> BillboardMap;
}

#include "GenericMap.h"
#include "GenericMapEvent.h"
#include "GenericMapWalkmeshSubManager.h"

namespace GenericMapModule
{
    class MapModule : public Sonetto::MapModule
    {
    public:
        MapModule() {}
        ~MapModule() {}

        void initialize();
        void update();
        void deinitialize();

        void halt();
        void resume();

        EntityMap &getSkyEntities() { return mSkyEntities; }
        Ogre::Entity *getSkyEntity(uint32 id) { return mSkyEntities[id]; }

        BillboardSetMap &getBillboardSets() { return mBillboardSets; }
        Ogre::BillboardSet *getBillboardSet(uint32 id)
                { return mBillboardSets[id]; }

        EventMap &getEvents() { return mEvents; }
        Event *getEvent(uint32 id) { return mEvents[id]; }

    private:
        void createScene();

        std::string mMapFolder;

        MapManager *mMapMan;
        MapPtr      mCurrentMap;

        Ogre::SceneNode *mSkyEntitiesNode;
        EntityMap mSkyEntities;

        BillboardSetMap mBillboardSets;
        BillboardMap mBillboards;

        WalkmeshSubManager mWalkmeshMan;
        EventMap mEvents;
    };
} // namespace

#endif
