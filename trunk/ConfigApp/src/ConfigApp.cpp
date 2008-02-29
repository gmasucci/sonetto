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

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#ifndef CA_USE_OPENGL
#   ifndef CA_USE_DIRECT3D
#       error Neither CA_USE_OPENGL nor CA_USE_DIRECT3D were set. A game cannot run without a Render System.
#   endif
#endif

#include <iostream>
#include <wx/wx.h>
#include "GL/Win32Handler.h"    // By now, only Win32 is supported
#include "D3D/D3DHandler.h"
#include "AL/ALHandler.h"
#include "ConfigApp.h"
#include "ConfigWindowImpl.h"

using namespace std;

namespace ConfigApplication {
    IMPLEMENT_APP(ConfigApp);

    bool ConfigApp::OnInit() {
        // Our main window
        ConfigWindowImpl *wnd = NULL;
        ALHandler        *al  = NULL;

        // Declare our used Render Systems' handlers (Direct3D is declared within ConfigApp's prototype)
#ifdef CA_USE_OPENGL
        GLHandler        *gl  = NULL;
#endif
        try {
            // Create ConfigWindow
            wnd = new ConfigWindowImpl(NULL,this);

            // Fills ConfigWindow with OpenGL data, or delete the OpenGL page if this compilation
            // doesn't use OpenGL
#ifdef CA_USE_OPENGL
            gl = new GLHandler();
            gl->fillWindow(wnd); // Fills with OpenGL data
            delete gl;
#else
            static_cast<wxChoicebook *>(wnd->FindWindow(ConfigWindow::ID_RSYS_CBOOK))->DeletePage(1); // 1 = OpenGL
#endif

            // Fills ConfigWindow with Direct3D data, or delete the Direct3D page if this compilation
            // doesn't use Direct3D
#ifdef CA_USE_DIRECT3D
            // Direct3D must be updated during application's execution,
            // so we wont delete it here yet
            d3d = new D3DHandler();
            d3d->fillWindow(wnd); // Fills with Direct3D data
#else
            static_cast<wxChoicebook *>(wnd->FindWindow(ConfigWindow::ID_RSYS_CBOOK))->DeletePage(0); // 0 = Direct3D 9
#endif

            al = new ALHandler();
            al->fillWindow(wnd);
            delete al;

            // OIS is also used during execution, so no deleting here too
            ois = new OISHandler(wnd);
            ois->fillWindow(wnd);

            // Show and set as top-level window our ConfigWindow
            wnd->Show(true);
            SetTopWindow(wnd);
        } catch (const wxChar *e) {
            wxMessageBox(e,wxT("An exception has been thrown"),wxOK|wxICON_ERROR,NULL);
            return false;
        } catch(OIS::Exception &e) {
            wxMessageBox(wxT(e.eText),wxT("An exception has been thrown"),wxOK|wxICON_ERROR,NULL);
            return false;
        } catch (...) {
            wxMessageBox(wxT("An unexpected exception has occurred"),
                         wxT("An exception has been thrown"),wxOK|wxICON_ERROR,NULL);

            return false;
        }

        return true;
    }

    int ConfigApp::OnExit() {
        printf("ConfigApp::OnExit reached.\n");

        // Destroy long-lived handlers
        delete d3d;
        delete ois;

        return 0;
    }
}
