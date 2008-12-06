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

#include <OgreStringConverter.h>
#include "SonettoException.h"

namespace Sonetto
{
    // ----------------------------------------------------------------------
    // Sonetto::Exception implementation
    // ----------------------------------------------------------------------
    Exception::Exception(std::string desc,const char *aFile,size_t line)
    {
        // Formats exception message
        try {
            std::string file = aFile;

            // Gets file name from file path
            file = file.substr(file.find_last_of("\\/") + 1);

            // Formats message
            mWhat = "An Exception has been\nthrown in " + file +
                    ", at line " + Ogre::StringConverter::toString(line) +
                    ".\n\n" + desc + ".";
        } catch (...) {
            mWhat = "Error formatting exception (out of memory?):\n" + desc;
        }
    }
}
