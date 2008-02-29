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

#ifdef CA_USE_DIRECT3D

#include <sstream>
#include <d3d9.h>
#include <wx/window.h>
#include <wx/choice.h>
#include "AppForms.h"
#include "D3D/D3DHandler.h"

using namespace std;

namespace ConfigApplication {
    D3DHandler::D3DHandler() {
        mD3D = Direct3DCreate9(D3D_SDK_VERSION);
        if (!mD3D)
            throw wxT("Failed initialising Direct3D in D3DHandler::D3DHandler()");

        enumDisplayModes();
    }

    D3DHandler::~D3DHandler() {
        // Should we really be deleting mD3D like this?
        // MSDN don't say a word about it, and on Google nobody
        // seems to care about this object's destruction...
        delete mD3D;
    }

    void D3DHandler::fillWindow(wxWindow *wnd) {
        wxString    previousSelection = wxT("");
        wxChoice   *displayModes      = static_cast<wxChoice   *>(wnd->FindWindow(ConfigWindow::ID_D3D9_RES));
        wxChoice   *fsaaLevels        = static_cast<wxChoice   *>(wnd->FindWindow(ConfigWindow::ID_D3D9_AA));
        wxCheckBox *fullscreen        = static_cast<wxCheckBox *>(wnd->FindWindow(ConfigWindow::ID_D3D9_FSCR));

        // Backup old selection and clean display modes' choicebox
        previousSelection = displayModes->GetStringSelection();
        displayModes->Clear();

        // Fill display modes' choicebox
        for (map<string,D3DDISPLAYMODE>::iterator i = mDisplayModes.begin();
                i != mDisplayModes.end();++i) {
            displayModes->Append(i->first);
        }

        // If we hadn't have anything selected yet, defaults to first entry
        // Else, restores old selection
        if (previousSelection == wxT("")) {
            displayModes->SetSelection(0);
        } else {
            displayModes->SetStringSelection(previousSelection);
        }

        // Get FSAA levels for choosen display mode
        refreshFSAALevels(mDisplayModes[(const char *)(displayModes->GetStringSelection())].Format,
                          fullscreen->IsChecked());

        // Backup old selection, clean FSAA levels' choicebox,
        // and append default value
        previousSelection = fsaaLevels->GetStringSelection();
        fsaaLevels->Clear();
        fsaaLevels->Append("None");

        // Fill FSAA levels' choicebox
        for (size_t i = 0;i < mFSAALevels.size();++i)
            fsaaLevels->Append(mFSAALevels[i]);

        if (previousSelection == wxT("")) {
            fsaaLevels->SetSelection(0);
        } else {
            fsaaLevels->SetStringSelection(previousSelection);
        }
    }

    void D3DHandler::enumDisplayModes() {
        // Get Video Modes for D3DFMT_R5G6B5(16bits) and D3DFMT_X8R8G8B8(32bits)
        for (size_t i = 1;i <= 2;++i) {
            D3DFORMAT format = (i == 1) ? D3DFMT_R5G6B5 : D3DFMT_X8R8G8B8;

            for (size_t j = 0;j < mD3D->GetAdapterModeCount(D3DADAPTER_DEFAULT,format);++j) {
                ostringstream  str;
                D3DDISPLAYMODE displayMode;

                // Pick adapter number 'j' from the adapters' list and describe it in displayMode
                mD3D->EnumAdapterModes(D3DADAPTER_DEFAULT,format,j,&displayMode);

                // Skip low resolutions (640x480 or less)
                if (displayMode.Height < 480)
                    continue;

                // i*16
                // If i == 1, the result will be 16 (bits)
                // If i == 2, the result will be 32 (bits)
                str << displayMode.Width << "x" << displayMode.Height << " at " << displayMode.RefreshRate;
                str << " Hz, " << i*16 << "bpp";
                mDisplayModes[str.str()] = displayMode;
            }
        }
    }

    void D3DHandler::refreshFSAALevels(D3DFORMAT format,bool windowed) {
        DWORD quality;

        // Clean FSAA levels' vector
        mFSAALevels.clear();

        // Check for Non-Maskable multisample levels support
        if (SUCCEEDED(mD3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,
                                                       format,windowed,D3DMULTISAMPLE_NONMASKABLE,&quality))) {
            if (quality > 0) {
                for (size_t i = 0;i < quality;++i) {
                    ostringstream str;

                    str << "Non-Maskable - Level " << i+1;
                    mFSAALevels.push_back(str.str());
                }
            }
        }

        // Check for Maskable multisample levels supported
        for (size_t i = 2;i <= 16;++i) {
            if (mD3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,
                                                 format,windowed,(D3DMULTISAMPLE_TYPE)i,NULL) == D3D_OK) {
                ostringstream str;

                str << "Level " << i;
                mFSAALevels.push_back(str.str());
            }
        }
    }
} // namespace

#endif // CA_USE_DIRECT3D
