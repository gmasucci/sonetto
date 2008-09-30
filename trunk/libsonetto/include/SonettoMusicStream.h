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

#ifndef SONETTO_MUSICSTREAM_H
#define SONETTO_MUSICSTREAM_H

#include <cstdlib>
#include <vorbis/vorbisfile.h>
#include "SonettoMain.h"

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

    /// Describes a music to be played, either as a BGM or an ME
    struct SONETTO_EXPORT Music
    {
        /// Subdirectory in which the musics are saved
        static const char *FOLDER;

        /// Constructor
        Music(std::string aFilename,size_t aLoopPoint)
                : filename(aFilename), loopPoint(aLoopPoint) {}

        /// Filename of this music inside FOLDER
        std::string filename;

        /// Loop point in PCM samples to where this music should loop
        /// when it reaches the end
        size_t loopPoint;
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
    class SONETTO_EXPORT MusicStream
    {
        friend class AudioManager;
    public:
        /// Used in mMusic to represent there is no music to be played
        static const size_t NONE = static_cast<size_t>(-1);

        /// Size of each OpenAL audio buffer
        static const size_t BUFFER_SIZE;

        // <todo> Should we clamp this value?
        /** Sets the maximum volume

        @see
            mMaxVolume
        */
        inline void setMaxVolume(float vol) { mMaxVolume = vol; }

        /// Checks whether this music is paused or not
        bool isPaused();

        /// Checks whether this music is stopped or not
        bool isStopped();

        /** Gets current music ID

        @return
            Music ID (index inside AudioManager::mMusics) or MusicStream::NONE
            when no music is set to stream.
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
        inline const StreamState &_getState() const { return mState; }

        /// Updates music stream
        void _update(float deltatime);

        /** Plays the music

            Plays the music fading it in. If `fadeIn' is 0.0f, the music
            is played at its maximum volume as soon it starts playing.
        @param
            id The music index inside AudioManager::mMusics to be played.
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

            Fades out the music and then stops it. If `fadeOut' is 0.0f,
            the music is stopped without fading.
        @param
            fadeOut Fade speed in which the music will fade out before being
            stopped.
        */
        void _stop(float fadeOut);

        /** Resumes the music

            Unpauses the music and fades it in. If `fadeIn' is 0.0f, the music
            is played at its maximum volume as soon it starts playing.
        @param
            fadeIn Fade speed in which the music will fade in before reaching
            its maximum volume.
        */
        void _resume(float fadeIn);

        /** Pauses the music

            Fades out the music and then pauses it. If `fadeOut' is 0.0f,
            the music is paused without fading.
        @param
            fadeOut Fade speed in which the music will fade out before being
            paused.
        */
        void _pause(float fadeOut);

    private:
        /** Protected constructor, only used by the AudioManager

        @param
            audioMan AudioManager pointer. Needed because when this
            MusicStream is created, the singletons are not set yet.
        */
        MusicStream(AudioManager *audioMan);

        /// Protected destructor, only used by the AudioManager
        ~MusicStream();

        /** Pulls OGG/Vorbis stream into OpenAL audio buffer

        @param
            buffer An OpenAL audio buffer handle
        */
        void pullStream(size_t buffer);

        /// AudioManager singleton pointer (for ease of use)
        AudioManager *mAudioMan;

        /// Music index inside AudioManager::mMusics
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
