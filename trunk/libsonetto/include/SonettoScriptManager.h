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
namespace Sonetto {
    class ScriptManager;

    const int SCRIPT_STOP     = -3;
    const int SCRIPT_SUSPEND  = -2;
    const int SCRIPT_CONTINUE = -1;
}

#include <OgreResourceManager.h>
#include <OgreSingleton.h>
#include "SonettoScript.h"
#include "SonettoScriptFile.h"
#include "SonettoOpcode.h"
#include "SonettoScriptFlowHandler.h"

namespace Sonetto {
    class SONETTO_EXPORT ScriptManager : public Ogre::ResourceManager,
            public Ogre::Singleton<ScriptManager>
    {
    public:
        ScriptManager();
        virtual ~ScriptManager();

        virtual ScriptFilePtr load(const Ogre::String &name,
                const Ogre::String &group);

        static inline ScriptManager &getSingleton()
        {
            assert(ms_Singleton);
            return (*ms_Singleton);
        }

        static inline ScriptManager *getSingletonPtr()
        {
            return ms_Singleton;
        }

        Script *createScript(const std::string &scriptName,
            const std::string &groupName);

        void updateScript(Script *script);

        void _registerOpcode(size_t id,const Opcode &args);
        void _unregisterOpcode(size_t id);


    protected:
        Ogre::Resource *createImpl(const Ogre::String &name,
                Ogre::ResourceHandle handle,const Ogre::String &group,
                bool isManual,Ogre::ManualResourceLoader *loader,
                const Ogre::NameValuePairList *createParams);

        void readScriptData(Script *script,void *dest,size_t bytes);

        size_t readOpcode(Script *script,size_t &id,
                OpcodeHandler **handler,OpcodeArguments **args);

        size_t tellOpcode(Script *script);
        size_t seekOpcode(Script *script,size_t opIndex);

        OpcodeTable mOpcodeTable;

        ScriptFlowHandler mFlowHandler;
    };
} // namespace Sonetto

#endif // SONETTO_SCRIPTMANAGER_H
