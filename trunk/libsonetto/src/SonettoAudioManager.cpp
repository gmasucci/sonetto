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
#include <OgreStringConverter.h>
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
        // Updates music stream
        mMusicStream->_update(deltatime);

        // Gets rid of non-playing, unreferenced sound sources
        for (size_t i = 0;i < mSoundSources.size();++i)
        {
            if (mSoundSources[i].unique())
            {
                if (mSoundSources[i]->getState() != SSS_PLAYING)
                {
                    mSoundSources.erase(mSoundSources.begin()+i);
                    --i;
                }
            }
        }
    }
    //-----------------------------------------------------------------------------
    void AudioManager::loadSound(size_t id)
    {
        // Bounds checking
        if (id >= mSoundDefs.size())
        {
            SONETTO_THROW("Unknown sound ID");
        }

        // Throw exception if the sound was already loaded
        if (mSounds.find(id) != mSounds.end())
        {
            SONETTO_THROW("Trying to load an already loaded sound");
        }

        int errCode;
        int bitstream;
        ALenum format;
        size_t soundLen;
        size_t offset = 0;
        size_t buffer;
        std::string path = SoundDef::FOLDER+mSoundDefs[id].filename;
        char *constlessStr = new char[path.length()+1];
        char *tmpBuffer;
        OggVorbis_File file;

        // We use this `constlessStr' here because ov_fopen() needs a char *
        // argument as filename, but std::string::c_str() returns a const char *,
        // and I don't like const-casts
        strcpy(constlessStr,path.c_str());

        // Opens file and checks for errors
        errCode = ov_fopen(constlessStr,&file);
        if (errCode != 0)
        {
            SONETTO_THROW("Failed opening OGG/Vorbis file ("+
                    Ogre::StringConverter::toString(errCode)+")");
        }

        // Not needed anymore
        delete constlessStr;

        // Gets sound length to create audio buffer
        soundLen = static_cast<size_t>(ov_pcm_total(&file,-1))*2;
        if ((int)soundLen == OV_EINVAL)
        {
            SONETTO_THROW("Failed telling OGG/Vorbis sound length ("+
                    Ogre::StringConverter::toString((int)soundLen)+")");
        }

        // Creates OpenAL buffer
        alGenBuffers(1,&buffer);
        _alErrorCheck("AudioManager::loadSound()","Failed to generate an OpenAL "
                "audio buffer");

        // Creates temporary audio buffer
        tmpBuffer = new char[soundLen];

        // While we haven't loaded the sound, read more data from OGG stream
        while (offset < soundLen)
        {
            int read;

            if (soundLen-offset <= 1024) {
                char chunk[1024];
                read = ov_read(&file,chunk,1024,0,2,1,&bitstream);

                if (read > 0)
                {
                    // Copy part of data read to `data' buffer
                    read = soundLen-offset;
                    memcpy(tmpBuffer+offset,chunk,read);
                }
            } else {
                read = ov_read(&file,tmpBuffer+offset,soundLen-offset,0,2,1,
                               &bitstream);
            }

            if (read > 0) { // There were bytes read
                offset += read;
            } else
            if (read == 0) { // No bytes were read
                break;
            } else { // Error while reading from file
                SONETTO_THROW("Failed reading from OGG/Vorbis file ("+
                    Ogre::StringConverter::toString(read)+")");
            }
        }

        // Gets information about the stream and figures
        // whether the format is mono or stereo
        if (file.vi->channels == 1) {
            format = AL_FORMAT_MONO16;
        } else {
            format = AL_FORMAT_STEREO16;
        }

        // Fills an OpenAL audio data buffer with our
        // just decompressed audio data
        alBufferData(buffer,format,tmpBuffer,offset,file.vi->rate);
        _alErrorCheck("AudioManager::loadSound()","Could not fill OpenAL audio buffer");

        // Closes OGG/Vorbis file and deletes temporary buffer
        ov_clear(&file);
        delete tmpBuffer;

        // Inserts our buffer as a loaded sound in mSounds
        mSounds.insert(std::pair<size_t,Sound>(id,Sound(buffer)));
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
    SoundSourcePtr AudioManager::createSound(size_t id)
    {
        // Checks whether it's loaded or not
        if (mSounds.find(id) == mSounds.end())
        {
            SONETTO_THROW("Trying to play an sound which is not loaded");
        }

        SoundSourcePtr snd;

        // Creates a new SoundSource and encapsulates it into an Ogre::SharedPtr
        snd.bind(new SoundSource(id));

        // Adds to list of SoundSources to be deleted when not needed anymore
        mSoundSources.push_back(snd);

        return snd;
    }
    //-----------------------------------------------------------------------------
    void AudioManager::playSound(size_t id)
    {
        // Creates sound and plays it
        createSound(id)->play();
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
