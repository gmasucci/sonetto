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

#include <cmath>
#include "SonettoMath.h"

namespace Sonetto
{
    //--------------------------------------------------------------------------
    // Sonetto::Math implementation.
    //--------------------------------------------------------------------------
    int Math::ipow(int base,int exp)
    {
        if (exp < 0)
        {
            return 0;
        }

        if (exp == 0)
        {
            return 1;
        }

        for (int i = 0;i < exp;++i)
        {
            base *= base;
        }

        return base;
    }
    //--------------------------------------------------------------------------
    float Math::frand(float from,float to)
    {
        assert(to - from > 0.0f);

        // <todo> Take into account std::RAND_MAX in 64bit platforms
        uint32 val = ((rand() & RAND_MAX) << 4) | (rand() & RAND_MAX);
        uint32 max = (RAND_MAX << 4) | RAND_MAX;
        return from + ( ((float)val / (float)max) * (to - from) );
    }
}
