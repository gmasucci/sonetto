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
    /*namespace __GCC_SINF_HACK
    {
        float w00t(float x)
        {
            static const float pi = 3.14159265f;
            static const float circle = pi * 2;
            static const float quarterCircle = circle / 4;

            float shifted = x - quarterCircle;
            if (shifted < 0.0f)
            {
                shifted = circle - shifted;
            }

            return cos(shifted);
        }
    }

    #define gccsinfhack __GCC_SINF_HACK::w00t*/

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
        switch (type)
        {
            case VT_INT32:
                switch (rhs.type)
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
                switch (rhs.type)
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
        switch (type)
        {
            case VT_INT32:
                switch (rhs.type)
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
                switch (rhs.type)
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
        switch (type)
        {
            case VT_INT32:
                switch (rhs.type)
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
                switch (rhs.type)
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
        switch (type)
        {
            case VT_INT32:
                switch (rhs.type)
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
                switch (rhs.type)
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
        switch (type)
        {
            case VT_INT32:
                switch (rhs.type)
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
                switch (rhs.type)
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
        switch (type)
        {
            case VT_INT32:
                switch (rhs.type)
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
                switch (rhs.type)
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
        Variable retn;
        retn.type = 3;

        switch (type)
        {
            case VT_INT32:
                switch (rhs.type)
                {
                    case VT_INT32:
                        retn = Variable(VT_INT32,_int + rhs._int);
                    break;

                    case VT_FLOAT:
                        retn = Variable(VT_FLOAT,_int + rhs._float);
                    break;
                }
            break;

            case VT_FLOAT:
                switch (rhs.type)
                {
                    case VT_INT32:
                        retn = Variable(VT_FLOAT,_float + rhs._int);
                    break;

                    case VT_FLOAT:
                        retn = Variable(VT_FLOAT,_float + rhs._float);
                    break;
                }
            break;
        }

        switch (retn.type)
        {
            case VT_FLOAT:
                retn.floatCheck();
            case VT_INT32:
                return retn;
            break;

            default:
                SONETTO_THROW("Variable has invalid type");
            break;
        }
    }
    //--------------------------------------------------------------------------
    Variable Variable::operator-(const Variable &rhs)
    {
        Variable retn;
        retn.type = 3;

        switch (type)
        {
            case VT_INT32:
                switch (rhs.type)
                {
                    case VT_INT32:
                        retn = Variable(VT_INT32,_int - rhs._int);
                    break;

                    case VT_FLOAT:
                        retn = Variable(VT_FLOAT,_int - rhs._float);
                    break;
                }
            break;

            case VT_FLOAT:
                switch (rhs.type)
                {
                    case VT_INT32:
                        retn = Variable(VT_FLOAT,_float - rhs._int);
                    break;

                    case VT_FLOAT:
                        retn = Variable(VT_FLOAT,_float - rhs._float);
                    break;
                }
            break;
        }

        switch (retn.type)
        {
            case VT_FLOAT:
                retn.floatCheck();
            case VT_INT32:
                return retn;
            break;

            default:
                SONETTO_THROW("Variable has invalid type");
            break;
        }
    }
    //--------------------------------------------------------------------------
    Variable Variable::operator*(const Variable &rhs)
    {
        Variable retn;
        retn.type = 3;

        switch (type)
        {
            case VT_INT32:
                switch (rhs.type)
                {
                    case VT_INT32:
                        retn = Variable(VT_INT32,_int * rhs._int);
                    break;

                    case VT_FLOAT:
                        retn = Variable(VT_FLOAT,_int * rhs._float);
                    break;
                }
            break;

            case VT_FLOAT:
                switch (rhs.type)
                {
                    case VT_INT32:
                        retn = Variable(VT_FLOAT,_float * rhs._int);
                    break;

                    case VT_FLOAT:
                        retn = Variable(VT_FLOAT,_float * rhs._float);
                    break;
                }
            break;
        }

        switch (retn.type)
        {
            case VT_FLOAT:
                retn.floatCheck();
            case VT_INT32:
                return retn;
            break;

            default:
                SONETTO_THROW("Variable has invalid type");
            break;
        }
    }
    //--------------------------------------------------------------------------
    Variable Variable::operator/(const Variable &rhs)
    {
        Variable retn;
        retn.type = 3;

        switch (type)
        {
            case VT_INT32:
                switch (rhs.type)
                {
                    case VT_INT32:
                        retn = Variable(VT_FLOAT,(float)(_int) / (float)(rhs._int));
                    break;

                    case VT_FLOAT:
                        retn = Variable(VT_FLOAT,_int / rhs._float);
                    break;
                }
            break;

            case VT_FLOAT:
                switch (rhs.type)
                {
                    case VT_INT32:
                        retn = Variable(VT_FLOAT,_float / rhs._int);
                    break;

                    case VT_FLOAT:
                        retn = Variable(VT_FLOAT,_float / rhs._float);
                    break;
                }
            break;
        }

        switch (retn.type)
        {
            case VT_FLOAT:
                retn.floatCheck();
            case VT_INT32:
                return retn;
            break;

            default:
                SONETTO_THROW("Variable has invalid type");
            break;
        }
    }
    //--------------------------------------------------------------------------
    Variable Variable::pow(const Variable &exp)
    {
        Variable retn;
        retn.type = 3;

        switch (type)
        {
            case VT_INT32:
                switch (exp.type)
                {
                    case VT_INT32:
                        if (exp._int < 0) {
                            //retn = Variable(VT_FLOAT,std::pow((float)_int,(float)exp._int));
                        } else {
                            retn = Variable(VT_INT32,simplePow(_int,exp._int));
                        }
                    break;

                    case VT_FLOAT:
                        //retn = Variable(VT_FLOAT,std::pow((float)_int,exp._float));
                    break;
                }
            break;

            case VT_FLOAT:
                switch (exp.type)
                {
                    case VT_INT32:
                        //retn = Variable(VT_FLOAT,std::pow(_float,(float)exp._int));
                    break;

                    case VT_FLOAT:
                        //retn = Variable(VT_FLOAT,std::pow(_float,exp._float));
                    break;
                }
            break;
        }

        switch (retn.type)
        {
            case VT_FLOAT:
                retn.floatCheck();
            case VT_INT32:
                return retn;
            break;

            default:
                SONETTO_THROW("Variable has invalid type");
            break;
        }
    }
    //--------------------------------------------------------------------------
    Variable Variable::sqrt()
    {
        Variable retn;
        retn.type = 3;

        switch (type)
        {
            case VT_INT32:
                retn = Variable(VT_FLOAT,std::sqrt((float)_int));
            break;

            case VT_FLOAT:
                retn = Variable(VT_FLOAT,std::sqrt(_float));
            break;
        }

        switch (retn.type)
        {
            case VT_FLOAT:
                retn.floatCheck();
            case VT_INT32:
                return retn;
            break;

            default:
                SONETTO_THROW("Variable has invalid type");
            break;
        }
    }
    //--------------------------------------------------------------------------
    Variable Variable::sin(const Variable &rhs)
    {
        switch (rhs.type)
        {
            case VT_INT32:
                //return Variable(VT_FLOAT,gccsinfhack((float)rhs._int));
            break;

            case VT_FLOAT:
                //return Variable(VT_FLOAT,gccsinfhack(rhs._float));
            break;

            default:
                SONETTO_THROW("Variable has invalid type");
            break;
        }
    }
    //--------------------------------------------------------------------------
    Variable Variable::cos(const Variable &rhs)
    {
        switch (rhs.type)
        {
            case VT_INT32:
                return Variable(VT_FLOAT,std::cos((float)rhs._int));
            break;

            case VT_FLOAT:
                return Variable(VT_FLOAT,std::cos(rhs._float));
            break;

            default:
                SONETTO_THROW("Variable has invalid type");
            break;
        }
    }
    //--------------------------------------------------------------------------
    Variable Variable::tan(const Variable &rhs)
    {
        switch (rhs.type)
        {
            case VT_INT32:
                return Variable(VT_FLOAT,std::tan((float)rhs._int));
            break;

            case VT_FLOAT:
                return Variable(VT_FLOAT,std::tan(rhs._float));
            break;

            default:
                SONETTO_THROW("Variable has invalid type");
            break;
        }
    }
} // namespace
