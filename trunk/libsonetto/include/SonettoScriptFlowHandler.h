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

#ifndef SONETTO_SCRIPTFLOWHANDLER_H
#define SONETTO_SCRIPTFLOWHANDLER_H

#include "SonettoOpcode.h"
#include "SonettoScriptManager.h"

namespace Sonetto {
    struct OpFlowStop : public OpcodeArguments
    {
        size_t getSize() const { return sizeof(OpFlowStop) - 4; }
        OpFlowStop *create() const { return new OpFlowStop; }
    };

    struct OpFlowGoto : public OpcodeArguments
    {
        size_t getSize() const { return sizeof(OpFlowGoto) - 4; }
        OpFlowGoto *create() const { return new OpFlowGoto; }

        size_t labelID;
    };

    struct OpFlowCGoto : public OpcodeArguments
    {
        enum Type
        {
            TYP_SWITCH,
            TYP_VARIABLE
        };

        enum Comparator
        {
            CMP_EQUAL_TO,
            CMP_NOT_EQUAL_TO,
            CMP_GREATER_THAN,
            CMP_GREATER_THAN_OR_EQUAL_TO,
            CMP_LESSER_THAN,
            CMP_LESSER_THAN_OR_EQUAL_TO
        };

        size_t getSize() const { return sizeof(OpFlowCGoto) - 4; }
        OpFlowCGoto *create() const { return new OpFlowCGoto; }

        Type       type;
        size_t     cmpIndex;
        Comparator comparator;

        union
        {
            bool  boolValue;
            float floatValue;
        };

        size_t labelID;
    };

    class ScriptFlowHandler : public OpcodeHandler
    {
    public:
        enum Opcodes
        {
            OP_FLOW_BASE = 1000,
            OP_STOP = OP_FLOW_BASE,
            OP_GOTO,
            OP_CGOTO
        };

        ScriptFlowHandler() {}
        virtual ~ScriptFlowHandler() {}

        void registerOpcodes(ScriptManager *scriptMan);
        int handleOpcode(Script *script,size_t id,
                OpcodeArguments *args);

    private:
        int goto_(Script *script,OpFlowGoto *args);
        int cgoto(Script *script,OpFlowCGoto *args);
    };
} // namespace Sonetto

#endif
