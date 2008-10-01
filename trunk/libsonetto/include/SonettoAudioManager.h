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
#include <string>
#include <AL/alc.h>
#include <OgreSharedPtr.h>
#include "SonettoMusicStream.h"
#include "SonettoSoundSource.h"

namespace Sonetto
{
    /// Vector of Music structures
    typedef std::vector<Music> MusicVector;

    /// Vector of SoundDef structures
    typedef std::vector<SoundDef> SoundDefVector;

    /// Map of Sound structures
    typedef std::map<size_t,Sound> SoundMap;

    /// Shared pointer to a SoundSource class
    typedef Ogre::SharedPtr<SoundSource> SoundSourcePtr;

    /// Vector of SoundSourcePtrs
    typedef std::vector<SoundSourcePtr> SoundSourceVector;

    /** AudioManager class

        This class is resposible for maintaining audio libraries and
        for providing a completely abstract interface to play musics
        (in the form of Background Musics and Music Effects) and sound
        effects. It only works with OGG compressed audio files.
    */
    class SONETTO_EXPORT AudioManager
    {
        friend class Kernel;
        friend class MusicStream;
    public:
        /// Used in mNextBGM and mNextME to represent there is no music to be played
        static const size_t NONE = static_cast<size_t>(-1);

        /// Updates
        void _update(float deltatime);

        /// @see mInitialised
        inline bool isInitialised() const { return mInitialised; }

        // <todo> Remember to protect MusicStream (copy constructor, etc)
        /// Returns a reference to the music stream
        inline MusicStream *getMusicStream() { return mMusicStream; }

        /// Returns the list of musics
        inline const MusicVector &_getMusics() const { return mMusics; }

        /// Returns the map of loaded sounds
        inline const Sound &_getSound(size_t id) const { return mSounds.find(id)->second; }

        /** Plays a background music

            This method has three case scenarios. It can be called when there
            is no music playing at all. In this case, it will start playing the
            desired music as soon as the method gets called. If there is already
            a BGM playing, it will be set to fade out at the desired speed
            (`fadeOut'), and only after that it will play this music (see fadeEnded()).
            If there is an ME currently playing, this method silently overwrites
            the saved data of the old BGM queued to play after the ME with this new
            music's information. This way, this music will only play when the ME ends
            streaming (see streamEnded()). In any case, when this music finally starts
            to be played, it will start at its maximum volume if `fadeIn' is set to 0.0f,
            or it will start from mute and fade in at the desired speed, until it reaches
            its maximum volume if not. Fade values must never be negative.
        @param
            id The music index inside mMusics to be played as a BGM.
        @param
            fadeOut The fade value in which the current BGM will fade out before starting
            the new BGM.
        @param
            fadeIn The fade value in which this new BGM will fade in.
        */
        void playBGM(size_t id,float fadeOut,float fadeIn);

        /** Plays a music effect

            The MEs can have three case scenarios. The first one is when no music is playing
            at all, in which case it just streams the music until its end and stop. In this
            case, there is no BGM to faded in or out, so the `fadeOut' and `fadeIn' variables
            are unused. The second case happens when there is a BGM currently playing. In this
            case, the BGM fades out (`fadeOut'), the ME is played once, and the old BGM is
            recovered from the position it stopped and fades in (`fadeIn'). If `fadeOut' is set
            to 0.0f, this method starts playing the ME as soon as it gets called. The third
            case scenario occurs when there already is an ME playing, in which case this method
            will replace the old ME without any fading at all. The supplied `fadeIn' argument
            will overwrite the old one too, and the old BGM will be recovered with the fade in
            speed given to this method.
        @param
            id The music index inside mMusics to be played as an ME.
        @param
            fadeOut The fade speed in which the current BGM will fade out before starting
            this ME.
        @param
            fadeIn The fade speed in which the old BGM will fade in when this ME ends.
        */
        void playME(size_t id,float fadeOut,float fadeIn);

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
            a music index in mNextBGM. If such variable has a value different from
            AudioManager::NONE, then there is a BGM to be played, and this method plays
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
            mNextME != AudioManager::NONE). If there is, it saves the current BGM
            state in the mNextBGM* variables and starts playing the ME. If not, the method
            then checks whether mNextBGM != AudioManager::NONE, what would mean there is a
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

            This method will get the sound information on mSoundDefs[id] and
            load it into mSounds[id]. After that, you can play the sound with
            the desired methods, either by creating it with createSound() or by
            playing it with playSound().
        @param
            id An index inside mSoundDefs to be loaded.
        */
        void loadSound(size_t id);

        SoundSourcePtr createSound(size_t id);

        void playSound(size_t id /*,Ogre::SceneNode*/);

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
        /** Private constructor, only accessible by the Kernel

        @param
            device Desired device name to be used (NULL is default).
        */
        AudioManager(const char *device = NULL);

        /// Private destructor, only accessible by the Kernel
        ~AudioManager();

        /// Whether the AudioManager has initialised correctly or not
        bool mInitialised;

        /// An open OpenAL audio device
        ALCdevice *mDevice;

        // Music
        /// Class responsible for streaming music for the AudioManager
        MusicStream *mMusicStream;

        /// AudioManager::NONE or the index of the next BGM to be played when the current one stops fading out
        size_t mNextBGM;

        /// Fade speed used to fade in the next BGM when needed, if mNextBGM != AudioManager::NONE
        float mNextBGMFade;

        /// Position from which the next BGM will start from
        size_t mNextBGMPos;

        /// AudioManager::NONE or the index of the next ME to be played when the current BGM stops fading out
        size_t mNextME;

        /** Vector of Music structures

            This is a vector of Music structures. The indexes of the Musics
            inside this vector represent them in playBGM() and playME().
        @see
            Music
        */
        MusicVector mMusics;

        // Sounds
        /** Vector of SoundDef structures

            This is a vector of SoundDef structures. The indexes of the SoundDefs
            inside this vector represent them in methods like loadSound, createSound()
            and playSound().
        @see
            SoundDef
        */
        SoundDefVector mSoundDefs;

        /** Map of Sound structures

            This is the map of currently loaded sounds. Their indexes match with mSoundDefs,
            but not always all mSoundDefs indexes exist in this map. Sounds are kept loaded
            into memory, and are generally only loaded or discarded on game map or module
            changes. The indexes not present in this map mean the corresponding sound in
            mSoundDefs is not loaded.
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
    };
} // namespace

#endif  // SONETTO_AUDIOMANAGER_H
