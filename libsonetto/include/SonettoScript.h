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

#ifndef SONETTO_SCRIPT_H
#define SONETTO_SCRIPT_H

// Typedefs and forward declarations
namespace Sonetto {
    class Script;
}

#include <vector>
#include "SonettoPrerequisites.h"
#include "SonettoScriptFile.h"
#include "SonettoVariable.h"

namespace Sonetto
{
    class SONETTO_API Script
    {
    public:
        Script(ScriptFilePtr file)
                : mScriptFile(file), mOffset(0) {}

        virtual ~Script() {}

        virtual inline VariableMap *getLocals() { return NULL; }

        virtual inline ScriptFilePtr getScriptFile() { return mScriptFile; }

        virtual inline void _setOffset(size_t offset) { mOffset = offset; }

        virtual inline size_t _getOffset() const { return mOffset; }

        virtual inline void stackPush(const Variable &var) { mVarStack.push(var); }

        virtual const Variable &stackPeek();

        virtual Variable stackPop();

    protected:
        /** ScriptFile pointer

            Holds opcodes to be used by this script.
        */
        ScriptFilePtr mScriptFile;

        size_t mOffset;

        VariableStack mVarStack;
    };

    typedef std::vector<Script *> ScriptVector;
} // namespace Sonetto

#endif
