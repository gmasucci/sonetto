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
#include "SonettoVariable.h"

namespace Sonetto
{
    //--------------------------------------------------------------------------
    // Temporary simplePow helper function
    //--------------------------------------------------------------------------
    inline int simplePow(int base,int exp)
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
    // Sonetto::Variable implementation.
    //--------------------------------------------------------------------------
    bool Variable::operator==(const Variable &rhs)
    {
        switch (getType())
        {
            case VT_INT32:
                switch (rhs.getType())
                {
                    case VT_INT32:
                        return (_int == rhs._int);
                    break;

                    case VT_FLOAT:
                        return (_int == rhs._float);
                    break;
                }
            break;

            case VT_FLOAT:
                switch (rhs.getType())
                {
                    case VT_INT32:
                        return (_float == rhs._int);
                    break;

                    case VT_FLOAT:
                        return (_float == rhs._float);
                    break;
                }
            break;
        }

        return false;
    }
    //--------------------------------------------------------------------------
    bool Variable::operator!=(const Variable &rhs)
    {
        switch (getType())
        {
            case VT_INT32:
                switch (rhs.getType())
                {
                    case VT_INT32:
                        return (_int != rhs._int);
                    break;

                    case VT_FLOAT:
                        return (_int != rhs._float);
                    break;
                }
            break;

            case VT_FLOAT:
                switch (rhs.getType())
                {
                    case VT_INT32:
                        return (_float != rhs._int);
                    break;

                    case VT_FLOAT:
                        return (_float != rhs._float);
                    break;
                }
            break;
        }

        return false;
    }
    //--------------------------------------------------------------------------
    bool Variable::operator>(const Variable &rhs)
    {
        switch (getType())
        {
            case VT_INT32:
                switch (rhs.getType())
                {
                    case VT_INT32:
                        return (_int > rhs._int);
                    break;

                    case VT_FLOAT:
                        return (_int > rhs._float);
                    break;
                }
            break;

            case VT_FLOAT:
                switch (rhs.getType())
                {
                    case VT_INT32:
                        return (_float > rhs._int);
                    break;

                    case VT_FLOAT:
                        return (_float > rhs._float);
                    break;
                }
            break;
        }

        return false;
    }
    //--------------------------------------------------------------------------
    bool Variable::operator>=(const Variable &rhs)
    {
        switch (getType())
        {
            case VT_INT32:
                switch (rhs.getType())
                {
                    case VT_INT32:
                        return (_int >= rhs._int);
                    break;

                    case VT_FLOAT:
                        return (_int >= rhs._float);
                    break;
                }
            break;

            case VT_FLOAT:
                switch (rhs.getType())
                {
                    case VT_INT32:
                        return (_float >= rhs._int);
                    break;

                    case VT_FLOAT:
                        return (_float >= rhs._float);
                    break;
                }
            break;
        }

        return false;
    }
    //--------------------------------------------------------------------------
    bool Variable::operator<(const Variable &rhs)
    {
        switch (getType())
        {
            case VT_INT32:
                switch (rhs.getType())
                {
                    case VT_INT32:
                        return (_int < rhs._int);
                    break;

                    case VT_FLOAT:
                        return (_int < rhs._float);
                    break;
                }
            break;

            case VT_FLOAT:
                switch (rhs.getType())
                {
                    case VT_INT32:
                        return (_float < rhs._int);
                    break;

                    case VT_FLOAT:
                        return (_float < rhs._float);
                    break;
                }
            break;
        }

        return false;
    }
    //--------------------------------------------------------------------------
    bool Variable::operator<=(const Variable &rhs)
    {
        switch (getType())
        {
            case VT_INT32:
                switch (rhs.getType())
                {
                    case VT_INT32:
                        return (_int <= rhs._int);
                    break;

                    case VT_FLOAT:
                        return (_int <= rhs._float);
                    break;
                }
            break;

            case VT_FLOAT:
                switch (rhs.getType())
                {
                    case VT_INT32:
                        return (_float <= rhs._int);
                    break;

                    case VT_FLOAT:
                        return (_float <= rhs._float);
                    break;
                }
            break;
        }

        return false;
    }
    //--------------------------------------------------------------------------
    bool Variable::compare(VariableComparator comparator,const Variable &rhs)
    {
        switch (comparator)
        {
            case VCMP_EQUAL_TO:
                return (*this == rhs);
            break;

            case VCMP_NOT_EQUAL_TO:
                return (*this != rhs);
            break;

            case VCMP_GREATER_THAN:
                return (*this > rhs);
            break;

            case VCMP_GREATER_THAN_OR_EQUAL_TO:
                return (*this >= rhs);
            break;

            case VCMP_LESSER_THAN:
                return (*this < rhs);
            break;

            case VCMP_LESSER_THAN_OR_EQUAL_TO:
                return (*this <= rhs);
            break;

            default:
                SONETTO_THROW("Invalid variable comparator");
            break;
        }
    }
    //--------------------------------------------------------------------------
    Variable Variable::operator+(const Variable &rhs)
    {
        switch (getType())
        {
            case VT_INT32:
                switch (rhs.getType())
                {
                    case VT_INT32:
                        return Variable(VT_INT32,_int + rhs._int);
                    break;

                    case VT_FLOAT:
                        return Variable(VT_FLOAT,_int + rhs._float);
                    break;
                }
            break;

            case VT_FLOAT:
                switch (rhs.getType())
                {
                    case VT_INT32:
                        return Variable(VT_FLOAT,_float + rhs._int);
                    break;

                    case VT_FLOAT:
                        return Variable(VT_FLOAT,_float + rhs._float);
                    break;
                }
            break;
        }

        // Avoids warnings
        return Variable();
    }
    //--------------------------------------------------------------------------
    Variable Variable::operator-(const Variable &rhs)
    {
        switch (getType())
        {
            case VT_INT32:
                switch (rhs.getType())
                {
                    case VT_INT32:
                        return Variable(VT_INT32,_int - rhs._int);
                    break;

                    case VT_FLOAT:
                        return Variable(VT_FLOAT,_int - rhs._float);
                    break;
                }
            break;

            case VT_FLOAT:
                switch (rhs.getType())
                {
                    case VT_INT32:
                        return Variable(VT_FLOAT,_float - rhs._int);
                    break;

                    case VT_FLOAT:
                        return Variable(VT_FLOAT,_float - rhs._float);
                    break;
                }
            break;
        }

        // Avoids warnings
        return Variable();
    }
    //--------------------------------------------------------------------------
    Variable Variable::operator*(const Variable &rhs)
    {
        switch (getType())
        {
            case VT_INT32:
                switch (rhs.getType())
                {
                    case VT_INT32:
                        return Variable(VT_INT32,_int * rhs._int);
                    break;

                    case VT_FLOAT:
                        return Variable(VT_FLOAT,_int * rhs._float);
                    break;
                }
            break;

            case VT_FLOAT:
                switch (rhs.getType())
                {
                    case VT_INT32:
                        return Variable(VT_FLOAT,_float * rhs._int);
                    break;

                    case VT_FLOAT:
                        return Variable(VT_FLOAT,_float * rhs._float);
                    break;
                }
            break;
        }

        // Avoids warnings
        return Variable();
    }
    //--------------------------------------------------------------------------
    Variable Variable::operator/(const Variable &rhs)
    {
        switch (getType())
        {
            case VT_INT32:
                switch (rhs.getType())
                {
                    case VT_INT32:
                        return Variable(VT_FLOAT,(float)(_int) / (float)(rhs._int));
                    break;

                    case VT_FLOAT:
                        return Variable(VT_FLOAT,_int / rhs._float);
                    break;
                }
            break;

            case VT_FLOAT:
                switch (rhs.getType())
                {
                    case VT_INT32:
                        return Variable(VT_FLOAT,_float / rhs._int);
                    break;

                    case VT_FLOAT:
                        return Variable(VT_FLOAT,_float / rhs._float);
                    break;
                }
            break;
        }

        // Avoids warnings
        return Variable();
    }
    //--------------------------------------------------------------------------
    Variable Variable::pow(const Variable &base,const Variable &exp)
    {
        switch (base.getType())
        {
            case VT_INT32:
                switch (exp.getType())
                {
                    case VT_INT32:
                        if (exp._int < 0) {
                            //return Variable(VT_FLOAT,std::pow(
                            //        (float)base._int,(float)exp._int));
                        } else {
                            return Variable(VT_INT32,
                                    simplePow(base._int,exp._int));
                        }
                    break;

                    case VT_FLOAT:
                        //return Variable(VT_FLOAT,std::pow(
                        //        (float)base._int,exp._float));
                    break;
                }
            break;

            case VT_FLOAT:
                switch (exp.getType())
                {
                    case VT_INT32:
                        //return Variable(VT_FLOAT,std::pow(
                        //        base._float,(float)exp._int));
                    break;

                    case VT_FLOAT:
                        //return Variable(VT_FLOAT,std::pow(
                        //        base._float,exp._float));
                    break;
                }
            break;
        }

        // Avoids warnings
        return Variable();
    }
    //--------------------------------------------------------------------------
    Variable Variable::sqrt(const Variable &value)
    {
        switch (value.getType())
        {
            case VT_INT32:
                return Variable(VT_FLOAT,std::sqrt((float)value._int));
            break;

            case VT_FLOAT:
                return Variable(VT_FLOAT,std::sqrt(value._float));
            break;
        }

        // Avoids warnings
        return Variable();
    }
    //--------------------------------------------------------------------------
    Variable Variable::sin(const Variable &radians)
    {
        switch (radians.getType())
        {
            case VT_INT32:
                //return Variable(VT_FLOAT,gccsinfhack((float)radians._int));
            break;

            case VT_FLOAT:
                //return Variable(VT_FLOAT,gccsinfhack(radians._float));
            break;
        }

        // Avoids warnings
        return Variable();
    }
    //--------------------------------------------------------------------------
    Variable Variable::cos(const Variable &radians)
    {
        switch (radians.getType())
        {
            case VT_INT32:
                return Variable(VT_FLOAT,std::cos((float)radians._int));
            break;

            case VT_FLOAT:
                return Variable(VT_FLOAT,std::cos(radians._float));
            break;
        }

        // Avoids warnings
        return Variable();
    }
    //--------------------------------------------------------------------------
    Variable Variable::tan(const Variable &radians)
    {
        switch (radians.getType())
        {
            case VT_INT32:
                return Variable(VT_FLOAT,std::tan((float)radians._int));
            break;

            case VT_FLOAT:
                return Variable(VT_FLOAT,std::tan(radians._float));
            break;
        }

        // Avoids warnings
        return Variable();
    }
    //--------------------------------------------------------------------------
    void Variable::setType(VariableType type)
    {
        if (type != VT_INT32 && type != VT_FLOAT)
        {
            SONETTO_THROW("Variable type is corrupted");
        }

        mType = type;
    }
    //--------------------------------------------------------------------------
    VariableType Variable::getType() const
    {
        if (mType != VT_INT32 && mType != VT_FLOAT)
        {
            SONETTO_THROW("Variable type is corrupted");
        }

        return (VariableType)mType;
    }
} // namespace
