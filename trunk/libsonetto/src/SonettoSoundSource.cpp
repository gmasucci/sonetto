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
    SoundSource::SoundSource(size_t id)
            : mSoundID(id)
    {
        // Gets AudioManager singleton
        mAudioMan = Kernel::get()->getAudioMan();

        // Generates OpenAL audio source
        alGenSources(1,&alSource);
        mAudioMan->_alErrorCheck("SoundSource::SoundSource()","Failed generating "
                "OpenAL audio source");

        // Attaches buffer to sound source
        alSourcei(alSource,AL_BUFFER,mAudioMan->_getSound(mSoundID).buffer);
        mAudioMan->_alErrorCheck("SoundSource::SoundSource()","Failed attaching "
                "audio buffer to OpenAL audio source");
    }
    //-----------------------------------------------------------------------------
    SoundSource::~SoundSource()
    {
        // Deletes OpenAL audio source
        alDeleteSources(1,&alSource);
        mAudioMan->_alErrorCheck("SoundSource::~SoundSource()","Failed deleting "
                "OpenAL audio source");

        std::cout << "Source deleted\n";
    }
    //-----------------------------------------------------------------------------
    SoundSourceState SoundSource::getState() const
    {
        ALenum state;

        // Get OpenAL audio source state
        alGetSourcei(alSource,AL_SOURCE_STATE,&state);
        mAudioMan->_alErrorCheck("SoundSource::getState()","Failed getting "
                "OpenAL audio source state");

        // Return corresponding state
        switch (state)
        {
            case AL_PLAYING: return SSS_PLAYING; break;
            case AL_PAUSED:  return SSS_PAUSED; break;

            default: break;
        }

        return SSS_STOPPED;
    }
    //-----------------------------------------------------------------------------
    void SoundSource::play()
    {
        if (getState() != SSS_PLAYING)
        {
            alSourcePlay(alSource);
            mAudioMan->_alErrorCheck("SoundSource::play()","Failed playing "
                    "OpenAL audio source");
        }
    }
    //-----------------------------------------------------------------------------
    void SoundSource::pause()
    {
        if (getState() == SSS_PLAYING)
        {
            alSourcePause(alSource);
            mAudioMan->_alErrorCheck("SoundSource::pause()","Failed pausing "
                    "OpenAL audio source");
        }
    }
    //-----------------------------------------------------------------------------
    void SoundSource::stop()
    {
        if (getState() != SSS_STOPPED)
        {
            alSourceStop(alSource);
            mAudioMan->_alErrorCheck("SoundSource::stop()","Failed stopping "
                    "OpenAL audio source");
        }
    }
}
