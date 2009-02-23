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
#include "SonettoAudioManager.h"
#include "SonettoVariable.h"
#include "SonettoScriptAudioHandler.h"

namespace Sonetto
{
    //--------------------------------------------------------------------------
    // Sonetto::OpDataPush implementation.
    //--------------------------------------------------------------------------
    OpDataPush::OpDataPush(OpcodeHandler *aHandler)
            : Opcode(aHandler)
    {
        arguments.push_back(
                OpcodeArgument(sizeof(variable._getRawType()),
                &variable._getRawType()));
        arguments.push_back(
                OpcodeArgument(sizeof(variable._int),&variable._int));

        calculateArgsSize();
    }
    //--------------------------------------------------------------------------
    // Sonetto::OpDataPushVar implementation.
    //--------------------------------------------------------------------------
    OpDataPushVar::OpDataPushVar(OpcodeHandler *aHandler)
            : Opcode(aHandler)
    {
        arguments.push_back(OpcodeArgument(sizeof(scope),&scope));
        arguments.push_back(OpcodeArgument(sizeof(varIndex),&varIndex));

        calculateArgsSize();
    }
    //--------------------------------------------------------------------------
    // Sonetto::OpDataPopVar implementation.
    //--------------------------------------------------------------------------
    OpDataPopVar::OpDataPopVar(OpcodeHandler *aHandler)
            : Opcode(aHandler)
    {
        arguments.push_back(OpcodeArgument(sizeof(scope),&scope));
        arguments.push_back(OpcodeArgument(sizeof(varIndex),&varIndex));

        calculateArgsSize();
    }
    //--------------------------------------------------------------------------
    // Sonetto::OpDataVarChg implementation.
    //--------------------------------------------------------------------------
    OpDataVarChg::OpDataVarChg(OpcodeHandler *aHandler)
            : Opcode(aHandler)
    {
        arguments.push_back(OpcodeArgument(sizeof(scope),&scope));
        arguments.push_back(OpcodeArgument(sizeof(varIndex),&varIndex));
        arguments.push_back(OpcodeArgument(sizeof(operation),&operation));

        calculateArgsSize();
    }
    //--------------------------------------------------------------------------
    // Sonetto::ScriptAudioHandler implementation.
    //--------------------------------------------------------------------------
    void ScriptAudioHandler::registerOpcodes()
    {
        ScriptManager &scriptMan = ScriptManager::getSingleton();

        // Registers opcodes that should be handled by ScriptAudioHandler
        scriptMan._registerOpcode(OP_PLAY_BGM,new Opcode(this));
        scriptMan._registerOpcode(OP_PLAY_ME,new Opcode(this));
        scriptMan._registerOpcode(OP_STOP_MUSIC,new Opcode(this));
        scriptMan._registerOpcode(OP_PAUSE_MUSIC,new Opcode(this));
        scriptMan._registerOpcode(OP_RESUME_MUSIC,new Opcode(this));
        scriptMan._registerOpcode(OP_GET_ID_FROM_SOUNDSET,new Opcode(this));
        scriptMan._registerOpcode(OP_PLAY_SOUND,new Opcode(this));
    }
    //--------------------------------------------------------------------------
    void ScriptAudioHandler::unregisterOpcodes()
    {
        ScriptManager &scriptMan = ScriptManager::getSingleton();

        scriptMan._unregisterOpcode(OP_PLAY_BGM);
        scriptMan._unregisterOpcode(OP_PLAY_ME);
        scriptMan._unregisterOpcode(OP_STOP_MUSIC);
        scriptMan._unregisterOpcode(OP_PAUSE_MUSIC);
        scriptMan._unregisterOpcode(OP_RESUME_MUSIC);
        scriptMan._unregisterOpcode(OP_GET_ID_FROM_SOUNDSET);
        scriptMan._unregisterOpcode(OP_PLAY_SOUND);
    }
    //--------------------------------------------------------------------------
    int ScriptAudioHandler::handleOpcode(Script *script,size_t id,
            Opcode *opcode)
    {
        Database &database = Database::getSingleton();
        AudioManager &audioMan = AudioManager::getSingleton();

        switch (id)
        {
            case OP_PLAY_BGM:
            {
                float fadeIn = script->stackPop().getValue<float>(false);
                float fadeOut = script->stackPop().getValue<float>(false);
                uint32 musicID = script->stackPop().getValue<int32>(true);

                if (musicID < 1 || musicID > database.musics.size())
                {
                    SONETTO_THROW("Script audio handler error: Unknown "
                            "music ID");
                }

                audioMan.playBGM(musicID,fadeOut,fadeIn);

                return SCRIPT_SUSPEND_NEXT;
            }
            break;

            case OP_PLAY_ME:
            {
                float fadeIn = script->stackPop().getValue<float>(false);
                float fadeOut = script->stackPop().getValue<float>(false);
                uint32 musicID = script->stackPop().getValue<int32>(true);

                if (musicID < 1 || musicID > database.musics.size())
                {
                    SONETTO_THROW("Script audio handler error: Unknown "
                            "music ID");
                }

                audioMan.playME(musicID,fadeOut,fadeIn);

                return SCRIPT_SUSPEND_NEXT;
            }
            break;

            case OP_STOP_MUSIC:
            {
                float fadeOut = script->stackPop().getValue<float>(false);
                audioMan.stopMusic(fadeOut);
                return SCRIPT_SUSPEND_NEXT;
            }
            break;

            case OP_PAUSE_MUSIC:
            {
                float fadeOut = script->stackPop().getValue<float>(false);
                audioMan.pauseMusic(fadeOut);
                return SCRIPT_SUSPEND_NEXT;
            }
            break;

            case OP_RESUME_MUSIC:
            {
                float fadeIn = script->stackPop().getValue<float>(false);
                audioMan.resumeMusic(fadeIn);
                return SCRIPT_SUSPEND_NEXT;
            }
            break;

            case OP_GET_ID_FROM_SOUNDSET:
            {
                uint32 soundID = script->stackPop().getValue<int32>(true);
                uint32 setID = script->stackPop().getValue<int32>(true);

                if (setID == 0 || soundID == 0)
                {
                    script->stackPush(Variable(VT_INT32,0));
                    return SCRIPT_CONTINUE;
                }

                if (setID > database.soundSets.size())
                {
                    SONETTO_THROW("Script audio handler error: Unknown "
                            "soundset ID");
                }

                if (setID > database.soundSets[setID - 1].getSounds().size())
                {
                    SONETTO_THROW("Script audio handler error: Unknown sound "
                            "ID in soundset");
                }

                script->stackPush(Variable(VT_INT32,
                        AudioManager::fromSoundSet(setID,soundID)));

                return SCRIPT_CONTINUE;
            }
            break;

            case OP_PLAY_SOUND:
            {
                uint32 soundID = script->stackPop().getValue<int32>(true);

                if (soundID < 1 || soundID > database.sounds.size())
                {
                    SONETTO_THROW("Script audio handler error: Unknown sound ID");
                }

                audioMan.playSound(soundID);

                return SCRIPT_SUSPEND_NEXT;
            }
            break;

            default:
                SONETTO_THROW("Script audio handler error: Unrecognized opcode");
            break;
        }
    }
} // namespace
