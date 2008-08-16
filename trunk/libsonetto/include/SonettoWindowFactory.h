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
#ifndef SONETTOWINDOWFACTORY_H_INCLUDED
#define SONETTOWINDOWFACTORY_H_INCLUDED

#include "SonettoMain.h"

#include <OGRE/OgreOverlayElementFactory.h>
#include "SonettoWindow.h"
#include "SonettoTailedWindow.h"
#include "SonettoSlimWindow.h"

namespace Sonetto
{
    using namespace Ogre;
    class SONETTO_EXPORT WindowFactory: public OverlayElementFactory
    {
    public:
        /// @brief See OverlayElementFactory
        OverlayElement* createOverlayElement(const String& instanceName)
        {
            return new Sonetto::Window(instanceName);
        }
        /// @brief See OverlayElementFactory
        const String& getTypeName(void) const
        {
            static String name = "Window";
            return name;
        }
    };

    class SONETTO_EXPORT TailedWindowFactory: public OverlayElementFactory
    {
    public:
        /// @brief See OverlayElementFactory
        OverlayElement* createOverlayElement(const String& instanceName)
        {
            return new Sonetto::TailedWindow(instanceName);
        }
        /// @brief See OverlayElementFactory
        const String& getTypeName(void) const
        {
            static String name = "TailedWindow";
            return name;
        }
    };

    class SONETTO_EXPORT SlimWindowFactory: public OverlayElementFactory
    {
    public:
        /// @brief See OverlayElementFactory
        OverlayElement* createOverlayElement(const String& instanceName)
        {
            return new Sonetto::SlimWindow(instanceName);
        }
        /// @brief See OverlayElementFactory
        const String& getTypeName(void) const
        {
            static String name = "SlimWindow";
            return name;
        }
    };
} // namespace

#endif // SONETTOWINDOWFACTORY_H_INCLUDED
