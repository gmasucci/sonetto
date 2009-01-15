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

#include "SonettoFontManager.h"

namespace Sonetto
{
    // ----------------------------------------------------------------------
    template<> FontManager *Ogre::Singleton<FontManager>::ms_Singleton = 0;
    // ----------------------------------------------------------------------
    FontManager *FontManager::getSingletonPtr()
    {
        return ms_Singleton;
    }
    // ----------------------------------------------------------------------
    FontManager &FontManager::getSingleton()
    {
        assert(ms_Singleton);
        return(*ms_Singleton);
    }
    // ----------------------------------------------------------------------
    FontManager::FontManager()
    {
        mResourceType = "SFont";

        // low, because it will likely reference other resources
        mLoadOrder = 30.0f;

        // Register the resorce manager with Ogre.
        Ogre::ResourceGroupManager::getSingleton()._registerResourceManager(mResourceType, this);
    }
    // ----------------------------------------------------------------------
    FontManager::~FontManager()
    {
        // unregister the resource manager.
        Ogre::ResourceGroupManager::getSingleton()._unregisterResourceManager(mResourceType);
    }
    // ----------------------------------------------------------------------
    FontPtr FontManager::load(const Ogre::String &name, const Ogre::String &group)
    {
        FontPtr fontf = getByName(name);

        if (fontf.isNull())
            fontf = create(name, group);

        fontf->load();
        return fontf;
    }
    // ----------------------------------------------------------------------
    Ogre::Resource *FontManager::createImpl(const Ogre::String &name, Ogre::ResourceHandle handle,
                                                const Ogre::String &group, bool isManual, Ogre::ManualResourceLoader *loader,
                                                const Ogre::NameValuePairList *createParams)
    {
        return new Font(this, name, handle, group, isManual, loader);
    }
    // ----------------------------------------------------------------------
} // namespace
