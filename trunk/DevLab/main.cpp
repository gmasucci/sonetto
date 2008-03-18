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

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#   include <windows.h>
#endif

#include <Ogre.h>
#include <OgreConfigFile.h>

// Set OIS_DYNAMIC_LIB to signify OIS will be used as a DLL
// (so that dll import/export macros are in effect)
#define OIS_DYNAMIC_LIB
#include <OIS.h>

#include <alc.h>
#include <al.h>
#include "SonettoKernel.h"
#include "SonettoModule.h"

using namespace std;
using namespace Ogre;

class TestModule2 : public Sonetto::Module {
public:
    TestModule2() {}
    ~TestModule2() {}

    void enter() {
        Sonetto::Module::enter();
        mKernel->mViewport->setBackgroundColour(ColourValue(0,0,1));
    }
    void update(Real timedelta) {
        if (mKernel->mInputMan->getButtonState(0,Sonetto::BTN_CIRCLE) == Sonetto::KS_PRESS)
            mKernel->popModule();
    }
    void exit() {
        Sonetto::Module::exit();
    }

    void halt() {
        Sonetto::Module::halt();
    }
    void wakeup() {
        Sonetto::Module::wakeup();
    }
};

class TestModule : public Sonetto::Module {
public:
    TestModule() {}
    ~TestModule() {}

    void enter() {
        Sonetto::Module::enter();
        assert(mKernel);
        assert(mKernel->mViewport);
        mKernel->mViewport->setBackgroundColour(ColourValue(1,0,0));
    }

    void update(Real timedelta) {
        float pos[3];

        if (mKernel->mInputMan->getButtonState(0,Sonetto::BTN_CROSS) == Sonetto::KS_PRESS)
            mKernel->pushModule(new TestModule2(), true);

        if (mKernel->mInputMan->getButtonState(0,Sonetto::BTN_L2) == Sonetto::KS_PRESS)
            mKernel->mAudioMan->memorizeMusic(true);

        if (mKernel->mInputMan->getButtonState(0,Sonetto::BTN_R2) == Sonetto::KS_PRESS)
            mKernel->mAudioMan->restoreMusic();

        if (mKernel->mInputMan->getButtonState(0,Sonetto::BTN_TRIANGLE) == Sonetto::KS_PRESS)
            mKernel->mAudioMan->playMusic(0,0.5f,0.5f,1.0f,0,true,true);

        if (mKernel->mInputMan->getButtonState(0,Sonetto::BTN_SQUARE) == Sonetto::KS_PRESS)
            mKernel->mAudioMan->playMusic(1,0.5f,0.5f);

        if (mKernel->mInputMan->getButtonState(0,Sonetto::BTN_R1) == Sonetto::KS_PRESS)
            printf("New SourceID (environmental): %d\n",mKernel->mAudioMan->playSound(0,NULL));

        if (mKernel->mInputMan->getButtonState(0,Sonetto::BTN_L1) == Sonetto::KS_PRESS)
            printf("New SourceID (dry): %d\n",mKernel->mAudioMan->playSound(0,NULL,false));

        if (mKernel->mInputMan->getButtonState(0,Sonetto::BTN_DPAD_DOWN) == Sonetto::KS_RELEASE)
            mKernel->shutdown();

        alGetListener3f(AL_POSITION,&pos[0],&pos[1],&pos[2]);
        if (mKernel->mInputMan->getButtonState(0,Sonetto::BTN_DPAD_LEFT) == Sonetto::KS_HOLD)
            pos[0] -= 0.000002f;

        if (mKernel->mInputMan->getButtonState(0,Sonetto::BTN_DPAD_RIGHT) == Sonetto::KS_HOLD)
            pos[0] += 0.000002f;

        if (mKernel->mInputMan->getButtonState(0,Sonetto::BTN_DPAD_UP) == Sonetto::KS_HOLD)
            pos[2] += 0.000002f;

        if (mKernel->mInputMan->getButtonState(0,Sonetto::BTN_DPAD_DOWN) == Sonetto::KS_HOLD)
            pos[2] -= 0.000002f;

        alListener3f(AL_POSITION,pos[0],pos[1],pos[2]);

    }
    void exit() {
        Sonetto::Module::exit();
    }

    void halt() {
        Sonetto::Module::halt();
    }
    void wakeup() {
        Sonetto::Module::wakeup();
        mKernel->mViewport->setBackgroundColour(ColourValue(1,0,0));
    }
};

int main(int argc, char **argv) {
    Sonetto::KERNEL         = new Sonetto::Kernel();
    TestModule *mTestModule = new TestModule();

    try {
        // Initialise Kernel
        if (!Sonetto::KERNEL->initialise()) {
            printf("Failed initialising kernel.\n");
            delete mTestModule;
            delete Sonetto::KERNEL;

            return -1;
        }

        // Push main module and run
        Sonetto::KERNEL->pushModule(mTestModule,false);
        Sonetto::KERNEL->run();
    } catch (Exception &e) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        MessageBox(NULL,e.getFullDescription().c_str(),"An exception has occured!",
                   MB_OK|MB_ICONERROR|MB_TASKMODAL);
#else
        cerr << "An exception has occured: " << e.getFullDescription();
#endif
    }

    // Clean stuff
    delete mTestModule;

    // <fixme> Delete kernel correctly
    // This is crashing, I don't know why
    // delete Sonetto::KERNEL;
    return 0;
}
