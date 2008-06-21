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

#include <sstream>
#include "SonettoException.h"

using namespace std;

namespace Sonetto {
    //-----------------------------------------------------------------------------
    // Sonetto::Exception implementation.
    //-----------------------------------------------------------------------------
    Exception::Exception(const char *desc,const char *src,size_t line) throw()
    {
        mDesc = new char[strlen(desc)];
        mSrc  = new char[strlen(src)];
        mLine = line;

        strcpy(mDesc,desc);
        strcpy(mSrc,src);
    }
    //-----------------------------------------------------------------------------
    const char *Exception::what()
    {
        try {
            size_t        skip;
            ostringstream str;

            for (skip = strlen(__FILE__);skip > 0 && __FILE__[skip-1] != '\\' && __FILE__[skip-1] != '/';--skip);
            str << "Sonetto Exception\n"
                   "  In: " << (const char *)(mSrc+skip) << " at line " << mLine << "\n\n"
                   "  " << mDesc << ".";

            return str.str().c_str();
        } catch(...) {
            return NULL;
        }
    }
    //-----------------------------------------------------------------------------
} // namespace
