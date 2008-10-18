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

#include "SonettoGauge.h"

#include <OgreHardwareBufferManager.h>
#include <OgrePrerequisites.h>
#include <OgreRoot.h>
#include <OgreRenderSystem.h>
#include <OgreOverlayManager.h>

namespace Sonetto {
    //---------------------------------------------------------------------
    Ogre::String Gauge::msTypeName = "Gauge";
    //---------------------------------------------------------------------
    // vertex buffer bindings, set at compile time (we could look these up but no point)
    #define POSITION_BINDING 0
    #define TEXCOORD_BINDING 1
    #define COLOR_BINDING 2
    //---------------------------------------------------------------------
    Gauge::Gauge(const Ogre::String &name)
    : Ogre::OverlayContainer(name),
    mGaugeRenderable(0),
    mScrMetricsMode(SMM_RELATIVE_ASPECT_ADJUSTED),
    mUpdateColor(true),
    mAspectRatio(1.0f),
    mInclination(1.0f),
    mGaugeSize(0.01875f),
    mBackDistance(0.0020833333333333333333333333333333f),
    mMaxVal(0),
    mCurVal(0)
    {

    }
    //---------------------------------------------------------------------
    Gauge::~Gauge()
    {
        // Delete the vertex data from memory
        delete mRenderOp.vertexData;
        // Delete the Gauge vertex data from memory
        delete mGaugeRenderOp.vertexData;
        // Delete the Gauge Renderable
        delete mGaugeRenderable;
    }
    //---------------------------------------------------------------------
    void Gauge::initialise(void)
    {
        bool init = !mInitialised;

		//OverlayElement::initialise();
		if (init)
		{
		    //---------------------------------------------------------------------
		    // Gauge Background
		    //---------------------------------------------------------------------
			// Setup render op in advance
			mRenderOp.vertexData = new Ogre::VertexData();
			// Vertex declaration: 1 position, add texcoords later depending on #layers
			// Create as separate buffers so we can lock & discard separately
			Ogre::VertexDeclaration* decl = mRenderOp.vertexData->vertexDeclaration;
			size_t offset = 0;
			// Positions
			decl->addElement(POSITION_BINDING, 0, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
			offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
			// Textures
			decl->addElement(TEXCOORD_BINDING, 0, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);
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

			//---------------------------------------------------------------------
		    // Gauge
		    //---------------------------------------------------------------------
		    mGaugeRenderable = new GaugeRenderable(this);
			// Setup render op in advance
			mGaugeRenderOp.vertexData = new Ogre::VertexData();
			// Vertex declaration: 1 position, add texcoords later depending on #layers
			// Create as separate buffers so we can lock & discard separately
			decl = mGaugeRenderOp.vertexData->vertexDeclaration;
            offset = 0;
			// Positions
			decl->addElement(POSITION_BINDING, 0, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
			offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
			// Texcoord
			decl->addElement(TEXCOORD_BINDING, 0, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);
			offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);
			// Colours - store these in a separate buffer because they change less often
			decl->addElement(COLOR_BINDING, 0, Ogre::VET_COLOUR, Ogre::VES_DIFFUSE);

			// Basic vertex data
			mGaugeRenderOp.vertexData->vertexStart = 0;
			mGaugeRenderOp.vertexData->vertexCount = 4;

			// Vertex buffer #1
			vbuf =
				Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
				decl->getVertexSize(POSITION_BINDING), mGaugeRenderOp.vertexData->vertexCount,
				Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY// mostly static except during resizing
				);
			// Bind buffer
			mGaugeRenderOp.vertexData->vertexBufferBinding->setBinding(POSITION_BINDING, vbuf);

			vbuf =
				Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
				decl->getVertexSize(TEXCOORD_BINDING), mGaugeRenderOp.vertexData->vertexCount,
				Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY// mostly static except during resizing
				);
			// Bind buffer
			mGaugeRenderOp.vertexData->vertexBufferBinding->setBinding(TEXCOORD_BINDING, vbuf);

			vbuf =
				Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
				decl->getVertexSize(COLOR_BINDING), mGaugeRenderOp.vertexData->vertexCount,
				Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY// mostly static except during resizing
				);
			// Bind buffer
			mGaugeRenderOp.vertexData->vertexBufferBinding->setBinding(COLOR_BINDING, vbuf);

			// No indexes & issue as a strip
			mGaugeRenderOp.useIndexes = false;
			mGaugeRenderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_STRIP;
            // set the flag to update the colors
            mUpdateColor = true;
			mInitialised = true;
		}
    }
    //---------------------------------------------------------------------
    void Gauge::setMaxValPtr(int * ptr)
    {
        mMaxVal = ptr;
    }
    //---------------------------------------------------------------------
    int * Gauge::getMaxValPtr()
    {
        return mMaxVal;
    }
    //---------------------------------------------------------------------
    void Gauge::setCurValPtr(int * ptr)
    {
        mCurVal = ptr;
    }
    //---------------------------------------------------------------------
    int * Gauge::getCurValPtr()
    {
        return mCurVal;
    }
    //---------------------------------------------------------------------
    void Gauge::setGaugeColor(const Ogre::ColourValue &lcolor, const Ogre::ColourValue &rcolor)
    {
        mLeftColor = lcolor;
        mRightColor = rcolor;
        mUpdateColor = true;
    }
    //---------------------------------------------------------------------
    Ogre::ColourValue Gauge::getLeftGaugeColor()
    {
        return mLeftColor;
    }
    //---------------------------------------------------------------------
    Ogre::ColourValue Gauge::getRightGaugeColor()
    {
        return mRightColor;
    }
    //---------------------------------------------------------------------
    void Gauge::setBackColor(const Ogre::ColourValue &color)
    {
        mBgColor = color;
        mUpdateColor = true;
    }
    //---------------------------------------------------------------------
    Ogre::ColourValue Gauge::getBackColor()
    {
        return mBgColor;
    }
    //---------------------------------------------------------------------
    void Gauge::setGaugeInclination(const Ogre::Real incl)
    {
        mInclination = incl;
    }
    //---------------------------------------------------------------------
    Ogre::Real Gauge::getGaugeInclination()
    {
        return mInclination;
    }
    //---------------------------------------------------------------------
    void Gauge::setGaugeSize(const Ogre::Real size)
    {
        mGaugeSize = size;
    }
    //---------------------------------------------------------------------
    Ogre::Real Gauge::getGaugeSize()
    {
        return mGaugeSize;
    }
    //---------------------------------------------------------------------
    void Gauge::setBackDistance(const Ogre::Real size)
    {
        mBackDistance = size;
    }
    //---------------------------------------------------------------------
    Ogre::Real Gauge::getBackDistance()
    {
        return mBackDistance;
    }
    //---------------------------------------------------------------------
    const Ogre::String& Gauge::getTypeName(void) const
    {
        return msTypeName;
    }
    //---------------------------------------------------------------------
    void Gauge::getRenderOperation(Ogre::RenderOperation& op)
    {
        op = mRenderOp;
    }
    //---------------------------------------------------------------------
    void Gauge::setMaterialName(const Ogre::String& matName)
    {
        Ogre::OverlayElement::setMaterialName(matName);
    }
    //---------------------------------------------------------------------
    void Gauge::_updateRenderQueue(Ogre::RenderQueue * queue)
    {
        // Add the renderable to the update render queue
        queue->addRenderable(mGaugeRenderable, Ogre::RENDER_QUEUE_OVERLAY, mZOrder+1);
        // Now call the base class function
        OverlayElement::_updateRenderQueue(queue);

        ChildIterator it = getChildIterator();
        while (it.hasMoreElements())
        {
            // Give children ZOrder 1 higher than this
            it.getNext()->_updateRenderQueue(queue);
        }
    }
    //---------------------------------------------------------------------
    /*
    void Gauge::visitRenderables(Ogre::Renderable::Visitor* visitor, bool debugRenderables)
    {
        visitor->visit(mGaugeRenderable, 0, false);
        Ogre::OverlayElement::visitRenderables(visitor, debugRenderables);
    }
    */
    //---------------------------------------------------------------------
    void Gauge::_update(void)
    {
        Ogre::OverlayElement::_update();
        // Calculate the aspect ratio
        Ogre::Real vpWidth, vpHeight;
        // Get the Viewport Width and Height
        vpWidth = (Ogre::Real) Ogre::OverlayManager::getSingleton().getViewportWidth();
        vpHeight = (Ogre::Real) Ogre::OverlayManager::getSingleton().getViewportHeight();
        // Calculate the aspect ratio
        mAspectRatio = vpHeight/vpWidth;

        // Check if we need to update the colors
        if(mUpdateColor)
        {
            updateColors();
            mUpdateColor = false;
        }

        // We'll set the object to update its position constantly
        // As there is no way to know if the values changed
        mGeomPositionsOutOfDate = true;
        // And as we don't (currently) use textures, there is no need to update it.
        mGeomUVsOutOfDate       = false;

        // Now call the Overlay Container update function.
        Ogre::OverlayContainer::_update();
    }
    //---------------------------------------------------------------------
    void Gauge::updatePositionGeometry(void)
    {
        //---------------------------------------------------------------------
        // Gauge Background
        //---------------------------------------------------------------------
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
        // Calculate the positions
        left = ((_getDerivedLeft() + mBackDistance) * (mAspect * 2.0f) - mAspect);
		right = left + (mWidth * (mAspect * 2.0f));
		top = -(((_getDerivedTop() + mBackDistance) * 2.0f) - 1.0f);
		bottom =  top -  (mGaugeSize * 2.0f);

		// Vertex 0
        *pPos++ = left + ((mInclination * mGaugeSize) * (mAspect * 2.0f));
		*pPos++ = top;
		*pPos++ = zValue;
        // Vertex 1
		*pPos++ = left;
		*pPos++ = bottom;
		*pPos++ = zValue;
        // Vertex 2
		*pPos++ = right + ((mInclination * mGaugeSize) * (mAspect * 2.0f));
		*pPos++ = top;
		*pPos++ = zValue;
        // Vertex 3
		*pPos++ = right;
		*pPos++ = bottom;
		*pPos++ = zValue;
        // We have finished here, unlock the vertex buffer.
        vbuf->unlock();
        //---------------------------------------------------------------------
        // Gauge
        //---------------------------------------------------------------------
        // Get the Vertex Buffer (for positions)
        vbuf =
            mGaugeRenderOp.vertexData->vertexBufferBinding->getBuffer(POSITION_BINDING);
        // Cast the vertex buffer to a single float
        pPos = static_cast<float*>(
                          vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD) );

        /*
			0-----2
			|    /|
			|  /  |
			|/    |
			1-----3
		*/

        // Set the positions
		left = (_getDerivedLeft() * (mAspect * 2.0f) - mAspect);
		right = left + ((mWidth * ((float)(*mCurVal)/(*mMaxVal)))* (mAspect * 2.0f));
		top = -((_getDerivedTop() * 2.0f) - 1.0f);
		bottom =  top -  (mGaugeSize * 2.0f);

        // Get (again) the maximun depth input value
        //zValue = Ogre::Root::getSingleton().getRenderSystem()->getMaximumDepthInputValue();
        // Vertex 0
        *pPos++ = left + ((mInclination * mGaugeSize) * (mAspect * 2.0f));
		*pPos++ = top;
		*pPos++ = zValue;
        // Vertex 1
		*pPos++ = left;
		*pPos++ = bottom;
		*pPos++ = zValue;
        // Vertex 2
		*pPos++ = right + ((mInclination * mGaugeSize) * (mAspect * 2.0f));
		*pPos++ = top;
		*pPos++ = zValue;
        // Vertex 3
		*pPos++ = right;
		*pPos++ = bottom;
		*pPos++ = zValue;

        // We have finished here, unlock the vertex buffer.
        vbuf->unlock();
    }
    //---------------------------------------------------------------------
    void Gauge::updateTextureGeometry(void)
    {
        // Generate for as many texture layers as there are in material
        if (mInitialised)
        {
            Ogre::HardwareVertexBufferSharedPtr vbuf =
            mRenderOp.vertexData->vertexBufferBinding->getBuffer(TEXCOORD_BINDING);
            float* pTex = static_cast<float*>(
                              vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD) );
            // Vertex 0
            *pTex++ = 0.0f;
            *pTex++ = 0.0f;
            // Vertex 1
            *pTex++ = 0.0f;
            *pTex++ = 1.0f;
            // Vertex 2
            *pTex++ = 1.0f;
            *pTex++ = 0.0f;
            // Vertex 3
            *pTex++ = 1.0f;
            *pTex++ = 1.0f;

            vbuf->unlock();
            // Gauge Renderable
            vbuf =
            mGaugeRenderOp.vertexData->vertexBufferBinding->getBuffer(TEXCOORD_BINDING);
            pTex = static_cast<float*>(
                              vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD) );
            // Vertex 0
            *pTex++ = 0.0f;
            *pTex++ = 0.0f;
            // Vertex 1
            *pTex++ = 0.0f;
            *pTex++ = 1.0f;
            // Vertex 2
            *pTex++ = 1.0f;
            *pTex++ = 0.0f;
            // Vertex 3
            *pTex++ = 1.0f;
            *pTex++ = 1.0f;

            vbuf->unlock();
        }
    }
    //---------------------------------------------------------------------
    void Gauge::updateColors(void)
    {
        if (mInitialised)
        {
            Ogre::RGBA leftColor, rightColor, backColor;
            Ogre::Root::getSingleton().convertColourValue(mLeftColor, &leftColor);
            Ogre::Root::getSingleton().convertColourValue(mRightColor, &rightColor);
            Ogre::Root::getSingleton().convertColourValue(mBgColor, &backColor);
            //---------------------------------------------------------------------
            // Gauge Background
            //---------------------------------------------------------------------
            Ogre::HardwareVertexBufferSharedPtr vbuf =
                mRenderOp.vertexData->vertexBufferBinding->getBuffer(COLOR_BINDING);

            Ogre::RGBA* pDest = static_cast<Ogre::RGBA*>(
                                    vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD) );

            *pDest++ = backColor;
            *pDest++ = backColor;
            *pDest++ = backColor;
            *pDest++ = backColor;

            vbuf->unlock();
            //---------------------------------------------------------------------
            // Gauge
            //---------------------------------------------------------------------
            vbuf =
                mGaugeRenderOp.vertexData->vertexBufferBinding->getBuffer(COLOR_BINDING);

            pDest = static_cast<Ogre::RGBA*>(
                                    vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD) );

            *pDest++ = leftColor;
            *pDest++ = leftColor;
            *pDest++ = rightColor;
            *pDest++ = rightColor;

            vbuf->unlock();
        }
    }
    //---------------------------------------------------------------------
};  // namespace Sonetto
