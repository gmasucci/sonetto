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

#ifndef SONETTO_SOUNDSOURCE_H
#define SONETTO_SOUNDSOURCE_H

#include <string>

namespace Sonetto
{
    // Forward declaration
    class AudioManager;

    enum SoundSourceState
    {
        SSS_PLAYING,
        SSS_PAUSED,
        SSS_STOPPED
    };

    struct SoundDef
    {
        static const char *FOLDER;

        SoundDef(std::string aFilename) : filename(aFilename) {}

        std::string filename;
    };

    struct Sound
    {
        Sound(size_t aBuffer) : buffer(aBuffer) {}

        size_t buffer;
    };

    class SoundSource
    {
    public:
        SoundSource(size_t id /*,Ogre::SceneNode*/);
        ~SoundSource();

        SoundSourceState getState() const;

        size_t getSoundID() const { return mSoundID; }

        void play();
        void pause();
        void stop();

    private:
        /// A pointer to the AudioManager singleton, for ease of use
        AudioManager *mAudioMan;

        /// Sound ID inside AudioManager::mSounds
        size_t mSoundID;

        /// An OpenAL audio source handle
        size_t alSource;
    };
} // namespace

#endif
