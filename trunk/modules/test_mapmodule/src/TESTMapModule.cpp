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
        mKernel->mMapFileManager = new MapFileManager();
        mBackgroundColor = Ogre::ColourValue(0.8f,0.8f,0.8f,1.0f);
        // Call the Module base function.
        Module::enter();

        mKernel->mResourceMan->addResourceLocation("hero/dummy_hero.zip", "Zip", "DUMMYHERO");
        mKernel->mResourceMan->initialiseResourceGroup("DUMMYHERO");
        mDummyHero = mSceneMan->createEntity("DummyHero", "dummy_hero.mesh");
        mDummyHeroSceneNode = mSceneMan->getRootSceneNode()->createChildSceneNode("DummyHeroSceneNode");
        mDummyHeroSceneNode->attachObject(mDummyHero);
        mCamera->setPosition(30.0f,30.0f,-30.0f);
        mCamera->lookAt(0.0f,0.0f,0.0f);
        mCamera->setNearClipDistance(0.1f);
        mCamera->setFOVy(Ogre::Radian(Ogre::Degree(6.5f)));

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
        mFrameNumber = 0.0f;
        mAngle = Ogre::Degree(360);

        mDummyHeroSceneNode->setOrientation(Ogre::Quaternion(Degree(0),Ogre::Vector3::UNIT_Y));

    }
    //-----------------------------------------------------------------------------
    void TestMapModule::update(Ogre::Real deltatime) // This one will be a pain...
    {
        if(!mKernel->getRenderWindow()->isFullScreen())
        {
            mFrameNumber += (1.0f/2.0f) * deltatime;
            if(mFrameNumber >= 1.0f)
            {
            const Ogre::RenderTarget::FrameStats& stats = mKernel->getRenderWindow()->getStatistics();
            mKernel->setWindowCaption("Sonetto - Current FPS: "+Ogre::StringConverter::toString(stats.lastFPS)+" Frame Time: "+Ogre::StringConverter::toString(mKernel->mFrameTime));
            mFrameNumber -= 1.0f;
            }
        }
        switch(mState)
        {
            case 0:
                mKernel->setFadeSpeed(1.0f/0.2f);
                mKernel->startFade(false);
                mState = 1;
            break;
            case 1:
            if( mKernel->getFadeStatus() == FS_IDLE_IN )
                mState = 2;
            break;
            case 2:
            PlayerInput * player = mKernel->getInputMan()->getPlayer(0);
            if(player->getBtnState(BTN_CROSS) == KS_PRESS)
                mKernel->popModule();

            Ogre::Vector2 mov,rot;

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

            Ogre::Vector3 camera_f, camera_r, camera_final;

            camera_f = mCamera->getUp();
            camera_r = mCamera->getRight();

            camera_final.x = (mov.x * camera_r.x) + ((-mov.y) * camera_f.x);
            camera_final.y = 0.0f;
            camera_final.z = (mov.x * camera_r.z) + ((-mov.y) * camera_f.z);

            Ogre::Real str = mov.normalise();

            Ogre::Vector3 src = mDummyHeroSceneNode->getOrientation() * Ogre::Vector3::UNIT_X;      // Orientation from initial direction
            src.y = 0;                                                    // Ignore pitch difference angle
            src.normalise();
            camera_final.normalise( );                     // Both vectors modified so renormalize them
            Ogre::Quaternion quat = src.getRotationTo(camera_final);
            mDummyHeroSceneNode->rotate(quat);

            if(str < 0.3f)
            {
                str = 0.0f;
            } else if(str > 1.0f) {
                str = 1.0f;
            }

            Ogre::Real speed = 5.0f;

            mDummyHeroSceneNode->translate((camera_final * (speed * str)) * deltatime);

            mAngle += Ogre::Radian(rot.y * deltatime);

            Ogre::Vector3 dpos = mDummyHeroSceneNode->getPosition();
            mCamera->setPosition(dpos.x+(Math::Sin(Ogre::Radian(mAngle)))*60.f,dpos.y + 62.f,dpos.z + (Math::Cos(Ogre::Radian(mAngle))*60.0f));
            mCamera->lookAt(dpos.x, dpos.y + 2.0f,dpos.z);

            break;
        }
    }
    //-----------------------------------------------------------------------------
    void TestMapModule::exit()
    {
        // Unload and remove the resource group.
        //mKernel->mResourceMan->destroyResourceGroup(mResGroupName);
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

            //mKernel->mResourceMan->declareResource("map.material", "Material", mResGroupName);
            mKernel->mResourceMan->initialiseResourceGroup(mResGroupName);
            mKernel->mResourceMan->loadResourceGroup(mResGroupName);

            MapFilePtr mapFile = mKernel->mMapFileManager->load("mapdata.dat", mResGroupName);

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
            mDummyHeroSceneNode->setPosition(mKernel->mDatabase->mPlayerPosX,mKernel->mDatabase->mPlayerPosY,mKernel->mDatabase->mPlayerPosZ);
            mDummyHeroSceneNode->setOrientation(mKernel->mDatabase->mPlayerRotW,mKernel->mDatabase->mPlayerRotX,mKernel->mDatabase->mPlayerRotY,mKernel->mDatabase->mPlayerRotZ);
        }

    }

}; // namespace Sonetto
