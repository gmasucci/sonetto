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
    Exception::Exception(const string &desc,const string &src,size_t line) throw() {
        mDesc = desc;
        mSrc  = src;
        mLine = line;
    }

    const char *Exception::what() {
        size_t        skip = string(__FILE__).find("SonettoException.cpp");
        ostringstream str;

        if(skip == string::npos) {
            // <todo> Use Ogre logger instead
            printf("Warning! Have you renamed SonettoException.cpp?\n");

            skip = 0;
        } else {
            skip -= 4;
        }

        str << "Sonetto Exception\n  In: " << (const char *)(mSrc.c_str()+skip) << " at line " <<
               mLine << "\n\n  " << mDesc << ".";

        return str.str().c_str();
    }
} // namespace
