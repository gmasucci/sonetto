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
            Ogre::Vector3 &position,const Ogre::Vector2 &move_vector)
    {
        int current_triangle = mEventInfo.find(evt)->second.triangle;
        if (current_triangle == -1)
        {
            return true;
        }

        Ogre::Vector2 pos = Ogre::Vector2(position.x, position.z);

        for (;;)
        {
            Ogre::Vector3 A3 = mWalkmesh.vertex[mWalkmesh.getTriangleVertex(current_triangle,0)];
            Ogre::Vector3 B3 = mWalkmesh.vertex[mWalkmesh.getTriangleVertex(current_triangle,1)];
            Ogre::Vector3 C3 = mWalkmesh.vertex[mWalkmesh.getTriangleVertex(current_triangle,2)];

            Ogre::Vector2 A(A3.x, A3.z);
            Ogre::Vector2 B(B3.x, B3.z);
            Ogre::Vector2 C(C3.x, C3.z);

            float sign1 = Math::side_of_vector(pos, B, A);
            float sign2 = Math::side_of_vector(pos, C, B);
            float sign3 = Math::side_of_vector(pos, A, C);

            int next_triangle = -1;

            if (sign1 < 0)
            {
                next_triangle = mWalkmesh.getBorderLink(current_triangle,0);
            }
            else if (sign2 < 0)
            {
                next_triangle = mWalkmesh.getBorderLink(current_triangle,1);
            }
            else if (sign3 < 0)
            {
                next_triangle = mWalkmesh.getBorderLink(current_triangle,2);
            }
            else
            {
                //log->logMessage("In triangle.");
                position.y = Math::pointElevation(Ogre::Vector3(pos.x,0.0f,pos.y),A3,B3,C3);
                //log->logMessage("Stop CheckTriangles with 0 and triangle " + Ogre::StringConverter::toString(current_triangle) + ".");
                mEventInfo.find(evt)->second.triangle = current_triangle;
                return false;
            }

            if (next_triangle >= 0)
            {
                current_triangle = next_triangle;
                continue;
            }

            position.y = Math::pointElevation(Ogre::Vector3(pos.x,0.0f,pos.y),A3,B3,C3);
            //log->logMessage("Stop CheckTriangles with 1 and triangle " + Ogre::StringConverter::toString(current_triangle) + ".");
            mEventInfo.find(evt)->second.triangle = current_triangle;
            return true;
        }
    }
    //-------------------------------------------------------------------------
    bool WalkmeshManager::checkCollisions(EventObject *evt,
            Ogre::Vector3 &position)
    {
        if (evt->getIgnoreCollision() == true)
        {
            return false;
        }

        for (WalkmeshEventInfoMap::iterator i = mEventInfo.begin();
                i != mEventInfo.end();++i)
        {
            const EventObject *evt2 = i->first;

            if (evt2->getIgnoreCollision() == true)
            {
                continue;
            }

            if (evt2 == evt)
            {
                continue;
            }

            Ogre::Vector3 pos1 = evt2->getPosition();
            int solid_range = 2.0f; // m_Models[i]->GetSolidRange();

            int height = (pos1.y < position.y) ? evt2->getHeight() : evt->getHeight();

            if (((pos1.y - position.y + height) < (height * 2)) && ((pos1.y - position.y + height) >= 0))
            {
                solid_range = solid_range * solid_range;
                float distance = (pos1.x - position.x) * (pos1.x - position.x) + (pos1.z - position.z) * (pos1.z - position.z);

                if (distance < solid_range)
                {
                    return true;
                }
            }
        }

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
            const Ogre::Vector3 &moveVector)
    {
        WalkmeshEventInfoMap::iterator iter = mEventInfo.find(evt);

        // Checks whether the event is registered or not
        if (iter == mEventInfo.end())
        {
            SONETTO_THROW("Trying to move an event object that is not registered");
        }

        int curTri = iter->second.triangle;
        Ogre::Vector3 start_point(evt->getPosition());
        Ogre::Vector2 direction(moveVector.x,moveVector.z);

        Ogre::Vector3 rotation(0.0f, 0.0f, 0.0f);
        Ogre::Quaternion q1 (0.0f, 0.0f, 0.0f, 1.0f);
        Ogre::Vector3 end_point(0.0f, 0.0f, 0.0f);
        Ogre::Vector3 end_point2(0.0f, 0.0f, 0.0f);
        bool first_triangle_check  = false;
        bool second_triangle_check = false;
        bool third_triangle_check  = false;
        bool last_triangle_check   = false;
        bool first_entity_check    = false;
        bool second_entity_check   = false;
        bool third_entity_check    = false;

        // shorten move vector by triangle angle
        end_point.x = start_point.x + direction.x;
        end_point.z = start_point.z + direction.y;
        Ogre::Vector3 A3 = mWalkmesh.vertex[mWalkmesh.getTriangleVertex(curTri,0)];
        Ogre::Vector3 B3 = mWalkmesh.vertex[mWalkmesh.getTriangleVertex(curTri,1)];
        Ogre::Vector3 C3 = mWalkmesh.vertex[mWalkmesh.getTriangleVertex(curTri,2)];
        end_point.y = Math::pointElevation(end_point,A3,B3,C3);
        Ogre::Vector3 temp = end_point - start_point;
        temp.normalise();
        temp = temp * direction.length();
        direction.x = temp.x;
        direction.y = temp.z;

        for (int i = 0; i < 17; ++i)
        {
            // multiply move_vector by speed
            ////////////////////////////////////////
            Ogre::Vector2 speed = direction;

            ////////////////////////////////////////
            //LOGGER->Log(LOGGER_WARNING, "Move vector with speed: %f %f.", direction.x, direction.y);
            // get ending point
            end_point = Ogre::Vector3(start_point.x + speed.x,start_point.y,start_point.z + speed.y);
            //LOGGER->Log(LOGGER_WARNING, "End point: %f %f %f.", end_point.x, end_point.y, end_point.z);



            // 1st check
            // rotate move_vector +45
            q1.FromAngleAxis(Ogre::Radian(Ogre::Degree(45)), Ogre::Vector3::UNIT_Y);
            rotation.x = direction.x;
            rotation.y = 0.0f;
            rotation.z = direction.y;
            rotation = q1 * rotation;

            // multiply move_vector by solid range
            rotation = rotation * 20.0f;
            end_point2.x = end_point.x + rotation.x;
            end_point2.z = end_point.z + rotation.z;

            // check_triangle
            first_triangle_check = walkmeshBorderCross(evt,end_point2,direction);
            mEventInfo.find(evt)->second.triangle = curTri;

            // model_check
            first_entity_check = checkCollisions(evt, end_point2);




            // 2nd check
            // rotate move_vector +45
            q1.FromAngleAxis(Ogre::Radian(Ogre::Degree(-45)), Ogre::Vector3::UNIT_Y);
            rotation.x = direction.x;
            rotation.y = 0.0f;
            rotation.z = direction.y;
            rotation = q1 * rotation;

            // multiply move_vector by solid range
            rotation = rotation * 20.0f;
            end_point2.x = end_point.x + rotation.x;
            end_point2.z = end_point.z + rotation.z;

            // check triangle
            second_triangle_check = walkmeshBorderCross(evt,end_point2,direction);
            mEventInfo.find(evt)->second.triangle = curTri;

            // model_check
            second_entity_check = checkCollisions(evt, end_point2);



            // 3rd check
            // multiply move_vector by solid range
            //rotation *= m_Models[model_id]->GetSolidRange();
            rotation.x = direction.x;
            rotation.y = 0.0f;
            rotation.z = direction.y;
            rotation = rotation * 20.0f;
            end_point2.x = end_point.x + rotation.x;
            end_point2.z = end_point.z + rotation.z;

            // check triangle
            third_triangle_check = walkmeshBorderCross(evt,end_point2,direction);
            mEventInfo.find(evt)->second.triangle = curTri;

            // model_check
            third_entity_check = checkCollisions(evt, end_point2);

            // check condition and modify move_vector
            if (first_triangle_check  != false || second_triangle_check != false || third_triangle_check  != false ||
                first_entity_check    != false || second_entity_check   != false || third_entity_check    != false)
            {
                {
                    if (first_entity_check  == false &&
                        second_entity_check == false &&
                        third_entity_check  == false)
                    {
                        // if not both left and right check was fail
                        if (first_triangle_check == false || second_triangle_check == false)
                        {
                            if ((first_triangle_check == false && first_entity_check != false) ||
                                (first_triangle_check != false && second_triangle_check == false))
                            {
                                q1.FromAngleAxis(Ogre::Radian(Ogre::Degree(-11.25f)), Ogre::Vector3::UNIT_Y);
                                rotation.x = direction.x;
                                rotation.z = direction.y;
                                rotation = q1 * rotation;
                                direction.x = rotation.x;
                                direction.y = rotation.z;
                            }

                            if (first_triangle_check == false &&
                                first_entity_check == false &&
                                (second_triangle_check != false || second_entity_check != false))
                            {
                                q1.FromAngleAxis(Ogre::Radian(Ogre::Degree(11.25f)), Ogre::Vector3::UNIT_Y);
                                rotation.x = direction.x;
                                rotation.z = direction.y;
                                rotation = q1 * rotation;
                                direction.x = rotation.x;
                                direction.y = rotation.z;
                            }

                            continue;
                        }

                        break;
                    }
                }
            }
        }

        // set new X, Y and Z
        last_triangle_check = walkmeshBorderCross(evt,end_point,direction);

        if (first_triangle_check  != false ||
            second_triangle_check != false ||
            third_triangle_check  != false ||
            last_triangle_check   != false ||
            first_entity_check    != false ||
            second_entity_check   != false ||
            third_entity_check    != false)
        {
            return;
        }

        evt->setPosition(end_point);
    }
} // namespace
