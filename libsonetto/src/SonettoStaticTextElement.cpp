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

#include "SonettoStaticTextElement.h"
#include "SonettoKernel.h"
#include <OgreRoot.h>
#include <OgreHardwareBufferManager.h>

namespace Sonetto
{
    // ----------------------------------------------------------------------
    Ogre::String StaticTextElement::msTypeName = "StaticText";
    // ----------------------------------------------------------------------
    StaticTextElement::StaticTextElement(const Ogre::String &name) :
    Ogre::OverlayElement(name),
    mInitialized(false),
    mStringSize(0),
    mAllocatedSize(0),
    mAlpha(1.0f),
    mTextSize(Ogre::Vector2(0.07f,0.07f)),
    mpFont(NULL)
    {
    }
    // ----------------------------------------------------------------------
    StaticTextElement::~StaticTextElement()
    {
        delete mRenderOp.vertexData;
    }
    // ----------------------------------------------------------------------
    void StaticTextElement::initialise(void)
    {
        if(!mInitialized)
        {
            // Create Vertex Data.
            mRenderOp.vertexData = new Ogre::VertexData();
            // get a pointer to vertex declaration.
            Ogre::VertexDeclaration * vertexdecl = mRenderOp.vertexData->vertexDeclaration;
            size_t offset = 0;

            // Vertex Positions
            vertexdecl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
			offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

			// Texture Coordinates
			vertexdecl->addElement(0, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, 0);
			offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);

			// Colors - store these in a separate
			vertexdecl->addElement(1, 0, Ogre::VET_COLOUR, Ogre::VES_DIFFUSE);

			mRenderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
			mRenderOp.useIndexes = false;
			mRenderOp.vertexData->vertexStart = 0;

			allocateMemory(16);

            mInitialized = true;
        }
    }
    // ----------------------------------------------------------------------
    void StaticTextElement::allocateMemory(size_t numchars)
    {
        if(mAllocatedSize < numchars)
        {
            // Create a new buffer, the old will be discarded automatically.
            // 6 verts per char since we're doing tri lists without indexes
            // Allocate space for positions & texture coords
            Ogre::VertexDeclaration* decl = mRenderOp.vertexData->vertexDeclaration;
            Ogre::VertexBufferBinding* bind = mRenderOp.vertexData->vertexBufferBinding;

            mRenderOp.vertexData->vertexCount = numchars * 6;

			Ogre::HardwareVertexBufferSharedPtr vbuf =
                Ogre::HardwareBufferManager::getSingleton().
                    createVertexBuffer(
                        decl->getVertexSize(0),
                        mRenderOp.vertexData->vertexCount,
                        Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY);
            bind->setBinding(0, vbuf);

            // colors
            vbuf = Ogre::HardwareBufferManager::getSingleton().
                    createVertexBuffer(
                        decl->getVertexSize(1),
                        mRenderOp.vertexData->vertexCount,
                        Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY);
            bind->setBinding(1, vbuf);

            mAllocatedSize = numchars;
        }
    }
    // ----------------------------------------------------------------------
    const Ogre::String& StaticTextElement::getTypeName(void) const
    {
        return msTypeName;
    }
    // ----------------------------------------------------------------------
    void StaticTextElement::getRenderOperation(Ogre::RenderOperation& op)
    {
        op = mRenderOp;
    }
    Ogre::MaterialPtr & StaticTextElement::getMaterial(void) const
    {
        return mpFont->mMaterial;
    }
    // ----------------------------------------------------------------------
    void StaticTextElement::_update(void)
    {
        Ogre::OverlayElement::_update();

        // Check text allocation
        mStringSize = mText.size();
        allocateMemory(mStringSize);

        float aspect = Kernel::getSingleton().mAspectRatio;

        // Set text position
        float o_txtPosX = ((_getDerivedLeft() * 2.0) - 1.0f)/aspect;
        float txtPosX = o_txtPosX;
        float o_txtPosY = -( (_getDerivedTop() * 2.0 ) - 1.0f );
        o_txtPosY += (mpFont->mVerticalOffsetTop* 2.0f) * mTextSize.y;
        float txtPosY = o_txtPosY;

        // Get Vertex Buffers
        Ogre::HardwareVertexBufferSharedPtr vbuf =
			mRenderOp.vertexData->vertexBufferBinding->getBuffer(0);
		float * pVert = static_cast<float*>(
			vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD) );

        Ogre::HardwareVertexBufferSharedPtr cvbuf =
			mRenderOp.vertexData->vertexBufferBinding->getBuffer(1);
		Ogre::RGBA * pCol = static_cast<Ogre::RGBA*>(
			cvbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD) );

        std::string::iterator itr;
        itr = mText.begin();

        size_t final_TextVertexCount = 0;

        // Setup the base plane values to ease calculations.
        float square[8];
        // Top Left
        square[0] = 0.0f;
        square[1] = 0.0f;
        // Top Right
        square[2] = ((2.0f /  aspect) * (mTextSize.x * mpFont->mHorizontalScale));
        square[3] = 0.0f;
        // Bottom Left
        square[4] = 0.0f;
        square[5] = (2.0f * mTextSize.y);
        // Bottom Right
        square[6] = ((2.0f /  aspect) * (mTextSize.x * mpFont->mHorizontalScale));
        square[7] = (2.0f * mTextSize.y);

        // Reset Text Color
        Ogre::RGBA cursorColor;
        Ogre::ColourValue color = mpFont->mColorList[0];

        for(size_t p = 0; p < mStringSize;)
        {
            switch((unsigned char)*itr)
            {
                case SCT_WHITESPACE:
                txtPosX -= ((mpFont->mGlyph[SCT_WHITESPACE].mLeftOffset* 2.0f)/aspect) * (mTextSize.x * mpFont->mHorizontalScale);
                txtPosX += ((mpFont->mGlyph[SCT_WHITESPACE].mRightOffset* 2.0f)/aspect) * (mTextSize.x * mpFont->mHorizontalScale);
                // Just move the cursor.
                case 0x00:
                // Advance to the next character in the string.
                ++itr;
                ++p;
                break;
                case SCT_NEWLINE:
                txtPosX = o_txtPosX;
                txtPosY -= ((mpFont->mVerticalOffsetBottom)* 2.0f) * mTextSize.y;
                txtPosY += ((mpFont->mVerticalOffsetTop)* 2.0f) * mTextSize.y;
                ++itr;
                ++p;
                break;
                case SCT_POST_PROCESSING:
                ++itr;
                ++p;
                switch(*itr)
                {
                    case SCI_COLOR_CHANGE:
                    ++itr;
                    {
                        color = mpFont->mColorList[(unsigned char)*itr];
                        ++itr;
                    }
                    ++p;
                    ++p;
                    break;
                    case SCI_COLOR_RESET:
                    ++itr;
                    {
                        color = mpFont->mColorList[0];
                    }
                    ++p;
                    break;
                    case SCI_COLOR_SET_RGB:
                    ++itr;
                    {
                        unsigned char r = *itr;
                        ++itr;
                        unsigned char g = *itr;
                        ++itr;
                        unsigned char b = *itr;
                        ++itr;
                        color = Ogre::ColourValue(r/256.0f,g/256.0f,b/256.0f,1.0f);
                    }
                    ++p;
                    ++p;
                    ++p;
                    break;
                }
                break;
                default:
                {
                    FontGlyph glyphData = mpFont->mGlyph[static_cast<unsigned char>(*itr)];
                    txtPosX -= ((glyphData.mLeftOffset * 2.0f)/aspect) * (mTextSize.x * mpFont->mHorizontalScale);

                    // each vert is (x, y, z, u, v, r, g, b, a)
                    //-----------------------------------------
                    // First tri
                    // 0  2
                    // 4  6
                    // Upper left
                    *pVert++ = (txtPosX + square[0]);
                    *pVert++ = (txtPosY - square[1]);
                    *pVert++ = -1.0;
                    *pVert++ = glyphData.mTopLeft.x;
                    *pVert++ = glyphData.mTopLeft.y;
                    // Bottom left
                    *pVert++ = (txtPosX + square[4]);
                    *pVert++ = (txtPosY - square[5]);
                    *pVert++ = -1.0;
                    *pVert++ = glyphData.mBottomLeft.x;
                    *pVert++ = glyphData.mBottomLeft.y;
                    // Top right
                    *pVert++ = (txtPosX + square[2]);
                    *pVert++ = (txtPosY - square[3]);
                    *pVert++ = -1.0;
                    *pVert++ = glyphData.mTopRight.x;
                    *pVert++ = glyphData.mTopRight.y;
                    //-----------------------------------------
                    //-----------------------------------------
                    // Second tri
                    // Top right (again)
                    *pVert++ = (txtPosX + square[2]);
                    *pVert++ = (txtPosY - square[3]);
                    *pVert++ = -1.0;
                    *pVert++ = glyphData.mTopRight.x;
                    *pVert++ = glyphData.mTopRight.y;
                    // Bottom left (again)
                    *pVert++ = (txtPosX + square[4]);
                    *pVert++ = (txtPosY - square[5]);
                    *pVert++ = -1.0;
                    *pVert++ = glyphData.mBottomLeft.x;
                    *pVert++ = glyphData.mBottomLeft.y;
                    // Bottom right
                    *pVert++ = (txtPosX + square[6]);
                    *pVert++ = (txtPosY - square[7]);
                    *pVert++ = -1.0;
                    *pVert++ = glyphData.mBottomRight.x;
                    *pVert++ = glyphData.mBottomRight.y;

                    txtPosX += ((glyphData.mRightOffset * 2.0f)/aspect) * (mTextSize.x * mpFont->mHorizontalScale);

                    // Set the current fade alpha to the cursor, and apply the base alpha into it.
                    color.a = mAlpha;
                    Ogre::Root::getSingleton().convertColourValue(color, &cursorColor);
                    // Now update the colors
                    // First tri (top, bottom, top)
                    *pCol++ = cursorColor;
                    *pCol++ = cursorColor;
                    *pCol++ = cursorColor;
                    // Second tri (top, bottom, bottom)
                    *pCol++ = cursorColor;
                    *pCol++ = cursorColor;
                    *pCol++ = cursorColor;
                    ++itr;
                    ++p;
                    ++final_TextVertexCount;
                }
                break;
            }
        }

        mRenderOp.vertexData->vertexCount = final_TextVertexCount * 6;

        cvbuf->unlock();
        vbuf->unlock();
    }
    // ----------------------------------------------------------------------
} // namespace
