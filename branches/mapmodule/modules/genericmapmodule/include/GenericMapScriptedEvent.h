/*-----------------------------------------------------------------------------
Copyright (c) 2009, Sonetto Project Developers
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1.  Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
2.  Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
3.  Neither the name of the Sonetto Project nor the names of its contributors
    may be used to endorse or promote products derived from this software
    without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
-----------------------------------------------------------------------------*/

#ifndef GENERICMAPMODULE_SCRIPTEDEVENT_H
#define GENERICMAPMODULE_SCRIPTEDEVENT_H

#include "GenericMapPrerequisites.h"
#include "GenericMapEvent.h"
#include "GenericMapScriptedEventPage.h"

namespace GenericMapModule
{
    class ScriptedEvent : public Event
    {
    public:
        ScriptedEvent(size_t id,
                const Ogre::Vector3 &position = Ogre::Vector3::ZERO,
                const Ogre::Quaternion &rotation = Ogre::Quaternion::IDENTITY)
                : Event(id,position,rotation),mCurPage(NULL) {}

        virtual ~ScriptedEvent() {}

        virtual ScriptedEventPageVector &_getPages() { return mPages; }
        virtual inline const ScriptedEventPage *getCurrentPage() const
                { return mCurPage; }

        virtual void update();

        virtual inline ScriptedEventPage *getCurrentPage()
        {
            updatePage();
            return mCurPage;
        }

        virtual bool getBlockEnabled();

    protected:
        ScriptedEventPage *selectPage();
        void setPage(ScriptedEventPage *page);
        void updatePage();

        void setCurrentScript(Sonetto::ScriptFilePtr scriptFile);

        Sonetto::VariableMap mLocals;

        ScriptedEventPageVector mPages;
        ScriptedEventPage *mCurPage;

        MeshSource mCurMeshSource;

        EventScript *mCurScript;
    };
} // namespace

#endif
