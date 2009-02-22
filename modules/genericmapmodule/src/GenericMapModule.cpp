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

#include <OgreResourceGroupManager.h>
#include "GenericMapManager.h"
#include "GenericMapModule.h"

namespace GenericMapModule
{
    // ----------------------------------------------------------------------
    // GenericMapModule::MapModule implementation.
    // ----------------------------------------------------------------------
    void MapModule::initialize()
    {
        mBgColor = Ogre::ColourValue(0.0f,1.0f,0.0f);
        Sonetto::MapModule::initialize();

        if (StaticData.id == 0)
        {
            SONETTO_THROW("Unknown map ID");
        }

        // <todo> Use OGRE_NEW
        mMapMan = new MapManager();

        mMapFolder = "map_" + Ogre::StringConverter::
                toString(StaticData.id - 1,2,'0');

        Ogre::ResourceGroupManager &resGroupMan =
                Ogre::ResourceGroupManager::getSingleton();

        resGroupMan.addResourceLocation("map/" + mMapFolder,"FileSystem",
                "MAP_LOCAL");
        resGroupMan.initialiseResourceGroup("MAP_LOCAL");
        resGroupMan.loadResourceGroup("MAP_LOCAL");

        mCurrentMap = mMapMan->load("map_00.map","MAP_LOCAL");
        createScene();

        mCamera->setPosition(0.0f,10.0f,-50.0f);
        mCamera->lookAt(Ogre::Vector3(0.0f,0.0f,50.0f));
    }
    // ----------------------------------------------------------------------
    void MapModule::update()
    {
        Sonetto::MapModule::update();

        //mSkyEntitiesNode->setPosition(mCamera->getDerivedPosition());
    }
    // ----------------------------------------------------------------------
    void MapModule::deinitialize()
    {
        Ogre::ResourceGroupManager::getSingleton().
                destroyResourceGroup("MAP_LOCAL");
        delete mMapMan;

        Sonetto::MapModule::deinitialize();
    }
    // ----------------------------------------------------------------------
    void MapModule::halt()
    {
        Ogre::ResourceGroupManager::getSingleton().
                unloadResourceGroup("MAP_LOCAL");

        Sonetto::MapModule::halt();
    }
    // ----------------------------------------------------------------------
    void MapModule::resume()
    {
        Sonetto::MapModule::resume();

        Ogre::ResourceGroupManager::getSingleton().
                loadResourceGroup("MAP_LOCAL");
    }
    // ----------------------------------------------------------------------
    void MapModule::createScene()
    {
        assert(!mCurrentMap.isNull());

        // Creates scene nodes
        mSkyEntitiesNode = mSceneMan->getRootSceneNode()->
                createChildSceneNode();

        // Shows map's static geometry
        mCurrentMap->getStaticGeometry()->setVisible(true);

        // Creates sky entities
        const SkyEntityDataMap &skyEntitiesMap =
                mCurrentMap->getSkyEntitiesData();

        for (SkyEntityDataMap::const_iterator iter = skyEntitiesMap.begin();
            iter != skyEntitiesMap.end();++iter)
        {
            const SkyEntityData &entityData = iter->second;

            Ogre::Entity *entity = mSceneMan->createEntity(
                    "MAP_SKYLAYER_ENTITY_" +
                    Ogre::StringConverter::toString(iter->first,3,'0'),
                    entityData.mesh->getName());

            // Sets render queue group
            // <todo> Check why this isn't working
            entity->setRenderQueueGroup(Ogre::RENDER_QUEUE_SKIES_EARLY);

            // Sets visibility
            entity->setVisible(entityData.initialVisibility);

            // Creates and attaches to scene node
            Ogre::SceneNode *entityNode = mSkyEntitiesNode->
                    createChildSceneNode(entityData.initialPosition);
            entityNode->attachObject(entity);

            // Adds to sky entities vector
            mSkyEntities[iter->first] = entity;
        }

        // Creates billboards and billboard sets
        const BillboardSetDataMap &mapBillboardSetData = mCurrentMap->
                getBillboardSetData();
        const BillboardDataMap &mapBillboardsData = mCurrentMap->
                getBillboardData();

        for (BillboardSetDataMap::const_iterator iter =
            mapBillboardSetData.begin();iter !=
            mapBillboardSetData.end();++iter)
        {
            const BillboardSetData &billboardSetData = iter->second;

            Ogre::BillboardSet *billboardSet = mSceneMan->
                    createBillboardSet("MAP_BILLBOARDLAYER_" +
                    Ogre::StringConverter::toString(iter->first),
                    billboardSetData.poolSize);

            billboardSet->setMaterialName(billboardSetData.materialName);

            Ogre::SceneNode *setNode = mSceneMan->getRootSceneNode()->
                    createChildSceneNode(billboardSetData.position);
            setNode->attachObject(billboardSet);

            mBillboardSets[iter->first] = billboardSet;

            // <todo> Stop using [i] in this kind of loop
            // Use iterators instead
            for (size_t i = 0;i < billboardSetData.billboards.size();++i)
            {
                uint32 id = billboardSetData.billboards[i];
                const BillboardData &billboardData =
                        mapBillboardsData.find(id)->second;

                // <todo> Check why color parameter isn't actually changing anything
                Ogre::Billboard *billboard = billboardSet->createBillboard(
                        billboardData.position,billboardData.color);

                if (billboardSetData.defaultSize != billboardData.size)
                {
                    billboard->setDimensions(billboardData.size.x,
                            billboardData.size.y);
                }

                billboard->setTexcoordRect(billboardData.texCoord);

                mBillboards[id] = billboard;
            }
        }
    }
} // namespace
