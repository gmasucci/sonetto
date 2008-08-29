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
#include "SonettoChoiceWindow.h"

namespace Sonetto {
    ChoiceWindow::ChoiceWindow(const Ogre::String& name, Module * module) :
    mStatus(WOS_NOTREADY),
    mpModule(module),
    mWindowName(name),
    mNumChoices(0),
    mCancelID(0),
    mChoice(0),
    mWindowPositionX(0),
    mWindowPositionY(0),
    mWindowWidth(0),
    mWindowHeight(0),
    mTextSize(29.0f/480.0f),
    mWindowAlpha(0.0f),
    mTextAlpha(0.0f),
    mOpenAnimSpeed(1.0f/0.3f),
    mCloseAnimSpeed(1.0f/0.2f),
    mAnimPos(0.0f),
    mCursorPosX(0.0f),
    mCursorPosY(0.0f),
    mCursorAnimSpeed(20.0f),
    mCursorAnimScale(2.5f/480.0f),
    mCursorAnimation(0.0f),
    mCursorDelay(1),
    mCursorDelayTime(0.0f),
    mNormalTextColor(0),
    mSelectedTextColor(1),
    mDisabledTextColor(2),
    mWindowBackground(0)
    {}
    void ChoiceWindow::setPosition(Ogre::Real posX, Ogre::Real posY)
    {
        if(mStatus != WOS_NOTREADY)
            return;
        mWindowPositionX = posX;
        mWindowPositionY = posY;
    }
    void ChoiceWindow::setDimensions(Ogre::Real width, Ogre::Real height)
    {
        if(mStatus != WOS_NOTREADY)
            return;
        mWindowWidth = width;
        mWindowHeight = height;
    }
    void ChoiceWindow::setupChoices(size_t numChoices, size_t cancelID)
    {
        if(mStatus != WOS_NOTREADY)
            return;
        mNumChoices = numChoices;
        mCancelID = cancelID;
        mChoiceMessageID.resize(numChoices, 0);
    }
    void ChoiceWindow::setChoice(size_t choiceID, size_t messageID)
    {
        if(mStatus != WOS_NOTREADY)
            return;
        mChoiceMessageID[choiceID] = messageID;
    }
    void ChoiceWindow::show()
    {
        if(mStatus != WOS_INITIALISED)
            return;

        mWindowBackground->show();
        for(size_t start = 0; start < mChoiceList.size(); ++start)
        {
            mChoiceList[start]->show();
        }
        mStatus = WOS_OPENING;
    }
    void ChoiceWindow::initialise()
    {
        if(mStatus != WOS_NOTREADY)
            return;
        Ogre::OverlayManager * overlayman = mpModule->mKernel->mOverlayMan;
        Ogre::Overlay * moduleoverlay = mpModule->mOverlay;

        mWindowBackground = static_cast<Ogre::OverlayContainer*>(overlayman->createOverlayElement("Window", mWindowName+"_win_bg"));
        mWindowBackground->setPosition(mWindowPositionX, mWindowPositionY);
        mWindowBackground->setDimensions(mWindowWidth, mWindowHeight);
        static_cast<Window*>(mWindowBackground)->setWindowType(WT_SYSTEM);
        static_cast<Window*>(mWindowBackground)->setWindowSkin(mpModule->mKernel->mDatabase->mWindowSkinList[0]);
        static_cast<Window*>(mWindowBackground)->setWindowAlpha(mWindowAlpha);
        static_cast<Window*>(mWindowBackground)->setBorderAlpha(mWindowAlpha);
        // Distance between the text and the border, for now let it be hardcoded.
        Ogre::Real bdistX = 16/480.0f;
        Ogre::Real bdistY = 16/480.0f;

        Ogre::Real curU1, curV1, curU2, curV2 = 0.0f;
        mCursorLine = overlayman->createOverlayElement("Plane", mWindowName+"_cursor_line");
        mCursorLine->setPosition(0.0f,0.0f);
        mCursorLine->setDimensions(mWindowBackground->getWidth() - (bdistX * 2),mTextSize);
        curU1 = mpModule->mKernel->mDatabase->mWindowSkinList[0]->mWinTexCoord.cursor_line.left;
        curU2 = mpModule->mKernel->mDatabase->mWindowSkinList[0]->mWinTexCoord.cursor_line.right;
        curV1 = mpModule->mKernel->mDatabase->mWindowSkinList[0]->mWinTexCoord.cursor_line.top;
        curV2 = mpModule->mKernel->mDatabase->mWindowSkinList[0]->mWinTexCoord.cursor_line.bottom;
        static_cast<Plane*>(mCursorLine)->setUV(curU1,curV1,curU2,curV2);
        static_cast<Plane*>(mCursorLine)->setScrMetricsMode(SMM_RELATIVE_ASPECT_ADJUSTED);
        mCursorLine->setMaterialName(mpModule->mKernel->mDatabase->mWindowSkinList[0]->mCursorLineMaterial->getName());
        mWindowBackground->addChild(mCursorLine);

        mCursorLine->hide();

        for(size_t start = 0; start < mChoiceMessageID.size(); ++start)
        {
            Ogre::OverlayElement * choiceText = overlayman->createOverlayElement("Text", mWindowName+"_"+Ogre::StringConverter::toString(start));
            choiceText->setDimensions(0.0f, 0.0f);
            choiceText->setPosition(bdistX, bdistY + (mTextSize * start));
            static_cast<TextElement*>(choiceText)->setFont(mpModule->mKernel->mDatabase->mGameFont);
            static_cast<TextElement*>(choiceText)->setTextSize(mTextSize);
            static_cast<TextElement*>(choiceText)->setAnimSettings(false, false, false, 0.0f, 0.0f);
            static_cast<TextElement*>(choiceText)->forceAnimReset();
            static_cast<TextElement*>(choiceText)->setMessage(mpModule->mKernel->mDatabase->mSystemMessage->getMessage(mChoiceMessageID[start]));
            static_cast<TextElement*>(choiceText)->setAlpha(mTextAlpha);
            mWindowBackground->addChild(choiceText);
            mChoiceList.push_back(choiceText);
        }

        mCursor = overlayman->createOverlayElement("Plane", mWindowName+"_cursor");
        mCursor->setPosition(0.0f,0.0f);
        mCursor->setDimensions(mTextSize,mTextSize);
        curU1 = mpModule->mKernel->mDatabase->mWindowSkinList[0]->mWinTexCoord.cursor.left;
        curU2 = mpModule->mKernel->mDatabase->mWindowSkinList[0]->mWinTexCoord.cursor.right;
        curV1 = mpModule->mKernel->mDatabase->mWindowSkinList[0]->mWinTexCoord.cursor.top;
        curV2 = mpModule->mKernel->mDatabase->mWindowSkinList[0]->mWinTexCoord.cursor.bottom;
        static_cast<Plane*>(mCursor)->setUV(curU1,curV1,curU2,curV2);
        static_cast<Plane*>(mCursor)->setScrMetricsMode(SMM_RELATIVE_ASPECT_ADJUSTED);
        mCursor->setMaterialName(mpModule->mKernel->mDatabase->mWindowSkinList[0]->mCursorMaterial->getName());

        mWindowBackground->addChild(mCursor);

        mCursor->hide();

        moduleoverlay->add2D(mWindowBackground);

        mStatus = WOS_INITIALISED;
    }
    void ChoiceWindow::deinitialise()
    {
        if(mStatus != WOS_RETURN_INPUT)
            return;

        for(size_t start = 0; start < mChoiceList.size(); ++start)
        {
            mWindowBackground->_removeChild(mChoiceList[start]);
            mpModule->mKernel->mOverlayMan->destroyOverlayElement(mChoiceList[start]);
        }

        mWindowBackground->_removeChild(mCursor);
        mpModule->mKernel->mOverlayMan->destroyOverlayElement(mCursor);

        mWindowBackground->_removeChild(mCursorLine);
        mpModule->mKernel->mOverlayMan->destroyOverlayElement(mCursorLine);

        mChoiceList.clear();
        mChoiceMessageID.clear();

        mpModule->mOverlay->remove2D(mWindowBackground);
        mpModule->mKernel->mOverlayMan->destroyOverlayElement(mWindowBackground);

        mStatus = WOS_DEINITIALISED;
    }
    void ChoiceWindow::update(float deltatime)
    {
        if((mStatus == WOS_NOTREADY)||(mStatus == WOS_DEINITIALISED)||(mStatus == WOS_RETURN_INPUT))
            return;

        switch (mStatus)
        {
            case WOS_OPENING:
                {
                    if(mAnimPos > 1.0f)
                    {
                        mAnimPos = 1.0f;
                        mWindowBackground->setPosition(mWindowPositionX, mWindowPositionY);
                        mWindowBackground->setDimensions(mWindowWidth, mWindowHeight);
                        static_cast<Window*>(mWindowBackground)->setWindowAlpha(1.0f);
                        static_cast<Window*>(mWindowBackground)->setBorderAlpha(1.0f);
                        for(size_t start = 0; start < mChoiceList.size(); ++start)
                        {
                            static_cast<TextElement*>(mChoiceList[start])->setAlpha(1.0f);
                        }
                        mCursor->show();
                        mCursorLine->show();
                        Ogre::Real curLeft = static_cast<TextElement*>(mChoiceList[0])->_getLeft();
                        Ogre::Real curTop = static_cast<TextElement*>(mChoiceList[0])->_getTop();
                        mCursor->setPosition(curLeft-mTextSize,curTop);
                        mCursorPosX = curLeft-mTextSize;
                        mCursorPosY = curTop;
                        mCursorLine->setPosition(mCursorLine->getLeft(),curTop);
                        static_cast<TextElement*>(mChoiceList[0])->setColor(mSelectedTextColor);
                        mStatus = WOS_WAITING;
                        break;
                    }

                    Ogre::Real winAnimPos = mAnimPos * 2.0f;
                    Ogre::Real txtAnimPos = mAnimPos * 2.0f - 1.0f;

                    if(winAnimPos > 1.0f)
                        winAnimPos = 1.0f;

                    if(txtAnimPos > 1.0f)
                        txtAnimPos = 1.0f;

                    if(txtAnimPos < 0.0f)
                        txtAnimPos = 0.0f;

                    Ogre::Real winHalfWidth = mWindowWidth / 2.0f;

                    mWindowBackground->setPosition(lerp(mWindowPositionX+winHalfWidth,mWindowPositionX,winAnimPos), mWindowPositionY);
                    mWindowBackground->setDimensions(lerp(0,mWindowWidth,winAnimPos), mWindowHeight);
                    static_cast<Window*>(mWindowBackground)->setWindowAlpha(winAnimPos);
                    static_cast<Window*>(mWindowBackground)->setBorderAlpha(winAnimPos);
                    for(size_t start = 0; start < mChoiceList.size(); ++start)
                    {
                        static_cast<TextElement*>(mChoiceList[start])->setAlpha(txtAnimPos);
                    }
                    mWindowAlpha = winAnimPos;
                    mTextAlpha = txtAnimPos;
                    mAnimPos += mOpenAnimSpeed * deltatime;
                }
            break;
            case WOS_WAITING:
                {
                    PlayerInput *player = mpModule->mKernel->getInputMan()->getPlayer(0);
                    Ogre::Vector2 plInputAxis = player->getAxisValue(Sonetto::AX_LEFT);

                    if(player->getBtnState(Sonetto::BTN_DPAD_UP) == Sonetto::KS_PRESS)
                        --mChoice;

                    if(player->getBtnState(Sonetto::BTN_DPAD_DOWN) == Sonetto::KS_PRESS)
                        ++mChoice;

                    if(player->getBtnState(Sonetto::BTN_CIRCLE) == Sonetto::KS_PRESS)
                        mStatus = WOS_CLOSING;

                    if(player->getBtnState(Sonetto::BTN_CROSS) == Sonetto::KS_PRESS)
                    {
                        mChoice = mCancelID - 1;
                        mStatus = WOS_CLOSING;
                    }

                    if(mChoice < 0)
                        mChoice = mNumChoices - 1;

                    if(mChoice >= mNumChoices)
                        mChoice = 0;

                    mCursorPosY = static_cast<TextElement*>(mChoiceList[mChoice])->_getTop();
                    for(size_t start = 0; start < mChoiceList.size(); ++start)
                    {
                        if(start == mChoice)
                        {
                            static_cast<TextElement*>(mChoiceList[start])->setColor(mSelectedTextColor);
                        } else {
                            static_cast<TextElement*>(mChoiceList[start])->setColor(mNormalTextColor);
                        }
                    }
                    Ogre::Real curNewPos = 0.0f;
                    mCursorAnimation += mCursorAnimSpeed * deltatime;
                    curNewPos = Ogre::Math::Sin(mCursorAnimation);
                    mCursor->setPosition(mCursorPosX+(curNewPos*mCursorAnimScale), mCursorPosY);
                    mCursorLine->setPosition(mCursorLine->getLeft(), mCursorPosY);
                }
            break;
            case WOS_CLOSING:
                {
                    if(mAnimPos <= 0.0f)
                    {
                        mAnimPos = 0.0f;
                        mStatus = WOS_RETURN_INPUT;
                        break;
                    }
                    static_cast<Window*>(mWindowBackground)->setWindowAlpha(mAnimPos);
                    static_cast<Window*>(mWindowBackground)->setBorderAlpha(mAnimPos);
                    for(size_t start = 0; start < mChoiceList.size(); ++start)
                    {
                        static_cast<TextElement*>(mChoiceList[start])->setAlpha(mAnimPos);
                    }
                    mWindowAlpha = mAnimPos;
                    mTextAlpha = mAnimPos;
                    mAnimPos -= mCloseAnimSpeed * deltatime;
                }
            break;
            case WOS_RETURN_INPUT:
            break;
            case WOS_DEINITIALISED:
            break;
            default:
            break;
        }
    }
    WndObjStatus ChoiceWindow::getStatus()
    {
        return mStatus;
    }
    size_t ChoiceWindow::getChoiceResult()
    {
        return mChoice;
    }
}; // namespace
