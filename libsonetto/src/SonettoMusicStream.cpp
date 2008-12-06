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
#include "SonettoMusicStream.h"

namespace Sonetto
{
    //-----------------------------------------------------------------------------
    // Sonetto::Music implementation.
    //-----------------------------------------------------------------------------
    const char *Music::FOLDER = "music/";
    //-----------------------------------------------------------------------------
    // Sonetto::MusicStream implementation.
    //-----------------------------------------------------------------------------
    const size_t MusicStream::BUFFER_SIZE = 163840;
    //-----------------------------------------------------------------------------
    MusicStream::MusicStream(AudioManager *audioMan)
            : mAudioMan(audioMan), mMusic(0), mMaxVolume(1.0f), mLoop(true),
                mFade(MF_NO_FADE), mState(MSS_IDLE)
    {
        // Creates OpenAL audio buffers
        alGenBuffers(2,mMusicBuf);
        mAudioMan->_alErrorCheck("MusicStream::MusicStream()",
                                "Failed generating music buffers");

        // Creates music source
        alGenSources(1,&mMusicSrc);
        mAudioMan->_alErrorCheck("MusicStream::MusicStream()",
                                "Failed generating music source");
    }
    //-----------------------------------------------------------------------------
    MusicStream::~MusicStream()
    {
        // Destroys OGG file handle
        if (mMusic != 0)
        {
            ov_clear(&mFile);
        }

        // Destroys music source
        alDeleteSources(1,&mMusicSrc);
        mAudioMan->_alErrorCheck("MusicStream::~MusicStream()",
                                "Failed destroying music source");

        // Destroys OpenAL audio buffers
        alDeleteBuffers(2,mMusicBuf);
        mAudioMan->_alErrorCheck("MusicStream::~MusicStream()",
                                "Failed destroying music buffers");
    }
    //-----------------------------------------------------------------------------
    void MusicStream::_update(float deltatime)
    {
        if (mMusic != 0)
        {
            ALenum srcState;
            int    processed;
            size_t buffers[2];

            // Updates fading
            switch (mFade)
            {
                case MF_FADE_IN:
                    mFadeVolume += mFadeSpd*deltatime;

                    if (mFadeVolume >= 1.0f)
                    {
                        // Stop fading
                        mFadeVolume = 1.0f;
                        mFade = MF_NO_FADE;
                        mAudioMan->_fadeEnded(MF_FADE_IN);
                    }
                break;

                case MF_FADE_OUT:
                    mFadeVolume -= mFadeSpd*deltatime;

                    if (mFadeVolume <= 0.0f)
                    {
                        // Stop fading
                        mFadeVolume = 0.0f;
                        mFade = MF_NO_FADE;

                        mAudioMan->_fadeEnded(MF_FADE_OUT);

                        switch (mState)
                        {
                            case MSS_PAUSING:
                                alSourcePause(mMusicSrc);
                                mAudioMan->_alErrorCheck("MusicStream::_update()",
                                        "Failed pausing music source");
                            break;

                            case MSS_STOPPING:
                                alSourceStop(mMusicSrc);
                                mAudioMan->_alErrorCheck("MusicStream::_update()",
                                        "Failed stopping music source");
                            break;

                            default: break;
                        }
                    }
                break;

                default: break;
            }

            // Sets source volume
            alSourcef(mMusicSrc,AL_GAIN,mFadeVolume * mMaxVolume *
                    mAudioMan->getMasterMusicVolume());
            mAudioMan->_alErrorCheck("MusicStream::_update()",
                                    "Failed setting music source volume");

            // Refills processed buffer with more data from the stream
            alGetSourcei(mMusicSrc,AL_BUFFERS_PROCESSED,&processed);
            mAudioMan->_alErrorCheck("MusicStream::_update()",
                    "Failed getting number of processed buffers from music source");

            alSourceUnqueueBuffers(mMusicSrc,processed,buffers);
            mAudioMan->_alErrorCheck("MusicStream::_update()",
                    "Failed unqueuing processed buffers from music source");

            for (int i = 0;i < processed;++i)
            {
                if (mState == MSS_ENDED)
                {
                    mAudioMan->_streamEnded();
                    break;
                }

                // Gets more data from OGG/Vorbis stream and queue it back
                pullStream(buffers[i]);
                alSourceQueueBuffers(mMusicSrc,1,&buffers[i]);
                mAudioMan->_alErrorCheck("MusicStream::_update()",
                        "Failed queueing audio buffer in music source");
            }

            // Makes sure the music doesn't stuck stopped
            // It may happen when you drag the window for too long
            alGetSourcei(mMusicSrc,AL_SOURCE_STATE,&srcState);
            mAudioMan->_alErrorCheck("MusicStream::_update()",
                    "Failed getting music source state");

            if (srcState == AL_STOPPED)
            {
                if (mState != MSS_ENDED)
                {
                    alSourcePlay(mMusicSrc);
                    mAudioMan->_alErrorCheck("MusicStream::_update()",
                            "Failed playing music source");
                }
            }
        }

        _getCurrentPos();
    }
    //-----------------------------------------------------------------------------
    void MusicStream::_play(size_t id,size_t pos,float fadeIn,bool loop)
    {
        // Checks bounds
        if (id == 0 || id >= Kernel::get()->mDatabase->mMusicList.size()+1)
        {
            SONETTO_THROW("Unknown music ID");
        }

        int errCode;
        char fill;
        std::string path = Music::FOLDER+Kernel::get()->mDatabase->
                mMusicList[id-1].filename;
        char *constlessStr = new char[path.length()+1];

        // We use this `constlessStr' here because ov_fopen() needs a char *
        // argument as filename, but std::string::c_str() returns a const char *,
        // and I don't like const-casts
        strcpy(constlessStr,path.c_str());

        // Stops current music, if any
        _stop(0.0f);

        // Opens file and checks for errors
        errCode = ov_fopen(constlessStr,&mFile);
        if (errCode != 0)
        {
            SONETTO_THROW("Failed opening OGG/Vorbis file ("+
                    Ogre::StringConverter::toString(errCode)+")");
        }

        // We don't need this anymore
        delete constlessStr;

        // Tells the length of our stream
        mStreamLen = static_cast<size_t>(ov_pcm_total(&mFile,-1));
        if ((int)mStreamLen == OV_EINVAL)
        {
            SONETTO_THROW("Failed telling OGG/Vorbis stream length ("+
                    Ogre::StringConverter::toString((int)mStreamLen)+")");
        }

        // Sets current music index
        mMusic = id;

        // Sets fade state and speed
        mFadeSpd = Math::clamp(fadeIn,0.0f,1.0f);
        if (mFadeSpd == 0.0f) {
            mFadeVolume = 1.0f;
            mFade = MF_NO_FADE;
        } else {
            mFadeVolume = 0.0f;
            mFade = MF_FADE_IN;
        }

        // Sets whether to loop or not
        mLoop = loop;

        // Resets stream state
        mState = MSS_IDLE;

        // Seeks stream
        if (pos != 0)
        {
            errCode = ov_pcm_seek(&mFile,pos);
            if (errCode != 0)
            {
                SONETTO_THROW("Couldn't seek OGG/Vorbis stream position");
            }
        }

        // Fills buffers
        fill = 1;
        pullStream(mMusicBuf[0]);
        if (loop) {
            fill = 2;
            pullStream(mMusicBuf[1]);
        } else {
            if (mState != MSS_ENDED)
            {
                fill = 2;
                pullStream(mMusicBuf[1]);
            }
        }

        // Queues buffers
        alSourceQueueBuffers(mMusicSrc,fill,mMusicBuf);
        mAudioMan->_alErrorCheck("MusicStream::_play()",
                                "Failed queuing music buffers");

        // Sets source volume
        alSourcef(mMusicSrc,AL_GAIN,mFadeVolume*mMaxVolume);
        mAudioMan->_alErrorCheck("MusicStream::_play()",
                                "Failed setting music source volume");

        // Starts playing the music
        alSourcePlay(mMusicSrc);
        mAudioMan->_alErrorCheck("MusicStream::_play()",
                                "Failed playing music source");
    }
    //-----------------------------------------------------------------------------
    void MusicStream::_stop(float aFadeOut)
    {
        if (mMusic != 0)
        {
            float fadeOut = Math::clamp(aFadeOut,0.0f,1.0f);
            if (fadeOut == 0.0f) {
                int buffers;
                size_t unqueue[2];

                ov_clear(&mFile);

                alSourceStop(mMusicSrc);
                mAudioMan->_alErrorCheck("MusicStream::_stop()",
                                        "Couldn't stop music source");

                alGetSourcei(mMusicSrc,AL_BUFFERS_PROCESSED,&buffers);
                mAudioMan->_alErrorCheck("MusicStream::_stop()",
                                        "Couldn't get number of processed buffers");

                alSourceUnqueueBuffers(mMusicSrc,buffers,unqueue);
                mAudioMan->_alErrorCheck("MusicStream::_stop()",
                                        "Couldn't unqueue buffers");

                mMusic = 0;
            } else {
                mState   = MSS_STOPPING;
                mFadeSpd = fadeOut;
                mFade    = MF_FADE_OUT;
            }
        }
    }
    //-----------------------------------------------------------------------------
    void MusicStream::_resume(float aFadeIn)
    {
        if (mMusic != 0)
        {
            ALenum srcState;

            // Checks whether the source is paused before resuming it
            alGetSourcei(mMusicSrc,AL_SOURCE_STATE,&srcState);
            mAudioMan->_alErrorCheck("MusicStream::_resume()",
                    "Failed getting music source state");

            if (srcState == AL_PAUSED)
            {
                float fadeIn = Math::clamp(aFadeIn,0.0f,1.0f);
                // Starts playing music
                alSourcePlay(mMusicSrc);
                mAudioMan->_alErrorCheck("MusicStream::_resume()",
                    "Failed playing music source");

                // Sets state to idle
                mState = MSS_IDLE;

                // Starts fading in if needed, or sets fade volume to its maximum
                // if we don't want to fade in
                if (fadeIn == 0.0f) {
                    mFadeVolume = 1.0f;
                    mFade = MF_NO_FADE;
                } else {
                    mFadeSpd = fadeIn;
                    mFade    = MF_FADE_IN;
                }
            }
        }
    }
    //-----------------------------------------------------------------------------
    void MusicStream::_pause(float aFadeOut)
    {
        if (mMusic != 0)
        {
            ALenum srcState;

            // Checks whether the source is playing before pausing it
            alGetSourcei(mMusicSrc,AL_SOURCE_STATE,&srcState);
            mAudioMan->_alErrorCheck("MusicStream::_pause()",
                    "Failed getting music source state");

            if (srcState == AL_PLAYING)
            {
                float fadeOut = Math::clamp(aFadeOut,0.0f,1.0f);
                if (fadeOut == 0.0f) {
                    alSourcePause(mMusicSrc);
                    mAudioMan->_alErrorCheck("MusicStream::_pause()",
                            "Failed pausing music source");
                } else {
                    mState   = MSS_PAUSING;
                    mFadeSpd = fadeOut;
                    mFade    = MF_FADE_OUT;
                }
            }
        }
    }
    //-----------------------------------------------------------------------------
    bool MusicStream::isPaused()
    {
        if (mMusic != 0)
        {
            ALenum srcState;

            alGetSourcei(mMusicSrc,AL_SOURCE_STATE,&srcState);
            mAudioMan->_alErrorCheck("MusicStream::isPaused()",
                    "Failed getting music source state");

            if (srcState == AL_PAUSED)
            {
                return true;
            }
        }

        return false;
    }
    //-----------------------------------------------------------------------------
    bool MusicStream::isStopped()
    {
        if (mMusic != 0)
        {
            ALenum srcState;

            alGetSourcei(mMusicSrc,AL_SOURCE_STATE,&srcState);
            mAudioMan->_alErrorCheck("MusicStream::isStopped()",
                    "Failed getting music source state");

            if (srcState != AL_STOPPED)
            {
                return false;
            }
        }

        return true;
    }
    //-----------------------------------------------------------------------------
    size_t MusicStream::_getCurrentPos()
    {
        // <todo> This method is not working with OGG files smaller than
        // BUFFER_SIZE (fix it)
        if (mMusic != 0)
        {
            int alPos,buffers;
            size_t streamPos     = static_cast<size_t>(ov_pcm_tell(&mFile));
            size_t loopPoint     = Kernel::get()->mDatabase->
                                   mMusicList[mMusic-1].loopPoint;
            size_t sizeInSamples = BUFFER_SIZE/2;
            size_t offset        = 666666666;

            if ((int)streamPos == OV_EINVAL)
            {
                SONETTO_THROW("Failed telling current stream offset in music file ("+
                        Ogre::StringConverter::toString(streamPos)+")");
            }

            if (sizeInSamples > mStreamLen)
            {
                sizeInSamples = mStreamLen;
            }

            alGetSourcei(mMusicSrc,AL_SAMPLE_OFFSET,&alPos);
            mAudioMan->_alErrorCheck("MusicStream::_getCurrentPos()",
                    "Failed getting music source sample offset");

            alGetSourcei(mMusicSrc,AL_BUFFERS_PROCESSED,&buffers);
            mAudioMan->_alErrorCheck("MusicStream::_getCurrentPos()",
                    "Failed getting number of processed buffers in music source");

            alPos -= buffers*sizeInSamples;
            if (streamPos < loopPoint || streamPos-loopPoint > sizeInSamples) {
                offset = streamPos-(sizeInSamples-alPos);
            } else {
                size_t warped   = streamPos-loopPoint;
                size_t unwarped = sizeInSamples-warped;

                if ((size_t)alPos >= unwarped) {
                    offset = streamPos-(sizeInSamples-alPos);
                } else {
                    offset = mStreamLen-(sizeInSamples-alPos);
                }
            }

            return offset;
        }

        return 0;
    }
    //-----------------------------------------------------------------------------
    void MusicStream::pullStream(size_t buffer)
    {
        int          bitstream;
        ALenum       format;              // Stream format (Mono/Stereo)
        char         data[BUFFER_SIZE*2]; // Temporary PCM buffer
        size_t       offset = 0;          // How much data was already read

        if (mState == MSS_ENDED)
        {
            SONETTO_THROW("Trying to stream an ended stream");
        }

        // While we haven't filled a buffer, read more data from OGG stream
        while (offset < BUFFER_SIZE)
        {
            int read;

            if (BUFFER_SIZE-offset <= 1024) {
                char chunk[1024];
                read = ov_read(&mFile,chunk,1024,0,2,1,&bitstream);

                if (read > 0)
                {
                    // Copy part of data read to `data' buffer
                    read = BUFFER_SIZE-offset;
                    memcpy(data+offset,chunk,read);
                }
            } else {
                read = ov_read(&mFile,data+offset,BUFFER_SIZE-offset,0,2,1,
                               &bitstream);
            }

            if (read > 0) { // There were bytes read
                offset += read;
            } else
            if (read == 0) { // No bytes were read
                if (mLoop) {
                    // Seeks stream to loop point
                    int errCode = ov_pcm_seek(&mFile,Kernel::get()->mDatabase->
                            mMusicList[mMusic-1].loopPoint);

                    if (errCode != 0)
                    {
                        SONETTO_THROW("Couldn't seek OGG/Vorbis stream position");
                    }

                    continue; // Continue reading from new position
                } else {
                    // Nothing more to be read
                    mState = MSS_ENDED;
                    break;
                }
            } else { // Error while reading from file
                SONETTO_THROW("Failed reading from OGG/Vorbis file ("+
                    Ogre::StringConverter::toString(read)+")");
            }
        }

        // Gets information about the stream and figures
        // whether the format is mono or stereo
        if (mFile.vi->channels == 1) {
            format = AL_FORMAT_MONO16;
        } else {
            format = AL_FORMAT_STEREO16;
        }

        // Fills an OpenAL audio data buffer with our
        // just decompressed audio data
        alBufferData(buffer,format,data,offset,mFile.vi->rate);
        mAudioMan->_alErrorCheck("MusicStream::pullStream()",
                "Could not fill OpenAL audio buffer");
    }
} // namespace

