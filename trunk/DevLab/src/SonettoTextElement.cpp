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

#include <OgreStableHeaders.h>
#include "SonettoTextElement.h"
#include <OgreRoot.h>
#include <OgreLogManager.h>
#include <OgreOverlayManager.h>
#include <OgreHardwareBufferManager.h>
#include <OgreHardwareVertexBuffer.h>
#include <OgreException.h>
#include <OgreStringConverter.h>
#include "SonettoFont.h"
#include "SonettoFontManager.h"
namespace Sonetto
{
    using namespace Ogre;
#define DEFAULT_INITIAL_CHARS 12
    String TextElement::msTypeName = "Text";
    TextElement::TextElement(const String& name)
            : OverlayElement(name)
    {
        mAllocSize = 0;
        mStringSize = 0;
        mStrCursorPosition = 0;
        mTextSize = 0.1f;
        mCurTextColor = ColourValue::White;
        mViewportAspectCoef = 1.0f;
        mIsAnimated = true;
        mUseFadeIn = true;
        mUseFadeOut = true;
        mSkipWhiteSpaces = false;
        mAnimationActive = false;
        mAnimSpeed = 1.0f;
        mDefAnimSpeed = mAnimSpeed;
        mFadeSpeed = 3.0f;
        if (createParamDictionary("Text"))
        {
            addBaseParameters();
        }
    }
    TextElement::~TextElement()
    {
        delete mRenderOp.vertexData;
    }
    void TextElement::initialise(void)
    {
        if (!mInitialised)
        {
            // Set up the render op
            // Combine positions and texture coords since they tend to change together
            // since character sizes are different
            mRenderOp.vertexData = new VertexData();
            VertexDeclaration* decl = mRenderOp.vertexData->vertexDeclaration;
            size_t offset = 0;
            // Positions
            decl->addElement(0, offset, VET_FLOAT3, VES_POSITION);
            offset += VertexElement::getTypeSize(VET_FLOAT3);
            // Texcoords
            decl->addElement(0, offset, VET_FLOAT2, VES_TEXTURE_COORDINATES, 0);
            offset += VertexElement::getTypeSize(VET_FLOAT2);
            // Colours - store in the same buffer
            decl->addElement(1, 0, VET_COLOUR, VES_DIFFUSE);
            mRenderOp.operationType = RenderOperation::OT_TRIANGLE_LIST;
            mRenderOp.useIndexes = false;
            mRenderOp.vertexData->vertexStart = 0;
            // Vertex buffer will be created in checkMemoryAllocation
            checkMemoryAllocation( DEFAULT_INITIAL_CHARS );
            allocateFadeList(DEFAULT_INITIAL_CHARS);
            mInitialised = true;
        }
    }
    void TextElement::setCaption(const std::string& text)
    {
        mCaption = text;
        mStringSize = getStrSize(mCaption, 0);
        checkMemoryAllocation(mStringSize);
        allocateFadeList(mStringSize);
        forceAnimReset();
        mGeomPositionsOutOfDate = true;
        mGeomUVsOutOfDate = true;
    }
    void TextElement::setFont(FontPtr font)
    {
        mFontPtr = font;
        mpMaterial = mFontPtr->font_material;
        mpMaterial->setDepthCheckEnabled(false);
        mpMaterial->setLightingEnabled(false);
    }
    FontPtr TextElement::getFont()
    {
        return mFontPtr;
    }
    void TextElement::setAnimSettings(bool isAnim, bool useFade, bool useFOut, Real anmSpd, Real fdSpd)
    {
        mIsAnimated = isAnim;
        mUseFadeIn = useFade;
        mUseFadeOut = useFOut;
        mAnimSpeed = anmSpd;
        mFadeSpeed = fdSpd;
    }
    void TextElement::setTextSize(Real size)
    {
        mTextSize = size;
    }
    Real TextElement::getTextSize()
    {
        return mTextSize;
    }
    void TextElement::forceAnimEnd()
    {
    }
    void TextElement::forceAnimReset()
    {
    }
    const String& TextElement::getTypeName(void) const
    {
        return msTypeName;
    }
    void TextElement::getRenderOperation(RenderOperation& op)
    {
        op = mRenderOp;
    }
    void TextElement::setMaterialName(const String& matName)
    {
        OverlayElement::setMaterialName(matName);
    }
    void TextElement::_update(void)
    {
        Real vpWidth, vpHeight;
        vpWidth = (Real) (OverlayManager::getSingleton().getViewportWidth());
        vpHeight = (Real) (OverlayManager::getSingleton().getViewportHeight());
        mViewportAspectCoef = vpWidth/vpHeight;
        if (mIsAnimated)
        {
            animate();
        }
        OverlayElement::_update();
    }
    void TextElement::allocateFadeList(size_t size)
    {
        mFadeList.clear();
        for (size_t i = 0; i <= size; ++i)
        {
            mFadeList.push_back(0.0f);
        }
    }
    void TextElement::animate()
    {
        if (mInitialised)
        {
            if (mStrCursorPosition < mStringSize)
            {
                if (deleteme > 1)
                {
                    ++mStrCursorPosition;
                    deleteme = 0;
                }
                ++deleteme;
                mAnimationActive = true;
            }
            else
            {
                mStrCursorPosition = 0;
                mAnimationActive = false;
                allocateFadeList(mStringSize);
            }
            if (mAnimationActive)
                mGeomPositionsOutOfDate = true;
        }
    }
    void TextElement::checkMemoryAllocation(size_t num_chars)
    {
        if ( mAllocSize < num_chars)
        {
            // Create and bind new buffers
            // Note that old buffers will be deleted automatically through reference counting
            // 6 verts per char since we're doing tri lists without indexes
            // Allocate space for positions & texture coords
            VertexDeclaration* decl = mRenderOp.vertexData->vertexDeclaration;
            VertexBufferBinding* bind = mRenderOp.vertexData->vertexBufferBinding;
            mRenderOp.vertexData->vertexCount = num_chars * 6;
            // Create dynamic since text tends to change alot
            // positions & texcoords
            HardwareVertexBufferSharedPtr vbuf =
                HardwareBufferManager::getSingleton().
                createVertexBuffer(
                    decl->getVertexSize(0),
                    mRenderOp.vertexData->vertexCount,
                    HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY);
            bind->setBinding(0, vbuf);
            // colours
            vbuf = HardwareBufferManager::getSingleton().
                   createVertexBuffer(
                       decl->getVertexSize(1),
                       mRenderOp.vertexData->vertexCount,
                       HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY);
            bind->setBinding(1, vbuf);
            mAllocSize = num_chars;
        }
    }
    void TextElement::phraseText()
    {
    }
    size_t TextElement::getStrSize(std::string &a_str, size_t max_len)
    {
        std::string::iterator itr, iend;
        itr =  a_str.begin();
        iend =  a_str.end();
        size_t temp = 0;
        for (; itr != iend;)
        {
            if (max_len != 0 && temp > max_len) // If max_len is not equal to zero and temp is higher or equal to max len...
            {
                break; // If max_lend is not equal zero, it means that it reached the limit imposed by the user
            }
            while (*itr == 0x00 || *itr == 0x0A || *itr == '^')
            {
                if (*itr == 0x0A)
                {
                    ++itr; // It's a New Line character, just skip it
                }
                if (*itr == 0x00)
                {
                    ++itr; // It's a Null character, just skip it
                }
                if (*itr == 0x20 && mSkipWhiteSpaces == true)
                {
                    ++itr; // It's a Null character, if Skip White Spaces is enabled, skip it then.
                }
                while (*itr == '^')
                {
                    ++itr;
                    if (*itr == 'c')
                    {
                        ++itr; // go to the next character
                        ++itr; // Thousands
                        ++itr; // Hundreds
                        ++itr; // Decimals?
                        ++itr; // Units?
                    }
                    else if (*itr == 's')
                    {
                        ++itr; // go to the next character
                        ++itr; // Command type
                    }
                }
            }
            ++itr;
            ++temp;
        }
        return temp;
    }
    void TextElement::updatePositionGeometry()
    {
        float *pVert;
        size_t t_curPos = 0;
        if (mIsAnimated)
            t_curPos = getStrSize(mCaption, mStrCursorPosition);
        else
            t_curPos = mStringSize;
        checkMemoryAllocation(t_curPos); // Allocate the memory to the text
        // Write the text to the screen
        mRenderOp.vertexData->vertexCount = t_curPos * 6;
        // Get the vertex buffer for texture and position
        HardwareVertexBufferSharedPtr p_vbuf =
            mRenderOp.vertexData->vertexBufferBinding->getBuffer(0);
        pVert = static_cast<float*>(
                    p_vbuf->lock(HardwareBuffer::HBL_DISCARD) );
        // Get the vertex buffer for colors
        HardwareVertexBufferSharedPtr c_vbuf =
            mRenderOp.vertexData->vertexBufferBinding->getBuffer(1);
        RGBA* pDest = static_cast<RGBA*>(
                          c_vbuf->lock(HardwareBuffer::HBL_DISCARD) );
        RGBA color_cursor;
        Real fade_alpha;
        mCurTextColor = mFontPtr->mColorList[0];
        Real txtPosX = _getDerivedLeft() * 2.0 - 1.0;
        Real txtPosY = -( (_getDerivedTop() * 2.0 ) - 1.0 );
        // Text Square Value
        Real square[8];
        // Top Left
        square[0] = 0.0f;
        square[1] = 0.0f;
        // Top Right
        square[2] = ((2.0f /  mViewportAspectCoef) * mTextSize);
        square[3] = 0.0f;
        // Bottom Left
        square[4] = 0.0f;
        square[5] = (2.0f * mTextSize);
        // Bottom Right
        square[6] = ((2.0f /  mViewportAspectCoef) * mTextSize);
        square[7] = (2.0f * mTextSize);
        // Loop drawing each letter for this frame
        std::string::iterator itr;
        itr = mCaption.begin();
        for ( size_t cur = 0; cur < t_curPos; ++cur)
        {
            while (*itr == 0x00 || *itr == 0x0A || *itr == '^')
            {
                if (*itr == 0x00)
                {
                    ++itr;
                    continue;
                }
                if (*itr == 0x0A)
                {
                    txtPosY -= 2.0 * mTextSize;
                    txtPosX = _getDerivedLeft() * 2.0 - 1.0;
                    ++itr;
                    continue;
                }
                if (*itr == '^')
                {
                    ++itr;
                    if (*itr == 'c')
                    {
                        ++itr; // go to the next character
                        // Convert the string to int (4 char)
                        size_t id = 0;
                        for (size_t pos1 = 4; pos1 > 0; --pos1)
                        {
                            size_t c = *itr;

                            size_t pos = pos1 - 1;
                            if (c == '0')
                                id += 0 * pow( 10, pos );
                            if (c == '1')
                                id += 1 * pow( 10, pos );
                            if (c == '2')
                                id += 2 * pow( 10, pos );
                            if (c == '3')
                                id += 3 * pow( 10, pos );
                            if (c == '4')
                                id += 4 * pow( 10, pos );
                            if (c == '5')
                                id += 5 * pow( 10, pos );
                            if (c == '6')
                                id += 6 * pow( 10, pos );
                            if (c == '7')
                                id += 7 * pow( 10, pos );
                            if (c == '8')
                                id += 8 * pow( 10, pos );
                            if (c == '9')
                                id += 9 * pow( 10, pos );
                            ++itr;
                        }
                        mCurTextColor = mFontPtr->mColorList[id];
                    }
                    else if (*itr == 's')
                    {
                        ++itr; // go to the next character
                        if (*itr == 'U')
                        {
                            mAnimSpeed += 0.5f;
                            ++itr;
                            continue;
                        }
                        if (*itr == 'D')
                        {
                            mAnimSpeed -= 0.5f;
                            ++itr;
                            continue;
                        }
                        if (*itr == 'N')
                        {
                            mAnimSpeed = mDefAnimSpeed;
                            ++itr;
                            continue;
                        }
                    }
                }
                continue;
            }
            fade_alpha = mFadeList[cur];
            // Start by updating the geometry and textures
            GlyphStruct glyphData = mFontPtr->glyphs[static_cast<unsigned char>(*itr)];
            // each vert is (x, y, z, u, v, r, g, b, a)
            //-------------------------------------------------------------------------------------
            // First tri
            // 0  2
            // 4  6
            // Upper left
            *pVert++ = (txtPosX + square[0]);
            *pVert++ = (txtPosY - square[1]);
            *pVert++ = -1.0;
            *pVert++ = glyphData.texcoord0.x;
            *pVert++ = glyphData.texcoord0.y;
            // Bottom left
            *pVert++ = (txtPosX + square[4]);
            *pVert++ = (txtPosY - square[5]);
            *pVert++ = -1.0;
            *pVert++ = glyphData.texcoord2.x;
            *pVert++ = glyphData.texcoord2.y;
            // Top right
            *pVert++ = (txtPosX + square[2]);
            *pVert++ = (txtPosY - square[3]);
            *pVert++ = -1.0;
            *pVert++ = glyphData.texcoord1.x;
            *pVert++ = glyphData.texcoord1.y;
            //-------------------------------------------------------------------------------------
            //-------------------------------------------------------------------------------------
            // Second tri
            // Top right (again)
            *pVert++ = (txtPosX + square[2]);
            *pVert++ = (txtPosY - square[3]);
            *pVert++ = -1.0;
            *pVert++ = glyphData.texcoord1.x;
            *pVert++ = glyphData.texcoord1.y;
            // Bottom left (again)
            *pVert++ = (txtPosX + square[4]);
            *pVert++ = (txtPosY - square[5]);
            *pVert++ = -1.0;
            *pVert++ = glyphData.texcoord2.x;
            *pVert++ = glyphData.texcoord2.y;
            // Bottom right
            *pVert++ = (txtPosX + square[6]);
            *pVert++ = (txtPosY - square[7]);
            *pVert++ = -1.0;
            *pVert++ = glyphData.texcoord3.x;
            *pVert++ = glyphData.texcoord3.y;
            //-------------------------------------------------------------------------------------
            txtPosX += ((glyphData.x_offset / mViewportAspectCoef) *2.0) * mTextSize;
            mCurTextColor.a = fade_alpha;
            Root::getSingleton().convertColourValue(mCurTextColor, &color_cursor);
            // Now update the colors
            // First tri (top, bottom, top)
            *pDest++ = color_cursor;
            *pDest++ = color_cursor;
            *pDest++ = color_cursor;
            // Second tri (top, bottom, bottom)
            *pDest++ = color_cursor;
            *pDest++ = color_cursor;
            *pDest++ = color_cursor;
            fade_alpha += mFadeSpeed * 0.012f;
            if (fade_alpha > 1.0f)
            {
                fade_alpha = 1.0f;
            }
            mFadeList[cur] = fade_alpha;
            ++itr;
        }
        c_vbuf->unlock();
        p_vbuf->unlock();
    }
    void TextElement::updateTextureGeometry()
    {

    }
} // namespace
