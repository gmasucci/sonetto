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

#ifndef SONETTO_SCRIPTMANAGER_H
#define SONETTO_SCRIPTMANAGER_H

// Forward declarations
namespace Sonetto
{
    class ScriptManager;

    const int SCRIPT_STOP         = -4;
    const int SCRIPT_SUSPEND      = -3;
    const int SCRIPT_SUSPEND_NEXT = -3;
    const int SCRIPT_CONTINUE     = -1;
}

#include <OgreResourceManager.h>
#include <OgreSingleton.h>
#include "SonettoScript.h"
#include "SonettoScriptFile.h"
#include "SonettoOpcodeHandler.h"
#include "SonettoOpcode.h"
#include "SonettoScriptFlowHandler.h"

namespace Sonetto
{
    class SONETTO_API ScriptManager : public Ogre::ResourceManager,
            public Ogre::Singleton<ScriptManager>
    {
    public:
        ScriptManager();
        virtual ~ScriptManager();

        /** Overrides standard Singleton retrieval

        @remarks
            Warning: These comments below were copied straight from Ogre3D code.
            Why do we do this? Well, it's because the Singleton
            implementation is in a .h file, which means it gets compiled
            into anybody who includes it. This is needed for the
            Singleton template to work, but we actually only want it
            compiled into the implementation of the class based on the
            Singleton, not all of them. If we don't change this, we get
            link errors when trying to use the Singleton-based class from
            an outside dll.
        @param
            This method just delegates to the template version anyway,
            but the implementation stays in this single compilation unit,
            preventing link errors.
        */
        static ScriptManager &getSingleton();

        /** Overrides standard Singleton retrieval

        @remarks
            Warning: These comments below were copied straight from Ogre3D code.
            Why do we do this? Well, it's because the Singleton
            implementation is in a .h file, which means it gets compiled
            into anybody who includes it. This is needed for the
            Singleton template to work, but we actually only want it
            compiled into the implementation of the class based on the
            Singleton, not all of them. If we don't change this, we get
            link errors when trying to use the Singleton-based class from
            an outside dll.
        @param
            This method just delegates to the template version anyway,
            but the implementation stays in this single compilation unit,
            preventing link errors.
        */
        static ScriptManager *getSingletonPtr();

        virtual ScriptFilePtr load(const Ogre::String &name,
                const Ogre::String &group);

        Script *createScript(const std::string &scriptName,
            const std::string &groupName,VariableMap *locals);

        void updateScript(Script *script);

        void _registerOpcode(size_t id,const Opcode *opcode);
        void _unregisterOpcode(size_t id);

    protected:
        Ogre::Resource *createImpl(const Ogre::String &name,
                Ogre::ResourceHandle handle,const Ogre::String &group,
                bool isManual,Ogre::ManualResourceLoader *loader,
                const Ogre::NameValuePairList *createParams);

        void readScriptData(Script *script,void *dest,size_t bytes);

        Opcode *readOpcode(Script *script,size_t &id,
                size_t &bytesRead);

        size_t seekOpcode(Script *script,size_t opIndex);

        OpcodeTable mOpcodeTable;

        ScriptFlowHandler mFlowHandler;
    };
} // namespace Sonetto

#endif // SONETTO_SCRIPTMANAGER_H
