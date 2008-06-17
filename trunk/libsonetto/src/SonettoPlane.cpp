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

#include "SonettoPlane.h"
#include "OGRE/OgrePrerequisites.h"
#include "OGRE/OgreHardwareBufferManager.h"
#include "OGRE/OgreRoot.h"
#include "OGRE/OgreRenderSystem.h"
#include "OGRE/OgreOverlayManager.h"

namespace Sonetto {
    //---------------------------------------------------------------------
    Ogre::String Plane::msTypeName = "Plane";
    //---------------------------------------------------------------------
    // vertex buffer bindings, set at compile time (we could look these up but no point)
    #define POSITION_BINDING 0
    #define TEXCOORD_BINDING 1
    #define COLOR_BINDING 2
    //---------------------------------------------------------------------
    Plane::Plane(const Ogre::String &name)
    : Ogre::OverlayContainer(name),
    mUpdateColors (true),
    mNumTexCoordsInBuffer (0),
    mU1(0.0),
    mV1(0.0),
    mU2(1.0),
    mV2(1.0)
    {

    }
    //---------------------------------------------------------------------
    Plane::~Plane()
    {
        delete mRenderOp.vertexData;
    }
    //---------------------------------------------------------------------
    void Plane::initialise(void)
    {
		bool init = !mInitialised;

		OverlayContainer::initialise();
		if (init)
		{
			// Setup render op in advance
			mRenderOp.vertexData = new Ogre::VertexData();
			// Vertex declaration: 1 position, add texcoords later depending on #layers
			// Create as separate buffers so we can lock & discard separately
			Ogre::VertexDeclaration* decl = mRenderOp.vertexData->vertexDeclaration;
			size_t offset = 0;
			// Positions
			decl->addElement(POSITION_BINDING, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
			offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
			// Positions
			decl->addElement(TEXCOORD_BINDING, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);
			offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);
			// Colours - store these in a separate buffer because they change less often
			decl->addElement(COLOR_BINDING, 0, Ogre::VET_COLOUR, Ogre::VES_DIFFUSE);

			// Basic vertex data
			mRenderOp.vertexData->vertexStart = 0;
			mRenderOp.vertexData->vertexCount = 4;

			// Vertex buffer #1
			Ogre::HardwareVertexBufferSharedPtr vbuf =
				Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
				decl->getVertexSize(POSITION_BINDING), mRenderOp.vertexData->vertexCount,
				Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY// mostly static except during resizing
				);
			// Bind buffer
			mRenderOp.vertexData->vertexBufferBinding->setBinding(POSITION_BINDING, vbuf);

            vbuf =
				Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
				decl->getVertexSize(TEXCOORD_BINDING), mRenderOp.vertexData->vertexCount,
				Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY// mostly static except during resizing
				);
			// Bind buffer
			mRenderOp.vertexData->vertexBufferBinding->setBinding(TEXCOORD_BINDING, vbuf);

			vbuf =
				Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
				decl->getVertexSize(COLOR_BINDING), mRenderOp.vertexData->vertexCount,
				Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY// mostly static except during resizing
				);
			// Bind buffer
			mRenderOp.vertexData->vertexBufferBinding->setBinding(COLOR_BINDING, vbuf);

			// No indexes & issue as a strip
			mRenderOp.useIndexes = false;
			mRenderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_STRIP;

            mUpdateColors = true; // force colour buffer regeneration

			mInitialised = true;
		}
    }
    //---------------------------------------------------------------------
    void Plane::setUV(Ogre::Real u1, Ogre::Real v1, Ogre::Real u2, Ogre::Real v2)
    {
		mU1 = u1;
		mU2 = u2;
		mV1 = v1;
		mV2 = v2;
		mGeomUVsOutOfDate = true;
    }
    //---------------------------------------------------------------------
    void Plane::getUV(Ogre::Real& u1, Ogre::Real& v1, Ogre::Real& u2, Ogre::Real& v2) const
    {
		u1 = mU1;
		u2 = mU2;
		v1 = mV1;
		v2 = mV2;
    }
    //---------------------------------------------------------------------
    void Plane::setColour(const Ogre::ColourValue& col)
    {
        mColour = col;
        mUpdateColors = true;
    }
    //-----------------------------------------------------------------------
    const Ogre::ColourValue& Plane::getColour(void) const
    {
        return mColour;
    }
    //---------------------------------------------------------------------
    const Ogre::String& Plane::getTypeName(void) const
    {
        return msTypeName;
    }
    //---------------------------------------------------------------------
    void Plane::getRenderOperation(Ogre::RenderOperation& op)
    {
        op = mRenderOp;
    }
    //---------------------------------------------------------------------
    void Plane::setMaterialName(const Ogre::String& matName)
    {
        OverlayContainer::setMaterialName(matName);
    }
    //---------------------------------------------------------------------
    void Plane::_updateRenderQueue(Ogre::RenderQueue* queue)
    {
        if (mVisible)
        {
            using namespace Ogre;
            // Also add children
            ChildIterator it = getChildIterator();
            while (it.hasMoreElements())
            {
                // Give children ZOrder 1 higher than this
                it.getNext()->_updateRenderQueue(queue);
            }
        }
    }
    //---------------------------------------------------------------------
    void Plane::_update(void)
    {
        OverlayContainer::_update();

        // Calculate the aspect ratio
        Ogre::Real vpWidth, vpHeight;
        vpWidth = (Ogre::Real) Ogre::OverlayManager::getSingleton().getViewportWidth();
        vpHeight = (Ogre::Real) Ogre::OverlayManager::getSingleton().getViewportHeight();
        Ogre::Real tmpAspectRatio = vpHeight/vpWidth;

        // Set up the flags to warn ogre we need an update.
        if(tmpAspectRatio != mAspectRatio) {
            mAspectRatio            = tmpAspectRatio;
            mGeomPositionsOutOfDate = true;
            mGeomUVsOutOfDate       = true;
        }

		if (mUpdateColors && mInitialised)
		{
			updateColors();
			mUpdateColors = false;
		}
    }
    //---------------------------------------------------------------------
    void Plane::updatePositionGeometry(void)
    {
        // Get the Vertex Buffer (for positions)
        Ogre::HardwareVertexBufferSharedPtr vbuf =
            mRenderOp.vertexData->vertexBufferBinding->getBuffer(POSITION_BINDING);
        // Cast the vertex buffer to a single float
        float* pPos = static_cast<float*>(
                          vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD) );
        // Get the Z Value for this element
        Ogre::Real zValue = Ogre::Root::getSingleton().getRenderSystem()->getMaximumDepthInputValue();
        // Store the aspect ratio in a temporary variable
        float mAspect = mAspectRatio;
        // Because if the Metrics Mode is Relative, we need to set it to 1.0
        if(mScrMetricsMode == SMM_RELATIVE)
            mAspect = 1.0f;

        /*
			0-----2
			|    /|
			|  /  |
			|/    |
			1-----3
		*/
		Ogre::Real left, right, top, bottom;

		left = _getDerivedLeft() * (mAspect * 2) - mAspect;
		right = left + ((mWidth * (mAspect * 2)) - mAspect);
		top = -((_getDerivedTop() * 2) - 1);
		bottom =  top -  (mHeight * 2);

        *pPos++ = left;
		*pPos++ = top;
		*pPos++ = zValue;

		*pPos++ = left;
		*pPos++ = bottom;
		*pPos++ = zValue;

		*pPos++ = right;
		*pPos++ = top;
		*pPos++ = zValue;

		*pPos++ = right;
		*pPos++ = bottom;
		*pPos++ = zValue;

        // We have finished here, unlock the vertex buffer.
        vbuf->unlock();
    }
    //---------------------------------------------------------------------
    void Plane::updateTextureGeometry(void)
    {
        // Generate for as many texture layers as there are in material
        if (!mpMaterial.isNull() && mInitialised)
        {
            Ogre::HardwareVertexBufferSharedPtr vbuf =
            mRenderOp.vertexData->vertexBufferBinding->getBuffer(TEXCOORD_BINDING);
            float* pTex = static_cast<float*>(
                              vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD) );

            *pTex++ = mU1;
            *pTex++ = mV1;

            *pTex++ = mU1;
            *pTex++ = mV2;

            *pTex++ = mU2;
            *pTex++ = mV1;

            *pTex++ = mU2;
            *pTex++ = mV2;

            vbuf->unlock();
        }
    }
    //---------------------------------------------------------------------
    void Plane::updateColors(void)
    {
        if (mInitialised)
        {
            Ogre::HardwareVertexBufferSharedPtr vbuf =
                mRenderOp.vertexData->vertexBufferBinding->getBuffer(COLOR_BINDING);

            Ogre::RGBA* pDest = static_cast<Ogre::RGBA*>(
                                    vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD) );
            Ogre::RGBA color;
            Ogre::Root::getSingleton().convertColourValue(mColour, &color);

            *pDest++ = color;
            *pDest++ = color;
            *pDest++ = color;
            *pDest++ = color;

            vbuf->unlock();
        }
    }
    //---------------------------------------------------------------------
}; // namespace Sonetto
