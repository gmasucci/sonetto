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

        Ogre::Real mFrameNumber;

        int mChoiceState;

        bool mChoiceHasShown;
};

#endif
