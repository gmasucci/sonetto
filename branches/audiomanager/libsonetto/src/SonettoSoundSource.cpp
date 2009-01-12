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
    SoundSource::SoundSource(size_t id,Ogre::Node *node)
            : mSoundID(id), mMaxVolume(1.0f), mNode(node)
    {
        Ogre::Vector3 pos;

        // Gets AudioManager singleton
        mAudioMan = AudioManager::getSingletonPtr();

        // Generates OpenAL audio source
        alGenSources(1,&mALSource);
        mAudioMan->_alErrorCheck("SoundSource::SoundSource()","Failed generating "
                "OpenAL audio source");

        // Attaches buffer to sound source
        alSourcei(mALSource,AL_BUFFER,mAudioMan->_getSound(mSoundID).buffer);
        mAudioMan->_alErrorCheck("SoundSource::SoundSource()","Failed attaching "
                "audio buffer to OpenAL audio source");

        if (mNode) {
            pos = mNode->_getDerivedPosition();
        } else {
            pos = mAudioMan->_getListenerPos();
        }

        alSource3f(mALSource,AL_POSITION,pos.x,pos.y,pos.z);
        mAudioMan->_alErrorCheck("SoundSource::SoundSource()","Failed positioning "
                "OpenAL audio source");
    }
    //-----------------------------------------------------------------------------
    SoundSource::~SoundSource()
    {
        // If this sound source is invalid, its OpenAL audio source was already
        // deleted, so we can't do this again
        if (isValid())
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
        if (isValid())
        {
            mMaxVolume = Math::clamp(maxVolume,0.0f,1.0f);
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
        if (isValid())
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
    void SoundSource::_invalidate()
    {
        // Throws an exception if the sound source was already invalidated
        if (!isValid())
        {
            SONETTO_THROW("Invalidating an already invalidated sound source");
        }

        // Deletes sound source (it won't be needed anymore)
        alDeleteSources(1,&mALSource);
        mAudioMan->_alErrorCheck("SoundSource::_removeBuffer()","Failed deleting "
                "OpenAL audio source");

        // Invalidates sound source
        mSoundID = 0;
    }
    //-----------------------------------------------------------------------------
    void SoundSource::_update()
    {
        // Only updates valid sound sources
        if (isValid())
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
            mAudioMan->_alErrorCheck("SoundSource::SoundSource()","Failed positioning "
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
        // Only plays valid sound sources that are not playing yet
        if (isValid() && getState() != SSS_PLAYING)
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
        if (isValid() && getState() == SSS_PLAYING)
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
        if (isValid() && getState() != SSS_STOPPED)
        {
            alSourceStop(mALSource);
            mAudioMan->_alErrorCheck("SoundSource::stop()","Failed stopping "
                    "OpenAL audio source");
        }
    }
}
