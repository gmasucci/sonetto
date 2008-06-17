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

#ifdef CA_USE_OPENGL

#include <algorithm>
#include <sstream>
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <wx/choice.h>
#include "Utils.h"
#include "AppForms.h"
#include "GL/wglext.h"
#include "GL/Win32Handler.h"

using namespace std;

// Much of this file was made based on, or even copied from, Ogre3D's OgreWin32GLSupport.cpp
// Thanks

namespace ConfigApplication {
    LRESULT CALLBACK GLHandler::dummyWndProc(HWND hwnd,UINT umsg,WPARAM wp,LPARAM lp) {
        return DefWindowProc(hwnd,umsg,wp,lp);
    }

    GLHandler::GLHandler() {
        // This constructor will create an hidden, dummy window
        // used for listing supported FSAA levels on this machine
        // It will also list all supported resolutions and color depths
        PIXELFORMATDESCRIPTOR pfd;
        int                   format;
        bool                  hasMultisample = false;
        LPCSTR                wndName = "OpenGL Window"; // Dummy window name
        HINSTANCE             hInst   = NULL;            // Module Handle
        HDC                   hDC     = NULL;            // Windows Device Context
        HGLRC                 hGLRC   = NULL;            // GL Render Context
        HWND                  glWindow;
        WNDCLASS              wndClass;

        // Get a module handle which describes our application
        // It is needed for registering a window class
        hInst = GetModuleHandle(NULL);

        // Clean struct
        memset(&wndClass,0x00,sizeof(WNDCLASS));

        wndClass.style         = CS_OWNDC; // CS_OWNDC flag is needed by OpenGL
        wndClass.hInstance     = hInst;
        wndClass.lpfnWndProc   = GLHandler::dummyWndProc;
        wndClass.lpszClassName = wndName;  // Class name same as window name, for ease of use
        // It wont appear, anyway..

        // Registers class
        // Further references will use its name (wndName)
        RegisterClass(&wndClass);

        // Spawns our hidden window
        glWindow = CreateWindow(wndName,wndName,WS_POPUP|WS_CLIPCHILDREN,
                                0,0,32,32,0,0,hInst,0);

        if (!glWindow)
            throw wxT("Could not create OpenGL window in GLHandler::GLHandler()");

        // Get window device context
        hDC = GetDC(glWindow);

        // Clean and fill pixelformat struct
        // This pixelformat should be supported
        // by any device. The purpose of the
        // dummy window is just enumerate FSAA
        // levels, but we must have a window to
        // do so. So we create this widely
        // supported one
        memset(&pfd,0x00,sizeof(PIXELFORMATDESCRIPTOR));
        pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
        pfd.nVersion   = 1;
        pfd.cColorBits = 16;
        pfd.cDepthBits = 15;
        pfd.dwFlags    = PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;

        format = ChoosePixelFormat(hDC,&pfd);
        if (format == 0)
            throw wxT("Your graphics card does not support the minimal requirements, sorry.");

        SetPixelFormat(hDC,format,&pfd);

        hGLRC = wglCreateContext(hDC);
        if (hGLRC) {
            // Backup contexts
            HDC   oldDC   = wglGetCurrentDC();
            HGLRC oldGLRC = wglGetCurrentContext();

            if (wglMakeCurrent(hDC,hGLRC)) {
                DEVMODE devMode;
                PFNWGLGETEXTENSIONSSTRINGARBPROC _wglGetExtensionsStringARB =
                    (PFNWGLGETEXTENSIONSSTRINGARBPROC)
                    wglGetProcAddress("wglGetExtensionsStringARB");

                // wglGetExtensionsStringARB will return a list of
                // extensions available for use on this machine
                if (_wglGetExtensionsStringARB) {
                    istringstream wglexts(_wglGetExtensionsStringARB(hDC));
                    string        ext;

                    // Each of these extensions are pulled out to `ext' here, which
                    // is checked if it is something of our interest
                    while (wglexts >> ext) {
                        if (ext == "WGL_ARB_pixel_format") {
                            // Will we use this?
                        } else
                            if (ext == "WGL_ARB_multisample") {
                                hasMultisample = true;
                            }
                    }

                    // List available FSAA levels
                    if (hasMultisample) {
                        int              formats[256];
                        unsigned int     count;
                        static const int iattr[] = {
                            // Field              // Value
                            WGL_DRAW_TO_WINDOW_ARB, GL_TRUE                  ,
                            WGL_SUPPORT_OPENGL_ARB, GL_TRUE                  ,
                            WGL_DOUBLE_BUFFER_ARB , GL_TRUE                  ,
                            WGL_SAMPLE_BUFFERS_ARB, GL_TRUE                  ,
                            WGL_ACCELERATION_ARB  , WGL_FULL_ACCELERATION_ARB,
                            WGL_SAMPLES_ARB       , 2                        ,
                            0
                        };

                        PFNWGLCHOOSEPIXELFORMATARBPROC _wglChoosePixelFormatARB =
                            (PFNWGLCHOOSEPIXELFORMATARBPROC)
                            wglGetProcAddress("wglChoosePixelFormatARB");

                        if (_wglChoosePixelFormatARB(hDC,iattr,0,256,formats,&count)) {
                            // Determine what multisampling levels are offered
                            int query = WGL_SAMPLES_ARB,samples;

                            // Get multisample levels of each pixelformat available
                            for (size_t i = 0;i < count;++i) {
                                PFNWGLGETPIXELFORMATATTRIBIVARBPROC _wglGetPixelFormatAttribivARB =
                                    (PFNWGLGETPIXELFORMATATTRIBIVARBPROC)
                                    wglGetProcAddress("wglGetPixelFormatAttribivARB");

                                // Put them in mFSAALevels vector
                                if (_wglGetPixelFormatAttribivARB(hDC,formats[i],0,1,&query,&samples))
                                    mFSAALevels.push_back(samples);
                            }

                            // As different pixelformats may share the same FSAA Levels,
                            // there may be duplicate entries in mFSAALevels, which
                            // are stripped off here
                            removeDuplicates(mFSAALevels);
                        }
                    } // mHasMultisample
                } // _wglGetExtensionsStringARB

                // List resolutions and color depths
                devMode.dmSize = sizeof(DEVMODE);
                for(size_t i = 0;EnumDisplaySettings(NULL,i,&devMode);++i) {
                    stringstream str;

                    // Skip modes lower than 16 bits of color depth or 640x480 pixels of resolution
                    if(devMode.dmBitsPerPel < 16 || devMode.dmPelsHeight < 480)
                        continue;

                    str << devMode.dmPelsWidth << "x" << devMode.dmPelsHeight;
                    mResolutions.push_back(str.str());
                    mColorDepths.push_back(devMode.dmBitsPerPel);
                    mFrequencies.push_back(devMode.dmDisplayFrequency);
                }

                removeDuplicates(mResolutions);
                removeDuplicates(mColorDepths);
                removeDuplicates(mFrequencies);

                // Recover backuped contexts
                wglMakeCurrent(oldDC,oldGLRC);
            }

            // Delete dummy context
            wglDeleteContext(hGLRC);
        }

        // Destroy dummy window and unregister dummy window class
        DestroyWindow(glWindow);
        UnregisterClass(wndName,hInst);
    }

    void GLHandler::fillWindow(wxWindow *wnd) {
        wxChoice *resolutions = static_cast<wxChoice *>(wnd->FindWindow(ConfigWindow::ID_OGL_RES));
        wxChoice *colordepths = static_cast<wxChoice *>(wnd->FindWindow(ConfigWindow::ID_OGL_CDEPTH));
        wxChoice *frequencies = static_cast<wxChoice *>(wnd->FindWindow(ConfigWindow::ID_OGL_FREQ));
        wxChoice *fsaaLevels  = static_cast<wxChoice *>(wnd->FindWindow(ConfigWindow::ID_OGL_FSAA));

        // Fill resolutions' choicebox
        for(size_t i = 0;i < mResolutions.size();++i)
            resolutions->Append(wxT(mResolutions[i]));

        resolutions->SetSelection(0);

        // Fill colordepths' choicebox
        for(size_t i = 0;i < mColorDepths.size();++i) {
            ostringstream str;

            str << mColorDepths[i] << " bits";
            colordepths->Append(wxT(str.str()));
        }

        colordepths->SetSelection(0);

        // Fill frequencies' choicebox
        for(size_t i = 0;i < mFrequencies.size();++i) {
            ostringstream str;

            str << mFrequencies[i] << " Hz";
            frequencies->Append(wxT(str.str()));
        }

        frequencies->SetSelection(0);

        // Fill FSAAs' choicebox
        fsaaLevels->Append(wxT("None"));
        fsaaLevels->SetSelection(0);
        for(size_t i = 0;i < mFSAALevels.size();++i) {
            ostringstream str;

            str << mFSAALevels[i];
            fsaaLevels->Append(wxT(str.str()));
        }
    }
} // namespace

#endif // CA_USE_OPENGL
