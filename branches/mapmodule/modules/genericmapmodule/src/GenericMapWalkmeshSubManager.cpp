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

#include <OgreSceneNode.h>
#include <SonettoMath.h>
#include "GenericMapWalkmeshSubManager.h"
#include "GenericMapWalkmesh.h"

namespace GenericMapModule
{
    //-------------------------------------------------------------------------
    // GenericMapModule::WalkmeshSubManager implementation.
    //-------------------------------------------------------------------------
    void WalkmeshSubManager::setWalkmesh(const Walkmesh *walkmesh)
    {
        if (!mEvents.empty())
        {
            SONETTO_THROW("Walkmesh is not empty of events");
        }

        mWalkmesh = walkmesh;
    }
    //-------------------------------------------------------------------------
    void WalkmeshSubManager::registerEvent(Event *evt)
    {
        // Checks if it was not already registered
        if (std::find(mEvents.begin(),mEvents.end(),evt) != mEvents.end())
        {
            SONETTO_THROW("Event already registered with walkmesh");
        }

        mEvents.push_back(evt);
    }
    //-------------------------------------------------------------------------
    void WalkmeshSubManager::unregisterEvent(Event *evt)
    {
        EventVector::iterator i = std::find(mEvents.begin(),
                mEvents.end(),evt);

        // Checks if it is not registered yet
        if (i == mEvents.end())
        {
            SONETTO_THROW("Event not registered with walkmesh");
        }

        // Unregisters event
        mEvents.erase(i);
    }
    //-------------------------------------------------------------------------
    bool WalkmeshSubManager::walkmeshBorderCross(Event *evt,
            Ogre::Vector3 &position,float *slideFactor)
    {
        uint32 curTri = evt->getCurrentTriangle();
        Ogre::Vector3 direction = position - evt->getSceneNode()->
                getPosition();
        direction.normalise();

        while (true)
        {
            uint32 nextTri = 0;

            // Gets current triangle's vertex coordinates
            const Ogre::Vector3 &vA = mWalkmesh->getTriangleVertex(curTri,0);
            const Ogre::Vector3 &vB = mWalkmesh->getTriangleVertex(curTri,1);
            const Ogre::Vector3 &vC = mWalkmesh->getTriangleVertex(curTri,2);

            // Checks what borders were crossed
            float sign1 = Sonetto::Math::sideOfVector(position,vB,vA);
            float sign2 = Sonetto::Math::sideOfVector(position,vC,vB);
            float sign3 = Sonetto::Math::sideOfVector(position,vA,vC);

            if (sign1 < 0) {
                nextTri = mWalkmesh->getEdgeLink(curTri,0);
                if (slideFactor)
                {
                    Ogre::Vector3 border = vB - vA;
                    float angle;

                    border.normalise();
                    Ogre::Quaternion quat;
                    quat.FromAngleAxis(Ogre::Radian(Ogre::Degree(90.0f)),
                            Ogre::Vector3::UNIT_Y);
                    border = quat * border;
                    angle = Ogre::Math::ACos(border.dotProduct(direction)).
                            valueDegrees();

                    *slideFactor = Sonetto::Math::clamp(angle / 180.0f,
                            0.0f,1.0f);
                }
            } else
            if (sign2 < 0) {
                nextTri = mWalkmesh->getEdgeLink(curTri,1);
                if (slideFactor)
                {
                    Ogre::Vector3 border = vC - vB;
                    float angle;

                    border.normalise();
                    Ogre::Quaternion quat;
                    quat.FromAngleAxis(Ogre::Radian(Ogre::Degree(90.0f)),
                            Ogre::Vector3::UNIT_Y);
                    border = quat * border;
                    angle = Ogre::Math::ACos(border.dotProduct(direction)).
                            valueDegrees();

                    *slideFactor = Sonetto::Math::clamp(angle / 180.0f,
                            0.0f,1.0f);
                }
            } else
            if (sign3 < 0) {
                nextTri = mWalkmesh->getEdgeLink(curTri,2);
                if (slideFactor)
                {
                    Ogre::Vector3 border = vA - vC;
                    float angle;

                    border.normalise();
                    Ogre::Quaternion quat;
                    quat.FromAngleAxis(Ogre::Radian(Ogre::Degree(90.0f)),
                            Ogre::Vector3::UNIT_Y);
                    border = quat * border;
                    angle = Ogre::Math::ACos(border.dotProduct(direction)).
                            valueDegrees();

                    *slideFactor = Sonetto::Math::clamp(angle / 180.0f,
                            0.0f,1.0f);
                }
            } else { // Not crossing any borders
                // Gets destination elevation and saves current triangle
                position.y = Sonetto::Math::pointElevation(position,vA,vB,vC);
                evt->setCurrentTriangle(curTri);

                // No borders were crossed, so our slide factor is 1.0f
                /*if (slideFactor)
                {
                    *slideFactor = 1.0f;
                }*/

                // false == No borders crossed
                return false;
            }

            if (nextTri > 0)
            {
                curTri = nextTri;
                continue;
            }

            position.y = Sonetto::Math::pointElevation(position,vA,vB,vC);
            evt->setCurrentTriangle(curTri);
            return true;
        }
    }
    //-------------------------------------------------------------------------
    bool WalkmeshSubManager::checkCollisions(Event *evt,
            Ogre::Vector3 &position)
    {
        // If we have our ignore collisions flag set, we should return false
        /*if (evt->getIgnoreCollision() == true)
        {
            return false;
        }

        // Loops through all registered events inside this walkmesh manager
        for (WalkmeshEventInfoMap::iterator i = mEventInfo.begin();
                i != mEventInfo.end();++i)
        {
            const EventObject *evt2 = i->first;

            // Skips if this event (`evt2') is the same as inputed (`evt')
            // or if its ignore collisions flag is set
            if (evt2 == evt || evt2->getIgnoreCollision() == true)
            {
                continue;
            }

            // Gets event's position and collision radius
            Ogre::Vector3 pos2 = evt2->getPosition();
            float colRadius = evt2->getColRadius();

            // The height we need to check depends on which event is below which
            float height;
            if (pos2.y >= position.y) {
                height = evt->getHeight();
            } else {
                height = evt2->getHeight();
            }

            // Check if the two events potentially intersect given their distance
            // on the Y axis and the height chosen up there
            if (( pos2.y - position.y + height <  height * 2 ) &&
                ( pos2.y - position.y + height >= 0          ) )
            {
                // Squares colRadius to avoid needing to square root the distance
                colRadius *= colRadius;
                float distance = ( (pos2.x - position.x) * (pos2.x - position.x) ) +
                                 ( (pos2.z - position.z) * (pos2.z - position.z) );

                // If the squared distance is smaller than the squared radius, then
                // the cylinders intersect, in which case we return true
                if (distance < colRadius)
                {
                    return true;
                }
            }
        }

        // Returns false if our event's cylinder didn't intersect with any
        // other events*/
        return false;
    }
    //-------------------------------------------------------------------------
    void WalkmeshSubManager::moveEvent(Event *evt,
            const Ogre::Vector3 &moveVector)
    {
        // <todo> Move the event freely when evt->isFlying() is true

        // Checks whether the event is registered or not
        if (std::find(mEvents.begin(),mEvents.end(),evt) == mEvents.end())
        {
            SONETTO_THROW("Event not registered with walkmesh");
        }

        // Gets this event's current triangle
        uint32 curTri = evt->getCurrentTriangle();

        // Checks whether this event was positioned on the walkmesh or not
        if (curTri == 0)
        {
            SONETTO_THROW("Event position not set in walkmesh");
        }

        // Gets this event's current position
        Ogre::Vector3 startPoint(evt->getSceneNode()->getPosition());

        // This `direction' vector will be modified later
        // That's why it is copied to a local variable instead of being used
        // directly by accessing `moveVector'
        Ogre::Vector3 direction(moveVector);

        // Do not move if the move vector has a length of 0.0f
        if (direction.x == 0.0f && direction.z == 0.0f)
        {
            return;
        }

        // Gets event collision radius
        float colRadius = evt->getRadius();

        // Used to rotate vectors
        Ogre::Quaternion q1(0.0f,0.0f, 0.0f,1.0f);

        // Calculates final desired destination on the XZ plane
        Ogre::Vector3 endPoint( startPoint +
                (direction * evt->getWalkSpeed()) );

        // Collision check flags
        bool first_triangle_check  = false;
        bool second_triangle_check = false;
        bool third_triangle_check  = false;
        bool last_triangle_check   = false;
        bool first_entity_check    = false;
        bool second_entity_check   = false;
        bool third_entity_check    = false;

        // Gets current triangle's vertex coordinates
        const Ogre::Vector3 &vA = mWalkmesh->getTriangleVertex(curTri,0);
        const Ogre::Vector3 &vB = mWalkmesh->getTriangleVertex(curTri,1);
        const Ogre::Vector3 &vC = mWalkmesh->getTriangleVertex(curTri,2);

        // Gets the Y elevation based on the destination
        // coordinates on the XZ plane
        endPoint.y = Sonetto::Math::pointElevation(endPoint,vA,vB,vC);

        // Gets a vector that begins on the start point and ends on
        // the end point, taking into account the Y-axis elevation difference
        // between them, and normalises it
        Ogre::Vector3 xyzMoveVector = endPoint - startPoint;
        xyzMoveVector.normalise();

        // Scales the XYZ move vector to the length of the XZ move vector
        // This will make the XYZ move vector have the same length as the
        // inputed moveVector, resulting in the same move speed
        xyzMoveVector *= direction.length();

        // Gets final direction back from the xyzMoveVector
        // They are changed now, since xyzMoveVector cares about elevation
        direction.x = xyzMoveVector.x;
        direction.z = xyzMoveVector.z;

        for (int i = 0; i < 17; ++i)
        {
            float scaleDirection,slideFactor = 1.0f,finalSlideFactor = 1.0f;
            Ogre::Vector3 rotatedDirection(0.0f, 0.0f, 0.0f);
            Ogre::Vector3 rotatedEndPoint(0.0f,0.0f,0.0f);

            // Sets new end point based on `direction'
            endPoint = startPoint + (direction * evt->getWalkSpeed());

            // 1st check
            // Rotates direction by +45 degrees on the Y axis
            q1.FromAngleAxis(Ogre::Radian(Ogre::Degree(45.0f)),
                    Ogre::Vector3::UNIT_Y);
            rotatedDirection = q1 * direction;

            // Scale the rotated direction so that it lengths the same as before
            // plus the collision radius of this event
            scaleDirection = rotatedDirection.normalise() + colRadius;
            rotatedDirection *= scaleDirection;

            // Calculates rotated end point
            rotatedEndPoint = endPoint + rotatedDirection;

            // Checks rotated end point against the walkmesh and restores
            // current triangle event information (it is changed by
            // walkmeshBorderCross())
            first_triangle_check = walkmeshBorderCross(evt,rotatedEndPoint,
                    &slideFactor);
            evt->setCurrentTriangle(curTri);

            if (first_triangle_check)
            {
                finalSlideFactor = slideFactor;
            }

            // Checks rotated end point against other events
            first_entity_check = checkCollisions(evt,rotatedEndPoint);

            // 2nd check
            // Rotates direction by -45 degrees on the Y axis
            q1.FromAngleAxis(Ogre::Radian(Ogre::Degree(-45.0f)),
                    Ogre::Vector3::UNIT_Y);
            rotatedDirection = q1 * direction;

            // Scale the rotated direction so that it lengths the same as before
            // plus the collision radius of this event
            scaleDirection = rotatedDirection.normalise() + colRadius;
            rotatedDirection *= scaleDirection;

            // Calculates rotated end point
            rotatedEndPoint = endPoint + rotatedDirection;

            // Checks rotated end point against the walkmesh and restores
            // current triangle event information (it is changed by
            // walkmeshBorderCross())
            second_triangle_check = walkmeshBorderCross(evt,rotatedEndPoint,
                    &slideFactor);
            evt->setCurrentTriangle(curTri);

            if (second_triangle_check && slideFactor < finalSlideFactor)
            {
                finalSlideFactor = slideFactor;
            }

            // Checks rotated end point against other events
            second_entity_check = checkCollisions(evt,rotatedEndPoint);



            // 3rd check
            // The third check doesn't rotate the direction vector
            // It is the check responsible for checking whether there is a
            // collision directly to the front of our move vector
            rotatedDirection = direction;
            rotatedEndPoint = endPoint + rotatedDirection;

            // Scale the rotated direction so that it lengths the same as before
            // plus the collision radius of this event
            scaleDirection = rotatedDirection.normalise() + colRadius;
            rotatedDirection *= scaleDirection;

            // Checks end point against the walkmesh and restores
            // current triangle event information (it is changed by
            // walkmeshBorderCross())
            third_triangle_check = walkmeshBorderCross(evt,rotatedEndPoint,
                    &slideFactor);
            evt->setCurrentTriangle(curTri);

            if (third_triangle_check && slideFactor < finalSlideFactor)
            {
                finalSlideFactor = slideFactor;
            }

            // Checks end point against other events
            third_entity_check = checkCollisions(evt,rotatedEndPoint);

            // Apply slide factor
            direction *= finalSlideFactor;

            // Checks collision flags and modify `direction' vector accordingly,
            // and then rechecking
            // <todo> Do we really need to check whether no event collision is
            // happening? It doesn't make sense!
            if (first_triangle_check  == true ||
                second_triangle_check == true ||
                third_triangle_check  == true )
            {
                // <todo> From here on I can't make heads or tails of this code
                // if not both left and right check was fail
                if (first_triangle_check == false || second_triangle_check == false)
                {
                    if ( (first_triangle_check == false && first_entity_check == true    ) ||
                         (first_triangle_check == true  && second_triangle_check == false) )
                    {
                        q1.FromAngleAxis(Ogre::Radian(Ogre::Degree(-11.25f)), Ogre::Vector3::UNIT_Y);
                        direction = q1 * direction;
                    }

                    if ( first_triangle_check  == false &&
                         first_entity_check    == false &&
                        (second_triangle_check == true || second_entity_check == true) )
                    {
                        q1.FromAngleAxis(Ogre::Radian(Ogre::Degree(11.25f)), Ogre::Vector3::UNIT_Y);
                        direction = q1 * direction;
                    }

                    continue;
                }

                break;
            }
        }

        // Checks whether our final position is free
        last_triangle_check = walkmeshBorderCross(evt,endPoint);

        // Only sets final event position if no collide flags are
        // still set to true
        if (first_triangle_check  == false &&
            second_triangle_check == false &&
            third_triangle_check  == false &&
            last_triangle_check   == false &&
            first_entity_check    == false &&
            second_entity_check   == false &&
            third_entity_check    == false )
        {
            evt->getSceneNode()->setPosition(endPoint);
        }
    }
    //-------------------------------------------------------------------------
    bool WalkmeshSubManager::setEventPosition(Event *evt,
            const Ogre::Vector3 &pos)
    {
        EventVector::iterator i = std::find(mEvents.begin(),mEvents.end(),evt);

        // Checks whether the event is registered or not
        if (i == mEvents.end())
        {
            SONETTO_THROW("Event not registered with walkmesh");
        }

        uint32 tri = 0;         // Current nearest triangle found
        bool firstFound = true; // Flagged false when the first triangle is found
        float yNearest;         // Y coordinate of the nearest triangle found
        Ogre::Vector3 vA,vB,vC; // Current nearest triangle vertices

        // Loops through each triangle in the walkmesh searching for the nearest
        // triangle from the given pos.y
        for (size_t i = 1;i <= mWalkmesh->triangles.size();++i)
        {
            // Gets the iterated triangle vertex coordinates
            const Ogre::Vector3 &vAtmp = mWalkmesh->getTriangleVertex(i,0);
            const Ogre::Vector3 &vBtmp = mWalkmesh->getTriangleVertex(i,1);
            const Ogre::Vector3 &vCtmp = mWalkmesh->getTriangleVertex(i,2);

            // If the given position `pos' is inside the iterated triangle,
            // we procceed with our tests
            if (Sonetto::Math::sideOfVector(pos,vBtmp,vAtmp) > 0.0f &&
                Sonetto::Math::sideOfVector(pos,vCtmp,vBtmp) > 0.0f &&
                Sonetto::Math::sideOfVector(pos,vAtmp,vCtmp) > 0.0f )
            {
                // Gets the medium Y coordinate of this triangle
                float yCenter = (vAtmp.y + vBtmp.y + vCtmp.y) / 3;

                // If this is the first triangle found, we just set the
                // variables to describe it
                // Elsewise we need to check whether this new triangle is more
                // near to the event than the last one found
                if (firstFound) {
                    yNearest = yCenter;
                    vA = vAtmp;
                    vB = vBtmp;
                    vC = vCtmp;
                    tri = i;
                    firstFound = false;
                } else {
                    // Gets distances between the event Y position and the iterated
                    // triangle, and between the event Y position and the last triangle
                    // position that was deemed to be the nearest to the event position
                    float yDistance = pos.y - yCenter;
                    float yOldDistance = pos.y - yNearest;

                    // If this iterated triangle is more near to the event position than
                    // the last one, we pick it to check against others
                    if (fabs(yDistance) < fabs(yOldDistance)) {
                        yNearest = yCenter;
                        vA = vAtmp;
                        vB = vBtmp;
                        vC = vCtmp;
                        tri = i;
                    }
                }
            }
        }

        // If we have found a triangle (i.e. the event is effectively over the
        // walkmesh), we need to get the Y elevation on the desired XZ plane and
        // set the event position to these new coordinates
        if (tri != 0)
        {
            Ogre::Vector3 finalPos(pos);

            finalPos.y = Sonetto::Math::pointElevation(pos,vA,vB,vC);
            evt->getSceneNode()->setPosition(finalPos);
            evt->setCurrentTriangle(tri);

            // Returns true: Yes, the new position is set
            return true;
        }

        // Otherwise, returns false: Could not set position because the desired
        // position on the XZ plane was outside the walkmesh
        return false;
    }
    //-------------------------------------------------------------------------
    bool WalkmeshSubManager::isEventOverTriangle(Event *evt,uint32 triangleID)
    {
        const Ogre::Vector3 &vA = mWalkmesh->getTriangleVertex(triangleID,0);
        const Ogre::Vector3 &vB = mWalkmesh->getTriangleVertex(triangleID,1);
        const Ogre::Vector3 &vC = mWalkmesh->getTriangleVertex(triangleID,2);

        if (Sonetto::Math::sideOfVector(evt->getSceneNode()->getPosition(),
                    vB,vA) > 0.0f &&
            Sonetto::Math::sideOfVector(evt->getSceneNode()->getPosition(),
                    vC,vB) > 0.0f &&
            Sonetto::Math::sideOfVector(evt->getSceneNode()->getPosition(),
                    vA,vC) > 0.0f )
        {
            return true;
        }

        return false;
    }
} // namespace
