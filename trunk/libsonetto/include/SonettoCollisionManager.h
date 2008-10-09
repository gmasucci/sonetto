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

#ifndef SONETTO_COLLISIONMANAGER_H
#define SONETTO_COLLISIONMANAGER_H

#include <vector>
#include <Ogre.h>
#include "SonettoMain.h"

// Note 1: Parts of this code are based on Q-Gears.
// Note 2: This file and its .cpp counterpart were made while listening
// to the Final Fantasy VII soundtrack.
namespace Sonetto
{
    // Forward declaration
    class EventObject;

    struct Walkmesh
    {
        struct Triangle
        {
            Triangle() { memset(borderLink,-1,sizeof(int)*3); }

            int vertexNum[3];
            int borderLink[3];
        };

        std::vector<Ogre::Vector3> vertex;
        std::vector<Triangle>      triangle;

        inline int getBorderLink(int tri,int index) const
        {
            return triangle[tri].borderLink[index];
        }

        inline int getTriangleVertex(int tri,int index) const
        {
            return triangle[tri].vertexNum[index];
        }
    };

    /** Collision manager

        The collision manager is a manager instatiated and held locally by each Module that wants to use it. It is
        responsible for checking for collisions and reacting to them, like making objects slide on walls and stay atop
        of the walkmesh.
    */
    class SONETTO_EXPORT CollisionManager
    {
    public:
        /// Constructor.
        CollisionManager();

        /// Destructor.
        ~CollisionManager();

		bool createWalkmesh(Ogre::Entity *entity);

		void setNextPos(EventObject *evt,const EventVector &events,
                const Ogre::Vector3 &move);

    private:
	    Walkmesh mWalkmesh;
    };
} // namespace

#endif
