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
#include "SonettoSlimWindow.h"
#include <OgreStableHeaders.h>
#include <OgreOverlayManager.h>
#include <OgreMaterial.h>
#include <OgreMaterialManager.h>
#include <OgreTechnique.h>
#include <OgrePass.h>
#include <OgreStringConverter.h>
#include <OgreHardwareBufferManager.h>
#include <OgreRoot.h>
#include <OgreRenderSystem.h>

namespace Sonetto
{
    Ogre::String SlimWindow::msTypeName = "SlimWindow";
    #define POSITION_BINDING 0
    #define COLOUR_BINDING 1
    #define TEXCOORD_BINDING 2
    SlimWindow::SlimWindow(const Ogre::String& name): Ogre::OverlayContainer(name),
            mUpdateBorderColors(true),
            mUpdateBackgroundColors(true),
            mBorderRenderable(0),
            mScrMetricsMode(SMM_RELATIVE_ASPECT_ADJUSTED),
            mAspectRatio(1.3333333333333333333333333333333f)
    {
        //mScrMetricsMode = SMM_RELATIVE_ASPECT_ADJUSTED;
    }
    SlimWindow::~SlimWindow()
    {
        delete mRenderOp.vertexData;
        delete mRenderOp2.vertexData;
        delete mBorderRenderable;
    }
    void SlimWindow::initialise (void)
    {
        bool init = !mInitialised;
        OverlayContainer::initialise();
        if (init)
        {
            //-------------------
            // SlimWindow background
            //-------------------
            // Set up the Render Operation in advance
            mRenderOp.vertexData = new Ogre::VertexData();
            // VertexDeclaration
            Ogre::VertexDeclaration * decl = mRenderOp.vertexData->vertexDeclaration;
            size_t offset = 0;
            decl->addElement(POSITION_BINDING, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
            //offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
            decl->addElement(COLOUR_BINDING, offset, Ogre::VET_COLOUR, Ogre::VES_DIFFUSE);
            //offset += Ogre::VertexElement::getTypeSize(Ogre::VET_COLOUR);
            decl->addElement(TEXCOORD_BINDING, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, 0);
            offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);
            decl->addElement(TEXCOORD_BINDING, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, 1);
            offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);
            // Vertex data
            mRenderOp.vertexData->vertexStart = 0;
            mRenderOp.vertexData->vertexCount = SLIM_NUM_VERTEX;
            // Vertex Buffer #1 (Position)
            Ogre::HardwareVertexBufferSharedPtr vbuf =
                Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
                    decl->getVertexSize(POSITION_BINDING), mRenderOp.vertexData->vertexCount,
                    Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY
                );
            // Bind buffer
            mRenderOp.vertexData->vertexBufferBinding->setBinding(POSITION_BINDING, vbuf);
            vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
                       decl->getVertexSize(COLOUR_BINDING), mRenderOp.vertexData->vertexCount,
                       Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY
                   );
            // Bind buffer
            mRenderOp.vertexData->vertexBufferBinding->setBinding(COLOUR_BINDING, vbuf);
            vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
                       decl->getVertexSize(TEXCOORD_BINDING), mRenderOp.vertexData->vertexCount,
                       Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY
                   );
            // Bind buffer
            mRenderOp.vertexData->vertexBufferBinding->setBinding(TEXCOORD_BINDING, vbuf);
            mRenderOp.useIndexes = false;
            mRenderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
            //----------------------
            // SlimWindow border
            //----------------------
            mBorderRenderable = new SlimBorderRenderable(this);
            // Set up the Render Operation in advance
            mRenderOp2.vertexData = new Ogre::VertexData();
            // VertexDeclaration
            decl = mRenderOp2.vertexData->vertexDeclaration;
            offset = 0;
            decl->addElement(POSITION_BINDING, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
            //offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
            decl->addElement(COLOUR_BINDING, offset, Ogre::VET_COLOUR, Ogre::VES_DIFFUSE);
            //offset += Ogre::VertexElement::getTypeSize(Ogre::VET_COLOUR);
            decl->addElement(TEXCOORD_BINDING, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, 0);
            offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);
            // Vertex data
            mRenderOp2.vertexData->vertexStart = 0;
            mRenderOp2.vertexData->vertexCount = SLIM_NUM_VERTEX;

            // Vertex Buffer #1 (Position)
            vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
                       decl->getVertexSize(POSITION_BINDING), mRenderOp2.vertexData->vertexCount,
                       Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY
                   );
            // Bind buffer
            mRenderOp2.vertexData->vertexBufferBinding->setBinding(POSITION_BINDING, vbuf);
            // Vertex Buffer #2 (Color)
            vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
                       decl->getVertexSize(COLOUR_BINDING), mRenderOp2.vertexData->vertexCount,
                       Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY
                   );
            // Bind buffer
            mRenderOp2.vertexData->vertexBufferBinding->setBinding(COLOUR_BINDING, vbuf);
            // Vertex Buffer #3 (Texcoord)
            vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
                       decl->getVertexSize(TEXCOORD_BINDING), mRenderOp2.vertexData->vertexCount,
                       Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY
                   );
            // Bind buffer
            mRenderOp2.vertexData->vertexBufferBinding->setBinding(TEXCOORD_BINDING, vbuf);
            mRenderOp2.useIndexes = false;
            mRenderOp2.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
            mInitialised = true;
            std::cout<<"\n\nInitialisation OK!\n\n";
        }
    }
    void SlimWindow::setWindowSkin(WindowSkinPtr winSkin)
    {
        mWindowSkin = winSkin;
        Ogre::OverlayContainer::setMaterialName(mWindowSkin->mWindowMaterial->getName());
        mWindowSkin->mBorderMaterial->load();
    }
    void SlimWindow::setWindowType(WindowType wt)
    {
        mWindowType = wt;
    }
    WindowType SlimWindow::getWindowType()
    {
        return mWindowType;
    }
    void SlimWindow::setWindowAlpha(Ogre::Real alpha)
    {
        mWindowAlpha = alpha;
    }
    Ogre::Real SlimWindow::getWindowAlpha()
    {
        return mWindowAlpha;
    }
    void SlimWindow::setBorderAlpha(Ogre::Real alpha)
    {
        mBorderAlpha = alpha;
    }
    Ogre::Real SlimWindow::getBorderAlpha()
    {
        return mBorderAlpha;
    }
    void SlimWindow::setScrMetricsMode(ScreenMetricsMode smm)
    {
        mScrMetricsMode = smm;
    }
    const Ogre::String& SlimWindow::getTypeName(void) const
    {
        return msTypeName;
    }
    void SlimWindow::getRenderOperation(Ogre::RenderOperation &op)
    {
        op = mRenderOp;
    }
    void SlimWindow::_updateRenderQueue(Ogre::RenderQueue * queue)
    {
        if (mVisible)
        {
            queue->addRenderable(mBorderRenderable, Ogre::RENDER_QUEUE_OVERLAY, mZOrder+1);

            if (!mpMaterial.isNull())
            {
                OverlayElement::_updateRenderQueue(queue);
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
    void SlimWindow::visitRenderables(Renderable::Visitor* visitor, bool debugRenderables)
    {
        visitor->visit(mBorderRenderable, 0, false);
        OverlayContainer::visitRenderables(visitor, debugRenderables);
    }
    void SlimWindow::_update(void)
    {
        Ogre::Real vpWidth, vpHeight;
        vpWidth = (Ogre::Real) (Ogre::OverlayManager::getSingleton().getViewportWidth());
        vpHeight = (Ogre::Real) (Ogre::OverlayManager::getSingleton().getViewportHeight());
        Ogre::Real tmpVPAspectCoef = vpHeight/vpWidth;

        if (tmpVPAspectCoef != mAspectRatio)
        {
            mAspectRatio = tmpVPAspectCoef;

            calculatePosition();
            calculateTexCoord();

        };
        if (mUpdateBorderColors && !mGeomPositionsOutOfDate)
            updateBorderColors();

        if (mUpdateBackgroundColors && !mGeomPositionsOutOfDate)
            updateBackgroundColors();
        Ogre::OverlayContainer::_update();
    }

    void SlimWindow::updatePositionGeometry(void)
    {
        /*
        	0-----2
        	|    /|
        	|  /  |
        	|/    |
        	1-----3
        */
        Ogre::HardwareVertexBufferSharedPtr vbuf =
            mRenderOp.vertexData->vertexBufferBinding->getBuffer(POSITION_BINDING);
        float* pPos = static_cast<float*>(
                          vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD) );
        Ogre::Real zValue = Ogre::Root::getSingleton().getRenderSystem()->getMaximumDepthInputValue();

        float mAspect = mAspectRatio;

        if(mScrMetricsMode == SMM_RELATIVE)
            mAspect = 1.0f;

        if (mScrMetricsMode == SMM_RELATIVE_ASPECT_ADJUSTED)
            mAspect = mAspectRatio;

        for (size_t i = 0; i < mRenderOp.vertexData->vertexCount; ++i)
        {
            *pPos++ = ((mPosCoord[i].x * (mAspect * 2)) - mAspect);
            *pPos++ = -(mPosCoord[i].y * 2) + 1;
            *pPos++ = zValue;
        }
        vbuf->unlock();
        vbuf = mRenderOp2.vertexData->vertexBufferBinding->getBuffer(POSITION_BINDING);
        float * pBorderPos = static_cast<float*>(
                                 vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD) );

        for (size_t i = 0; i < mRenderOp2.vertexData->vertexCount; ++i)
        {
            *pBorderPos++ = ((mPosCoord[i].x * (mAspect * 2)) - mAspect);
            *pBorderPos++ = -(mPosCoord[i].y * 2) + 1;
            *pBorderPos++ = zValue;
        }
        vbuf->unlock();
    }
    void SlimWindow::updateTextureGeometry(void)
    {
        Ogre::HardwareVertexBufferSharedPtr vbuf =
            mRenderOp.vertexData->vertexBufferBinding->getBuffer(TEXCOORD_BINDING);
        float* pTex = static_cast<float*>(
                          vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD) );
        /*
        	0---2 3
        	|  / /|
        	| / / |
        	|/ /  |
        	1 4---5
        */
        // Texcoord 0
        *pTex++ = mWindowSkin->mWinTexCoord.slim_left.left;
        *pTex++ = mWindowSkin->mWinTexCoord.slim_left.top;
        // Texcoord 1
        *pTex++ = mTexCoord1[0].x;
        *pTex++ = mTexCoord1[0].y;
        // Texcoord 0
        *pTex++ = mWindowSkin->mWinTexCoord.slim_left.left;
        *pTex++ = mWindowSkin->mWinTexCoord.slim_left.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord1[1].x;
        *pTex++ = mTexCoord1[1].y;
        // Texcoord 0
        *pTex++ = mWindowSkin->mWinTexCoord.slim_left.right;
        *pTex++ = mWindowSkin->mWinTexCoord.slim_left.top;
        // Texcoord 1
        *pTex++ = mTexCoord1[2].x;
        *pTex++ = mTexCoord1[2].y;
        // Texcoord 0
        *pTex++ = mWindowSkin->mWinTexCoord.slim_left.right;
        *pTex++ = mWindowSkin->mWinTexCoord.slim_left.top;
        // Texcoord 1
        *pTex++ = mTexCoord1[3].x;
        *pTex++ = mTexCoord1[3].y;
        // Texcoord 0
        *pTex++ = mWindowSkin->mWinTexCoord.slim_left.left;
        *pTex++ = mWindowSkin->mWinTexCoord.slim_left.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord1[4].x;
        *pTex++ = mTexCoord1[4].y;
        // Texcoord 0
        *pTex++ = mWindowSkin->mWinTexCoord.slim_left.right;
        *pTex++ = mWindowSkin->mWinTexCoord.slim_left.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord1[5].x;
        *pTex++ = mTexCoord1[5].y;

        // Texcoord 0
        *pTex++ = mWindowSkin->mWinTexCoord.slim_center.left;
        *pTex++ = mWindowSkin->mWinTexCoord.slim_center.top;
        // Texcoord 1
        *pTex++ = mTexCoord1[6].x;
        *pTex++ = mTexCoord1[6].y;
        // Texcoord 0
        *pTex++ = mWindowSkin->mWinTexCoord.slim_center.left;
        *pTex++ = mWindowSkin->mWinTexCoord.slim_center.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord1[7].x;
        *pTex++ = mTexCoord1[7].y;
        // Texcoord 0
        *pTex++ = mWindowSkin->mWinTexCoord.slim_center.right;
        *pTex++ = mWindowSkin->mWinTexCoord.slim_center.top;
        // Texcoord 1
        *pTex++ = mTexCoord1[8].x;
        *pTex++ = mTexCoord1[8].y;
        // Texcoord 0
        *pTex++ = mWindowSkin->mWinTexCoord.slim_center.right;
        *pTex++ = mWindowSkin->mWinTexCoord.slim_center.top;
        // Texcoord 1
        *pTex++ = mTexCoord1[9].x;
        *pTex++ = mTexCoord1[9].y;
        // Texcoord 0
        *pTex++ = mWindowSkin->mWinTexCoord.slim_center.left;
        *pTex++ = mWindowSkin->mWinTexCoord.slim_center.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord1[10].x;
        *pTex++ = mTexCoord1[10].y;
        // Texcoord 0
        *pTex++ = mWindowSkin->mWinTexCoord.slim_center.right;
        *pTex++ = mWindowSkin->mWinTexCoord.slim_center.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord1[11].x;
        *pTex++ = mTexCoord1[11].y;

        // Texcoord 0
        *pTex++ = mWindowSkin->mWinTexCoord.slim_right.left;
        *pTex++ = mWindowSkin->mWinTexCoord.slim_right.top;
        // Texcoord 1
        *pTex++ = mTexCoord1[12].x;
        *pTex++ = mTexCoord1[12].y;
        // Texcoord 0
        *pTex++ = mWindowSkin->mWinTexCoord.slim_right.left;
        *pTex++ = mWindowSkin->mWinTexCoord.slim_right.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord1[13].x;
        *pTex++ = mTexCoord1[13].y;
        // Texcoord 0
        *pTex++ = mWindowSkin->mWinTexCoord.slim_right.right;
        *pTex++ = mWindowSkin->mWinTexCoord.slim_right.top;
        // Texcoord 1
        *pTex++ = mTexCoord1[14].x;
        *pTex++ = mTexCoord1[14].y;
        // Texcoord 0
        *pTex++ = mWindowSkin->mWinTexCoord.slim_right.right;
        *pTex++ = mWindowSkin->mWinTexCoord.slim_right.top;
        // Texcoord 1
        *pTex++ = mTexCoord1[15].x;
        *pTex++ = mTexCoord1[15].y;
        // Texcoord 0
        *pTex++ = mWindowSkin->mWinTexCoord.slim_right.left;
        *pTex++ = mWindowSkin->mWinTexCoord.slim_right.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord1[16].x;
        *pTex++ = mTexCoord1[16].y;
        // Texcoord 0
        *pTex++ = mWindowSkin->mWinTexCoord.slim_right.right;
        *pTex++ = mWindowSkin->mWinTexCoord.slim_right.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord1[17].x;
        *pTex++ = mTexCoord1[17].y;

        vbuf->unlock();

        vbuf = mRenderOp2.vertexData->vertexBufferBinding->getBuffer(TEXCOORD_BINDING);
        pTex = static_cast<float*>(
                                 vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD) );

        /*
        	0---2 3
        	|  / /|
        	| / / |
        	|/ /  |
        	1 4---5
        */
        // Texcoord 0
        *pTex++ = mWindowSkin->mWinTexCoord.slim_left.left;
        *pTex++ = mWindowSkin->mWinTexCoord.slim_left.top;
        // Texcoord 0
        *pTex++ = mWindowSkin->mWinTexCoord.slim_left.left;
        *pTex++ = mWindowSkin->mWinTexCoord.slim_left.bottom;
        // Texcoord 0
        *pTex++ = mWindowSkin->mWinTexCoord.slim_left.right;
        *pTex++ = mWindowSkin->mWinTexCoord.slim_left.top;
        // Texcoord 0
        *pTex++ = mWindowSkin->mWinTexCoord.slim_left.right;
        *pTex++ = mWindowSkin->mWinTexCoord.slim_left.top;
        // Texcoord 0
        *pTex++ = mWindowSkin->mWinTexCoord.slim_left.left;
        *pTex++ = mWindowSkin->mWinTexCoord.slim_left.bottom;
        // Texcoord 0
        *pTex++ = mWindowSkin->mWinTexCoord.slim_left.right;
        *pTex++ = mWindowSkin->mWinTexCoord.slim_left.bottom;

        // Texcoord 0
        *pTex++ = mWindowSkin->mWinTexCoord.slim_center.left;
        *pTex++ = mWindowSkin->mWinTexCoord.slim_center.top;
        // Texcoord 0
        *pTex++ = mWindowSkin->mWinTexCoord.slim_center.left;
        *pTex++ = mWindowSkin->mWinTexCoord.slim_center.bottom;
        // Texcoord 0
        *pTex++ = mWindowSkin->mWinTexCoord.slim_center.right;
        *pTex++ = mWindowSkin->mWinTexCoord.slim_center.top;
        // Texcoord 0
        *pTex++ = mWindowSkin->mWinTexCoord.slim_center.right;
        *pTex++ = mWindowSkin->mWinTexCoord.slim_center.top;
        // Texcoord 0
        *pTex++ = mWindowSkin->mWinTexCoord.slim_center.left;
        *pTex++ = mWindowSkin->mWinTexCoord.slim_center.bottom;
        // Texcoord 0
        *pTex++ = mWindowSkin->mWinTexCoord.slim_center.right;
        *pTex++ = mWindowSkin->mWinTexCoord.slim_center.bottom;

        // Texcoord 0
        *pTex++ = mWindowSkin->mWinTexCoord.slim_right.left;
        *pTex++ = mWindowSkin->mWinTexCoord.slim_right.top;
        // Texcoord 0
        *pTex++ = mWindowSkin->mWinTexCoord.slim_right.left;
        *pTex++ = mWindowSkin->mWinTexCoord.slim_right.bottom;
        // Texcoord 0
        *pTex++ = mWindowSkin->mWinTexCoord.slim_right.right;
        *pTex++ = mWindowSkin->mWinTexCoord.slim_right.top;
        // Texcoord 0
        *pTex++ = mWindowSkin->mWinTexCoord.slim_right.right;
        *pTex++ = mWindowSkin->mWinTexCoord.slim_right.top;
        // Texcoord 0
        *pTex++ = mWindowSkin->mWinTexCoord.slim_right.left;
        *pTex++ = mWindowSkin->mWinTexCoord.slim_right.bottom;
        // Texcoord 0
        *pTex++ = mWindowSkin->mWinTexCoord.slim_right.right;
        *pTex++ = mWindowSkin->mWinTexCoord.slim_right.bottom;

        vbuf->unlock();
    }

    void SlimWindow::updateBorderColors(void)
    {
        if (mInitialised)
        {
            Ogre::HardwareVertexBufferSharedPtr vbuf =
                mRenderOp2.vertexData->vertexBufferBinding->getBuffer(COLOUR_BINDING);

            Ogre::RGBA* pDest = static_cast<Ogre::RGBA*>(
                                    vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD) );
            Ogre::RGBA color;
            Ogre::Root::getSingleton().convertColourValue(mBorderColor, &color);

            for (size_t i = 0; i < mRenderOp2.vertexData->vertexCount; ++i)
            {
                *pDest++ = color;
            }

            vbuf->unlock();

            std::cout<<"\n\nUpdate Border Colors OK!\n\n";

            mUpdateBorderColors = false;
        }
    }

    void SlimWindow::updateBackgroundColors(void)
    {
        if (mInitialised)
        {
            Ogre::HardwareVertexBufferSharedPtr vbuf =
                mRenderOp.vertexData->vertexBufferBinding->getBuffer(COLOUR_BINDING);

            Ogre::RGBA* pDest = static_cast<Ogre::RGBA*>(
                                    vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD) );
            Ogre::RGBA color;
            Ogre::Root::getSingleton().convertColourValue(mBackgroundColor, &color);

            for (size_t i = 0; i < mRenderOp2.vertexData->vertexCount; ++i)
            {
                *pDest++ = color;
            }

            vbuf->unlock();

            std::cout<<"\n\nUpdate Background Colors OK!\n\n";

            mUpdateBackgroundColors = false;
        }
    }

    void SlimWindow::calculatePosition(void)
    {
        float left, right, top, bottom;

        left = _getDerivedLeft();
        right = left + mWidth;
        top = _getDerivedTop();
        bottom =  top + mWindowSkin->mBorderDimensions.y;

        mPosCoord[0].x = left;
        mPosCoord[0].y = top;
        mPosCoord[1].x = left;
        mPosCoord[1].y = bottom;
        mPosCoord[2].x = left+mWindowSkin->mBorderDimensions.x;
        mPosCoord[2].y = top;
        mPosCoord[3].x = left+mWindowSkin->mBorderDimensions.x;
        mPosCoord[3].y = top;
        mPosCoord[4].x = left;
        mPosCoord[4].y = bottom;
        mPosCoord[5].x = left+mWindowSkin->mBorderDimensions.x;
        mPosCoord[5].y = bottom;

        mPosCoord[6].x = left+mWindowSkin->mBorderDimensions.x;
        mPosCoord[6].y = top;
        mPosCoord[7].x = left+mWindowSkin->mBorderDimensions.x;
        mPosCoord[7].y = bottom;
        mPosCoord[8].x = right-mWindowSkin->mBorderDimensions.x;
        mPosCoord[8].y = top;
        mPosCoord[9].x = right-mWindowSkin->mBorderDimensions.x;
        mPosCoord[9].y = top;
        mPosCoord[10].x = left+mWindowSkin->mBorderDimensions.x;
        mPosCoord[10].y = bottom;
        mPosCoord[11].x = right-mWindowSkin->mBorderDimensions.x;
        mPosCoord[11].y = bottom;

        mPosCoord[12].x = right-mWindowSkin->mBorderDimensions.x;
        mPosCoord[12].y = top;
        mPosCoord[13].x = right-mWindowSkin->mBorderDimensions.x;
        mPosCoord[13].y = bottom;
        mPosCoord[14].x = right;
        mPosCoord[14].y = top;
        mPosCoord[15].x = right;
        mPosCoord[15].y = top;
        mPosCoord[16].x = right-mWindowSkin->mBorderDimensions.x;
        mPosCoord[16].y = bottom;
        mPosCoord[17].x = right;
        mPosCoord[17].y = bottom;

        mGeomPositionsOutOfDate = true;
    }

    void SlimWindow::calculateTexCoord(void)
    {
        // TEMPORARY: Set up a test vertex mesh!
        /*
        0---2   3
        |  /   /|
        | /<->/ |
        |/   /  |
        1   4---5
        */
        float left, right, top, bottom;
        Ogre::Vector2 mBorderFixed;

        if (!mWindowSkin->mTileMode) // If the texture mode is on stretch mode
        {
            left = 0.0f;
            right = mWindowSkin->mTiling.x;
            top = 0.0f;
            bottom =  mWindowSkin->mTiling.y;
            mBorderFixed.x = (mWindowSkin->mBorderDimensions.x / mWidth)*mWindowSkin->mTiling.x;
            mBorderFixed.y = (mWindowSkin->mBorderDimensions.y / mHeight)*mWindowSkin->mTiling.y;
        }
        else // Else if the texture mode is on tile mode
        {
            left = _getDerivedLeft() * mWindowSkin->mTiling.x;
            right = left + (mWidth * mWindowSkin->mTiling.x);
            top = _getDerivedTop() * mWindowSkin->mTiling.y;
            bottom =  top + (mWindowSkin->mBorderDimensions.y * mWindowSkin->mTiling.y);
            mBorderFixed.x = mWindowSkin->mBorderDimensions.x*mWindowSkin->mTiling.x;
            mBorderFixed.y = mWindowSkin->mBorderDimensions.y*mWindowSkin->mTiling.y;
        }

        mTexCoord1[0].x = left;
        mTexCoord1[0].y = top;
        mTexCoord1[1].x = left;
        mTexCoord1[1].y = bottom;
        mTexCoord1[2].x = left+mBorderFixed.x;
        mTexCoord1[2].y = top;
        mTexCoord1[3].x = left+mBorderFixed.x;
        mTexCoord1[3].y = top;
        mTexCoord1[4].x = left;
        mTexCoord1[4].y = bottom;
        mTexCoord1[5].x = left+mBorderFixed.x;
        mTexCoord1[5].y = bottom;

        mTexCoord1[6].x = left+mBorderFixed.x;
        mTexCoord1[6].y = top;
        mTexCoord1[7].x = left+mBorderFixed.x;
        mTexCoord1[7].y = bottom;
        mTexCoord1[8].x = right-mBorderFixed.x;
        mTexCoord1[8].y = top;
        mTexCoord1[9].x = right-mBorderFixed.x;
        mTexCoord1[9].y = top;
        mTexCoord1[10].x = left+mBorderFixed.x;
        mTexCoord1[10].y = bottom;
        mTexCoord1[11].x = right-mBorderFixed.x;
        mTexCoord1[11].y = bottom;

        mTexCoord1[12].x = right-mBorderFixed.x;
        mTexCoord1[12].y = top;
        mTexCoord1[13].x = right-mBorderFixed.x;
        mTexCoord1[13].y = bottom;
        mTexCoord1[14].x = right;
        mTexCoord1[14].y = top;
        mTexCoord1[15].x = right;
        mTexCoord1[15].y = top;
        mTexCoord1[16].x = right-mBorderFixed.x;
        mTexCoord1[16].y = bottom;
        mTexCoord1[17].x = right;
        mTexCoord1[17].y = bottom;

        mGeomUVsOutOfDate = true;
    }

} // namespace
