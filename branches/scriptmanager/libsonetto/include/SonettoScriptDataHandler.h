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

#ifndef SONETTO_SCRIPTDATAHANDLER_H
#define SONETTO_SCRIPTDATAHANDLER_H

#include "SonettoPrerequisites.h"
#include "SonettoOpcodeHandler.h"
#include "SonettoOpcode.h"
#include "SonettoVariable.h"

namespace Sonetto
{
    class OpDataPush : public Opcode
    {
    public:
        OpDataPush(OpcodeHandler *aHandler)
                : Opcode(aHandler) {}

        OpDataPush *create() const
        {
            OpDataPush *opcode = new OpDataPush(handler);
            opcode->arguments.push_back(
                    OpcodeArgument(sizeof(variable.type),&opcode->variable.type));
            opcode->arguments.push_back(
                    OpcodeArgument(sizeof(variable._int),&opcode->variable._int));

            return opcode;
        }

        Variable variable;
    };

    class OpDataPushVar : public Opcode
    {
    public:
        OpDataPushVar(OpcodeHandler *aHandler)
                : Opcode(aHandler) {}

        OpDataPushVar *create() const
        {
            OpDataPushVar *opcode = new OpDataPushVar(handler);
            opcode->arguments.push_back(
                    OpcodeArgument(sizeof(scope),&opcode->scope));
            opcode->arguments.push_back(
                    OpcodeArgument(sizeof(varIndex),&opcode->varIndex));

            return opcode;
        }

        char scope;
        uint32 varIndex;
    };

    class OpDataPop : public Opcode
    {
    public:
        OpDataPop(OpcodeHandler *aHandler)
            : Opcode(aHandler) {}

        OpDataPop *create() const
        {
            return new OpDataPop(handler);
        }
    };

    class OpDataPopVar : public Opcode
    {
    public:
        OpDataPopVar(OpcodeHandler *aHandler)
            : Opcode(aHandler) {}

        OpDataPopVar *create() const
        {
            OpDataPopVar *opcode = new OpDataPopVar(handler);
            opcode->arguments.push_back(
                    OpcodeArgument(sizeof(scope),&opcode->scope));
            opcode->arguments.push_back(
                    OpcodeArgument(sizeof(varIndex),&opcode->varIndex));

            return opcode;
        }

        char scope;
        uint32 varIndex;
    };

    enum VarChgOperation
    {
        VCO_SET,
        VCO_ADD,
        VCO_SUBTRACT,
        VCO_MULTIPLY,
        VCO_DIVIDE,
        VCO_POWER,
        VCO_SQRT,
        VCO_SINE,
        VCO_COSINE,
        VCO_TANGENT,
        VCO_LOGARITHM
    };

    class OpDataVarChg : public Opcode
    {
    public:
        OpDataVarChg(OpcodeHandler *aHandler)
                : Opcode(aHandler) {}

        OpDataVarChg *create() const
        {
            OpDataVarChg *opcode = new OpDataVarChg(handler);
            opcode->arguments.push_back(
                    OpcodeArgument(sizeof(scope),&opcode->scope));
            opcode->arguments.push_back(
                    OpcodeArgument(sizeof(varIndex),&opcode->varIndex));
            opcode->arguments.push_back(
                    OpcodeArgument(sizeof(operation),&opcode->operation));

            return opcode;
        }

        char scope;
        uint32 varIndex;
        char operation;
    };

    class ScriptDataHandler : public OpcodeHandler
    {
    public:
        enum Opcodes
        {
            OP_DATA_BASE = 2000,
            OP_PUSH = OP_DATA_BASE,
            OP_PUSHV,
            OP_POP,
            OP_POPV,
            OP_VCHG
        };

        ScriptDataHandler() {}
        virtual ~ScriptDataHandler() {}

        void registerOpcodes();
        void unregisterOpcodes();
        int handleOpcode(Script *script,size_t id,Opcode *opcode);

    private:
        int push(Script *script,OpDataPush *opcode);
        int pushv(Script *script,OpDataPushVar *opcode);
        int pop(Script *script,OpDataPop *opcode);
        int popv(Script *script,OpDataPopVar *opcode);
        int vchg(Script *script,OpDataVarChg *opcode);
    };
} // namespace

#endif
