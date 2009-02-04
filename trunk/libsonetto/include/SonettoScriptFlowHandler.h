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

#ifndef SONETTO_SCRIPTFLOWHANDLER_H
#define SONETTO_SCRIPTFLOWHANDLER_H

#include "SonettoPrerequisites.h"
#include "SonettoOpcodeHandler.h"
#include "SonettoOpcode.h"
#include "SonettoVariable.h"

namespace Sonetto
{
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

        uint32 address;
    };

    class OpFlowCJmp : public Opcode
    {
    public:
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
                    OpcodeArgument(sizeof(variable.type),&opcode->variable.type));
            opcode->arguments.push_back(
                    OpcodeArgument(sizeof(variable._int),&opcode->variable._int));

            opcode->arguments.push_back(
                    OpcodeArgument(sizeof(address),&opcode->address));

            return opcode;
        }

        char scope;
        uint32 cmpIndex;
        char comparator;
        Variable variable;
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

        void registerOpcodes();
        void unregisterOpcodes();
        int handleOpcode(Script *script,size_t id,Opcode *opcode);

    private:
        int jmp(OpFlowJmp *opcode);
        int cjmp(Script *script,OpFlowCJmp *opcode);
    };
} // namespace Sonetto

#endif
