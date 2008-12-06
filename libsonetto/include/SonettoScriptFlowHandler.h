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

#include "SonettoSaveMap.h"
#include "SonettoOpcode.h"
#include "SonettoScriptManager.h"

namespace Sonetto {
    class OpFlowStop : public Opcode
    {
    public:
        OpFlowStop(OpcodeHandler *aHandler)
                : Opcode(aHandler) {}

        OpFlowStop *create() const { return new OpFlowStop(handler); }
    };

    class OpFlowJmp : public Opcode
    {
    public:
        OpFlowJmp(OpcodeHandler *aHandler)
                : Opcode(aHandler) {}

        OpFlowJmp *create() const {
            OpFlowJmp *opcode = new OpFlowJmp(handler);
            opcode->arguments.push_back(
                    OpcodeArgument(sizeof(address),&opcode->address));

            return opcode;
        }

        size_t address;
    };

    class OpFlowCJmp : public Opcode
    {
    public:
        enum Scope
        {
            CJS_LOCAL,
            CJS_GLOBAL
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

        OpFlowCJmp(OpcodeHandler *aHandler)
                : Opcode(aHandler) {}

        OpFlowCJmp *create() const
        {
            OpFlowCJmp *opcode = new OpFlowCJmp(handler);

            opcode->arguments.push_back(
                    OpcodeArgument(sizeof(scope),&opcode->scope));
            opcode->arguments.push_back(
                    OpcodeArgument(sizeof(cmpIndex),&opcode->cmpIndex));
            opcode->arguments.push_back(
                    OpcodeArgument(sizeof(comparator),&opcode->comparator));
            opcode->arguments.push_back(
                    OpcodeArgument(sizeof(varType),&opcode->varType));

            // We only put reference to one of the largest members of an union
            // It will cover all the union memory region
            opcode->arguments.push_back(
                    OpcodeArgument(sizeof(cmpInt),&opcode->cmpInt));

            opcode->arguments.push_back(
                    OpcodeArgument(sizeof(address),&opcode->address));

            return opcode;
        }

        char   scope;
        size_t cmpIndex;
        char   comparator;

        char varType;
        union
        {
            int   cmpInt;
            float cmpFloat;
        };

        int address;
    };

    class ScriptFlowHandler : public OpcodeHandler
    {
    public:
        enum Opcodes
        {
            OP_FLOW_BASE = 1000,
            OP_STOP = OP_FLOW_BASE,
            OP_JMP,
            OP_CJMP
        };

        ScriptFlowHandler() {}
        virtual ~ScriptFlowHandler() {}

        void registerOpcodes(ScriptManager *scriptMan);
        int handleOpcode(Script *script,size_t id,Opcode *opcode);

    private:
        int jmp(OpFlowJmp *opcode);
        int cjmp(Script *script,OpFlowCJmp *opcode);
    };
} // namespace Sonetto

#endif
