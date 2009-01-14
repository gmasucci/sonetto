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

#include "SonettoKernel.h"
#include "SonettoScriptManager.h"
#include "SonettoDatabase.h"
#include "SonettoVariable.h"
#include "SonettoScriptDataHandler.h"

namespace Sonetto
{
    //--------------------------------------------------------------------------
    // Sonetto::ScriptDataHandler implementation.
    //--------------------------------------------------------------------------
    void ScriptDataHandler::registerOpcodes()
    {
        ScriptManager &scriptMan = ScriptManager::getSingleton();

        scriptMan._registerOpcode(OP_PUSH,new OpDataPush(this));
        scriptMan._registerOpcode(OP_PUSHV,new OpDataPushVar(this));
        scriptMan._registerOpcode(OP_POP,new OpDataPop(this));
        scriptMan._registerOpcode(OP_POPV,new OpDataPopVar(this));
        scriptMan._registerOpcode(OP_VCHG,new OpDataVarChg(this));

        OpcodeHandler::registerOpcodes();
    }
    //--------------------------------------------------------------------------
    void ScriptDataHandler::unregisterOpcodes()
    {
        ScriptManager &scriptMan = ScriptManager::getSingleton();

        scriptMan._unregisterOpcode(OP_PUSH);
        scriptMan._unregisterOpcode(OP_PUSHV);
        scriptMan._unregisterOpcode(OP_POP);
        scriptMan._unregisterOpcode(OP_POPV);
        scriptMan._unregisterOpcode(OP_VCHG);

        OpcodeHandler::unregisterOpcodes();
    }
    //--------------------------------------------------------------------------
    int ScriptDataHandler::handleOpcode(Script *script,size_t id,Opcode *opcode)
    {
        // Calls opcode handler methods
        switch (id)
        {
            case OP_PUSH:
                return push(script,dynamic_cast<OpDataPush *>(opcode));
            break;

            case OP_PUSHV:
                return pushv(script,dynamic_cast<OpDataPushVar *>(opcode));
            break;

            case OP_POP:
                return pop(script,dynamic_cast<OpDataPop *>(opcode));
            break;

            case OP_POPV:
                return popv(script,dynamic_cast<OpDataPopVar *>(opcode));
            break;

            case OP_VCHG:
                return vchg(script,dynamic_cast<OpDataVarChg *>(opcode));
            break;

            default:
                SONETTO_THROW("Script data handler error: Unrecognized opcode");
            break;
        }
    }
    //--------------------------------------------------------------------------
    int ScriptDataHandler::push(Script *script,OpDataPush *opcode)
    {
        script->stackPush(opcode->variable);
        return SCRIPT_CONTINUE;
    }
    //--------------------------------------------------------------------------
    int ScriptDataHandler::pushv(Script *script,OpDataPushVar *opcode)
    {
        VariableMap::iterator iter;
        VariableMap *tVars;

        switch (opcode->scope)
        {
            case VS_LOCAL:
                tVars = script->_getLocals();

                if (!tVars)
                {
                    SONETTO_THROW("Script data handler error: Script has no "
                            "local variables");
                }
            break;

            case VS_GLOBAL:
                tVars = &Database::getSingleton().savemap.variables;
            break;

            default:
                SONETTO_THROW("Script data handler error: Invalid scope "
                        "identifier in script");
            break;
        }

        Variable &tVar = (*tVars)[opcode->varIndex];

        script->stackPush(tVar);
        return SCRIPT_CONTINUE;
    }
    //--------------------------------------------------------------------------
    int ScriptDataHandler::pop(Script *script,OpDataPop *opcode)
    {
        script->stackPop();
        return SCRIPT_CONTINUE;
    }
    //--------------------------------------------------------------------------
    int ScriptDataHandler::popv(Script *script,OpDataPopVar *opcode)
    {
        VariableMap::iterator iter;
        VariableMap *tVars;

        switch (opcode->scope)
        {
            case VS_LOCAL:
                tVars = script->_getLocals();

                if (!tVars)
                {
                    SONETTO_THROW("Script data handler error: Script has no "
                            "local variables");
                }
            break;

            case VS_GLOBAL:
                tVars = &Database::getSingleton().savemap.variables;
            break;

            default:
                SONETTO_THROW("Script data handler error: Invalid scope "
                        "identifier in script");
            break;
        }

        (*tVars)[opcode->varIndex] = script->stackPeek();
        script->stackPop();
        return SCRIPT_CONTINUE;
    }
    //--------------------------------------------------------------------------
    int ScriptDataHandler::vchg(Script *script,OpDataVarChg *opcode)
    {
        Variable variable;
        VariableMap::iterator iter;
        VariableMap *tVars;

        switch (opcode->scope)
        {
            case VS_LOCAL:
                tVars = script->_getLocals();

                if (!tVars)
                {
                    SONETTO_THROW("Script data handler error: Script has no "
                            "local variables");
                }
            break;

            case VS_GLOBAL:
                tVars = &Database::getSingleton().savemap.variables;
            break;

            default:
                SONETTO_THROW("Script data handler error: Invalid scope "
                        "identifier in script");
            break;
        }

        Variable &tVar = (*tVars)[opcode->varIndex];

        if (opcode->operation != VCO_SQRT)
        {
            variable = script->stackPeek();
            std::cout << ((variable.type == VT_INT32) ? variable._int : variable._float) << " ("
                    << ((variable.type == VT_INT32) ? "int32" : "float") << ")\n";
            script->stackPop();
        }

        switch (opcode->operation)
        {
            case VCO_SET:
                tVar = variable;
            break;

            case VCO_ADD:
                tVar += variable;
            break;

            case VCO_SUBTRACT:
                tVar -= variable;
            break;

            case VCO_MULTIPLY:
                tVar *= variable;
            break;

            case VCO_DIVIDE:
                tVar /= variable;
            break;

            case VCO_POWER:
                tVar = tVar.pow(variable);
            break;

            case VCO_SQRT:
                tVar = tVar.sqrt();
            break;

            case VCO_SINE:
                tVar = Variable::sin(variable);
            break;

            case VCO_COSINE:
                tVar = Variable::cos(variable);
            break;

            case VCO_TANGENT:
                tVar = Variable::tan(variable);
            break;

            default:
                SONETTO_THROW("Script data handler error: Invalid variable "
                        "operation");
            break;
        }

        std::cout << ((tVar.type == VT_INT32) ? tVar._int : tVar._float) << " ("
                << ((tVar.type == VT_INT32) ? "int32" : "float") << ")\n";
        system("pause");

        return SCRIPT_CONTINUE;
    }
} // namespace
