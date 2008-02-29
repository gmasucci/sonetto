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

#ifndef __OIS_HANDLER_H_
#define __OIS_HANDLER_H_

#include <vector>
#include <wx/window.h>
#include <OIS.h>

namespace ConfigApplication {
    class KeyListener : public OIS::KeyListener {
        public:
            bool keyPressed(const OIS::KeyEvent &evt);
            bool keyReleased(const OIS::KeyEvent &evt) {}
    };

    class JoyStickListener : public OIS::JoyStickListener {
        public:
            bool buttonPressed(const OIS::JoyStickEvent &evt,int btn);
            bool buttonReleased(const OIS::JoyStickEvent &evt,int btn) {}
            bool axisMoved(const OIS::JoyStickEvent &evt,int axis);
            
            std::vector<OIS::JoyStick *> *getJoysVector() { return &mJoys; }
        
        private:
            std::vector<OIS::JoyStick *> mJoys;
    };

    /// Class responsible for quering OIS capabilities over
    /// current hardware and placing options for the end-user
    /// in our ConfigWindow
    class OISHandler {
        public:
            /** @brief Initialises OIS and get handles for input devices
             *
             *  This constructor will try to initialise OIS and get
             *  handles for currently plugged in joysticks, as well as
             *  a handle for the keyboard.
             *  @remarks Under Windows, you must supply your wxWindow,
             *           so that can OIS bind the input devices held by us
             *           to our window.
             *           OIS will throw OIS::Exception's on errors.
             */
            OISHandler(wxWindow *wnd = NULL);
            ~OISHandler();

            void fillWindow(wxWindow *wnd);
            
            void update(wxWindow *wnd);

        protected:
            OIS::InputManager            *mInputMan;

            OIS::Keyboard                *mKeyboard;
            std::vector<OIS::JoyStick *>  mJoys;
            
            KeyListener                   mKeyListener;
            JoyStickListener              mJoyListener;
    };
} // namespace

#endif
