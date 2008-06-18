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
    //-----------------------------------------------------------------------------
    // Sonetto::TextElement implementation
    //-----------------------------------------------------------------------------
    #define DEFAULT_INITIAL_CHARS 12
    #define NEW_LINE_CHARACTER 0x0D
    #define TEXT_COMMAND 0x0C
    #define TEXT_COLOR_COMMAND 0x43
    #define TEXT_SPEED_COMMAND 0x53
    //-----------------------------------------------------------------------------
    String TextElement::msTypeName = "Text";
    //-----------------------------------------------------------------------------
    TextElement::TextElement(const String& name)
            : OverlayElement(name)
    {
        mAllocSize = 0;
        mStringSize = 0;
        mStrCursorPosition = 0;
        mTextSize = 0.1f;
        mNewLineSize = 1.0f;
        mCurTextColor = ColourValue::White;
        mViewportAspectCoef = 1.0f;
        mIsAnimated = true;
        mUseFadeIn = true;
        mUseFadeOut = true;
        mSkipWhiteSpaces = false;
        mAnimationActive = false;
        mAnimSpeed = 20.0f;
        mDefAnimSpeed = mAnimSpeed;
        mFadeSpeed = 0.5f;
        mFadeLevel = 1.0f;
        mScrMetricsMode = SMM_RELATIVE_ASPECT_ADJUSTED;
        initialise();
    }
    //-----------------------------------------------------------------------------
    TextElement::~TextElement()
    {
        delete mRenderOp.vertexData;
    }
    //-----------------------------------------------------------------------------
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
    //-----------------------------------------------------------------------------
    void TextElement::setCaption(const Ogre::String& text)
    {
        mCaption = text;
        mStringSize = getStrSize(mCaption, 0);
        checkMemoryAllocation(mStringSize);
        allocateFadeList(mStringSize);
        forceAnimReset();
        mGeomPositionsOutOfDate = true;
        mGeomUVsOutOfDate = true;
    }
    //-----------------------------------------------------------------------------
    void TextElement::setMessage(const Ogre::String& text)
    {
        mCaption = text;
        mStringSize = getStrSize(mCaption, 0);
        checkMemoryAllocation(mStringSize);
        allocateFadeList(mStringSize);
        forceAnimReset();
        mGeomPositionsOutOfDate = true;
        mGeomUVsOutOfDate = true;
    }
    //-----------------------------------------------------------------------------
    /*void TextElement::setCaption(const Ogre::String * text)
    {
        mCaption = *text;
        mStringSize = getStrSize(mCaption, 0);
        checkMemoryAllocation(mStringSize);
        allocateFadeList(mStringSize);
        if(mIsAnimated)
            forceAnimReset();
        mGeomPositionsOutOfDate = true;
        mGeomUVsOutOfDate = true;
    }*/
    //-----------------------------------------------------------------------------
    void TextElement::setFont(FontPtr font)
    {
        mFontPtr = font;
        mpMaterial = mFontPtr->font_material;
        mpMaterial->setDepthCheckEnabled(false);
        mpMaterial->setLightingEnabled(false);
    }
    //-----------------------------------------------------------------------------
    FontPtr TextElement::getFont()
    {
        return mFontPtr;
    }
    //-----------------------------------------------------------------------------
    void TextElement::setAnimSettings(bool isAnim, bool useFade, bool useFOut, Real anmSpd, Real fdSpd)
    {
        mIsAnimated = isAnim;
        mUseFadeIn = useFade;
        mUseFadeOut = useFOut;
        mAnimSpeed = anmSpd;
        mFadeSpeed = fdSpd;
    }
    //-----------------------------------------------------------------------------
    void TextElement::setTextSize(Real size)
    {
        mTextSize = size;
    }
    //-----------------------------------------------------------------------------
    Real TextElement::getTextSize()
    {
        return mTextSize;
    }
    //-----------------------------------------------------------------------------
    void TextElement::forceAnimEnd()
    {
        if(mAnimationActive && mIsAnimated)
        {
            mStrCursorPosition = mStringSize;
            updatePositionGeometry();
            mGeomPositionsOutOfDate = false;
        }
    }
    //-----------------------------------------------------------------------------
    void TextElement::forceAnimReset()
    {
        if(mIsAnimated)
        {
            mStrCursorPosition = 0;
            mDifference = 0.0f;
            allocateFadeList(mStringSize);
            updatePositionGeometry();
        }
    }
    //-----------------------------------------------------------------------------
    const String& TextElement::getTypeName(void) const
    {
        return msTypeName;
    }
    //-----------------------------------------------------------------------------
    void TextElement::getRenderOperation(RenderOperation& op)
    {
        op = mRenderOp;
    }
    //-----------------------------------------------------------------------------
    void TextElement::setMaterialName(const String& matName)
    {
        OverlayElement::setMaterialName(matName);
    }
    //-----------------------------------------------------------------------------
    void TextElement::_update(void)
    {
        Ogre::ControllerValueRealPtr tmpFTV = Ogre::ControllerManager::getSingleton().getFrameTimeSource();
        mTimeSinceLastFrame = tmpFTV->getValue();
        Ogre::Real vpWidth, vpHeight;
        vpWidth = (Ogre::Real) (Ogre::OverlayManager::getSingleton().getViewportWidth());
        vpHeight = (Ogre::Real) (Ogre::OverlayManager::getSingleton().getViewportHeight());
        Ogre::Real tmpVPAspectCoef = vpHeight/vpWidth;

        mViewportAspectCoef = tmpVPAspectCoef;

        if (mIsAnimated)
            animate();

        OverlayElement::_update();
    }
    //-----------------------------------------------------------------------------
    Ogre::Real TextElement::getLineLenght(const char * line)
    {
        Ogre::Real ret = 0;
        const char * cur = line;
        while( (*cur != 0) && (*cur != NEW_LINE_CHARACTER ) )
        {
            switch(*cur)
            {
                case TEXT_COMMAND:
                    ++cur;
                    switch(*cur)
                    {
                        case TEXT_COLOR_COMMAND:
                            ++cur;
                            ++cur;
                            ++cur;
                            ++cur;
                            ++cur;
                        break;
                        case TEXT_SPEED_COMMAND:
                            ++cur;
                            ++cur;
                        break;
                    }
                break;
                default:
                {
                    Glyphstruct  glyphData = mFontPtr->glyphs[static_cast<int>(*cur)];
                    switch(mScrMetricsMode)
                    {
                        case SMM_RELATIVE:
                            ret += glyphData.x_offset * mTextSize;
                        break;
                        case SMM_RELATIVE_ASPECT_ADJUSTED:
                            ret += (glyphData.x_offset * mViewportAspectCoef) * mTextSize;
                        break;
                    }
                    ++cur;
                }
                break;
            }
        }
        ret += mViewportAspectCoef * mTextSize;
        return ret;
    }
    //-----------------------------------------------------------------------------
    const char * TextElement::getLargestLine(const char * str)
    {
        size_t l_line_size = 0;
        size_t c_line_size = 0;
        const char * cur = str;
        const char * l_line_ptr = cur;
        const char * c_line_ptr = cur;
        while( (*cur != 0) )
        {
            switch(*cur)
            {
                case NEW_LINE_CHARACTER:
                {
                    if(c_line_size > l_line_size)
                    {
                        l_line_size = c_line_size;
                        l_line_ptr = c_line_ptr;
                        ++cur;
                        c_line_size = 0;
                        c_line_ptr = cur;
                    }
                    else
                    {
                        ++cur;
                        c_line_size = 0;
                        c_line_ptr = cur;
                    }
                }
                break;
                case 0:
                {
                    if(c_line_size > l_line_size)
                    {
                        std::cout<<"c_line_size: "<<c_line_size<<"\n";
                        ++cur;
                        l_line_ptr = cur;
                        l_line_size = c_line_size;
                        c_line_size = 0;

                    }
                    else
                    {
                        ++cur;
                    }
                }
                break;
                case TEXT_COMMAND:
                    ++cur;
                    switch(*cur)
                    {
                        case TEXT_COLOR_COMMAND:
                            ++cur;
                            ++cur;
                            ++cur;
                            ++cur;
                            ++cur;
                        break;
                        case TEXT_SPEED_COMMAND:
                            ++cur;
                            ++cur;
                        break;
                    }
                break;
                default:
                {
                    ++c_line_size;
                    ++cur;
                }
                break;
            }
        }
        return l_line_ptr;
    }
    //-----------------------------------------------------------------------------
    Ogre::Real TextElement::getLargestLineLenght(const char * str)
    {
        Ogre::Real ret = 0;
        const char * cur = getLargestLine(str);
        while( (*cur != 0) && (*cur != NEW_LINE_CHARACTER ) )
        {
            switch(*cur)
            {
                case TEXT_COMMAND:
                    ++cur;
                    switch(*cur)
                    {
                        case TEXT_COLOR_COMMAND:
                            ++cur;
                            ++cur;
                            ++cur;
                            ++cur;
                            ++cur;
                        break;
                        case TEXT_SPEED_COMMAND:
                            ++cur;
                            ++cur;
                        break;
                    }
                break;
                default:
                {
                    Glyphstruct  glyphData = mFontPtr->glyphs[static_cast<int>(*cur)];
                    switch(mScrMetricsMode)
                    {
                        case SMM_RELATIVE:
                            ret += glyphData.x_offset * mTextSize;
                        break;
                        case SMM_RELATIVE_ASPECT_ADJUSTED:
                            ret += (glyphData.x_offset * mViewportAspectCoef) * mTextSize;
                        break;
                    }
                    ++cur;
                }
                break;
            }
        }
        ret += mViewportAspectCoef * mTextSize;
        return ret;
    }
    //-----------------------------------------------------------------------------
    Ogre::Real TextElement::getStringHeight(const char * str)
    {
        Ogre::Real ret = 0;
        const char * cur = str;
        bool finished = false;
        while(!finished)
        {
            switch(*cur)
            {
                default:
                    ++cur;
                break;
                case NEW_LINE_CHARACTER:
                    ret += mTextSize * mNewLineSize;
                    ++cur;
                break;
                case 0:
                    finished = true;
                break;
            }
        }
        ret += mTextSize;
        return ret;
    }
    //-----------------------------------------------------------------------------
    Ogre::Vector2 TextElement::getTextDimensions()
    {
        return Ogre::Vector2(getLargestLineLenght(&mCaption[0]),
        getStringHeight(&mCaption[0]));
    }
    //-----------------------------------------------------------------------------
    void TextElement::allocateFadeList(size_t size, float value)
    {
        mFadeList.clear();
        for (size_t i = 0; i <= size; ++i)
        {
            mFadeList.push_back(0.0f);
        }
    }
    //-----------------------------------------------------------------------------
    void TextElement::animate()
    {
        if (mInitialised)
        {
            mDifference += mAnimSpeed * mTimeSinceLastFrame;
            mStrCursorPosition = (size_t) mDifference;

            if(mStrCursorPosition > mStringSize && mFadeList[mStrCursorPosition-1] >= 1.0f)
                forceAnimReset();



            mAnimationActive = true;

            if (mAnimationActive)
                mGeomPositionsOutOfDate = true;

        }
    }
    //-----------------------------------------------------------------------------
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
    //-----------------------------------------------------------------------------
    void TextElement::phraseText()
    {
    }
    //-----------------------------------------------------------------------------
    size_t TextElement::getStrSize(std::string &a_str, size_t max_len)
    {
        std::string::iterator itr, iend;
        itr =  a_str.begin();
        iend =  a_str.end();
        size_t temp = 0;
        for (; itr != iend;)
        {
            if (max_len != 0 && temp >= max_len) // If max_len is not equal to zero and temp is higher or equal to max len...
            {
                break; // If max_lend is not equal zero, it means that it reached the limit imposed by the user
            }
            while (*itr == 0x00 || *itr == NEW_LINE_CHARACTER || *itr == TEXT_COMMAND)
            {
                if (*itr == NEW_LINE_CHARACTER)
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
                while (*itr == TEXT_COMMAND)
                {
                    ++itr;
                    if (*itr == TEXT_COLOR_COMMAND)
                    {
                        ++itr; // go to the next character
                        ++itr; // Thousands
                        ++itr; // Hundreds
                        ++itr; // Decimals?
                        ++itr; // Units?
                    }
                    else if (*itr == TEXT_SPEED_COMMAND)
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
    //-----------------------------------------------------------------------------
    void TextElement::updatePositionGeometry()
    {
        float *pVert;
        size_t t_curPos = 0;
        if (mIsAnimated)
        {
            if(mStrCursorPosition)
                t_curPos = getStrSize(mCaption, mStrCursorPosition);
            else
                t_curPos = 0;
        }
        else
        {
            t_curPos = mStringSize;
        }

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

        Real view_ratio = 1.0f;

        if(SMM_RELATIVE_ASPECT_ADJUSTED)
            view_ratio = mViewportAspectCoef;


        Real txtPosX,o_txtPosX;
        Real txtPosY,o_txtPosY;

        switch(mScrMetricsMode)
        {
        case SMM_RELATIVE:
        o_txtPosX = _getDerivedLeft() * 2.0f - 1.0f;
        o_txtPosY = -( (_getDerivedTop() * 2.0f ) - 1.0f );
        break;
        case SMM_RELATIVE_ASPECT_ADJUSTED:
        o_txtPosX = _getDerivedLeft() * (2.0 * view_ratio) - view_ratio;
        o_txtPosY = -( (_getDerivedTop() * 2.0 ) - 1.0f );
        break;
        }

        txtPosX = o_txtPosX;
        txtPosY = o_txtPosY;

        // Text Square Value
        Real square[8];
        // Top Left
        if(mScrMetricsMode == SMM_RELATIVE_ASPECT_ADJUSTED)
        {
        square[0] = 0.0f;
        square[1] = 0.0f;
        // Top Right
        square[2] = ((2.0f *  view_ratio) * mTextSize);
        square[3] = 0.0f;
        // Bottom Left
        square[4] = 0.0f;
        square[5] = (2.0f * mTextSize);
        // Bottom Right
        square[6] = ((2.0f *  view_ratio) * mTextSize);
        square[7] = (2.0f * mTextSize);
        } else {
        square[0] = 0.0f;
        square[1] = 0.0f;
        // Top Right
        square[2] = (2.0f * mTextSize);
        square[3] = 0.0f;
        // Bottom Left
        square[4] = 0.0f;
        square[5] = (2.0f * mTextSize);
        // Bottom Right
        square[6] = (2.0f * mTextSize);
        square[7] = (2.0f * mTextSize);
        }
        // Loop drawing each letter for this frame
        std::string::iterator itr;
        itr = mCaption.begin();
        for ( size_t cur = 0; cur < t_curPos; ++cur)
        {
            while (*itr == 0x00 || *itr == NEW_LINE_CHARACTER || *itr == TEXT_COMMAND)
            {
                if (*itr == 0x00)
                {
                    ++itr;
                    continue;
                }
                if (*itr == NEW_LINE_CHARACTER)
                {
                    txtPosY -= 2.0 * (mTextSize * mNewLineSize);

                    txtPosX = o_txtPosX;
                    ++itr;
                    continue;
                }
                if (*itr == TEXT_COMMAND)
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
                                id += 0 * (size_t) pow( 10, pos );
                            if (c == '1')
                                id += 1 * (size_t) pow( 10, pos );
                            if (c == '2')
                                id += 2 * (size_t) pow( 10, pos );
                            if (c == '3')
                                id += 3 * (size_t) pow( 10, pos );
                            if (c == '4')
                                id += 4 * (size_t) pow( 10, pos );
                            if (c == '5')
                                id += 5 * (size_t) pow( 10, pos );
                            if (c == '6')
                                id += 6 * (size_t) pow( 10, pos );
                            if (c == '7')
                                id += 7 * (size_t) pow( 10, pos );
                            if (c == '8')
                                id += 8 * (size_t) pow( 10, pos );
                            if (c == '9')
                                id += 9 * (size_t) pow( 10, pos );
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
            if(mUseFadeIn)
            {
                fade_alpha = mFadeList[cur];
            } else {
                fade_alpha = mFadeLevel;
            }
            // Start by updating the geometry and textures
            Glyphstruct  glyphData = mFontPtr->glyphs[static_cast<unsigned char>(*itr)];
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
            switch(mScrMetricsMode)
            {
                case SMM_RELATIVE:
                    txtPosX += (glyphData.x_offset * 2.0f) * mTextSize;
                break;
                case SMM_RELATIVE_ASPECT_ADJUSTED:
                    txtPosX += ((glyphData.x_offset * view_ratio) * 2.0f) * mTextSize;
                break;
            }

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

            if(mUseFadeIn)
            {
                fade_alpha += mFadeSpeed * mTimeSinceLastFrame;
                if (fade_alpha > 1.0f)
                {
                    fade_alpha = 1.0f;
                }
                mFadeList[cur] = fade_alpha;
            }

            ++itr;
        }
        c_vbuf->unlock();
        p_vbuf->unlock();
    }
    //-----------------------------------------------------------------------------
    void TextElement::updateTextureGeometry()
    {}
    //-----------------------------------------------------------------------------
} // namespace
