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
        // Hardcoded walkmesh
        /*Walkmesh::Triangle tri;
        float width = 13.0f, height = 17.0f;

        mWalkmesh.vertex.push_back(Ogre::Vector3((0.088397f-0.5f)*width, 0.0f, (0.238805f-0.5f)*height));
        mWalkmesh.vertex.push_back(Ogre::Vector3((0.386740f-0.5f)*width, 0.0f, (0.0f-0.5f)*height));
        mWalkmesh.vertex.push_back(Ogre::Vector3((0.784530f-0.5f)*width, 0.0f, (0.089552f-0.5f)*height));
        mWalkmesh.vertex.push_back(Ogre::Vector3((1.0f-0.5f)*width,      0.0f, (0.582089f-0.5f)*height));
        mWalkmesh.vertex.push_back(Ogre::Vector3((0.767955f-0.5f)*width, 0.0f, (0.890547f-0.5f)*height));
        mWalkmesh.vertex.push_back(Ogre::Vector3((0.088397f-0.5f)*width, 0.0f, (1.0f-0.5f)*height));
        mWalkmesh.vertex.push_back(Ogre::Vector3((0.0f-0.5f)*width,      0.0f, (0.726368f-0.5f)*height));
        mWalkmesh.vertex.push_back(Ogre::Vector3((0.265193f-0.5f)*width, 0.0f, (0.567164f-0.5f)*height));

        // 0th triangle
        // Vertices
        tri.vertexNum[0] = 0;
        tri.vertexNum[1] = 1;
        tri.vertexNum[2] = 2;

        // Border Links
        tri.borderLink[2] = 1;

        // Push
        mWalkmesh.triangle.push_back(tri);
        tri = Walkmesh::Triangle();

        // 1st triangle
        // Vertices
        tri.vertexNum[0] = 0;
        tri.vertexNum[1] = 2;
        tri.vertexNum[2] = 7;

        // Border Links
        tri.borderLink[0] = 0;
        tri.borderLink[1] = 2;

        // Push
        mWalkmesh.triangle.push_back(tri);
        tri = Walkmesh::Triangle();

        // 2nd triangle
        // Vertices
        tri.vertexNum[0] = 7;
        tri.vertexNum[1] = 2;
        tri.vertexNum[2] = 3;

        // Border Links
        tri.borderLink[0] = 1;
        tri.borderLink[2] = 3;

        // Push
        mWalkmesh.triangle.push_back(tri);
        tri = Walkmesh::Triangle();

        // 3rd triangle
        // Vertices
        tri.vertexNum[0] = 7;
        tri.vertexNum[1] = 3;
        tri.vertexNum[2] = 4;

        // Border Links
        tri.borderLink[0] = 2;
        tri.borderLink[2] = 4;

        // Push
        mWalkmesh.triangle.push_back(tri);
        tri = Walkmesh::Triangle();

        // 4th triangle
        // Vertices
        tri.vertexNum[0] = 7;
        tri.vertexNum[1] = 4;
        tri.vertexNum[2] = 5;

        // Border Links
        tri.borderLink[0] = 3;
        tri.borderLink[2] = 5;

        // Push
        mWalkmesh.triangle.push_back(tri);
        tri = Walkmesh::Triangle();

        // 5th triangle
        // Vertices
        tri.vertexNum[0] = 7;
        tri.vertexNum[1] = 5;
        tri.vertexNum[2] = 6;

        // Border Links
        tri.borderLink[0] = 4;

        // Push
        mWalkmesh.triangle.push_back(tri);
        tri = Walkmesh::Triangle();*/

        // Convert mesh
        std::vector<int> triangle;

        CollisionUtils::convertMeshData(entity,&mWalkmesh.vertex,&triangle);
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

                        if (triangle[k] == searchVerts[0])
                            ++sharedVerts;
                        if (triangle[k] == searchVerts[1])
                            ++sharedVerts;

                        if (triangle[k+1] == searchVerts[0])
                            ++sharedVerts;
                        if (triangle[k+1] == searchVerts[1])
                            ++sharedVerts;

                        if (triangle[k+2] == searchVerts[0])
                            ++sharedVerts;
                        if (triangle[k+2] == searchVerts[1])
                            ++sharedVerts;

                        if (sharedVerts >= 2)
                        {
                            std::cout << "Found " << sharedVerts << " vertices "
                                    "shared between triangles " <<i/3 << " and " <<
                                    k/3 << ".\n";
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
    Ogre::Vector3 CollisionManager::setNextPos(EventObject *evt,
            const Ogre::Vector3 &move)
    {
        // <todo> Set this thing to NULL
        const Walkmesh::Triangle *prevTri = (Walkmesh::Triangle *)0x01; // Previous triangle

        Ogre::Vector3 pos  = evt->getPosition(); // Current position
        Ogre::Vector3 dest = pos+move;           // Destination coordinates

        // Current triangle over which is the event
        const Walkmesh::Triangle *tri = &mWalkmesh.triangle[evt->triangle];

        if (move.x == 0.0f && move.z == 0.0f)
        {
            return dest;
        }

        while (true)
        {
            char  cross = -1; // What border does the move cross? (-1 for none)
            const Walkmesh::Triangle *nextTri = NULL; // Destination triangle

            // Gets vertices' coordinates
            Ogre::Vector3 vA = mWalkmesh.vertex[tri->vertexNum[0]];
            Ogre::Vector3 vB = mWalkmesh.vertex[tri->vertexNum[1]];
            Ogre::Vector3 vC = mWalkmesh.vertex[tri->vertexNum[2]];

            // Gets border links from current triangle to other triangles
            const Walkmesh::Triangle *tA = NULL;
            const Walkmesh::Triangle *tB = NULL;
            const Walkmesh::Triangle *tC = NULL;

            if (tri->borderLink[0] != -1)
                tA = &mWalkmesh.triangle[tri->borderLink[0]];
            if (tri->borderLink[1] != -1)
                tB = &mWalkmesh.triangle[tri->borderLink[1]];
            if (tri->borderLink[2] != -1)
                tC = &mWalkmesh.triangle[tri->borderLink[2]];

            /*system("cls");
            std::cout << "Current position: " << pos << "\n";
            std::cout << "Geometry of triangle " << evt->triangle << ":\n";
            std::cout << "vA: " << vA << "\t(" << tri->vertexNum[0] << ")\nvB" <<
                    vB << "\t(" << tri->vertexNum[1] << ")\nvC: " << vC << "\t(" <<
                    tri->vertexNum[2] << ")\n";
            std::cout << "tA: " << tri->borderLink[0] << "\ntB: " << tri->borderLink[1] <<
                    "\ntC: " << tri->borderLink[2] << "\n";
            std::cout << "Inside triange? " << CollisionUtils::pointInTriangle(pos,vA,vB,vC) << "\n";*/

            /*std::cout << "AB: " << (Ogre::Vector3(0.5f,0.0f,-1.0f)-
                Ogre::Vector3(0.0f,0.0f,0.0f)).length() << "\nBC: " <<
                (Ogre::Vector3(0.5f,0.0f,-1.0f)-Ogre::Vector3(1.0f,0.0f,0.0f)).length() << "\nAC: " <<
                (Ogre::Vector3(1.0f,0.0f,0.0f)-Ogre::Vector3(0.0f,0.0f,0.0f)).length() << "\n";

            std::cout << CollisionUtils::triangleXZArea(Ogre::Vector3(0.0f,0.0f,0.0f),
                    Ogre::Vector3(0.5f,0.0f,-1.0f),Ogre::Vector3(1.0f,0.0f,0.0f)) << " <<\n";*/

            // Checks what border the move would cross
            if (CollisionUtils::lineCrosses(pos,dest,vA,vB) && tA != prevTri) {
                cross   = 0;
                nextTri = tA;
            } else
            if (CollisionUtils::lineCrosses(pos,dest,vB,vC) && tB != prevTri) {
                cross   = 1;
                nextTri = tB;
            } else
            if (CollisionUtils::lineCrosses(pos,dest,vA,vC) && tC != prevTri) {
                cross   = 2;
                nextTri = tC;
            }

            if (CollisionUtils::pointOnLine(vA,pos,dest) ||
                CollisionUtils::pointOnLine(vB,pos,dest) ||
                CollisionUtils::pointOnLine(vC,pos,dest) )
            {
                // std::cout << "Point on line\n";
                // Sleep(3000);
            }

            if (cross == -1) { // No borders being crossed
                dest.y = CollisionUtils::getPointElevation(dest,vA,vB,vC);

                // <todo> Check for collisions

                return dest;
            } else { // A border is being crossed
                //std::cout << "[!] Crossing border: " << (int)cross << std::endl;
                //std::cout << "[!] prevTri: " << (int)prevTri << "\n";
                //Sleep(1000);

                // If this border gives access to another triangle (nextTri != NULL),
                // we set the current triangle to this one and recalculate everything
                // If not, we make the vector slide across the border
                if (tri->borderLink[(int)cross] != -1) { // Goes to next triangle
                    // Makes sure the move won't cross a triangle's corner, moving the
                    // destination coordinates a little bit and rechecking everything
                    if (CollisionUtils::pointOnLine(vA,pos,dest) ||
                        CollisionUtils::pointOnLine(vB,pos,dest) ||
                        CollisionUtils::pointOnLine(vC,pos,dest) )
                    {
                        dest.x += 0.2f;
                        dest.z += 0.2f;
                        Kernel::get()->mLogMan->logMessage("CollisionManager: Avoiding "
                                "to cross a triangle's corner.");
                        // Sleep(1000);
                        continue;
                        //return pos;
                        //Ogre::Vector3 sp1 = (cross == 0 || cross == 2) ? vA : vB;
                        //Ogre::Vector3 sp2 = (cross == 0) ? vB : vC;
                        //return setNextPos(evt,CollisionUtils::getProjectionOnLine(move,sp1,sp2));
                    }

                    // Moves to the next triangle
                    evt->triangle = tri->borderLink[(int)cross];
                    prevTri = tri;
                    tri = nextTri;
                } else { // Slides on border
                    Ogre::Vector3 sp1 = (cross == 0 || cross == 2) ? vA : vB;
                    Ogre::Vector3 sp2 = (cross == 0) ? vB : vC;

                    // std::
                    return setNextPos(evt,CollisionUtils::getProjectionOnLine(move,sp1,sp2));
                    // return pos;
                }
            }
        }
    }
} // namespace
