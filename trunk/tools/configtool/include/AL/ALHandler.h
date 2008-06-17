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

#ifndef __AL_HANDLER_H_
#define __AL_HANDLER_H_

#include <string>
#include <vector>
#include "AppForms.h"

namespace ConfigApplication {
    /// Class responsible for quering OpenAL capabilities over
    /// current hardware and placing options for the end-user
    /// in our ConfigWindow
    class ALHandler {
        public:
            /** @brief Initialises and gets information about OpenAL devices
             *
             *  Everything this constructor does is query for OpenAL available
             *  audio devices, to be afterwards filled in the Audio/Audio Devices
             *  choicebox using fillWindow().
             */
            ALHandler();

            /** @brief Fills window's fields
             *
             *  This will fill the Audio/Audio Devices choicebox with information
             *  obtained from OpenAL.
             *  @param wnd Configuration window handle.
             */
            void fillWindow(ConfigWindow *wnd);

        private:
            /// Device name vector
            std::vector<std::string> mDevices;
    };
} // namespace

#endif
