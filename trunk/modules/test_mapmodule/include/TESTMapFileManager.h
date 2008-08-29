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
#ifndef TEST_MAPFILEMANAGER_H
#define TEST_MAPFILEMANAGER_H

// If it's the client, then import the library from the dll, export otherwise.
#if defined( WIN32 )
#   if defined( MODULE_DLL )
#        define MODULE_EXPORT __declspec( dllexport )
#   else
#       define MODULE_EXPORT __declspec( dllimport )
#   endif
#endif

#include <OgreResourceManager.h>
#include <OgreSingleton.h>
#include <OgreString.h>
//#include <Ogre
#include "TESTMapFile.h"

namespace Sonetto{

    class MODULE_EXPORT MapFileManager : public Ogre::ResourceManager, public Ogre::Singleton<MapFileManager>
    {
    protected:
        Ogre::Resource *createImpl(const Ogre::String& name, Ogre::ResourceHandle handle,
			const Ogre::String& group, bool isManual, Ogre::ManualResourceLoader* loader,
            const Ogre::NameValuePairList* createParams);
    public:
        MapFileManager();
        virtual ~MapFileManager();

        virtual MapFilePtr load(const Ogre::String &name, const Ogre::String &group);

        static MapFileManager &getSingleton();
        static MapFileManager *getSingletonPtr();

    };

}; // namespace.

#endif // TEST_MAPFILEMANAGER_H
