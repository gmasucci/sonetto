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
        SoundSource(size_t id,Ogre::Node *node);
        ~SoundSource();

        void setMaxVolume(float maxVolume);
        inline float getMaxVolume() const { return mMaxVolume; }

        SoundSourceState getState() const;

        inline size_t getSoundID() const { return mSoundID; }

        inline bool isValid() const { return (mSoundID > 0); }

        void _invalidate();

        void _update();

        void play();
        void pause();
        void stop();

    private:
        /// A pointer to the AudioManager singleton, for ease of use
        AudioManager *mAudioMan;

        /// Sound ID inside AudioManager::mSounds
        size_t mSoundID;

        /// An OpenAL audio source handle
        size_t mALSource;

        /// The maximum volume this source will reach
        float mMaxVolume;

        /// The Ogre::Node onto which this sound source is attached
        Ogre::Node *mNode;
    };

    /// Vector of SoundDef structures
    typedef std::vector<SoundDef> SoundDefVector;

    /// Map of Sound structures
    typedef std::map<size_t,Sound> SoundMap;

    /// Shared pointer to a SoundSource class
    typedef Ogre::SharedPtr<SoundSource> SoundSourcePtr;

    /// Vector of SoundSourcePtrs
    typedef std::vector<SoundSourcePtr> SoundSourceVector;
} // namespace

#endif // SONETTO_SOUNDSOURCE_H
