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
        scriptMan._registerOpcode(OP_POP,new Opcode(this));
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
    int ScriptDataHandler::handleOpcode(Script *script,size_t id,
            Opcode *opcode)
    {
        // Calls opcode handler methods
        switch (id)
        {
            case OP_PUSH:
            {
                OpDataPush *pushOpcode = dynamic_cast<OpDataPush *>(opcode);
                assert(pushOpcode);

                script->stackPush(pushOpcode->variable);
                return SCRIPT_CONTINUE;
            }
            break;

            case OP_PUSHV:
            {
                OpDataPushVar *pushvOpcode = dynamic_cast<OpDataPushVar *>(opcode);
                assert(pushvOpcode);

                VariableMap::iterator iter;
                VariableMap *tVars;

                switch (pushvOpcode->scope)
                {
                    case VS_LOCAL:
                        tVars = script->getLocals();

                        if (!tVars)
                        {
                            SONETTO_THROW("Script data handler error: Script "
                                    "has no local variables");
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

                Variable &tVar = (*tVars)[pushvOpcode->varIndex];

                script->stackPush(tVar);
                return SCRIPT_CONTINUE;
            }
            break;

            case OP_POP:
                script->stackPop();
                return SCRIPT_CONTINUE;
            break;

            case OP_POPV:
            {
                OpDataPopVar *popvOpcode = dynamic_cast<OpDataPopVar *>(opcode);
                assert(popvOpcode);

                VariableMap::iterator iter;
                VariableMap *tVars;

                switch (popvOpcode->scope)
                {
                    case VS_LOCAL:
                        tVars = script->getLocals();

                        if (!tVars)
                        {
                            SONETTO_THROW("Script data handler error: Script "
                                    "has no local variables");
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

                (*tVars)[popvOpcode->varIndex] = script->stackPop();
                return SCRIPT_CONTINUE;
            }
            break;

            case OP_VCHG:
            {
                OpDataVarChg *varchgOpcode = dynamic_cast<OpDataVarChg *>(opcode);
                assert(varchgOpcode);

                Variable variable;
                VariableMap::iterator iter;
                VariableMap *tVars;

                switch (varchgOpcode->scope)
                {
                    case VS_LOCAL:
                        tVars = script->getLocals();

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

                Variable &tVar = (*tVars)[varchgOpcode->varIndex];

                if (varchgOpcode->operation != VCO_SQRT)
                {
                    variable = script->stackPop();
                }

                switch (varchgOpcode->operation)
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
                        tVar = Variable::pow(tVar,variable);
                    break;

                    case VCO_SQRT:
                        tVar = Variable::sqrt(tVar);
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

                return SCRIPT_CONTINUE;
            }
            break;

            default:
                SONETTO_THROW("Script data handler error: Unrecognized opcode");
            break;
        }
    }
} // namespace
