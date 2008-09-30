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

#include <sstream>
#include <AL/al.h>
#include "SonettoException.h"
#include "SonettoAudioManager.h"

namespace Sonetto
{
    //-----------------------------------------------------------------------------
    // Sonetto::AudioManager implementation.
    //-----------------------------------------------------------------------------
    void AudioManager::playBGM(size_t id,float fadeOut,float fadeIn)
    {
        // If there is nothing being played, start playing new BGM
        if (mMusicStream->getCurrentMusic() == NONE ||
            mMusicStream->isStopped()       == true ||
            mMusicStream->isPaused()        == true )
        {
            mMusicStream->_play(id,0,fadeIn,true);
            return;
        }

        if (fadeOut > 0.0f) {
            // Setups next BGM values (used in AudioManager::_fadeEnded()
            // and AudioManager::_streamEnded())
            mNextBGM     = id;
            mNextBGMFade = fadeIn;
            mNextBGMPos  = 0;

            // Stops the music with the desired fade out speed if the stream
            // is currently a BGM, or waits for the ME to end elsewise
            if (mMusicStream->_getLoop() == true)
            {
                mMusicStream->_stop(fadeOut);
            }
        } else {
            // If we don't want to fade out, simply play the music if the stream
            // is currently a BGM, or waits for the ME to end elsewise
            if (mMusicStream->_getLoop() == true) {
                mMusicStream->_play(id,0,fadeIn,true);
            } else {
                // Setups this BGM to be played when the ME end
                mNextBGM     = id;
                mNextBGMFade = fadeIn;
                mNextBGMPos  = 0;
            }
        }
    }
    //-----------------------------------------------------------------------------
    void AudioManager::playME(size_t id,float fadeOut,float fadeIn)
    {
        // If there is nothing being played, start playing new ME
        if (mMusicStream->getCurrentMusic() == NONE ||
            mMusicStream->isStopped()       == true ||
            mMusicStream->isPaused()        == true )
        {
            mMusicStream->_play(id,0,0.0f,false);
            return;
        }

        if (fadeOut > 0.0f) {
            if (mMusicStream->_getLoop() == true) { // BGM playing
                mNextME      = id;
                mNextBGMFade = fadeIn;
                mNextBGMPos  = 0;
                mMusicStream->_stop(fadeOut);
            } else { // ME playing
                mMusicStream->_play(id,0,0.0f,false);
            }
        } else {
            if (mMusicStream->_getLoop() == true) // BGM playing
            {
                mNextBGM     = mMusicStream->getCurrentMusic();
                mNextBGMFade = fadeIn;
                mNextBGMPos  = mMusicStream->_getCurrentPos();
            }

            mMusicStream->_play(id,0,0.0f,false);
        }
    }
    //-----------------------------------------------------------------------------
    void AudioManager::stopMusic(float fadeOut)
    {
        // Clears next BGM and ME values to make sure AudioManager::fadeEnded()
        // won't start the queued music when the fade out ends
        mNextBGM = NONE;
        mNextME  = NONE;

        // Stops the music
        if (mMusicStream->_getLoop() == true) { // BGM
            mMusicStream->_stop(fadeOut);
        } else { // ME (Music Effects don't fade)
            mMusicStream->_stop(0.0f);
        }
    }
    //-----------------------------------------------------------------------------
    void AudioManager::resumeMusic(float fadeIn)
    {
        mMusicStream->_resume(fadeIn);
    }
    //-----------------------------------------------------------------------------
    void AudioManager::pauseMusic(float fadeOut)
    {
        mMusicStream->_pause(fadeOut);
    }
    //-----------------------------------------------------------------------------
    AudioManager::AudioManager(const char *device)
            : mInitialised(false), mNextBGM(NONE), mNextBGMPos(0), mNextME(NONE)
    {
        ALCcontext *context;

        // Opens audio device
        mDevice = alcOpenDevice(device);

        // If an audio device couldn't be open, quit
        if (!mDevice)
        {
            return;
        }

        // Creates a rendering context
        context = alcCreateContext(mDevice,NULL);

        // If we couldn't create a rendering context, quit
        if (!context)
        {
            alcCloseDevice(mDevice);
            return;
        }

        // Activates created context
        alcMakeContextCurrent(context);

        // Clears error string
        alGetError();

        // <todo> Load music list from somewhere
        mMusics.push_back(Music("Field.ogg",0));
        mMusics.push_back(Music("Field.ogg.KAT",0));

        // Now that OpenAL is initialised, we can initialise the music stream
        mMusicStream = new MusicStream(this);

        // Everything is fine
        mInitialised = true;
    }
    //-----------------------------------------------------------------------------
    AudioManager::~AudioManager()
    {
        if (mInitialised)
        {
            ALCcontext *context;

            // Deletes music stream
            delete mMusicStream;

            // Destroys current rendering context
            context = alcGetCurrentContext();
            alcMakeContextCurrent(NULL);
            alcDestroyContext(context);

            // Closes audio device
            alcCloseDevice(mDevice);
        }
    }
    //-----------------------------------------------------------------------------
    void AudioManager::_update(float deltatime)
    {
        mMusicStream->_update(deltatime);
    }
    //-----------------------------------------------------------------------------
    void AudioManager::_streamEnded()
    {
        if (mNextBGM != NONE)
        {
            mMusicStream->_play(mNextBGM,mNextBGMPos,mNextBGMFade,true);
            mNextBGM = NONE;
        }
    }
    //-----------------------------------------------------------------------------
    void AudioManager::_fadeEnded(MusicFading fade)
    {
        switch (fade)
        {
            case MF_FADE_IN: break;

            case MF_FADE_OUT:
                if (mMusicStream->_getState() == MSS_STOPPING)
                {
                    if (mNextME != NONE) {
                        mNextBGM    = mMusicStream->getCurrentMusic();
                        mNextBGMPos = mMusicStream->_getCurrentPos();
                        mMusicStream->_play(mNextME,0,0.0f,false);
                        mNextME = NONE;
                    } else
                    if (mNextBGM != NONE) {
                        mMusicStream->_play(mNextBGM,0,mNextBGMFade,true);
                        mNextBGM = NONE;
                    }
                }
            break;

            default: break;
        }
    }
    //-----------------------------------------------------------------------------
    void AudioManager::_alErrorCheck(const char *location,const char *desc)
    {
        ALenum  alError  = alGetError();
        ALCenum alcError = alcGetError(mDevice);

        // Checks for errors
        if (alError != AL_NO_ERROR || alcError != ALC_NO_ERROR)
        {
            std::ostringstream format;
            std::string msg = "An OpenAL error has occurred in "+
                    std::string(location)+".\n  "+std::string(desc)+".\n  ";

            if (alError != AL_NO_ERROR)
            {
                format << std::hex << alError;
                msg += "AL error is: 0x"+format.str();
            }

            if (alcError != ALC_NO_ERROR)
            {
                format << std::hex << alcError;
                msg += ".\n  ALC error is: 0x"+format.str();
            }

            SONETTO_THROW(msg);
        }
    }
} // namespace
