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
#ifndef __D3D_HANDLER_H_
#define __D3D_HANDLER_H_

#include <d3d9.h>
#include <vector>
#include <map>
#include <wx/window.h>

namespace ConfigApplication {
    /// Class responsible for quering Direct3D capabilities over
    /// current hardware and placing options for the end-user
    /// in our ConfigWindow
    class D3DHandler {
        public:
            /** @brief Initialises Direct3D object and enumerates display modes
             *
             *  This constructor creates a Direct3D object and calls enumDisplayModes(),
             *  so that fillWindow() can fill a window with information obtained from here on.
             *  @remarks Direct3D's anti-aliasing levels must be refreshed everytime
             *           there is a resolution change or fullscreen switch, as they depend
             *           on this information (i.e. an anti-aliasing level may be available in
             *           fullscreen mode, while others don't).
             *           Thus, this object should not be deleted after calling
             *           fillWindow() but, instead, should call fillWindow() anytime there is
             *           a resolution or fullscreen change.
             *           Anyway, just do not forget to delete it when it is not needed anymore.
             */
            D3DHandler();

            /// @brief Deletes Direct3D object
            ~D3DHandler();

            /** @brief Fills window's fields
             *
             *  This will fill the Video/Direct3D 9 fields with information
             *  obtained from Direct3D.
             *  @param wnd Configuration window handle.
             */
            void fillWindow(wxWindow *wnd);

        private:
            /// Direct3D object
            LPDIRECT3D9                          mD3D;

            /// Display mode names
            std::map<std::string,D3DDISPLAYMODE> mDisplayModes;

            /// Fullscreen Anti-aliasing options
            std::vector<std::string>             mFSAALevels;

            /// Gets information about current hardware Direct3D support,
            /// filling mDisplayModes with such information
            void enumDisplayModes();

            /// Lists supported FSAA levels in supplied
            void refreshFSAALevels(D3DFORMAT format,bool windowed);
    };
} // namespace

#endif // __D3DHANDLER_H_
#endif // CA_USE_DIRECT3D
