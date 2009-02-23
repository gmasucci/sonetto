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

#ifndef SONETTO_SCRIPTMANAGER_H
#define SONETTO_SCRIPTMANAGER_H

// Forward declarations
namespace Sonetto
{
    class ScriptManager;

    const int SCRIPT_STOP         = -4;
    const int SCRIPT_SUSPEND      = -3;
    const int SCRIPT_SUSPEND_NEXT = -2;
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

        template<class ScriptImpl>
        inline ScriptImpl *createScript(
            const std::string &scriptName,const std::string &groupName)
        {
            ScriptImpl *script = new ScriptImpl(load(scriptName,groupName));
            mScripts.push_back(script);
            return script;
        }

        inline void destroyScript(Sonetto::Script *script)
        {
            mScripts.erase(std::find(mScripts.begin(),mScripts.end(),script));
            delete script;
        }

        bool updateScript(Script *script);

        void _registerOpcode(size_t id,const Opcode *opcode);
        void _unregisterOpcode(size_t id);

    protected:
        inline Ogre::Resource *createImpl(const Ogre::String &name,
                Ogre::ResourceHandle handle,const Ogre::String &group,
                bool isManual,Ogre::ManualResourceLoader *loader,
                const Ogre::NameValuePairList *createParams)
        {
            return new ScriptFile(this,name,handle,group,isManual,loader);
        }

        void readScriptData(Script *script,void *dest,
                size_t bytes,bool updateCursor);

        Opcode *readOpcode(Script *script,size_t &id,
                size_t &bytesRead);

        size_t seekOpcode(Script *script,size_t opIndex);

        OpcodeTable mOpcodeTable;

        ScriptFlowHandler mFlowHandler;

        ScriptVector mScripts;
    };
} // namespace Sonetto

#endif // SONETTO_SCRIPTMANAGER_H
