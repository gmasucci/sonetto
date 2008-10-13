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

#ifndef SONETTO_MATH_H
#define SONETTO_MATH_H

#include <OgreVector3.h>

namespace Sonetto
{
    class Math
    {
    public:
        static float pointElevation(const Ogre::Vector3 &point,
            const Ogre::Vector3 &A,const Ogre::Vector3 &B,
            const Ogre::Vector3 &C);

        static bool lineCrosses(const Ogre::Vector3 &p11,
            const Ogre::Vector3 &p12,const Ogre::Vector3 &p21,
            const Ogre::Vector3 &p22);

        static Ogre::Vector3 projectOnLine(const Ogre::Vector3 &move,
            const Ogre::Vector3 &sp1,const Ogre::Vector3 &sp2);

        static inline float sign(float value)
        {
            if (value > 0.0f) {
                return 1.0f;
            } else
            if (value < 0.0f) {
                return -1.0f;
            }

            return 0.0f;
        }

        static bool circleLineIntersection(float circleRadius,
                Ogre::Vector3 lineStart,Ogre::Vector3 lineEnd,
                Ogre::Vector3 &iA,Ogre::Vector3 &iB);

        static float sideOfVector(const Ogre::Vector3 &point,
            const Ogre::Vector3 &p1,const Ogre::Vector3 &p2);

        static float clamp(float value,float min,float max)
        {
            if (value < 0.0f) {
                return 0.0f;
            } else
            if (value > 1.0f) {
                return 1.0f;
            }

            return value;
        }

    private:
        Math() {}
        ~Math() {}
    };
} // namespace

#endif
