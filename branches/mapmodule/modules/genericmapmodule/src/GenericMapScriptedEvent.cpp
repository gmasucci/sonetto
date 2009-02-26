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

#include <SonettoKernel.h>
#include <SonettoScriptManager.h>
#include "GenericMapScriptedEvent.h"
#include "GenericMapEventScript.h"

namespace GenericMapModule
{
    // ----------------------------------------------------------------------
    // GenericMapModule::ScriptedEvent implementation.
    // ----------------------------------------------------------------------
    void ScriptedEvent::update()
    {
        Sonetto::ScriptManager *scriptMan =
                Sonetto::ScriptManager::getSingletonPtr();

        updatePage();

        if (!mCurPage)
        {
            // No page selected; stop updating
            return;
        }

        if (scriptMan->updateScript(mCurScript))
        {
            // If the page's trigger condition was TRG_AUTORUN,
            // tag it as executed
            if (mCurPage->triggerCondition == ScriptedEventPage::TRG_AUTORUN)
            {
                mCurPage->autorun.executed = true;
            }

            // Script has ended; reselect page
            setPage(selectPage());
        }
    }
    // ----------------------------------------------------------------------
    bool ScriptedEvent::getBlockEnabled()
    {
        if (mBlockEnabled)
        {
            // Event manually set to block
            return true;
        }

        // Makes sure a page is selected if its conditions are met
        updatePage();

        if (!mCurPage)
        {
            // No page was selected, don't block
            return false;
        }

        if (mCurPage->triggerCondition == ScriptedEventPage::TRG_AUTORUN &&
            mCurPage->autorun.blockEnabled && !mCurPage->autorun.executed)
        {
            // Blocking autorun set and not yet finished executing; block
            return true;
        }

        return false;
    }
    // ----------------------------------------------------------------------
    ScriptedEventPage *ScriptedEvent::selectPage()
    {
        ScriptedEventPageVector::iterator i;
        for (i = mPages.begin();i != mPages.end();++i)
        {
            ScriptedEventPage &page = *i;

            switch (page.triggerCondition)
            {
                case ScriptedEventPage::TRG_BUTTON:
                    // Breaks for loop
                    i = mPages.end();
                    continue;
                break;

                case ScriptedEventPage::TRG_EVENT_TOUCH:
                    // Breaks for loop
                    i = mPages.end();
                    continue;
                break;

                case ScriptedEventPage::TRG_AUTORUN:
                    if (page.autorun.executed)
                    {
                        // Breaks for loop
                        i = mPages.end();
                        continue;
                    }
                break;

                case ScriptedEventPage::TRG_PARALLEL_PROCESS:
                break;
            }

            Sonetto::VariableConditionVector::iterator j;
            for (j = page.conditions.begin();
                 j != page.conditions.end();++j)
            {
                Sonetto::VariableCondition &condition = *j;
                Sonetto::VariableMap::iterator vIter,vEnd;
                Sonetto::Variable lhs;

                switch (condition.scope)
                {
                    case Sonetto::VS_LOCAL:
                        vIter = mLocals.find(condition.variableID);
                        vEnd = mLocals.end();
                    break;

                    case Sonetto::VS_GLOBAL:
                    {
                        Sonetto::VariableMap &globals =
                                Sonetto::Database::getSingleton().
                                savemap.variables;

                        vIter = globals.find(condition.variableID);
                        vEnd = globals.end();
                    }
                    break;
                }

                // If the variable was found, get it
                // (otherwise it's [VT_INT32,0])
                if (vIter != vEnd)
                {
                    lhs = vIter->second;
                }

                if (!lhs.compare((Sonetto::VariableComparator)(condition.
                        comparator),condition.rhsValue))
                {
                    // Comparison failed; skip this page
                    j = page.conditions.end();
                    i = mPages.end();
                    continue;
                }
            }

            if (i != mPages.end())
            {
                // This page has all conditions met; select it
                return &page;
            }
        }

        // No page has all conditions met
        return NULL;
    }
    // ----------------------------------------------------------------------
    void ScriptedEvent::setPage(ScriptedEventPage *page)
    {
        // Sets current page pointer
        mCurPage = page;

        if (page) {
            // Updates entity mesh
            setEntityMesh(page->meshSource,page->meshID);

            // Updates script
            setCurrentScript(page->scriptFile);

            // Updates walk speed
            mWalkSpeed = page->walkSpeed;
        } else { // If page is NULL, clears event
            mCurPage = NULL;

            // Destroys entity
            setEntityMesh(MES_NONE,0);

            // Destroys script
            setCurrentScript(Sonetto::ScriptFilePtr());

            // Resets walk speed
            mWalkSpeed = 1.0f;
        }
    }
    // ----------------------------------------------------------------------
    void ScriptedEvent::updatePage()
    {
        if (!mCurPage)
        {
            if (mPages.empty())
            {
                SONETTO_THROW("ScriptedEvent has no pages");
            }

            setPage(selectPage());
        }
    }
    // ----------------------------------------------------------------------
    void ScriptedEvent::setCurrentScript(Sonetto::ScriptFilePtr scriptFile)
    {
        Sonetto::ScriptManager *scriptMan =
                Sonetto::ScriptManager::getSingletonPtr();

        if (mCurScript) {
            if (mCurScript->getScriptFile() != scriptFile)
            {
                scriptMan->destroyScript(mCurScript);

                if (!scriptFile.isNull()) {
                    mCurScript = scriptMan->
                        createScript<EventScript>(scriptFile->getName(),
                        "MAP_LOCAL");

                    mCurScript->setOwner(this);
                } else {
                    mCurScript = NULL;
                }
            }
        } else {
            if (!scriptFile.isNull()) {
                mCurScript = scriptMan->
                    createScript<EventScript>(scriptFile->getName(),
                    "MAP_LOCAL");

                mCurScript->setOwner(this);
            }
        }
    }
} // namespace
