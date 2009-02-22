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

#include <typeinfo>
#include <cmath>
#include <map>
#include <stack>
#include <vector>
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
        inline Variable() : _int(0),mType(VT_INT32) {}

        inline Variable(VariableType aType,float value)
        {
            setType(aType);

            switch (mType)
            {
                case VT_INT32:
                    _int = static_cast<int32>(value);
                break;

                case VT_FLOAT:
                    _float = value;
                    floatCheck();
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

        static Variable pow(const Variable &base,const Variable &exp);
        static Variable sqrt(const Variable &value);
        static Variable sin(const Variable &radians);
        static Variable cos(const Variable &radians);
        static Variable tan(const Variable &radians);

        template<typename T> T getValue(bool strongTyping) const
        {
            // <todo> Remove RTTI; I didn't know it was
            // slow like people say it is
            // I could even have used VariableType here as template
            // parameter; I'd better separate this in two methods, though
            if (typeid(T) == typeid(int32)) {
                if (mType == VT_INT32) {
                    return static_cast<T>(_int);
                } else {
                    if (!strongTyping) {
                        return static_cast<T>(_float);
                    } else {
                        SONETTO_THROW("Variable isn't of requested type "
                                "(requested int32)");
                    }
                }
            } else
            if (typeid(T) == typeid(float)) {
                if (mType == VT_FLOAT) {
                    return static_cast<T>(_float);
                } else {
                    if (!strongTyping) {
                        return static_cast<T>(_int);
                    } else {
                        SONETTO_THROW("Variable isn't of requested type "
                                "(requested float)");
                    }
                }
            } else {
                SONETTO_THROW("Cannot convert Variable to type `" +
                        std::string(typeid(T).name()) + "'");
            }
        }

        VariableType getType() const;
        inline char &_getRawType() { return mType; }

        union
        {
            int32 _int;
            float _float;
        };

    private:
        void setType(VariableType type);

        inline void floatCheck()
        {
            int32 nint = (int32)(std::floor(_float));
            if (nint == _float)
            {
                mType = VT_INT32;
                _int = nint;
            }
        }

        char mType;
    };

    struct VariableCondition
    {
        VariableCondition() {}

        uint8 scope;
        uint32 variableID;
        uint8 comparator;
        Variable rhsValue;
    };

    typedef std::map<int,Variable> VariableMap;
    typedef std::stack<Variable> VariableStack;
    typedef std::vector<VariableCondition> VariableConditionVector;
} // namespace

#endif
