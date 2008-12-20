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

#ifndef SONETTO_INPUTMANAGER_H
#define SONETTO_INPUTMANAGER_H

#include <OgreSingleton.h>
#include <SDL/SDL.h>
#include "SonettoPrerequisites.h"
#include "SonettoPlayerInput.h"


namespace Sonetto
{
    /** @brief Class responsible for managing input resources

        From this class, you can get Sonetto::PlayerInput instances, which you
        can use for retrieving input from joysticks and keyboard.
    */
    class SONETTO_API InputManager : public Ogre::Singleton<InputManager>
    {
        public:
            /** Constructor

            @param
                players Number of PlayerInputs to be created
            */
            InputManager(size_t players)
                    : mInitialized(false), mPlayerNum(players) {}

            /// Destructor
            ~InputManager();

            /// Initializes InputManager
            void initialize();

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
            static InputManager &getSingleton();

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
            static InputManager *getSingletonPtr();

            /** Updates all PlayerInputs.

                This is called by Kernel::run(), so you
                don't have to worry about it.
            */
            void update();

            /** Retrieves a PlayerInput given its index

            @remarks
                Be careful not to access an index greater than
                or equal to getPlayerNum().
            */
            PlayerInput *getPlayer(size_t num);

            /// Retrieves the number of PlayerInputs in the manager
            inline size_t getPlayerNum() const { return mPlayerNum; }

            /** Gets a joystick to be used in by a PlayerInput

            @remarks
                Must be released using _releaseJoystick(). Be careful
                not to access an index greater than getJoystickNum().
            */
            Joystick *_getJoystick(uint32 index);

            /** Releases a joystick that was being used by a PlayerInput

                PlayerInput's release their joysticks using this method so
                that the InputManager can free joysticks not being used by
                any other PlayerInput.
            */
            void _releaseJoystick(uint32 index);

            /// Returns the number of available joysticks for use
            uint16 getJoystickNum() const;

            /// Checks whether the joystick in the given index is being used by a PlayerInput or not
            bool joystickAttached(uint16 index) const;

            /** Directly checks a physical keyboard key state

                This method is intended mainly for debugging, when user configuration is something
                that should be avoided. It returns the state of a given keyboard key.
            @param
                key Key to be checked.
            @see

            */
            inline KeyState getDirectKeyState(SDLKey key) const { return mKeyboardStates[key]; }

        private:
            /** Reference counting structure for joysticks

                Keeps track of how many PlayerInput's use a given Joystick. This is used
                to free unused joysticks.
            */
            struct JoyRefCount
            {
                /// Default constructor
                JoyRefCount() : joyPtr(NULL), refCount(0) {}

                /// Quick constructor
                JoyRefCount(Joystick *aJoyPtr) : joyPtr(aJoyPtr), refCount(0) {}

                /// Joystick pointer
                Joystick *joyPtr;

                /// Number of references to joyPtr
                size_t refCount;
            };

            /// Map of JoyRefCount's
            typedef std::map<size_t,JoyRefCount> JoyRefCountMap;

            /// PlayerInput vector
            PlayerInputVector mPlayers;

            /// Reference counts of joysticks currently in use
            JoyRefCountMap mJoyRefCounts;

            /// Whether this singleton is initialized or not
            bool mInitialized;

            /// Number of player inputs
            size_t mPlayerNum;

            /** Keyboard keystates

            @see
                getDirectKeyState()
            */
            KeyState mKeyboardStates[SDLK_LAST + 1];
    };
} // namespace

#endif
