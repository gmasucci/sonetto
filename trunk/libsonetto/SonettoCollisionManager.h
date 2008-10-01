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

		Ogre::Vector3 setNextPos(EventObject *evt,const Ogre::Vector3 &move);

    private:
	    Walkmesh mWalkmesh;
    };

    /// Collision-related math utilities
    class SONETTO_EXPORT CollisionUtils
    {
    public:
        static bool lineCrosses(const Ogre::Vector3 &p11,const Ogre::Vector3 &p12,
                const Ogre::Vector3 &p21,const Ogre::Vector3 &p22)
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

        static bool pointOnLine(const Ogre::Vector3 &point,
                const Ogre::Vector3 &p1,const Ogre::Vector3 &p2)
        {
            float mul = 1000.0f;
            float tx = (point.x*mul - p1.x*mul) / (p2.x*mul - p1.x*mul);
            float tz = (point.z*mul - p1.z*mul) / (p2.z*mul - p1.z*mul);

            /*if (((p1.x == p2.x) && (tz >= 0) && (tz <= 1)) ||
                ((p1.z == p2.z) && (tx >= 0) && (tx <= 1)) ||
                ((tx == tz) && (tx >= 0) && (tx <= 1)))*/
            if (((fabs(p1.x*mul-p2.x*mul) < 0.1f) && (tz >= 0) && (tz <= 1)) ||
                ((fabs(p1.z*mul-p2.z*mul) < 0.1f) && (tx >= 0) && (tx <= 1)) ||
                ((fabs(tx*mul-tz*mul) < 0.1f) && (tx >= 0) && (tx <= 1)))
            {
                return true;
            }

            return false;
        }

        static bool pointInTriangle(const Ogre::Vector3 &point,
                const Ogre::Vector3 &vA,const Ogre::Vector3 &vB,const Ogre::Vector3 &vC)
        {
            Ogre::Vector3 flatPoint = point;
            float area = triangleXZArea(vA,vB,vC);

            flatPoint.y = 0.0f;

            if (fabs((triangleXZArea(flatPoint,vA,vB)+
                triangleXZArea(flatPoint,vB,vC)+
                triangleXZArea(flatPoint,vA,vC))-area) < 0.0001f)
            {
                return true;
            }

            return false;
        }

        inline static float triangleXZArea(const Ogre::Vector3 &vA,const Ogre::Vector3 &vB,
                const Ogre::Vector3 &vC)
        {
            return fabs(vA.x*vB.z+vB.x*vC.z+vC.x*vA.z-vA.x*vC.z-vC.x*vB.z-vB.x*vA.z)/2.0f;
        }

        /** Gets the Y coordinate of the XZ coordinates of a triangle

            Given an Ogre::Vector3, and taking into account its X and Z coordinates,
            this method calculates the Y elevation of that point inside a set of three
            Ogre::Vector3's that describe a 3D triangle.

            Important: This function was copied from Q-Gears.
        @param
            point A point inside the triangle (Y coordinate is ignored).
        @param
            vA First vertex describing the triangle.
        @param
            vB Second vertex describing the triangle.
        @param
            vC Third vertex describing the triangle.
        @return
            The Y-elevation of the given coordinates inside the triangle.
        */
        static float getPointElevation(const Ogre::Vector3 &point,const Ogre::Vector3 &vA,
                const Ogre::Vector3 &vB,const Ogre::Vector3 &vC)
        {
            float _A = vA.y * (vB.z - vC.z) + vB.y * (vC.z - vA.z) + vC.y * (vA.z - vB.z);
            float _B = vA.z * (vB.x - vC.x) + vB.z * (vC.x - vA.x) + vC.z * (vA.x - vB.x);
            float _C = vA.x * (vB.y - vC.y) + vB.x * (vC.y - vA.y) + vC.x * (vA.y - vB.y);
            float _D = vA.x * (vB.y * vC.z - vC.y * vB.z) + vB.x * (vC.y * vA.z - vA.y * vC.z)
                    + vC.x * (vA.y * vB.z - vB.y * vA.z);

            return (_D - _C * point.z - _A * point.x) / _B;
        }

        static Ogre::Vector3 getProjectionOnLine(const Ogre::Vector3 &move,
                const Ogre::Vector3 &sp1,const Ogre::Vector3 &sp2)
        {
            Ogre::Vector2 v(0.0f, 0.0f);
            v.x = sp2.x - sp1.x;
            v.y = sp2.z - sp1.z;
            v.normalise();

            v *= move.x * v.x + move.z * v.y;
            return Ogre::Vector3(v.x,0.0f,v.y);
        }

        static Ogre::Vector3 computeTrianglePoint(size_t triangle[3],const float *verts,float u,float v)
        {
            // Get face in local or global space
            Ogre::Vector3 p0; // = verts[mVRef[0]];
            Ogre::Vector3 p1; // = verts[mVRef[1]];
            Ogre::Vector3 p2; // = verts[mVRef[2]];
            p0.x = verts[triangle[0]*3];
            p0.y = verts[triangle[0]*3+1];
            p0.z = verts[triangle[0]*3+2];
            p1.x = verts[triangle[1]*3];
            p1.y = verts[triangle[1]*3+1];
            p1.z = verts[triangle[1]*3+2];
            p2.x = verts[triangle[2]*3];
            p2.y = verts[triangle[2]*3+1];
            p2.z = verts[triangle[2]*3+2];

            // Compute point coordinates
            Ogre::Vector3 pt = (1.0f - u - v)*p0 + u*p1 + v*p2;

            std::cout << "computeTrianglePoint: " << pt << std::endl;
            return pt;
        }

        static void convertMeshData(Ogre::Entity *entity,
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
    };
} // namespace

#endif
