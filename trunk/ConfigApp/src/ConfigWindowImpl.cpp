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

#include <wx/app.h>
#include <wx/timer.h>
#include "ConfigApp.h"
#include "ConfigWindowImpl.h"

namespace ConfigApplication {
    BEGIN_EVENT_TABLE(ConfigWindowImpl,ConfigWindow)
        EVT_TIMER(-1,ConfigWindowImpl::onTimerEvent)
    END_EVENT_TABLE()
    
    ConfigWindowImpl::ConfigWindowImpl(wxWindow* parent,ConfigApp *app) :
            ConfigWindow(parent) {
        mApp = app;
        
        mTimer = new wxTimer(this);
        mTimer->Start(32);
    }

    void ConfigWindowImpl::onRsysChange( wxChoicebookEvent& event ) {
        // TODO: Implement onRsysChange
    }

    void ConfigWindowImpl::onRsysChanging( wxChoicebookEvent& event ) {
        // TODO: Implement onRsysChanging
    }

    void ConfigWindowImpl::onChoiceD3D9Res( wxCommandEvent& event ) {
        // Re-fill window
        // Needed by FSAA level enumeration
        mApp->getD3DHandler()->fillWindow(this);
    }

    void ConfigWindowImpl::onChoiceD3D9AA( wxCommandEvent& event ) {
        // TODO: Implement onChoiceD3D9AA
    }

    void ConfigWindowImpl::onCheckD3D9FullScreen( wxCommandEvent& event ) {
        // Re-fill window
        // Needed by FSAA level enumeration
        mApp->getD3DHandler()->fillWindow(this);
    }

    void ConfigWindowImpl::onCheckD3D9VSync( wxCommandEvent& event ) {
        // TODO: Implement onCheckD3D9VSync
    }

    void ConfigWindowImpl::onChoiceOGLRes( wxCommandEvent& event ) {
        // TODO: Implement onChoiceOGLRes
    }

    void ConfigWindowImpl::onChoiceOGLColDepth( wxCommandEvent& event ) {
        // TODO: Implement onChoiceOGLColDepth
    }

    void ConfigWindowImpl::onChoiceOGLFreq( wxCommandEvent& event ) {
        // TODO: Implement onChoiceOGLFreq
    }

    void ConfigWindowImpl::onChoiceOGLFSAA( wxCommandEvent& event ) {
        // TODO: Implement onChoiceOGLFSAA
    }

    void ConfigWindowImpl::onCheckOGLFullScreen( wxCommandEvent& event ) {
        // TODO: Implement onCheckOGLFullScreen
    }

    void ConfigWindowImpl::onCheckOGLVSync( wxCommandEvent& event ) {
        // TODO: Implement onCheckOGLVSync
    }
    
    void ConfigWindowImpl::onTimerEvent( wxTimerEvent& event ) {
        mApp->getOISHandler()->update(this);
    }

    void ConfigWindowImpl::OnClickApply( wxCommandEvent& event ) {
        // TODO: Implement OnClickApply
    }

    void ConfigWindowImpl::OnClickCancel( wxCommandEvent& event ) {
        // Destroys window
        // Even after a call for wxWindow::Destroy(), however, one cannot
        // ensure whether the window have really closed. Frames and dialogs
        // are queued for destruction by wxWidgets, and will be so when
        // every of its pending events have been processed.
        if (!Destroy())
            printf("Frame queued for destruction.\n");
    }

    void ConfigWindowImpl::OnClickOk( wxCommandEvent& event ) {
        // TODO: Implement OnClickOk
        Close(true);
    }
} // namespace
