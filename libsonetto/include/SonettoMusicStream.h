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

#ifndef SONETTO_MUSICSTREAM_H
#define SONETTO_MUSICSTREAM_H

#include <cstdlib>
#include <vorbis/vorbisfile.h>
#include "SonettoMath.h"
#include "SonettoAudioManager.h"

namespace Sonetto
{

    /// Describes a  music fade state
    enum MusicFading
    {
        /// Music fading in
        MF_FADE_IN,
        /// Music not fading
        MF_NO_FADE,
        /// Music fading out
        MF_FADE_OUT
    };

    /// Current stream state
    enum StreamState
    {
        /// Default state
        MSS_IDLE,
        /// Is set when the stream is fading out to be paused
        MSS_PAUSING,
        /// Is set when the stream is fading out to be stopped
        MSS_STOPPING,
        /// Is set when the stream is non loopable and reaches its end
        MSS_ENDED
    };

    /** MusicStream class

        Class responsible for streaming music from an OGG file. It is used
        either by BGMs and MEs. This class is mainly used by the AudioManager.
        Use AudioManager's own interface to play/stop/pause/resume/etc your
        musics.
    */
    class SONETTO_API MusicStream
    {
    public:
        /** Protected constructor, only used by the AudioManager

        @param
            audioMan AudioManager pointer. Needed because when this
            MusicStream is created, the singletons are not set yet.
        */
        MusicStream(AudioManager *audioMan);

        /// Protected destructor, only used by the AudioManager
        ~MusicStream();

        /// Size of each OpenAL audio buffer
        static const size_t BUFFER_SIZE;

        /** Sets the maximum volume

        @see
            mMaxVolume
        */
        inline void setMaxVolume(float vol) { mMaxVolume = Math::clamp(vol,0.0f,1.0f); }

        /// Checks whether this music is paused or not
        bool isPaused();

        /// Checks whether this music is stopped or not
        bool isStopped();

        /** Gets current music ID

        @return
            Music ID or 0 when no music is set to stream.
        */
        inline size_t getCurrentMusic() const { return mMusic; }

        /// Returns current stream position (in PCM samples)
        size_t _getCurrentPos();

        /// Gets whether this stream loops or not
        inline bool _getLoop() const { return mLoop; }

        /** Gets current stream state

        @see
            mState
        */
        inline StreamState _getState() const { return mState; }

        inline MusicFading getFadingState() const { return mFade; }

        /// Updates music stream
        void _update();

        /** Plays the music

            Plays the music fading it in. If `fadeIn' is 0.0f, the music
            is played at its maximum volume as soon it starts playing.
        @param
            id The music ID to be played (0 is invalid).
        @param
            pos Seek music stream to here before playing (value in PCM samples)
        @param
            fadeIn Fade speed in which the music will fade in before reaching
            its maximum volume.
        @param
            loop Whether to loop the music or not.
        */
        void _play(size_t id,size_t pos,float fadeIn,bool loop);

        /** Stops the music

            Fades out the music and then stops it. If `aFadeOut' is 0.0f,
            the music is stopped without fading.
        @param
            aFadeOut Fade speed in which the music will fade out before being
            stopped.
        */
        void _stop(float aFadeOut);

        /** Resumes the music

            Unpauses the music and fades it in. If `aFadeIn' is 0.0f, the music
            is played at its maximum volume as soon it starts playing.
        @param
            aFadeIn Fade speed in which the music will fade in before reaching
            its maximum volume.
        */
        void _resume(float aFadeIn);

        /** Pauses the music

            Fades out the music and then pauses it. If `aFadeOut' is 0.0f,
            the music is paused without fading.
        @param
            aFadeOut Fade speed in which the music will fade out before being
            paused.
        */
        void _pause(float aFadeOut);

    private:
        /** Pulls OGG/Vorbis stream into OpenAL audio buffer

        @param
            buffer An OpenAL audio buffer handle
        */
        void pullStream(size_t buffer);

        /// AudioManager singleton pointer (for ease of use)
        AudioManager *mAudioMan;

        /// Music ID
        size_t mMusic;

        /** OpenAL's audio buffers

            OpenAL has audio sources and buffers. Buffers hold only data, whilst
            sources have 3D positioning, and etc. Buffers can be attached to sources
            so that the data held by them can be played. We use two buffers so that
            while one is playing, the other is filled with more data from the OGG file
            (streamed).
        */
        size_t mMusicBuf[2];

        /// OpenAL's audio source
        size_t mMusicSrc;

        /// Maximum volume this music can reach
        float mMaxVolume;

        /// Fade volume multiplier
        float mFadeVolume;

        /// Whether this stream should loop or not
        bool mLoop;

        /// Fading state of the music (fading in/out or not fading)
        MusicFading mFade;

        /// Fade speed used when fading in or out
        float mFadeSpd;

        /// Stream state
        StreamState mState;

        /// OGG Vorbis file handle
        OggVorbis_File mFile;

        /// Total length of OGG/Vorbis stream (in PCM samples)
        size_t mStreamLen;
    };
} // namespace

#endif  // SONETTO_MUSICSTREAM_H
