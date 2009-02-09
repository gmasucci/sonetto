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

#ifndef SONETTO_AUDIOMANAGER_H
#define SONETTO_AUDIOMANAGER_H

#include <string>
#include <AL/alc.h>
#include <OgreSingleton.h>
#include <OgreSharedPtr.h>
#include "SonettoDatabase.h"
#include "SonettoMusic.h"
#include "SonettoMusicStream.h"
#include "SonettoSoundSource.h"
#include "SonettoScriptAudioHandler.h"

namespace Sonetto
{
    /** AudioManager class

        This class is resposible for maintaining audio libraries and
        for providing a completely abstract interface to play musics
        (in the form of Background Musics and Music Effects) and sound
        effects. It only works with OGG compressed audio files.
    */
    class SONETTO_API AudioManager : public Ogre::Singleton<AudioManager>
    {
    public:
        /** Private constructor, only accessible by the Kernel

        @param
            device Desired device name to be used (NULL is default).
        */
        AudioManager()
                : mInitialised(false), mMasterMusicVolume(1.0f), mNextBGM(0),
                mNextBGMPos(0), mNextME(0), mMasterSoundVolume(1.0f),
                mListenerNode(NULL) {}

        /// Private destructor, only accessible by the Kernel
        ~AudioManager();

        void initialize(const char *device = NULL);

        /** Overrides standard Singleton retrieval

        @remarks
            Warning: These comments below were copied straight from Ogre3D code.
            Why do we do this? Well, it's because the Singleton
            implementation is in a .h file, which means it gets compiled
            into anybody who includes it. This is needed for the
            Singleton template to work, but we actually only want it
            compiled into the implementation of the class based on the
            Singleton, not all of them. If we don't change this, we get
            link errors when trying to use the Singleton-based class from
            an outside dll.
        @param
            This method just delegates to the template version anyway,
            but the implementation stays in this single compilation unit,
            preventing link errors.
        */
        static AudioManager &getSingleton();

        /** Overrides standard Singleton retrieval

        @remarks
            Warning: These comments below were copied straight from Ogre3D code.
            Why do we do this? Well, it's because the Singleton
            implementation is in a .h file, which means it gets compiled
            into anybody who includes it. This is needed for the
            Singleton template to work, but we actually only want it
            compiled into the implementation of the class based on the
            Singleton, not all of them. If we don't change this, we get
            link errors when trying to use the Singleton-based class from
            an outside dll.
        @param
            This method just delegates to the template version anyway,
            but the implementation stays in this single compilation unit,
            preventing link errors.
        */
        static AudioManager *getSingletonPtr();

        /// Updates
        void _update();

        /// @see mInitialised
        inline bool isInitialised() const { return mInitialised; }

        /** Sets the AudioManager Master Music Volume

            The Master Music Volume will ensure no musics are going to be
            louder than it. It should be user-defined, usually set by the
            configuration utility.
        @param
            volume Master Music Volume (clamped between 0.0f and 1.0f).
        */
        inline void setMasterMusicVolume(float volume)
                { mMasterMusicVolume = Math::clamp(volume,0.0f,1.0f); }

        /** Gets the AudioManager Master Music Volume

        @see
            setMasterMusicVolume()
        */
        inline float getMasterMusicVolume() const { return mMasterMusicVolume; }

        /** Sets the AudioManager Master Sound Volume

            The Master Sound Volume will ensure no sound sources are going to be
            louder than it. It should be user-defined, usually set by the
            configuration utility.
        @param
            volume Master Sound Volume (clamped between 0.0f and 1.0f).
        */
        inline void setMasterSoundVolume(float volume)
                { mMasterSoundVolume = Math::clamp(volume,0.0f,1.0f); }

        /** Gets the AudioManager Master Sound Volume

        @see
            setMasterSoundVolume()
        */
        inline float getMasterSoundVolume() const { return mMasterSoundVolume; }

        /** Sets listener node

            The audio listener will inherit its coordinates from this Ogre::Node.
            The most proper node to attach the listener to is the Hero, or the
            object the player is currently controlling. It can also be set to NULL
            to use the current module's camera coordinates.
        @param
            node Node to attach listener to, or NULL to attach it to camera.
        */
        inline void setListenerNode(const Ogre::Node *node) { mListenerNode = node; }

        /// Gets listener node
        inline const Ogre::Node *getListenerNode() const { return mListenerNode; }

        /// Returns listener node world position
        Ogre::Vector3 _getListenerPos();

        // <todo> Remember to protect MusicStream (copy constructor, etc)
        /// Returns a reference to the music stream
        inline MusicStream *getMusicStream() { return mMusicStream; }

        static inline uint32 fromSoundSet(uint32 setID,uint32 soundID)
        {
            if (setID == 0 || soundID == 0)
            {
                return 0;
            }

            return Database::getSingleton().soundSets[setID - 1].getSounds()[soundID - 1];
        }

        /// Returns a sound reference from the loaded sounds map
        inline const Sound &_getSound(size_t id) const { return mSounds.find(id)->second; }

        /** Plays a background music

            This method has three case scenarios. It can be called when there
            is no music playing at all. In this case, it will start playing the
            desired music as soon as the method gets called. If there is already
            a BGM playing, it will be set to fade out at the desired speed
            (`aFadeOut'), and only after that it will play this music (see fadeEnded()).
            If there is an ME currently playing, this method silently overwrites
            the saved data of the old BGM queued to play after the ME with this new
            music's information. This way, this music will only play when the ME ends
            streaming (see streamEnded()). In any case, when this music finally starts
            to be played, it will start at its maximum volume if `aFadeIn' is set to 0.0f,
            or it will start from mute and fade in at the desired speed, until it reaches
            its maximum volume if not. Fade values must never be negative.
        @param
            id The music index inside mMusics to be played as a BGM.
        @param
            aFadeOut The fade value in which the current BGM will fade out before starting
            the new BGM (clamped between 0.0f and 1.0f).
        @param
            aFadeIn The fade value in which this new BGM will fade in (clamped between 0.0f
            and 1.0f).
        */
        void playBGM(size_t id,float aFadeOut,float aFadeIn);

        /** Plays a music effect

            The MEs can have three case scenarios. The first one is when no music is playing
            at all, in which case it just streams the music until its end and stop. In this
            case, there is no BGM to faded in or out, so the `aFadeOut' and `aFadeIn' variables
            are unused. The second case happens when there is a BGM currently playing. In this
            case, the BGM fades out (`aFadeOut'), the ME is played once, and the old BGM is
            recovered from the position it stopped and fades in (`aFadeIn'). If `aFadeOut' is set
            to 0.0f, this method starts playing the ME as soon as it gets called. The third
            case scenario occurs when there already is an ME playing, in which case this method
            will replace the old ME without any fading at all. The supplied `aFadeIn' argument
            will overwrite the old one too, and the old BGM will be recovered with the fade in
            speed given to this method.
        @param
            id The music index inside mMusics to be played as an ME.
        @param
            aFadeOut The fade speed in which the current BGM will fade out before starting
            this ME.
        @param
            aFadeIn The fade speed in which the old BGM will fade in when this ME ends.
        */
        void playME(size_t id,float aFadeOut,float aFadeIn);

        /** Stops current music being played

            Stops the current music, being it a BGM or an ME. The fade out argument
            only applies to BGMs, however, because MEs are never faded in or out.
        @param
            fadeOut Fade speed in which the BGM will fade out before stopping.
        */
        void stopMusic(float fadeOut);

        /** Resumes the current paused music

            Resumes the current paused music. The fade out argument only applies to
            BGMs (MEs never fade).
        @param
            fadeIn Fade speed in which the BGM will fade in when resuming.
        */
        void resumeMusic(float fadeIn);

        /** Pauses the current music

            Pauses the current music (either BGM or ME). The fade out argument only
            applies to BGMs (MEs never fade).
        @param
            fadeOut Fade speed in which the BGM will fade out before pausing.
        */
        void pauseMusic(float fadeOut);

        /** Gets called by mMusicStream when it ends streaming a music

            This method only gets called when an ME is playing, as BGMs loop forever
            and thus never stop streaming. When this method gets called, it looks for
            a music index in mNextBGM. If such variable has a value different than
            zero, then there is a BGM to be played, and this method plays
            it from the position described in mNextBGMPos, and with a fade in speed of
            mNextBGMFade.
        */
        void _streamEnded();

        /** Gets called by mMusicStream when it ends fading a music (both in or out)

            Short explaination:
            This method will try to do one of the following things when it gets
            called after a fade out that stopped the music:
            1 - Save current BGM info and play queued ME.
            2 - Play queued BGM.
            If there is no ME and no BGM queued, this method does nothing.

            Long explaination:
            When this method gets called, `fade' equals to MusicFading::MF_FADE_OUT,
            and the stream state is set to StreamState::MSS_STOPPING, it means a music
            has finished its fade out cycle and the queued ones have to be played.
            First the method checks whether there is a queued ME (that is, if
            mNextME != 0). If there is, it saves the current BGM
            state in the "mNextBGM*" variables and starts playing the ME. If not, the method
            then checks whether mNextBGM != 0, what would mean there is a
            BGM waiting to be played. If there is such a queued BGM, this method plays it
            with a fade in speed of mNextBGMFade. In this case, mNextBGMPos is ignored,
            since it is only used when a BGM is interrupted by an ME and has to restart
            playing, in which case streamEnded() takes care of it.
        @param
            fade The fade state that was ended. Currently, there is nothing to be done
            when a fade in ends, but this method gets called in such case anyway (it may
            be useful later).
        */
        void _fadeEnded(MusicFading fade);

        /** Loads a sound into memory

            This method will get the sound information on Database::mSoundDefList[id-1]
            and load it into mSounds[id]. After that, you can play the sound with
            the desired methods, either by creating it with createSound() or by
            playing it with playSound(). A loaded sound can be unloaded with unloadSound().
            Sounds that remain in memory when the AudioManager is destroyed are destroyed
            too.
        @param
            id An index inside Database::mSoundDefList to be loaded (0 is invalid).
        */
        void loadSound(uint32 id);

        /** Unloads a sound from memory

            This method will delete the mSounds[id] sound entry. Any sound sources using
            this sound will be invalidated.
        @param
            id Index of a loaded sound in mSounds to be unloaded (0 is invalid).
        */
        void unloadSound(uint32 id);

        inline bool isSoundLoaded(uint32 id) const
        {
            return (mSounds.find(id) != mSounds.end());
        }

        void loadSoundSet(uint32 id);
        void unloadSoundSet(uint32 id);

        /** Creates a new sound source

            This method creates a sound source and attaches it to the given
            Ogre::Node. If `node' is set to NULL, the sound source will use
            AudioManager's listener position. It returns a reference counted
            SoundSource pointer, so when you don't need it anymore you can
            simply discard it. Sound sources with no more external references
            and that are not playing (either paused or stopped) are destroyed
            automatically.
        @param
            id Sound ID to be created (0 is invalid).
        @param
            node Node to which attach the sound source. NULL for it to follow
            AudioManager's listener node.
        @return
            Reference counted sound source pointer. Discard when not anymore
            needed.
        */
        template<class TSoundSourceImpl>
        Ogre::SharedPtr<TSoundSourceImpl> createSound()
        {
            // Creates a new TSoundSourceImpl and encapsulates it into an
            // Ogre::SharedPtr
            Ogre::SharedPtr<TSoundSourceImpl> snd(new TSoundSourceImpl());

            // Adds to list of SoundSources to be deleted when not needed anymore
            mSoundSources.push_back(snd);

            return snd;
        }

        /** Plays a sound

            This method creates a sound the same way you would using createSound(),
            but it automatically plays the sound source, and returns no reference to
            it, so that it will be played once and then deleted.
        @note
            This method will not created any sound if aMaxVolume is set to 0.0f (it
            would never be heard, anyway).
        @see
            createSound()
        @param
            id Sound ID to be created (0 is invalid).
        @param
            aMaxVolume The maximum volume the sound source will reach (i.e. when near
            to the AudioManager listener - clamped between 0.0f and 1.0f).
        @param
            node Node to which attach the sound source. NULL for it to follow
            AudioManager's listener node.
        */
        void playSound(size_t id,float aMaxVolume = 1.0f,Ogre::Node *node = NULL);

        /** Throws an exception if OpenAL reports an error

            This method will throw an Exception with information about
            from where it was thrown (`location') and the error description
            (`desc'), *if* alGetError() return something other than AL_NO_ERROR
            or alcGetError() return something other than ALC_NO_ERROR.
        @param
            location Method calling _alErrorCheck() (e.g. MusicStream::_play()).
        @param
            desc Describes the reason there could be OpenAL errors.
        */
        void _alErrorCheck(const char *location,const char *desc);

    private:
        /// Whether the AudioManager has initialised correctly or not
        bool mInitialised;

        /// An open OpenAL audio device
        ALCdevice *mDevice;

        // Music
        /// Master Music Volume
        float mMasterMusicVolume;

        /// Class responsible for streaming music for the AudioManager
        MusicStream *mMusicStream;

        /// Zero or the index of the next BGM to be played when the current one stops fading out
        size_t mNextBGM;

        /// Fade speed used to fade in the next BGM when needed, if mNextBGM != 0
        float mNextBGMFade;

        /// Position from which the next BGM will start from
        size_t mNextBGMPos;

        /// Zero or the index of the next ME to be played when the current BGM stops fading out
        size_t mNextME;

        // Sounds
        /// Master Sound Volume
        float mMasterSoundVolume;

        /** Map of Sound structures

            This is the map of currently loaded sounds. Their indexes match with
            Database::mSoundDefList, but not always all Database::mSoundDefList indexes
            exist in this map. Sounds are kept loaded into memory, and are generally
            only loaded or discarded on game map or module changes. The indexes not present
            in this map mean the corresponding sound in Database::mSoundDefList is not loaded.
        @see
            SoundSource
        */
        SoundMap mSounds;

        /** Vector of SoundSource classes

            The contained SoundSources are reference counted, and are deallocated
            automatically when there are no external references to the SoundSource
            and the SoundSource is not playing.
        */
        SoundSourceVector mSoundSources;

        /** Node from which the audio listener will inherit its coordinates

            If set to NULL, the audio listener will use Kernel's top module camera position.
        */
        const Ogre::Node *mListenerNode;

        ScriptAudioHandler mScriptAudioHandler;
    };
} // namespace

#endif  // SONETTO_AUDIOMANAGER_H
