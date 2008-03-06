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

#ifndef __SONETTO_AUDIOMANAGER_H_
#define __SONETTO_AUDIOMANAGER_H_

#include <vector>
#include <queue>
#include <al.h>
#include <alc.h>
#include <vorbis/vorbisfile.h>
#include "SonettoMain.h"

namespace Sonetto {
    class AudioManager {
        public:
            AudioManager() : mInitialised(false), mDevice(0), mContext(0), mMemorized(false) {}
            ~AudioManager() {}

            /** @brief Attempt to initialise AudioManager
             *
             *  @remarks Sonetto will run without an AudioManager. But if the initialisation fails,
             *           make sure no calls to other methods are made. Otherwise, nightmares may
             *           become reality.
             *  @return  Whether initialisation occurred successfuly.
             */
            bool initialise();

            /// @brief Clean environment.
            bool deinitialise();

            /** @brief Play a music
             *
             *  This function will open an OGG file specified in mMusics[musicID].
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
             *                     Defaults to 0(first PCM sample).
             */
            void playMusic(size_t musicID, float fadeOutSpd = 0.0f, float fadeInSpd = 0.0f, float musicVolume = 1.0f, ogg_uint64_t pcmPos = 0);

            /** @brief Stop the actual music or all musics
             *
             *  This will stop the actual music of being played. If fadeSpd is different than 0.0f,
             *  the actual music will fade out at that speed. If there was another music queued for
             *  playing(ie. if you called playMusic twice, and on the second time you set a fade out
             *  value), stopMusic() will "forget" about it if you set allMusics to true. Elsewhere
             *  stopMusic() will make that queued music to be played.
             *  @param fadeSpd   Defaults to 0.0f (No fading).
             *  @param allMusics Defaults to true (Stop queued musics too).
             */
            void stopMusic(float fadeSpd = 0.0f,bool allMusics = true);

            /** @brief Play the music once, without loop, and return to the previous music
             *
             *  @remarks If you're already playing one music using this function,
             *           you can't play another while this one still playing.
             */
            void playMusicOver(size_t musicID, float fadeOutSpd = 1.0f, float fadeInSpd = 0.0f, float musicVolume = 1.0f);

            /// Memorize current music
            void memorizeMusic();
            /// Restore memorized music
            void restoreMusic(float fadeOutSpd = 0.0f,float fadeInSpd = 0.0f);
            /// Pause current music
            void pauseMusic();
            /// Resume paused music
            void resumeMusic();

            /** @brief Creates and plays a sound source
             *
             *  Creates a sound source and feeds it with data from mSounds[soundID]->bufferID.
             *  Its coordinates will be took from supplied parentNode.
             *  @param  soundID    Sound index in AudioManager::mSounds.
             *  @param  parentNode Ogre::SceneNode to which this sound source will belong to.
             *  @return Sound source handle, or -1 on failure. Used to reference the new sound
             *          source on other function calls.
             */
            int playSound(size_t soundID,Ogre::SceneNode *parentNode);

            /// Updates AudioManager
            bool update();

        private:
            bool        mInitialised;

            ALCdevice  *mDevice;
            ALCcontext *mContext;

            std::vector<SoundInfo   *> mSounds;
            std::vector<SoundSource *> mSoundSources;
            std::vector<MusicInfo   *> mMusics;
            std::queue <MusicStream *> mStreamQueue;

            // Memorized Music Data
            bool        mMemorized;
            size_t      mMemMusicID;
            float       mMemVolume;
            ogg_int64_t mMemPosition;

            bool pullStream(MusicStream *stream,ALuint buffer);

            void throwALError(const char *errMsg,const char *source);
    };
}; // namespace

#endif
