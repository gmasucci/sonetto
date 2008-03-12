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

#ifndef SONETTO_AUDIOMANAGER_H
#define SONETTO_AUDIOMANAGER_H

#include <vector>
#include <queue>
#include <Ogre.h>
#include <al.h>
#include <alc.h>
#include <efx.h>
#include <vorbis/vorbisfile.h>

namespace Sonetto {
    /** @brief Definitions about *the* sound
     *
     *  Fixed sound properties loaded from the database.
     *  This effectively contains the sound to be played. The SoundSource
     *  is just a player which plays the sound of a SoundInfo with some
     *  added exclusive properties that only affects itself. SoundInfo will
     *  only care about the most top-level properties of a sound(loop points, etc),
     *  while SoundSource will care about lower-level properties(filter passes, etc).
     *  @remarks These properties cannot be changed on-the-fly.
     */
    struct SoundInfo {
        /// @brief Sound buffer (PCM Data to be used by SoundSources)
        ALuint       bufferID;

        /// @brief Whether to use loop or not
        bool         loopEnabled;

        /// @brief PCM Sample where the sound begins
        ogg_uint64_t loopBegin;

        /// @brief PCM Sample where the sound ends
        ogg_uint64_t loopEnd;

        /// @brief Volume
        float        volume;

        /** @brief Fade direction
         *
         *  fadeDir <  0 for fade out
         *  fadeDir == 0 for no fade
         *  fadeDir  > 0 for fade in
         */
        char         fadeDir;

        /** @brief Fade speed
         *
         *  @remarks A fade speed greater than 1.0f will cause
         *           an instantaneous fade. A fade speed lower
         *           than 0.0f is unexpected behaviour.
         */
        float        fadeSpd;
    };

    /** @brief Definitions about *a* sound source
     *
     *  @see SoundInfo
     */
    struct SoundSource {
        /// @brief Index of a SoundInfo in AudioManager::mSounds
        ALuint           soundID;

        /// @brief An OpenAL source ID
        ALuint           sourceID;

        /// @brief An OpenAL filter ID
        ALuint           filterID;

        /// @brief Whether will this source be affected by the
        ///        environment auxiliary effect slot
        bool             enableEffects;

        /// @brief SceneNode which's coordinates will be mimicked,
        ///        or NULL for camera following
        Ogre::SceneNode *parentNode;
    };

    /** @brief Definitions about *the* music
     *
     *  Just as SoundInfo, MusicInfo is also top-level information. But in this case
     *  about music. The main difference is that musics are streamed, and so its "player"
     *  (the MusicStream) needs to keep pulling data from the audio file while playing.
     *  Those streaming informations aren't held by MusicInfo because __theoretically__,
     *  there could exist more than a MusicStream streaming(playing) the same music.
     *  Althought in Sonetto one cannot play two musics at once, there isn't a real
     *  limitation on how many musics may be playing, not taking into account hardware
     *  limitations. We opted not to allow two or more musics at once in Sonetto because, firtsly,
     *  it doesn't really makes sense, if you think about it. And secondly, because that
     *  would mess the fading queue.
     *
     *  @remarks These properties cannot be changed on-the-fly.
     */
    struct MusicInfo {
        /// @brief Filename of the music (relative to music/)
        std::string  filename;

        /// @brief Whether to use loop or not
        bool         loopEnabled;

        /// @brief PCM Sample where the sound begins
        ogg_uint64_t loopBegin;

        /// @brief PCM Sample where the sound ends
        ogg_uint64_t loopEnd;
    };

    /** @brief Definitions about *a* music stream
     *
     *  @see MusicInfo
     */
    struct MusicStream {
        /// @brief Index of a MusicInfo in AudioManager::mMusics
        size_t         musicID;

        /// @brief An OpenAL source ID
        ALuint         sourceID;

        /// @brief OpenAL front and back buffer IDs
        ALuint         bufferIDs[2];

        /// @brief An open OGG Vorbis stream
        OggVorbis_File oggStream;

        /// @brief OGG Vorbis bitstream
        int            bitstream;

        /// @brief The maximum volume the music will reach when it finished
        ///        fading in
        float          maxVolume;

        /** @brief Fade direction
         *
         *  fadeDir <  0 for fade out
         *  fadeDir == 0 for no fade
         *  fadeDir  > 0 for fade in
         */
        char           fadeDir;

        /** @brief Fade speed
         *
         *  @remarks A fade speed greater than 1.0f will cause
         *           an instantaneous fade. A fade speed lower
         *           than 0.0f is unexpected behaviour.
         */
        float          fadeSpd;
    };

    class AudioManager {
    public:
        /// @brief Initialises members
        AudioManager() : mInitialised(false), mpDevice(NULL),
                mpContext(NULL), mUseEffectsExt(true) {};

        ~AudioManager() {}

        /** @brief Attempt to initialise AudioManager
         *
         *  @remarks Sonetto will run without an AudioManager if there isn't essential
         *           hardware support or if some odd bug happen. But if the initialisation fails,
         *           make sure no calls to other methods are made, as this will cause exceptions.
         *  @param   device Audio device name to use. Defaults to NULL (use default device).
         *  @return  Whether initialisation occurred successfuly.
         */
        bool initialise(const char *device = NULL);

        /// @brief Clean environment
        bool deinitialise();

        /** @brief Play a music
         *
         *  This function will open an OGG file specified in AudioManager::mMusics[musicID].
         *  A fadeOutSpd greater than 0.0f will cause the music currently being played to be
         *  faded with supplied speed until its volume reaches 0.0f. When that happen, the
         *  new music will be played. If fadeOutSpd is 0.0f, it will be automatically stopped.
         *  A fadeInSpd greater than 0.0f will cause the new music's volume to grow from 0.0f up to
         *  supplied musicVolume. If fadeInSpd is 0.0f, the music will get played with a volume of
         *  musicVolume from its play time's bare beginning.
         *  @param fadeOutSpd  Defaults to 0.0f (No fading).
         *  @param fadeInSpd   Defaults to 0.0f (No fading).
         *  @param musicVolume Defaults to 1.0f (Maximum volume).
         *  @param pcmPos      A position for the music to start from, measured in PCM samples.
         *                     Defaults to zero (first PCM sample).
         */
        void playMusic(size_t musicID, float fadeOutSpd = 0.0f, float fadeInSpd = 0.0f, float musicVolume = 1.0f, ogg_uint64_t pcmPos = 0);

        /** @brief Stop the actual music or all musics
         *
         *  This will stop the music being played. If fadeSpd is different than 0.0f,
         *  the music will fade out at that speed. If there was another music queued for
         *  playing(ie. if you called playMusic twice, and on the second time you set a fade out
         *  value), and if you set allMusics to true, stopMusic() will "forget" about it. Elsewhere
         *  stopMusic() will make that queued music to be played.
         *  @param fadeSpd   Defaults to 0.0f (No fading).
         *  @param allMusics Defaults to true (Stop queued musics too).
         */
        void stopMusic(float fadeSpd = 0.0f,bool allMusics = true);

        /// @brief Memorize current music
        void memorizeMusic() {} // <todo> Do

        /// @brief Restore memorized music
        void restoreMusic(float fadeOutSpd = 0.0f,float fadeInSpd = 0.0f) {} // <todo> Do

        /// @brief Pause current music
        void pauseMusic();

        /// @brief Resume paused music
        void resumeMusic();

        /** @brief Create and play a sound source
         *
         *  Creates a sound source and feeds it with data from AudioManager::mSounds[soundID]->bufferID.
         *  Its coordinates will be took from supplied parentNode.
         *  @param  soundID      Sound index in AudioManager::mSounds.
         *  @param  parentNode   Ogre::SceneNode to which this sound source will belong to. Defaults
         *                       to NULL (Camera).
         *  @param  filterType   Source's filter type (AL_FILTER_LOWPASS, etc). Defaults to
         *                       AL_FILTER_NULL.
         *  @param  useEnvEffect Whether to use environmental sound effects or not (Defaults to true).
         *  @return Sound source handle, or -1 on failure. Used to reference the new sound
         *          source on other function calls.
         */
        int playSound(size_t soundID,Ogre::SceneNode *parentNode = NULL,ALenum filterType = AL_FILTER_NULL,bool useEnvEffect = true);

        /// @brief Update AudioManager
        bool update();

    protected:
        /** @brief Pulls and decompress data from a music stream into a sound buffer
         *
         *  @remarks Throws exception on fatal errors (Out of memory, etc).
         *  @param   stream Music stream to pull data from
         *  @param   buffer OpenAL Sound Buffer to load data into
         *  @return  Whether data was read(true) or stream has ended(false).
         */
        bool pullStream(MusicStream *stream,ALuint buffer);

        /** @brief Throws an exception on OpenAL errors
         *
         *  This function checks whether OpenAL errors have occurred.
         *  If they do so, it will throw an exception with the supplied
         *  message and source information.
         *  @param errMsg Error description.
         *  @param source Source file where the error occurred.
         */
        void throwALError(const char *errMsg,const char *source);

        /// @brief Whether AudioManager::initialise() was successful or not
        bool mInitialised;

        /// @brief OpenAL audio device (Renderer)
        ALCdevice  *mpDevice;

        /// @brief OpenAL rendering context
        ALCcontext *mpContext;

        /// @brief Whether we're using effects extension or not
        bool        mUseEffectsExt;

        /// @brief OpenAL auxiliary effect slot ID for current environment
        ALuint      mEnvEffectSlot;

        /// @brief Sound information
        std::vector<SoundInfo   *> mSounds;

        /// @brief Sound sources
        std::vector<SoundSource *> mSoundSources;

        /// @brief Music information
        std::vector<MusicInfo   *> mMusics;

        /// @brief Music stream queue
        std::queue <MusicStream *> mStreamQueue;
    };
}
; // namespace

#endif
