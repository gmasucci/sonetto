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

#include <sstream>
#ifdef _WIN32
#   include <windows.h>
#endif
#include <wx/window.h>
#include <OIS.h>
#include "AppForms.h"
#include "OIS/OISHandler.h"

using namespace std;

namespace ConfigApplication {
    bool KeyListener::keyPressed(const OIS::KeyEvent &evt) {
        wxWindow *focus = wxWindow::FindFocus();
        
        if(focus->GetId() == ConfigWindow::ID_BTNCONFIG) {
            wxTextCtrl    *textbox = static_cast<wxTextCtrl *>(focus);
            ostringstream  str;
            
            str << "KEY_" << evt.key;
            textbox->SetValue(wxT(str.str()));
        }
    }
    
    bool JoyStickListener::buttonPressed(const OIS::JoyStickEvent &evt,int btn) {
        wxWindow *focus = wxWindow::FindFocus();
        
        if(focus->GetId() == ConfigWindow::ID_BTNCONFIG) {
            size_t      devID = 0;
            wxNotebook *notebook = static_cast<wxNotebook *>(wxWindow::FindWindowById(ConfigWindow::ID_PLAYERS_TAB));
            wxPanel    *ptab     = static_cast<wxPanel *>(notebook->GetCurrentPage());
            wxChoice   *device   = static_cast<wxChoice *>(ptab->FindWindow(ConfigWindow::ID_INPUT_DEVICE));
            
            for(size_t i = 0;i < mJoys.size();++i) {
                if(mJoys[i] == evt.device) {
                    devID = i;
                    break;
                }
            }
            
            if((int)devID == device->GetSelection()-1) {
                wxTextCtrl    *textbox = static_cast<wxTextCtrl *>(focus);
                ostringstream  str;
            
                str << "BTN_" << btn;
                textbox->SetValue(wxT(str.str()));
            }
        }
    }
    
    bool JoyStickListener::axisMoved(const OIS::JoyStickEvent &evt,int axis) {
        wxWindow      *focus = wxWindow::FindFocus();
        
        if(focus->GetId() == ConfigWindow::ID_BTNCONFIG && abs(evt.state.mAxes[axis].abs) > OIS::JoyStick::MAX_AXIS/2) {
            size_t      devID = 0;
            wxNotebook *notebook = static_cast<wxNotebook *>(wxWindow::FindWindowById(ConfigWindow::ID_PLAYERS_TAB));
            wxPanel    *ptab     = static_cast<wxPanel *>(notebook->GetCurrentPage());
            wxChoice   *device   = static_cast<wxChoice *>(ptab->FindWindow(ConfigWindow::ID_INPUT_DEVICE));
            
            for(size_t i = 0;i < mJoys.size();++i) {
                if(mJoys[i] == evt.device) {
                    devID = i;
                    break;
                }
            }
            
            if((int)devID == device->GetSelection()-1) {
                wxTextCtrl    *textbox = static_cast<wxTextCtrl *>(focus);
                ostringstream  str;
            
                str << "AXS_" << axis;
                textbox->SetValue(wxT(str.str()));
            }
        }
    }

    OISHandler::OISHandler(wxWindow *wnd) {
        OIS::ParamList pl;

#ifdef _WIN32
        // We need to extract from the window its handle
        // under Windows
        ostringstream  windowHndStr;
        size_t         windowHnd = 0;

        // This will cause an assertion error if wnd is NULL
        assert(wnd && "Under windows, `wnd' cannot be NULL");

        // Grab window handle from wnd and format it as a string
        windowHnd = (size_t)(wnd->GetHandle());
        windowHndStr << windowHnd;

        // Insert a new parameter field called "WINDOW" containing
        // a window handle, as expected by OIS
        pl.insert(make_pair(string("WINDOW"),windowHndStr.str()));
#endif

        // Create an input manager
        mInputMan = OIS::InputManager::createInputSystem(pl);

        // Get handles for the keyboard and all present joysticks
        // Also attaches event listeners to them
        mKeyboard = static_cast<OIS::Keyboard *>(mInputMan->createInputObject(OIS::OISKeyboard,false));
        mKeyboard->setEventCallback(&mKeyListener);
        mKeyboard->setBuffered(true);
        
        for(size_t i = 0;i < (size_t)(mInputMan->getNumberOfDevices(OIS::OISJoyStick));++i) {
            mJoys.push_back(static_cast<OIS::JoyStick *>(mInputMan->
                               createInputObject(OIS::OISJoyStick,false)));
            
            mJoys.back()->setEventCallback(&mJoyListener);
            mJoys.back()->setBuffered(true);
            mJoyListener.getJoysVector()->push_back(mJoys.back());
        }
    }

    OISHandler::~OISHandler() {
        // Release handles
        mInputMan->destroyInputObject(mKeyboard);
        for(size_t i = 0;i < mJoys.size();++i)
            mInputMan->destroyInputObject(mJoys[i]);

        // Release our input manager
        OIS::InputManager::destroyInputSystem(mInputMan);
    }

    void OISHandler::fillWindow(wxWindow *wnd) {
        //wxNotebook *notebook = static_cast<wxNotebook *>(wnd->FindWindow(ConfigWindow::ID_PLAYERS_TAB));
        wxChoice   *devices = static_cast<wxChoice *>(wnd->FindWindow(ConfigWindow::ID_INPUT_DEVICE));

        //for(size_t i = 0;i < 4;++i) {
            //wxPanel *ptab = notebook->
            // Get each devices' vendors and append in choicebox
            devices->Append(wxT("Standard Keyboard (Default)"));
            for(size_t i = 0;i < mJoys.size();++i)
                devices->Append(wxT(mJoys[i]->vendor()));

            // Defaults to keyboard
            devices->SetSelection(0);
        //}
    }

    void OISHandler::update(wxWindow *wnd) {
            mKeyboard->capture();
            for(size_t i = 0;i < mJoys.size();++i)
                mJoys[i]->capture();
    }
} // namespace
