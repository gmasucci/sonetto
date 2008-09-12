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

#include <Ogre.h>
#include "SonettoInputManager.h"
#include "BasicModule.h"
#include "TESTMapModule.h"

using namespace Ogre;
using namespace Sonetto;

void BasicModule::enter()
{
    mStatus = BMS_FADEIN_START;
    PlayerInput *player;
    // Set a custom color for the viewport background.
    // Let's select a nice background colour for our viewport
    mBackgroundColor = Ogre::ColourValue(0.01f,0.01f,0.01f,1.0f);
    Module::enter();
    // And setup the first controller
    player = mKernel->getInputMan()->getPlayer(0);

    player->setJoystick(mKernel->getInputMan()->getJoystick(1));
    player->configAxis(Sonetto::AX_LEFT,Sonetto::InputSource(true,Sonetto::InputSource::IST_AXIS,Sonetto::AX_LEFT));
    player->configAxis(Sonetto::AX_RIGHT,Sonetto::InputSource(true,Sonetto::InputSource::IST_AXIS,Sonetto::AX_RIGHT,Sonetto::InputSource::INV_ORDER));
    player->configBtn(Sonetto::BTN_DPAD_UP,Sonetto::InputSource(true,Sonetto::InputSource::IST_BUTTON,12));
    player->configBtn(Sonetto::BTN_DPAD_DOWN,Sonetto::InputSource(true,Sonetto::InputSource::IST_BUTTON,14));
    player->configBtn(Sonetto::BTN_CROSS,Sonetto::InputSource(true,Sonetto::InputSource::IST_BUTTON,2));
    player->configBtn(Sonetto::BTN_CIRCLE,Sonetto::InputSource(true,Sonetto::InputSource::IST_BUTTON,1));
    player->configBtn(Sonetto::BTN_TRIANGLE,Sonetto::InputSource(true,Sonetto::InputSource::IST_BUTTON,4));
    player->configBtn(Sonetto::BTN_SQUARE,Sonetto::InputSource(true,Sonetto::InputSource::IST_BUTTON,3));

    /*
    player->configAxis(Sonetto::AXE_LEFT_UP, Sonetto::InputSource(true, Sonetto::InputSource::IST_KEY, SDLK_w));
    player->configAxis(Sonetto::AXE_LEFT_DOWN, Sonetto::InputSource(true, Sonetto::InputSource::IST_KEY, SDLK_s));
    player->configAxis(Sonetto::AXE_LEFT_LEFT, Sonetto::InputSource(true, Sonetto::InputSource::IST_KEY, SDLK_a));
    player->configAxis(Sonetto::AXE_LEFT_RIGHT, Sonetto::InputSource(true, Sonetto::InputSource::IST_KEY, SDLK_d));

    player->configAxis(Sonetto::AXE_RIGHT_UP, Sonetto::InputSource(true, Sonetto::InputSource::IST_KEY, SDLK_UP));
    player->configAxis(Sonetto::AXE_RIGHT_DOWN, Sonetto::InputSource(true, Sonetto::InputSource::IST_KEY, SDLK_DOWN));
    player->configAxis(Sonetto::AXE_RIGHT_LEFT, Sonetto::InputSource(true, Sonetto::InputSource::IST_KEY, SDLK_LEFT));
    player->configAxis(Sonetto::AXE_RIGHT_RIGHT, Sonetto::InputSource(true, Sonetto::InputSource::IST_KEY, SDLK_RIGHT));

    player->configBtn(Sonetto::BTN_CROSS,Sonetto::InputSource(true,Sonetto::InputSource::IST_KEY,SDLK_z));
    player->configBtn(Sonetto::BTN_CIRCLE,Sonetto::InputSource(true,Sonetto::InputSource::IST_KEY,SDLK_x));
    */
    player->setEnabled(true);

    mOverlayContainer = static_cast<OverlayContainer*>(mKernel->mOverlayMan->createOverlayElement("Window","text_01_01"));
    static_cast<Window*>(mOverlayContainer)->setWindowType(WT_MENU);
    mOverlayContainer->setPosition(-(64/480.0f),(16/480.0f));
    mOverlayContainer->setDimensions(1.0f,64/480.0f);
    static_cast<Window*>(mOverlayContainer)->setWindowSkin(mKernel->mDatabase->mWindowSkinList[0]);

    Ogre::OverlayContainer * mOverlayContainer2 = static_cast<OverlayContainer*>(mKernel->mOverlayMan->createOverlayElement("Window","text_01_02"));
    static_cast<Window*>(mOverlayContainer2)->setWindowType(WT_MENU);
    mOverlayContainer2->setPosition(-(64/480.0f),(324/480.0f));
    mOverlayContainer2->setDimensions((608/480.0f),140/480.0f);
    static_cast<Window*>(mOverlayContainer2)->setWindowSkin(mKernel->mDatabase->mWindowSkinList[0]);

    //WindowSkinPtr activewskin = mKernel->mWindowSkinManager->load("windowskin_00.bin", "TEMP");

    Ogre::OverlayElement * text0 = mKernel->mOverlayMan->createOverlayElement("Text","text_00_00");
    static_cast<TextElement*>(text0)->setFont(mKernel->mDatabase->mGameFont);
    mOverlayContainer->addChild(text0);
    text0->setDimensions(1.0, 1.0);
    text0->setPosition(16/480.0f,16/480.0f);
    static_cast<TextElement*>(text0)->setTextSize(0.07f);
    static_cast<TextElement*>(text0)->setAnimSettings(false, false, false, 0.0f, 0.0f);
    static_cast<TextElement*>(text0)->forceAnimReset();
    static_cast<TextElement*>(text0)->setMessage(mKernel->mDatabase->mSystemMessage->getMessage(0));

    Ogre::OverlayElement * text1 = mKernel->mOverlayMan->createOverlayElement("Text","text_00_01");
    static_cast<TextElement*>(text1)->setFont(mKernel->mDatabase->mGameFont);
    mOverlayContainer2->addChild(text1);
    text1->setDimensions(1.0, 1.0);
    text1->setPosition(16/480.0f,16/480.0f);
    static_cast<TextElement*>(text1)->setTextSize(0.05f);
    static_cast<TextElement*>(text1)->setAnimSettings(false, false, false, 0.0f, 0.0f);
    static_cast<TextElement*>(text1)->forceAnimReset();
    static_cast<TextElement*>(text1)->setMessage(mKernel->mDatabase->mSystemMessage->getMessage(1));

    mOverlay->add2D(mOverlayContainer);
    mOverlay->add2D(mOverlayContainer2);
    mOverlayContainer->show();
    mOverlayContainer2->show();
    text0->show();
    text1->show();

    mChoiceHasShown = false;

}

void BasicModule::update(Ogre::Real deltatime)
{
    PlayerInput *player = mKernel->getInputMan()->getPlayer(0);
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
    switch(mStatus)
    {
        case BMS_FADEIN_START:
            mKernel->setFadeSpeed(1.0f/0.5f);
            mKernel->startFade(false);
            mStatus = BMS_FADEIN;
        break;
        case BMS_FADEIN:
            if( mKernel->getFadeStatus() == FS_IDLE_IN )
                mStatus = BMS_UPDATE;
        break;
        case BMS_FADEOUT_START:
            mKernel->setFadeSpeed(1.0f/0.5f);
            mKernel->startFade(true);
            mStatus = BMS_FADEOUT;
        break;
        case BMS_FADEOUT:
            if( mKernel->getFadeStatus() == FS_IDLE_OUT )
            {
                switch(mChoiceState)
                {
                    case 0:
                        mChoiceHasShown = false;
                        mStatus = BMS_FADEIN_START;
                        mKernel->pushModule(new TestMapModule(), true);
                    break;
                    case 1:
                        mKernel->shutdown();
                    break;
                    case 2:
                        mKernel->shutdown();
                    break;
                    case 3:
                        mKernel->shutdown();
                    break;
                    case 4:
                        mKernel->shutdown();
                    break;
                }
            }


        break;
        case BMS_UPDATE:

            // Axes test
            //std::cout << player->getAxisValue(AX_LEFT) << " / " << player->getAxisValue(AX_RIGHT) << "\n";
            if((!mChoiceWindow) && (!mChoiceHasShown))
            {
                mChoiceWindow = new ChoiceWindow("test_choicewindow",static_cast<Module*>(this));
                mChoiceWindow->setPosition(95.0f/480.0f,140.0f/480.0f);
                mChoiceWindow->setDimensions(290.0f/480.0f,180.0f/480.0f);
                mChoiceWindow->setupChoices(5,5);
                mChoiceWindow->setChoice(0,2);
                mChoiceWindow->setChoice(1,3);
                mChoiceWindow->setChoice(2,4);
                mChoiceWindow->setChoice(3,5);
                mChoiceWindow->setChoice(4,6);
                mChoiceWindow->initialise();
                mChoiceHasShown = true;
            } else {
            switch(mChoiceWindow->getStatus())
            {
                case WOS_INITIALISED:
                    mChoiceWindow->show();
                break;
                case WOS_RETURN_INPUT:
                    mChoiceState = mChoiceWindow->getChoiceResult();
                    mChoiceWindow->deinitialise();
                    delete mChoiceWindow;
                    mChoiceWindow = NULL;
                    mStatus = BMS_FADEOUT_START;
                break;
                default:
                break;
            }
            if(mChoiceWindow)
                mChoiceWindow->update(deltatime);
            }
        break;
    }
}
