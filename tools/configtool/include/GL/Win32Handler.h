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
#ifndef __GL_HANDLER_H_
#define __GL_HANDLER_H_

#include <vector>
#include <gl/gl.h>
#include <gl/glu.h>
#include <wx/window.h>

namespace ConfigApplication {
    /// Class responsible for quering OpenGL capabilities over
    /// current hardware and placing options for the end-user
    /// in our ConfigWindow
    class GLHandler {
        public:
            /** @brief Lists actual hardware capabilities
             *
             *  The GLHandler constructor will simply query OpenGL for
             *  supported resolutions, color depths, available monitor
             *  frequencies, etc. It will silently create a dummy window
             *  for listing the supported FSAA levels, but such window
             *  wont appear, so don't care about it.
             */
            GLHandler();

            /// A do-nothing window procedure function for a do-nothing window :-)
            static LRESULT CALLBACK GLHandler::dummyWndProc(HWND hwnd,UINT umsg,WPARAM wp,LPARAM lp);

            /** @brief Fills window's fields
             *
             *  This will fill the Video/OpenGL fields with information
             *  obtained from OpenGL.
             *  @param wnd Configuration window handle.
             */
            void fillWindow(wxWindow *wnd);

        private:
            /// Available resolutions
            std::vector<std::string> mResolutions;

            /// Available color depths
            std::vector<int>         mColorDepths;

            /// Available frequencies
            std::vector<int>         mFrequencies;

            /// Available FSAA levels
            std::vector<int>         mFSAALevels;
    };
} // namespace

#endif // __GL_HANDLER_H_
#endif // CA_USE_OPENGL
