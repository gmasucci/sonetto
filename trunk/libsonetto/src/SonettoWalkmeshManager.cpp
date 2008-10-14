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

#include "SonettoMath.h"
#include "SonettoWalkmeshManager.h"

namespace Sonetto
{
    //-------------------------------------------------------------------------
    // Sonetto::WalkmeshManager implementation.
    //-------------------------------------------------------------------------
    void WalkmeshManager::createWalkmesh(Ogre::Entity *mesh)
    {
        std::vector<int> triangle;

        convertMeshData(mesh,&mWalkmesh.vertex,&triangle);

        for (size_t i = 0;i < triangle.size();i += 3)
        {
            WalkmeshTriangle tri;
            int searchVerts[2];

            tri.vertexNum[0] = triangle[i];
            tri.vertexNum[1] = triangle[i+1];
            tri.vertexNum[2] = triangle[i+2];

            for (size_t j = 0;j < 3;++j)
            {
                switch (j) {
                    case 0: // Side A
                        searchVerts[0] = tri.vertexNum[0];
                        searchVerts[1] = tri.vertexNum[1];
                    break;

                    case 1: // Side B
                        searchVerts[0] = tri.vertexNum[1];
                        searchVerts[1] = tri.vertexNum[2];
                    break;

                    case 2: // Side C
                        searchVerts[0] = tri.vertexNum[2];
                        searchVerts[1] = tri.vertexNum[0];
                    break;
                }

                for (size_t k = 0;k < triangle.size();k += 3)
                {
                    if (k != i)
                    {
                        int sharedVerts = 0;

                        if (mWalkmesh.vertex[triangle[k]] == mWalkmesh.vertex[searchVerts[0]])
                            ++sharedVerts;
                        if (mWalkmesh.vertex[triangle[k]] == mWalkmesh.vertex[searchVerts[1]])
                            ++sharedVerts;

                        if (mWalkmesh.vertex[triangle[k+1]] == mWalkmesh.vertex[searchVerts[0]])
                            ++sharedVerts;
                        if (mWalkmesh.vertex[triangle[k+1]] == mWalkmesh.vertex[searchVerts[1]])
                            ++sharedVerts;

                        if (mWalkmesh.vertex[triangle[k+2]] == mWalkmesh.vertex[searchVerts[0]])
                            ++sharedVerts;
                        if (mWalkmesh.vertex[triangle[k+2]] == mWalkmesh.vertex[searchVerts[1]])
                            ++sharedVerts;

                        if (sharedVerts >= 2)
                        {
                            tri.borderLink[j] = k/3;
                        }
                    }
                }
            }

            mWalkmesh.triangle.push_back(tri);
        }
    }
    //-------------------------------------------------------------------------
    void WalkmeshManager::convertMeshData(Ogre::Entity *entity,
            std::vector<Ogre::Vector3> *vertexBuf,std::vector<int> *faceBuf)
    {
        Ogre::MeshPtr mesh = entity->getMesh();
        bool added_shared = false;
        size_t current_offset = 0;
        size_t shared_offset = 0;
        size_t next_offset = 0;
        size_t index_offset = 0;
        // int numOfSubs = 0;

        bool useSoftwareBlendingVertices = entity->hasSkeleton();

        if (useSoftwareBlendingVertices)
        {
            entity->_updateAnimation();
        }

        // Run through the submeshes again, adding the data into the arrays
        for ( size_t i = 0; i < mesh->getNumSubMeshes(); ++i)
        {
            Ogre::SubMesh* submesh = mesh->getSubMesh(i);
            bool useSharedVertices = submesh->useSharedVertices;

            if (vertexBuf)
            {
                //----------------------------------------------------------------
                // GET VERTEXDATA
                //----------------------------------------------------------------
                const Ogre::VertexData * vertex_data;
                if(useSoftwareBlendingVertices)
                    vertex_data = useSharedVertices ? entity->_getSkelAnimVertexData() : entity->getSubEntity(i)->_getSkelAnimVertexData();
                else
                    vertex_data = useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;

                if((!useSharedVertices)||(useSharedVertices && !added_shared))
                {
                    if(useSharedVertices)
                    {
                        added_shared = true;
                        shared_offset = current_offset;
                    }

                    const Ogre::VertexElement* posElem =
                        vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);

                    Ogre::HardwareVertexBufferSharedPtr vbuf =
                        vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

                    unsigned char* vertex =
                        static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

                    // There is _no_ baseVertexPointerToElement() which takes an Ogre::Real or a double
                    //  as second argument. So make it float, to avoid trouble when Ogre::Real is
                    //  comiled/typedefed as double:
                    float* pReal;

                    for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
                    {
                        posElem->baseVertexPointerToElement(vertex, &pReal);

                        // size_t n = current_offset*3 + j*3;

                        vertexBuf->push_back(Ogre::Vector3(pReal[0],pReal[1],pReal[2]));
                        // <chan> vertexBuf[n + 0] = pReal[0];
                        // <chan> vertexBuf[n + 1] = pReal[1];
                        // <chan> vertexBuf[n + 2] = pReal[2];
                    }

                    vbuf->unlock();
                    next_offset += vertex_data->vertexCount;
                }
            }

            if (faceBuf)
            {
                //----------------------------------------------------------------
                // GET INDEXDATA
                //----------------------------------------------------------------
                Ogre::IndexData* index_data = submesh->indexData;
                size_t numTris = index_data->indexCount / 3;
                Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

                bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

                unsigned int *pLong = static_cast<unsigned int*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
                unsigned short *pShort = reinterpret_cast<unsigned short*>(pLong);


                size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;

                if ( use32bitindexes )
                {
                    for ( size_t k = 0; k < numTris*3; ++k)
                    {
                        faceBuf->push_back(pLong[k] + static_cast<int>(offset));
                        ++index_offset;
                        // <chan> faceBuf[index_offset++] = pLong[k] + static_cast<int>(offset);
                    }
                }
                else
                {
                    for ( size_t k = 0; k < numTris*3; ++k)
                    {
                        faceBuf->push_back(static_cast<int>(pShort[k]) + static_cast<int>(offset));
                        ++index_offset;
                        // <chan> faceBuf[index_offset++] = static_cast<int>(pShort[k]) + static_cast<int>(offset);
                    }
                }

                ibuf->unlock();
            }

            current_offset = next_offset;
        }
    }
    //-------------------------------------------------------------------------
    bool WalkmeshManager::walkmeshBorderCross(EventObject *evt,
            Ogre::Vector3 &position,float *slideFactor)
    {
        int curTri = mEventInfo.find(evt)->second.triangle;
        Ogre::Vector3 direction = position - evt->getPosition();
        direction.normalise();

        while (true)
        {
            int nextTri = -1;

            // Gets current triangle's vertex coordinates
            const Ogre::Vector3 &vA = mWalkmesh.vertex[mWalkmesh.getTriangleVertex(curTri,0)];
            const Ogre::Vector3 &vB = mWalkmesh.vertex[mWalkmesh.getTriangleVertex(curTri,1)];
            const Ogre::Vector3 &vC = mWalkmesh.vertex[mWalkmesh.getTriangleVertex(curTri,2)];

            // <todo> Unknown variables - What the hell are they?
            // We can only guess that they will be lesser than 0.0f when
            // the line was crossed, but how it works I simply cannot understand
            float sign1 = Math::sideOfVector(position,vB,vA);
            float sign2 = Math::sideOfVector(position,vC,vB);
            float sign3 = Math::sideOfVector(position,vA,vC);

            if (sign1 < 0) {
                nextTri = mWalkmesh.getBorderLink(curTri,0);
                if (slideFactor)
                {
                    float angle;
                    Ogre::Vector3 border = vB - vA;

                    border.normalise();
                    angle = Ogre::Math::ACos(border.dotProduct(direction)).valueDegrees();

                    *slideFactor = Math::clamp(angle / 90.0f,0.0f,1.0f);
                }
            } else
            if (sign2 < 0) {
                nextTri = mWalkmesh.getBorderLink(curTri,1);
                if (slideFactor)
                {
                    float angle;
                    Ogre::Vector3 border = vC - vB;

                    border.normalise();
                    angle = Ogre::Math::ACos(border.dotProduct(direction)).valueDegrees();

                    *slideFactor = Math::clamp(angle / 90.0f,0.0f,1.0f);
                }
            } else
            if (sign3 < 0) {
                nextTri = mWalkmesh.getBorderLink(curTri,2);
                if (slideFactor)
                {
                    float angle;
                    Ogre::Vector3 border = vC - vA;

                    border.normalise();
                    angle = Ogre::Math::ACos(border.dotProduct(direction)).valueDegrees();

                    *slideFactor = Math::clamp(angle / 90.0f,0.0f,1.0f);
                }
            } else { // Not crossing any borders
                // Gets destination elevation and saves current
                // triangle into event info map
                position.y = Math::pointElevation(position,vA,vB,vC);
                mEventInfo.find(evt)->second.triangle = curTri;

                // No borders were crossed, so our slide factor is 1.0f
                /*if (slideFactor)
                {
                    *slideFactor = 1.0f;
                }*/

                // false == No borders crossed
                return false;
            }

            if (nextTri >= 0)
            {
                curTri = nextTri;
                continue;
            }

            position.y = Math::pointElevation(position,vA,vB,vC);
            mEventInfo.find(evt)->second.triangle = curTri;
            return true;
        }
    }
    //-------------------------------------------------------------------------
    bool WalkmeshManager::checkCollisions(EventObject *evt,
            Ogre::Vector3 &position)
    {
        // If we have our ignore collisions flag set, we should return false
        if (evt->getIgnoreCollision() == true)
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
        // other events
        return false;
    }
    //-------------------------------------------------------------------------
    void WalkmeshManager::registerEvent(const EventObject *evt)
    {
        // Checks if it was not already registered
        if (mEventInfo.find(evt) != mEventInfo.end())
        {
            SONETTO_THROW("Trying to register an already registered event object");
        }

        // Pairs it with a WalkmeshEventInfo inside mEventInfo
        // <todo> Set triangle to -1
        mEventInfo.insert(std::pair<const EventObject *,WalkmeshEventInfo>
                (evt,WalkmeshEventInfo(0)));
    }
    //-------------------------------------------------------------------------
    void WalkmeshManager::unregisterEvent(const EventObject *evt)
    {
        // Checks if it is not registered yet
        WalkmeshEventInfoMap::iterator iter = mEventInfo.find(evt);
        if (iter == mEventInfo.end())
        {
            SONETTO_THROW("Trying to unregister an event object that is not registered yet");
        }

        // Unregisters event
        mEventInfo.erase(iter);
    }
    //-------------------------------------------------------------------------
    void WalkmeshManager::moveEvent(EventObject *evt,
            const Ogre::Vector3 &xzMoveVector)
    {
        // <todo> Move the event freely when evt->getIgnoreCollisions() is set
        // We will need a better name to `xzMoveVector' when we implement that

        WalkmeshEventInfoMap::iterator iter = mEventInfo.find(evt);

        // Checks whether the event is registered or not
        if (iter == mEventInfo.end())
        {
            SONETTO_THROW("Trying to move an event object that is not "
                    "registered");
        }

        // Gets this event object's current triangle
        int curTri = iter->second.triangle;

        // Checks whether the EventInfo for this EventObject
        // is initialised or not
        if (curTri == -1)
        {
            SONETTO_THROW("Trying to use a registered but not initialised "
                    "event object");
        }

        // Gets this event object's current position
        Ogre::Vector3 startPoint(evt->getPosition());

        // This `direction' vector will be modified later
        // That's why it is copied to a local variable instead of being used
        // directly by accessing `xzMoveVector'
        Ogre::Vector3 direction(xzMoveVector);

        // Do not move if the move vector has a length of 0.0f
        if (direction.x == 0.0f && direction.z == 0.0f)
        {
            return;
        }

        // Gets event collision radius
        float colRadius = evt->getColRadius();

        // Used to rotate vectors
        Ogre::Quaternion q1(0.0f, 0.0f, 0.0f, 1.0f);

        // Calculates final desired destination on the XZ plane
        Ogre::Vector3 endPoint(startPoint + ( direction * evt->getSpeed() ));

        // Collision check flags
        bool first_triangle_check  = false;
        bool second_triangle_check = false;
        bool third_triangle_check  = false;
        bool last_triangle_check   = false;
        bool first_entity_check    = false;
        bool second_entity_check   = false;
        bool third_entity_check    = false;

        // Gets current triangle's vertex coordinates
        Ogre::Vector3 vA =
                mWalkmesh.vertex[mWalkmesh.getTriangleVertex(curTri,0)];
        Ogre::Vector3 vB =
                mWalkmesh.vertex[mWalkmesh.getTriangleVertex(curTri,1)];
        Ogre::Vector3 vC =
                mWalkmesh.vertex[mWalkmesh.getTriangleVertex(curTri,2)];

        // Gets the Y elevation based on the destination
        // coordinates on the XZ plane
        endPoint.y = Math::pointElevation(endPoint,vA,vB,vC);

        // Gets a vector that begins on the start point and ends on
        // the end point, taking into account the Y-axis elevation difference
        // between them, and normalises it
        Ogre::Vector3 xyzMoveVector = endPoint - startPoint;
        xyzMoveVector.normalise();

        // Scales the XYZ move vector to the length of the XZ move vector
        // This will make the XYZ move vector have the same length as the
        // inputed xzMoveVector, resulting in the same move velocity
        xyzMoveVector *= direction.length();

        // Gets final direction back from the xyzMoveVector
        // They are changed now, since xyzMoveVector cares about elevation
        direction.x = xyzMoveVector.x;
        direction.z = xyzMoveVector.z;

        for (int i = 0; i < 17; ++i)
        {
            int frontTriangle;
            float scaleDirection,slideFactor = 1.0f,finalSlideFactor = 1.0f;
            Ogre::Vector3 rotatedDirection(0.0f, 0.0f, 0.0f);
            Ogre::Vector3 rotatedEndPoint(0.0f,0.0f,0.0f);

            // Sets new end point based on `direction'
            endPoint = startPoint + (direction * evt->getSpeed());

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
            first_triangle_check = walkmeshBorderCross(evt,rotatedEndPoint,&slideFactor);
            mEventInfo.find(evt)->second.triangle = curTri;

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
            second_triangle_check = walkmeshBorderCross(evt,rotatedEndPoint,&slideFactor);
            mEventInfo.find(evt)->second.triangle = curTri;

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
            rotatedDirection = direction;
            scaleDirection = rotatedDirection.normalise() + colRadius;
            rotatedDirection *= scaleDirection;

            // Checks end point against the walkmesh and restores
            // current triangle event information (it is changed by
            // walkmeshBorderCross())
            third_triangle_check = walkmeshBorderCross(evt,rotatedEndPoint,&slideFactor);
            mEventInfo.find(evt)->second.triangle = curTri;

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
            evt->setPosition(endPoint);
        }
    }
} // namespace
