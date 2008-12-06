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

#ifndef SONETTO_PREREQUISITES_H
#define SONETTO_PREREQUISITES_H

// SONETTO_API definition
#ifdef WIN32 // Under Windows we must import/export object definitions
#   ifdef SONETTO_DLL_BUILD
#       define SONETTO_API __declspec(dllexport)
#   else
#       define SONETTO_API __declspec(dllimport)
#   endif
#else        // Under Linux that's not needed
#   define SONETTO_API
#endif

// Sonetto Exception throw macro
#define SONETTO_THROW(msg) throw Sonetto::Exception(msg,__FILE__,__LINE__)

// Macro for implementing Ogre::Singletons (avoids typos and
// makes code cleaner)
#define SONETTO_SINGLETON_IMPLEMENT(singleton) \
    template<> SONETTO_API singleton *Ogre::Singleton<singleton>::ms_Singleton = NULL; \
    singleton &singleton::getSingleton() \
    { \
        return *ms_Singleton; \
    } \
    singleton *singleton::getSingletonPtr() \
    { \
        return ms_Singleton; \
    }

// Forward declarations
namespace Sonetto
{
    class Kernel;
    class Exception;
} // namespace

// Include only essential headers here
#include "SonettoException.h"

#endif
