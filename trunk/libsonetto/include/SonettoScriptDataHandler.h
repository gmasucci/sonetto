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
