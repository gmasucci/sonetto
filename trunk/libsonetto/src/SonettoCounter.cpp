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

#include "SonettoCounter.h"
#include <OgreOverlayManager.h>
#include <OgreRoot.h>
#include <OgreRenderSystem.h>
#include <OgreHardwareBufferManager.h>

namespace Sonetto {
    //-----------------------------------------------------------------------------
    // Sonetto::Counter implementation
    //-----------------------------------------------------------------------------
    #define TEX_NUM_BLOCKS 16
    #define TEX_VAL_BLOCKS 0.0625f
    //-----------------------------------------------------------------------------
    Ogre::String Counter::msTypeName = "Counter";
    //-----------------------------------------------------------------------------
    Counter::Counter(const Ogre::String &name) : Ogre::OverlayElement(name),
    mInclination(0.0f),
    mAllocSize(0),
    mScrMetricsMode(SMM_RELATIVE_ASPECT_ADJUSTED),
    mFlags(Counter::FLAG_SHOW_MAXIMUM_VALUE | Counter::FLAG_COLOR_NUMBERS),
    mNumDigits(4),
    mThreshold(25),
    mCurrentValue(0),
    mMaxValue(0)
    {}
    //-----------------------------------------------------------------------------
    Counter::~Counter()
    {
        mCurrentValue = 0;
        mMaxValue = 0;
        delete mRenderOp.vertexData;
    }
    //-----------------------------------------------------------------------------
    void Counter::initialise(void)
    {
        if (!mInitialised)
        {
            // Set up the render op
            // Combine positions and texture coords since they tend to change together
            // since character sizes are different
            mRenderOp.vertexData = new Ogre::VertexData();
            Ogre::VertexDeclaration* decl = mRenderOp.vertexData->vertexDeclaration;
            size_t offset = 0;
            // Positions
            decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
            offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
            // Texcoords
            decl->addElement(0, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, 0);
            offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);
            // Colours - store in the same buffer
            decl->addElement(1, 0, Ogre::VET_COLOUR, Ogre::VES_DIFFUSE);
            mRenderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
            mRenderOp.useIndexes = false;
            mRenderOp.vertexData->vertexStart = 0;
            // Allocate memory for the object for memory buffer.
            allocateMemory((mNumDigits*2)+1);
            mInitialised = true;
        }
    }
    //-----------------------------------------------------------------------------
    void Counter::setFlags(unsigned char flags)
    {
        mFlags = flags;
    }
    //-----------------------------------------------------------------------------
    unsigned char Counter::getFlags()
    {
        return mFlags;
    }
    //-----------------------------------------------------------------------------
    void Counter::setNumDigits(unsigned char num)
    {
        mNumDigits = num;
        allocateMemory(mNumDigits);
    }
    //-----------------------------------------------------------------------------
    unsigned char Counter::getNumDigits()
    {
        return mNumDigits;
    }
    //-----------------------------------------------------------------------------
    void Counter::setThreshold(char percentage)
    {
        mThreshold = percentage;
    }
    //-----------------------------------------------------------------------------
    char Counter::getThreshold()
    {
        return mThreshold;
    }
    //-----------------------------------------------------------------------------
    void Counter::setCurValPtr(int * ptr)
    {
        mCurrentValue = ptr;
    }
    //-----------------------------------------------------------------------------
    int * Counter::getCurValPtr()
    {
        return mCurrentValue;
    }
    //-----------------------------------------------------------------------------
    void Counter::setMaxValPtr(int * ptr)
    {
        mMaxValue = ptr;
    }
    //-----------------------------------------------------------------------------
    int * Counter::getMaxValPtr()
    {
        return mMaxValue;
    }
    //-----------------------------------------------------------------------------
    void Counter::setInclination(Ogre::Real incl)
    {
        mInclination = incl;
    }
    //-----------------------------------------------------------------------------
    void Counter::setColors(const Ogre::ColourValue &normal,const Ogre::ColourValue &below,const Ogre::ColourValue &zero)
    {
        mNormalColor = normal;
        mBelowThrColor = below;
        mZeroValColor = zero;
    }
    //-----------------------------------------------------------------------------
    void Counter::setMaterialName(const Ogre::String& matName)
    {
        Ogre::OverlayElement::setMaterialName(matName);
    }
    //-----------------------------------------------------------------------------
    void Counter::setScrMetricsMode(ScreenMetricsMode smm)
    {
        mScrMetricsMode = smm;
    }
    //-----------------------------------------------------------------------------
    ScreenMetricsMode Counter::getScrMetricsMode()
    {
        return mScrMetricsMode;
    }
    //-----------------------------------------------------------------------------
    const Ogre::String& Counter::getTypeName(void) const
    {
        return msTypeName;
    }
    //-----------------------------------------------------------------------------
    void Counter::getRenderOperation(Ogre::RenderOperation& op)
    {
        op = mRenderOp;
    }
    //-----------------------------------------------------------------------------
    void Counter::_update(void)
    {
        // Calculate the screen ratio.
        Ogre::Real vpWidth, vpHeight;
        vpWidth = (Ogre::Real) (Ogre::OverlayManager::getSingleton().getViewportWidth());
        vpHeight = (Ogre::Real) (Ogre::OverlayManager::getSingleton().getViewportHeight());
        // Assign the result.
        mViewportAspect = vpHeight/vpWidth;

        // Call the _updateFromParent function from OverlayElement.
        Ogre::OverlayElement::_updateFromParent();

		Ogre::Real digitWidth = mWidth * (mViewportAspect * 2.0f);

		b_left = _getDerivedLeft() * (mViewportAspect * 2) - mViewportAspect;
		b_right = b_left + digitWidth;
		t_left = b_left + ((mWidth * mInclination) * (mViewportAspect * 2.0f));
		t_right = b_right + ((mWidth * mInclination) * (mViewportAspect * 2.0f));
		top = -((_getDerivedTop() * 2) - 1);
		bottom =  top -  (mHeight * 2);

        // Get the number of digits
        unsigned char numDigits = 0;
        if(mFlags & FLAG_SHOW_MAXIMUM_VALUE)
            numDigits = (mNumDigits * 2) + 1;
        else
            numDigits = mNumDigits;
        // Set the vertex count.
        mRenderOp.vertexData->vertexCount = numDigits * 6;

        Ogre::Real * pVert; // Vertex and TexCoord pointer.
        Ogre::RGBA * pColor; // Vertex Colors pointer.

        // Get the vertex buffer for positions and texture coordinates
        Ogre::HardwareVertexBufferSharedPtr p_vbuf =
            mRenderOp.vertexData->vertexBufferBinding->getBuffer(0);
        // Lock the vertex buffer and assign its address to the pointer.
        pVert = static_cast<float*>( p_vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD) );

        // Get the vertex buffer for colors
        Ogre::HardwareVertexBufferSharedPtr c_vbuf =
            mRenderOp.vertexData->vertexBufferBinding->getBuffer(1);
        // Lock the vertex buffer and assign its address to the pointer.
        pColor = static_cast<Ogre::RGBA*>( c_vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD) );
        // Get the depth.
        Ogre::Real zValue = Ogre::Root::getSingleton().getRenderSystem()->getMaximumDepthInputValue();

        Ogre::RGBA currentColor;

        if(mFlags & FLAG_COLOR_NUMBERS)
        {
            if(*mCurrentValue <= 0)
                Ogre::Root::getSingleton().convertColourValue(mZeroValColor, &currentColor);
            else if(((*mCurrentValue * 100)/(*mMaxValue)) < mThreshold)
                Ogre::Root::getSingleton().convertColourValue(mBelowThrColor, &currentColor);
            else
                Ogre::Root::getSingleton().convertColourValue(mNormalColor, &currentColor);
        } else {
            Ogre::Root::getSingleton().convertColourValue(mNormalColor, &currentColor);
        }

        // Offset value to draw the digits.
        float bPos = 0;

        // Draw the Current Value.
        for(unsigned char i = mNumDigits; i > 0; --i)
        {
            char n = getDigit(*mCurrentValue, i-1);
            if( n >= 0 )
            {
                drawDigit(pVert, pColor, n, currentColor, bPos, zValue);
            }
            else
            {
                if( n < 0 && (mFlags & FLAG_SHOW_ZEROES))
                {
                    drawDigit(pVert, pColor, 0, currentColor, bPos, zValue);
                }
                else
                {
                    for(size_t t = 0; t < 6; ++t)
                    {
                        *pVert++ = 0.0f;
                        *pVert++ = 0.0f;
                        *pVert++ = 0.0f;
                        *pVert++ = 0.0f;
                        *pVert++ = 0.0f;
                        *pColor++ = currentColor;
                    }
                }
            }
            bPos += digitWidth;
        }

        Ogre::Root::getSingleton().convertColourValue(mNormalColor, &currentColor);

        // Only process this if the "Show Maximum value" is on.
        if(mFlags & FLAG_SHOW_MAXIMUM_VALUE)
        {
            // Draw the separator
            drawDigit(pVert, pColor, 11, currentColor, bPos, zValue);
            bPos += digitWidth;

            // Draw the Maximum Value
            for(size_t i = mNumDigits; i > 0; --i)
            {
                char n = getDigit(*mMaxValue, i-1);
                if( n >= 0 )
                {
                    drawDigit(pVert, pColor, n, currentColor, bPos, zValue);
                }
                else
                {
                    if( n < 0 && (mFlags & FLAG_SHOW_ZEROES))
                    {
                        drawDigit(pVert, pColor, 0, currentColor, bPos, zValue);
                    }
                    else
                    {
                        for(size_t t = 0; t < 6; ++t)
                        {
                            *pVert++ = 0.0f;
                            *pVert++ = 0.0f;
                            *pVert++ = 0.0f;
                            *pVert++ = 0.0f;
                            *pVert++ = 0.0f;
                            *pColor++ = currentColor;
                        }
                    }
                }
                bPos += digitWidth;
            }
        }
        // Unlock the color vertex buffer.
        c_vbuf->unlock();
        // Unlock the position and texture coordinates vertex buffer.
        p_vbuf->unlock();
    }
    //-----------------------------------------------------------------------------
    void Counter::allocateMemory(char numchar)
    {
        if(mAllocSize < numchar)
        {
            // Create and bind new buffers
            // Note that old buffers will be deleted automatically through reference counting
            // 6 verts per char since we're doing tri lists without indexes
            Ogre::VertexDeclaration* decl = mRenderOp.vertexData->vertexDeclaration;
            Ogre::VertexBufferBinding* bind = mRenderOp.vertexData->vertexBufferBinding;
            mRenderOp.vertexData->vertexCount = numchar * 6;
            // Create dynamic since text tends to change alot
            // positions & texcoords
            Ogre::HardwareVertexBufferSharedPtr vbuf =
                Ogre::HardwareBufferManager::getSingleton().
                createVertexBuffer(
                    decl->getVertexSize(0),
                    mRenderOp.vertexData->vertexCount,
                    Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY);
            bind->setBinding(0, vbuf);
            // colours
            vbuf = Ogre::HardwareBufferManager::getSingleton().
                   createVertexBuffer(
                       decl->getVertexSize(1),
                       mRenderOp.vertexData->vertexCount,
                       Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY);
            bind->setBinding(1, vbuf);
            mAllocSize = numchar;
        }
    }
    //-----------------------------------------------------------------------------
    void Counter::updatePositionGeometry()
    {
        // Do nothing.
    }
    //-----------------------------------------------------------------------------
    void Counter::updateTextureGeometry()
    {
        // Do nothing.
    }
    //-----------------------------------------------------------------------------
    int Counter::simplePow(int a, char b)
    {
        int r = a;
        for(int i = 0;i < b;++i)
            r *= a;
        return r;
    }
    //-----------------------------------------------------------------------------
    char Counter::getDigit(int value, char idx)
    {
        if (value == 0 && idx == 0)
            return 0;
        else if (value == 0 && idx != 0)
            return -1;

        int power = simplePow(10,idx);
        if (power/10 > value && value != 0)
            return -1;

        return (value%power)/(power/10);
    }
    //-----------------------------------------------------------------------------
    void Counter::drawDigit(float *& pvert, Ogre::RGBA *& pcolor, char digit, Ogre::RGBA color, float offset, float depth)
    {
        /*
        0---23
        |  //|
        | // |
        |//  |
        14---5
        */
        float vertex_offset = TEX_VAL_BLOCKS * digit;
        // Upper Left - Vertex 0
        // Positions.
        *pvert++ =  offset + t_left; // X
        *pvert++ =  top; // Y
        *pvert++ =  depth; // Z
        // Texcoords.
        *pvert++ =  vertex_offset; // U
        *pvert++ =  0.0f; // V
        // Colors.
        *pcolor++ = color;
        // Lower Left - Vertex 1
        // Positions.
        *pvert++ =  offset + b_left; // X
        *pvert++ =  bottom; // Y
        *pvert++ =  depth; // Z
        // Texcoords.
        *pvert++ =  vertex_offset; // U
        *pvert++ =  1.0f; // V
        // Colors.
        *pcolor++ = color;
        // Upper Right - Vertex 2
        // Positions.
        *pvert++ =  offset + t_right; // X
        *pvert++ =  top; // Y
        *pvert++ =  depth; // Z
        // Texcoords.
        *pvert++ =  vertex_offset + TEX_VAL_BLOCKS; // U
        *pvert++ =  0.0f; // V
        // Colors.
        *pcolor++ = color;
        // Upper Right - Vertex 3
        // Positions.
        *pvert++ =  offset + t_right; // X
        *pvert++ =  top; // Y
        *pvert++ =  depth; // Z
        // Texcoords.
        *pvert++ =  vertex_offset + TEX_VAL_BLOCKS; // U
        *pvert++ =  0.0f; // V
        // Colors.
        *pcolor++ = color;
        // Lower Left - Vertex 4
        // Positions.
        *pvert++ =  offset + b_left; // X
        *pvert++ =  bottom; // Y
        *pvert++ =  depth; // Z
        // Texcoords.
        *pvert++ =  vertex_offset; // U
        *pvert++ =  1.0f; // V
        // Colors.
        *pcolor++ = color;
        // Lower Right - Vertex 5
        // Positions.
        *pvert++ =  offset + b_right; // X
        *pvert++ =  bottom; // Y
        *pvert++ =  depth; // Z
        // Texcoords.
        *pvert++ =  vertex_offset + TEX_VAL_BLOCKS; // U
        *pvert++ =  1.0f; // V
        // Colors.
        *pcolor++ = color;
    }
    //-----------------------------------------------------------------------------
}
