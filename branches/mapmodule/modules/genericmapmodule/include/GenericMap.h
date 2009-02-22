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

#ifndef GENERICMAPMODULE_MAP_H
#define GENERICMAPMODULE_MAP_H

#include <OgrePrerequisites.h>
#include <OgreResource.h>
#include <SonettoSharedPtr.h>
#include "GenericMapPrerequisites.h"
#include "GenericMapWalkmesh.h"
#include "GenericMapSkyEntityData.h"
#include "GenericMapBillboardSetData.h"
#include "GenericMapBillboardData.h"
#include "GenericMapEventPage.h"

namespace GenericMapModule
{
    struct EventData
    {
        EventPageVector pages;
    };

    typedef std::map<uint32,EventData> EventDataMap;

    class Map : public Ogre::Resource
    {
    public:
        Map(Ogre::ResourceManager *creator,const Ogre::String &name,
                Ogre::ResourceHandle handle,const Ogre::String &group,
                bool isManual = false,Ogre::ManualResourceLoader *loader = 0)
                : Ogre::Resource(creator,name,handle,group,isManual,loader) {}

        virtual ~Map();

        size_t calculateSize() const;

        inline void _setMapName(const std::string &name) { mMapName = name; }
        inline const std::string &getMapName() const { return mMapName; }

        inline Ogre::StaticGeometry *getStaticGeometry() const
                { return mStaticGeom; }
        inline void _setStaticGeometry(Ogre::StaticGeometry *geom)
                { mStaticGeom = geom; }

        inline const Walkmesh &getWalkmesh() const { return mWalkmesh; }
        inline Walkmesh &_getWalkmesh() { return mWalkmesh; }

        inline const SkyEntityDataMap &getSkyEntitiesData() const
                { return mSkyEntitiesData; }
        inline SkyEntityDataMap &_getSkyEntitiesData()
                { return mSkyEntitiesData; }

        inline const BillboardSetDataMap &getBillboardSetData() const
                { return mBillboardSetData; }
        inline BillboardSetDataMap &_getBillboardSetData()
                { return mBillboardSetData; }

        inline const BillboardDataMap &getBillboardData() const
                { return mBillboardData; }
        inline BillboardDataMap &_getBillboardData()
                { return mBillboardData; }

        inline const EventDataMap &getEventData() const { return mEventData; }
        inline EventDataMap &_getEventData() { return mEventData; }

        inline void _setResourceSize(size_t size) { mResourceSize = size; }

    protected:
        // Ogre::Resource interface implementation
        void loadImpl();
        void unloadImpl();

        size_t mResourceSize;

        std::string mMapName;

        Ogre::StaticGeometry *mStaticGeom;

        Walkmesh mWalkmesh;

        SkyEntityDataMap mSkyEntitiesData;

        BillboardSetDataMap mBillboardSetData;
        BillboardDataMap mBillboardData;

        EventDataMap mEventData;
    };

    typedef Sonetto::SharedPtr<Map> MapPtr;
} // namespace

#endif
