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

#include "SonettoKernel.h"
#include "SonettoScriptManager.h"
#include "SonettoDatabase.h"
#include "SonettoVariable.h"
#include "SonettoScriptFlowHandler.h"

namespace Sonetto
{
    //--------------------------------------------------------------------------
    // Sonetto::ScriptFlowHandler implementation.
    //--------------------------------------------------------------------------
    void ScriptFlowHandler::registerOpcodes()
    {
        ScriptManager &scriptMan = ScriptManager::getSingleton();

        // Registers opcodes that should be handled by ScriptFlowHandler
        scriptMan._registerOpcode(OP_STOP,new OpFlowStop(this));
        scriptMan._registerOpcode(OP_JMP,new OpFlowJmp(this));
        scriptMan._registerOpcode(OP_CJMP,new OpFlowCJmp(this));
    }
    //--------------------------------------------------------------------------
    void ScriptFlowHandler::unregisterOpcodes()
    {
        ScriptManager &scriptMan = ScriptManager::getSingleton();

        scriptMan._unregisterOpcode(OP_STOP);
        scriptMan._unregisterOpcode(OP_JMP);
        scriptMan._unregisterOpcode(OP_CJMP);
    }
    //--------------------------------------------------------------------------
    int ScriptFlowHandler::handleOpcode(Script *script,size_t id,Opcode *opcode)
    {
        // Calls opcode handler methods
        switch (id)
        {
            case OP_STOP:
                return SCRIPT_STOP;
            break;

            case OP_JMP:
                return jmp(dynamic_cast<OpFlowJmp *>(opcode));
            break;

            case OP_CJMP:
                return cjmp(script,dynamic_cast<OpFlowCJmp *>(opcode));
            break;

            default:
                SONETTO_THROW("Script flow error: Unrecognized opcode");
            break;
        }
    }
    //--------------------------------------------------------------------------
    int ScriptFlowHandler::jmp(OpFlowJmp *opcode)
    {
        assert(opcode);
        return opcode->address;
    }
    //--------------------------------------------------------------------------
    int ScriptFlowHandler::cjmp(Script *script,OpFlowCJmp *opcode)
    {
        assert(opcode);

        Variable lvar(VT_INT32,0); // Unexistent variables default to zero
        Variable rvar();
        VariableMap *rvars;
        int retn = SCRIPT_CONTINUE;

        // The jump address is based on the beginning of the script,
        // so it cannot be lesser than zero
        if (opcode->address < 0)
        {
            SONETTO_THROW("Script flow error: Invalid jump address");
        }

        // Gets variable desired to do the checking
        switch (opcode->scope)
        {
            // The variable can be local, in which case it is taken from
            // the script's local variable map
            case VS_LOCAL:
                rvars = script->getLocals();
            break;

            // And the variable can be global, in which case it is taken from
            // the database's savemap
            case VS_GLOBAL:
                rvars = &Database::getSingleton().savemap.variables;
            break;

            // Or it might also be a terrible corruption problem :-)
            default:
                SONETTO_THROW("Script flow error: Unregonized conditional "
                            "jump scope");
            break;
        }

        if (rvars)
        {
            VariableMap::iterator iter = rvars->find(opcode->cmpIndex);

            if (iter != rvars->end())
            {
                lvar = iter->second;
            }
        }

        if (lvar.compare((VariableComparator)(opcode->comparator),
                opcode->variable))
        {
            retn = opcode->address;
        }

        return retn;
    }
} // namespace Sonetto
