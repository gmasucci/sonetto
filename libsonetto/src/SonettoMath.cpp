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

#include <OgreVector2.h>
#include "SonettoMath.h"

namespace Sonetto
{
    //-------------------------------------------------------------------------
    // Sonetto::Math implementation.
    //-------------------------------------------------------------------------
    float Math::pointElevation(const Ogre::Vector3 &point,
            const Ogre::Vector3 &A,const Ogre::Vector3 &B,
            const Ogre::Vector3 &C)
    {
        float _A = A.y * (B.z - C.z) + B.y * (C.z - A.z) + C.y * (A.z - B.z);
        float _B = A.z * (B.x - C.x) + B.z * (C.x - A.x) + C.z * (A.x - B.x);
        float _C = A.x * (B.y - C.y) + B.x * (C.y - A.y) + C.x * (A.y - B.y);
        float _D = A.x * (B.y * C.z - C.y * B.z) + B.x *
                (C.y * A.z - A.y * C.z) + C.x * (A.y * B.z - B.y * A.z);

        return (_D - _C * point.z - _A * point.x) / _B;
    }
    //-------------------------------------------------------------------------
    bool Math::lineCrosses(const Ogre::Vector3 &p11,const Ogre::Vector3 &p12,
            const Ogre::Vector3 &p21,const Ogre::Vector3 &p22)
    {
        float D  = (p12.z - p11.z) * (p21.x - p22.x) - (p21.z - p22.z) *
                (p12.x - p11.x);
        float D1 = (p12.z - p11.z) * (p21.x - p11.x) - (p21.z - p11.z) *
                (p12.x - p11.x);
        float D2 = (p21.z - p11.z) * (p21.x - p22.x) - (p21.z - p22.z) *
                (p21.x - p11.x);

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

        // if our line ends on triangle border - we cross the line
        // otherwise not so we check 0 < t1 instead of 0 <= t1
        if ((0 < t1) && (t1 <= 1) && (0 <= t2) && (t2 <= 1))
        {
    //        x = p11.x + (p12.x - p11.x) * (t1 - 0.01f);
    //        y = p11.y + (p12.y - p11.y) * (t1 - 0.01f);
    //        z = p11.z + (p12.z - p11.z) * (t1 - 0.01f);
            return true;
        }

        return false;
    }
    //---------------------------------------------------------------------
    Ogre::Vector3 Math::projectOnLine(const Ogre::Vector3 &move,
            const Ogre::Vector3 &sp1,const Ogre::Vector3 &sp2)
    {
        Ogre::Vector3 v(0.0f,0.0f,0.0f);
        v.x = sp2.x - sp1.x;
        v.z = sp2.z - sp1.z;
        v.normalise();

        v *= move.x * v.x + move.z * v.z;
        return v;
    }
    //---------------------------------------------------------------------
    bool Math::circleLineIntersection(float circleRadius,
            Ogre::Vector3 lineStart,Ogre::Vector3 lineEnd,
            Ogre::Vector3 &iA,Ogre::Vector3 &iB)
    {
        float dX = lineEnd.x - lineStart.x;
        float dY = lineEnd.y - lineStart.y;

        float dR = sqrt( (dX * dX) + (dY * dY) );

        float D = (lineStart.x * lineEnd.y) - (lineEnd.x * lineStart.y);

        float delta = sqrt( ((circleRadius * circleRadius) * ( dR * dR )) - (D * D) );

        if (delta > 0)
        {
            float dR_squared = dR * dR;
            iA.x = ( (D * dY) + (sign(dY) * dX * delta) ) / dR_squared;
            iB.x = ( (D * dY) - (sign(dY) * dX * delta) ) / dR_squared;

            iA.y = ( - (D * dX) + (fabs(dY) * delta) ) / dR_squared;
            iB.y = ( - (D * dX) - (fabs(dY) * delta) ) / dR_squared;
        }

        return (delta > 0);
    }
    //---------------------------------------------------------------------
    float Math::sideOfVector(const Ogre::Vector3 &point,
            const Ogre::Vector3 &p1,const Ogre::Vector3 &p2)
    {
        Ogre::Vector3 AB = p2    - p1;
        Ogre::Vector3 AP = point - p1;
        return AB.x * AP.z - AB.z * AP.x;
    }
} // namespace
