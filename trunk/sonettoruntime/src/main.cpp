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

#include <OGRE/Ogre.h>
#include "Sonetto.h"
#include "BasicModule.h"

using namespace std;
using namespace Sonetto;

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
            cout << "Sonetto Kernel could not be created.\n";
            return -1;
        }

        Kernel::get()->pushModule(new BasicModule());
        Kernel::get()->run();

        Kernel::destroy();
    } catch(Sonetto::Exception &e) {
        const char *what = e.what();

        if (!what)
        {
            what = "Sonetto Exception\n"
                   "  In: <unknown>\n\n"
                   "  An exception was thrown during Sonetto exception message formatting.";
        }

        #ifdef _WINDOWS
            MessageBox(NULL,what,"A Sonetto exception has occurred!",
                    MB_OK|MB_ICONERROR|MB_TASKMODAL);
        #else
            cerr << "A Sonetto exception has occurred!\n" << what << "\n";
        #endif
    } catch(Ogre::Exception &e) {
        #ifdef _WINDOWS
            MessageBox(NULL,e.getFullDescription().c_str(),"An Ogre exception has occurred!",
                    MB_OK|MB_ICONERROR|MB_TASKMODAL);
        #else
            cerr << "An Ogre exception has occurred!\n" << e.getFullDescription().c_str() << "\n";
        #endif
    } catch(std::exception &e) {
        #ifdef _WINDOWS
            MessageBox(NULL,e.what(),"A standard exception has occurred!",
                    MB_OK|MB_ICONERROR|MB_TASKMODAL);
        #else
            cerr << "A standard exception has occurred!\n" << e.what() << "\n";
        #endif
    }

    return 0;
}
