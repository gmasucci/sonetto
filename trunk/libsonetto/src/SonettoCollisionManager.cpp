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
#include "SonettoKernel.h"
#include "SonettoEventObject.h"
#include "SonettoCollisionManager.h"

namespace Sonetto
{
    //-------------------------------------------------------------------------
    // Sonetto::CollisionUtils implementation
    //-------------------------------------------------------------------------
    void convertMeshData(Ogre::Entity *entity,
        std::vector<Ogre::Vector3> *vertexBuf,std::vector<int> *faceBuf)
    {
        //---------------------------------------------------------------------
        // CONVERT MESH DATA
        //---------------------------------------------------------------------
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
    float pointElevation(const Ogre::Vector3 &point,const Ogre::Vector3 &A,
            const Ogre::Vector3 &B,const Ogre::Vector3 &C)
    {
        float _A = A.y * (B.z - C.z) + B.y * (C.z - A.z) + C.y * (A.z - B.z);
        float _B = A.z * (B.x - C.x) + B.z * (C.x - A.x) + C.z * (A.x - B.x);
        float _C = A.x * (B.y - C.y) + B.x * (C.y - A.y) + C.x * (A.y - B.y);
        float _D = A.x * (B.y * C.z - C.y * B.z) + B.x * (C.y * A.z - A.y * C.z) +
                C.x * (A.y * B.z - B.y * A.z);

        return (_D - _C * point.z - _A * point.x) / _B;
    }
    //-------------------------------------------------------------------------
    bool lineCrosses(const Vector3 &p11,const Vector3 &p12,const Vector3 &p21,
            const Vector3 &p22)
    {
        float D  = (p12.z - p11.z) * (p21.x - p22.x) - (p21.z - p22.z) * (p12.x - p11.x);
        float D1 = (p12.z - p11.z) * (p21.x - p11.x) - (p21.z - p11.z) * (p12.x - p11.x);
        float D2 = (p21.z - p11.z) * (p21.x - p22.x) - (p21.z - p22.z) * (p21.x - p11.x);

        // if lines are the same
        if ((D == 0) && (D1 == 0) && (D2 == 0))
        {
            return false;
        }

        // if lines are paralel
        if (D == 0)
        {
            return false;
        }

        // find time
        float t1 = D2 / D;
        float t2 = D1 / D;

        // if our line ends on triangle border - we cross the line otherwise not
        // so we check 0 < t1 instead of 0 <= t1
        if ((0 < t1) && (t1 <= 1) && (0 <= t2) && (t2 <= 1))
        {
    //        x = p11.x + (p12.x - p11.x) * (t1 - 0.01f);
    //        y = p11.y + (p12.y - p11.y) * (t1 - 0.01f);
    //        z = p11.z + (p12.z - p11.z) * (t1 - 0.01f);
            return true;
        }

        return false;
    }
    //-------------------------------------------------------------------------
    Ogre::Vector3 projectOnLine(const Ogre::Vector3 &move,
            const Ogre::Vector3 &sp1,const Ogre::Vector3 &sp2)
    {
        Ogre::Vector3 v(0.0f,0.0f,0.0f);
        v.x = sp2.x - sp1.x;
        v.z = sp2.z - sp1.z;
        v.normalise();

        v *= move.x * v.x + move.z * v.z;
        return v;
    }
    //-------------------------------------------------------------------------
    // Sonetto::CollisionManager implementation.
    //-------------------------------------------------------------------------
    CollisionManager::CollisionManager()
    {
    }
    //-------------------------------------------------------------------------
    CollisionManager::~CollisionManager()
    {
    }
    //-------------------------------------------------------------------------
    bool CollisionManager::createWalkmesh(Ogre::Entity *entity)
    {
        // Convert mesh
        std::vector<int> triangle;

        convertMeshData(entity,&mWalkmesh.vertex,&triangle);
        std::cout << "Vertices: " << mWalkmesh.vertex.size() << "\n" <<
                     "Triangles: " << triangle.size()/3 << "\n";

        for (size_t i = 0;i < triangle.size();i += 3)
        {
            Walkmesh::Triangle tri;
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
                            /*std::cout << "Found " << sharedVerts << " vertices "
                                    "shared between triangles " <<i/3 << " and " <<
                                    k/3 << ".\n";*/
                            tri.borderLink[j] = k/3;
                        }
                    }
                }
            }

            mWalkmesh.triangle.push_back(tri);
        }

        std::cout << "Translated triangles: " << mWalkmesh.triangle.size() << "\n";

        return true;
    }
    //-------------------------------------------------------------------------
    void CollisionManager::setNextPos(EventObject *evt,
            const EventVector &events,const Ogre::Vector3 &move)
    {
        int                  prevTri  = -2; // Why -2? Ctrl-F "<EX0>"
        int                  curTri   = evt->getTriangle();
        const Ogre::Vector3 &curPos   = evt->getPosition();
        Ogre::Vector3        endPoint = curPos + move;

        while (true) // Loops until break
        {
            bool crossA = false; // Was border A crossed?
            bool crossB = false; // Was border B crossed?
            bool crossC = false; // Was border C crossed?

            // `tri' is the current triangle, `vA', `vB' and `vC' are the
            // coordinates of each vertex that compose the current triangle,
            // and `tA', `tB' and `tC' are pointers to the current triangle's
            // neighbors (they can be NULL, in which case the corresponding
            // triangle border is a walkmesh edge)
            const Walkmesh::Triangle *tri = &mWalkmesh.triangle[curTri];
            const Ogre::Vector3      &vA  = mWalkmesh.vertex[tri->vertexNum[0]];
            const Ogre::Vector3      &vB  = mWalkmesh.vertex[tri->vertexNum[1]];
            const Ogre::Vector3      &vC  = mWalkmesh.vertex[tri->vertexNum[2]];
            const Walkmesh::Triangle *tA  = NULL;
            const Walkmesh::Triangle *tB  = NULL;
            const Walkmesh::Triangle *tC  = NULL;

            // Gets triangle neighbors if they are not -1, what would mean the
            // triangle border leads out the walkmesh (i.e. the object should
            // slide if it collides it)
            if (tri->borderLink[0] != -1) // Border A
            {
                tA = &mWalkmesh.triangle[tri->borderLink[0]];
            }

            if (tri->borderLink[1] != -1) // Border B
            {
                tB = &mWalkmesh.triangle[tri->borderLink[1]];
            }

            if (tri->borderLink[2] != -1) // Border C
            {
                tC = &mWalkmesh.triangle[tri->borderLink[2]];
            }

            // We surely have crossed the border that links the current
            // triangle to the previous, but that doesn't matter for us
            // So we do not set a flag for the border leading to the
            // previous triangle
            // <EX0> Here is why we set prevTri to -2 -- It is simply
            // because -1 could be confused with a borderLink of value -1,
            // that mean it didn't have a neighbor
            if (tri->borderLink[0] != prevTri)
            {
                // The line from vertex A to vertex B forms border A
                crossA = lineCrosses(curPos,endPoint,vA,vB);
            }

            if (tri->borderLink[1] != prevTri)
            {
                // The line from vertex B to vertex C forms border B
                crossB = lineCrosses(curPos,endPoint,vB,vC);
            }

            if (tri->borderLink[2] != prevTri)
            {
                // The line from vertex C to vertex A forms border C
                crossC = lineCrosses(curPos,endPoint,vC,vA);
            }

            // If the movement crosses two borders at once, it means it is
            // crossing the shared vertex between them, right?
            // The shared vertex between borders A and B is vertex B
            /* if (crossA && crossB)
            {
                endPoint = vB;
                break;
            }

            // The shared vertex between borders A and C is vertex A
            if (crossA && crossC)
            {
                endPoint = vA;
                break;
            }

            // The shared vertex between borders B and C is vertex C
            if (crossB && crossC)
            {
                endPoint = vC;
                break;
            } */

            if (crossA) { // Border A crossed
                // If the border links to another triangle (i.e. the event is
                // allowed to cross it)
                if (tri->borderLink[0] != -1)
                {
                    // Set previous triangle to current and
                    // current to the next one (the one the border leads to)
                    prevTri = curTri;
                    curTri = tri->borderLink[0];

                    // And then recalculate everything inside it
                    continue;
                }

                // Otherwise we need to slide on it
                endPoint = curPos + projectOnLine(move,vA,vB);
                break;
            } else
            if (crossB) { // Border B crossed
                // If the border links to another triangle (i.e. the event is
                // allowed to cross it)
                if (tri->borderLink[1] != -1)
                {
                    // Set previous triangle to current and
                    // current to the next one (the one the border leads to)
                    prevTri = curTri;
                    curTri = tri->borderLink[1];

                    // And then recalculate everything inside it
                    continue;
                }

                // Otherwise we need to slide on it
                endPoint = curPos + projectOnLine(move,vB,vC);
                break;
            } else
            if (crossC) { // Border C crossed
                // If the border links to another triangle (i.e. the event is
                // allowed to cross it)
                if (tri->borderLink[2] != -1)
                {
                    // Set previous triangle to current and
                    // current to the next one (the one the border leads to)
                    prevTri = curTri;
                    curTri = tri->borderLink[2];

                    // And then recalculate everything inside it
                    continue;
                }

                // Otherwise we need to slide on it
                endPoint = curPos + projectOnLine(move,vA,vC);
                break;
            }

            // If no border were crossed, we just need to set the event's Y
            // coordinate to the same as the ground on the given XZ coordinates
            endPoint.y = pointElevation(endPoint,vA,vB,vC);
            break;
        }

        // Keeps event where it is if there is a collision on its way
        if (evt->getIgnoreCollision() == false)
        {
            // Gets final XZ coordinates
            Ogre::Vector2 coordA(endPoint.x,endPoint.z);

            // Checks collision with each other event
            for (size_t i = 0;i < events.size();++i)
            {
                const EventObject *evt2 = events[i];

                // Skips the this event's very self and events
                // flagged to ignore collisions
                if (evt != evt2 && evt2->getIgnoreCollision() == false)
                {
                    const Ogre::Vector3 &coord3B = evt2->getPosition();
                    float yDistance,yLimit;

                    // Gets Y distance between the two events
                    yDistance = endPoint.y - coord3B.y;

                    // If the distance is negative, `evt' is below
                    // `evt2', so we should use `evt' height or
                    // `evt2' height otherwise
                    if (yDistance < 0.0f) {
                        yLimit = evt->getHeight();
                    } else {
                        yLimit = evt2->getHeight();
                    }

                    // If the Y distance smaller than the height of the
                    // lower event, there *may* be a collision happening
                    if (fabs(yDistance) < yLimit)
                    {
                        Ogre::Vector2 coordB(coord3B.x,coord3B.z);
                        Ogre::Vector2 rel = coordA-coordB;

                        // If the distance between the two events on the XZ
                        // plane if smaller than the sum of each event's
                        // collision radius, there is a collision
                        if (rel.length() < evt->getColRadius() +
                                evt2->getColRadius())
                        {
                            // If there is a collision, we reset the destination
                            // coordinate to the start position and stop searching
                            // for more collisions
                            endPoint = curPos;
                            break;
                        } // W
                    } // e
                } // e
            } // e
        } // !

        // Sets the event to its final position and triangle
        evt->setPosition(endPoint);
        evt->setTriangle(curTri);
    }
} // namespace
