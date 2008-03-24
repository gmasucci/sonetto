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

#include <exception>
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#   include <windows.h>
#endif

#include "SonettoKernel.h"

using namespace std;
using namespace Sonetto;

class TestModule2 : public Sonetto::Module {
public:
    TestModule2() {}
    ~TestModule2() {}

    void enter() {
        Sonetto::Module::enter();
        mKernel->getViewport()->setBackgroundColour(Ogre::ColourValue(0,0,1));
    }
    void update(Ogre::Real timedelta) {
        if (mKernel->getInputMan()->getButtonState(0,Sonetto::BTN_CIRCLE) == Sonetto::KS_PRESS)
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
        mKernel->getViewport()->setBackgroundColour(Ogre::ColourValue(1,0,0));
    }

    void update(Ogre::Real timedelta) {
        Ogre::Vector3 pos;

        if (mKernel->getInputMan()->getButtonState(0,Sonetto::BTN_CROSS) == Sonetto::KS_PRESS)
            mKernel->pushModule(new TestModule2(), true);

        if (mKernel->getInputMan()->getButtonState(0,Sonetto::BTN_L2) == Sonetto::KS_PRESS)
            mKernel->getAudioMan()->memorizeMusic(true);

        if (mKernel->getInputMan()->getButtonState(0,Sonetto::BTN_R2) == Sonetto::KS_PRESS)
            mKernel->getAudioMan()->restoreMusic();

        if (mKernel->getInputMan()->getButtonState(0,Sonetto::BTN_TRIANGLE) == Sonetto::KS_PRESS)
            mKernel->getAudioMan()->playMusic(0,0.5f,0.5f,1.0f,0,true,true);

        if (mKernel->getInputMan()->getButtonState(0,Sonetto::BTN_SQUARE) == Sonetto::KS_PRESS)
            mKernel->getAudioMan()->playMusic(1,0.5f,0.5f);

        if (mKernel->getInputMan()->getButtonState(0,Sonetto::BTN_R1) == Sonetto::KS_PRESS) {
            size_t id = mKernel->getAudioMan()->playSound(0,NULL);
            printf("New SourceID (environmental): %d\n",id);
        }

        if (mKernel->getInputMan()->getButtonState(0,Sonetto::BTN_L1) == Sonetto::KS_PRESS)
            printf("New SourceID (dry): %d\n",mKernel->getAudioMan()->playSound(0,NULL,false));

        if (mKernel->getInputMan()->getButtonState(0,Sonetto::BTN_DPAD_DOWN) == Sonetto::KS_RELEASE)
            mKernel->shutdown();

        pos = mCamera->getPosition();
        if (mKernel->getInputMan()->getButtonState(0,Sonetto::BTN_DPAD_LEFT) == Sonetto::KS_RELEASE) {
            float rolloff = mKernel->getAudioMan()->getSourceRolloff(0)+0.1f;
            if(rolloff > 1.0f)
                rolloff = 1.0f;

            mKernel->getAudioMan()->setSourceRolloff(0,rolloff);
        }

        if (mKernel->getInputMan()->getButtonState(0,Sonetto::BTN_DPAD_RIGHT) == Sonetto::KS_RELEASE) {
            float rolloff = mKernel->getAudioMan()->getSourceRolloff(0)-0.1f;
            if(rolloff < 0.0f)
                rolloff = 0.0f;

            mKernel->getAudioMan()->setSourceRolloff(0,rolloff);
        }

        if (mKernel->getInputMan()->getButtonState(0,Sonetto::BTN_DPAD_UP) == Sonetto::KS_HOLD)
            pos.z += 0.02f;

        if (mKernel->getInputMan()->getButtonState(0,Sonetto::BTN_DPAD_DOWN) == Sonetto::KS_HOLD)
            pos.z -= 0.02f;
    }
    void exit() {
        Sonetto::Module::exit();
    }

    void halt() {
        Sonetto::Module::halt();
    }
    void wakeup() {
        Sonetto::Module::wakeup();
        mKernel->getViewport()->setBackgroundColour(Ogre::ColourValue(1,0,0));
    }
};

int main(int argc,char **argv) {
    try {
        int     errorCode;
        Module *mainModule;

        Kernel::initialise();

        mainModule = new TestModule();
        Kernel::getSingleton()->pushModule(mainModule);

        errorCode = Kernel::getSingleton()->run();

        delete mainModule;
        return errorCode;
    } catch (Exception &e) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        MessageBox(NULL,e.what(),"An exception has occured!",
                   MB_OK|MB_ICONERROR|MB_TASKMODAL);
#else
        cerr << "An exception has occured!\n" << e.what() << "\n";
#endif
    }

    return -1;
}
