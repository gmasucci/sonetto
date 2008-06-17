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
#include "SonettoTailedWindow.h"
#include <OGRE/OgreStableHeaders.h>
#include <OGRE/OgreOverlayManager.h>
#include <OGRE/OgreMaterial.h>
#include <OGRE/OgreMaterialManager.h>
#include <OGRE/OgreTechnique.h>
#include <OGRE/OgrePass.h>
#include <OGRE/OgreStringConverter.h>
#include <OGRE/OgreHardwareBufferManager.h>
#include <OGRE/OgreRoot.h>
#include <OGRE/OgreRenderSystem.h>
namespace Sonetto {
    //-----------------------------------------------------------------------------
    // Sonetto::TailedWindow implementation
    //-----------------------------------------------------------------------------
    // Binding defines
    #define POSITION_BINDING 0
    #define COLOUR_BINDING 1
    #define TEXCOORD_BINDING 2
    //-----------------------------------------------------------------------------
    // Ogre object type name
    Ogre::String TailedWindow::msTypeName = "TailedWindow";
    //-----------------------------------------------------------------------------
    TailedWindow::TailedWindow(const Ogre::String &name) : Ogre::OverlayContainer(name),
    mpWindowSkin(0), mWindowType(WT_MENU), mWindowAlpha(1.0f), mBorderAlpha(1.0f),
    mUpdateWindowColors(true), mUpdateBorderColors(true), mpBorderRenderable(0),
    mTailVisible(true), mTailDirection(false), mTailPosition(0.1f),
    mScrMetricsMode(SMM_RELATIVE_ASPECT_ADJUSTED),
    mAspectRatio(1.0f)
    {}
    //-----------------------------------------------------------------------------
    TailedWindow::~TailedWindow()
    {
        if(mInitialised)
        {
            delete mWindowRenderOp.vertexData;
            delete mBorderRenderOp.vertexData;
            delete mpBorderRenderable;
        }
    }
    //-----------------------------------------------------------------------------
    void TailedWindow::initialise(void)
    {
        // Check if this element has not initialised
        if(!mInitialised)
        {
            //-------------------
            // Window Background
            //-------------------
            // Setup the Window Render Operation.
            mWindowRenderOp.vertexData      = new Ogre::VertexData();
            // Create the Vertex Declaration
            Ogre::VertexDeclaration * decl  = mWindowRenderOp.vertexData->vertexDeclaration;
            // Offset value (for the Vertex Declaration)
            size_t offset = 0;
            // Add the Vertex position binding to the vertex declaration.
            decl->addElement(POSITION_BINDING, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
            // Add the Vertex color binding to the vertex declaration.
            decl->addElement(COLOUR_BINDING, offset, Ogre::VET_COLOUR, Ogre::VES_DIFFUSE);
            // Add the Mask Texture Coordinates binding
            decl->addElement(TEXCOORD_BINDING, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, 0);
            // Offset by the size of two floats.
            offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);
            // Add the Background Texture Coordinates binding.
            decl->addElement(TEXCOORD_BINDING, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, 1);
            // Now offset again by the size of two floats, just in case we need to add another one.
            offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);
            // Setup the Vertex data
            mWindowRenderOp.vertexData->vertexStart = 0;
            mWindowRenderOp.vertexData->vertexCount = TAILED_NUM_VERTEX;
            // Set up the Vertex Buffer
            Ogre::HardwareVertexBufferSharedPtr vbuf =
                Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
                    decl->getVertexSize(POSITION_BINDING), mWindowRenderOp.vertexData->vertexCount,
                    Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY
                );
            // Bind buffer
            mWindowRenderOp.vertexData->vertexBufferBinding->setBinding(POSITION_BINDING, vbuf);
            vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
                       decl->getVertexSize(COLOUR_BINDING), mWindowRenderOp.vertexData->vertexCount,
                       Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY
                   );
            // Bind buffer
            mWindowRenderOp.vertexData->vertexBufferBinding->setBinding(COLOUR_BINDING, vbuf);
            vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
                       decl->getVertexSize(TEXCOORD_BINDING), mWindowRenderOp.vertexData->vertexCount,
                       Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY
                   );
            // Bind buffer
            mWindowRenderOp.vertexData->vertexBufferBinding->setBinding(TEXCOORD_BINDING, vbuf);
            mWindowRenderOp.useIndexes    = false;
            mWindowRenderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
            //----------------------
            // Window border
            //----------------------
            mpBorderRenderable           = new TailedBorderRenderable(this);
            // Set up the Render Operation in advance
            mBorderRenderOp.vertexData  = new Ogre::VertexData();
            // Create the Vertex Declaration
            decl = mBorderRenderOp.vertexData->vertexDeclaration;
            // Reset the offset variable
            offset = 0;
            // Add the Vertex position binding to the vertex declaration.
            decl->addElement(POSITION_BINDING, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
            // Add the Vertex color binding to the vertex declaration.
            decl->addElement(COLOUR_BINDING, offset, Ogre::VET_COLOUR, Ogre::VES_DIFFUSE);
            // Add the Vertex texture coordinates binding to the vertex declaration.
            decl->addElement(TEXCOORD_BINDING, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, 0);
            // Now offset again by the size of two floats, just in case we need to add another one.
            offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);
            // Set up the Vertex data
            mBorderRenderOp.vertexData->vertexStart = 0;
            mBorderRenderOp.vertexData->vertexCount = TAILED_NUM_VERTEX;

            // Vertex Buffer #1 (Position)
            vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
                       decl->getVertexSize(POSITION_BINDING), mBorderRenderOp.vertexData->vertexCount,
                       Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY
                   );
            // Bind buffer
            mBorderRenderOp.vertexData->vertexBufferBinding->setBinding(POSITION_BINDING, vbuf);
            // Vertex Buffer #2 (Color)
            vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
                       decl->getVertexSize(COLOUR_BINDING), mBorderRenderOp.vertexData->vertexCount,
                       Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY
                   );
            // Bind buffer
            mBorderRenderOp.vertexData->vertexBufferBinding->setBinding(COLOUR_BINDING, vbuf);
            // Vertex Buffer #3 (Texcoord)
            vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
                       decl->getVertexSize(TEXCOORD_BINDING), mBorderRenderOp.vertexData->vertexCount,
                       Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY
                   );
            // Bind buffer
            mBorderRenderOp.vertexData->vertexBufferBinding->setBinding(TEXCOORD_BINDING, vbuf);
            mBorderRenderOp.useIndexes      = false;
            mBorderRenderOp.operationType   = Ogre::RenderOperation::OT_TRIANGLE_LIST;
            // Now we have initialised this element, now it's ready to go.
            mInitialised = true;
            // Activate the update flag, because we need to update.
            mUpdateWindowColors     = true;
            mUpdateBorderColors     = true;
            mGeomPositionsOutOfDate = true;
            mGeomUVsOutOfDate       = true;
        }
    }
    //-----------------------------------------------------------------------------
    void TailedWindow::setWindowSkin(WindowSkinPtr pwskin)
    {
        mpWindowSkin = pwskin;
        Ogre::OverlayContainer::setMaterialName(mpWindowSkin->mWindowMaterial->getName());
        mpWindowSkin->mBorderMaterial->load();
        mGeomPositionsOutOfDate = true;
        mGeomUVsOutOfDate       = true;
        mUpdateWindowColors     = true;
        mUpdateBorderColors     = true;
    }
    //-----------------------------------------------------------------------------
    WindowSkinPtr TailedWindow::getWindowSkin(void)
    {
        return mpWindowSkin;
    }
    //-----------------------------------------------------------------------------
    void TailedWindow::setWindowType(WindowType wtype)
    {
        mWindowType         = wtype;
        mUpdateWindowColors = true;
        mUpdateBorderColors = true;
    }
    //-----------------------------------------------------------------------------
    WindowType TailedWindow::getWindowType(void)
    {
        return mWindowType;
    }
    //-----------------------------------------------------------------------------
    void TailedWindow::setWindowAlpha(Ogre::Real alpha)
    {
        mWindowAlpha        = alpha;
        mUpdateWindowColors = true;
    }
    //-----------------------------------------------------------------------------
    Ogre::Real TailedWindow::getWindowAlpha(void)
    {
        return mWindowAlpha;
    }
    //-----------------------------------------------------------------------------
    void TailedWindow::setBorderAlpha(Ogre::Real alpha)
    {
        mBorderAlpha        = alpha;
        mUpdateBorderColors = true;
    }
    //-----------------------------------------------------------------------------
    Ogre::Real TailedWindow::getBorderAlpha(void)
    {
        return mBorderAlpha;
    }
    //-----------------------------------------------------------------------------
    void TailedWindow::setWindowTail(bool visible, bool dir, float pos)
    {
        mTailVisible = visible;
        mTailDirection = dir;
        mTailPosition = pos;
    }
    //-----------------------------------------------------------------------------
    void TailedWindow::setTailVisibility(bool visible)
    {
        mTailVisible = visible;
    }
    //-----------------------------------------------------------------------------
    bool TailedWindow::isVisible()
    {
        return mTailVisible;
    }
    //-----------------------------------------------------------------------------
    void TailedWindow::setTailDirection(bool dir)
    {
        mTailDirection = dir;
    }
    //-----------------------------------------------------------------------------
    bool TailedWindow::getTailDirection()
    {
        return mTailDirection;
    }
    //-----------------------------------------------------------------------------
    void TailedWindow::setTailPosition(float pos)
    {
        mTailPosition = pos;
    }
    //-----------------------------------------------------------------------------
    float TailedWindow::getTailPosition()
    {
        return mTailPosition;
    }
    //-----------------------------------------------------------------------------
    void TailedWindow::setScrMetricsMode(ScreenMetricsMode smm)
    {
        mScrMetricsMode         = smm;
        // We need to update the positions and UVs now.
        mGeomPositionsOutOfDate = true;
        mGeomUVsOutOfDate       = true;
    }
    //-----------------------------------------------------------------------------
    ScreenMetricsMode TailedWindow::getScrMetricsMode(void)
    {
        return mScrMetricsMode;
    }
    //-----------------------------------------------------------------------------
    const Ogre::String & TailedWindow::getTypeName(void) const
    {
        return msTypeName;
    }
    //-----------------------------------------------------------------------------
    void TailedWindow::getRenderOperation(Ogre::RenderOperation &op)
    {
        op = mWindowRenderOp;
    }
    //-----------------------------------------------------------------------------
    void TailedWindow::_updateRenderQueue(Ogre::RenderQueue * queue)
    {
        if (mVisible)
        {
            queue->addRenderable(mpBorderRenderable, Ogre::RENDER_QUEUE_OVERLAY, mZOrder+1);

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
    //-----------------------------------------------------------------------------
    void TailedWindow::visitRenderables(Renderable::Visitor* visitor, bool debugRenderables)
    {
        visitor->visit(mpBorderRenderable, 0, false);
        OverlayContainer::visitRenderables(visitor, debugRenderables);
    }
    //-----------------------------------------------------------------------------
    void TailedWindow::_update(void)
    {
        // Calculate the aspect ratio
        Ogre::Real vpWidth, vpHeight;
        vpWidth = (Ogre::Real) (Ogre::OverlayManager::getSingleton().getViewportWidth());
        vpHeight = (Ogre::Real) (Ogre::OverlayManager::getSingleton().getViewportHeight());
        Ogre::Real tmpAspectRatio = vpHeight/vpWidth;

        // Set up the flags to warn ogre we need an update.
        if((tmpAspectRatio != mAspectRatio)) {
            mAspectRatio            = tmpAspectRatio;
            mGeomPositionsOutOfDate = true;
            mGeomUVsOutOfDate       = true;
        }

        if(mUpdateWindowColors)
            updateWindowColors();

        if(mUpdateBorderColors)
            updateBorderColors();

        if(mGeomPositionsOutOfDate)
            calculatePosition();

        if(mGeomUVsOutOfDate)
            calculateTexCoord();

        // Now call the Overlay Container update function.
        Ogre::OverlayContainer::_update();
    }
    //-----------------------------------------------------------------------------
    void TailedWindow::updatePositionGeometry(void)
    {
        // TailedWindow Background
        // Get the Vertex Buffer (for positions)
        Ogre::HardwareVertexBufferSharedPtr vbuf =
            mWindowRenderOp.vertexData->vertexBufferBinding->getBuffer(POSITION_BINDING);
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
        // Set the position values to each vertex
        for (size_t i = 0; i < mWindowRenderOp.vertexData->vertexCount; ++i)
        {
            *pPos++ = ((mPosCoord[i].x * (mAspect * 2)) - mAspect);
            *pPos++ = -(mPosCoord[i].y * 2) + 1;
            *pPos++ = zValue;
        }
        // We have finished here, unlock the vertex buffer.
        vbuf->unlock();
        // Window Border
        // Get the Vertex Buffer (for positions)
        vbuf = mBorderRenderOp.vertexData->vertexBufferBinding->getBuffer(POSITION_BINDING);
        // Cast the Vertex Buffer to floats
        float * pBorderPos = static_cast<float*>(
                                 vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD) );
        // Set the position to each vertex
        for (size_t i = 0; i < mBorderRenderOp.vertexData->vertexCount; ++i)
        {
            *pBorderPos++ = ((mPosCoord[i].x * (mAspect * 2)) - mAspect);
            *pBorderPos++ = -(mPosCoord[i].y * 2) + 1;
            *pBorderPos++ = zValue;
        }
        // Ok, we're all set up to the next frames, for now.
        vbuf->unlock();
    }
    //-----------------------------------------------------------------------------
    void TailedWindow::updateTextureGeometry(void)
    {
        Ogre::HardwareVertexBufferSharedPtr vbuf =
            mWindowRenderOp.vertexData->vertexBufferBinding->getBuffer(TEXCOORD_BINDING);
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
        *pTex++ = mpWindowSkin->mWinTexCoord.top_left.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.top_left.top;
        // Texcoord 1
        *pTex++ = mTexCoord[0].x;
        *pTex++ = mTexCoord[0].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.top_left.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.top_left.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord[1].x;
        *pTex++ = mTexCoord[1].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.top_left.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.top_left.top;
        // Texcoord 1
        *pTex++ = mTexCoord[2].x;
        *pTex++ = mTexCoord[2].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.top_left.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.top_left.top;
        // Texcoord 1
        *pTex++ = mTexCoord[3].x;
        *pTex++ = mTexCoord[3].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.top_left.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.top_left.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord[4].x;
        *pTex++ = mTexCoord[4].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.top_left.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.top_left.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord[5].x;
        *pTex++ = mTexCoord[5].y;

        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.top.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.top.top;
        // Texcoord 1
        *pTex++ = mTexCoord[6].x;
        *pTex++ = mTexCoord[6].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.top.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.top.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord[7].x;
        *pTex++ = mTexCoord[7].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.top.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.top.top;
        // Texcoord 1
        *pTex++ = mTexCoord[8].x;
        *pTex++ = mTexCoord[8].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.top.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.top.top;
        // Texcoord 1
        *pTex++ = mTexCoord[9].x;
        *pTex++ = mTexCoord[9].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.top.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.top.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord[10].x;
        *pTex++ = mTexCoord[10].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.top.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.top.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord[11].x;
        *pTex++ = mTexCoord[11].y;

        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.top_right.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.top_right.top;
        // Texcoord 1
        *pTex++ = mTexCoord[12].x;
        *pTex++ = mTexCoord[12].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.top_right.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.top_right.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord[13].x;
        *pTex++ = mTexCoord[13].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.top_right.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.top_right.top;
        // Texcoord 1
        *pTex++ = mTexCoord[14].x;
        *pTex++ = mTexCoord[14].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.top_right.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.top_right.top;
        // Texcoord 1
        *pTex++ = mTexCoord[15].x;
        *pTex++ = mTexCoord[15].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.top_right.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.top_right.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord[16].x;
        *pTex++ = mTexCoord[16].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.top_right.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.top_right.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord[17].x;
        *pTex++ = mTexCoord[17].y;

        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.left.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.left.top;
        // Texcoord 1
        *pTex++ = mTexCoord[18].x;
        *pTex++ = mTexCoord[18].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.left.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.left.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord[19].x;
        *pTex++ = mTexCoord[19].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.left.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.left.top;
        // Texcoord 1
        *pTex++ = mTexCoord[20].x;
        *pTex++ = mTexCoord[20].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.left.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.left.top;
        // Texcoord 1
        *pTex++ = mTexCoord[21].x;
        *pTex++ = mTexCoord[21].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.left.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.left.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord[22].x;
        *pTex++ = mTexCoord[22].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.left.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.left.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord[23].x;
        *pTex++ = mTexCoord[23].y;

        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.center.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.center.top;
        // Texcoord 1
        *pTex++ = mTexCoord[24].x;
        *pTex++ = mTexCoord[24].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.center.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.center.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord[25].x;
        *pTex++ = mTexCoord[25].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.center.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.center.top;
        // Texcoord 1
        *pTex++ = mTexCoord[26].x;
        *pTex++ = mTexCoord[26].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.center.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.center.top;
        // Texcoord 1
        *pTex++ = mTexCoord[27].x;
        *pTex++ = mTexCoord[27].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.center.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.center.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord[28].x;
        *pTex++ = mTexCoord[28].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.center.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.center.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord[29].x;
        *pTex++ = mTexCoord[29].y;

        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.right.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.right.top;
        // Texcoord 1
        *pTex++ = mTexCoord[30].x;
        *pTex++ = mTexCoord[30].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.right.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.right.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord[31].x;
        *pTex++ = mTexCoord[31].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.right.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.right.top;
        // Texcoord 1
        *pTex++ = mTexCoord[32].x;
        *pTex++ = mTexCoord[32].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.right.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.right.top;
        // Texcoord 1
        *pTex++ = mTexCoord[33].x;
        *pTex++ = mTexCoord[33].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.right.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.right.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord[34].x;
        *pTex++ = mTexCoord[34].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.right.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.right.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord[35].x;
        *pTex++ = mTexCoord[35].y;

        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom_left.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom_left.top;
        // Texcoord 1
        *pTex++ = mTexCoord[36].x;
        *pTex++ = mTexCoord[36].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom_left.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom_left.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord[37].x;
        *pTex++ = mTexCoord[37].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom_left.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom_left.top;
        // Texcoord 1
        *pTex++ = mTexCoord[38].x;
        *pTex++ = mTexCoord[38].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom_left.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom_left.top;
        // Texcoord 1
        *pTex++ = mTexCoord[39].x;
        *pTex++ = mTexCoord[39].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom_left.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom_left.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord[40].x;
        *pTex++ = mTexCoord[40].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom_left.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom_left.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord[41].x;
        *pTex++ = mTexCoord[41].y;

        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom.top;
        // Texcoord 1
        *pTex++ = mTexCoord[42].x;
        *pTex++ = mTexCoord[42].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord[43].x;
        *pTex++ = mTexCoord[43].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom.top;
        // Texcoord 1
        *pTex++ = mTexCoord[44].x;
        *pTex++ = mTexCoord[44].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom.top;
        // Texcoord 1
        *pTex++ = mTexCoord[45].x;
        *pTex++ = mTexCoord[45].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord[46].x;
        *pTex++ = mTexCoord[46].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord[47].x;
        *pTex++ = mTexCoord[47].y;

        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom_right.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom_right.top;
        // Texcoord 1
        *pTex++ = mTexCoord[48].x;
        *pTex++ = mTexCoord[48].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom_right.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom_right.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord[49].x;
        *pTex++ = mTexCoord[49].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom_right.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom_right.top;
        // Texcoord 1
        *pTex++ = mTexCoord[50].x;
        *pTex++ = mTexCoord[50].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom_right.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom_right.top;
        // Texcoord 1
        *pTex++ = mTexCoord[51].x;
        *pTex++ = mTexCoord[51].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom_right.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom_right.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord[52].x;
        *pTex++ = mTexCoord[52].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom_right.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom_right.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord[53].x;
        *pTex++ = mTexCoord[53].y;
        if(!mTailDirection)
        {
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.tail_down.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.tail_down.top;
        // Texcoord 1
        *pTex++ = mTexCoord[54].x;
        *pTex++ = mTexCoord[54].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.tail_down.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.tail_down.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord[55].x;
        *pTex++ = mTexCoord[55].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.tail_down.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.tail_down.top;
        // Texcoord 1
        *pTex++ = mTexCoord[56].x;
        *pTex++ = mTexCoord[56].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.tail_down.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.tail_down.top;
        // Texcoord 1
        *pTex++ = mTexCoord[57].x;
        *pTex++ = mTexCoord[57].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.tail_down.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.tail_down.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord[58].x;
        *pTex++ = mTexCoord[58].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.tail_down.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.tail_down.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord[59].x;
        *pTex++ = mTexCoord[59].y;

        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom.top;
        // Texcoord 1
        *pTex++ = mTexCoord[60].x;
        *pTex++ = mTexCoord[60].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord[61].x;
        *pTex++ = mTexCoord[61].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom.top;
        // Texcoord 1
        *pTex++ = mTexCoord[62].x;
        *pTex++ = mTexCoord[62].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom.top;
        // Texcoord 1
        *pTex++ = mTexCoord[63].x;
        *pTex++ = mTexCoord[63].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord[64].x;
        *pTex++ = mTexCoord[64].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord[65].x;
        *pTex++ = mTexCoord[65].y;
        } else {
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.tail_up.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.tail_up.top;
        // Texcoord 1
        *pTex++ = mTexCoord[54].x;
        *pTex++ = mTexCoord[54].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.tail_up.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.tail_up.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord[55].x;
        *pTex++ = mTexCoord[55].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.tail_up.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.tail_up.top;
        // Texcoord 1
        *pTex++ = mTexCoord[56].x;
        *pTex++ = mTexCoord[56].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.tail_up.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.tail_up.top;
        // Texcoord 1
        *pTex++ = mTexCoord[57].x;
        *pTex++ = mTexCoord[57].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.tail_up.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.tail_up.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord[58].x;
        *pTex++ = mTexCoord[58].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.tail_up.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.tail_up.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord[59].x;
        *pTex++ = mTexCoord[59].y;

        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.top.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.top.top;
        // Texcoord 1
        *pTex++ = mTexCoord[60].x;
        *pTex++ = mTexCoord[60].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.top.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.top.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord[61].x;
        *pTex++ = mTexCoord[61].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.top.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.top.top;
        // Texcoord 1
        *pTex++ = mTexCoord[62].x;
        *pTex++ = mTexCoord[62].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.top.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.top.top;
        // Texcoord 1
        *pTex++ = mTexCoord[63].x;
        *pTex++ = mTexCoord[63].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.top.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.top.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord[64].x;
        *pTex++ = mTexCoord[64].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.top.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.top.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord[65].x;
        *pTex++ = mTexCoord[65].y;
        }

        vbuf->unlock();

        vbuf = mBorderRenderOp.vertexData->vertexBufferBinding->getBuffer(TEXCOORD_BINDING);
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
        *pTex++ = mpWindowSkin->mWinTexCoord.top_left.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.top_left.top;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.top_left.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.top_left.bottom;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.top_left.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.top_left.top;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.top_left.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.top_left.top;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.top_left.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.top_left.bottom;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.top_left.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.top_left.bottom;

        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.top.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.top.top;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.top.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.top.bottom;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.top.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.top.top;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.top.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.top.top;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.top.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.top.bottom;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.top.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.top.bottom;

        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.top_right.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.top_right.top;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.top_right.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.top_right.bottom;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.top_right.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.top_right.top;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.top_right.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.top_right.top;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.top_right.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.top_right.bottom;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.top_right.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.top_right.bottom;

        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.left.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.left.top;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.left.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.left.bottom;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.left.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.left.top;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.left.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.left.top;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.left.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.left.bottom;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.left.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.left.bottom;

        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.center.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.center.top;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.center.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.center.bottom;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.center.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.center.top;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.center.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.center.top;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.center.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.center.bottom;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.center.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.center.bottom;

        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.right.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.right.top;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.right.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.right.bottom;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.right.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.right.top;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.right.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.right.top;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.right.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.right.bottom;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.right.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.right.bottom;

        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom_left.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom_left.top;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom_left.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom_left.bottom;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom_left.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom_left.top;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom_left.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom_left.top;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom_left.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom_left.bottom;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom_left.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom_left.bottom;

        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom.top;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom.bottom;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom.top;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom.top;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom.bottom;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom.bottom;

        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom_right.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom_right.top;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom_right.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom_right.bottom;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom_right.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom_right.top;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom_right.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom_right.top;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom_right.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom_right.bottom;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom_right.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom_right.bottom;
        if(!mTailDirection)
        {
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.tail_down.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.tail_down.top;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.tail_down.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.tail_down.bottom;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.tail_down.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.tail_down.top;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.tail_down.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.tail_down.top;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.tail_down.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.tail_down.bottom;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.tail_down.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.tail_down.bottom;

        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom.top;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom.bottom;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom.top;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom.top;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom.bottom;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.bottom.bottom;
        } else {
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.tail_up.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.tail_up.top;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.tail_up.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.tail_up.bottom;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.tail_up.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.tail_up.top;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.tail_up.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.tail_up.top;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.tail_up.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.tail_up.bottom;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.tail_up.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.tail_up.bottom;

        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.top.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.top.top;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.top.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.top.bottom;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.top.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.top.top;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.top.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.top.top;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.top.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.top.bottom;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.top.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.top.bottom;
        }

        vbuf->unlock();
    }
    //-----------------------------------------------------------------------------
    void TailedWindow::updateWindowColors(void)
    {
        if (mInitialised)
        {
            Ogre::HardwareVertexBufferSharedPtr vbuf =
                mWindowRenderOp.vertexData->vertexBufferBinding->getBuffer(COLOUR_BINDING);

            Ogre::RGBA* pDest = static_cast<Ogre::RGBA*>(
                                    vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD) );
            Ogre::RGBA color;
            Ogre::ColourValue colorTmp;
            switch(mWindowType)
            {
                case WT_MENU:
                colorTmp = mpWindowSkin->mMenuColor;
                break;
                case WT_MAIN:
                colorTmp = mpWindowSkin->mMainColor;
                break;
                case WT_ENEMY:
                colorTmp = mpWindowSkin->mEnemyColor;
                break;
                case WT_SYSTEM:
                colorTmp = mpWindowSkin->mSystemColor;
                break;
            }

            colorTmp.a = colorTmp.a * mWindowAlpha;

            Ogre::Root::getSingleton().convertColourValue(colorTmp, &color);

            for (size_t i = 0; i < mWindowRenderOp.vertexData->vertexCount; ++i)
            {
                *pDest++ = color;
            }

            vbuf->unlock();
            mUpdateWindowColors = false;
        }
    }
    //-----------------------------------------------------------------------------
    void TailedWindow::updateBorderColors(void)
    {
        if (mInitialised)
        {
            Ogre::HardwareVertexBufferSharedPtr vbuf =
                mBorderRenderOp.vertexData->vertexBufferBinding->getBuffer(COLOUR_BINDING);

            Ogre::RGBA* pDest = static_cast<Ogre::RGBA*>(
                                    vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD) );
            Ogre::RGBA color;
            Ogre::ColourValue colorTmp;
            switch(mWindowType)
            {
                case WT_MENU:
                colorTmp = mpWindowSkin->mMenuBorderColor;
                break;
                case WT_MAIN:
                colorTmp = mpWindowSkin->mMainBorderColor;
                break;
                case WT_ENEMY:
                colorTmp = mpWindowSkin->mEnemyBorderColor;
                break;
                case WT_SYSTEM:
                colorTmp = mpWindowSkin->mSystemBorderColor;
                break;
            }

            colorTmp.a = colorTmp.a * mBorderAlpha;

            Ogre::Root::getSingleton().convertColourValue(colorTmp, &color);

            for (size_t i = 0; i < mBorderRenderOp.vertexData->vertexCount; ++i)
            {
                *pDest++ = color;
            }

            vbuf->unlock();
            mUpdateBorderColors = false;
        }
    }
    //-----------------------------------------------------------------------------
    void TailedWindow::calculatePosition(void)
    {
        float left, right, top, bottom, tail;

        left = _getDerivedLeft();
        right = left + mWidth;
        top = _getDerivedTop();
        bottom =  top + mHeight;

        if(mTailVisible)
        {
            Ogre::Real start = left+mpWindowSkin->mBorderDimensions.x;
            Ogre::Real end = right - (mpWindowSkin->mBorderDimensions.x * 2);
            tail = lerp(start,end,mTailPosition);
        }

        mPosCoord[0].x = left; // Assign to mPosCoord.
        mPosCoord[0].y = top; // Assign to mPosCoord.
        mPosCoord[1].x = left; // Assign to mPosCoord.
        mPosCoord[1].y = top+mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
        mPosCoord[2].x = left+mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
        mPosCoord[2].y = top; // Assign to mPosCoord.
        mPosCoord[3].x = left+mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
        mPosCoord[3].y = top; // Assign to mPosCoord.
        mPosCoord[4].x = left; // Assign to mPosCoord.
        mPosCoord[4].y = top+mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
        mPosCoord[5].x = left+mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
        mPosCoord[5].y = top+mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
        if(mTailVisible)
        {
            if(!mTailDirection)
            {
                mPosCoord[6].x = left+mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
                mPosCoord[6].y = top; // Assign to mPosCoord.
                mPosCoord[7].x = left+mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
                mPosCoord[7].y = top+mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
                mPosCoord[8].x = right-mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
                mPosCoord[8].y = top; // Assign to mPosCoord.
                mPosCoord[9].x = right-mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
                mPosCoord[9].y = top; // Assign to mPosCoord.
                mPosCoord[10].x = left+mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
                mPosCoord[10].y = top+mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
                mPosCoord[11].x = right-mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
                mPosCoord[11].y = top+mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
            } else {
                mPosCoord[6].x = left+mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
                mPosCoord[6].y = top; // Assign to mPosCoord.
                mPosCoord[7].x = left+mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
                mPosCoord[7].y = top+mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
                mPosCoord[8].x = tail; // Assign to mPosCoord.
                mPosCoord[8].y = top; // Assign to mPosCoord.
                mPosCoord[9].x = tail; // Assign to mPosCoord.
                mPosCoord[9].y = top; // Assign to mPosCoord.
                mPosCoord[10].x = left+mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
                mPosCoord[10].y = top+mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
                mPosCoord[11].x = tail; // Assign to mPosCoord.
                mPosCoord[11].y = top+mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
            }
        } else {
                mPosCoord[6].x = left+mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
                mPosCoord[6].y = top; // Assign to mPosCoord.
                mPosCoord[7].x = left+mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
                mPosCoord[7].y = top+mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
                mPosCoord[8].x = right-mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
                mPosCoord[8].y = top; // Assign to mPosCoord.
                mPosCoord[9].x = right-mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
                mPosCoord[9].y = top; // Assign to mPosCoord.
                mPosCoord[10].x = left+mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
                mPosCoord[10].y = top+mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
                mPosCoord[11].x = right-mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
                mPosCoord[11].y = top+mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
        }

        mPosCoord[12].x = right-mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
        mPosCoord[12].y = top; // Assign to mPosCoord.
        mPosCoord[13].x = right-mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
        mPosCoord[13].y = top+mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
        mPosCoord[14].x = right; // Assign to mPosCoord.
        mPosCoord[14].y = top; // Assign to mPosCoord.
        mPosCoord[15].x = right; // Assign to mPosCoord.
        mPosCoord[15].y = top; // Assign to mPosCoord.
        mPosCoord[16].x = right-mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
        mPosCoord[16].y = top+mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
        mPosCoord[17].x = right; // Assign to mPosCoord.
        mPosCoord[17].y = top+mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.

        mPosCoord[18].x = left; // Assign to mPosCoord.
        mPosCoord[18].y = top+mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
        mPosCoord[19].x = left; // Assign to mPosCoord.
        mPosCoord[19].y = bottom-mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
        mPosCoord[20].x = left+mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
        mPosCoord[20].y = top+mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
        mPosCoord[21].x = left+mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
        mPosCoord[21].y = top+mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
        mPosCoord[22].x = left; // Assign to mPosCoord.
        mPosCoord[22].y = bottom-mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
        mPosCoord[23].x = left+mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
        mPosCoord[23].y = bottom-mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.

        mPosCoord[24].x = left+mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
        mPosCoord[24].y = top+mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
        mPosCoord[25].x = left+mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
        mPosCoord[25].y = bottom-mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
        mPosCoord[26].x = right-mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
        mPosCoord[26].y = top+mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
        mPosCoord[27].x = right-mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
        mPosCoord[27].y = top+mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
        mPosCoord[28].x = left+mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
        mPosCoord[28].y = bottom-mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
        mPosCoord[29].x = right-mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
        mPosCoord[29].y = bottom-mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.

        mPosCoord[30].x = right-mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
        mPosCoord[30].y = top+mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
        mPosCoord[31].x = right-mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
        mPosCoord[31].y = bottom-mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
        mPosCoord[32].x = right; // Assign to mPosCoord.
        mPosCoord[32].y = top+mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
        mPosCoord[33].x = right; // Assign to mPosCoord.
        mPosCoord[33].y = top+mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
        mPosCoord[34].x = right-mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
        mPosCoord[34].y = bottom-mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
        mPosCoord[35].x = right; // Assign to mPosCoord.
        mPosCoord[35].y = bottom-mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.

        mPosCoord[36].x = left; // Assign to mPosCoord.
        mPosCoord[36].y = bottom-mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
        mPosCoord[37].x = left; // Assign to mPosCoord.
        mPosCoord[37].y = bottom; // Assign to mPosCoord.
        mPosCoord[38].x = left+mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
        mPosCoord[38].y = bottom-mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
        mPosCoord[39].x = left+mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
        mPosCoord[39].y = bottom-mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
        mPosCoord[40].x = left; // Assign to mPosCoord.
        mPosCoord[40].y = bottom; // Assign to mPosCoord.
        mPosCoord[41].x = left+mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
        mPosCoord[41].y = bottom; // Assign to mPosCoord.
        if(mTailVisible)
        {
            if(mTailDirection)
            {
                mPosCoord[42].x = left+mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
                mPosCoord[42].y = bottom-mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
                mPosCoord[43].x = left+mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
                mPosCoord[43].y = bottom; // Assign to mPosCoord.
                mPosCoord[44].x = right-mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
                mPosCoord[44].y = bottom-mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
                mPosCoord[45].x = right-mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
                mPosCoord[45].y = bottom-mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
                mPosCoord[46].x = left+mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
                mPosCoord[46].y = bottom; // Assign to mPosCoord.
                mPosCoord[47].x = right-mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
                mPosCoord[47].y = bottom; // Assign to mPosCoord.
            } else {
                mPosCoord[42].x = left+mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
                mPosCoord[42].y = bottom-mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
                mPosCoord[43].x = left+mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
                mPosCoord[43].y = bottom; // Assign to mPosCoord.
                mPosCoord[44].x = tail; // Assign to mPosCoord.
                mPosCoord[44].y = bottom-mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
                mPosCoord[45].x = tail; // Assign to mPosCoord.
                mPosCoord[45].y = bottom-mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
                mPosCoord[46].x = left+mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
                mPosCoord[46].y = bottom; // Assign to mPosCoord.
                mPosCoord[47].x = tail; // Assign to mPosCoord.
                mPosCoord[47].y = bottom; // Assign to mPosCoord.
            }
        } else {
            mPosCoord[42].x = left+mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
            mPosCoord[42].y = bottom-mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
            mPosCoord[43].x = left+mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
            mPosCoord[43].y = bottom; // Assign to mPosCoord.
            mPosCoord[44].x = right-mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
            mPosCoord[44].y = bottom-mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
            mPosCoord[45].x = right-mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
            mPosCoord[45].y = bottom-mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
            mPosCoord[46].x = left+mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
            mPosCoord[46].y = bottom; // Assign to mPosCoord.
            mPosCoord[47].x = right-mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
            mPosCoord[47].y = bottom; // Assign to mPosCoord.
        }

        mPosCoord[48].x = right-mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
        mPosCoord[48].y = bottom-mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
        mPosCoord[49].x = right-mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
        mPosCoord[49].y = bottom; // Assign to mPosCoord.
        mPosCoord[50].x = right; // Assign to mPosCoord.
        mPosCoord[50].y = bottom-mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
        mPosCoord[51].x = right; // Assign to mPosCoord.
        mPosCoord[51].y = bottom-mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
        mPosCoord[52].x = right-mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
        mPosCoord[52].y = bottom; // Assign to mPosCoord.
        mPosCoord[53].x = right; // Assign to mPosCoord.
        mPosCoord[53].y = bottom; // Assign to mPosCoord.

        if(mTailVisible)
        {
            if(!mTailDirection)
            {
                mPosCoord[54].x = tail; // Assign to mPosCoord.
                mPosCoord[54].y = bottom-mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
                mPosCoord[55].x = tail; // Assign to mPosCoord.
                mPosCoord[55].y = bottom+mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
                mPosCoord[56].x = tail+mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
                mPosCoord[56].y = bottom-mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
                mPosCoord[57].x = tail+mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
                mPosCoord[57].y = bottom-mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
                mPosCoord[58].x = tail; // Assign to mPosCoord.
                mPosCoord[58].y = bottom+mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
                mPosCoord[59].x = tail+mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
                mPosCoord[59].y = bottom+mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.

                mPosCoord[60].x = tail+mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
                mPosCoord[60].y = bottom-mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
                mPosCoord[61].x = tail+mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
                mPosCoord[61].y = bottom; // Assign to mPosCoord.
                mPosCoord[62].x = right-mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
                mPosCoord[62].y = bottom-mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
                mPosCoord[63].x = right-mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
                mPosCoord[63].y = bottom-mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
                mPosCoord[64].x = tail+mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
                mPosCoord[64].y = bottom; // Assign to mPosCoord.
                mPosCoord[65].x = right-mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
                mPosCoord[65].y = bottom; // Assign to mPosCoord.
            } else {
                mPosCoord[54].x = tail; // Assign to mPosCoord.
                mPosCoord[54].y = top-mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
                mPosCoord[55].x = tail; // Assign to mPosCoord.
                mPosCoord[55].y = top+mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
                mPosCoord[56].x = tail+mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
                mPosCoord[56].y = top-mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
                mPosCoord[57].x = tail+mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
                mPosCoord[57].y = top-mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
                mPosCoord[58].x = tail; // Assign to mPosCoord.
                mPosCoord[58].y = top+mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
                mPosCoord[59].x = tail+mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
                mPosCoord[59].y = top+mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.

                mPosCoord[60].x = tail+mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
                mPosCoord[60].y = top; // Assign to mPosCoord.
                mPosCoord[61].x = tail+mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
                mPosCoord[61].y = top+mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
                mPosCoord[62].x = right-mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
                mPosCoord[62].y = top; // Assign to mPosCoord.
                mPosCoord[63].x = right-mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
                mPosCoord[63].y = top; // Assign to mPosCoord.
                mPosCoord[64].x = tail+mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
                mPosCoord[64].y = top+mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
                mPosCoord[65].x = right-mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
                mPosCoord[65].y = top+mpWindowSkin->mBorderDimensions.y; // Assign to mPosCoord.
            }
        } else {
            mPosCoord[54].x = 0.0f; // Assign to mPosCoord.
            mPosCoord[54].y = 0.0f; // Assign to mPosCoord.
            mPosCoord[55].x = 0.0f; // Assign to mPosCoord.
            mPosCoord[55].y = 0.0f; // Assign to mPosCoord.
            mPosCoord[56].x = 0.0f; // Assign to mPosCoord.
            mPosCoord[56].y = 0.0f; // Assign to mPosCoord.
            mPosCoord[57].x = 0.0f; // Assign to mPosCoord.
            mPosCoord[57].y = 0.0f; // Assign to mPosCoord.
            mPosCoord[58].x = 0.0f; // Assign to mPosCoord.
            mPosCoord[58].y = 0.0f; // Assign to mPosCoord.
            mPosCoord[59].x = 0.0f; // Assign to mPosCoord.
            mPosCoord[59].y = 0.0f; // Assign to mPosCoord.

            mPosCoord[60].x = 0.0f; // Assign to mPosCoord.
            mPosCoord[60].y = 0.0f; // Assign to mPosCoord.
            mPosCoord[61].x = 0.0f; // Assign to mPosCoord.
            mPosCoord[61].y = 0.0f; // Assign to mPosCoord.
            mPosCoord[62].x = 0.0f; // Assign to mPosCoord.
            mPosCoord[62].y = 0.0f; // Assign to mPosCoord.
            mPosCoord[63].x = 0.0f; // Assign to mPosCoord.
            mPosCoord[63].y = 0.0f; // Assign to mPosCoord.
            mPosCoord[64].x = 0.0f; // Assign to mPosCoord.
            mPosCoord[64].y = 0.0f; // Assign to mPosCoord.
            mPosCoord[65].x = 0.0f; // Assign to mPosCoord.
            mPosCoord[65].y = 0.0f; // Assign to mPosCoord.
        }

        mGeomPositionsOutOfDate = true;
    }
    //-----------------------------------------------------------------------------
    void TailedWindow::calculateTexCoord(void)
    {
        float left, right, top, bottom, tail;

        Ogre::Vector2 mBorderFixed;

        if (!mpWindowSkin->mTileMode) // If the texture mode is on stretch mode
        {
            left = 0.0f;
            right = mpWindowSkin->mTiling.x;
            top = 0.0f;
            bottom =  mpWindowSkin->mTiling.y;
            mBorderFixed.x = (mpWindowSkin->mBorderDimensions.x / mWidth)*mpWindowSkin->mTiling.x;
            mBorderFixed.y = (mpWindowSkin->mBorderDimensions.y / mHeight)*mpWindowSkin->mTiling.y;
        }
        else // Else if the texture mode is on tile mode
        {
            left = _getDerivedLeft() * mpWindowSkin->mTiling.x;
            right = left + (mWidth * mpWindowSkin->mTiling.x);
            top = _getDerivedTop() * mpWindowSkin->mTiling.y;
            bottom =  top + (mHeight * mpWindowSkin->mTiling.y);
            mBorderFixed.x = mpWindowSkin->mBorderDimensions.x*mpWindowSkin->mTiling.x;
            mBorderFixed.y = mpWindowSkin->mBorderDimensions.y*mpWindowSkin->mTiling.y;
        }

        if(mTailVisible)
        {
            Ogre::Real start = left+mBorderFixed.x;
            Ogre::Real end = right - (mBorderFixed.x * 2);
            tail = lerp(start,end,mTailPosition);
        }

        mTexCoord[0].x = left; // Assign to mTexCoord.
        mTexCoord[0].y = top; // Assign to mTexCoord.
        mTexCoord[1].x = left; // Assign to mTexCoord.
        mTexCoord[1].y = top+mBorderFixed.y; // Assign to mTexCoord.
        mTexCoord[2].x = left+mBorderFixed.x; // Assign to mTexCoord.
        mTexCoord[2].y = top; // Assign to mTexCoord.
        mTexCoord[3].x = left+mBorderFixed.x; // Assign to mTexCoord.
        mTexCoord[3].y = top; // Assign to mTexCoord.
        mTexCoord[4].x = left; // Assign to mTexCoord.
        mTexCoord[4].y = top+mBorderFixed.y; // Assign to mTexCoord.
        mTexCoord[5].x = left+mBorderFixed.x; // Assign to mTexCoord.
        mTexCoord[5].y = top+mBorderFixed.y; // Assign to mTexCoord.
        if(mTailVisible)
        {
            if(!mTailDirection)
            {
                mTexCoord[6].x = left+mBorderFixed.x; // Assign to mTexCoord.
                mTexCoord[6].y = top; // Assign to mTexCoord.
                mTexCoord[7].x = left+mBorderFixed.x; // Assign to mTexCoord.
                mTexCoord[7].y = top+mBorderFixed.y; // Assign to mTexCoord.
                mTexCoord[8].x = right-mBorderFixed.x; // Assign to mTexCoord.
                mTexCoord[8].y = top; // Assign to mTexCoord.
                mTexCoord[9].x = right-mBorderFixed.x; // Assign to mTexCoord.
                mTexCoord[9].y = top; // Assign to mTexCoord.
                mTexCoord[10].x = left+mBorderFixed.x; // Assign to mTexCoord.
                mTexCoord[10].y = top+mBorderFixed.y; // Assign to mTexCoord.
                mTexCoord[11].x = right-mBorderFixed.x; // Assign to mTexCoord.
                mTexCoord[11].y = top+mBorderFixed.y; // Assign to mTexCoord.
            } else {
                mTexCoord[6].x = left+mBorderFixed.x; // Assign to mTexCoord.
                mTexCoord[6].y = top; // Assign to mTexCoord.
                mTexCoord[7].x = left+mBorderFixed.x; // Assign to mTexCoord.
                mTexCoord[7].y = top+mBorderFixed.y; // Assign to mTexCoord.
                mTexCoord[8].x = tail; // Assign to mTexCoord.
                mTexCoord[8].y = top; // Assign to mTexCoord.
                mTexCoord[9].x = tail; // Assign to mTexCoord.
                mTexCoord[9].y = top; // Assign to mTexCoord.
                mTexCoord[10].x = left+mBorderFixed.x; // Assign to mTexCoord.
                mTexCoord[10].y = top+mBorderFixed.y; // Assign to mTexCoord.
                mTexCoord[11].x = tail; // Assign to mTexCoord.
                mTexCoord[11].y = top+mBorderFixed.y; // Assign to mTexCoord.
            }
        } else {
                mTexCoord[6].x = left+mBorderFixed.x; // Assign to mTexCoord.
                mTexCoord[6].y = top; // Assign to mTexCoord.
                mTexCoord[7].x = left+mBorderFixed.x; // Assign to mTexCoord.
                mTexCoord[7].y = top+mBorderFixed.y; // Assign to mTexCoord.
                mTexCoord[8].x = right-mBorderFixed.x; // Assign to mTexCoord.
                mTexCoord[8].y = top; // Assign to mTexCoord.
                mTexCoord[9].x = right-mBorderFixed.x; // Assign to mTexCoord.
                mTexCoord[9].y = top; // Assign to mTexCoord.
                mTexCoord[10].x = left+mBorderFixed.x; // Assign to mTexCoord.
                mTexCoord[10].y = top+mBorderFixed.y; // Assign to mTexCoord.
                mTexCoord[11].x = right-mBorderFixed.x; // Assign to mTexCoord.
                mTexCoord[11].y = top+mBorderFixed.y; // Assign to mTexCoord.
        }

        mTexCoord[12].x = right-mBorderFixed.x; // Assign to mTexCoord.
        mTexCoord[12].y = top; // Assign to mTexCoord.
        mTexCoord[13].x = right-mBorderFixed.x; // Assign to mTexCoord.
        mTexCoord[13].y = top+mBorderFixed.y; // Assign to mTexCoord.
        mTexCoord[14].x = right; // Assign to mTexCoord.
        mTexCoord[14].y = top; // Assign to mTexCoord.
        mTexCoord[15].x = right; // Assign to mTexCoord.
        mTexCoord[15].y = top; // Assign to mTexCoord.
        mTexCoord[16].x = right-mBorderFixed.x; // Assign to mTexCoord.
        mTexCoord[16].y = top+mBorderFixed.y; // Assign to mTexCoord.
        mTexCoord[17].x = right; // Assign to mTexCoord.
        mTexCoord[17].y = top+mBorderFixed.y; // Assign to mTexCoord.

        mTexCoord[18].x = left; // Assign to mTexCoord.
        mTexCoord[18].y = top+mBorderFixed.y; // Assign to mTexCoord.
        mTexCoord[19].x = left; // Assign to mTexCoord.
        mTexCoord[19].y = bottom-mBorderFixed.y; // Assign to mTexCoord.
        mTexCoord[20].x = left+mBorderFixed.x; // Assign to mTexCoord.
        mTexCoord[20].y = top+mBorderFixed.y; // Assign to mTexCoord.
        mTexCoord[21].x = left+mBorderFixed.x; // Assign to mTexCoord.
        mTexCoord[21].y = top+mBorderFixed.y; // Assign to mTexCoord.
        mTexCoord[22].x = left; // Assign to mTexCoord.
        mTexCoord[22].y = bottom-mBorderFixed.y; // Assign to mTexCoord.
        mTexCoord[23].x = left+mBorderFixed.x; // Assign to mTexCoord.
        mTexCoord[23].y = bottom-mBorderFixed.y; // Assign to mTexCoord.

        mTexCoord[24].x = left+mBorderFixed.x; // Assign to mTexCoord.
        mTexCoord[24].y = top+mBorderFixed.y; // Assign to mTexCoord.
        mTexCoord[25].x = left+mBorderFixed.x; // Assign to mTexCoord.
        mTexCoord[25].y = bottom-mBorderFixed.y; // Assign to mTexCoord.
        mTexCoord[26].x = right-mBorderFixed.x; // Assign to mTexCoord.
        mTexCoord[26].y = top+mBorderFixed.y; // Assign to mTexCoord.
        mTexCoord[27].x = right-mBorderFixed.x; // Assign to mTexCoord.
        mTexCoord[27].y = top+mBorderFixed.y; // Assign to mTexCoord.
        mTexCoord[28].x = left+mBorderFixed.x; // Assign to mTexCoord.
        mTexCoord[28].y = bottom-mBorderFixed.y; // Assign to mTexCoord.
        mTexCoord[29].x = right-mBorderFixed.x; // Assign to mTexCoord.
        mTexCoord[29].y = bottom-mBorderFixed.y; // Assign to mTexCoord.

        mTexCoord[30].x = right-mBorderFixed.x; // Assign to mTexCoord.
        mTexCoord[30].y = top+mBorderFixed.y; // Assign to mTexCoord.
        mTexCoord[31].x = right-mBorderFixed.x; // Assign to mTexCoord.
        mTexCoord[31].y = bottom-mBorderFixed.y; // Assign to mTexCoord.
        mTexCoord[32].x = right; // Assign to mTexCoord.
        mTexCoord[32].y = top+mBorderFixed.y; // Assign to mTexCoord.
        mTexCoord[33].x = right; // Assign to mTexCoord.
        mTexCoord[33].y = top+mBorderFixed.y; // Assign to mTexCoord.
        mTexCoord[34].x = right-mBorderFixed.x; // Assign to mTexCoord.
        mTexCoord[34].y = bottom-mBorderFixed.y; // Assign to mTexCoord.
        mTexCoord[35].x = right; // Assign to mTexCoord.
        mTexCoord[35].y = bottom-mBorderFixed.y; // Assign to mTexCoord.

        mTexCoord[36].x = left; // Assign to mTexCoord.
        mTexCoord[36].y = bottom-mBorderFixed.y; // Assign to mTexCoord.
        mTexCoord[37].x = left; // Assign to mTexCoord.
        mTexCoord[37].y = bottom; // Assign to mTexCoord.
        mTexCoord[38].x = left+mBorderFixed.x; // Assign to mTexCoord.
        mTexCoord[38].y = bottom-mBorderFixed.y; // Assign to mTexCoord.
        mTexCoord[39].x = left+mBorderFixed.x; // Assign to mTexCoord.
        mTexCoord[39].y = bottom-mBorderFixed.y; // Assign to mTexCoord.
        mTexCoord[40].x = left; // Assign to mTexCoord.
        mTexCoord[40].y = bottom; // Assign to mTexCoord.
        mTexCoord[41].x = left+mBorderFixed.x; // Assign to mTexCoord.
        mTexCoord[41].y = bottom; // Assign to mTexCoord.

        if(mTailVisible)
        {
            if(mTailDirection)
            {
                mTexCoord[42].x = left+mBorderFixed.x; // Assign to mTexCoord.
                mTexCoord[42].y = bottom-mBorderFixed.y; // Assign to mTexCoord.
                mTexCoord[43].x = left+mBorderFixed.x; // Assign to mTexCoord.
                mTexCoord[43].y = bottom; // Assign to mTexCoord.
                mTexCoord[44].x = right-mBorderFixed.x; // Assign to mTexCoord.
                mTexCoord[44].y = bottom-mBorderFixed.y; // Assign to mTexCoord.
                mTexCoord[45].x = right-mBorderFixed.x; // Assign to mTexCoord.
                mTexCoord[45].y = bottom-mBorderFixed.y; // Assign to mTexCoord.
                mTexCoord[46].x = left+mBorderFixed.x; // Assign to mTexCoord.
                mTexCoord[46].y = bottom; // Assign to mTexCoord.
                mTexCoord[47].x = right-mBorderFixed.x; // Assign to mTexCoord.
                mTexCoord[47].y = bottom; // Assign to mTexCoord.
            } else {
                mTexCoord[42].x = left+mBorderFixed.x; // Assign to mTexCoord.
                mTexCoord[42].y = bottom-mBorderFixed.y; // Assign to mTexCoord.
                mTexCoord[43].x = left+mBorderFixed.x; // Assign to mTexCoord.
                mTexCoord[43].y = bottom; // Assign to mTexCoord.
                mTexCoord[44].x = tail; // Assign to mTexCoord.
                mTexCoord[44].y = bottom-mBorderFixed.y; // Assign to mTexCoord.
                mTexCoord[45].x = tail; // Assign to mTexCoord.
                mTexCoord[45].y = bottom-mBorderFixed.y; // Assign to mTexCoord.
                mTexCoord[46].x = left+mBorderFixed.x; // Assign to mTexCoord.
                mTexCoord[46].y = bottom; // Assign to mTexCoord.
                mTexCoord[47].x = tail; // Assign to mTexCoord.
                mTexCoord[47].y = bottom; // Assign to mTexCoord.
            }
        } else {
            mTexCoord[42].x = left+mBorderFixed.x; // Assign to mTexCoord.
            mTexCoord[42].y = bottom-mBorderFixed.y; // Assign to mTexCoord.
            mTexCoord[43].x = left+mBorderFixed.x; // Assign to mTexCoord.
            mTexCoord[43].y = bottom; // Assign to mTexCoord.
            mTexCoord[44].x = right-mBorderFixed.x; // Assign to mTexCoord.
            mTexCoord[44].y = bottom-mBorderFixed.y; // Assign to mTexCoord.
            mTexCoord[45].x = right-mBorderFixed.x; // Assign to mTexCoord.
            mTexCoord[45].y = bottom-mBorderFixed.y; // Assign to mTexCoord.
            mTexCoord[46].x = left+mBorderFixed.x; // Assign to mTexCoord.
            mTexCoord[46].y = bottom; // Assign to mTexCoord.
            mTexCoord[47].x = right-mBorderFixed.x; // Assign to mTexCoord.
            mTexCoord[47].y = bottom; // Assign to mTexCoord.
        }

        mTexCoord[48].x = right-mBorderFixed.x; // Assign to mTexCoord.
        mTexCoord[48].y = bottom-mBorderFixed.y; // Assign to mTexCoord.
        mTexCoord[49].x = right-mBorderFixed.x; // Assign to mTexCoord.
        mTexCoord[49].y = bottom; // Assign to mTexCoord.
        mTexCoord[50].x = right; // Assign to mTexCoord.
        mTexCoord[50].y = bottom-mBorderFixed.y; // Assign to mTexCoord.
        mTexCoord[51].x = right; // Assign to mTexCoord.
        mTexCoord[51].y = bottom-mBorderFixed.y; // Assign to mTexCoord.
        mTexCoord[52].x = right-mBorderFixed.x; // Assign to mTexCoord.
        mTexCoord[52].y = bottom; // Assign to mTexCoord.
        mTexCoord[53].x = right; // Assign to mTexCoord.
        mTexCoord[53].y = bottom; // Assign to mTexCoord.

        if(mTailVisible)
        {
            if(!mTailDirection)
            {
                mTexCoord[54].x = tail; // Assign to mTexCoord.
                mTexCoord[54].y = bottom-mBorderFixed.y; // Assign to mTexCoord.
                mTexCoord[55].x = tail; // Assign to mTexCoord.
                mTexCoord[55].y = bottom+mBorderFixed.y; // Assign to mTexCoord.
                mTexCoord[56].x = tail+mBorderFixed.x; // Assign to mTexCoord.
                mTexCoord[56].y = bottom-mBorderFixed.y; // Assign to mTexCoord.
                mTexCoord[57].x = tail+mBorderFixed.x; // Assign to mTexCoord.
                mTexCoord[57].y = bottom-mBorderFixed.y; // Assign to mTexCoord.
                mTexCoord[58].x = tail; // Assign to mTexCoord.
                mTexCoord[58].y = bottom+mBorderFixed.y; // Assign to mTexCoord.
                mTexCoord[59].x = tail+mBorderFixed.x; // Assign to mTexCoord.
                mTexCoord[59].y = bottom+mBorderFixed.y; // Assign to mTexCoord.

                mTexCoord[60].x = tail+mBorderFixed.x; // Assign to mTexCoord.
                mTexCoord[60].y = bottom-mBorderFixed.y; // Assign to mTexCoord.
                mTexCoord[61].x = tail+mBorderFixed.x; // Assign to mTexCoord.
                mTexCoord[61].y = bottom; // Assign to mTexCoord.
                mTexCoord[62].x = right-mBorderFixed.x; // Assign to mTexCoord.
                mTexCoord[62].y = bottom-mBorderFixed.y; // Assign to mTexCoord.
                mTexCoord[63].x = right-mBorderFixed.x; // Assign to mTexCoord.
                mTexCoord[63].y = bottom-mBorderFixed.y; // Assign to mTexCoord.
                mTexCoord[64].x = tail+mBorderFixed.x; // Assign to mTexCoord.
                mTexCoord[64].y = bottom; // Assign to mTexCoord.
                mTexCoord[65].x = right-mBorderFixed.x; // Assign to mTexCoord.
                mTexCoord[65].y = bottom; // Assign to mTexCoord.
            } else {
                mTexCoord[54].x = tail; // Assign to mTexCoord.
                mTexCoord[54].y = top-mBorderFixed.y; // Assign to mTexCoord.
                mTexCoord[55].x = tail; // Assign to mTexCoord.
                mTexCoord[55].y = top+mBorderFixed.y; // Assign to mTexCoord.
                mTexCoord[56].x = tail+mBorderFixed.x; // Assign to mTexCoord.
                mTexCoord[56].y = top-mBorderFixed.y; // Assign to mTexCoord.
                mTexCoord[57].x = tail+mBorderFixed.x; // Assign to mTexCoord.
                mTexCoord[57].y = top-mBorderFixed.y; // Assign to mTexCoord.
                mTexCoord[58].x = tail; // Assign to mTexCoord.
                mTexCoord[58].y = top+mBorderFixed.y; // Assign to mTexCoord.
                mTexCoord[59].x = tail+mBorderFixed.x; // Assign to mTexCoord.
                mTexCoord[59].y = top+mBorderFixed.y; // Assign to mTexCoord.

                mTexCoord[60].x = tail+mBorderFixed.x; // Assign to mTexCoord.
                mTexCoord[60].y = top; // Assign to mTexCoord.
                mTexCoord[61].x = tail+mBorderFixed.x; // Assign to mTexCoord.
                mTexCoord[61].y = top+mBorderFixed.y; // Assign to mTexCoord.
                mTexCoord[62].x = right-mBorderFixed.x; // Assign to mTexCoord.
                mTexCoord[62].y = top; // Assign to mTexCoord.
                mTexCoord[63].x = right-mBorderFixed.x; // Assign to mTexCoord.
                mTexCoord[63].y = top; // Assign to mTexCoord.
                mTexCoord[64].x = tail+mBorderFixed.x; // Assign to mTexCoord.
                mTexCoord[64].y = top+mBorderFixed.y; // Assign to mTexCoord.
                mTexCoord[65].x = right-mBorderFixed.x; // Assign to mTexCoord.
                mTexCoord[65].y = top+mBorderFixed.y; // Assign to mTexCoord.
            }
        } else {
            mTexCoord[54].x = 0.0f; // Assign to mTexCoord.
            mTexCoord[54].y = 0.0f; // Assign to mTexCoord.
            mTexCoord[55].x = 0.0f; // Assign to mTexCoord.
            mTexCoord[55].y = 0.0f; // Assign to mTexCoord.
            mTexCoord[56].x = 0.0f; // Assign to mTexCoord.
            mTexCoord[56].y = 0.0f; // Assign to mTexCoord.
            mTexCoord[57].x = 0.0f; // Assign to mTexCoord.
            mTexCoord[57].y = 0.0f; // Assign to mTexCoord.
            mTexCoord[58].x = 0.0f; // Assign to mTexCoord.
            mTexCoord[58].y = 0.0f; // Assign to mTexCoord.
            mTexCoord[59].x = 0.0f; // Assign to mTexCoord.
            mTexCoord[59].y = 0.0f; // Assign to mTexCoord.

            mTexCoord[60].x = 0.0f; // Assign to mTexCoord.
            mTexCoord[60].y = 0.0f; // Assign to mTexCoord.
            mTexCoord[61].x = 0.0f; // Assign to mTexCoord.
            mTexCoord[61].y = 0.0f; // Assign to mTexCoord.
            mTexCoord[62].x = 0.0f; // Assign to mTexCoord.
            mTexCoord[62].y = 0.0f; // Assign to mTexCoord.
            mTexCoord[63].x = 0.0f; // Assign to mTexCoord.
            mTexCoord[63].y = 0.0f; // Assign to mTexCoord.
            mTexCoord[64].x = 0.0f; // Assign to mTexCoord.
            mTexCoord[64].y = 0.0f; // Assign to mTexCoord.
            mTexCoord[65].x = 0.0f; // Assign to mTexCoord.
            mTexCoord[65].y = 0.0f; // Assign to mTexCoord.
        }

        mGeomUVsOutOfDate = true;
    }
    //-----------------------------------------------------------------------------
} // namespace
