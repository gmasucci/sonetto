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

#ifndef SONETTO_PREREQUISITES_H
#define SONETTO_PREREQUISITES_H

#include <vector>
#include <map>

// SONETTO_API definition
#ifdef WINDOWS // Under Windows we must import/export object definitions
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
#define SONETTO_SINGLETON_IMPLEMENT(attrib,singleton) \
    template<> attrib singleton *Ogre::Singleton<singleton>::ms_Singleton = NULL; \
    singleton &singleton::getSingleton() \
    { \
        assert(ms_Singleton); \
        return *ms_Singleton; \
    } \
    singleton *singleton::getSingletonPtr() \
    { \
        assert(ms_Singleton); \
        return ms_Singleton; \
    }

namespace Sonetto
{
    // Declare common data types
    typedef unsigned long uint32;
    typedef unsigned short uint16;
    typedef unsigned char uint8;
    typedef signed long int32;
    typedef signed short int16;
    typedef signed char int8;

    typedef std::vector<uint32> IDVector;
    typedef std::map<uint32,uint32> IDIDMap;

    // Forward declarations
    class Kernel;
    class Exception;
    class Module;
    class ModuleFactory;
    class BootModule;
    class TitleModule;
    class MapModule;
    class MenuModule;
    class WorldModule;
    class BattleModule;
    class InputManager;
    class PlayerInput;
    class InputSource;
    class Joystick;
    class FontManager;
    class Font;
    class FontSerializer;
    class StaticTextElement;
    class StaticTextElementFactory;
    class ScriptManager;
    class ScriptFile;
    class Script;
    class OpcodeHandler;
    class OpcodeArgument;
    class Opcode;
    class OpFlowStop;
    class OpFlowJmp;
    class OpFlowCJmp;
    class ScriptFlowHandler;
    class Database;
    class Savemap;
    class Variable;
    class ScriptDataHandler;
    class AudioManager;
    class Music;
    class MusicStream;
    class SoundDef;
    class SoundSource;
    class InputManager;

    // <todo> Find a good place for this (I don't think this is a good place to
    // put things we don't know where to put; it will probably lead to problems of
    // having to recompile the whole library each time you edit those things)
    /*enum ScreenMetricsMode {
        SMM_RELATIVE,
        SMM_RELATIVE_ASPECT_ADJUSTED
    };*/
} // namespace

#define MKFOURCC(c0,c1,c2,c3) ((uint32)(uint8)(c0)|((uint32)(uint8)(c1)<<8)|((uint32)(uint8)(c2)<<16)|((uint32)(uint8)(c3)<<24))

// Include only essential headers here
#include "SonettoException.h"

#endif
