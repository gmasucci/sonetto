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
#ifndef TEST_MAPMODULE_H
#define TEST_MAPMODULE_H

// If it's the client, then import the library from the dll, export otherwise.
#if defined( WIN32 )
#   if defined( MODULE_DLL )
#        define MODULE_EXPORT __declspec( dllexport )
#   else
#       define MODULE_EXPORT __declspec( dllimport )
#   endif
#endif

#include "SonettoModule.h"
#include "TESTMapFileManager.h"

namespace Sonetto {

    struct MapLayerFile;

    struct MODULE_EXPORT MapLayer
    {
        /// This Layer Model.
        Ogre::Entity * mModel;

        /// The Layer Model Node.
        Ogre::SceneNode * mNode;

        /// The Node parent.
        Ogre::SceneNode * mParentNode;

        /// Animation speed (zero equals to no animation).
        Ogre::Real mAnimationSpeed;

    };

    /** Description coming soon...
    */
    class MODULE_EXPORT TestMapModule : public Module
    {
    public:
        /// Default Constructor.
        TestMapModule();
        /// Default Destructor.
        ~TestMapModule();

        void enter();

        void update(Ogre::Real deltatime);

        void exit();

        void halt();

        void wakeup();

        protected:
        // Map Module internal functions.
        /** Upon calling this function, a new map will be loaded,
            and the current active map will be discarded.
            Do not call this function directly!
            This function reads from a certain location in the memory,
            so if you call this function, it will load the map regardless
            of what's hapenning and will discard everything, even if it's the same map.
        */
        void changeMap();

        void loadMapFile(std::string filename);

        void cleanMap();

    protected:
        MapFilePtr mMapFile;
        std::vector<MapLayer> mMapLayers;
    };
}; // namespace Sonetto

#endif // TEST_MAPMODULE_H
