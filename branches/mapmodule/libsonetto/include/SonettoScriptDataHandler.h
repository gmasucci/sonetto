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
#include "SonettoScript.h"

namespace Sonetto
{
    class OpDataPush : public Opcode
    {
    public:
        OpDataPush(OpcodeHandler *aHandler);

        inline OpDataPush *create() const
                { return new OpDataPush(handler); }

        Variable variable;
    };

    class OpDataPushVar : public Opcode
    {
    public:
        OpDataPushVar(OpcodeHandler *aHandler);

        inline OpDataPushVar *create() const
                { return new OpDataPushVar(handler); }

        char scope;
        uint32 varIndex;
    };

    class OpDataPopVar : public Opcode
    {
    public:
        OpDataPopVar(OpcodeHandler *aHandler);

        inline OpDataPopVar *create() const
                { return new OpDataPopVar(handler); }

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
        OpDataVarChg(OpcodeHandler *aHandler);

        inline OpDataVarChg *create() const
                { return new OpDataVarChg(handler); }

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
        int handleOpcode(ScriptPtr script,size_t id,Opcode *opcode);
    };
} // namespace

#endif
