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

#include "SonettoKernel.h"

namespace Sonetto
{
    // ----------------------------------------------------------------------
    // Sonetto::Kernel implementation
    // ----------------------------------------------------------------------
    SONETTO_SINGLETON_IMPLEMENT(Kernel);
    // ----------------------------------------------------------------------
    void Kernel::initialise()
    {
        // Checks if wasn't initialised yet
        if (mInitialised)
        {
            SONETTO_THROW("Kernel is already initialised");
        }

        // Creates Ogre root
        // Produce logs only on debug compilations
        #ifdef DEBUG
            mOgreRoot = new Ogre::Root("","","game.log");
        #else
            mOgreRoot = new Ogre::Root("","","");
        #endif

        // Configures Sonetto based on configuration file
        loadConfig("config.ini");

        // <todo> Get this caption from somewhere that makes more sense
        // (Sonetto Runtime, maybe?)
        mOgreRoot->initialise(true,"Game");

        // Creates a Boot Module and activates it
        pushModule(Module::MT_BOOT,MA_CHANGE);

        // Flags we have initialised
        mInitialised = true;
    }
    // ----------------------------------------------------------------------
    Kernel::~Kernel()
    {
        // Deinitialise if initialised
        if (mInitialised)
        {
            // Deinitialises and deletes instantiated modules
            while (!mModuleStack.empty())
            {
                Module *module = mModuleStack.top();

                // Deinitialises, deletes and removes from stack
                module->deinitialise();
                delete mModuleStack.top();
                mModuleStack.pop();
            }

            // Deletes Ogre
            delete mOgreRoot;
        }
    }
    // ----------------------------------------------------------------------
    void Kernel::run()
    {
        bool running = true;

        // <todo> Implement this method correctly
        while (running)
        {
            // Update managers
            // Update top module
            // Render one frame

            switch (mKernelAction)
            {
                case KA_CHANGE_MODULE:
                    if (mModuleAction == MA_RETURN) {
                        // Pops current module, returning to the previous one
                        popModule();
                    } else {
                        // Pushes desired module with the desired action
                        pushModule(mNextModuleType,mModuleAction);
                    }

                    // Resets action parameters
                    mKernelAction   = KA_NONE;
                    mModuleAction   = MA_NONE;
                    mNextModuleType = Module::MT_NONE;
                break;

                case KA_SHUTDOWN:
                    // Stops running
                    running = false;
                break;

                default: break;
            }
        }
    }
    // ----------------------------------------------------------------------
    void Kernel::setAction(KernelAction kact,ModuleAction mact,
            Module::ModuleType modtype)
    {
        switch (kact)
        {
            case KA_NONE:
                // Makes sure parameters are valid
                assert(mact == MA_NONE && modtype == Module::MT_NONE);

                // Sets action parameters
                mKernelAction   = KA_NONE;
                mModuleAction   = MA_NONE;
                mNextModuleType = Module::MT_NONE;
            break;

            case KA_CHANGE_MODULE:
                // Makes sure parameters are valid
                assert(mact != MA_NONE && modtype != Module::MT_NONE);

                // Sets action parameters
                mKernelAction   = KA_CHANGE_MODULE;
                mModuleAction   = mact;
                mNextModuleType = modtype;
            break;

            case KA_SHUTDOWN:
                // Makes sure parameters are valid
                assert(mact == MA_NONE && modtype == Module::MT_NONE);

                // Sets action parameters
                mKernelAction   = KA_SHUTDOWN;
                mModuleAction   = MA_NONE;
                mNextModuleType = Module::MT_NONE;
            break;
        }
    }
    // ----------------------------------------------------------------------
    void Kernel::loadConfig(std::string file) {}
    // ----------------------------------------------------------------------
    void Kernel::pushModule(Module::ModuleType modtype,ModuleAction mact)
    {
        Module *newmod,*curmod = NULL;

        // Makes sure parameters are valid
        assert(modtype != Module::MT_NONE && mact != MA_NONE &&
                mact != MA_RETURN);

        // Instantiates new module
        newmod = mModuleFactory->createModule(modtype);

        // Gets current active module (if any)
        if (!mModuleStack.empty())
        {
            curmod = mModuleStack.top();

            if (mact == MA_CHANGE) {
                // Deinitialises, deletes and removes the
                // current module from the stack
                curmod->deinitialise();
                delete curmod;
                mModuleStack.pop();
            } else {
                // Halts current module
                curmod->halt();
            }
        }

        // Pushes new module into stack and initialises it
        mModuleStack.push(newmod);
        newmod->initialise();
    }
    // ----------------------------------------------------------------------
    void Kernel::popModule()
    {
        Module *module;

        // Makes sure this won't leave the stack empty
        if (mModuleStack.size() < 2)
        {
            SONETTO_THROW("Cannot empty module stack");
        }

        // Gets current active module, deinitialises and deletes it
        module = mModuleStack.top();
        module->deinitialise();
        delete module;

        // Gets new top module and resume its execution
        mModuleStack.top()->resume();
    }
} // namespace
