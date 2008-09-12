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
#ifndef SONETTO_CHOICEWINDOW_H
#define SONETTO_CHOICEWINDOW_H

#include "SonettoModule.h"
#include "SonettoWindow.h"
#include "SonettoTextElement.h"
#include "SonettoMain.h"
#include "SonettoWindowSkin.h"

namespace Sonetto {

    enum WndObjStatus
    {
        /// The Window is not ready to show on screen. (After setting it up, call initialise())
        WOS_NOTREADY,
        /// The Window is initialised and ready to show.
        WOS_INITIALISED,
        /// The Window is showing its opening animation.
        WOS_OPENING,
        /// The Window is awaiting for player input.
        WOS_WAITING,
        /// The Window is showing its closing animation.
        WOS_CLOSING,
        /** This indicates the player has already done his choice and it's ready to return it.
        @remarks
            Use ChoiceWindow::getChoiceResult() to check the choice the player made,
            After getting the input, you must get rid of the window, then
            call ChoiceWindow::deinitialise(); and destroy the object.
        */
        WOS_RETURN_INPUT,
        /// This indicates the windows has already been deinitialised, so you must destroy it.
        WOS_DEINITIALISED
    };

    enum CursorState
    {
        CUR_IDLE,
        CUR_MOVE_UP_START,
        CUR_MOVE_UP_WAIT,
        CUR_MOVE_UP,
        CUR_MOVE_UP_HOLD,
        CUR_MOVE_DOWN_START,
        CUR_MOVE_DOWN_WAIT,
        CUR_MOVE_DOWN,
        CUR_MOVE_DOWN_HOLD
    };

    class SONETTO_EXPORT ChoiceWindow
    {
    public:
        ChoiceWindow(const Ogre::String& name, Module * module);
        ~ChoiceWindow()
        { mpModule = 0; }
        void setPosition(Ogre::Real posX, Ogre::Real posY);
        void setDimensions(Ogre::Real width, Ogre::Real height);
        void setupChoices(size_t numChoices, size_t cancelID);
        void setChoice(size_t choiceID, size_t messageID);

        void show();

        void initialise();
        void deinitialise();

        void update(float timedelta);

        WndObjStatus getStatus();
        size_t getChoiceResult();

        WndObjStatus mStatus;

        Module * mpModule;
        Ogre::String mWindowName;

        size_t mNumChoices;
        size_t mCancelID;
        int mChoice;

        Ogre::Real mCurHoldTime;
        Ogre::Real mCurDelayTime;

        Ogre::Real mCurTimer;
        CursorState mCurState;

        bool mCurSubState;

        Ogre::Real mWindowPositionX;
        Ogre::Real mWindowPositionY;
        Ogre::Real mWindowWidth;
        Ogre::Real mWindowHeight;

        Ogre::Real mTextSize;

        Ogre::Real mWindowAlpha;
        Ogre::Real mTextAlpha;

        Ogre::Real mOpenAnimSpeed;
        Ogre::Real mCloseAnimSpeed;

        Ogre::Real mAnimPos;

        Ogre::Real mCursorPosX;
        Ogre::Real mCursorPosY;

        Ogre::Real mCursorAnimSpeed;
        Ogre::Real mCursorAnimScale;
        Ogre::Real mCursorAnimation;

        size_t mCursorDelay;
        Ogre::Real mCursorDelayTime;

        size_t mNormalTextColor;
        size_t mSelectedTextColor;
        size_t mDisabledTextColor;

        Ogre::OverlayContainer * mWindowBackground;
        Ogre::OverlayContainer * mWindowTextContainer;
        std::vector<size_t> mChoiceMessageID;
        std::vector<Ogre::OverlayElement*> mChoiceList;
        Ogre::OverlayElement * mCursor;
        Ogre::OverlayElement * mCursorLine;

        float lerp(float a, float b, float t)
        {
            return a + t * (b - a);
        }
    };
}; // namespace

#endif // SONETTO_CHOICEWINDOW_H
