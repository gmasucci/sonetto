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
#   include "windows.h"
#endif

#include <exception>
#include "SonettoKernel.h"
#include "GenericModuleFactory.h"

#ifdef WINDOWS
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
int main(int argc, char **argv)
#endif
{
    try {
        GenericModuleFactory *factory = new GenericModuleFactory;

        // Instantiates the Kernel, initializes and runs it
        Sonetto::Kernel *kernel = new Sonetto::Kernel(factory);
        kernel->initialize();
        kernel->run();

        // Deletes Kernel when finished running
        delete kernel;
    } catch(Sonetto::Exception &e) {
        const char *what = e.what();
        if (!what)
        {
            what = "An unknown error has happened,\n"
                   "It was not possible to identify the error.";
        }

        #ifdef WINDOWS
            MessageBox(NULL,what,"Game Runtime Error",
                    MB_OK|MB_ICONERROR|MB_TASKMODAL);
        #else
            cerr << "[!] Game Runtime Error\n" << what << "\n";
        #endif
    } catch(std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
