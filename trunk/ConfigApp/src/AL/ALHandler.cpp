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
#include <wx/window.h>
#include <wx/choice.h>
#include <alc.h>
#include "AppForms.h"
#include "AL/ALHandler.h"

using namespace std;

namespace ConfigApplication {
    ALHandler::ALHandler() {
        size_t offset = 0;
        const  ALCchar *devices,*defaultDev,*devName;

        // Check if we can enumerate available audio output devices
        if(!alcIsExtensionPresent(NULL,"ALC_ENUMERATION_EXT"))
            throw wxT("OpenAL's device enumeration extension is not present");

        // Get device list and default device
        devices    = alcGetString(NULL,ALC_DEVICE_SPECIFIER);
        defaultDev = alcGetString(NULL,ALC_DEFAULT_DEVICE_SPECIFIER);

        // Parse device list
        do {
            // Skips offsets already read
            devName = devices+offset;

            // Increments offset
            offset += strlen(devName)+1;

            // The list is ended with a 0-length string
            if(strlen(devName) > 0) {
                ostringstream str;

                // Write the device name, and append "(Default)" on its
                // side if it is the default device
                str << devName;
                if(strcmp(devName,defaultDev) == 0)
                    str << " (Default)";

                // Subscribe this device in our vector
                mDevices.push_back(str.str());
            }
        } while(strlen(devName) > 0);
    }

    void ALHandler::fillWindow(ConfigWindow *wnd) {
        wxString  previousSelection;
        wxChoice *devices = static_cast<wxChoice *>(wnd->FindWindow(ConfigWindow::ID_OAL_DEVICE));

        // Backup old selection, clear devices' choicebox
        // and append a very, very portable choice
        previousSelection = devices->GetStringSelection();
        devices->Clear();
        devices->Append("None");

        // Fill devices' choicebox with available audio devices
        for(size_t i = 0;i < mDevices.size();++i)
            devices->Append(mDevices[i]);

        // Restore previous selection if available, or selects
        // the default one if not
        if(previousSelection == wxT("")) {
            if(devices->GetCount() > 1) {
                devices->SetSelection(1); // Select first device
            } else {
                devices->SetSelection(0); // Select "None"
            }
        } else {
            if(devices->FindString(previousSelection) != wxNOT_FOUND) {
                devices->SetStringSelection(previousSelection);
            } else {
                if(devices->GetCount() > 1) {
                    devices->SetSelection(1); // Select first device
                } else {
                    devices->SetSelection(0); // Select "None"
                }
            }
        }
    }
} // namespace
