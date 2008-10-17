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
        scriptMan->_registerOpcode(OP_STOP,Opcode(this,new OpFlowStop));
        scriptMan->_registerOpcode(OP_GOTO,Opcode(this,new OpFlowGoto));
        scriptMan->_registerOpcode(OP_CGOTO,Opcode(this,new OpFlowCGoto));
    }
    //--------------------------------------------------------------------------
    int ScriptFlowHandler::handleOpcode(Script *script,size_t id,
            OpcodeArguments *args)
    {
        switch (id)
        {
            case OP_STOP:
                return SCRIPT_STOP;
            break;

            case OP_GOTO:
                return goto_(script,dynamic_cast<OpFlowGoto *>(args));
            break;

            case OP_CGOTO:
                return cgoto(script,dynamic_cast<OpFlowCGoto *>(args));
            break;

            default:
                SONETTO_THROW("Script flow error: Unrecognized opcode");
            break;
        }
    }
    //--------------------------------------------------------------------------
    int ScriptFlowHandler::goto_(Script *script,OpFlowGoto *args)
    {
        const LabelVector &labels = script->_getScriptFile()->_getLabels();

        if (args->labelID >= labels.size())
        {
            SONETTO_THROW("Script flow error: Missing a goto label");
        }

        return labels[args->labelID];
    }
    //--------------------------------------------------------------------------
    int ScriptFlowHandler::cgoto(Script *script,OpFlowCGoto *args)
    {
        /*int labelValue;
        const LabelVector &labels = script->_getScriptFile()->_getLabels();
        OpFlowCGoto::Comparator cmp = args->comparator;
        int retn = SCRIPT_CONTINUE;

        if (args->labelID >= labels.size())
        {
            SONETTO_THROW("Script flow error: Missing a goto label");
        }

        labelValue = labels[args->labelID];

        switch (args->type)
        {
            case OpFlowCGoto::TYP_SWITCH:
            {
                bool value = true; // Temporary
                bool cmpValue = args->boolValue;

                switch (cmp)
                {
                    case OpFlowCGoto::CMP_EQUAL_TO:
                        if (cmpValue == value)
                        {
                            retn = labelValue;
                        }
                    break;

                    case OpFlowCGoto::CMP_NOT_EQUAL_TO:
                        if (cmpValue != value)
                        {
                            retn = labelValue;
                        }
                    break;

                    default:
                        SONETTO_THROW("Script flow error: Invalid "
                                "comparator used in switch comparison");
                    break;
                }
            }
            break;

            case OpFlowCGoto::TYP_VARIABLE:
            {
                float value = 69.0f; // Temporary
                float cmpValue = args->floatValue;

                switch (cmp)
                {
                    case OpFlowCGoto::CMP_EQUAL_TO:
                        if (cmpValue == value)
                        {
                            retn = labelValue;
                        }
                    break;

                    case OpFlowCGoto::CMP_NOT_EQUAL_TO:
                        if (cmpValue != value)
                        {
                            retn = labelValue;
                        }
                    break;

                    default:
                        SONETTO_THROW("Script flow error: Comparison not "
                                "implemented");
                    break;
                }
            }
            break;
        }

        return retn;*/
        return SCRIPT_CONTINUE;
    }
    //--------------------------------------------------------------------------
} // namespace Sonetto
