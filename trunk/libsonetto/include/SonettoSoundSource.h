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
