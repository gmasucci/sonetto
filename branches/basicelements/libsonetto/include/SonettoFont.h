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

#ifndef SONETTO_FONT_H
#define SONETTO_FONT_H

#include "SonettoPrerequisites.h"

#include <OgreResourceManager.h>
#include <OgreVector2.h>
#include <OgreMaterial.h>
#include <OgreTexture.h>

namespace Sonetto
{
    // Font Structures
    /// Information about the font's current version.
    enum FontVersion
    {
        SFV_NONE,
        SFV_VER_1_0
    };

    /// Font Supported Encoding.
    enum FontEncode
    {
        FE_UNKNOWN,
        FE_ASCII
    };

    /// Glyph Structure.
    struct FontGlyph
    {
        Ogre::Vector2 mTopLeft;
        Ogre::Vector2 mTopRight;
        Ogre::Vector2 mBottomLeft;
        Ogre::Vector2 mBottomRight;
        float mLeftOffset;
        float mRightOffset;
    };

    /** Sonetto Texture Font. */
    class SONETTO_API Font : public Ogre::Resource
    {
    protected:

        /// Load Implementation.
        void loadImpl();
        /// Unload Implementation.
        void unloadImpl();
        /// Calculates file size.
        size_t calculateSize() const;

    public:

        /// Constructor.
        Font(   Ogre::ResourceManager *creator, const Ogre::String &name,
                Ogre::ResourceHandle handle, const Ogre::String &group, bool isManual = false,
                Ogre::ManualResourceLoader *loader = 0);
        /// Destructor.
        virtual ~Font();

        static uint32 mFourCC;

        std::string mName;
        /// Font File Version.
        uint32 mVersion;
        /// Font File Encode.
        uint32 mEncode;
        /// Global Glyph Top Offset.
        float mVerticalOffsetTop;
        /// Global Glyph Bottom Offset.
        float mVerticalOffsetBottom;
        /// Global Glyph Horizontal Scale.
        float mHorizontalScale;
        /// Font Material.
        Ogre::MaterialPtr mMaterial;
        /// Font Color List.
        std::vector<Ogre::ColourValue> mColorList;
        /// Glyph Data.
        std::vector<FontGlyph> mGlyph;
        /// Texture Data.
        Ogre::Image * mFontImage;

    };

    class SONETTO_API FontPtr : public Ogre::SharedPtr<Font>
    {
    public:
        FontPtr() : Ogre::SharedPtr<Font>() {}
        explicit FontPtr(Font *rep) : Ogre::SharedPtr<Font>(rep) {}
        FontPtr(const FontPtr &r) : Ogre::SharedPtr<Font>(r) {}
        FontPtr(const Ogre::ResourcePtr &r) : Ogre::SharedPtr<Font>()
        {
            // lock & copy other mutex pointer
            OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
                OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
                pRep = static_cast<Font*>(r.getPointer());
            pUseCount = r.useCountPointer();
            if (pUseCount)
            {
                ++(*pUseCount);
            }
        }

        /// Operator used to convert a ResourcePtr to a FontPtr
        FontPtr& operator=(const Ogre::ResourcePtr& r)
        {
            if (pRep == static_cast<Font*>(r.getPointer()))
                return *this;
            release();
            // lock & copy other mutex pointer
            OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
                OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
                pRep = static_cast<Font*>(r.getPointer());
            pUseCount = r.useCountPointer();
            if (pUseCount)
            {
                ++(*pUseCount);
            }
            return *this;
        }
    };

} // namespace

#endif // SONETTO_FONT_H
