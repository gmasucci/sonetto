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

#ifndef __ConfigWindowImpl__
#define __ConfigWindowImpl__

#include <wx/app.h>
#include <wx/timer.h>
#include <wx/settings.h>
#include "ConfigApp.h"
#include "AppForms.h"

namespace ConfigApplication {
    /** Implementing ConfigWindow */
    class ConfigWindowImpl : public ConfigWindow {
        DECLARE_EVENT_TABLE()

    private:
        ConfigApp *mApp;

        wxTimer   *mTimer;

        /// @brief Event callback for D3D9 resolution change
        void onChoiceD3D9Res(wxCommandEvent& event);

        /// @brief Event callback for D3D9 fullscreen trigger change
        void onCheckD3D9FullScreen(wxCommandEvent& event);

        /** @brief Event callback for timer
         *
         *  This timer is used for updating OIS Input (Keyboard and
         *  Joysticks) and placing new states in the selected widgets.
         */
        void onTimerEvent(wxTimerEvent& event);

        /// @brief Event callback for input device change
        void onDeviceChoice(wxCommandEvent& event);
        void OnClickApply(wxCommandEvent& event);
        void OnClickCancel(wxCommandEvent& event);
        void OnClickOk(wxCommandEvent& event);

    public:
        /** Constructor */
        ConfigWindowImpl(wxWindow* parent,ConfigApp *app);
    };
} // namespace

#endif // __ConfigWindowImpl__
