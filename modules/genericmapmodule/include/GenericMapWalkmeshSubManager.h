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

#ifndef GENERICMAP_WALKMESHSUBMANAGER_H
#define GENERICMAP_WALKMESHSUBMANAGER_H

#include <OgreVector3.h>
#include "GenericMapEvent.h"

namespace GenericMapModule
{
    class WalkmeshSubManager
    {
    public:
        WalkmeshSubManager() : mWalkmesh(NULL) {}
        virtual ~WalkmeshSubManager() {}

        virtual void setWalkmesh(const Walkmesh *walkmesh);
        virtual inline const Walkmesh *getWalkmesh() const
                { return mWalkmesh; }

        virtual void registerEvent(Event *evt);
        virtual void unregisterEvent(Event *evt);
        virtual inline void clearRegisteredEvents() { mEvents.clear(); }

        /** Moves a registered event

            This method moves the object, translating it from it's current location.
            Note that the final position may not be the same as inputed in this
            function, as it will first pass through a collision test against the walkmesh
            and other registered events if the EventObject::mIgnoreCollisions flag is not set.

        @param
            moveVector This it the desired movement vector the character will move if it doesn't
            find any obstacle on its path.
        */
        virtual void moveEvent(Event *evt,const Ogre::Vector3 &moveVector);

        virtual bool setEventPosition(Event *evt,const Ogre::Vector3 &pos);

        virtual bool isEventOverTriangle(Event *evt,uint32 triangleID);

    protected:
        virtual bool walkmeshBorderCross(Event *evt,Ogre::Vector3 &position,
                float *slideFactor = NULL);

        virtual bool checkCollisions(Event *evt,Ogre::Vector3 &position);

        const Walkmesh *mWalkmesh;

        EventVector mEvents;
    };
} // namespace

#endif
