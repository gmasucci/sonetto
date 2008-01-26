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
#include "SonettoSTRManager.h"

namespace Sonetto
{
    template<> STRManager *Ogre::Singleton<STRManager>::ms_Singleton = 0;
    STRManager *STRManager::getSingletonPtr()
    {
        return ms_Singleton;
    }
    STRManager &STRManager::getSingleton()
    {
        assert(ms_Singleton);
        return(*ms_Singleton);
    }
    STRManager::STRManager()
    {
        mResourceType = "STR";

        // low, because it will likely reference other resources
        mLoadOrder = 30.0f;

        // Register the ResourceManager with OGRE
        Ogre::ResourceGroupManager::getSingleton()._registerResourceManager(mResourceType, this);
    }
    STRManager::~STRManager()
    {
        // Unregister the ResourceManager from OGRE
        Ogre::ResourceGroupManager::getSingleton()._unregisterResourceManager(mResourceType);
    }
    STRDataPtr STRManager::load(const Ogre::String &name, const Ogre::String &group)
    {
        STRDataPtr strdata = getByName(name);

        if (strdata.isNull())
            strdata = create(name, group);

        strdata->load();
        return strdata;
    }
    Ogre::Resource * STRManager::createImpl(const Ogre::String &name, Ogre::ResourceHandle handle,
                                           const Ogre::String &group, bool isManual, Ogre::ManualResourceLoader *loader,
                                           const Ogre::NameValuePairList *createParams)
    {
        return new STRData(this, name, handle, group, isManual, loader);
    }
} // namespace
