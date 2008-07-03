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

#include "SonettoMain.h"

#define SONETTO_THROW(desc) \
    { \
        std::string ___sonetto_throwstr = desc; \
        throw Sonetto::Exception(___sonetto_throwstr.c_str(),__FILE__,__LINE__); \
    }

#include <string>
#include <exception>

namespace Sonetto
{
    /** @brief Sonetto Exception class

        Instances of this class are thrown when Sonetto exceptions occurs. They
        are typically thrown using the macro SONETTO_THROW, that accepts any
        std::string-assignable value. It also uses __FILE__ and __LINE__ macros
        to display the place from which the exception was thrown. SONETTO_THROW
        throws a Sonetto::Exception, and thus the catch block should catch it by
        reference, as an assignment between two Sonetto::Exceptions would lead to
        a crash (the pointers of the strings would be copied, and the old exception
        would be destructed, deallocating such pointers).
    */
    class SONETTO_EXPORT Exception : public std::exception
    {
    public:
        /** @brief Constructor

            Copies `desc' and `src' to buffers held by Sonetto::Exception.
        @param
            desc Exception description
        @param
            src Source file from which the exception was thrown
        @param
            line The line in file `src' from which the exception was thrown
        */
        Exception(const char *desc,const char *src,size_t line) throw();

        /// @brief Destructor
        ~Exception() throw();

        /** @brief Get parsed exception in a NULL-ended C-like string.

        @remarks
            This pointer is only valid as long as the Exception is not
            destructed. It can be NULL if another exception gets thrown
            while it was parsing the string.
        */
        const char *what();

    private:
        /// @brief Exception description
        char *mDesc;

        /// @brief Complete path and source file from where the exception was thrown
        char *mSrc;

        /// @brief Line in source from which the exception was thrown
        size_t mLine;

        /// @brief Parsed message to be shown to the end-user
        std::string mParsed;
    };
} // namespace

#endif
