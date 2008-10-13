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
#include "SonettoWalkmeshManager.h"
#include "TESTMapFileManager.h"

namespace Sonetto {

    struct MapLayerFile;

    struct MODULE_EXPORT MapLayer
    {
        Ogre::String mLayerName;

        Ogre::String mParentLayerName;

        /// This Layer Model.
        Ogre::Entity * mModel;

        /// The Layer Model Node.
        Ogre::SceneNode * mNode;

        /// The Node parent.
        Ogre::SceneNode * mParentNode;

        /*
            0x01 - Animate Object (Bone)
            0x02 - Loop Animation
            0x04 - Use Rotation Animation
            0x08 - Cast Shadows

            0x10 - Override Mesh Material
        */
        Ogre::uint16 mLayerSettingsFlags;

        Ogre::String mAnimationName;

        Ogre::Vector3 mAnimRotation;

        Ogre::Real mAnimationSpeed;

        Ogre::String mMaterialName;
    };

    struct MapRTT
    {
        Ogre::TexturePtr mTexture;
        Ogre::RenderTexture  * mRTT;
        Ogre::Camera * mCamera;
        Ogre::ColourValue mBackgroundColor;
        MapRTTBehavior mRTTBehavior;
        std::vector<Ogre::String> mExcludeObjects;
        std::vector<Ogre::String> mIncludeObjects;
        Ogre::Vector3 mCameraPositionOffset;
        Ogre::Vector4 mCameraOrientationOffset;
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
        void changeMap();

    protected:
        Ogre::Vector3 applyPerspective(const Ogre::Vector3 &aVector,
                const Ogre::Vector3 &perspective);

        WalkmeshManager *mWalkmeshMan;

        Ogre::Radian mAngle;
        bool mResourceGroupCreated;
        size_t mCurrentMapID;
        Ogre::String mCurrentMapName;
        Ogre::String mResGroupName;
        MapFilePtr mMapFile;
        std::vector<MapLayer> mMapLayers;
        std::vector<MapRTT> mMapRTT;

        Ogre::Light * mMainLight0;
        Ogre::Light * mMainLight1;

        // Temporary Dummy Hero
        EventObject *mDummyHero;

        EventVector  mEvents;

        size_t mState;
    };
}; // namespace Sonetto

#endif // TEST_MAPMODULE_H
