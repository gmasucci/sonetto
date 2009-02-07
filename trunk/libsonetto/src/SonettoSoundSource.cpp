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

#include <AL/al.h>
#include "SonettoKernel.h"
#include "SonettoSoundSource.h"
#include "SonettoAudioManager.h"

namespace Sonetto
{
    //-----------------------------------------------------------------------------
    // Sonetto::SoundDef implementation.
    //-----------------------------------------------------------------------------
    const char *SoundDef::FOLDER = "sound/";
    //-----------------------------------------------------------------------------
    // Sonetto::SoundSource implementation.
    //-----------------------------------------------------------------------------
    SoundSource::SoundSource()
            : mSoundID(0),mMaxVolume(1.0f),mNode(NULL)
    {
        // Gets AudioManager singleton
        mAudioMan = AudioManager::getSingletonPtr();
    }
    //-----------------------------------------------------------------------------
    SoundSource::~SoundSource()
    {
        // If this sound source is invalid, its OpenAL audio source was already
        // deleted, so we can't do this again
        if (mSoundID > 0)
        {
            // Deletes OpenAL audio source
            alDeleteSources(1,&mALSource);
            mAudioMan->_alErrorCheck("SoundSource::~SoundSource()","Failed deleting "
                    "OpenAL audio source");
        }
    }
    //-----------------------------------------------------------------------------
    void SoundSource::setMaxVolume(float maxVolume)
    {
        mMaxVolume = Math::clamp(maxVolume,0.0f,1.0f);

        if (mSoundID > 0)
        {
            alSourcef(mALSource,AL_GAIN,maxVolume *
                    mAudioMan->getMasterSoundVolume());
            mAudioMan->_alErrorCheck("SoundSource::setMaxVolume()","Failed setting "
                    "OpenAL audio source gain");
        }
    }
    //-----------------------------------------------------------------------------
    SoundSourceState SoundSource::getState() const
    {
        // If this sound source is invalid, this method will report it is stopped
        if (mSoundID > 0)
        {
            ALenum state;

            // Gets OpenAL audio source state
            alGetSourcei(mALSource,AL_SOURCE_STATE,&state);
            mAudioMan->_alErrorCheck("SoundSource::getState()","Failed getting "
                    "OpenAL audio source state");

            // Returns corresponding SoundSourceState
            switch (state)
            {
                case AL_PLAYING: return SSS_PLAYING; break;
                case AL_PAUSED:  return SSS_PAUSED; break;

                default: break;
            }
        }

        return SSS_STOPPED;
    }
    //-----------------------------------------------------------------------------
    void SoundSource::setSoundID(uint32 id)
    {
        if (id > 0) {
            if (mSoundID == 0)
            {
                // Generates OpenAL audio source
                alGenSources(1,&mALSource);
                mAudioMan->_alErrorCheck("SoundSource::setSoundID()","Failed "
                        "generating OpenAL audio source");
            }

            alSourceStop(mALSource);

            // Attaches buffer to sound source
            alSourcei(mALSource,AL_BUFFER,mAudioMan->_getSound(id).buffer);
            mAudioMan->_alErrorCheck("SoundSource::setSoundID()","Failed attaching "
                    "audio buffer to OpenAL audio source");

            alSourcef(mALSource,AL_GAIN,mMaxVolume *
                    mAudioMan->getMasterSoundVolume());
            mAudioMan->_alErrorCheck("SoundSource::setSoundID()","Failed setting "
                    "OpenAL audio source gain");
        } else {
            if (mSoundID > 0)
            {
                alDeleteSources(1,&mALSource);
                mAudioMan->_alErrorCheck("SoundSource::setSoundID()","Failed deleting "
                        "OpenAL audio source");
            }
        }

        mSoundID = id;
    }
    //-----------------------------------------------------------------------------
    void SoundSource::setNode(Ogre::Node *node)
    {
        mNode = node;

        if (mSoundID > 0)
        {
            Ogre::Vector3 pos;

            if (node) {
                pos = node->_getDerivedPosition();
            } else {
                pos = mAudioMan->_getListenerPos();
            }

            alSource3f(mALSource,AL_POSITION,pos.x,pos.y,pos.z);
            mAudioMan->_alErrorCheck("SoundSource::setNode()","Failed positioning "
                    "OpenAL audio source");
        }
    }
    //-----------------------------------------------------------------------------
    void SoundSource::_update()
    {
        // Only updates valid sound sources
        if (mSoundID > 0)
        {
            Ogre::Vector3 pos;

            // Gets attached node position or AudioManager's listener position
            // if no node was attached
            if (mNode) {
                pos = mNode->_getDerivedPosition();
            } else {
                pos = mAudioMan->_getListenerPos();
            }

            // Sets new sound source position
            alSource3f(mALSource,AL_POSITION,pos.x,pos.y,pos.z);
            mAudioMan->_alErrorCheck("SoundSource::_update()","Failed positioning "
                    "OpenAL audio source");

            // Sets sound source gain based on its local maximum volume and
            // AudioManager's master sound volume
            alSourcef(mALSource,AL_GAIN,mMaxVolume *
                    mAudioMan->getMasterSoundVolume());
            mAudioMan->_alErrorCheck("SoundSource::_update()","Failed setting "
                    "OpenAL audio source gain");
        }
    }
    //-----------------------------------------------------------------------------
    void SoundSource::play()
    {
        if (mSoundID > 0)
        {
            alSourcePlay(mALSource);
            mAudioMan->_alErrorCheck("SoundSource::play()","Failed playing "
                    "OpenAL audio source");
        }
    }
    //-----------------------------------------------------------------------------
    void SoundSource::pause()
    {
        // Only pauses valid sound sources that are currently playing
        if (mSoundID > 0 && getState() == SSS_PLAYING)
        {
            alSourcePause(mALSource);
            mAudioMan->_alErrorCheck("SoundSource::pause()","Failed pausing "
                    "OpenAL audio source");
        }
    }
    //-----------------------------------------------------------------------------
    void SoundSource::stop()
    {
        // Only stops valid sound sources that are not stopped yet
        if (mSoundID > 0 && getState() != SSS_STOPPED)
        {
            alSourceStop(mALSource);
            mAudioMan->_alErrorCheck("SoundSource::stop()","Failed stopping "
                    "OpenAL audio source");
        }
    }
}
