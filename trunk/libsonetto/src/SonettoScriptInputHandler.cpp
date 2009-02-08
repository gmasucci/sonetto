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

#include "SonettoKernel.h"
#include "SonettoScriptManager.h"
#include "SonettoInputManager.h"
#include "SonettoVariable.h"
#include "SonettoScriptInputHandler.h"

namespace Sonetto
{
    //--------------------------------------------------------------------------
    // Sonetto::ScriptInputHandler implementation.
    //--------------------------------------------------------------------------
    void ScriptInputHandler::registerOpcodes()
    {
        ScriptManager &scriptMan = ScriptManager::getSingleton();

        // Registers opcodes that should be handled by ScriptInputHandler
        scriptMan._registerOpcode(OP_GET_PLAYER_NUM,new Opcode(this));
        scriptMan._registerOpcode(OP_GET_DIRECT_KEY_STATE,new Opcode(this));
        scriptMan._registerOpcode(OP_GET_PLAYER_JOYSTICK,new Opcode(this));
        scriptMan._registerOpcode(OP_IS_JOYSTICK_PLUGGED,new Opcode(this));
        scriptMan._registerOpcode(OP_GET_PLAYER_BTN_STATE,new Opcode(this));
        scriptMan._registerOpcode(OP_GET_PLAYER_ANALOG_VALUE,new Opcode(this));
    }
    //--------------------------------------------------------------------------
    void ScriptInputHandler::unregisterOpcodes()
    {
        ScriptManager &scriptMan = ScriptManager::getSingleton();

        scriptMan._unregisterOpcode(OP_GET_PLAYER_NUM);
        scriptMan._unregisterOpcode(OP_GET_DIRECT_KEY_STATE);
        scriptMan._unregisterOpcode(OP_GET_PLAYER_JOYSTICK);
        scriptMan._unregisterOpcode(OP_IS_JOYSTICK_PLUGGED);
        scriptMan._unregisterOpcode(OP_GET_PLAYER_BTN_STATE);
        scriptMan._unregisterOpcode(OP_GET_PLAYER_ANALOG_VALUE);
    }
    //--------------------------------------------------------------------------
    int ScriptInputHandler::handleOpcode(ScriptPtr script,size_t id,
            Opcode *opcode)
    {
        InputManager &inputMan = InputManager::getSingleton();

        switch (id)
        {
            case OP_GET_PLAYER_NUM:
                script->stackPush(Variable(VT_INT32,inputMan.getPlayerNum()));
                return SCRIPT_CONTINUE;
            break;

            case OP_GET_DIRECT_KEY_STATE:
            {
                uint32 keyID = script->stackPop().getValue<int32>(true);

                if (keyID < 1 || keyID > 256)
                {
                    SONETTO_THROW("Script input handler error: Unknown keyboard key ID");
                }

                script->stackPush(Variable(VT_INT32,
                        inputMan.getDirectKeyState(keyID - 1)));

                return SCRIPT_CONTINUE;
            }
            break;

            case OP_GET_PLAYER_JOYSTICK:
            {
                uint32 playerID = script->stackPop().getValue<int32>(true);

                if (playerID < 1 || playerID > inputMan.getPlayerNum())
                {
                    SONETTO_THROW("Script input handler error: Unknown "
                            "player ID");
                }

                PlayerInput *input = inputMan.getPlayer(playerID);
                script->stackPush(Variable(VT_INT32,input->getJoystick()));

                return SCRIPT_CONTINUE;
            }
            break;

            case OP_IS_JOYSTICK_PLUGGED:
            {
                uint32 jid = script->stackPop().getValue<int32>(true);

                if (jid < 1 || jid > inputMan.getJoystickNum())
                {
                    SONETTO_THROW("Script input handler error: Unknown joystick ID");
                }

                JoystickPtr joystick = inputMan._getJoystick(jid);
                script->stackPush(Variable(VT_INT32,joystick->isPlugged()));

                return SCRIPT_CONTINUE;
            }
            break;

            case OP_GET_PLAYER_BTN_STATE:
            {
                uint32 btnID = script->stackPop().getValue<int32>(true);
                uint32 playerID = script->stackPop().getValue<int32>(true);

                if (playerID < 1 || playerID > inputMan.getPlayerNum())
                {
                    SONETTO_THROW("Script input handler error: Unknown player ID");
                }

                if (btnID < 1 || btnID > BTN_LAST + 1)
                {
                    SONETTO_THROW("Script input handler error: Unknown button ID");
                }

                PlayerInput *input = inputMan.getPlayer(playerID);
                script->stackPush(Variable(VT_INT32,
                        input->getBtnState( (Button)(btnID) )));

                return SCRIPT_CONTINUE;
            }
            break;

            case OP_GET_PLAYER_ANALOG_VALUE:
            {
                Axis axis;
                Ogre::Vector2 analogValue;
                uint8 analogStick = script->stackPop().getValue<int32>(true);
                uint32 playerID = script->stackPop().getValue<int32>(true);

                if (playerID < 1 || playerID > inputMan.getPlayerNum())
                {
                    SONETTO_THROW("Script input handler error: Unknown player ID");
                }

                switch (analogStick)
                {
                    case 0:
                        axis = AX_LEFT;
                    break;

                    case 1:
                        axis = AX_RIGHT;
                    break;

                    default:
                        SONETTO_THROW("Script input handler error: Invalid analog stick "
                                "parameter");
                    break;
                }

                PlayerInput *input = inputMan.getPlayer(playerID);

                analogValue = input->getAnalogValue(axis);
                script->stackPush(Variable(VT_FLOAT,analogValue.x));
                script->stackPush(Variable(VT_FLOAT,analogValue.y));

                return SCRIPT_CONTINUE;
            }
            break;

            default:
                SONETTO_THROW("Script input handler error: Unrecognized opcode");
            break;
        }
    }
} // namespace
