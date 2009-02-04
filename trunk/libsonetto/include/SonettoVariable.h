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

#ifndef SONETTO_VARIABLE_H
#define SONETTO_VARIABLE_H

#include <cmath>
#include <map>
#include <stack>
#include "SonettoPrerequisites.h"

namespace Sonetto
{
    enum VariableType
    {
        VT_INT32,
        VT_FLOAT
    };

    enum VariableComparator
    {
        VCMP_EQUAL_TO,
        VCMP_NOT_EQUAL_TO,
        VCMP_GREATER_THAN,
        VCMP_GREATER_THAN_OR_EQUAL_TO,
        VCMP_LESSER_THAN,
        VCMP_LESSER_THAN_OR_EQUAL_TO
    };

    enum VariableScope
    {
        VS_LOCAL,
        VS_GLOBAL
    };

    class SONETTO_API Variable
    {
    public:
        inline Variable() : type(VT_INT32),_int(0) {}

        inline Variable(VariableType aType,float value)
                : type(aType)
        {
            switch (aType)
            {
                case VT_INT32:
                    _int = static_cast<int32>(value);
                break;

                case VT_FLOAT:
                    _float = value;
                break;
            }
        }

        bool operator==(const Variable &rhs);
        bool operator!=(const Variable &rhs);
        bool operator>(const Variable &rhs);
        bool operator>=(const Variable &rhs);
        bool operator<(const Variable &rhs);
        bool operator<=(const Variable &rhs);
        bool compare(VariableComparator comparator,const Variable &rhs);

        Variable operator+(const Variable &rhs);
        inline void operator+=(const Variable &rhs) { *this = (*this) + rhs; }
        Variable operator-(const Variable &rhs);
        inline void operator-=(const Variable &rhs) { *this = (*this) - rhs; }
        Variable operator*(const Variable &rhs);
        inline void operator*=(const Variable &rhs) { *this = (*this) * rhs; }
        Variable operator/(const Variable &rhs);
        inline void operator/=(const Variable &rhs) { *this = (*this) / rhs; }

        Variable pow(const Variable &exp);
        Variable sqrt();
        static Variable sin(const Variable &radians);
        static Variable cos(const Variable &radians);
        static Variable tan(const Variable &radians);

        char type;
        union
        {
            int32 _int;
            float _float;
        };

    private:
        inline bool floatCheck()
        {
            float floored = std::floor(_float);
            int32 nint = (int32)floored;

            if (nint == _float)
            {
                type = VT_INT32;
                _int = nint;

                return true;
            }

            return false;
        }
    };

    typedef std::map<int,Variable> VariableMap;
    typedef std::stack<Variable> VariableStack;
} // namespace

#endif
