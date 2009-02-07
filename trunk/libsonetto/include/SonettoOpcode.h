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

#ifndef SONETTO_OPCODE_H
#define SONETTO_OPCODE_H

#include <vector>
#include <map>
#include "SonettoPrerequisites.h"

namespace Sonetto
{
    struct SONETTO_API OpcodeArgument
    {
        OpcodeArgument(size_t aSize,void *aArg)
                : size(aSize), arg(aArg) {}

        size_t size;
        void *arg;
    };

    typedef std::vector<OpcodeArgument> ArgumentVector;

    class SONETTO_API Opcode
    {
    public:
        Opcode(OpcodeHandler *aHandler)
                : handler(aHandler),mArgsSize(0) {}
        virtual ~Opcode() {}

        virtual inline Opcode *create() const { return new Opcode(handler); }

        virtual inline size_t getArgsSize() const { return mArgsSize; }

        OpcodeHandler *handler;
        ArgumentVector arguments;

    protected:
        virtual void calculateArgsSize();

        size_t mArgsSize;
    };

    typedef std::map<size_t,const Opcode *> OpcodeTable;
} // namespace

#endif
