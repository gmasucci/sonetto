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
#include <fstream>
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
        ALHandler        *al  = NULL;

        // Declare our used Render Systems' handlers (Direct3D is declared within ConfigApp's prototype)
#ifdef CA_USE_OPENGL
        GLHandler        *gl  = NULL;
#endif
        try {
            // Create ConfigWindow (our main and only window)
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

            // Grab Audio information, fill window, and discard handler
            al = new ALHandler();
            al->fillWindow(wnd);
            delete al;

            // OIS is also used during execution, so no deleting here too
            ois = new OISHandler(wnd);
            ois->fillWindow(wnd);

            // Show and set ConfigWindow as top-level window
            wnd->Show(true);
            SetTopWindow(wnd);
        } catch (const wxChar *e) {  // ConfigApp exception
            // Report problem
            wxMessageBox(e,wxT("An exception has been thrown"),wxOK|wxICON_ERROR,NULL);

            // Abort application
            return false;
        } catch(OIS::Exception &e) { // OIS exception
            // Report problem
            wxMessageBox(wxT(e.eText),wxT("An exception has been thrown"),wxOK|wxICON_ERROR,NULL);

            // Abort application
            return false;
        } catch (...) {              // Unexpected exception
            // Report problem
            wxMessageBox(wxT("An unexpected exception has occurred"),
                         wxT("An exception has been thrown"),wxOK|wxICON_ERROR,NULL);

            // Abort application
            return false;
        }

        // Everything went fine, the application should continue running
        return true;
    }

    int ConfigApp::OnExit() {
        printf("ConfigApp::OnExit reached.\n");

        // Destroy long-lived handlers
        delete d3d;
        delete ois;

        // No errors occurred
        return 0;
    }

    bool ConfigApp::Save() {
        ofstream fs;

        // Get handles to widgets
        wxChoicebook *rsys  = static_cast<wxChoicebook *>(wnd->FindWindow(ConfigWindow::ID_RSYS_CBOOK));
        wxChoice     *adev  = static_cast<wxChoice *>(wnd->FindWindow(ConfigWindow::ID_OAL_DEVICE));
        wxSlider     *mvol  = static_cast<wxSlider *>(wnd->FindWindow(ConfigWindow::ID_OAL_MVOL));
        wxSlider     *evol  = static_cast<wxSlider *>(wnd->FindWindow(ConfigWindow::ID_OAL_EVOL));
        wxSlider     *vvol  = static_cast<wxSlider *>(wnd->FindWindow(ConfigWindow::ID_OAL_VVOL));
        wxNotebook   *ptabs = static_cast<wxNotebook *>(wnd->FindWindow(ConfigWindow::ID_PLAYERS_TABS));

        // Opens configuration file for output, and truncates it to the beginning
        fs.open("game.cfg",ios_base::out | ios_base::trunc);
        if(!fs.is_open())
            return false;

        // Write options
        fs << "# Video options\n";
        fs << "RenderSystem=" << rsys->GetPageText(rsys->GetSelection()).c_str() << "\n";

        // Only selected renderer's options must be written
        if(strcmp(rsys->GetPageText(rsys->GetSelection()).c_str(),"OpenGL") == 0) {
            // OpenGL
            wxChoice   *res    = static_cast<wxChoice *>(wnd->FindWindow(ConfigWindow::ID_OGL_RES));
            wxChoice   *cdepth = static_cast<wxChoice *>(wnd->FindWindow(ConfigWindow::ID_OGL_CDEPTH));
            wxChoice   *freq   = static_cast<wxChoice *>(wnd->FindWindow(ConfigWindow::ID_OGL_FREQ));
            wxChoice   *fsaa   = static_cast<wxChoice *>(wnd->FindWindow(ConfigWindow::ID_OGL_FSAA));
            wxCheckBox *fscr   = static_cast<wxCheckBox *>(wnd->FindWindow(ConfigWindow::ID_OGL_FSCR));
            wxCheckBox *vsync  = static_cast<wxCheckBox *>(wnd->FindWindow(ConfigWindow::ID_OGL_VSYNC));

            fs << "Resolution=" << res->GetStringSelection().c_str() << "\n";
            fs << "ColourDepth=" << cdepth->GetStringSelection().c_str() << "\n";
            fs << "Frequency=" << freq->GetStringSelection().c_str() << "\n";
            fs << "AntiAliasing=" << fsaa->GetStringSelection().c_str() << "\n";
            fs << "Fullscreen=" << fscr->GetValue() << "\n";
            fs << "VSync=" << vsync->GetValue() << "\n";
        } else {
            // Direct3D
            wxChoice   *dspm   = static_cast<wxChoice *>(wnd->FindWindow(ConfigWindow::ID_D3D9_RES));
            wxChoice   *fsaa   = static_cast<wxChoice *>(wnd->FindWindow(ConfigWindow::ID_D3D9_AA));
            wxCheckBox *fscr   = static_cast<wxCheckBox *>(wnd->FindWindow(ConfigWindow::ID_D3D9_FSCR));
            wxCheckBox *vsync  = static_cast<wxCheckBox *>(wnd->FindWindow(ConfigWindow::ID_D3D9_VSYNC));

            fs << "DisplayMode=" << dspm->GetStringSelection().c_str() << "\n";
            fs << "AntiAliasing=" << fsaa->GetStringSelection().c_str() << "\n";
            fs << "Fullscreen=" << fscr->GetValue() << "\n";
            fs << "VSync=" << vsync->GetValue() << "\n";
        }

        fs << "\n# Audio options\n";
        fs << "AudioDevice=" << adev->GetStringSelection().c_str() << "\n";
        fs << "MusicVolume=" << mvol->GetValue() << "\n";
        fs << "EffectsVolume=" << evol->GetValue() << "\n";
        fs << "VoiceVolume=" << vvol->GetValue() << "\n";

        fs << "\n# Input options\n";

        // We must write information about every player tab
        // This iterates through them
        for(size_t i = 0;i < ptabs->GetPageCount();++i) {
            wxWindowList  tabChildren;
            wxPanel      *ptab   = static_cast<wxPanel *>(ptabs->GetPage(i));
            wxChoice     *idev   = static_cast<wxChoice *>(ptab->FindWindow(ConfigWindow::ID_INPUT_DEVICE));
            wxCheckBox   *plugd  = static_cast<wxCheckBox *>(ptab->FindWindow(ConfigWindow::ID_CTRL_PLUGGED));
            wxCheckBox   *ffback = static_cast<wxCheckBox *>(ptab->FindWindow(ConfigWindow::ID_CTRL_FFBACK));

            fs << "P" << i+1 << "_InputDevice=" << idev->GetSelection() << "\n";
            fs << "P" << i+1 << "_PluggedIn=" << plugd->GetValue() << "\n";
            fs << "P" << i+1 << "_ForceFeedback=" << ffback->GetValue() << "\n";

            // It is a little harder to write the button configurations
            // We have to iterate through every child window of the current
            // player tab and, if such window is a button configuration one,
            // we must write its value to the stream
            tabChildren = ptab->GetChildren();
            for(size_t j = 0;j < tabChildren.size();++j) {
                if(tabChildren[j]->GetId() == ConfigWindow::ID_BTNCONFIG) {
                    size_t      num = 0;
                    wxTextCtrl *btn = static_cast<wxTextCtrl *>(tabChildren[j]);

                    // TODO: Represent buttons by name
                    //       I think this indexing here(`num') is buggy, and won't give
                    //       correct values. This is just a proof of concept
                    num = (j-tabChildren.IndexOf(ptab->FindWindow(ConfigWindow::ID_BTNCONFIG)))/2;
                    fs << "P" << i+1 << "_Button" << num << "=" << btn->GetValue() << "\n";
                }
            }

            fs << "\n";
        }

        fs.flush();
        fs.close();

        return true;
    }
}
