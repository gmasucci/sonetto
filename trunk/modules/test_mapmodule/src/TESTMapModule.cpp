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
        mResGroupName = "MAP_DATA_RG";
        mResourceGroupCreated = false;
    }
    //-----------------------------------------------------------------------------
    TestMapModule::~TestMapModule()
    {
    }
    //-----------------------------------------------------------------------------
    void TestMapModule::enter()
    {
        EventObject *dummyEvent;

        mWalkmeshMan = new WalkmeshManager();
        mKernel->mMapFileManager = new MapFileManager();
        mBackgroundColor = Ogre::ColourValue(0.8f,0.8f,0.8f,1.0f);

        // Call the Module base function.
        Module::enter();

        mKernel->mResourceMan->addResourceLocation("hero/dummy_hero.zip", "Zip", "DUMMYHERO");
        mKernel->mResourceMan->initialiseResourceGroup("DUMMYHERO");

        mKernel->mResourceMan->addResourceLocation("debug/debug_data.zip", "Zip", "DEBUGDATA");
        mKernel->mResourceMan->initialiseResourceGroup("DEBUGDATA");

        Ogre::Entity * worldmesh = mSceneMan->createEntity("CollisionMesh", "walkmesh.mesh");
        mSceneMan->getRootSceneNode()->attachObject(worldmesh);

        mWalkmeshMan->createWalkmesh(worldmesh);

        dummyEvent = new EventObject("Sphere",
                                    mSceneMan->getRootSceneNode(),
                                    mSceneMan,
                                    2.0f,1.0f,0.6f,5.0f,
                                    false,
                                    "dummy_hero.mesh");
        dummyEvent->setPosition(7.20862f-3.0f,3.56544f,31.1969f-3.0f);

        mDummyHero = new EventObject("DummyHero",
                                    mSceneMan->getRootSceneNode(),
                                    mSceneMan,
                                    2.0f,1.0f,0.6f,5.0f,
                                    false,
                                    "dummy_hero.mesh");
        //mDummyHero->setPosition(7.20862f,3.36544f,31.1969f-1.0f);
        mWalkmeshMan->registerEvent(dummyEvent);
        mWalkmeshMan->registerEvent(mDummyHero);
        mWalkmeshMan->setEventPosition(mDummyHero,Ogre::Vector3(0.0f,-4.0f,4.0f));
        mWalkmeshMan->setEventPosition(dummyEvent,Ogre::Vector3(0.0f,20.0f,-4.0f));
        mCamera->setPosition(30.0f,30.0f,-30.0f);
        mCamera->lookAt(0.0f,0.0f,0.0f);
        mCamera->setNearClipDistance(1.0f);
        mCamera->setFarClipDistance(1000.0f);
        mCamera->setFOVy(Ogre::Radian(Ogre::Degree(13.5f)));

        mEvents.push_back(mDummyHero);
        mEvents.push_back(dummyEvent);

        MapIndexData mapdata;

        mapdata.mType = "FileSystem";
        mapdata.mFile = "test_map";
        mKernel->mDatabase->mDestMapID = 1;
        mKernel->mDatabase->mPlayerPosX = 0.0f;
    	mKernel->mDatabase->mPlayerPosY = 0.0f;
    	mKernel->mDatabase->mPlayerPosZ = 0.0f;
    	// Temporary Map Start Rotation. (only Y axis is used for now)
        mKernel->mDatabase->mPlayerRotX = 0.0f; // Unused
    	mKernel->mDatabase->mPlayerRotY = 0.0f;
    	mKernel->mDatabase->mPlayerRotZ = 0.0f; // Unused
    	mKernel->mDatabase->mPlayerRotW = 0.0f; // Unused
    	mKernel->mDatabase->mMapList.insert(std::pair<size_t, MapIndexData>(1,mapdata));

    	changeMap();

        mState = 0;
        mAngle = Ogre::Degree(360);

        mDummyHero->setOrientation(Ogre::Quaternion(Degree(0),Ogre::Vector3::UNIT_Y));

    }
    //-----------------------------------------------------------------------------
    void TestMapModule::update(Ogre::Real deltatime) // This one will be a pain...
    {
        Module::update(deltatime);
#ifdef DEBUG_BUILD
        // Debug function used to change the Scene Polygon mode from Solid to Wireframe or Points
        Module::setPolygonMode(deltatime);
#endif
        PlayerInput * player = mKernel->getInputMan()->getPlayer(0);

        Ogre::Vector2 mov,rot;

        float movMagnitude;
        mov = player->getAxisValue(AX_LEFT);
        rot = player->getAxisValue(AX_RIGHT);


        /*if(mov.x < 0.3f && mov.x > -0.3f)
            mov.x = 0.0f;

        if(mov.y < 0.3f && mov.y > -0.3f)
            mov.y = 0.0f;*/

        if(rot.x < 0.3f && rot.x > -0.3f)
            rot.x = 0.0f;

        if(rot.y < 0.3f && rot.y > -0.3f)
            rot.y = 0.0f;

        //mDummyHero->setBaseDirection(mCamera->getDirection());
        //mDummyHero->setBaseDirection(Ogre::Vector3(0.0f,0.0f,-1.0f));
        //mDummyHero->setMovementInput(mov);
        movMagnitude = mov.length();

        // Clamp the Magnitude values to 0.3f to 1.0f
        Math::clamp(movMagnitude,0.3f,1.0f);

        if (movMagnitude >= 0.3f)
        {
            Ogre::Vector3 mov3(mov.x,0.0f,mov.y),interpolate;
            Ogre::Vector3 final,source = mDummyHero->getOrientation() *
                    Ogre::Vector3::UNIT_X;
            Ogre::Quaternion finalOrientation;

            final = applyPerspective(mov3,mCamera->getDirection());

            interpolate = (source * (1.0f - (deltatime * 25.0f))) +
                    (final * (deltatime * 25.0f));

            finalOrientation = source.getRotationTo(interpolate);
            mDummyHero->setOrientation(finalOrientation *
                    mDummyHero->getOrientation());

            // <todo> Get speed from EventObject
            mWalkmeshMan->moveEvent(mDummyHero,final *
                    movMagnitude * deltatime);
        }

        mKernel->getScriptMan()->updateScript(mTestScript);

        for (size_t i = 0;i < mEvents.size();++i)
        {
            mEvents[i]->update(deltatime);
        }
        Ogre::Vector3 heropos(mDummyHero->getPosition());
        std::cout<<"HeroPos: "<<heropos.x<<"\t"<<heropos.y<<"\t"<<heropos.z<<"\n";

        mAngle += Ogre::Radian(rot.x * deltatime);

        Ogre::Vector3 dpos = mDummyHero->getPosition();
        mCamera->setPosition(dpos.x + (Ogre::Math::Sin(Ogre::Radian(mAngle))) * 30.f,
                dpos.y + 32.f,dpos.z + (Ogre::Math::Cos(Ogre::Radian(mAngle)) * 30.0f));
        mCamera->lookAt(dpos.x, dpos.y + 2.0f,dpos.z);

        switch(mState)
        {
            case 0:
                mKernel->getAudioMan()->playBGM(2, 0.0f, 0.0f);
                mKernel->setFadeSpeed(1.0f/0.2f);
                mKernel->startFade(false);
                mState = 1;
            break;
            case 1:
            if( mKernel->getFadeStatus() == FS_IDLE_IN )
                mState = 2;
            break;
            case 3:
                mKernel->getAudioMan()->stopMusic(1.0f/0.5f);
                mKernel->setFadeSpeed(1.0f/0.5f);
                mKernel->startFade(true);
                mState = 4;
            break;
            case 4:
            if( mKernel->getFadeStatus() == FS_IDLE_OUT )
            {
                mKernel->popModule();
            }
            break;
            case 2:
            if(player->getBtnState(BTN_CROSS) == KS_PRESS)
                mState = 3;

            break;
        }
    }
    //-----------------------------------------------------------------------------
    void TestMapModule::exit()
    {
        // Unload and remove the resource group.
        if(mResourceGroupCreated)
        {
            mKernel->mResourceMan->unloadResourceGroup(mResGroupName);
            mKernel->mResourceMan->destroyResourceGroup(mResGroupName);
        }

        mKernel->mResourceMan->_unregisterResourceManager("MapFile");
        mKernel->mMapFileManager = 0;

        for (size_t i = 0;i < mEvents.size();++i)
        {
            delete mEvents[i];
        }

        delete mWalkmeshMan;
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
    int TestMapModule::handleOpcode(Script *script,size_t id,
            OpcodeArguments *aArgs)
    {
        switch (id)
        {
            default:
                SONETTO_THROW("Unrecognized opcode sent to map module handler");
            break;
        }
    }
    //-----------------------------------------------------------------------------
    void TestMapModule::changeMap()
    {
        std::cout<<"changeMap()\n";
        if(mCurrentMapID != mKernel->mDatabase->mDestMapID)
        {
            mCurrentMapID = mKernel->mDatabase->mDestMapID;
            // Clean the last map resources
            if(mResourceGroupCreated)
            {
                mKernel->mResourceMan->unloadResourceGroup(mResGroupName);
                mKernel->mResourceMan->destroyResourceGroup(mResGroupName);
            }

            Ogre::String mapName = mKernel->mDatabase->mMapList[mCurrentMapID].mFile;
            Ogre::String mapType = mKernel->mDatabase->mMapList[mCurrentMapID].mType;

            // Create a new resource group, the MAP_DATA_RG.
            mKernel->mResourceMan->createResourceGroup(mResGroupName);
            mResourceGroupCreated = true;
            mKernel->mResourceMan->addResourceLocation("map/"+mapName, mapType, mResGroupName);
            mKernel->mResourceMan->addResourceLocation("scripts/" + mapName,
                    "FileSystem",mResGroupName);

            //mKernel->mResourceMan->declareResource("map.material", "Material", mResGroupName);
            mKernel->mResourceMan->initialiseResourceGroup(mResGroupName);
            mKernel->mResourceMan->loadResourceGroup(mResGroupName);

            MapFilePtr mapFile = mKernel->mMapFileManager->load("mapdata.dat", mResGroupName);
            mTestScript = mKernel->getScriptMan()->createScript("event_00.evt",mResGroupName);

            mCurrentMapName = mapFile->mMapName;
            mSceneMan->setAmbientLight(mapFile->mAmbientColor);
            mBackgroundColor = mapFile->mBackgroundColor;
            mKernel->getViewport()->setBackgroundColour(mBackgroundColor);

            if(mapFile->mUseLight0)
            {
                mMainLight0 = mSceneMan->createLight("MainLight0");
                mMainLight0->setType(Ogre::Light::LT_DIRECTIONAL);
                mMainLight0->setPosition(0.0f,0.0f,0.0f);
                mMainLight0->setDirection(mapFile->mLightDirection0);
                mMainLight0->setDiffuseColour(mapFile->mLightColor0);
                mMainLight0->setSpecularColour(mapFile->mLightColor0);
            }

            if(mapFile->mUseLight1)
            {
                mMainLight1 = mSceneMan->createLight("MainLight1");
                mMainLight1->setType(Ogre::Light::LT_DIRECTIONAL);
                mMainLight1->setPosition(0.0f,0.0f,0.0f);
                mMainLight1->setDirection(mapFile->mLightDirection1);
                mMainLight1->setDiffuseColour(mapFile->mLightColor1);
                mMainLight1->setSpecularColour(mapFile->mLightColor1);
            }
            // Layer setup, first pass, copy and create every object first
            for(size_t pos = 0; pos < mapFile->mMapLayer.size(); ++pos)
            {
                MapLayer mapLayer;
                mapLayer.mLayerName = mapFile->mMapLayer[pos].mLayerName;
                mapLayer.mParentLayerName = mapFile->mMapLayer[pos].mParentLayerName;
                mapLayer.mModel = mSceneMan->createEntity(mapLayer.mLayerName+"_model", mapFile->mMapLayer[pos].mLayerModelName);
                mapLayer.mNode = mSceneMan->createSceneNode(mapLayer.mLayerName+"_node");
                mapLayer.mNode->setPosition(mapFile->mMapLayer[pos].mLayerPosition);
                mapLayer.mNode->setOrientation(mapFile->mMapLayer[pos].mLayerOrientation);
                mapLayer.mNode->attachObject(mapLayer.mModel);
                mapLayer.mLayerSettingsFlags = mapFile->mMapLayer[pos].mLayerSettingsFlags;
                mapLayer.mAnimationName = mapFile->mMapLayer[pos].mAnimationName;
                mapLayer.mAnimRotation = mapFile->mMapLayer[pos].mAnimRotation;
                mapLayer.mAnimationSpeed = mapFile->mMapLayer[pos].mAnimationSpeed;
                mapLayer.mMaterialName = mapFile->mMapLayer[pos].mMaterialName;
                mMapLayers.push_back(mapLayer);
            }
            // Layer setup, second pass,
            for(size_t pos = 0; pos < mMapLayers.size(); ++pos)
            {
                if(mMapLayers[pos].mParentLayerName == Ogre::String("ROOT"))
                {
                    std::cout<<"Parent name is Root\n";
                    mSceneMan->getRootSceneNode()->addChild(mMapLayers[pos].mNode);
                } else {
                    std::cout<<"Parent name is not Root\n";
                    for(size_t i = 0; i < mMapLayers.size(); ++i)
                    {
                        if(mMapLayers[pos].mParentLayerName == mMapLayers[i].mLayerName)
                        {
                            mMapLayers[i].mNode->addChild(mMapLayers[pos].mNode);
                        }
                    }
                }
                /*if(mMapLayers[pos].mLayerSettingsFlags & 0x01)
                {
                    Ogre::AnimationState * anime = mMapLayers[pos].mModel->getAnimationState(mMapLayers[pos].mAnimationName);
                    anime->setEnabled(true);
                }
                if(mMapLayers[pos].mLayerSettingsFlags & 0x02)
                {
                    Ogre::AnimationState * anime = mMapLayers[pos].mModel->getAnimationState(mMapLayers[pos].mAnimationName);
                    anime->setLoop(true);
                } else {
                    Ogre::AnimationState * anime = mMapLayers[pos].mModel->getAnimationState(mMapLayers[pos].mAnimationName);
                    anime->setLoop(false);
                }
                if(mMapLayers[pos].mLayerSettingsFlags & 0x08)
                {
                    mMapLayers[pos].mModel->setCastShadows(true);
                } else {
                    mMapLayers[pos].mModel->setCastShadows(false);
                }

                if(mMapLayers[pos].mLayerSettingsFlags & 0x10)
                {
                    mMapLayers[pos].mModel->setMaterialName(mMapLayers[pos].mMaterialName);
                }*/
            }

            mSceneMan->setFog(Ogre::FOG_NONE,mapFile->mFogColor,mapFile->mFogExpDensity,mapFile->mFogStart,mapFile->mFogEnd);

            // Here we would set the Hero's initial position, so let's set the dummy in their place instead
            //mDummyHero->setPosition(mKernel->mDatabase->mPlayerPosX,mKernel->mDatabase->mPlayerPosY,mKernel->mDatabase->mPlayerPosZ);
            //mDummyHero->setOrientation(mKernel->mDatabase->mPlayerRotW,mKernel->mDatabase->mPlayerRotX,mKernel->mDatabase->mPlayerRotY,mKernel->mDatabase->mPlayerRotZ);
        }

    }
    //-----------------------------------------------------------------------------
    Ogre::Vector3 TestMapModule::applyPerspective(const Ogre::Vector3 &aVector,
                const Ogre::Vector3 &perspective)
    {
        Ogre::Vector3 vector(aVector);
        Ogre::Vector3 perspectiveFront(perspective),perspectiveRight,
                directionFinal;

        vector.normalise();

        perspectiveRight.x = perspectiveFront.z;
        perspectiveRight.y = perspectiveFront.y;
        perspectiveRight.z = -perspectiveFront.x;

        directionFinal.x = ((-vector.x) * perspectiveRight.x) + ((-vector.z) *
                perspectiveFront.x);
        directionFinal.y = 0.0f;
        directionFinal.z = ((-vector.x) * perspectiveRight.z) + ((-vector.z) *
                perspectiveFront.z);

        // Normalise vector
        directionFinal.normalise();

        return directionFinal;
    }
}; // namespace Sonetto
