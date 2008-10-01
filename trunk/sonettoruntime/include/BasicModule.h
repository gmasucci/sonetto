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

#ifndef BASICMODULE_H
#define BASICMODULE_H

#include <cstdio>
#include "SonettoChoiceWindow.h"
#include "SonettoModule.h"
#include "SonettoInputManager.h"

class BootModule : public Sonetto::Module
{
    public:
    enum BootModuleStatus
    {
        BOS_LOGO_A_IN_START,
        BOS_LOGO_A_IN,
        BOS_LOGO_A_WAIT,
        BOS_LOGO_A_OUT_START,
        BOS_LOGO_A_OUT,
        BOS_LOGO_B_IN_START,
        BOS_LOGO_B_IN,
        BOS_LOGO_B_WAIT,
        BOS_LOGO_B_OUT_START,
        BOS_LOGO_B_OUT
    };

    BootModule() : Module()
    {
        mModuleOverlayName = "BOOT_OVERLAY";
        mCurrentWaitTime = 3.0f;
        mWaitTime = 3.0f;
    }
    void enter()
    {
        mBackgroundColor = Ogre::ColourValue(0.0f,0.0f,0.0f,1.0f);
        Module::enter();

        mKernel->mResourceMan->addResourceLocation("boot/boot.zip", "Zip", "BOOTDATA");
        mKernel->mResourceMan->initialiseResourceGroup("BOOTDATA");

        mOverlayContainer = static_cast<Ogre::OverlayContainer*>(mKernel->mOverlayMan->createOverlayElement("Plane", "BOOT_LOGO_PLANE"));
        mOverlayContainer->setPosition(128.0f/768.0f,320.0f/768.0f);
        mOverlayContainer->setDimensions(512.0f/768.0f,128.0f/768.0f);
        mOverlayContainer->setMaterialName("BOOT_00");
        mOverlayContainer->setColour(Ogre::ColourValue(1.0f,1.0f,1.0f,1.0f));

        static_cast<Sonetto::Plane*>(mOverlayContainer)->setScrMetricsMode(Sonetto::SMM_RELATIVE_ASPECT_ADJUSTED);
        mOverlay->add2D(mOverlayContainer);
        mOverlay->show();

        mStatus = BOS_LOGO_A_IN_START;
    }

    void update(Ogre::Real deltatime)
    {
        Sonetto::PlayerInput *player = mKernel->getInputMan()->getPlayer(0);
        switch (mStatus)
        {
            case BOS_LOGO_A_IN_START:
                mCurrentWaitTime = mWaitTime;
                mOverlayContainer->setPosition(128.0f/768.0f,320.0f/768.0f);
                mOverlayContainer->setDimensions(512.0f/768.0f,128.0f/768.0f);
                mOverlayContainer->setMaterialName("BOOT_00");
                mKernel->setFadeSpeed(1.0f/0.5f);
                mKernel->startFade(false);
                mStatus = BOS_LOGO_A_IN;
            break;
            case BOS_LOGO_A_IN:
                if( mKernel->getFadeStatus() == Sonetto::FS_IDLE_IN )
                    mStatus = BOS_LOGO_A_WAIT;
            break;
            case BOS_LOGO_A_WAIT:
                mCurrentWaitTime -= deltatime;
                if((mCurrentWaitTime < 0.0f) ||
                (player->getBtnState(Sonetto::BTN_START) == Sonetto::KS_PRESS) ||
                (player->getBtnState(Sonetto::BTN_SELECT) == Sonetto::KS_PRESS) ||
                (player->getBtnState(Sonetto::BTN_CROSS) == Sonetto::KS_PRESS) ||
                (player->getBtnState(Sonetto::BTN_CIRCLE) == Sonetto::KS_PRESS) ||
                (player->getBtnState(Sonetto::BTN_SQUARE) == Sonetto::KS_PRESS) ||
                (player->getBtnState(Sonetto::BTN_TRIANGLE) == Sonetto::KS_PRESS))
                {
                    mStatus = BOS_LOGO_A_OUT_START;
                    mCurrentWaitTime = mWaitTime;
                }
            break;
            case BOS_LOGO_A_OUT_START:
                mKernel->setFadeSpeed(1.0f/0.5f);
                mKernel->startFade(true);
                mStatus = BOS_LOGO_A_OUT;
            break;
            case BOS_LOGO_A_OUT:
                if( mKernel->getFadeStatus() == Sonetto::FS_IDLE_OUT )
                    mStatus = BOS_LOGO_B_IN_START;
            break;
            case BOS_LOGO_B_IN_START:
                mOverlayContainer->setPosition(128.0f/768.0f,256.0f/768.0f);
                mOverlayContainer->setDimensions(512.0f/768.0f,256.0f/768.0f);
                mOverlayContainer->setMaterialName("BOOT_01");
                mKernel->setFadeSpeed(1.0f/0.5f);
                mKernel->startFade(false);
                mStatus = BOS_LOGO_B_IN;
            break;
            case BOS_LOGO_B_IN:
                if( mKernel->getFadeStatus() == Sonetto::FS_IDLE_IN )
                    mStatus = BOS_LOGO_B_WAIT;
            break;
            case BOS_LOGO_B_WAIT:
            mCurrentWaitTime -= deltatime;
                if((mCurrentWaitTime < 0.0f) ||
                (player->getBtnState(Sonetto::BTN_START) == Sonetto::KS_PRESS) ||
                (player->getBtnState(Sonetto::BTN_SELECT) == Sonetto::KS_PRESS) ||
                (player->getBtnState(Sonetto::BTN_CROSS) == Sonetto::KS_PRESS) ||
                (player->getBtnState(Sonetto::BTN_CIRCLE) == Sonetto::KS_PRESS) ||
                (player->getBtnState(Sonetto::BTN_SQUARE) == Sonetto::KS_PRESS) ||
                (player->getBtnState(Sonetto::BTN_TRIANGLE) == Sonetto::KS_PRESS))
                {
                    mStatus = BOS_LOGO_B_OUT_START;
                    mCurrentWaitTime = mWaitTime;
                }
            break;
            case BOS_LOGO_B_OUT_START:
                mKernel->setFadeSpeed(1.0f/0.5f);
                mKernel->startFade(true);
                mStatus = BOS_LOGO_B_OUT;
            break;
            case BOS_LOGO_B_OUT:
                if( mKernel->getFadeStatus() == Sonetto::FS_IDLE_OUT )
                {
                    mKernel->popModule();
                }
            break;
            default:
            break;
        }
    }

    void exit()
    {
        mOverlay->remove2D(mOverlayContainer);
        mKernel->mOverlayMan->destroyOverlayElement(mOverlayContainer);
        mKernel->mResourceMan->unloadResourceGroup("BOOTDATA");
        mKernel->mResourceMan->destroyResourceGroup("BOOTDATA");

        Module::exit();
    }
    void wakeup() { Module::wakeup(); }

    Ogre::OverlayContainer * mOverlayContainer;

    BootModuleStatus mStatus;
    float mWaitTime;
    float mCurrentWaitTime;
};

class BasicModule : public Sonetto::Module
{
    public:

        enum BasicModuleStatus
        {
            BMS_FADEIN_START,
            BMS_FADEIN,
            BMS_FADEOUT_START,
            BMS_FADEOUT,
            BMS_UPDATE
        };

        BasicModule() : Module()
        {
            mModuleOverlayName = "BASIC_OVERLAY";
            mCaptureFileName = "ScreenOut";
        }
        void enter();

        void update(Ogre::Real deltatime);

        void exit()   { Module::exit();   }
        void wakeup() { Module::wakeup(); }

        Ogre::OverlayContainer *mOverlayContainer;
        std::vector<Ogre::OverlayElement*> mOverlayElement;
        BasicModuleStatus mStatus;

        Sonetto::ChoiceWindow * mChoiceWindow;

        Ogre::String mCaptureFileName;

        int mChoiceState;

        bool mChoiceHasShown;
};

#endif
