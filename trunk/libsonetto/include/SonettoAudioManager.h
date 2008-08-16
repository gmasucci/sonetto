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

#include "SonettoMain.h"

#include <vector>
#include <queue>
#include <OGRE/Ogre.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <vorbis/vorbisfile.h>
#include "SonettoVirtualFile.h"

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
    struct SONETTO_EXPORT  SoundInfo {
        /// @brief Sound buffer (PCM Data to be used by SoundSources)
        ALuint       bufferID;

        /// @brief Whether to use loop or not
        bool         loopEnabled;

        /// @brief PCM Sample where the sound begins
//        ogg_uint64_t loopBegin;

        /// @brief PCM Sample where the sound ends
//        ogg_uint64_t loopEnd;

        /// @brief Minimal volume gain (when far from the listener)
        float        minGain;

        /// @brief Maximal volume gain (when near to the listener)
        float        maxGain;

        /// @brief Volume gain
        float        gain;

        /// @brief Rolloff factor (distance at which the sound won't be heard by the listener)
        float        rolloffFactor;
    };

    /** @brief Definitions about *a* sound source
     *
     *  @see SoundInfo
     */
    struct SONETTO_EXPORT  SoundSource {
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
    struct SONETTO_EXPORT MusicInfo {
        /// @brief Filename of the music (relative to music/)
        std::string  filename;

        /// @brief Whether to use loop or not
        bool         loopEnabled;

        /// @brief PCM Sample where the sound begins
//        ogg_uint64_t loopBegin;
//
        /// @brief PCM Sample where the sound ends
//        ogg_uint64_t loopEnd;
    };

    /** @brief Definitions about *a* music stream
     *
     *  @see MusicInfo
     */
    struct SONETTO_EXPORT MusicStream {
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

        /// @brief The maximum volume the music will reach when it finishes
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

        /** @brief Whether to memorize music on fade out
         *
         *  This is used by AudioManager::update() to determine whether
         *  to memorize this music when it finally fades out or not.
         */
        bool           fadeOutMem;

        /** @brief Whether to memorize music position on fade out
         *
         *  @see fadeOutMem
         *  @remarks Ignored if fadeOutMem is false.
         */
        bool           fadeOutMemPos;
    };

    /** @brief Data struct of a memorized music
     *
     *  This struct is mostly internal to the AudioManager, and hardly will
     *  need to be known by someone else than an AudioManager coder. It contains
     *  information about the state of a music when it was memorized, like
     *  volume and position. This could perfecly be hardcoded in the AudioManager,
     *  but we though using a struct would be cleaner.
     *  @see AudioManager::memorizeMusic()
     *  @see AudioManager::restoreMusic()
     */
    struct SONETTO_EXPORT  MusicMemory {
        /// @brief An index to a MusicInfo within AudioManager::mMusics
        size_t      musicID;

        /// @brief An OpenAL audio source state (AL_PLAYING or AL_PAUSED)
        ALenum      state;

        /// @brief The maximum volume the audio source can go (after fading in)
        float       maxVolume;

        /// @brief PCM position of the music stream (measured in PCM samples)
        ogg_int64_t pcmPos;
    };

    class SONETTO_EXPORT AudioManager {
    public:
        /// @brief Initialises members
        AudioManager() : mInitialised(false), mpDevice(NULL),
                mpContext(NULL), mUseEffectsExt(true), mMemorizedMusic(false) {};

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

        /** @brief Add sound information
         *
         *  This method creates a SoundInfo based on the arguments passed, and inserts
         *  it into the mSounds vector. As a SoundInfo contains a buffer, and thus needs
         *  to fill it somehow, this method will load the specified file to memory. With
         *  such loading comes the possibility of errors popping: addSound() can throw
         *  exceptions if the file was not found, is corrupted, is not a valid OGG Vorbis
         *  file or has no audio data.
         *  @param  filename OGG Vorbis file to load audio data from.
         *  @see    For the other arguments explaination, see the SoundInfo fields.
         *  @return Sound ID (index inside mSounds).
         */
        size_t addSound(std::string filename,float minGain,float maxGain,float gain,
                        float rolloffFactor,bool loopEnabled = false,ogg_int64_t loopBegin = 0,
                        ogg_int64_t loopEnd = 0);

        size_t addSound(VirtualFile *file,float minGain,float maxGain,float gain,
                        float rolloffFactor,bool loopEnabled = false,ogg_int64_t loopBegin = 0,
                        ogg_int64_t loopEnd = 0);

        /** @brief Add music information
         *
         *  This method creates a MusicInfo based on the arguments passed, and inserts
         *  it into the mMusics vector.
         *  @param  filename    OGG Vorbis file from where to load audio data.
         *  @param  loopEnabled Whether the music sources derived from this should loop or not.
         *  @param  loopBegin   PCM sample where the loop begins at. Defaults to zero (beginning
         *                     of the stream).
         *  @param  loopEnd     PCM sample where the loop ends at. Defaults to zero (end of the
         *                     stream).
         *  @return Music ID (index inside mMusics).
         */
        size_t addMusic(std::string filename,bool loopEnabled = false,ogg_int64_t loopBegin = 0,
                        ogg_int64_t loopEnd = 0);

        /** @brief Play a music
         *
         *  This function will open an OGG file specified in AudioManager::mMusics[musicID].
         *  A fadeOutSpd greater than 0.0f will cause the music currently being played to be
         *  faded with supplied speed until its volume reaches 0.0f. When that happen, the
         *  new music will be played. If fadeOutSpd is 0.0f, it will be automatically stopped.
         *  A fadeInSpd greater than 0.0f will cause the new music's volume to grow from 0.0f up to
         *  supplied musicVolume. If fadeInSpd is 0.0f, the music will get played with a volume of
         *  musicVolume from its play time's bare beginning.
         *  @param fadeOutSpd    Defaults to 0.0f (No fading).
         *  @param fadeInSpd     Defaults to 0.0f (No fading).
         *  @param musicVolume   Defaults to 1.0f (Maximum volume).
         *  @param pcmPos        A position for the music to start from, measured in PCM samples.
         *                       Defaults to zero (first PCM sample).
         *  @param fadeOutMem    Whether to memorize the current music when it fades out. Defaults to false.
         *  @param fadeOutMemPos Whether to memorize the current music position when it fades out.
         *                       This argument is ignored if fadeOutMem is false. Defaults to false.
         */
        void playMusic(size_t musicID, float fadeOutSpd = 0.0f, float fadeInSpd = 0.0f,
                       float musicVolume = 1.0f, /*ogg_uint64_t pcmPos = 0,*/ bool fadeOutMem = false,
                       bool fadeOutMemPos = false);

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

        /** @brief Memorize current music
         *
         *  Memorizes a music to be restored with AudioManager::restoreMusic().
         *  This method will only actually memorize something if the current music
         *  is fading in or not fading at all, and if its audio source is currently
         *  AL_PLAYING or AL_STOPPED. If such conditions are not true, the function
         *  will fail.
         *  @remarks Beware that a playMusic() call with a fadeOutMem argument set
         *           to true will overwrite any memorized musics.
         *  @return  Whether the music was successfuly memorized or not.
         */
        bool memorizeMusic(bool memorizePos);

        /** @brief Restore memorized music
         *
         *  Restores a music memorized with AudioManager::memorizeMusic().
         *  @param  fadeOutSpd Speed at which the current music will fade out. 0.0f
         *          will cause it not to fade. 1.0f is the maximum speed. Defaults to
         *          0.0f.
         *  @param  fadeInSpd  Speed at which the restored music will fade in. 0.0f
         *          will cause it not to fade. 1.0f is the maximum speed. Defaults to
         *          0.0f.
         *  @return Whether a music was restored or not.
         */
        bool restoreMusic(float fadeOutSpd = 0.0f,float fadeInSpd = 0.0f);

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
         *  @param  useEnvEffect Whether to use environmental sound effects or not (Defaults to true).
         *  @return Sound source handle, or -1 on failure. Used to reference the new sound
         *          source on other function calls.
         */
        int playSound(size_t soundID,Ogre::SceneNode *parentNode = NULL,bool useEnvEffect = true);

        /// @brief Check for existance of a given sound source
        bool sourceExists(size_t sourceID);

        /** @brief Set the Ogre::SceneNode a sound source follows
         *
         *  @remarks The coordinates of the sound source will only be the same ones of
         *           the SceneNode after a call to AudioManager::update().
         */
        void setSourceNode(size_t sourceID,Ogre::SceneNode *node);

        /** @brief Set the minimal and maximal volume gain of a sound source
         *
         *  The maximal and minimal gain is a gain to which the final sound source's gain
         *  is compared. This final gain is then truncated inside this range.
         *  @param minGain Allowed values: 0.0f ~ 1.0f
         *  @param maxGain Allowed values: 0.0f ~ 1.0f
         */
        void setSourceGainRange(size_t sourceID,float minGain,float maxGain);

        /** @brief Set the gain of a sound source
         *
         *  @param gain Allowed values: 0.0f ~ Any
         */
        void setSourceGain(size_t sourceID,float gain);

        void setSourceRolloff(size_t sourceID,float factor);

        /** @brief Set the gain of a sound source's lowpass filter
         *
         *  @param gain Allowed values: 0.0f ~ 1.0f
         */
        void setSourceFilterGain(size_t sourceID,float gain);

        /** @brief Set the high-frequency gain of a sound source's lowpass filter
         *
         *  @param gain Allowed values: 0.0f ~ 1.0f
         */
        void setSourceFilterGainHF(size_t sourceID,float gain);

        float getSourceRolloff(size_t sourceID);

        /// @brief Update AudioManager
        bool update();

    private:
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
        ALuint      mEnvSlotID;

        /// @brief Sound information
        std::vector<SoundInfo *>            mSounds;

        /// @brief Sound sources
        std::map   <size_t,SoundSource   *> mSoundSources;

        /// @brief Music information
        std::vector<MusicInfo     *>        mMusics;

        /// @brief Music stream queue
        std::queue <MusicStream   *>        mStreamQueue;

        /// @brief Whether we have or not a memorized music
        bool        mMemorizedMusic;

        /// @brief Memorized music data
        MusicMemory mMusicMem;
    };
} // namespace

#endif
