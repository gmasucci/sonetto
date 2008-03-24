/*-----------------------------------------------------------------------------
This source file is part of Sonetto RPG Engine.

Copyright (C) 2007,2008 Arthur Carvalho de Souza Lima, Guilherme Pr� Vieira


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
#ifndef FONTMANAGER_H_INCLUDED
#define FONTMANAGER_H_INCLUDED

#include <OgreResourceManager.h>
#include "SonettoFont.h"

namespace Sonetto {
    class FontManager : public Ogre::ResourceManager, public Ogre::Singleton<FontManager>
    {
    protected:

        // must implement this from ResourceManager's interface
        Ogre::Resource *createImpl(const Ogre::String &name, Ogre::ResourceHandle handle,
                                   const Ogre::String &group, bool isManual, Ogre::ManualResourceLoader *loader,
                                   const Ogre::NameValuePairList *createParams);

    public:

        FontManager();
        virtual ~FontManager();

        virtual FontPtr load(const Ogre::String &name, const Ogre::String &group);

        static FontManager &getSingleton();
        static FontManager *getSingletonPtr();
    };

} // namespace
#endif // FONTMANAGER_H_INCLUDED
