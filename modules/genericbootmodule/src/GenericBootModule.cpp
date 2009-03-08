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

#include <SDL/SDL.h>
#include "SonettoInputManager.h"
#include "GenericBootModule.h"

#include "SonettoPrerequisites.h"
#include "SonettoKernel.h"
#include "SonettoFontManager.h"
#include "SonettoFont.h"
#include "SonettoFontSerializer.h"
#include "SonettoStaticTextElement.h"

namespace BootModule
{
    // ----------------------------------------------------------------------
    // BootModule::GenericBootModule implementation
    // ----------------------------------------------------------------------
    GenericBootModule::GenericBootModule() : BootModule::BootModule()
    {}
    // ----------------------------------------------------------------------
    GenericBootModule::~GenericBootModule() {}
    // ----------------------------------------------------------------------
    void GenericBootModule::initialize()
    {
        BootModule::initialize();
        setBgColor(Ogre::ColourValue(16.0f/255.0f,16.0f/255.0f,235.0f/255.0f));

        //ManualFontLoader * fontloader = new ManualFontLoader();
        Ogre::ResourceGroupManager::getSingleton().createResourceGroup("TEMP");
        //Ogre::ResourceGroupManager::getSingleton ().declareResource("font.bin", "SFont", "TEMP", fontloader);
        Ogre::ResourceGroupManager::getSingleton ().addResourceLocation("font","FileSystem","TEMP");
        Ogre::ResourceGroupManager::getSingleton ().declareResource("baar_sophia.sfn", "SFont", "TEMP");
        Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("TEMP");
        //Sonetto::FontPtr afont = Sonetto::Kernel::getSingletonPtr()->mFontMan->load("font.bin", "TEMP");
        //Sonetto::FontSerializer serializer;
        //serializer.exportFont((Sonetto::Font*)afont.getPointer(), "font/baar_sophia.sfn");
        Sonetto::FontPtr font = Sonetto::Kernel::getSingletonPtr()->mFontMan->load("baar_sophia.sfn", "TEMP");
/*
        Sonetto::Kernel::getSingleton().mDatabase.system.defaultFont = font.getPointer();
        Sonetto::Kernel::getSingleton().mDatabase.system.textSize = 0.05f;
        Sonetto::Kernel::getSingleton().mDatabase.system.textAnimationSpeed = 1.0f;
        Sonetto::Kernel::getSingleton().mDatabase.system.textFadeInSpeed = 1.0f;
        Sonetto::Kernel::getSingleton().mDatabase.system.textFadeOutSpeed = 1.0f;
        Sonetto::Kernel::getSingleton().mDatabase.system.textHorizontalScale = 1.0f;
        Sonetto::Kernel::getSingleton().mDatabase.system.textVerticalSpacing = 1.0f;
        Sonetto::Kernel::getSingleton().mDatabase.system.defaultColor = 0;
*/
/*
        Ogre::ResourceGroupManager::getSingleton().createResourceGroup("TEMP");
        Ogre::ResourceGroupManager::getSingleton().addResourceLocation("temp","FileSystem","TEMP");
        Ogre::ResourceGroupManager::getSingleton ().declareResource("baar_sophia.material", "Material", "TEMP");
        Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("TEMP");*/

        Ogre::ResourceGroupManager::getSingleton().createResourceGroup("DEVDATA");
        Ogre::ResourceGroupManager::getSingleton ().addResourceLocation("dev_data","FileSystem","DEVDATA");
        Ogre::ResourceGroupManager::getSingleton ().declareResource("Arrow.material", "Material", "DEVDATA");
        Ogre::ResourceGroupManager::getSingleton ().declareResource("arrow_model.mesh", "Mesh", "DEVDATA");
        Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("DEVDATA");
        Ogre::ResourceGroupManager::getSingleton().loadResourceGroup("DEVDATA");

        Ogre::OverlayContainer * lOverlayContainer = static_cast<Ogre::OverlayContainer*>(Sonetto::Kernel::getSingleton().mOverlayMan->createOverlayElement("Panel","DbgContainer"));
        lOverlayContainer->setPosition(-(80.0f/480.0f),0.0f);
        lOverlayContainer->setDimensions(1.0f,1.0f);

        mDebugText = static_cast<Sonetto::StaticTextElement*>(Sonetto::Kernel::getSingleton().mOverlayMan->createOverlayElement("StaticText", "DbgOverlay"));
        mDebugText->mpFont = font.getPointer();
        //mDebugText->setMaterialName(font->mMaterial->getName());
        mDebugText->setDimensions(1.0, 1.0);
        mDebugText->setPosition(20.0f/480.0f,20.0f/480.0f);
        mDebugText->mText = std::string("Prism Barrier\nRaging Beast\nWater Laser\n\x1B\x80\x1Red\x1B\x80\x2 Green\x1B\x80\x3 Blue\x1B\x81 Color Reset.\n\x1B\x82\xFF\xFF\xFF-Custom RGB Text Color!\nAll hail SATIN 666");

        mAxisInfoText = static_cast<Sonetto::StaticTextElement*>(Sonetto::Kernel::getSingleton().mOverlayMan->createOverlayElement("StaticText", "AxisInfoOverlay"));
        mAxisInfoText->mpFont = font.getPointer();
        //mDebugText->setMaterialName(font->mMaterial->getName());
        mAxisInfoText->setDimensions(1.0, 1.0);
        mAxisInfoText->setPosition(20.0f/480.0f,400.0f/480.0f);
        mAxisInfoText->mText = std::string("Awaiting Initialisation...");



        mOverlay->add2D(lOverlayContainer);
        lOverlayContainer->addChild(mDebugText);
        mDebugText->hide();

        lOverlayContainer->addChild(mAxisInfoText);
        mAxisInfoText->hide();

        Sonetto::InputManager &inputMan =
                Sonetto::InputManager::getSingleton();

        inputMan.getPlayer(0)->configBtn(Sonetto::BTN_CROSS,Sonetto::InputSource(
                true,Sonetto::InputSource::IST_KEY,SDLK_x));

        /*inputMan.getPlayer(0)->configAxis(Sonetto::AXE_LEFT_UP,Sonetto::InputSource(
                true,Sonetto::InputSource::IST_KEY,SDLK_s));
        inputMan.getPlayer(0)->configAxis(Sonetto::AXE_LEFT_DOWN,Sonetto::InputSource(
                true,Sonetto::InputSource::IST_KEY,SDLK_w));
        inputMan.getPlayer(0)->configAxis(Sonetto::AXE_LEFT_LEFT,Sonetto::InputSource(
                true,Sonetto::InputSource::IST_KEY,SDLK_a));
        inputMan.getPlayer(0)->configAxis(Sonetto::AXE_LEFT_RIGHT,Sonetto::InputSource(
                true,Sonetto::InputSource::IST_KEY,SDLK_d));*/
        inputMan.getPlayer(0)->configAxis(Sonetto::AX_LEFT,Sonetto::InputSource(
                true,Sonetto::InputSource::IST_AXIS,Sonetto::AX_LEFT,Sonetto::InputSource::INV_Y_POLARITY));

        inputMan.getPlayer(0)->setJoystick(1);


        inputMan.getPlayer(0)->setEnabled(true);
        std::cout << "Press [Cross] to show the debug text\n";

        mpEntity = mSceneMan->createEntity("ArrowEnt", "arrow_model.mesh");
        mpEntityNode = mSceneMan->getRootSceneNode()->createChildSceneNode();
        mpEntityNode->attachObject(mpEntity);

        mSceneMan->setAmbientLight(Ogre::ColourValue(0.5f,0.5f,0.5f));

        Ogre::Light * mpLight = mSceneMan->createLight("StdDirLt0");

        mCamera->setPosition(-20.0f,100.0f,20.0f);
        mCamera->lookAt(0.0f,0.0f,0.0f);
        mCamera->setNearClipDistance(1.0f);

        mpLight->setType(Ogre::Light::LT_DIRECTIONAL);
        mpLight->setDiffuseColour(1.0f,1.0f,1.0f);
        mpLight->setDirection(mCamera->getDirection());

        mCamera->setPosition(-20.0f,30.0f,20.0f);
        mCamera->lookAt(0.0f,0.0f,0.0f);


        //mDebugText->show();
    }
    // ----------------------------------------------------------------------
    void GenericBootModule::update()
    {
        Sonetto::PlayerInput *p0 = Sonetto::InputManager::getSingleton().
                getPlayer(0);

        if (p0->getBtnState(Sonetto::BTN_CROSS) == Sonetto::KS_RELEASE)
        {
            if(!mDebugText->isVisible())
            {
                mDebugText->show();
                mAxisInfoText->show();
            }
            else
            {
                mDebugText->hide();
                mAxisInfoText->hide();
            }
        }

        Ogre::Vector2 pl0axis = p0->getAxisValue(Sonetto::AX_LEFT);

        mAxisInfoText->mText = std::string("Axis Input Values\nX: "+
        Ogre::StringConverter::toString(pl0axis.x)
        +"\nY: "+
        Ogre::StringConverter::toString(pl0axis.y)
        +"");

        Ogre::Vector3 camDir = mCamera->getDirection();
        Ogre::Vector3 camDirRight(camDir.z,0.0f,-camDir.x);

        float pl0axismag = pl0axis.normalise();

        Ogre::Vector3 directionFinal(   ((-pl0axis.x) * camDir.z) + (pl0axis.y * camDir.x),
                                        0.0f,
                                        ((-pl0axis.x) * (-camDir.x)) + (pl0axis.y * camDir.z));

        directionFinal.normalise();

        Ogre::Quaternion destOrientation = (mpEntityNode->getOrientation() * Ogre::Vector3::UNIT_Z).getRotationTo(directionFinal);
        destOrientation = destOrientation * mpEntityNode->getOrientation();

        Ogre::Quaternion finalOrientation = Ogre::Quaternion::nlerp(15.0f*Sonetto::Kernel::getSingleton().getFrameTime(),mpEntityNode->getOrientation(),destOrientation);

        if(pl0axismag > 0.3f)
        {
            float tmag = pl0axismag;
            tmag -= 0.3f;
            tmag /= 0.7;
        mpEntityNode->setOrientation(finalOrientation);
        mpEntityNode->translate((directionFinal*tmag*70.0f*Sonetto::Kernel::getSingleton().getFrameTime()));
        }

        mDebugText->mText = std::string("Debug\nFrame Time: "+
        Ogre::StringConverter::toString(Sonetto::Kernel::getSingleton().getFrameTime())+"\n"
        "Frames Per Second: "+Ogre::StringConverter::toString(Sonetto::Kernel::getSingleton().getRenderWindow()->getLastFPS())+"");

        BootModule::update();
    }
    // ----------------------------------------------------------------------
    void GenericBootModule::deinitialize()
    {
        BootModule::deinitialize();
    }
    // ----------------------------------------------------------------------
    void GenericBootModule::halt()
    {
        BootModule::halt();
    }
    // ----------------------------------------------------------------------
    void GenericBootModule::resume()
    {
        BootModule::resume();
    }
} // namespace
