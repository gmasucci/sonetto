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

#include <cmath>
#include "SonettoVariable.h"

namespace Sonetto
{
    //--------------------------------------------------------------------------
    // Temporary simplePow helper function
    //--------------------------------------------------------------------------
    inline int simplePow(int base,int exp)
    {
        assert(exp >= 0);

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
                            retn = Variable(VT_FLOAT,std::pow((float)_int,(int)exp._int));
                        } else {
                            retn = Variable(VT_INT32,simplePow(_int,exp._int));
                        }
                    break;

                    case VT_FLOAT:
                        retn = Variable(VT_FLOAT,std::pow((float)_int,exp._float));
                    break;
                }
            break;

            case VT_FLOAT:
                switch (exp.type)
                {
                    case VT_INT32:
                        retn = Variable(VT_FLOAT,std::pow(_float,(float)exp._int));
                    break;

                    case VT_FLOAT:
                        retn = Variable(VT_FLOAT,std::pow(_float,exp._float));
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
                return Variable(VT_FLOAT,std::sin((float)rhs._int));
            break;

            case VT_FLOAT:
                return Variable(VT_FLOAT,std::sin(rhs._float));
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
