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

#ifdef WINDOWS
#   include <windows.h>
#endif

#include <exception>
#include <OgreLogManager.h>
#include "SonettoKernel.h"
#include "GenericModuleFactory.h"

void reportException(const char *msg);

#ifdef WINDOWS
INT WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,INT)
#else
int main(int argc,char *argv[])
#endif
{
    GenericModuleFactory factory;
    Sonetto::Kernel kernel(&factory);

    try {
        kernel.initialize();
        kernel.run();
    } catch(Sonetto::Exception &e) {
        const char *what = e.what();

        if (!what)
        {
            what = "An unknown error has happened.\n"
                   "It was not possible to identify the error.";
        }

        reportException(what);
    } catch(Ogre::FileNotFoundException &e) {
        reportException(("A game file could not be found.\n" +
                e.getDescription()).c_str());
    } catch(std::bad_alloc &e) {
        reportException("Out of system memory.");
    } catch(std::exception &e) {
        reportException(e.what());
    }

    return 0;
}

void reportException(const char *msg)
{
    std::string logMessage = "\n[!] Game Runtime Error\n" + std::string(msg);

    Ogre::LogManager::getSingleton().getDefaultLog()->
            logMessage(logMessage,Ogre::LML_CRITICAL);

    #ifdef WINDOWS
        MessageBox(NULL,msg,"Game Runtime Error",
                MB_OK | MB_ICONERROR | MB_TASKMODAL);
    #else
        cerr << logMessage << '\n';
    #endif
}
