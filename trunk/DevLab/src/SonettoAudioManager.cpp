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

#ifdef _WIN32
#   include <windows.h>
#endif
#include <cstdio>
#include <vector>
#include <queue>
#include <Ogre.h>
#include <OgreException.h>
#include <al.h>
#include <alc.h>
#include <efx.h>
#include <vorbis/vorbisfile.h>
#include "SonettoKernel.h"
#include "SonettoAudioManager.h"

using namespace std;

// OpenAL's effects extension function addresses
LPALGENFILTERS    alGenFilters    = NULL;
LPALDELETEFILTERS alDeleteFilters = NULL;
LPALISFILTER      alIsFilter      = NULL;
LPALFILTERI       alFilteri       = NULL;
LPALFILTERIV      alFilteriv      = NULL;
LPALFILTERF       alFilterf       = NULL;
LPALFILTERFV      alFilterfv      = NULL;
LPALGETFILTERI    alGetFilteri    = NULL;
LPALGETFILTERIV   alGetFilteriv   = NULL;
LPALGETFILTERF    alGetFilterf    = NULL;
LPALGETFILTERFV   alGetFilterfv   = NULL;

LPALGENEFFECTS    alGenEffects    = NULL;
LPALDELETEEFFECTS alDeleteEffects = NULL;
LPALISEFFECT      alIsEffect      = NULL;
LPALEFFECTI       alEffecti       = NULL;
LPALEFFECTIV      alEffectiv      = NULL;
LPALEFFECTF       alEffectf       = NULL;
LPALEFFECTFV      alEffectfv      = NULL;
LPALGETEFFECTI    alGetEffecti    = NULL;
LPALGETEFFECTIV   alGetEffectiv   = NULL;
LPALGETEFFECTF    alGetEffectf    = NULL;
LPALGETEFFECTFV   alGetEffectfv   = NULL;

LPALGENAUXILIARYEFFECTSLOTS    alGenAuxiliaryEffectSlots    = NULL;
LPALDELETEAUXILIARYEFFECTSLOTS alDeleteAuxiliaryEffectSlots = NULL;
LPALISAUXILIARYEFFECTSLOT      alIsAuxiliaryEffectSlot      = NULL;
LPALAUXILIARYEFFECTSLOTI       alAuxiliaryEffectSloti       = NULL;
LPALAUXILIARYEFFECTSLOTIV      alAuxiliaryEffectSlotiv      = NULL;
LPALAUXILIARYEFFECTSLOTF       alAuxiliaryEffectSlotf       = NULL;
LPALAUXILIARYEFFECTSLOTFV      alAuxiliaryEffectSlotfv      = NULL;
LPALGETAUXILIARYEFFECTSLOTI    alGetAuxiliaryEffectSloti    = NULL;
LPALGETAUXILIARYEFFECTSLOTIV   alGetAuxiliaryEffectSlotiv   = NULL;
LPALGETAUXILIARYEFFECTSLOTF    alGetAuxiliaryEffectSlotf    = NULL;
LPALGETAUXILIARYEFFECTSLOTFV   alGetAuxiliaryEffectSlotfv   = NULL;

namespace Sonetto {
    const size_t BUFFER_SIZE = 163840; // 160KB Sound Buffer

    bool AudioManager::initialise(const char *device) {
        ALCint maxSends;                 // Max send request
        ALint  attribs[4] = { 0,0,0,0 }; // Context creation attribute list

        // Fails if already initialised
        if (mInitialised) {
            KERNEL->mLogMan->logMessage("( AudioManager::initialise() ) AudioManager was asked to be "
                                        "initialised twice. This will possibly cause errors.\n",
                                        Ogre::LML_CRITICAL);

            return false;
        }

        // Opens desired audio device
        mpDevice = alcOpenDevice(device);
        if (!mpDevice) {
            KERNEL->mLogMan->logMessage("( AudioManager::initialise() ) OpenAL failed openning "
                                        "audio device.\n");

            return false;
        }

        // Cleans up errors (no errors may have happened, but this ensures so)
        alGetError();

        // Checks whether the current device supports sound effects
        if (alcIsExtensionPresent(mpDevice,"ALC_EXT_EFX") == AL_FALSE) {
            KERNEL->mLogMan->logMessage("( AudioManager::initialise() ) OpenAL Effects Extension "
                                        "not present.\n");

            mUseEffectsExt = false;
        }

        // Request 1 auxiliary effect slot sends (how many auxiliary effect slots
        // can a source feed)
        if (mUseEffectsExt) {
            attribs[0] = ALC_MAX_AUXILIARY_SENDS;
            attribs[1] = 1;
        }

        // Creates a rendering context in the audio device
        mpContext = alcCreateContext(mpDevice,attribs);
        if (!mpContext) {
            KERNEL->mLogMan->logMessage("( AudioManager::initialise() ) OpenAL Failed creating rendering "
                                        "context.\n");

            return false;
        }

        // We are only going to use one render context, so we select it
        alcMakeContextCurrent(mpContext);

        // Checks if we have at least 1 auxiliary effect slot sends
        // We won't use effects extension if not
        if (mUseEffectsExt) {
            alcGetIntegerv(mpDevice,ALC_MAX_AUXILIARY_SENDS,1,&maxSends);
            if (maxSends < 1) {
                KERNEL->mLogMan->logMessage("( AudioManager::initialise() ) Audio device doesn't support "
                                            "auxiliary effect slot sends.\n");

                mUseEffectsExt = false;
            }
        }

        // Get effects extension function pointers
        // (I know, this is ugly like hell)
        if (mUseEffectsExt) {
            alGenFilters    = (LPALGENFILTERS   )alGetProcAddress("alGenFilters");
            alDeleteFilters = (LPALDELETEFILTERS)alGetProcAddress("alDeleteFilters");
            alIsFilter      = (LPALISFILTER     )alGetProcAddress("alIsFilter");
            alFilteri       = (LPALFILTERI      )alGetProcAddress("alFilteri");
            alFilteriv      = (LPALFILTERIV     )alGetProcAddress("alFilteriv");
            alFilterf       = (LPALFILTERF      )alGetProcAddress("alFilterf");
            alFilterfv      = (LPALFILTERFV     )alGetProcAddress("alFilterfv");
            alGetFilteri    = (LPALGETFILTERI   )alGetProcAddress("alGetFilteri");
            alGetFilteriv   = (LPALGETFILTERIV  )alGetProcAddress("alGetFilteriv");
            alGetFilterf    = (LPALGETFILTERF   )alGetProcAddress("alGetFilterf");
            alGetFilterfv   = (LPALGETFILTERFV  )alGetProcAddress("alGetFilterfv");

            alGenEffects    = (LPALGENEFFECTS   )alGetProcAddress("alGenEffects");
            alDeleteEffects = (LPALDELETEEFFECTS)alGetProcAddress("alDeleteEffects");
            alIsEffect      = (LPALISEFFECT     )alGetProcAddress("alIsEffect");
            alEffecti       = (LPALEFFECTI      )alGetProcAddress("alEffecti");
            alEffectiv      = (LPALEFFECTIV     )alGetProcAddress("alEffectiv");
            alEffectf       = (LPALEFFECTF      )alGetProcAddress("alEffectf");
            alEffectfv      = (LPALEFFECTFV     )alGetProcAddress("alEffectfv");
            alGetEffecti    = (LPALGETEFFECTI   )alGetProcAddress("alGetEffecti");
            alGetEffectiv   = (LPALGETEFFECTIV  )alGetProcAddress("alGetEffectiv");
            alGetEffectf    = (LPALGETEFFECTF   )alGetProcAddress("alGetEffectf");
            alGetEffectfv   = (LPALGETEFFECTFV  )alGetProcAddress("alGetEffectfv");

            alGenAuxiliaryEffectSlots    = (LPALGENAUXILIARYEFFECTSLOTS   )alGetProcAddress("alGenAuxiliaryEffectSlots");
            alDeleteAuxiliaryEffectSlots = (LPALDELETEAUXILIARYEFFECTSLOTS)alGetProcAddress("alDeleteAuxiliaryEffectSlots");
            alIsAuxiliaryEffectSlot      = (LPALISAUXILIARYEFFECTSLOT     )alGetProcAddress("alIsAuxiliaryEffectSlot");
            alAuxiliaryEffectSloti       = (LPALAUXILIARYEFFECTSLOTI      )alGetProcAddress("alAuxiliaryEffectSloti");
            alAuxiliaryEffectSlotiv      = (LPALAUXILIARYEFFECTSLOTIV     )alGetProcAddress("alAuxiliaryEffectSlotiv");
            alAuxiliaryEffectSlotf       = (LPALAUXILIARYEFFECTSLOTF      )alGetProcAddress("alAuxiliaryEffectSlotf");
            alAuxiliaryEffectSlotfv      = (LPALAUXILIARYEFFECTSLOTFV     )alGetProcAddress("alAuxiliaryEffectSlotfv");
            alGetAuxiliaryEffectSloti    = (LPALGETAUXILIARYEFFECTSLOTI   )alGetProcAddress("alGetAuxiliaryEffectSloti");
            alGetAuxiliaryEffectSlotiv   = (LPALGETAUXILIARYEFFECTSLOTIV  )alGetProcAddress("alGetAuxiliaryEffectSlotiv");
            alGetAuxiliaryEffectSlotf    = (LPALGETAUXILIARYEFFECTSLOTF   )alGetProcAddress("alGetAuxiliaryEffectSlotf");
            alGetAuxiliaryEffectSlotfv   = (LPALGETAUXILIARYEFFECTSLOTFV  )alGetProcAddress("alGetAuxiliaryEffectSlotfv");

            // Checks whether one of them failed
            if (!alGenFilters     || !alDeleteFilters || !alIsFilter                   ||
                    !alFilteri    || !alFilteriv      || !alFilterf  || !alFilterfv    ||
                    !alGenEffects || !alDeleteEffects || !alIsEffect                   ||
                    !alEffecti    || !alEffectiv      || !alEffectf  || !alEffectfv    ||
                    !alGenAuxiliaryEffectSlots        || !alDeleteAuxiliaryEffectSlots ||
                    !alIsAuxiliaryEffectSlot          || !alAuxiliaryEffectSloti       ||
                    !alAuxiliaryEffectSlotiv          || !alAuxiliaryEffectSlotf       ||
                    !alAuxiliaryEffectSlotfv                                              ) {
                KERNEL->mLogMan->logMessage("( AudioManager::initialise() ) OpenAL effects extension "
                                            "function pointers could not be retrieved.\n");

                mUseEffectsExt = false;
            }
        }

        // Generates an environmental auxiliary effect slot
        if (mUseEffectsExt) {
            alGenAuxiliaryEffectSlots(1,&mEnvEffectSlot);

            if (alGetError() != AL_NO_ERROR) {
                KERNEL->mLogMan->logMessage("( AudioManager::initialise() ) Could not create "
                                            "environmental effect slot.\n");

                mUseEffectsExt = false;
            }
        }

        ///////// <fixme> Test area
        SoundInfo *sinfo  = new SoundInfo;
        alGenBuffers(1,&sinfo->bufferID);
        sinfo->volume      = 1.0f;
        sinfo->loopEnabled = true;
        OggVorbis_File oggStream;
        char *data;
        ov_fopen("sounds/sound000.ogg",&oggStream);
        data = new char[ov_pcm_total(&oggStream,-1)*2];
        ALenum       format;
        int          bitstream = 0;
        size_t       size = 0;
        vorbis_info *sInfo;

        while (size < ov_pcm_total(&oggStream,-1)*2) {
            int read = ov_read(&oggStream,data+size,2048,0,2,1,&bitstream);

            if (read > 0) {
                // Some data was read
                size += read;
            } else {
                // read <  0 means an error has occurred
                // read == 0 means there isn't more data to read (ie. end of file)
                if (read < 0) {
                    OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                                "OGG Vorbis data is corrupted ()","AudioManager::pullStream()");
                } else {
                    break; // while loop
                }
            }
        }

        // If no data was read, the stream has ended
        if (size == 0)
            assert(NULL && "Screwed");

        // Gets information about the stream
        // and figures whether the format is
        // mono or stereo
        sInfo = ov_info(&oggStream,-1);
        if (sInfo->channels == 1) {
            format = AL_FORMAT_MONO16;
        } else {
            format = AL_FORMAT_STEREO16;
        }

        // Fill an OpenAL uncompressed PCM data buffer with our
        // just decompressed buffer
        alBufferData(sinfo->bufferID,format,data,size,sInfo->rate);
        mSounds.push_back(sinfo);
        /////////

        // We're fine
        mInitialised = true;
        return true;
    }

    bool AudioManager::deinitialise() {
        if (!mInitialised) {
            KERNEL->mLogMan->logMessage("( AudioManager::deinitialise() ) AudioManager was asked to be "
                                        "deinitialised before its initialisation. This will possibly "
                                        "cause errors.\n",Ogre::LML_CRITICAL);

            return false;
        }

        // Deletes music streams
        while(!mStreamQueue.empty()) {
            MusicStream *stream = mStreamQueue.front();
            alDeleteSources(1,&stream->sourceID);
            alDeleteBuffers(2,stream->bufferIDs);
            ov_clear(&stream->oggStream);

            delete stream;
            mStreamQueue.pop();
        }

        // Clears music info vector
        mMusics.clear();

        // Deletes sound sources
        while(!mSoundSources.empty()) {
            SoundSource *source = mSoundSources.back();
            alDeleteSources(1,&source->sourceID);

            delete source;
            mSoundSources.pop_back();
        }

        // Deletes sound infos
        while(!mSounds.empty()) {
            SoundInfo *info = mSounds.back();
            alDeleteBuffers(1,&info->bufferID);

            delete info;
            mSounds.pop_back();
        }

        // Releases current context and destroys it
        alcMakeContextCurrent(NULL);
        alcDestroyContext(mpContext);

        // Closes sound device
        alcCloseDevice(mpDevice);

        // Resets variables
        mpDevice    = NULL;
        mpContext   = NULL;

        // Not initialised anymore
        mInitialised = false;
        return true;
    }

    void AudioManager::playMusic(size_t musicID, float fadeOutSpd, float fadeInSpd, float musicVolume, ogg_uint64_t pcmPos) {
        int              errCode;
        MusicInfo       *music     = NULL;
        MusicStream     *stream    = new MusicStream;
        MusicStream     *outStream = NULL;

        // Makes sure we're talking about an existing music
        if (musicID >= mMusics.size()) {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                        "MusicID outbounds mMusics size",
                        "AudioManager::playMusic()");
        }

        // Gets music information and creates an OGG stream
        music   = mMusics[musicID];
        errCode = ov_fopen((char *)(music->filename.c_str()),&stream->oggStream);
        if (errCode < 0) {
            // Throws an exception if something bad happens
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                        "A required audio file is missing or is not an OGG Vorbis file ("+
                        Ogre::String(music->filename)+")","AudioManager::playMusic()");
        }

        // Creates an OpenAL source and two PCM data buffers (front/back buffer)
        alGenSources(1,&stream->sourceID);
        alGenBuffers(2,stream->bufferIDs);
        throwALError("Failed generating audio sources or data buffers",
                     "AudioManager::playMusic()");

        // Setups new stream configuration
        stream->musicID   = musicID;
        stream->maxVolume = musicVolume;
        stream->fadeDir   = (fadeInSpd > 0.0f); // 0.0f fade in speed means no fade in
        stream->fadeSpd   = fadeInSpd;

        // If we are fading in, we start from 0.0f volume
        // Otherwise we should start with the supplied volume
        if (stream->fadeDir != 0) {
            alSourcef(stream->sourceID,AL_GAIN,0.0f);
        } else {
            alSourcef(stream->sourceID,AL_GAIN,musicVolume);
        }

        // Sets the OGG stream to desired position, and fills front and back buffer
        // The back buffer will only get filled if the music is sufficiently long for that
        stream->bitstream = 0;
        ov_pcm_seek(&stream->oggStream,pcmPos);
        if (pullStream(stream,stream->bufferIDs[0]))
            alSourceQueueBuffers(stream->sourceID,1,&stream->bufferIDs[0]);
        if (pullStream(stream,stream->bufferIDs[1]))
            alSourceQueueBuffers(stream->sourceID,1,&stream->bufferIDs[1]);

        // Pops the actual music to set fade out, if needed
        if (!mStreamQueue.empty() && fadeOutSpd != 0.0f) {
            outStream = mStreamQueue.front();
            outStream->fadeDir = -1*(fadeOutSpd != 0.0f);
            outStream->fadeSpd = fadeOutSpd;
            mStreamQueue.pop();
        }

        // Clear all pending musics
        while (!mStreamQueue.empty()) {
            alDeleteSources(1,&mStreamQueue.front()->sourceID);
            alDeleteBuffers(2,mStreamQueue.front()->bufferIDs);
            ov_clear(&mStreamQueue.front()->oggStream);

            delete mStreamQueue.front();
            mStreamQueue.pop();
        }

        // If we are fading out a music, we should push it back in the queue
        // If not, we should start playing the new one already !
        if (outStream) {
            mStreamQueue.push(outStream);
        } else {
            alSourcePlay(stream->sourceID);
        }

        // And push this new music back in the queue
        mStreamQueue.push(stream);
    }

    int AudioManager::playSound(size_t soundID,Ogre::SceneNode *parentNode,ALenum filterType,bool useEnvEffect) {
        SoundInfo   *info   = NULL;
        SoundSource *source = NULL;

        // Checks whether soundID is valid
        if(soundID >= mSounds.size()) {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                        "SoundID outbounds mSounds size",
                        "AudioManager::playSound()");
        }

        // Gets sound information
        info = mSounds[soundID];

        // Creates and setups the new sound source
        source             = new SoundSource;
        source->soundID    = soundID;
        source->parentNode = parentNode;

        // Creates one OpenAL sound source and checks for errors
        alGenSources(1,&source->sourceID);
        if(alGetError() != AL_NO_ERROR) {
            KERNEL->mLogMan->logMessage("( AudioManager::playSound() ) OpenAL was unable to "
                                        "generate a sound source.");

            return -1;
        }

        // Creates one OpenAL audio filter and checks for errors
        alGenFilters(1,&source->filterID);
        if(alGetError() != AL_NO_ERROR) {
            KERNEL->mLogMan->logMessage("( AudioManager::playSound() ) OpenAL was unable to "
                                        "generate a audio filter.");

            return -1;
        }

        // Sets OpenAL source configuration, based on mSounds[soundID]
        // <fixme> Set rolloff factor, etc...
        alSourcei(source->sourceID,AL_BUFFER,info->bufferID);
        alSourcef(source->sourceID,AL_GAIN,info->volume);
        alSourcei(source->sourceID,AL_LOOPING,info->loopEnabled);

        // Attaches filter
        if(useEnvEffect) {
            alSource3i(source->sourceID,AL_AUXILIARY_SEND_FILTER,mEnvEffectSlot,0,source->filterID);
        } else {
            alSourcei(source->sourceID,AL_DIRECT_FILTER,mEnvEffectSlot);
        }

#ifdef _WIN32
        // Wait for changes to take effect
        // <fixme> REEEEEMOOOOOVEEEEE THIS CRAP!
        Sleep(15);
#endif

        mSoundSources.push_back(source); // Pushes new source in sources' vector
        alSourcePlay(source->sourceID);  // Plays our new sound source

        // Throws exception for unexpected errors
        throwALError("Unable to play sound source","AudioManager::playSound()");

        // Returns a handle to the source
        return mSoundSources.size()-1;
    }

    void AudioManager::pauseMusic() {
        if (!mStreamQueue.empty()) {
            MusicStream *stream;      // Current music stream
            ALenum       sourceState; // Current OpenAL source state

            // Switches to paused if source is playing
            alGetSourcei(stream->sourceID,AL_SOURCE_STATE,&sourceState);
            if (sourceState == AL_PLAYING)
                alSourcePause(stream->sourceID);
        }
    }

    void AudioManager::resumeMusic() {
        if (!mStreamQueue.empty()) {
            MusicStream *stream;      // Current music stream
            ALenum       sourceState; // Current OpenAL source state

            // Switches to playing if source is paused
            alGetSourcei(stream->sourceID,AL_SOURCE_STATE,&sourceState);
            if (sourceState == AL_PAUSED)
                alSourcePlay(stream->sourceID);
        }
    }

    void AudioManager::stopMusic(float fadeSpd,bool allMusics) {
        if (!mStreamQueue.empty()) {
            MusicStream *stream = mStreamQueue.front(); // Current music stream

            // Should we clear all musics from the queue?
            if (allMusics) {
                // Take off the actual music
                mStreamQueue.pop();

                // And delete the others
                while (!mStreamQueue.empty()) {
                    alDeleteSources(1,&mStreamQueue.front()->sourceID);
                    alDeleteBuffers(2,mStreamQueue.front()->bufferIDs);
                    ov_clear(&mStreamQueue.front()->oggStream);

                    delete mStreamQueue.front();
                    mStreamQueue.pop();
                }
            }

            if (fadeSpd != 0.0f) {
                // Set a fading
                stream->fadeDir = -1;      // Fade out
                stream->fadeSpd = fadeSpd; // Speed

                if (allMusics)
                    mStreamQueue.push(stream);
            } else {
                // Delete the actual music imediately
                alDeleteSources(1,&stream->sourceID);
                alDeleteBuffers(2,stream->bufferIDs);
                ov_clear(&stream->oggStream);

                delete stream;
            }
        }
    }

    bool AudioManager::update() {
        // <fixme> Get real deltatime
        Ogre::Real deltatime = 0.012f;

        // Updates musics
        if (!mStreamQueue.empty()) {
            int              processed; // Processed buffers
            MusicInfo       *musicInfo; // Current music information
            MusicStream     *stream;    // Current music stream

            // Gets current music's stream and information
            stream    = mStreamQueue.front();
            musicInfo = mMusics[stream->musicID];

            // Refills the buffers already processed
            alGetSourcei(stream->sourceID,AL_BUFFERS_PROCESSED,&processed);
            for (int i = 0;i < processed;++i) {
                ALenum sourceState;
                ALuint buffer;

                // Unqueue buffer
                alSourceUnqueueBuffers(stream->sourceID,1,&buffer);
                throwALError("Unexpected error (alSourceUnqueueBuffers())",
                             "AudioManager::update()");

                // Fill it with data from OGG stream
                // If this fails, it means that end of file was reached, and we need to jump
                // to the loop beginning if looping is enabled
                // If so, fills the buffer with fresh new data
                // If not, ends looping
                if (!pullStream(stream,buffer)) {
                    if (musicInfo->loopEnabled) {
                        ov_pcm_seek(&stream->oggStream,musicInfo->loopBegin);
                        pullStream(stream,buffer);
                    } else {
                        break; // for loop
                    }
                }

                // Requeue the just-filled buffers
                alSourceQueueBuffers(stream->sourceID,1,&buffer);
                throwALError("Unexpected error (alSourceQueueBuffers())",
                             "AudioManager::update()");

                // There is still buffers to be played!
                // The following lines ensures the source is still playing
                // It may stop playing if the buffer was played
                // so fast that the streaming was not suficiently fast to
                // feed OpenAL with data
                // This may be caused by, for example, dragging the game window
                alGetSourcei(stream->sourceID,AL_SOURCE_STATE,&sourceState);
                if (sourceState == AL_STOPPED)
                    alSourcePlay(stream->sourceID);
            }

            // Handles fade in/out
            switch (stream->fadeDir) {
            case -1: { // Fade out
                float gain;

                // Gets current volume (GAIN) and
                // decrements it based on stream's
                // fade speed
                alGetSourcef(stream->sourceID,AL_GAIN,&gain);
                gain -= stream->fadeSpd*deltatime;

                // Delete this music stream if volume reaches 0.0f
                if (gain <= 0.0f) {
                    alDeleteSources(1,&stream->sourceID);
                    alDeleteBuffers(2,stream->bufferIDs);
                    ov_clear(&stream->oggStream);

                    delete stream;
                    mStreamQueue.pop();

                    // If there are still queued musics, plays the top one
                    if (!mStreamQueue.empty())
                        alSourcePlay(mStreamQueue.front()->sourceID);

                    break;
                } else {
                    // Sets source's new volume
                    alSourcef(stream->sourceID,AL_GAIN,gain);
                }
            }
            break;

            case +1: { // Fade in
                float gain;

                // Gets source volume (GAIN) and increments
                // it based on stream's fade speed
                alGetSourcef(stream->sourceID,AL_GAIN,&gain);
                gain += stream->fadeSpd*deltatime;

                // If we have reached stream's maximum volume,
                // setups stream for no fading anymore
                if (gain >= stream->maxVolume) {
                    gain = stream->maxVolume;
                    stream->fadeDir = 0;
                    break;
                }

                // Sets source's new volume
                alSourcef(stream->sourceID,AL_GAIN,gain);
            }
            break;

            default:
                break;
            }
        }

        // Updates sound sources
        for(size_t i = 0;i < mSoundSources.size();++i) {
            Ogre::SceneNode *node;
            Ogre::Vector3    nodePos(0,0,0);
            SoundSource     *source = mSoundSources[i];

            // Gets SceneNode position(or Camera position, if NULL)
            node = source->parentNode;
            if(node) {
                nodePos = node->_getDerivedPosition();
            } else {
                nodePos = KERNEL->mModuleList.top()->mCamera->getPosition();
            }

            alSource3f(source->sourceID,AL_POSITION,nodePos[0],nodePos[1],nodePos[2]);
        }

        return true;
    }

    void AudioManager::throwALError(const char *errMsg,const char *source) {
        // Throws an Ogre::Exception if OpenAL's state is not error-free
        if (alGetError() != AL_NO_ERROR) {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                        "OpenAL Error: "+Ogre::String(errMsg),
                        source);
        }
    }

    bool AudioManager::pullStream(MusicStream *stream,ALuint buffer) {
        ALenum       format;             // Stream format (Mono/Stereo)
        char         data[BUFFER_SIZE];  // Temporary PCM buffer
        size_t       size = 0;           // How much data was already read
        vorbis_info *sInfo;              // Compressed stream information
        ogg_uint64_t pos;                // PCM position
        size_t       endingOverflow = 0; // Bytes to truncate

        // While we haven't filled a buffer, read more data from OGG stream
        while (size < BUFFER_SIZE) {
            int read = ov_read(&stream->oggStream,data+size,BUFFER_SIZE-size,0,2,1,&stream->bitstream);

            if (read > 0) {
                // Some data was read
                size += read;
            } else {
                // read <  0 means an error has occurred
                // read == 0 means there isn't more data to read (ie. end of file)
                if (read < 0) {
                    OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                                "OGG Vorbis data is corrupted ("+
                                Ogre::String(mMusics[stream->musicID]->filename)+
                                ")","AudioManager::pullStream()");
                } else {
                    break; // while loop
                }
            }
        }

        // If no data was read, the stream has ended
        if (size == 0)
            return false;

        // If loop is enabled, get current stream position. And
        // if we got past stream's loopEnd, we must truncate
        // the leading data
        if (mMusics[stream->musicID]->loopEnabled) {
            pos = ov_pcm_tell(&stream->oggStream);
            if (pos > mMusics[stream->musicID]->loopEnd) {
                // Calculates how much bytes to truncate
                endingOverflow = pos-(mMusics[stream->musicID]->loopEnd);

                // Rewinds stream to its beginning
                ov_pcm_seek(&stream->oggStream,mMusics[stream->musicID]->loopBegin);
            }
        }

        // Gets information about the stream
        // and figures whether the format is
        // mono or stereo
        sInfo = ov_info(&stream->oggStream,-1);
        if (sInfo->channels == 1) {
            format = AL_FORMAT_MONO16;
        } else {
            format = AL_FORMAT_STEREO16;
        }

        // Fill an OpenAL uncompressed PCM data buffer with our
        // just decompressed buffer
        printf("Alloc: %d bytes.\n",size-endingOverflow);
        alBufferData(buffer,format,data,size-endingOverflow,sInfo->rate);
        throwALError("Out of memory, could not allocate sound buffer",
                     "AudioManager::pullStream()");

        return true;
    }
}; // namespace
