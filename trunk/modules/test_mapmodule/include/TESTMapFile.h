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
#ifndef TEST_MAPFILE_H
#define TEST_MAPFILE_H

// If it's the client, then import the library from the dll, export otherwise.
#if defined( WIN32 )
#   if defined( MODULE_DLL )
#        define MODULE_EXPORT __declspec( dllexport )
#   else
#       define MODULE_EXPORT __declspec( dllimport )
#   endif
#endif

#include <OgreResourceManager.h>
#include <OgreColourValue.h>
#include <OgreVector3.h>

namespace Sonetto {
    struct MODULE_EXPORT MapLayerFile
    {
        Ogre::String mLayerModelName;
        Ogre::Real mAnimationSpeed;
    };
    class MODULE_EXPORT MapFile : public Ogre::Resource
    {
    protected:

        void loadImpl();
        void unloadImpl();
        size_t calculateSize() const;

    public:

        MapFile(Ogre::ResourceManager *creator, const Ogre::String &name,
        Ogre::ResourceHandle handle, const Ogre::String &group, bool isManual = false,
        Ogre::ManualResourceLoader *loader = 0);

        virtual ~MapFile();

        // Map File Data
        Ogre::String mMapName;
        Ogre::ColourValue mAmbientColor;
        Ogre::ColourValue mBackgroundColor;
        bool mUseLight0;
        bool mUseLight1;
        Ogre::Vector3 mLightDirection0;
        Ogre::Vector3 mLightDirection1;
        Ogre::ColourValue mLightColor0;
        Ogre::ColourValue mLightColor1;
        Ogre::FogMode mFogMode;
        Ogre::ColourValue mFogColor;
        Ogre::Real mFogExpDensity;
        Ogre::Real mFogStart;
        Ogre::Real mFogEnd;
        std::vector<MapLayerFile> mMapLayer;

    };

    class MODULE_EXPORT MapFilePtr : public Ogre::SharedPtr<MapFile>
    {
        public:
        MapFilePtr() : Ogre::SharedPtr<MapFile>() {}
        explicit MapFilePtr(MapFile *rep) : Ogre::SharedPtr<MapFile>(rep) {}
        MapFilePtr(const MapFilePtr &r) : Ogre::SharedPtr<MapFile>(r) {}
        MapFilePtr(const Ogre::ResourcePtr &r) : Ogre::SharedPtr<MapFile>()
        {
            // lock & copy other mutex pointer
            OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
                OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
                pRep = static_cast<MapFile*>(r.getPointer());
            pUseCount = r.useCountPointer();
            if (pUseCount)
            {
                ++(*pUseCount);
            }
        }

        /// Operator used to convert a ResourcePtr to a MapFilePtr
        MapFilePtr& operator=(const Ogre::ResourcePtr& r)
        {
            if (pRep == static_cast<MapFile*>(r.getPointer()))
                return *this;
            release();
            // lock & copy other mutex pointer
            OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
                OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
                pRep = static_cast<MapFile*>(r.getPointer());
            pUseCount = r.useCountPointer();
            if (pUseCount)
            {
                ++(*pUseCount);
            }
            return *this;
        }
    };

}; // namespace

#endif // TEST_MAPFILE_H
