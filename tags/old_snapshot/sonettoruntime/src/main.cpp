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

#include <cstdio>

#ifdef _WINDOWS
#include "windows.h"
#endif

#include <Ogre.h>
#include "Sonetto.h"
#include "SonettoException.h"
#include "BasicModule.h"
#include "TESTMapFileManager.h"
#include "TESTMapModule.h"

using namespace std;
using namespace Sonetto;

class TestModuleFactory : public Sonetto::ModuleFactory
{
public:
    Sonetto::Module * createBootModule()
    {
        return new BootModule();
    }
    Sonetto::Module * createTitleModule()
    {
        return NULL;
    }
    Sonetto::Module * createMapModule()
    {
        return new TestMapModule();
    }
    Sonetto::Module * createMenuModule()
    {
        return NULL;
    }
    Sonetto::Module * createBattleModule()
    {
        return NULL;
    }
    Sonetto::Module * createWorldMapModule()
    {
        return NULL;
    }
    Sonetto::Module * createExtraModuleA()
    {
        return NULL;
    }
    Sonetto::Module * createExtraModuleB()
    {
        return NULL;
    }
    Sonetto::Module * createExtraModuleC()
    {
        return NULL;
    }
};
#ifdef _WINDOWS
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
int main(int argc, char **argv)
#endif
{
    try {
        // Create Kernel Singleton
        if (!Kernel::create())
        {
            SONETTO_THROW("The Kernel could not be created");
        }
        Kernel::get()->initialise();
        TestModuleFactory * moduleFactory = new TestModuleFactory();
        Kernel::get()->registerModuleFactory(moduleFactory);
        Kernel::get()->pushModule(new BasicModule());
        // Set up Custom Resources.
        Kernel::get()->run();
        Kernel::destroy();
        delete moduleFactory;
    } catch(Sonetto::Exception &e) {
        const char *what = e.what();

        if (!what)
        {
            what =  "An unknown error has happened,\n"
                    "It was not possible to identify the error.";
        }

    #ifdef _WINDOWS
        MessageBox(NULL,what,"Game Runtime Error",
                MB_OK|MB_ICONERROR|MB_TASKMODAL);
    #else
        cerr << "[!] Game Runtime Error\n" << what << "\n";
    #endif
    } catch(Ogre::FileNotFoundException &e) {
        std::string whaterror = "A game file was not found.\n" + e.getDescription();
        #ifdef _WINDOWS
        MessageBox(NULL,whaterror.c_str(),"Game Runtime Error",
                    MB_OK|MB_ICONERROR|MB_TASKMODAL);
    #else
        cerr << "[!] Game Runtime Error\n" << e.what() << "\n";
    #endif
    } catch(std::exception &e) {
    #ifdef _WINDOWS
        MessageBox(NULL,e.what(),"Game Runtime Error",
                    MB_OK|MB_ICONERROR|MB_TASKMODAL);
    #else
        cerr << "[!] Game Runtime Error\n" << e.what() << "\n";
    #endif
    }/* catch(Ogre::Exception &e) {
        #ifdef _WINDOWS
            MessageBox(NULL,e.getFullDescription().c_str(),"Game Runtime Error",
                    MB_OK|MB_ICONERROR|MB_TASKMODAL);
        #else
            cerr << "[!] Game Runtime Error\n" << e.getFullDescription().c_str() << "\n";
        #endif
    } catch(std::exception &e) {
        #ifdef _WINDOWS
            MessageBox(NULL,e.what(),"Game Runtime Error",
                    MB_OK|MB_ICONERROR|MB_TASKMODAL);
        #else
            cerr << "[!] Game Runtime Error\n" << e.what() << "\n";
        #endif
    }*/

    return 0;
}
