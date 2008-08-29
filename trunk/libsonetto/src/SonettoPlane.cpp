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
#include <OgrePrerequisites.h>
#include <OgreHardwareBufferManager.h>
#include <OgreRoot.h>
#include <OgreRenderSystem.h>
#include <OgreOverlayManager.h>

namespace Sonetto {
    //---------------------------------------------------------------------
    Ogre::String Plane::msTypeName = "Plane";
    //---------------------------------------------------------------------
    // vertex buffer bindings, set at compile time (we could look these up but no point)
    #define POSITION_BINDING 0
    #define COLOUR_BINDING 1
    #define TEXCOORD_BINDING 2

    //---------------------------------------------------------------------
    Plane::Plane(const Ogre::String& name)
        : OverlayContainer(name)
        , mColorUpdate(false)
        , mTransparent(false)
        // Defer creation of texcoord buffer until we know how big it needs to be
        , mNumTexCoordsInBuffer(0)
        , mU1(0.0)
        , mV1(0.0)
        , mU2(1.0)
        , mV2(1.0)
        , mScrMetricsMode(SMM_RELATIVE_ASPECT_ADJUSTED)
        , mAlphaLevel (1.0f)

    {
        // Init tiling
        for (Ogre::ushort i = 0; i < OGRE_MAX_TEXTURE_COORD_SETS; ++i)
        {
            mTileX[i] = 1.0f;
            mTileY[i] = 1.0f;
        }

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

		Ogre::OverlayContainer::initialise();
		if (init)
		{
			// Setup render op in advance
			mRenderOp.vertexData = new Ogre::VertexData();
			// Vertex declaration: 1 position, add texcoords later depending on #layers
			// Create as separate buffers so we can lock & discard separately
			Ogre::VertexDeclaration* decl = mRenderOp.vertexData->vertexDeclaration;
			decl->addElement(POSITION_BINDING, 0, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
			decl->addElement(COLOUR_BINDING, 0, Ogre::VET_COLOUR, Ogre::VES_DIFFUSE);

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

			// Vertex buffer #2
			vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
				decl->getVertexSize(COLOUR_BINDING), mRenderOp.vertexData->vertexCount,
				Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY// mostly static except during resizing
				);
			// Bind buffer
			mRenderOp.vertexData->vertexBufferBinding->setBinding(COLOUR_BINDING, vbuf);

			// No indexes & issue as a strip
			mRenderOp.useIndexes = false;
			mRenderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_STRIP;

			mInitialised = true;
			mColorUpdate = true;
		}
    }
    //---------------------------------------------------------------------
    void Plane::setTiling(Ogre::Real x, Ogre::Real y, Ogre::ushort layer)
    {
        assert (layer < OGRE_MAX_TEXTURE_COORD_SETS);
        assert (x != 0 && y != 0);

        mTileX[layer] = x;
        mTileY[layer] = y;

        mGeomUVsOutOfDate = true;

    }
    //---------------------------------------------------------------------
    Ogre::Real Plane::getTileX(Ogre::ushort layer) const
    {
        return mTileX[layer];
    }
    //---------------------------------------------------------------------
    Ogre::Real Plane::getTileY(Ogre::ushort layer) const
    {
        return mTileY[layer];
    }
    //---------------------------------------------------------------------
    void Plane::setTransparent(bool isTransparent)
    {
        mTransparent = isTransparent;
    }
    //---------------------------------------------------------------------
    bool Plane::isTransparent(void) const
    {
        return mTransparent;
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
    void Plane::getUV(Ogre::Real& u1, Ogre::Real& v1, Ogre::Real& u2, Ogre::Real& v2) const
    {
		u1 = mU1;
		u2 = mU2;
		v1 = mV1;
		v2 = mV2;
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
        Ogre::OverlayContainer::setMaterialName(matName);
    }
    //---------------------------------------------------------------------
    void Plane::setScrMetricsMode(ScreenMetricsMode smm)
    {
        mScrMetricsMode = smm;
    }
    //---------------------------------------------------------------------
    ScreenMetricsMode Plane::getScrMetricsMode()
    {
        return mScrMetricsMode;
    }
    //---------------------------------------------------------------------
    void Plane::_updateRenderQueue(Ogre::RenderQueue* queue)
    {
        if (mVisible)
        {

            if (!mTransparent && !mpMaterial.isNull())
            {
                Ogre::OverlayElement::_updateRenderQueue(queue);
            }

            // Also add children
            ChildIterator it = getChildIterator();
            while (it.hasMoreElements())
            {
                // Give children ZOrder 1 higher than this
                it.getNext()->_updateRenderQueue(queue);
            }
        }
    }
    void Plane::_update(void)
    {
        Ogre::OverlayContainer::_update();

        if(mColorUpdate && mInitialised)
        {
            updateColour();
            mColorUpdate = false;
        }
    }
    void Plane::setAlpha(Ogre::Real alpha)
    {
        mAlphaLevel = alpha;
        mColorUpdate = true;
    }
    //---------------------------------------------------------------------
    void Plane::updatePositionGeometry(void)
    {
        Ogre::Real aRatio = 1.0f;
        if(mScrMetricsMode == SMM_RELATIVE_ASPECT_ADJUSTED)
        {
            Ogre::Real vpWidth, vpHeight;
            vpWidth = (Ogre::Real) (Ogre::OverlayManager::getSingleton().getViewportWidth());
            vpHeight = (Ogre::Real) (Ogre::OverlayManager::getSingleton().getViewportHeight());
            aRatio = vpHeight/vpWidth;
        }
        /*
			0-----2
			|    /|
			|  /  |
			|/    |
			1-----3
		*/
		Ogre::Real left, right, top, bottom;

		/* Convert positions into -1, 1 coordinate space (homogenous clip space).
			- Left / right is simple range conversion
			- Top / bottom also need inverting since y is upside down - this means
			that top will end up greater than bottom and when computing texture
			coordinates, we have to flip the v-axis (ie. subtract the value from
			1.0 to get the actual correct value).
		*/
		left = _getDerivedLeft() * (aRatio * 2) - aRatio;
		right = left + (mWidth * (aRatio * 2));
		top = -((_getDerivedTop() * 2) - 1);
		bottom =  top -  (mHeight * 2);

		Ogre::HardwareVertexBufferSharedPtr vbuf =
			mRenderOp.vertexData->vertexBufferBinding->getBuffer(POSITION_BINDING);
		float* pPos = static_cast<float*>(
			vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD) );

		// Use the furthest away depth value, since materials should have depth-check off
		// This initialised the depth buffer for any 3D objects in front
		Ogre::Real zValue = Ogre::Root::getSingleton().getRenderSystem()->getMaximumDepthInputValue();
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

		vbuf->unlock();
    }
    //---------------------------------------------------------------------
    void Plane::updateTextureGeometry(void)
    {
        // Generate for as many texture layers as there are in material
        if (!mpMaterial.isNull() && mInitialised)
        {
            // Assume one technique and pass for the moment
            size_t numLayers = mpMaterial->getTechnique(0)->getPass(0)->getNumTextureUnitStates();

            Ogre::VertexDeclaration* decl = mRenderOp.vertexData->vertexDeclaration;
            // Check the number of texcoords we have in our buffer now
            if (mNumTexCoordsInBuffer > numLayers)
            {
                // remove extras
                for (size_t i = mNumTexCoordsInBuffer; i > numLayers; --i)
                {
                    decl->removeElement(Ogre::VES_TEXTURE_COORDINATES,
						static_cast<unsigned short>(i));
                }
            }
            else if (mNumTexCoordsInBuffer < numLayers)
            {
                // Add extra texcoord elements
                size_t offset = Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2) * mNumTexCoordsInBuffer;
                for (size_t i = mNumTexCoordsInBuffer; i < numLayers; ++i)
                {
                    decl->addElement(TEXCOORD_BINDING,
                        offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES,
						static_cast<unsigned short>(i));
                    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);

                }
            }

            // if number of layers changed at all, we'll need to Ogre::Reallocate buffer
            if (mNumTexCoordsInBuffer != numLayers)
            {
                // NB reference counting will take care of the old one if it exists
                Ogre::HardwareVertexBufferSharedPtr newbuf =
                    Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
                    decl->getVertexSize(TEXCOORD_BINDING), mRenderOp.vertexData->vertexCount,
                    Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY // mostly static except during resizing
                    );
                // Bind buffer, note this will unbind the old one and destroy the buffer it had
                mRenderOp.vertexData->vertexBufferBinding->setBinding(TEXCOORD_BINDING, newbuf);
                // Set num tex coords in use now
                mNumTexCoordsInBuffer = numLayers;
            }

            // Get the tcoord buffer & lock
			if (mNumTexCoordsInBuffer)
			{
				Ogre::HardwareVertexBufferSharedPtr vbuf =
					mRenderOp.vertexData->vertexBufferBinding->getBuffer(TEXCOORD_BINDING);
				float* pVBStart = static_cast<float*>(
					vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD) );

				size_t uvSize = Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2) / sizeof(float);
				size_t vertexSize = decl->getVertexSize(TEXCOORD_BINDING) / sizeof(float);
				for (Ogre::ushort i = 0; i < numLayers; ++i)
				{
				    // Calc upper tex coords
                    Ogre::Real upperX = mU2 * mTileX[i];
                    Ogre::Real upperY = mV2 * mTileY[i];


				    /*
					    0-----2
					    |    /|
					    |  /  |
					    |/    |
					    1-----3
				    */
				    // Find start offset for this set
				    float* pTex = pVBStart + (i * uvSize);

                    pTex[0] = mU1;
                    pTex[1] = mV1;

                    pTex += vertexSize; // jump by 1 vertex stride
                    pTex[0] = mU1;
                    pTex[1] = upperY;

                    pTex += vertexSize;
                    pTex[0] = upperX;
                    pTex[1] = mV1;

                    pTex += vertexSize;
                    pTex[0] = upperX;
                    pTex[1] = upperY;
				}
				vbuf->unlock();
			}
        }
    }
    void Plane::updateColour(void)
    {
        Ogre::ColourValue finalColor = mColour;
        finalColor.a = mColour.a * mAlphaLevel;
        Ogre::RGBA colorOut;
        Ogre::Root::getSingleton().convertColourValue(finalColor, &colorOut);

        Ogre::HardwareVertexBufferSharedPtr vbuf =
            mRenderOp.vertexData->vertexBufferBinding->getBuffer(COLOUR_BINDING);

        Ogre::RGBA* pDest = static_cast<Ogre::RGBA*>(
            vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD) );

        *pDest++ = colorOut;
        *pDest++ = colorOut;
        *pDest++ = colorOut;
        *pDest++ = colorOut;

        vbuf->unlock();
    }
}; // namespace Sonetto
