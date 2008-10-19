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
#include "SonettoScriptFlowHandler.h"

namespace Sonetto {
    //--------------------------------------------------------------------------
    // Sonetto::ScriptFlowHandler implementation.
    //--------------------------------------------------------------------------
    void ScriptFlowHandler::registerOpcodes(ScriptManager *scriptMan)
    {
        // Registers opcodes that should be handled by ScriptFlowHandler
        scriptMan->_registerOpcode(OP_STOP,new OpFlowStop(this));
        scriptMan->_registerOpcode(OP_JMP,new OpFlowJmp(this));
        scriptMan->_registerOpcode(OP_CJMP,new OpFlowCJmp(this));
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
        // The jump address is based on the beginning of the script,
        // so it cannot be lesser than zero
        if (opcode->address < 0)
        {
            SONETTO_THROW("Script flow error: Invalid jump address");
        }

        // Simply jump to the desired address
        return opcode->address;
    }
    //--------------------------------------------------------------------------
    int ScriptFlowHandler::cjmp(Script *script,OpFlowCJmp *opcode)
    {
        Variable variable(0); // Unexistent variables default to zero
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
            case OpFlowCJmp::CJS_LOCAL:
                // <todo> Add check for local variables
            break;

            // And the variable can be global, in which case it is taken from
            // the database's savemap
            case OpFlowCJmp::CJS_GLOBAL:
            {
                VariableMap::iterator iter;
                VariableMap &variables = Kernel::get()->getDatabase()->
                        mSaveMap->getVariables();

                // If it exists, we replace the default value by it
                iter = variables.find(opcode->cmpIndex);
                if (iter != variables.end())
                {
                    variable = iter->second;
                }
            }
            break;

            // Or it might also be a terrible corruption problem :-)
            default:
                SONETTO_THROW("Script flow error: Unregonized conditional "
                            "jump scope");
            break;
        }

        // Checks condition
        switch (opcode->varType)
        {
            // Checks with integers
            case Variable::VTP_INT:
                switch (opcode->comparator)
                {
                    case OpFlowCJmp::CMP_EQUAL_TO:
                        if (opcode->cmpInt == variable.iValue)
                        {
                            retn = opcode->address;
                        }
                    break;

                    case OpFlowCJmp::CMP_NOT_EQUAL_TO:
                        if (opcode->cmpInt != variable.iValue)
                        {
                            retn = opcode->address;
                        }
                    break;

                    case OpFlowCJmp::CMP_GREATER_THAN:
                        if (opcode->cmpInt > variable.iValue)
                        {
                            retn = opcode->address;
                        }
                    break;

                    case OpFlowCJmp::CMP_GREATER_THAN_OR_EQUAL_TO:
                        if (opcode->cmpInt >= variable.iValue)
                        {
                            retn = opcode->address;
                        }
                    break;

                    case OpFlowCJmp::CMP_LESSER_THAN:
                        if (opcode->cmpInt < variable.iValue)
                        {
                            retn = opcode->address;
                        }
                    break;

                    case OpFlowCJmp::CMP_LESSER_THAN_OR_EQUAL_TO:
                        if (opcode->cmpInt <= variable.iValue)
                        {
                            retn = opcode->address;
                        }
                    break;

                    default:
                        SONETTO_THROW("Script flow error: Unregonized "
                                "comparator");
                    break;
                }
            break;

            // Checks with floats
            case Variable::VTP_FLOAT:
                switch (opcode->comparator)
                {
                    case OpFlowCJmp::CMP_EQUAL_TO:
                        if (opcode->cmpFloat == variable.fValue)
                        {
                            retn = opcode->address;
                        }
                    break;

                    case OpFlowCJmp::CMP_NOT_EQUAL_TO:
                        if (opcode->cmpFloat != variable.fValue)
                        {
                            retn = opcode->address;
                        }
                    break;

                    case OpFlowCJmp::CMP_GREATER_THAN:
                        if (opcode->cmpFloat > variable.fValue)
                        {
                            retn = opcode->address;
                        }
                    break;

                    case OpFlowCJmp::CMP_GREATER_THAN_OR_EQUAL_TO:
                        if (opcode->cmpFloat >= variable.fValue)
                        {
                            retn = opcode->address;
                        }
                    break;

                    case OpFlowCJmp::CMP_LESSER_THAN:
                        if (opcode->cmpFloat < variable.fValue)
                        {
                            retn = opcode->address;
                        }
                    break;

                    case OpFlowCJmp::CMP_LESSER_THAN_OR_EQUAL_TO:
                        if (opcode->cmpFloat <= variable.fValue)
                        {
                            retn = opcode->address;
                        }
                    break;

                    default:
                        SONETTO_THROW("Script flow error: Unregonized "
                                "comparator");
                    break;
                }
            break;

            default:
                SONETTO_THROW("Script flow error: Unregonized variable type");
            break;
        }

        // If the variable wasn't found, `retn' will have a value of
        // SCRIPT_CONTINUE, so the jump will not be taken
        return retn;
    }
    //--------------------------------------------------------------------------
} // namespace Sonetto
