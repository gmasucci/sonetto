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

        // Check whether the focus is about a button configuration window
        if (focus->GetId() >= ConfigWindow::ID_AXIS_LH1 &&
                focus->GetId() <= ConfigWindow::ID_BTN_DLEFT   ) {
            wxWindow      *iter    = NULL;
            wxTextCtrl    *textbox = static_cast<wxTextCtrl *>(focus);
            ostringstream  str;

            // Set text to "KEY_[keycode]"
            str << "KEY_" << evt.key;
            textbox->SetValue(wxT(str.str()));

            // Based on the focus window, iterates back parents until
            // a player tab is found, where we can search for other
            // button config windows
            iter = focus->GetParent();
            while (iter->GetId() != ConfigWindow::ID_PLAYER_TAB)
                iter = iter->GetParent();

            if (focus->GetId() == ConfigWindow::ID_AXIS_LH1) {
                wxTextCtrl *lh2 = static_cast<wxTextCtrl *>(iter->FindWindow(ConfigWindow::ID_AXIS_LH2));

                if (strcmp(lh2->GetValue().Left(3).c_str(),"AXS") == 0)
                    lh2->SetValue(wxT("Unused"));
            }

            if (focus->GetId() == ConfigWindow::ID_AXIS_LH2) {
                wxTextCtrl *lh1 = static_cast<wxTextCtrl *>(iter->FindWindow(ConfigWindow::ID_AXIS_LH1));

                if (strcmp(lh1->GetValue().Left(3).c_str(),"AXS") == 0)
                    lh1->SetValue(wxT("Unused"));
            }

            if (focus->GetId() == ConfigWindow::ID_AXIS_LV1) {
                wxTextCtrl *lv2 = static_cast<wxTextCtrl *>(iter->FindWindow(ConfigWindow::ID_AXIS_LV2));

                if (strcmp(lv2->GetValue().Left(3).c_str(),"AXS") == 0)
                    lv2->SetValue(wxT("Unused"));
            }

            if (focus->GetId() == ConfigWindow::ID_AXIS_LV2) {
                wxTextCtrl *lv1 = static_cast<wxTextCtrl *>(iter->FindWindow(ConfigWindow::ID_AXIS_LV1));

                if (strcmp(lv1->GetValue().Left(3).c_str(),"AXS") == 0)
                    lv1->SetValue(wxT("Unused"));
            }

            if (focus->GetId() == ConfigWindow::ID_AXIS_RH1) {
                wxTextCtrl *rh2 = static_cast<wxTextCtrl *>(iter->FindWindow(ConfigWindow::ID_AXIS_RH2));

                if (strcmp(rh2->GetValue().Left(3).c_str(),"AXS") == 0)
                    rh2->SetValue(wxT("Unused"));
            }

            if (focus->GetId() == ConfigWindow::ID_AXIS_RH2) {
                wxTextCtrl *rh1 = static_cast<wxTextCtrl *>(iter->FindWindow(ConfigWindow::ID_AXIS_RH1));

                if (strcmp(rh1->GetValue().Left(3).c_str(),"AXS") == 0)
                    rh1->SetValue(wxT("Unused"));
            }

            if (focus->GetId() == ConfigWindow::ID_AXIS_RV1) {
                wxTextCtrl *rv2 = static_cast<wxTextCtrl *>(iter->FindWindow(ConfigWindow::ID_AXIS_RV2));

                if (strcmp(rv2->GetValue().Left(3).c_str(),"AXS") == 0)
                    rv2->SetValue(wxT("Unused"));
            }

            if (focus->GetId() == ConfigWindow::ID_AXIS_RV2) {
                wxTextCtrl *rv1 = static_cast<wxTextCtrl *>(iter->FindWindow(ConfigWindow::ID_AXIS_RV1));

                if (strcmp(rv1->GetValue().Left(3).c_str(),"AXS") == 0)
                    rv1->SetValue(wxT("Unused"));
            }
        }

        return true;
    }

    bool JoyStickListener::buttonPressed(const OIS::JoyStickEvent &evt,int btn) {
        wxWindow *focus = wxWindow::FindFocus();

        // Check whether the focus is about a button configuration window
        if (focus->GetId() >= ConfigWindow::ID_AXIS_LH1 &&
                focus->GetId() <= ConfigWindow::ID_BTN_DLEFT   ) {
            size_t      devID = 0;
            wxNotebook *notebook = static_cast<wxNotebook *>(wxWindow::FindWindowById(ConfigWindow::ID_PLAYERS_TABS));
            wxPanel    *ptab     = static_cast<wxPanel *>(notebook->GetCurrentPage());
            wxChoice   *device   = static_cast<wxChoice *>(ptab->FindWindow(ConfigWindow::ID_INPUT_DEVICE));

            // Discovers device index in mJoys vector
            // [Warning] If a non-indexed joystick generates an event,
            // it >> will default to the one in index 0! <<
            for (size_t i = 0;i < mJoys.size();++i) {
                if (mJoys[i] == evt.device) {
                    devID = i;
                    break;
                }
            }

            // Compares to see whether the joystick which generated the event
            // is the one selected on the devices' choicebox
            if ((int)devID == device->GetSelection()-1) {
                wxWindow      *iter    = NULL;
                wxTextCtrl    *textbox = static_cast<wxTextCtrl *>(focus);
                ostringstream  str;

                // Set text to "BTN_[btn]"
                str << "BTN_" << btn;
                textbox->SetValue(wxT(str.str()));

                // Based on the focus window, iterates back parents until
                // a player tab is found, where we can search for other
                // button config windows
                iter = focus->GetParent();
                while (iter->GetId() != ConfigWindow::ID_PLAYER_TAB)
                    iter = iter->GetParent();

                if (focus->GetId() == ConfigWindow::ID_AXIS_LH1) {
                    wxTextCtrl *lh2 = static_cast<wxTextCtrl *>(iter->FindWindow(ConfigWindow::ID_AXIS_LH2));

                    if (strcmp(lh2->GetValue().Left(3).c_str(),"AXS") == 0)
                        lh2->SetValue(wxT("Unused"));
                }

                if (focus->GetId() == ConfigWindow::ID_AXIS_LH2) {
                    wxTextCtrl *lh1 = static_cast<wxTextCtrl *>(iter->FindWindow(ConfigWindow::ID_AXIS_LH1));

                    if (strcmp(lh1->GetValue().Left(3).c_str(),"AXS") == 0)
                        lh1->SetValue(wxT("Unused"));
                }

                if (focus->GetId() == ConfigWindow::ID_AXIS_LV1) {
                    wxTextCtrl *lv2 = static_cast<wxTextCtrl *>(iter->FindWindow(ConfigWindow::ID_AXIS_LV2));

                    if (strcmp(lv2->GetValue().Left(3).c_str(),"AXS") == 0)
                        lv2->SetValue(wxT("Unused"));
                }

                if (focus->GetId() == ConfigWindow::ID_AXIS_LV2) {
                    wxTextCtrl *lv1 = static_cast<wxTextCtrl *>(iter->FindWindow(ConfigWindow::ID_AXIS_LV1));

                    if (strcmp(lv1->GetValue().Left(3).c_str(),"AXS") == 0)
                        lv1->SetValue(wxT("Unused"));
                }

                if (focus->GetId() == ConfigWindow::ID_AXIS_RH1) {
                    wxTextCtrl *rh2 = static_cast<wxTextCtrl *>(iter->FindWindow(ConfigWindow::ID_AXIS_RH2));

                    if (strcmp(rh2->GetValue().Left(3).c_str(),"AXS") == 0)
                        rh2->SetValue(wxT("Unused"));
                }

                if (focus->GetId() == ConfigWindow::ID_AXIS_RH2) {
                    wxTextCtrl *rh1 = static_cast<wxTextCtrl *>(iter->FindWindow(ConfigWindow::ID_AXIS_RH1));

                    if (strcmp(rh1->GetValue().Left(3).c_str(),"AXS") == 0)
                        rh1->SetValue(wxT("Unused"));
                }

                if (focus->GetId() == ConfigWindow::ID_AXIS_RV1) {
                    wxTextCtrl *rv2 = static_cast<wxTextCtrl *>(iter->FindWindow(ConfigWindow::ID_AXIS_RV2));

                    if (strcmp(rv2->GetValue().Left(3).c_str(),"AXS") == 0)
                        rv2->SetValue(wxT("Unused"));
                }

                if (focus->GetId() == ConfigWindow::ID_AXIS_RV2) {
                    wxTextCtrl *rv1 = static_cast<wxTextCtrl *>(iter->FindWindow(ConfigWindow::ID_AXIS_RV1));

                    if (strcmp(rv1->GetValue().Left(3).c_str(),"AXS") == 0)
                        rv1->SetValue(wxT("Unused"));
                }
            }
        }

        return true;
    }

    bool JoyStickListener::axisMoved(const OIS::JoyStickEvent &evt,int axis) {
        wxWindow      *focus = wxWindow::FindFocus();

        // Check whether the focus is about a button configuration window
        if (focus->GetId() >= ConfigWindow::ID_AXIS_LH1                &&
                focus->GetId() <= ConfigWindow::ID_BTN_DLEFT               &&
                abs(evt.state.mAxes[axis].abs) > OIS::JoyStick::MAX_AXIS/2    ) {
            size_t      devID = 0;
            wxNotebook *notebook = static_cast<wxNotebook *>(wxWindow::FindWindowById(ConfigWindow::ID_PLAYERS_TABS));
            wxPanel    *ptab     = static_cast<wxPanel *>(notebook->GetCurrentPage());
            wxChoice   *device   = static_cast<wxChoice *>(ptab->FindWindow(ConfigWindow::ID_INPUT_DEVICE));

            // Discovers device index in mJoys vector
            // [Warning] If a non-indexed joystick generates an event,
            // it >> will default to the one in index 0! <<
            for (size_t i = 0;i < mJoys.size();++i) {
                if (mJoys[i] == evt.device) {
                    devID = i;
                    break;
                }
            }

            // Compares to see whether the joystick which generated the event
            // is the one selected on the devices' choicebox
            if ((int)devID == device->GetSelection()-1) {
                wxWindow      *iter    = NULL;
                wxTextCtrl    *textbox = static_cast<wxTextCtrl *>(focus);
                ostringstream  str;

                // Based on the focus window, iterates back parents until
                // a player tab is found, where we can search for other
                // button config windows
                iter = focus->GetParent();
                while (iter->GetId() != ConfigWindow::ID_PLAYER_TAB)
                    iter = iter->GetParent();

                // Set text to "AXS_[axis]"
                str << "AXS_" << axis;
                textbox->SetValue(wxT(str.str()));

                if (focus->GetId() == ConfigWindow::ID_AXIS_LH1) {
                    wxTextCtrl *lh2 = static_cast<wxTextCtrl *>(iter->FindWindow(ConfigWindow::ID_AXIS_LH2));

                    lh2->SetValue(wxT(str.str()));
                }

                if (focus->GetId() == ConfigWindow::ID_AXIS_LH2) {
                    wxTextCtrl *lh1 = static_cast<wxTextCtrl *>(iter->FindWindow(ConfigWindow::ID_AXIS_LH1));

                    lh1->SetValue(wxT(str.str()));
                }

                if (focus->GetId() == ConfigWindow::ID_AXIS_LV1) {
                    wxTextCtrl *lv2 = static_cast<wxTextCtrl *>(iter->FindWindow(ConfigWindow::ID_AXIS_LV2));

                    lv2->SetValue(wxT(str.str()));
                }

                if (focus->GetId() == ConfigWindow::ID_AXIS_LV2) {
                    wxTextCtrl *lv1 = static_cast<wxTextCtrl *>(iter->FindWindow(ConfigWindow::ID_AXIS_LV1));

                    lv1->SetValue(wxT(str.str()));
                }

                if (focus->GetId() == ConfigWindow::ID_AXIS_RH1) {
                    wxTextCtrl *rh2 = static_cast<wxTextCtrl *>(iter->FindWindow(ConfigWindow::ID_AXIS_RH2));

                    rh2->SetValue(wxT(str.str()));
                }

                if (focus->GetId() == ConfigWindow::ID_AXIS_RH2) {
                    wxTextCtrl *rh1 = static_cast<wxTextCtrl *>(iter->FindWindow(ConfigWindow::ID_AXIS_RH1));

                    rh1->SetValue(wxT(str.str()));
                }

                if (focus->GetId() == ConfigWindow::ID_AXIS_RV1) {
                    wxTextCtrl *rv2 = static_cast<wxTextCtrl *>(iter->FindWindow(ConfigWindow::ID_AXIS_RV2));

                    rv2->SetValue(wxT(str.str()));
                }

                if (focus->GetId() == ConfigWindow::ID_AXIS_RV2) {
                    wxTextCtrl *rv1 = static_cast<wxTextCtrl *>(iter->FindWindow(ConfigWindow::ID_AXIS_RV1));

                    rv1->SetValue(wxT(str.str()));
                }
            }
        }

        return true;
    }

    OISHandler::OISHandler(wxWindow *wnd) {
        OIS::ParamList pl;

#ifdef _WIN32
        // We need to extract from the window its handle
        // under Windows unoperational system
        ostringstream  windowHndStr;
        size_t         windowHnd = 0;

        // This will cause an assertion error if `wnd' is NULL
        assert(wnd && "Under windows, `wnd' cannot be NULL");

        // Grab window handle from `wnd' and format it as a string
        windowHnd = (size_t)(wnd->GetHandle());
        windowHndStr << windowHnd;

        // Insert a new parameter field called "WINDOW" containing
        // a window handle, as expected by OIS
        pl.insert(make_pair(string("WINDOW"),windowHndStr.str()));
#endif

        // Create an input manager
        // If this fails it will throw an exception
        mInputMan = OIS::InputManager::createInputSystem(pl);

        // Gets handles for the keyboard and all present joysticks
        // Also attaches event listeners to them
        mKeyboard = static_cast<OIS::Keyboard *>(mInputMan->createInputObject(OIS::OISKeyboard,false));
        mKeyboard->setEventCallback(&mKeyListener);
        mKeyboard->setBuffered(true);

        for (size_t i = 0;i < (size_t)(mInputMan->getNumberOfDevices(OIS::OISJoyStick));++i) {
            // Push all detected joysticks inside mJoys vector
            mJoys.push_back(static_cast<OIS::JoyStick *>(mInputMan->
                            createInputObject(OIS::OISJoyStick,false)));

            mJoys.back()->setEventCallback(&mJoyListener);
            mJoys.back()->setBuffered(true);

            // The event listener must also keep track of available joysticks,
            // so we push it there too
            mJoyListener.getJoysVector()->push_back(mJoys.back());
        }
    }

    OISHandler::~OISHandler() {
        // Release handles
        mInputMan->destroyInputObject(mKeyboard);
        for (size_t i = 0;i < mJoys.size();++i)
            mInputMan->destroyInputObject(mJoys[i]);

        // Release input manager
        OIS::InputManager::destroyInputSystem(mInputMan);
    }

    void OISHandler::fillWindow(wxWindow *wnd) {
        // Get notebook containing players' tabs
        wxNotebook *notebook = static_cast<wxNotebook *>(wnd->FindWindow(ConfigWindow::ID_PLAYERS_TABS));

        // Iterate through each page, filling them with device
        // options
        for (size_t i = 0;i < notebook->GetPageCount();++i) {
            wxPanel  *ptab    = static_cast<wxPanel *>(notebook->GetPage(i));
            wxChoice *devices = static_cast<wxChoice *>(ptab->FindWindow(ConfigWindow::ID_INPUT_DEVICE));

            // Get each devices' vendors and append in choicebox
            devices->Append(wxT("Standard Keyboard (Default)"));
            for (size_t i = 0;i < mJoys.size();++i)
                devices->Append(wxT(mJoys[i]->vendor()));

            // Defaults to keyboard
            devices->SetSelection(0);
        }
    }

    void OISHandler::update(wxWindow *wnd) {
        // Capture inputs and generate events
        mKeyboard->capture();
        for (size_t i = 0;i < mJoys.size();++i)
            mJoys[i]->capture();
    }
} // namespace
