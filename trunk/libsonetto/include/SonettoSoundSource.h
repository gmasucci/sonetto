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
#include <OgreSharedPtr.h>

namespace Sonetto
{
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

    class SONETTO_API SoundSource
    {
    public:
        SoundSource();
        virtual ~SoundSource();

        virtual void setMaxVolume(float maxVolume);
        virtual inline float getMaxVolume() const { return mMaxVolume; }

        virtual SoundSourceState getState() const;

        virtual inline uint32 getSoundID() const { return mSoundID; }

        virtual void setSoundID(uint32 id);
        virtual void setNode(Ogre::Node *node);

        virtual void _update();

        virtual void play();
        virtual void pause();
        virtual void stop();

    protected:
        /// A pointer to the AudioManager singleton, for ease of use
        AudioManager *mAudioMan;

        /// Sound ID inside AudioManager::mSounds
        uint32 mSoundID;

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
    typedef std::map<uint32,Sound> SoundMap;

    /// Shared pointer to a SoundSource class
    typedef Ogre::SharedPtr<SoundSource> SoundSourcePtr;

    /// Vector of SoundSourcePtrs
    typedef std::vector<SoundSourcePtr> SoundSourceVector;
} // namespace

#endif // SONETTO_SOUNDSOURCE_H
