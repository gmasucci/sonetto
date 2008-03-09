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
#include <Ogre.h>
#include <OgreConfigFile.h>
//Use this define to signify OIS will be used as a DLL
//(so that dll import/export macros are in effect)
#define OIS_DYNAMIC_LIB
#include <OIS.h>
//#include "CustomPanelElementFactory.h"

#include <alc.h>
#include <al.h>

#include "SonettoKernel.h"
#include "SonettoModule.h"

class TestModule2 : public Sonetto::Module {
public:
    TestModule2() {}
    ~TestModule2() {}

    int counter;

    void enter() {
        Sonetto::Module::enter();
        mKernel->mViewport->setBackgroundColour(Ogre::ColourValue(0,0,1));
        counter = 0;
    }
    void update(Ogre::Real timedelta) {
        if (mKernel->mInputMan->getButtonState(0,Sonetto::BTN_CIRCLE) == Sonetto::KS_PRESS) {
            printf("Circle\n");
            mKernel->popModule();
        }
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

    int counter;

    void enter() {
        Sonetto::Module::enter();
        assert(mKernel);
        assert(mKernel->mViewport);
        mKernel->mViewport->setBackgroundColour(Ogre::ColourValue(1,0,0));

        /////////////////////// <fixme> Test area
        /*Ogre::Entity    *ent  = mSceneMan->createEntity(("Robot","robot.mesh");
        Ogre::SceneNode *node = mSceneMan->getRootSceneNode()->createChildSceneNode();

        node->setPosition(Ogre::Vector3(0,0,20.0f));
        node->attachObject(ent);*/
        ///////////////////////
    }
    void update(Ogre::Real timedelta) {
        float pos[3];

        if (mKernel->mInputMan->getButtonState(0,Sonetto::BTN_CROSS) == Sonetto::KS_PRESS)
            mKernel->pushModule(new TestModule2(), true);

        if(mKernel->mInputMan->getButtonState(0,Sonetto::BTN_L1) == Sonetto::KS_PRESS)
//            mKernel->mAudioMan->stopMusic(0.8f,false);

        if(mKernel->mInputMan->getButtonState(0,Sonetto::BTN_L2) == Sonetto::KS_PRESS)
//            mKernel->mAudioMan->memorizeMusic();

        if(mKernel->mInputMan->getButtonState(0,Sonetto::BTN_R2) == Sonetto::KS_PRESS)
//            mKernel->mAudioMan->restoreMusic();

        if(mKernel->mInputMan->getButtonState(0,Sonetto::BTN_TRIANGLE) == Sonetto::KS_PRESS)
//            mKernel->mAudioMan->playMusic(0,0.5f,0.5f);

        if(mKernel->mInputMan->getButtonState(0,Sonetto::BTN_SQUARE) == Sonetto::KS_PRESS)
//            mKernel->mAudioMan->playMusic(1,0.5f,0.5f);

        if(mKernel->mInputMan->getButtonState(0,Sonetto::BTN_R1) == Sonetto::KS_PRESS)
//            printf("New SourceID: %d\n",mKernel->mAudioMan->playSound(0,NULL));

        alGetListener3f(AL_POSITION,&pos[0],&pos[1],&pos[2]);
        if(mKernel->mInputMan->getButtonState(0,Sonetto::BTN_DPAD_LEFT) == Sonetto::KS_HOLD)
            pos[0] -= 0.000002f;
        if(mKernel->mInputMan->getButtonState(0,Sonetto::BTN_DPAD_RIGHT) == Sonetto::KS_HOLD)
            pos[0] += 0.000002f;
        if(mKernel->mInputMan->getButtonState(0,Sonetto::BTN_DPAD_UP) == Sonetto::KS_HOLD)
            pos[2] += 0.000002f;
        if(mKernel->mInputMan->getButtonState(0,Sonetto::BTN_DPAD_DOWN) == Sonetto::KS_HOLD)
            pos[2] -= 0.000002f;
//        alListener3f(AL_POSITION,pos[0],pos[1],pos[2]);

    }
    void exit() {
        Sonetto::Module::exit();
    }

    void halt() {
        Sonetto::Module::halt();

        counter = 0;
    }
    void wakeup() {
        Sonetto::Module::wakeup();
        mKernel->mViewport->setBackgroundColour(Ogre::ColourValue(1,0,0));
    }
};
using namespace Ogre;
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
int main(int argc, char **argv)
#endif
{
    Sonetto::KERNEL = new Sonetto::Kernel();
    TestModule *mTestModule = new TestModule();
    // Create application object
    try {
        if(!Sonetto::KERNEL->initialise()) {
            printf("Failed initialising kernel.\n");
            delete mTestModule;
            delete Sonetto::KERNEL;

            return -1;
        }

        // Push main module and run
        Sonetto::KERNEL->pushModule(mTestModule,false);
        Sonetto::KERNEL->run();
    } catch ( Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        std::cerr << "An exception has occured: " << e.getFullDescription();
#endif
    }
    delete mTestModule;
    return 0;
}
