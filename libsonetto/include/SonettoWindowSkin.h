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

#ifndef SONETTO_WINDOWSKIN_H
#define SONETTO_WINDOWSKIN_H

#include <OgreResource.h>
#include "SonettoPrerequisites.h"

namespace Sonetto
{
    struct TileCoord
    {
        float topLeftX;
        float topLeftY;
        float topRightX;
        float topRightY;
        float bottomLeftX;
        float bottomLeftY;
        float bottomRightX;
        float bottomRightY;
    };

    struct WindowGeneral
    {
        // Cursor
        // 0x01: Use cursor horizontal animation
        // 0x02: Use cursor vertical animation
        // 0x04: Invert horizontal animation
        // 0x08: Invert hertical animation
        // 0x10: Use cursor trail (ToDo)
        uint32 cursorFlags;
        uint32 cursorMatId;
        TileCoord cursorCoords;
        float cursorColorR;
        float cursorColorG;
        float cursorColorB;
        float cursorColorA;
        float cursorAnimationSpeedX;
        float cursorAnimationSpeedY;
        float cursorAnimationAmplitudeX;
        float cursorAnimationAmplitudeY;
        uint32 cursorTrailNumIterations;
        uint32 cursorTrailMaterial;
        float cursorTrailSpeed;
    };

    struct WindowStyle
    {
        uint32 flags0;          // 0x1: Have Border

        uint32 flags1;          // Reserved

        uint32 materialID;      // Material used in this Window Style

        uint32 texCoordSettings;
        //FFFF - 0: Do not use, 1: Use Coords, 2: Use Stretch, 3: Use Tile
        //|||+-- TexCoord0
        //||+--- TexCoord1
        //|+---- TexCoord2
        //+----- TexCoord3

        float texCoordTileX0;
        float texCoordTileY0;
        float texCoordTileX1;
        float texCoordTileY1;
        float texCoordTileX2;
        float texCoordTileY2;
        float texCoordTileX3;
        float texCoordTileY3;

        TileCoord tileTopLeft;
        TileCoord tileTopCenter;
        TileCoord tileTopRight;
        TileCoord tileMiddleLeft;
        TileCoord tileMiddleCenter;
        TileCoord tileMiddleRight;
        TileCoord tileBottomLeft;
        TileCoord tileBottomCenter;
        TileCoord tileBottomRight;
        // Tailed Window Coords
        TileCoord tileTailTop;
        TileCoord tileTailBottom;
        // Alternative Corner Coords
        TileCoord tileTopLeftB;
        TileCoord tileTopRightB;
        TileCoord tileBottomLeftB;
        TileCoord tileBottomRightB;
        // Slim Windows Tex Coords
        TileCoord tileSlimLeft;
        TileCoord tileSlimCenter;
        TileCoord tileSlimRight;
    };

	class SONETTO_API WindowSkin : public Ogre::Resource
	{
    protected:

        /// Load Implementation.
        void loadImpl();
        /// Unload Implementation.
        void unloadImpl();
        /// Calculates file size.
        size_t calculateSize() const;
	public:
		WindowSkin( Ogre::ResourceManager *creator, const Ogre::String &name,
                    Ogre::ResourceHandle handle, const Ogre::String &group, bool isManual = false,
                    Ogre::ManualResourceLoader *loader = 0);
		virtual ~WindowSkin();

		static uint32 mFourCC;



		std::vector<WindowStyle> mTexCoordSet;

		std::vector<Ogre::MaterialPtr> mMaterial;

		std::vector<Ogre::Image *> mImage;
	};

	class SONETTO_API WindowSkinPtr : public Ogre::SharedPtr<WindowSkin>
    {
    public:
        WindowSkinPtr() : Ogre::SharedPtr<WindowSkin>() {}
        explicit WindowSkinPtr(WindowSkin *rep) : Ogre::SharedPtr<WindowSkin>(rep) {}
        WindowSkinPtr(const WindowSkinPtr &r) : Ogre::SharedPtr<WindowSkin>(r) {}
        WindowSkinPtr(const Ogre::ResourcePtr &r) : Ogre::SharedPtr<WindowSkin>()
        {
            // lock & copy other mutex pointer
            OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
                OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
                pRep = static_cast<WindowSkin*>(r.getPointer());
            pUseCount = r.useCountPointer();
            if (pUseCount)
            {
                ++(*pUseCount);
            }
        }

        /// Operator used to convert a ResourcePtr to a WindowSkinPtr
        WindowSkinPtr& operator=(const Ogre::ResourcePtr& r)
        {
            if (pRep == static_cast<WindowSkin*>(r.getPointer()))
                return *this;
            release();
            // lock & copy other mutex pointer
            OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
                OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
                pRep = static_cast<WindowSkin*>(r.getPointer());
            pUseCount = r.useCountPointer();
            if (pUseCount)
            {
                ++(*pUseCount);
            }
            return *this;
        }
    };
}

#endif // SONETTO_WINDOWSKIN_H
