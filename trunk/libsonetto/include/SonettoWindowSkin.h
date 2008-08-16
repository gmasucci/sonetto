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
#ifndef _SONETTO_WINDOWSKIN_H_
#define _SONETTO_WINDOWSKIN_H_

#include "SonettoMain.h"

#include <OGRE/OgreResourceManager.h>
#include <OGRE/OgreMaterialManager.h>
#include <OGRE/OgreTextureManager.h>
#include <OGRE/OgreTechnique.h>
#include "SonettoMain.h"

namespace Sonetto {

    class SONETTO_EXPORT WindowSkin : public Ogre::Resource {
    public:
        /// @brief Constructor.
        WindowSkin(Ogre::ResourceManager *creator, const Ogre::String &name,
                   Ogre::ResourceHandle handle, const Ogre::String &group, bool isManual = false,
                   Ogre::ManualResourceLoader *loader = 0);
        /// @brief Destructor.
        virtual ~WindowSkin();

    //protected: Let's just keep things simple for now.

        // must implement these from the Ogre::Resource interface
        void loadImpl();
        void unloadImpl();
        size_t calculateSize() const;

        bool createMaterial();
        bool deleteMaterial();

        Ogre::String        mWindowMaskTextureName;
        Ogre::String        mWindowBgTextureName;
        Ogre::String        mBorderTextureName;

        // Materials
        Ogre::MaterialPtr   mWindowMaterial;
        Ogre::MaterialPtr   mBorderMaterial;

        WindowTexCoord 		mWinTexCoord;
        Ogre::Vector2 		mBorderDimensions;
        bool                mTileMode;
        Ogre::Vector2       mTiling;
        // System Colors
        Ogre::ColourValue	mMenuColor;
        Ogre::ColourValue   mMenuBorderColor;

        Ogre::ColourValue	mMainColor;
        Ogre::ColourValue   mMainBorderColor;

        Ogre::ColourValue	mEnemyColor;
        Ogre::ColourValue   mEnemyBorderColor;

        Ogre::ColourValue	mSystemColor;
        Ogre::ColourValue   mSystemBorderColor;

        // Menu Background
        Ogre::MaterialPtr   mMenuBgMaterial;
        Ogre::ColourValue   mMenuBgTopLeft;
        Ogre::ColourValue   mMenuBgTopRight;
        Ogre::ColourValue   mMenuBgBottomLeft;
        Ogre::ColourValue   mMenuBgBottomRight;

        // Cursor Line
        Ogre::MaterialPtr   mCursorLineMaterial;
        Ogre::ColourValue   mCursorLineColor;
    };

    class SONETTO_EXPORT WindowSkinPtr : public Ogre::SharedPtr<WindowSkin> {
    public:
        WindowSkinPtr() : Ogre::SharedPtr<WindowSkin>() {}
        explicit WindowSkinPtr(WindowSkin *rep) : Ogre::SharedPtr<WindowSkin>(rep) {}
        WindowSkinPtr(const WindowSkinPtr &r) : Ogre::SharedPtr<WindowSkin>(r) {}
        WindowSkinPtr(const Ogre::ResourcePtr &r) : Ogre::SharedPtr<WindowSkin>() {
            // lock & copy other mutex pointer
            OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
            OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
            pRep = static_cast<WindowSkin*>(r.getPointer());
            pUseCount = r.useCountPointer();
            if (pUseCount) {
                ++(*pUseCount);
            }
        }

        /// @brief Operator used to convert a Ogre::ResourcePtr into a WindowSkinPtr
        WindowSkinPtr& operator=(const Ogre::ResourcePtr& r) {
            if (pRep == static_cast<WindowSkin*>(r.getPointer()))
                return *this;
            release();
            // lock & copy other mutex pointer
            OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
            OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
            pRep = static_cast<WindowSkin*>(r.getPointer());
            pUseCount = r.useCountPointer();
            if (pUseCount) {
                ++(*pUseCount);
            }
            return *this;
        }
    };

}; // namespace

#endif // _SONETTO_WINDOWSKIN_H_
