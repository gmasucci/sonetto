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
#   include <windows.h> // Will crash, for sure
#endif
#include <cstdio>
#include <vector>
#include <queue>
#include <al.h>
#include <alc.h>
#include <vorbis/vorbisfile.h>
#include <Ogre.h>
#include <OgreException.h>
#include "SonettoKernel.h"
#include "SonettoAudioManager.h"

using namespace std;

namespace Sonetto {
    const size_t BUFFER_SIZE = 32768*5; // 160KB Sound Buffer

    bool AudioManager::initialise() {
        // Fails if we already initialised
        if (mInitialised) {
            KERNEL->mLogMan->logMessage("[AudioManager::initialise()] AudioManager was asked to be "
                                        "initialised twice. This will possibly cause errors.\n",
                                        Ogre::LML_CRITICAL);

            return false;
        }

        // Opens sound device and reports errors
        mDevice = alcOpenDevice(NULL);
        if (!mDevice) {
            KERNEL->mLogMan->logMessage("[AudioManager::initialise()] OpenAL failed openning "
                                        "sound device.\n");

            return false;
        }

        // Cleans up errors (no errors may have happened, but this ensures so)
        alGetError();

        // Creates a contexts in the device - NULL means no special option taken
        mContext = alcCreateContext(mDevice,NULL);
        alcMakeContextCurrent(mContext);

        // If there are errors, reports them and returns false
        if (alGetError() != AL_NO_ERROR) {
            KERNEL->mLogMan->logMessage("[AudioManager::initialise()] OpenAL failed creating "
                                        "render context.\n");

            return false;
        }

        //////////////// TODO: Remove this test area
        MusicInfo *info   = new MusicInfo;
        info->filename    = "music/music000.ogg";
        info->loopEnabled = true;
        info->loopBegin   = 0;
        info->loopEnd     = BUFFER_SIZE*90;
        mMusics.push_back(info);

        info              = new MusicInfo;
        info->filename    = "music/music001.ogg";
        info->loopEnabled = true;
        info->loopBegin   = 0;
        info->loopEnd     = BUFFER_SIZE*4;
        mMusics.push_back(info);

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
        ////////////////

        // We are fine ;-)
        mInitialised = true;
        return true;
    }

    bool AudioManager::deinitialise() {
        // If we haven't initialised, we have nothing to deinitialise
        if(!mInitialised)
            return false;

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

        // Releases current context and destroys it
        alcMakeContextCurrent(NULL);
        alcDestroyContext(mContext);

        // Closes sound device
        alcCloseDevice(mDevice);

        // Resets variables
        mDevice    = NULL;
        mContext   = NULL;
        mMemorized = false;

        // Not initialised anymore :-)
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
        stream->fade      = (fadeInSpd > 0.0f); // 0.0f fade in speed means no fade in
        stream->fadeSpd   = fadeInSpd;

        // If we are fading in, we start from 0.0f volume
        // Otherwise we should start with supplied volume
        if (stream->fade != 0) {
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
            outStream->fade    = -1*(fadeOutSpd != 0.0f);
            outStream->fadeSpd = fadeOutSpd;
            mStreamQueue.pop();
        }

        // Clean all pending musics
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

    int AudioManager::playSound(size_t soundID,Ogre::SceneNode *parentNode) {
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
            KERNEL->mLogMan->logMessage("[AudioManager::playSound()] OpenAL was unable to "
                                        "generate a sound source.");

            return -1;
        }

        // Sets OpenAL source configuration, based on mSounds[soundID]
        // <fixme> Set rolloff factor, etc...
        alSourcei(source->sourceID,AL_BUFFER,info->bufferID);
        alSourcef(source->sourceID,AL_GAIN,info->volume);
        alSourcei(source->sourceID,AL_LOOPING,info->loopEnabled);

        // Wait for changes to take effect
        // <fixme> REEEEEMOOOOOVEEEEE THIS CRAP!
        Sleep(15);

        mSoundSources.push_back(source); // Pushes new source in sources' vector
        alSourcePlay(source->sourceID);  // Plays our new sound source

        // Throws exception for unexpected errors
        throwALError("Unable to play sound source","AudioManager::playSound()");

        // Returns a handle to the source
        return mSoundSources.size()-1;
    }

    bool AudioManager::update() {
        Ogre::Real deltatime = 0.012f;

        // Updates musics
        if (!mStreamQueue.empty()) {
            int processed;
            float x,y,z;
            MusicInfo       *musicInfo;
            MusicStream     *stream;

            // Gets current music's stream and information
            stream = mStreamQueue.front();
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
                alGetSourcei(stream->sourceID,AL_SOURCE_STATE,&sourceState);
                if (sourceState != AL_PLAYING)
                    alSourcePlay(stream->sourceID);

                // Flip current active buffer (just for informational purposes)
                stream->activeBuffer = !stream->activeBuffer;
            }

            alGetListener3f(AL_POSITION,&x,&y,&z);
            // <fixme> Remove these comments when AudioManager is finished
            /*KERNEL->mDebugText->setMessage("MusicID: "+Ogre::StringConverter::toString(stream->musicID)+"\x0D"+
                                           Ogre::String("Buffer: ")+((stream->activeBuffer) ? "Back\x0D" : "Front\x0D")+
                                           Ogre::String("PCM Position: ")+
                                           Ogre::StringConverter::toString((Ogre::uint32)(ov_pcm_tell(&stream->oggStream)))+"\x0D"+
                                           Ogre::StringConverter::toString(x)+"/"+Ogre::StringConverter::toString(y)+"/"+
                                           Ogre::StringConverter::toString(z));*/

            // Handles fade in/out
            switch (stream->fade) {
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
                    stream->fade = 0;
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
                //nodePos = KERNEL->mModuleList.top()->mCamera->getWorldPosition();
            }

            alSource3f(source->sourceID,AL_POSITION,nodePos[0],nodePos[1],nodePos[2]);
        }

        return true;
    }

    void AudioManager::pauseMusic() {
        if (!mStreamQueue.empty()) {
            MusicStream *stream;
            ALenum sourceState;

            alGetSourcei(stream->sourceID,AL_SOURCE_STATE,&sourceState);
            if (sourceState == AL_PLAYING)
                alSourcePause(stream->sourceID);
        }
    }

    void AudioManager::resumeMusic() {
        if (!mStreamQueue.empty()) {
            MusicStream *stream;
            ALenum sourceState;

            alGetSourcei(stream->sourceID,AL_SOURCE_STATE,&sourceState);
            if (sourceState == AL_PAUSED)
                alSourcePlay(stream->sourceID);
        }
    }

    void AudioManager::throwALError(const char *errMsg,const char *source) {
        // Throws an Ogre::Exception if OpenAL is not error-free
        if (alGetError() != AL_NO_ERROR) {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                        "OpenAL Error: "+Ogre::String(errMsg),
                        source);
        }
    }

    bool AudioManager::pullStream(MusicStream *stream,ALuint buffer) {
        ALenum       format;
        char         data[BUFFER_SIZE];
        size_t       size = 0;
        vorbis_info *sInfo;
        ogg_uint64_t pos;
        size_t       endingOverflow = 0;

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
        if (size == 0) {
            printf("wtf?\n");
            return false;
        }

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
        switch(alGetError()) {
            case AL_OUT_OF_MEMORY:
                printf("AL_OUT_OF_MEMORY... WTF???\n");
            break;
            case AL_INVALID_VALUE:
                printf("AL_INVALID_VALUE... Hum.. makes sense\n");
            break;
            case AL_INVALID_ENUM:
                printf("AL_INVALID_ENUM: WTRFFFFFFFFFFFFFFFFf?????\n");
            break;
        }
        throwALError("Out of memory, could not allocate sound buffer",
                     "AudioManager::pullStream()");

        return true;
    }

    void AudioManager::stopMusic(float fadeSpd,bool allMusics) {
        if (!mStreamQueue.empty()) {
            MusicStream *stream = mStreamQueue.front();

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
                stream->fade    = -1;      // Fade out
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

    void AudioManager::memorizeMusic() {
        if (!mStreamQueue.empty()) {
            MusicStream *stream;
            ALenum       sourceState;

            // Gets currently being played music stream
            stream = mStreamQueue.front();
            alGetSourcei(stream->sourceID,AL_SOURCE_STATE,&sourceState);

            // And memorize its information (only if it is being played)
            if (sourceState == AL_PLAYING) {
                int sourceOffset;
                int processed;

                mMemorized  = true;            // Flags we have something to remember
                mMemMusicID = stream->musicID; // Memorizes music ID

                alGetSourcef(stream->sourceID,AL_GAIN,&mMemVolume); // Memorizes volume

                // Discovers which byte we are playing at this moment
                // and how many buffers were already processed
                alGetSourcei(stream->sourceID,AL_BYTE_OFFSET,&sourceOffset);
                alGetSourcei(stream->sourceID,AL_BUFFERS_PROCESSED,&processed);

                // The PCM offset in the stream is the last buffer read, multiplied by a buffer size
                // minus what rests to its completion
                sourceOffset -= processed*BUFFER_SIZE;
                mMemPosition = ov_pcm_tell(&stream->oggStream)-(BUFFER_SIZE-sourceOffset);
            }
        }
    }

    void AudioManager::restoreMusic(float fadeOutSpd,float fadeInSpd) {
        if (mMemorized)
            playMusic(mMemMusicID,fadeOutSpd,fadeInSpd,mMemVolume,mMemPosition);
    }
}
; // namespace
