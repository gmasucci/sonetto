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

#ifndef SONETTO_SCRIPTINPUTHANDLER_H
#define SONETTO_SCRIPTINPUTHANDLER_H

#include "SonettoPrerequisites.h"
#include "SonettoOpcodeHandler.h"
#include "SonettoOpcode.h"
#include "SonettoScript.h"

namespace Sonetto
{
    class ScriptInputHandler : public OpcodeHandler
    {
    public:
        enum Opcodes
        {
            OP_INPUT_BASE = 3000,
            OP_GET_PLAYER_NUM = OP_INPUT_BASE,
            OP_GET_DIRECT_KEY_STATE,
            OP_GET_PLAYER_JOYSTICK,
            OP_IS_JOYSTICK_PLUGGED,
            OP_GET_PLAYER_BTN_STATE,
            OP_GET_PLAYER_ANALOG_VALUE
        };

        ScriptInputHandler() {}
        virtual ~ScriptInputHandler() {}

        void registerOpcodes();
        void unregisterOpcodes();
        int handleOpcode(Script *script,size_t id,Opcode *opcode);
    };
} // namespace

#endif
