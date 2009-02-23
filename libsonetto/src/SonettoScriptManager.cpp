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

#include "SonettoException.h"
#include "SonettoScriptManager.h"

namespace Sonetto
{
    //--------------------------------------------------------------------------
    // Sets ScriptManager singleton pointer to NULL
    //--------------------------------------------------------------------------
    SONETTO_SINGLETON_IMPLEMENT(SONETTO_API,ScriptManager);
    //--------------------------------------------------------------------------
    ScriptManager::ScriptManager()
    {
        mResourceType = "SonettoScript";

        // low, because it will likely reference other resources
        mLoadOrder = 30.0f;

        // this is how we register the ResourceManager with OGRE
        Ogre::ResourceGroupManager::getSingleton().
                _registerResourceManager(mResourceType,this);

        // Registers basic flow control opcodes
        mFlowHandler.registerOpcodes();
    }
    //--------------------------------------------------------------------------
    ScriptManager::~ScriptManager()
    {
        // and this is how we unregister it
        Ogre::ResourceGroupManager::getSingleton().
                _unregisterResourceManager(mResourceType);
    }
    //--------------------------------------------------------------------------
    ScriptFilePtr ScriptManager::load(const Ogre::String &name,
            const Ogre::String &group)
    {
        ScriptFilePtr scriptf(getByName(name));

        if (scriptf.isNull())
        {
            scriptf = create(name,group);
        }

        scriptf->load();
        return scriptf;
    }
    //--------------------------------------------------------------------------
    bool ScriptManager::updateScript(Script *script)
    {
        size_t scriptSize = script->getScriptFile()->calculateSize();

        // Empty scripts are valid, but there is nothing to do with them
        if (scriptSize == 0)
        {
            return true;
        }

        // Starts executing the script
        while (true)
        {
            size_t opID,oplength;
            int opmove;
            Opcode *opcode;
            //ArgumentVector *args;

            // Reads opcode from script file
            opcode = readOpcode(script,opID,oplength);

            //std::cout << " --> readOpcode (id: " << opID << ", length: "
            //        << oplength << ")\n";

            /*args = &opcode->arguments;
            std::cout << "Arguments dump (" << args->size() << "):\n-----\n";
            for (size_t i = 0;i < args->size();++i)
            {
                OpcodeArgument &arg = (*args)[i];

                std::cout << i + 1 << ": ";
                switch (arg.size)
                {
                    case 1:
                        std::cout << "(char) " << (int)(*(char *)(arg.arg));
                    break;

                    case 2:
                        std::cout << "(short) " << *(short *)(arg.arg);
                    break;

                    case 4:
                        std::cout << "(int) " << *(int *)(arg.arg) <<
                                " or (float) " << *(float *)(arg.arg);
                    break;

                    default:
                        std::cout << "(string) " << (char *)(arg.arg);
                    break;
                }

                std::cout << std::endl;
            }
            std::cout << "-----\n";*/

            // Send opcode to its handler and deletes it
            opmove = opcode->handler->handleOpcode(script,opID,opcode);
            delete opcode;

            //std::cout << " --> handleOpcode (id: " << opID << ", move: "
            //        << opmove << ")\n";

            if (opmove == SCRIPT_STOP) {
                seekOpcode(script,0);
                break;
            } else
            if (opmove == SCRIPT_SUSPEND) {
                // Rewinds to the beginning of this opcode
                script->_setOffset(script->_getOffset() - oplength);
                break;
            } else
            if (opmove == SCRIPT_SUSPEND_NEXT) {
                if (script->_getOffset() == scriptSize)
                {
                    seekOpcode(script,0);
                }

                break;
            } else
            if (opmove >= 0) {
                // Sets opcode offset based on the handlers' answer
                seekOpcode(script,opmove);
            }

            // If the script has ended, rewinds it to the beginning
            // and stops executing it
            if (script->_getOffset() == scriptSize)
            {
                seekOpcode(script,0);
                return true; // true, the script has ended
            }
        }

        return false;
    }
    //--------------------------------------------------------------------------
    void ScriptManager::_registerOpcode(size_t id,const Opcode *opcode)
    {
        if (mOpcodeTable.find(id) != mOpcodeTable.end())
        {
            SONETTO_THROW("Requested opcode is already registered");
        }

        mOpcodeTable.insert(std::pair<size_t,const Opcode *>(id,opcode));
    }
    //--------------------------------------------------------------------------
    void ScriptManager::_unregisterOpcode(size_t id)
    {
        OpcodeTable::iterator iter = mOpcodeTable.find(id);

        if (iter == mOpcodeTable.end())
        {
            SONETTO_THROW("Requested opcode is not registered");
        }

        // Deletes and removes opcode from opcode table
        delete iter->second;
        mOpcodeTable.erase(iter);
    }
    //--------------------------------------------------------------------------
    void ScriptManager::readScriptData(Script *script,void *dest,
            size_t bytes,bool updateCursor)
    {
        size_t offset = script->_getOffset();
        const ScriptData &data = script->getScriptFile()->_getScriptData();

        if (offset + bytes > data.size())
        {
            SONETTO_THROW("Requested read length overflows script data");
        }

        if (updateCursor)
        {
            script->_setOffset(offset + bytes);
        }

        memcpy(dest,&data[offset],bytes);
    }
    //--------------------------------------------------------------------------
    Opcode *ScriptManager::readOpcode(Script *script,size_t &id,
            size_t &bytesRead)
    {
        OpcodeTable::iterator iter;
        Opcode *opcode;

        // Reads ID and moves forward
        readScriptData(script,&id,sizeof(id),true);
        bytesRead = sizeof(id);

        // Gets iterator to the corresponding mOpcodeTable entry for
        // the opcode ID read (`id')
        iter = mOpcodeTable.find(id);

        // Checks whether the opcode is registered in mOpcodeTable
        if (iter == mOpcodeTable.end())
        {
            SONETTO_THROW("Script manager error: Invalid opcode");
        }

        // Creates the desired opcode
        opcode = iter->second->create();

        // Reads opcode arguments into its pointers
        for (size_t i = 0;i < opcode->arguments.size();++i)
        {
            readScriptData(script,opcode->arguments[i].arg,
                    opcode->arguments[i].size,true);

            bytesRead += opcode->arguments[i].size;
        }

        // Returns opcode read
        return opcode;
    }
    //--------------------------------------------------------------------------
    size_t ScriptManager::seekOpcode(Script *script,size_t opIndex)
    {
        script->_setOffset(0);
        for (size_t opnum = 0;opnum < opIndex;++opnum)
        {
            OpcodeTable::iterator iter;
            size_t opID,oplength;

            readScriptData(script,&opID,sizeof(opID),true);

            iter = mOpcodeTable.find(opID);
            if (iter == mOpcodeTable.end())
            {
                SONETTO_THROW("Script manager error: Invalid opcode");
            }

            oplength = iter->second->getArgsSize();
            if (script->_getOffset() + oplength >
                    script->getScriptFile()->_getScriptData().size())
            {
                SONETTO_THROW("Script manager error: Opcode length overflows "
                        "script data");
            }

            script->_setOffset(script->_getOffset() + oplength);
        }

        return script->_getOffset();
    }
} // namespace
