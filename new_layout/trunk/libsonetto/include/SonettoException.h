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

#ifndef SONETTO_EXCEPTION_H
#define SONETTO_EXCEPTION_H

#include <exception>

namespace Sonetto
{
    /** Sonetto Exception

        Carries information about an error that ocurred during Sonetto execution.
    */
    class Exception : public std::exception
    {
    public:
        /** Default constructor

        @remarks
            Do not instantiate exceptions by yourself. Use the SONETTO_THROW
            macro instead.
        @param desc
            Error description.
        @param aFile
            File from which the exception was thrown.
        @param line
            Line from which the exception was thrown.
        */
        Exception(std::string desc,const char *aFile,size_t line);

        /// Destructor
        inline ~Exception() throw() {}

        /// Gets formated error message
        inline const char *what() const throw() { return mWhat.c_str(); }

    private:
        /// Formated error message
        std::string mWhat;
    };
} // namespace

#endif
