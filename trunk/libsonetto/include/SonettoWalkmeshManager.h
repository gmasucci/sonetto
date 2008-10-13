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

#ifndef SONETTO_WALKMESHMANAGER_H
#define SONETTO_WALKMESHMANAGER_H

#include <vector>
#include <memory>
#include <OgreVector3.h>
#include "SonettoEventObject.h"

namespace Sonetto
{
    struct WalkmeshTriangle
    {
        WalkmeshTriangle() { memset(borderLink,-1,sizeof(int)*3); }

        int vertexNum[3];
        int borderLink[3];
    };

    typedef std::vector<WalkmeshTriangle> WalkmeshTriangleVector;

    struct WalkmeshData
    {
        std::vector<Ogre::Vector3> vertex;
        WalkmeshTriangleVector triangle;

        inline int getBorderLink(int tri,int index) const
        {
            return triangle[tri].borderLink[index];
        }

        inline int getTriangleVertex(int tri,int index) const
        {
            return triangle[tri].vertexNum[index];
        }
    };

    struct WalkmeshEventInfo
    {
        WalkmeshEventInfo(int aTriangle)
                : triangle(aTriangle) {}

        int triangle;
    };

    typedef std::map<const EventObject *,WalkmeshEventInfo> WalkmeshEventInfoMap;

    class SONETTO_EXPORT WalkmeshManager
    {
    public:
        WalkmeshManager() {}
        ~WalkmeshManager() {}

        void createWalkmesh(Ogre::Entity *mesh);

        void registerEvent(const EventObject *evt);
        void unregisterEvent(const EventObject *evt);

        /** Moves a registered event

            This method moves the object, translating it from it's current location.
            Note that the final position may not be the same as inputed in this
            function, as it will first pass through a collision test against the walkmesh
            and other registered events if the EventObject::mIgnoreCollisions flag is not set.

        @param
            moveVector This it the desired movement vector the character will move if it doesn't
            find any obstacle on its path.
        */
        void moveEvent(EventObject *evt,const Ogre::Vector3 &moveVector);
        // bool setEventPosition(EventObject *evt,const Ogre::Vector3 &pos);

    private:
        void convertMeshData(Ogre::Entity *entity,std::vector<Ogre::Vector3> *vertexBuf,
                std::vector<int> *faceBuf);

        bool walkmeshBorderCross(EventObject *evt,Ogre::Vector3 &position);

        bool checkCollisions(EventObject *evt,Ogre::Vector3 &position);

        WalkmeshData mWalkmesh;
        WalkmeshEventInfoMap mEventInfo;
    };
} // namespace

#endif // SONETTO_WALKMESHMANAGER_H
