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

#include "SonettoException.h"
#include "SonettoScriptManager.h"

namespace Sonetto
{

    //--------------------------------------------------------------------------
    // Sets ScriptManager singleton pointer to NULL
    //--------------------------------------------------------------------------
    template<> ScriptManager *Ogre::Singleton<ScriptManager>::ms_Singleton = 0;
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
        mFlowHandler.registerOpcodes(this);
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
        ScriptFilePtr scriptf = getByName(name);

        if (scriptf.isNull())
        {
            scriptf = create(name,group);
        }

        scriptf->load();
        return scriptf;
    }
    //--------------------------------------------------------------------------
    Ogre::Resource *ScriptManager::createImpl(const Ogre::String &name,
            Ogre::ResourceHandle handle,const Ogre::String &group,bool isManual,
            Ogre::ManualResourceLoader *loader,
            const Ogre::NameValuePairList *createParams)
    {
        return new ScriptFile(this,name,handle,group,isManual,loader);
    }
    //--------------------------------------------------------------------------
    Script *ScriptManager::createScript(const std::string &scriptName,
            const std::string &groupName)
    {
        ScriptFilePtr scriptFile = load(scriptName,groupName);
        Script *script = new Script(scriptFile);
        return script;
    }
    //--------------------------------------------------------------------------
    void ScriptManager::updateScript(Script *script)
    {
        size_t scriptSize = script->_getScriptFile()->calculateSize();

        // Empty scripts are valid, but there is nothing to do with them
        if (scriptSize == 0)
        {
            return;
        }

        // Starts executing the script
        while (true)
        {
            size_t opID,oplength;
            int opmove;
            OpcodeHandler   *handler;
            OpcodeArguments *args;

            // Reads opcode from script file
            oplength = readOpcode(script,opID,&handler,&args);

            std::cout << "readOpcode (id: " << opID << ", length: "
                    << oplength << ")\n";

            std::cout << "Arguments dump:\n-----\n";
            for (size_t i = 0;i < args->getSize();++i)
            {
                std::cout << (size_t)(*( (char *)(args) + 4 + i )) << "  ";
            }
            std::cout << "\n-----\n";

            // Send opcode to its handler and deletes it
            opmove = handler->handleOpcode(script,opID,args);
            delete args;

            std::cout << "handleOpcode (id: " << opID << ", move: "
                    << opmove << ")\n";

            // If the opcode handler returned SCRIPT_STOP, we must stop
            // executing this script and set its offset back to the beginning
            // If it returned SCRIPT_SUSPEND, we must stop executing it for now,
            // setting its opcode position to the one that caused the suspension
            // If it returned SCRIPT_CONTINUE (in which case it won't be caught
            // by neither of the if conditions below), we must execute the next
            // opcode
            // And if it returned a value greater than or equal to 0, we
            // must set the script position to that given opcode index
            if (opmove == SCRIPT_STOP) {
                seekOpcode(script,0);
                break;
            } else
            if (opmove == SCRIPT_SUSPEND) {
                // Rewinds to the beginning of this opcode
                script->_setOffset(script->_getOffset() - oplength);

                break;
            } else
            if (opmove >= 0) {
                // Sets opcode offset based on the handlers' answer
                if (seekOpcode(script,opmove) == scriptSize)
                {
                    // If the script has ended, rewinds it to the beginning
                    // and stops executing this script
                    seekOpcode(script,0);
                    break;
                }
            }
        }
    }
    //--------------------------------------------------------------------------
    void ScriptManager::_registerOpcode(size_t id,const Opcode &args)
    {
        if (mOpcodeTable.find(id) != mOpcodeTable.end())
        {
            SONETTO_THROW("Requested opcode is already registered");
        }

        mOpcodeTable.insert(std::pair<size_t,Opcode>(id,args));
    }
    //--------------------------------------------------------------------------
    void ScriptManager::_unregisterOpcode(size_t id)
    {
        OpcodeTable::iterator iter = mOpcodeTable.find(id);

        if (iter == mOpcodeTable.end())
        {
            SONETTO_THROW("Requested opcode is not registered");
        }

        mOpcodeTable.erase(iter);
    }
    //--------------------------------------------------------------------------
    void ScriptManager::readScriptData(Script *script,void *dest,size_t bytes)
    {
        size_t offset = script->_getOffset();
        const ScriptData &data = script->_getScriptFile()->_getScriptData();

        if (offset + bytes > data.size())
        {
            SONETTO_THROW("Requested read length overflows script data");
        }

        memcpy(dest,&data[offset],bytes);
    }
    //--------------------------------------------------------------------------
    size_t ScriptManager::readOpcode(Script *script,size_t &id,
                OpcodeHandler **handler,OpcodeArguments **args)
    {
        assert(handler && args);

        OpcodeTable::iterator  iter;
        const Opcode          *opcode;
        char                  *superPtr;

        // Reads ID and moves forward
        readScriptData(script,&id,sizeof(id));
        script->_setOffset(script->_getOffset() + sizeof(id));

        // Gets iterator to the corresponding mOpcodeTable entry for
        // the opcode ID read (`id')
        iter = mOpcodeTable.find(id);

        // Checks whether the opcode is registered in mOpcodeTable
        if (iter == mOpcodeTable.end())
        {
            SONETTO_THROW("Script error: Invalid opcode");
        }

        // Gets pointer to opcode structure
        opcode = &iter->second;

        // Gets opcode handler pointer
        *handler = opcode->handler;

        // Creates an instance of the desired OpcodeArguments structure
        // and fills it with data read from the script (the '+4' on the
        // second line skips the VPTR, a pointer to the vtable present
        // in all classes using virtual inheritance)
        *args = opcode->arguments->create();
        superPtr = ((char *)(*args)) + 4;
        readScriptData(script,superPtr,(*args)->getSize());

        // Moves offset forward by amount of bytes read
        script->_setOffset(script->_getOffset() + (*args)->getSize());

        return ( sizeof(id) + (*args)->getSize() );
    }
    //--------------------------------------------------------------------------
    size_t ScriptManager::tellOpcode(Script *script)
    {
        size_t offset = script->_getOffset();
        size_t opnum;

        script->_setOffset(0);
        for (opnum = 0;script->_getOffset() < offset;++opnum)
        {
            size_t           opID;
            OpcodeHandler   *handler;
            OpcodeArguments *args;

            readOpcode(script,opID,&handler,&args);
        }

        return opnum;
    }
    //--------------------------------------------------------------------------
    size_t ScriptManager::seekOpcode(Script *script,size_t opIndex)
    {
        script->_setOffset(0);
        for (size_t opnum = 0;opnum < opIndex;++opnum)
        {
            size_t           opID;
            OpcodeHandler   *handler;
            OpcodeArguments *args;

            readOpcode(script,opID,&handler,&args);
        }

        return script->_getOffset();
    }
    //--------------------------------------------------------------------------
} // namespace
