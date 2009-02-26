/*-----------------------------------------------------------------------------
Copyright (c) 2009, Sonetto Project Developers
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1.  Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
2.  Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
3.  Neither the name of the Sonetto Project nor the names of its contributors
    may be used to endorse or promote products derived from this software
    without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
-----------------------------------------------------------------------------*/

#ifndef SONETTO_MATH_H
#define SONETTO_MATH_H

#include <OgreVector3.h>
#include "SonettoPrerequisites.h"

namespace Sonetto
{
	class SONETTO_API Math
	{
	public:
		Math() {}
		~Math() {}

		template<class type>
		static inline type clamp(type value,type min,type max)
		{
		    if (value < min) {
		        return min;
		    } else
		    if (value > max) {
		        return max;
		    }

		    return value;
		}

		static int ipow(int base,int exp);

		static inline int irand(int from,int to)
		{
		    assert(to - from > 0);
		    return from + (rand() % ((to - from) + 1));
        }

        static float frand(float from,float to);

        static float sideOfVector(const Ogre::Vector3 &point,
            const Ogre::Vector3 &p1,const Ogre::Vector3 &p2);

        static float pointElevation(const Ogre::Vector3 &point,
            const Ogre::Vector3 &A,const Ogre::Vector3 &B,
            const Ogre::Vector3 &C);
	};
} // namespace

#endif
