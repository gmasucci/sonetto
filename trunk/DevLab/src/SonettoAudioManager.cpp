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

#include <cstdio>
#include <vector>
#include <queue>
#include <Ogre.h>
#include <OgreException.h>
#include <al.h>
#include <alc.h>
#include <efx.h>
#include <efx-creative.h>
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
        ALint  attribs[4] = { 0,0,0,0 }; // Context creation attribute list

        // Fails if already initialised
        if (mInitialised) {
            KERNEL->mLogMan->logMessage("( AudioManager::initialise() ) AudioManager was asked to be "
                                        "initialised twice. This will possibly cause errors.",
                                        Ogre::LML_CRITICAL);

            return false;
        }

        // Opens desired audio device
        mpDevice = alcOpenDevice(device);
        if (!mpDevice) {
            KERNEL->mLogMan->logMessage("( AudioManager::initialise() ) OpenAL failed openning "
                                        "audio device.");

            return false;
        }

        // Cleans up errors (no errors may have happened, but this ensures so)
        alGetError();
        alcGetError(mpDevice);

        // Checks whether the current device supports sound effects
        if (alcIsExtensionPresent(mpDevice,"ALC_EXT_EFX") == AL_FALSE) {
            KERNEL->mLogMan->logMessage("( AudioManager::initialise() ) OpenAL Effects Extension "
                                        "not present.");

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
                                        "context.");

            return false;
        }

        // We are only going to use one render context, so we select it
        alcMakeContextCurrent(mpContext);

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
                                            "function pointers could not be retrieved.");

                mUseEffectsExt = false;
            }
        }

        // Generates an environmental auxiliary effect slot
        if (mUseEffectsExt) {
            ALCint maxSends; // Max auxiliary effect slot sends

            // Checks if we have at least 1 auxiliary effect slot sends
            // If yes, we should create the environmental effect slot,
            // a Reverb effect and attach it to the slot
            alcGetIntegerv(mpDevice,ALC_MAX_AUXILIARY_SENDS,1,&maxSends);
            if (maxSends < 1) {
                KERNEL->mLogMan->logMessage("( AudioManager::initialise() ) Audio device doesn't support "
                                            "auxiliary effect slot sends.");

                mEnvSlotID = 0;
            } else {
                ALuint effectID = 0;

                alGenAuxiliaryEffectSlots(1,&mEnvSlotID);
                if (alGetError() != AL_NO_ERROR) {
                    KERNEL->mLogMan->logMessage("( AudioManager::initialise() ) Could not create "
                                                "environmental effect slot.");

                    mEnvSlotID = 0;
                } else {
                    // If one of these fails, it will cascade until the error check
                    alGenEffects(1,&effectID);
                    if (alGetError() != AL_NO_ERROR) {
                        KERNEL->mLogMan->logMessage("( AudioManager::initialise() ) Could not create "
                                                    "environmental effect.");

                        alDeleteAuxiliaryEffectSlots(1,&mEnvSlotID);
                        mEnvSlotID = 0;
                    } else {
                        alEffecti(effectID,AL_EFFECT_TYPE,AL_EFFECT_EAXREVERB);
                        if (alGetError() != AL_NO_ERROR) {
                            KERNEL->mLogMan->logMessage("( AudioManager::initialise() ) EAX Reverb "
                                                        "is not supported by this audio device.");

                            if (alIsEffect(effectID))
                                alDeleteEffects(1,&effectID);

                            alDeleteAuxiliaryEffectSlots(1,&mEnvSlotID);
                            mEnvSlotID = 0;
                        } else {
                            alEffectf(effectID,AL_EAXREVERB_DECAY_TIME,20.0f);
                            alAuxiliaryEffectSloti(mEnvSlotID,AL_EFFECTSLOT_EFFECT,effectID);
                            if (alGetError() != AL_NO_ERROR) {
                                KERNEL->mLogMan->logMessage("( AudioManager::initialise() ) Could not attach effect "
                                                            "in the effect slot.");

                                if (alIsEffect(effectID))
                                    alDeleteEffects(1,&effectID);

                                alDeleteAuxiliaryEffectSlots(1,&mEnvSlotID);
                                mEnvSlotID = 0;
                            }
                        }
                    }
                }
            }
        }

        // We're fine
        mInitialised = true;
        return true;
    }

    bool AudioManager::deinitialise() {
        if (!mInitialised) {
            KERNEL->mLogMan->logMessage("( AudioManager::deinitialise() ) AudioManager was asked to be "
                                        "deinitialised before its initialisation. This will possibly "
                                        "cause errors.",Ogre::LML_CRITICAL);

            return false;
        }

        // Deletes music streams
        while (!mStreamQueue.empty()) {
            MusicStream *stream = mStreamQueue.front();
            alDeleteSources(1,&stream->sourceID);
            alDeleteBuffers(2,stream->bufferIDs);
            ov_clear(&stream->oggStream);

            delete stream;
            mStreamQueue.pop();
        }

        // Deletes music information
        while (!mMusics.empty()) {
            MusicInfo *info = mMusics.back();
            delete info;

            mMusics.pop_back();
        }

        // Deletes sound sources
        while (!mSoundSources.empty()) {
            SoundSource *source = mSoundSources.begin()->second;

            alDeleteSources(1,&source->sourceID);
            if (mUseEffectsExt && alIsFilter(source->filterID))
                alDeleteFilters(1,&source->filterID);

            delete source;
            mSoundSources.erase(mSoundSources.begin());
        }

        // Deletes sound infos
        while (!mSounds.empty()) {
            SoundInfo *info = mSounds.back();
            alDeleteBuffers(1,&info->bufferID);

            delete info;
            mSounds.pop_back();
        }

        // Delete environmental effect slot and the effect within it
        if (mUseEffectsExt && alIsAuxiliaryEffectSlot(mEnvSlotID)) {
            ALint effectID;

            // Get attached effect, and delete the effect and the effectslot
            alGetAuxiliaryEffectSloti(mEnvSlotID,AL_EFFECTSLOT_EFFECT,&effectID);
            alDeleteEffects(1,(ALuint *)(&effectID));
            alDeleteAuxiliaryEffectSlots(1,&mEnvSlotID);
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

    size_t AudioManager::addMusic(string filename,bool loopEnabled,ogg_int64_t loopBegin,
                                  ogg_int64_t loopEnd) {
        MusicInfo *info = new MusicInfo;

        // Set struct fields
        info->filename    = filename;
        info->loopEnabled = loopEnabled;
        info->loopBegin   = loopBegin;
        info->loopEnd     = loopEnd;

        // Insert into vector and return index
        // The deletion of this struct happens when the AudioManager
        // gets deinitialise()'ed
        mMusics.push_back(info);
        return mMusics.size()-1;
    }

    size_t AudioManager::addSound(string filename,float minGain,float maxGain,float gain,
                                  float rolloffFactor,bool loopEnabled,ogg_int64_t loopBegin,
                                  ogg_int64_t loopEnd) {
        int             errCode;              // OGG Vorbis error code
        OggVorbis_File  oggStream;            // Compressed OGG stream
        SoundInfo      *info = new SoundInfo; // New sound information
        char           *data = NULL;          // Pointer to where we will decompress
                                              // the OGG stream
        ostringstream   str;                  // String formatter
        ALenum          format;               // Audio format (AL_FORMAT_MONO16, etc)
        int             bitstream = 0;        // OGG bitstream
        ogg_int64_t     size = 0;             // Bytes read from stream
        ogg_int64_t     totalBytes = 0;       // Total decompressed bytes in stream

        // Generates an OpenAL audio buffer
        alGenBuffers(1,&info->bufferID);

        // Set fields
        info->minGain       = minGain;
        info->maxGain       = maxGain;
        info->gain          = gain;
        info->rolloffFactor = rolloffFactor;
        info->loopEnabled   = loopEnabled;

        // Opens OGG vorbis file
        str << "sound/" << filename;
        errCode = ov_fopen((char *)(str.str().c_str()),&oggStream);
        if (errCode < 0) {
            // Throws an exception if something bad happens
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                        "A required audio file is missing or it is not an OGG Vorbis file ("+
                        Ogre::String(str.str())+")","AudioManager::addSound()");
        }

        // Allocates enough room for the PCM samples of the stream and its channels
        totalBytes = ov_pcm_total(&oggStream,-1)*2;
        data = new char[totalBytes];

        // A loopEnd of zero means one wants the loop to end on the end of the music file
        if (loopEnd == 0)
            loopEnd = ov_pcm_total(&oggStream,-1)-loopBegin;

        // Decompress data
        ov_pcm_seek(&oggStream,loopBegin);
        while (size < (loopEnd-loopBegin)*2) {
            int read = ov_read(&oggStream,data+size,totalBytes-size,0,2,1,&bitstream);

            if (read > 0) {
                // Some data was read
                size += read;
            } else {
                // read <  0 means an error has occurred
                // read == 0 means there isn't more data to read (ie. end of file)
                if (read < 0) {
                    OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                                "OGG Vorbis data is corrupted ("+Ogre::String(str.str())+")",
                                "AudioManager::addSound()");
                }
            }
        }

        // If no data was read something must be wrong
        if (size == 0) {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                        "An audio file seems to be empty ("+Ogre::String(str.str())+")",
                        "AudioManager::addSound()");
        }

        // Discovers file format (mono or stereo)
        if (oggStream.vi->channels == 1) {
            format = AL_FORMAT_MONO16;
        } else {
            format = AL_FORMAT_STEREO16;
        }

        // Fill an OpenAL uncompressed PCM data buffer with our
        // just decompressed buffer
        alBufferData(info->bufferID,format,data,size,oggStream.vi->rate);
        mSounds.push_back(info);

        // Deletes data buffer and returns the sound info index
        delete[] data;
        return mSounds.size()-1;
    }

    void AudioManager::playMusic(size_t musicID, float fadeOutSpd, float fadeInSpd, float musicVolume,
                                 ogg_uint64_t pcmPos, bool fadeOutMem, bool fadeOutMemPos) {
        if (!mInitialised) {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                        "Call to a function member before its class' initialisation",
                        "AudioManager::pauseMusic()");
        }

        int              errCode;                     // OGG Vorbis error code
        MusicInfo       *music     = NULL;            // Music information for the new music
        MusicStream     *stream    = new MusicStream; // New music
        MusicStream     *outStream = NULL;            // Music to be faded out
        ostringstream    str;                         // String formatter

        // Makes sure we're talking about an existing music
        if (musicID >= mMusics.size()) {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                        "MusicID outbounds mMusics size",
                        "AudioManager::playMusic()");
        }

        // Gets music information and creates an OGG stream
        music   = mMusics[musicID];
        str << "music/" << music->filename;
        errCode = ov_fopen((char *)(str.str().c_str()),&stream->oggStream);
        if (errCode < 0) {
            // Throws an exception if something bad happens
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                        "A required audio file is missing or it is not an OGG Vorbis file ("+
                        Ogre::String(str.str())+")","AudioManager::playMusic()");
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

            // Set up for memorizing on fade out
            outStream->fadeOutMem    = fadeOutMem;
            outStream->fadeOutMemPos = fadeOutMemPos;
        } else {
            alSourcePlay(stream->sourceID);
        }

        // And push this new music back in the queue
        mStreamQueue.push(stream);
    }

    int AudioManager::playSound(size_t soundID,Ogre::SceneNode *parentNode,bool useEnvEffect) {
        if (!mInitialised) {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                        "Call to a function member before its class' initialisation",
                        "AudioManager::pauseMusic()");
        }

        size_t       index  = 0;
        SoundInfo   *info   = NULL;
        SoundSource *source = NULL;

        // Checks whether soundID is valid
        if (soundID >= mSounds.size()) {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                        "SoundID outbounds mSounds size",
                        "AudioManager::playSound()");
        }

        // Gets sound information
        info = mSounds[soundID];

        // Creates and setups the new sound source
        source             = new SoundSource;
        source->soundID    = soundID;
        source->filterID   = 0;
        source->parentNode = parentNode;

        // Creates one OpenAL sound source and checks for errors
        alGenSources(1,&source->sourceID);
        if (alGetError() != AL_NO_ERROR) {
            KERNEL->mLogMan->logMessage("( AudioManager::playSound() ) OpenAL was unable to "
                                        "generate a sound source.");

            delete source;
            return -1;
        }

        // Sets OpenAL source configuration, based on mSounds[soundID]
        // <fixme> Set rolloff factor, etc...
        alSourcei(source->sourceID,AL_BUFFER,info->bufferID);
        alSourcef(source->sourceID,AL_MIN_GAIN,info->minGain);
        alSourcef(source->sourceID,AL_MAX_GAIN,info->maxGain);
        alSourcef(source->sourceID,AL_GAIN,info->gain);
        alSourcef(source->sourceID,AL_ROLLOFF_FACTOR,info->rolloffFactor);
        alSourcei(source->sourceID,AL_LOOPING,info->loopEnabled);

        // Creates one OpenAL audio filter and checks for errors
        if (mUseEffectsExt) {
            alGenFilters(1,&source->filterID);
            if (alGetError() != AL_NO_ERROR) {
                KERNEL->mLogMan->logMessage("( AudioManager::playSound() ) OpenAL was unable to "
                                            "generate an audio filter.");

                source->filterID = 0;
            } else {
                alFilteri(source->filterID,AL_FILTER_TYPE,AL_FILTER_LOWPASS);
                if (alGetError() != AL_NO_ERROR) {
                    KERNEL->mLogMan->logMessage("( AudioManager::playSound() ) Current hardware doesn't "
                                                "support lowpass filter.");

                    alDeleteFilters(1,&source->filterID);
                    source->filterID = 0;
                }
            }

            if (useEnvEffect) {
                // Attaches direct filter to source
                if (alIsFilter(source->filterID)) {
                    // Direct output sound filtered by source->filterID
                    alSourcei(source->sourceID,AL_DIRECT_FILTER,source->filterID);
                    if (alGetError() != AL_NO_ERROR) {
                        KERNEL->mLogMan->logMessage("( AudioManager::playSound() ) OpenAL was unable to "
                                                    "attach a direct audio filter to sound source.");

                        // If we could not attach, we are not going to use it
                        alDeleteFilters(1,&source->filterID);
                        source->filterID = 0;
                    }
                }

                // Attaches a filter to the sound source, and the sound source to the environmental
                // effect slot
                if (alIsAuxiliaryEffectSlot(mEnvSlotID)) {
                    // Auxiliary Send 0 feeds mEnvSlotID through filter source->filterID
                    // If the filter is invalid, it will not be used
                    alSource3i(source->sourceID,AL_AUXILIARY_SEND_FILTER,mEnvSlotID,0,source->filterID);
                    if (alGetError() != AL_NO_ERROR) {
                        KERNEL->mLogMan->logMessage("( AudioManager::playSound() ) OpenAL was unable to "
                                                    "attach sound source to environmental effect slot or "
                                                    "audio filter to sound source auxiliary send.");

                        // If we could not attach the filter, we are not going to use it
                        if (alIsFilter(source->filterID)) {
                            alDeleteFilters(1,&source->filterID);
                            source->filterID = 0;
                        }
                    }
                } else {
                    alSource3i(source->sourceID,AL_AUXILIARY_SEND_FILTER,AL_EFFECTSLOT_NULL,1,AL_FILTER_NULL);
                }
            }
        }

        // Insert source in the first not used ID gap of mSoundSources map
        if (!mSoundSources.empty()) {
            size_t lastIndex = 0;
            map<size_t,SoundSource *>::iterator last = mSoundSources.end();

            // The iterator returned by mSoundSources.end() actually doesn't
            // point to the last item, but an invalid item after the last one,
            // so we decrement it one time and get the key of the last iterator
            lastIndex = (--last)->first;

            // Searches for a gap from the first to the last
            // plus one index available
            for (index = 0;index <= lastIndex+1;++index) {
                if (mSoundSources.find(index) == mSoundSources.end()) {
                    mSoundSources[index] = source;
                    break;
                }
            }
        } else {
            // If the map is empty, we use the first index (zero)
            index = 0;
            mSoundSources[0] = source;
        }

        // Plays our new sound source
        alSourcePlay(source->sourceID);

        // Throws exception for unexpected errors
        throwALError("Unable to play sound source","AudioManager::playSound()");

        // Returns a handle to the source
        return index;
    }

    bool AudioManager::sourceExists(size_t sourceID) {
        if (!mInitialised) {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                        "Call to a function member before its class' initialisation",
                        "AudioManager::sourceExists()");
        }

        // The ending iterator represents a non-existent index
        return mSoundSources.find(sourceID) != mSoundSources.end();
    }

    void AudioManager::setSourceNode(size_t sourceID,Ogre::SceneNode *node) {
        if (!mInitialised) {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                        "Call to a function member before its class' initialisation",
                        "AudioManager::setSourceNode()");
        }

        // Checks for existance
        if (!sourceExists(sourceID)) {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                        "Invalid sound source ID",
                        "AudioManager::setSourceNode()");
        }

        // Changes source to follow the new SceneNode
        mSoundSources[sourceID]->parentNode = node;
    }

    void AudioManager::setSourceGainRange(size_t sourceID,float minGain,float maxGain) {
        if (!mInitialised) {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                        "Call to a function member before its class' initialisation",
                        "AudioManager::setSourceGainRange()");
        }

        // Checks for existance
        if (!sourceExists(sourceID)) {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                        "Invalid sound source ID",
                        "AudioManager::setSourceGainRange()");
        }

        // Changes source properties
        alSourcef(mSoundSources[sourceID]->sourceID,AL_MIN_GAIN,minGain);
        alSourcef(mSoundSources[sourceID]->sourceID,AL_MAX_GAIN,maxGain);

        // Warn about errors
        if (alGetError() != AL_NO_ERROR) {
            KERNEL->mLogMan->logMessage("(AudioManager::setSourceGainRange()) Could not set source gain "
                                        "range.");
        }
    }

    void AudioManager::setSourceGain(size_t sourceID,float gain) {
        if (!mInitialised) {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                        "Call to a function member before its class' initialisation",
                        "AudioManager::setSourceGain()");
        }

        // Checks for existance
        if (!sourceExists(sourceID)) {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                        "Invalid sound source ID",
                        "AudioManager::setSourceGain()");
        }

        // Changes source properties
        alSourcef(mSoundSources[sourceID]->sourceID,AL_GAIN,gain);

        // Warn about errors
        if (alGetError() != AL_NO_ERROR)
            KERNEL->mLogMan->logMessage("(AudioManager::setSourceGain()) Could not set source gain.");
    }

    void AudioManager::setSourceRolloff(size_t sourceID,float factor) {
        if (!mInitialised) {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                        "Call to a function member before its class' initialisation",
                        "AudioManager::setSourceRolloff()");
        }

        // Checks for existance
        if (!sourceExists(sourceID)) {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                        "Invalid sound source ID",
                        "AudioManager::setSourceRolloff()");
        }

        // Changes source properties
        alSourcef(mSoundSources[sourceID]->sourceID,AL_ROLLOFF_FACTOR,factor);

        // Warn about errors
        if (alGetError() != AL_NO_ERROR) {
            KERNEL->mLogMan->logMessage("(AudioManager::setSourceRolloff()) Could not set source rolloff "
                                        "factor.");
        }
    }

    void AudioManager::setSourceFilterGain(size_t sourceID,float gain) {
        if (!mInitialised) {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                        "Call to a function member before its class' initialisation",
                        "AudioManager::setSourceFilterGain()");
        }

        // Checks for existance
        if (!sourceExists(sourceID)) {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                        "Invalid sound source ID",
                        "AudioManager::setSourceFilterGain()");
        }

        // If we are not using effects extension or if the source filter is not
        // valid, this function just fails on its purpose silently
        if (mUseEffectsExt && alIsFilter(mSoundSources[sourceID]->filterID)) {
            // Changes source properties
            alFilterf(mSoundSources[sourceID]->filterID,AL_LOWPASS_GAIN,gain);

            // Warn about errors
            if (alGetError() != AL_NO_ERROR) {
                KERNEL->mLogMan->logMessage("(AudioManager::setSourceFilterGain()) Could not set source "
                                            "rolloff factor.");
            }
        }
    }

    void AudioManager::setSourceFilterGainHF(size_t sourceID,float gain) {
        if (!mInitialised) {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                        "Call to a function member before its class' initialisation",
                        "AudioManager::setSourceFilterGainHF()");
        }

        // Checks for existance
        if (!sourceExists(sourceID)) {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                        "Invalid sound source ID",
                        "AudioManager::setSourceFilterGainHF()");
        }

        // If we are not using effects extension or if the source filter is not
        // valid, this function just fails on its purpose silently
        if (mUseEffectsExt && alIsFilter(mSoundSources[sourceID]->filterID)) {
            // Changes source properties
            alFilterf(mSoundSources[sourceID]->filterID,AL_LOWPASS_GAINHF,gain);

            // Warn about errors
            if (alGetError() != AL_NO_ERROR) {
                KERNEL->mLogMan->logMessage("(AudioManager::setSourceFilterGainHF()) Could not set source "
                                            "rolloff factor.");
            }
        }
    }

    void AudioManager::pauseMusic() {
        if (!mInitialised) {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                        "Call to a function member before its class' initialisation",
                        "AudioManager::pauseMusic()");
        }

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
        if (!mInitialised) {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                        "Call to a function member before its class' initialisation",
                        "AudioManager::pauseMusic()");
        }

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
        if (!mInitialised) {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                        "Call to a function member before its class' initialisation",
                        "AudioManager::pauseMusic()");
        }

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

    bool AudioManager::memorizeMusic(bool memorizePos) {
        if (!mInitialised) {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                        "Call to a function member before its class' initialisation",
                        "AudioManager::memorizeMusic()");
        }

        if (!mStreamQueue.empty()) {
            MusicStream *stream;      // Currently playing music stream
            ALenum       sourceState; // Sound source state(playing, paused, etc)

            stream = mStreamQueue.front();

            // We can memorize a being played and a paused music,
            // but we couldn't memorize a stopped one, as it would
            // probably be pumped from the queue on the next loop
            // Such a thing should never happen, but who knows?
            alGetSourcei(stream->sourceID,AL_SOURCE_STATE,&sourceState);
            if (sourceState == AL_PLAYING || sourceState == AL_PAUSED) {
                // Also we won't memorize a music which is being faded out (fadeDir == -1)
                if (stream->fadeDir != -1) {
                    int         sourceOffset; // PCM sample lastly played by OpenAL
                    int         processed;    // Number of buffers already played by OpenAL
                    ogg_int64_t pcmPos;       // OGG Stream PCM position

                    // Those values got from `stream' were set there at AudioManager::playMusic()
                    mMusicMem.musicID   = stream->musicID;
                    mMusicMem.state     = sourceState;
                    mMusicMem.maxVolume = stream->maxVolume;

                    if (memorizePos) {
                        size_t pcmBufferSize = BUFFER_SIZE; // Buffer size in PCM samples

                        // If there are two channels per PCM sample,
                        // a buffer size measured in PCM samples should
                        // be the number of physical bytes divided by two
                        if (stream->oggStream.vi->channels == 2)
                            pcmBufferSize = pcmBufferSize/2;

                        // Discovers how many PCM samples were already played by OpenAL
                        alGetSourcei(stream->sourceID,AL_SAMPLE_OFFSET,&sourceOffset);
                        alGetSourcei(stream->sourceID,AL_BUFFERS_PROCESSED,&processed);

                        // Do not take into account buffers already played
                        // The offset of buffers already played can be took
                        // from the OGG stream
                        sourceOffset -= processed*pcmBufferSize;

                        // Gets the number of PCM samples read from the OGG stream
                        pcmPos = ov_pcm_tell(&stream->oggStream);

                        // Memorize PCM position
                        mMusicMem.pcmPos = pcmPos-(pcmBufferSize-sourceOffset);
                    } else {
                        // If we were asked not to memorize the
                        // position, we set it to zero
                        mMusicMem.pcmPos = 0;
                    }

                    // Memorized ok
                    mMemorizedMusic = true;
                    return true;
                }
            }
        }

        // There isn't any music to be memorized
        // (the queue is empty, the only one is stopped or is fading out)
        return false;
    }

    bool AudioManager::restoreMusic(float fadeOutSpd,float fadeInSpd) {
        if (!mInitialised) {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                        "Call to a function member before its class' initialisation",
                        "AudioManager::pauseMusic()");
        }

        // Play memory (if existent)
        if (mMemorizedMusic) {
            playMusic(mMusicMem.musicID,fadeOutSpd,fadeInSpd,mMusicMem.maxVolume,mMusicMem.pcmPos);

            return true;
        }

        return false;
    }

    bool AudioManager::update() {
        if (!mInitialised) {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                        "Call to a function member before its class' initialisation",
                        "AudioManager::pauseMusic()");
        }

        Ogre::Vector3  camPos;                 // Camera position
        Ogre::Vector3  camDir;                 // Camera direction
        ALfloat        listenerOrientation[6]; // Listener orientation

        // <fixme> Get real deltatime
        Ogre::Real deltatime = 0.012f;

        // Gets the camera position and direction
        camPos = KERNEL->mModuleList.top()->mCamera->getPosition();
        camDir = KERNEL->mModuleList.top()->mCamera->getDirection();

        // Setups listener orientation values
        // The first 3 array fields compose a vector defining the up direction
        // of the listener's orientation
        // The last 3 ones compose the actual direction, based on the up one
        // <fixme> Put listener orientation to work correctly
        // listenerOrientation[0] = 0.0f;
        // listenerOrientation[1] = 0.0f;
        // listenerOrientation[2] = 1.0f;
        // listenerOrientation[3] = camDir.x;
        // listenerOrientation[4] = camDir.y;
        // listenerOrientation[5] = camDir.z;

        // Makes the listener follow the camera
        //alListener3f(AL_POSITION,camPos.x,camPos.y,camPos.z);
        // alListenerfv(AL_ORIENTATION,listenerOrientation);

        // Updates musics
        if (!mStreamQueue.empty()) {
            int            processed; // Processed buffers
            MusicInfo     *musicInfo; // Current music information
            MusicStream   *stream;    // Current music stream

            // Gets current music's stream and information
            stream    = mStreamQueue.front();
            musicInfo = mMusics[stream->musicID];

            // Makes it follow the camera
            // This isn't by stereo musics, but one may want to use mono musics
            alSource3f(stream->sourceID,AL_POSITION,camPos.x,camPos.y,camPos.z);
            alSource3f(stream->sourceID,AL_DIRECTION,camDir.x,camDir.y,camDir.z);

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
                    // Finished fading
                    stream->fadeDir = 0;

                    // Memorize it, if asked to
                    if (stream->fadeOutMem) {
                        // Small error checking, just to ensure no future surprises
                        if (!memorizeMusic(stream->fadeOutMemPos)) {
                            KERNEL->mLogMan->logMessage("( AudioManager::update() ) Failed memorizing "
                                                        "music on fade out.");
                        }
                    }

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
        if (mSoundSources.size() > 0) {
            size_t lastIndex;
            map<size_t,SoundSource *>::iterator iter = mSoundSources.end();

            // Gets the key of the last item
            lastIndex = (--iter)->first;
            for (size_t i = 0;i <= lastIndex;++i) {
                // Skips invalid indexes (map gaps)
                if (mSoundSources.find(i) == mSoundSources.end())
                    continue;

                ALenum       state;
                SoundSource *source = mSoundSources.find(i)->second;

                // Discovers what out OpenAL sound source is doing
                alGetSourcei(source->sourceID,AL_SOURCE_STATE,&state);
                if (state == AL_STOPPED) {
                    // If our sound source is stopped, then it's time
                    // to deallocate it and remove it from the map
                    alDeleteSources(1,&source->sourceID);
                    if (mUseEffectsExt && alIsFilter(source->filterID))
                        alDeleteFilters(1,&source->filterID);

                    delete source;
                    mSoundSources.erase(i);
                } else {
                    // Or if our sound source is playing or paused, we
                    // must update its coordinates
                    Ogre::SceneNode *node;
                    Ogre::Vector3    nodePos(0,0,0);
                    Ogre::Vector3    nodeDir(0,0,0);

                    // Gets SceneNode's(or Camera's, if NULL) position and direction
                    node = source->parentNode;
                    if (node) {
                        nodePos = node->_getDerivedPosition();
                        // <todo> Get camera direction
                        // nodeDir = node->_getDerivedDirection();
                    } else {
                        nodePos = camPos;
                        nodeDir = camDir;
                    }

                    alSource3f(source->sourceID,AL_POSITION,nodePos.x,nodePos.y,nodePos.z);
                    alSource3f(source->sourceID,AL_DIRECTION,nodeDir.x,nodeDir.y,nodeDir.z);
                }
            }
        }

        return true;
    }

    void AudioManager::throwALError(const char *errMsg,const char *source) {
        Ogre::String context("");

        // If this is an OpenAL Context error, we should insert the
        // word 'Context' to the error message
        if (alcGetError(mpDevice) != ALC_NO_ERROR)
            context = "Context ";

        // Throws an Ogre::Exception if OpenAL's state is not error-free
        if (alGetError() != AL_NO_ERROR || context != "") {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                        "OpenAL "+context+"Error: "+Ogre::String(errMsg),
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
            if (mMusics[stream->musicID]->loopEnd != 0 && pos > mMusics[stream->musicID]->loopEnd) {
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
        alBufferData(buffer,format,data,size-endingOverflow,sInfo->rate);
        throwALError("Out of memory, could not allocate sound buffer",
                     "AudioManager::pullStream()");

        return true;
    }
}; // namespace
