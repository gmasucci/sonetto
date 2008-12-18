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

#ifndef SONETTO_KERNEL_H
#define SONETTO_KERNEL_H

#include <Ogre.h>
#include "SonettoPrerequisites.h"
#include "SonettoModule.h"
#include "SonettoModuleFactory.h"

namespace Sonetto
{
    /** Sonetto Kernel

        This singleton is the core of this library. When its time to run Sonetto,
        this singleton will start its main loop and will only return when the
        game end.
    */
    class SONETTO_API Kernel : public Ogre::Singleton<Kernel>
    {
    public:
        /** Describes what to do after rendering

        @see
            Sonetto::Kernel::setAction()
        */
        enum KernelAction
        {
            /// Does nothing
            KA_NONE,
            /// Changes active module
            KA_CHANGE_MODULE,
            /// Shutdowns Sonetto
            KA_SHUTDOWN
        };

        /** Describes what to do with a module

        @see
            Sonetto::Kernel::setAction()
        */
        enum ModuleAction
        {
            /// Does nothing
            MA_NONE,
            /// Deletes it and instantiates a new one to take its place
            MA_CHANGE,
            /// Instantiates a new one and halts current one
            MA_CALL,
            /// Deletes it and gives control to the previous module
            MA_RETURN
        };

        /** Constructor

        @remarks
            This instance will only be ready for use when initialise() is called.
        @param moduleFactory
            External module factory used to help the Kernel to create modules
            unknown at compile time. It will not be deleted it for you even after
            Kernel's destruction.
        @see
            Kernel::initialise().
        */
        Kernel(const ModuleFactory *moduleFactory)
                : mModuleFactory(moduleFactory),mInitialised(false) {}

        /** Destructor

            If initialise() was called before this destructor, it will
            deinitialise Sonetto and the other libraries used during this
            singleton's lifetime.
        @see
            Kernel::initialise()
        */
        ~Kernel();

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
        static Kernel &getSingleton();

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
        static Kernel *getSingletonPtr();

        /** Initialises the Kernel

            This is where Sonetto and all libraries it uses are set up. After a
            call to this method, the Kernel is ready to start its main loop. See
            run().
        @see
            Kernel::run().
        */
        void initialise();

        /** Starts running the game

            This method is where the main loop of the game lives. It will only
            return when the game end. The program can then return zero, meaning
            everything went smoothly. If any fatal error occurs, Sonetto will
            throw an exception.
        */
        void run();

        /** Gets currently active game module

        @see
            Sonetto::Module
        */
        inline Module *getActiveModule() { return mModuleStack.top(); }

        /** Sets kernel action to be done after rendering

            This method can be used for two things: to shutdown Sonetto and to
            change the current active module. When shutdowning, `mact' must
            be MA_NONE and `modtype' must be MT_NONE. When changing the active
            module, `mact' can be MA_CHANGE_MODULE, MA_CALL_MODULE or MA_RETURN.
            When `mact' is set to MA_RETURN, `modtype' must be MT_NONE. When
            `mact' is set to MA_CHANGE_MODULE or MA_CALL_MODULE, `modtype' must
            be the type of module you want to instantiate.
        @remarks
            When calling this method more than once in the same main loop, only
            the last call to it will be in effect.
        @see
            Sonetto::Kernel::KernelAction
        @see
            Sonetto::Kernel::ModuleAction
        @see
            Sonetto::Module::ModuleType
        */
        void setAction(KernelAction kact,ModuleAction mact = MA_NONE,
                Module::ModuleType modtype = Module::MT_NONE);

    private:
        /// Loads configuration from file and configures Sonetto
        void loadConfig(std::string file);

        /** Pushes new module into stack

            Instantiates a new module of type `modtype' using the module
            factory. This new module will be the new active module. If
            `mact' is set to MA_CALL, the old module remains in the stack.
            If `mact' is set to MA_CHANGE, the old one will be removed from
            the stack and deleted.
        @remarks
            MA_NONE and MA_RETURN are both invalid values for `mact'.
        @param modtype
            Module type to be instantiated.
        @param mact
            Whether to call it or change to it (see ModuleAction::MA_CHANGE
            and ModuleAction::MA_CALL).
        */
        void pushModule(Module::ModuleType modtype,ModuleAction mact);

        /** Pops current active module from stack

            Deinitialises, removes from stack and deletes the current active
            module, resuming any halted module below it in the stack.
        @remarks
            If no module remains in the stack, this method will throw an
            exception.
        */
        void popModule();

        /// Module factory
        const ModuleFactory *mModuleFactory;

        /// Ogre::Root instance
        Ogre::Root *mOgreRoot;

        /// Whether initialise() was called or not
        bool mInitialised;

        /** Stack of instantiated modules

        @see
            Sonetto::Module
        */
        ModuleStack mModuleStack;

        /** Kernel action

        @see
            Sonetto::Kernel::setAction()
        */
        KernelAction mKernelAction;

        /** Module action

        @see
            Sonetto::Kernel::setAction()
        */
        ModuleAction mModuleAction;

        /** Next module type to be instantiated

        @see
            Sonetto::Kernel::setAction()
        */
        Module::ModuleType mNextModuleType;
    };
} // namespace

#endif