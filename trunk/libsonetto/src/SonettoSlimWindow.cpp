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
#include "SonettoKernel.h"
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
    //-----------------------------------------------------------------------------
    // Sonetto::SlimWindow implementation
    //-----------------------------------------------------------------------------
    // Binding defines
    #define POSITION_BINDING 0
    #define COLOUR_BINDING 1
    #define TEXCOORD_BINDING 2
    //-----------------------------------------------------------------------------
    // Ogre object type name
    Ogre::String SlimWindow::msTypeName = "SlimWindow";
    //-----------------------------------------------------------------------------
    SlimWindow::SlimWindow(const Ogre::String &name) : Ogre::OverlayContainer(name),
    mpWindowSkin(0), mWindowType(WT_MENU), mWindowAlpha(1.0f), mBorderAlpha(1.0f),
    mUpdateWindowColors(true), mUpdateBorderColors(true), mpBorderRenderable(0),
    mScrMetricsMode(SMM_RELATIVE_ASPECT_ADJUSTED),
    mAspectRatio(1.0f)
    {}
    //-----------------------------------------------------------------------------
    SlimWindow::~SlimWindow()
    {
        if(mInitialised)
        {
            delete mWindowRenderOp.vertexData;
            delete mBorderRenderOp.vertexData;
            delete mpBorderRenderable;
        }
    }
    //-----------------------------------------------------------------------------
    void SlimWindow::initialise(void)
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
            mWindowRenderOp.vertexData->vertexCount = SLIM_NUM_VERTEX;
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
            mpBorderRenderable           = new SlimBorderRenderable(this);
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
            mBorderRenderOp.vertexData->vertexCount = SLIM_NUM_VERTEX;

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
    void SlimWindow::setWindowSkin(WindowSkinPtr pwskin)
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
    WindowSkinPtr SlimWindow::getWindowSkin(void)
    {
        return mpWindowSkin;
    }
    //-----------------------------------------------------------------------------
    void SlimWindow::setWindowType(WindowType wtype)
    {
        mWindowType         = wtype;
        mUpdateWindowColors = true;
        mUpdateBorderColors = true;
    }
    //-----------------------------------------------------------------------------
    WindowType SlimWindow::getWindowType(void)
    {
        return mWindowType;
    }
    //-----------------------------------------------------------------------------
    void SlimWindow::setWindowAlpha(Ogre::Real alpha)
    {
        mWindowAlpha        = alpha;
        mUpdateWindowColors = true;
    }
    //-----------------------------------------------------------------------------
    Ogre::Real SlimWindow::getWindowAlpha(void)
    {
        return mWindowAlpha;
    }
    //-----------------------------------------------------------------------------
    void SlimWindow::setBorderAlpha(Ogre::Real alpha)
    {
        mBorderAlpha        = alpha;
        mUpdateBorderColors = true;
    }
    //-----------------------------------------------------------------------------
    Ogre::Real SlimWindow::getBorderAlpha(void)
    {
        return mBorderAlpha;
    }
    //-----------------------------------------------------------------------------
    void SlimWindow::setScrMetricsMode(ScreenMetricsMode smm)
    {
        mScrMetricsMode         = smm;
        // We need to update the positions and UVs now.
        mGeomPositionsOutOfDate = true;
        mGeomUVsOutOfDate       = true;
    }
    //-----------------------------------------------------------------------------
    ScreenMetricsMode SlimWindow::getScrMetricsMode(void)
    {
        return mScrMetricsMode;
    }
    //-----------------------------------------------------------------------------
    const Ogre::String & SlimWindow::getTypeName(void) const
    {
        return msTypeName;
    }
    //-----------------------------------------------------------------------------
    void SlimWindow::getRenderOperation(Ogre::RenderOperation &op)
    {
        op = mWindowRenderOp;
    }
    //-----------------------------------------------------------------------------
    void SlimWindow::_updateRenderQueue(Ogre::RenderQueue * queue)
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
    /*
    void SlimWindow::visitRenderables(Renderable::Visitor* visitor, bool debugRenderables)
    {
        visitor->visit(mpBorderRenderable, 0, false);
        OverlayContainer::visitRenderables(visitor, debugRenderables);
    }
    */
    //-----------------------------------------------------------------------------
    void SlimWindow::_update(void)
    {
        // Calculate the aspect ratio
        Ogre::Real tmpAspectRatio = Kernel::get()->mAspectRatio;

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
    void SlimWindow::updatePositionGeometry(void)
    {
        // Window Background
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
            *pPos++ = (((mPosCoord[i].x * 2) - 1)/mAspect);
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
            *pBorderPos++ = (((mPosCoord[i].x * 2) - 1)/mAspect);
            *pBorderPos++ = -(mPosCoord[i].y * 2) + 1;
            *pBorderPos++ = zValue;
        }
        // Ok, we're all set up to the next frames, for now.
        vbuf->unlock();
    }
    //-----------------------------------------------------------------------------
    void SlimWindow::updateTextureGeometry(void)
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
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_left.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_left.top;
        // Texcoord 1
        *pTex++ = mTexCoord[0].x;
        *pTex++ = mTexCoord[0].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_left.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_left.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord[1].x;
        *pTex++ = mTexCoord[1].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_left.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_left.top;
        // Texcoord 1
        *pTex++ = mTexCoord[2].x;
        *pTex++ = mTexCoord[2].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_left.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_left.top;
        // Texcoord 1
        *pTex++ = mTexCoord[3].x;
        *pTex++ = mTexCoord[3].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_left.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_left.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord[4].x;
        *pTex++ = mTexCoord[4].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_left.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_left.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord[5].x;
        *pTex++ = mTexCoord[5].y;

        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_center.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_center.top;
        // Texcoord 1
        *pTex++ = mTexCoord[6].x;
        *pTex++ = mTexCoord[6].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_center.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_center.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord[7].x;
        *pTex++ = mTexCoord[7].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_center.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_center.top;
        // Texcoord 1
        *pTex++ = mTexCoord[8].x;
        *pTex++ = mTexCoord[8].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_center.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_center.top;
        // Texcoord 1
        *pTex++ = mTexCoord[9].x;
        *pTex++ = mTexCoord[9].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_center.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_center.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord[10].x;
        *pTex++ = mTexCoord[10].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_center.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_center.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord[11].x;
        *pTex++ = mTexCoord[11].y;

        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_right.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_right.top;
        // Texcoord 1
        *pTex++ = mTexCoord[12].x;
        *pTex++ = mTexCoord[12].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_right.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_right.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord[13].x;
        *pTex++ = mTexCoord[13].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_right.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_right.top;
        // Texcoord 1
        *pTex++ = mTexCoord[14].x;
        *pTex++ = mTexCoord[14].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_right.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_right.top;
        // Texcoord 1
        *pTex++ = mTexCoord[15].x;
        *pTex++ = mTexCoord[15].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_right.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_right.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord[16].x;
        *pTex++ = mTexCoord[16].y;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_right.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_right.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord[17].x;
        *pTex++ = mTexCoord[17].y;

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
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_left.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_left.top;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_left.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_left.bottom;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_left.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_left.top;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_left.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_left.top;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_left.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_left.bottom;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_left.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_left.bottom;

        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_center.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_center.top;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_center.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_center.bottom;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_center.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_center.top;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_center.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_center.top;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_center.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_center.bottom;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_center.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_center.bottom;

        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_right.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_right.top;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_right.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_right.bottom;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_right.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_right.top;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_right.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_right.top;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_right.left;
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_right.bottom;
        // Texcoord 0
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_right.right;
        *pTex++ = mpWindowSkin->mWinTexCoord.slim_right.bottom;

        vbuf->unlock();
    }
    //-----------------------------------------------------------------------------
    void SlimWindow::updateWindowColors(void)
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
    void SlimWindow::updateBorderColors(void)
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
    void SlimWindow::calculatePosition(void)
    {
        float left, right, top, bottom;

        left = _getDerivedLeft();
        right = left + mWidth;
        top = _getDerivedTop();
        bottom =  top + mpWindowSkin->mBorderDimensions.y;

        mPosCoord[0].x = left; // Assign to mPosCoord.
        mPosCoord[0].y = top; // Assign to mPosCoord.
        mPosCoord[1].x = left; // Assign to mPosCoord.
        mPosCoord[1].y = bottom; // Assign to mPosCoord.
        mPosCoord[2].x = left+mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
        mPosCoord[2].y = top; // Assign to mPosCoord.
        mPosCoord[3].x = left+mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
        mPosCoord[3].y = top; // Assign to mPosCoord.
        mPosCoord[4].x = left; // Assign to mPosCoord.
        mPosCoord[4].y = bottom; // Assign to mPosCoord.
        mPosCoord[5].x = left+mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
        mPosCoord[5].y = bottom; // Assign to mPosCoord.

        mPosCoord[6].x = left+mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
        mPosCoord[6].y = top; // Assign to mPosCoord.
        mPosCoord[7].x = left+mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
        mPosCoord[7].y = bottom; // Assign to mPosCoord.
        mPosCoord[8].x = right-mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
        mPosCoord[8].y = top; // Assign to mPosCoord.
        mPosCoord[9].x = right-mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
        mPosCoord[9].y = top; // Assign to mPosCoord.
        mPosCoord[10].x = left+mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
        mPosCoord[10].y = bottom; // Assign to mPosCoord.
        mPosCoord[11].x = right-mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
        mPosCoord[11].y = bottom; // Assign to mPosCoord.

        mPosCoord[12].x = right-mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
        mPosCoord[12].y = top; // Assign to mPosCoord.
        mPosCoord[13].x = right-mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
        mPosCoord[13].y = bottom; // Assign to mPosCoord.
        mPosCoord[14].x = right; // Assign to mPosCoord.
        mPosCoord[14].y = top; // Assign to mPosCoord.
        mPosCoord[15].x = right; // Assign to mPosCoord.
        mPosCoord[15].y = top; // Assign to mPosCoord.
        mPosCoord[16].x = right-mpWindowSkin->mBorderDimensions.x; // Assign to mPosCoord.
        mPosCoord[16].y = bottom; // Assign to mPosCoord.
        mPosCoord[17].x = right; // Assign to mPosCoord.
        mPosCoord[17].y = bottom; // Assign to mPosCoord.

        mGeomPositionsOutOfDate = true;
    }
    //-----------------------------------------------------------------------------
    void SlimWindow::calculateTexCoord(void)
    {
        float left, right, top, bottom;

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

        mTexCoord[0].x = left; // Assign to mTexCoord.
        mTexCoord[0].y = top; // Assign to mTexCoord.
        mTexCoord[1].x = left; // Assign to mTexCoord.
        mTexCoord[1].y = bottom; // Assign to mTexCoord.
        mTexCoord[2].x = left+mBorderFixed.x; // Assign to mTexCoord.
        mTexCoord[2].y = top; // Assign to mTexCoord.
        mTexCoord[3].x = left+mBorderFixed.x; // Assign to mTexCoord.
        mTexCoord[3].y = top; // Assign to mTexCoord.
        mTexCoord[4].x = left; // Assign to mTexCoord.
        mTexCoord[4].y = bottom; // Assign to mTexCoord.
        mTexCoord[5].x = left+mBorderFixed.x; // Assign to mTexCoord.
        mTexCoord[5].y = bottom; // Assign to mTexCoord.

        mTexCoord[6].x = left+mBorderFixed.x; // Assign to mTexCoord.
        mTexCoord[6].y = top; // Assign to mTexCoord.
        mTexCoord[7].x = left+mBorderFixed.x; // Assign to mTexCoord.
        mTexCoord[7].y = bottom; // Assign to mTexCoord.
        mTexCoord[8].x = right-mBorderFixed.x; // Assign to mTexCoord.
        mTexCoord[8].y = top; // Assign to mTexCoord.
        mTexCoord[9].x = right-mBorderFixed.x; // Assign to mTexCoord.
        mTexCoord[9].y = top; // Assign to mTexCoord.
        mTexCoord[10].x = left+mBorderFixed.x; // Assign to mTexCoord.
        mTexCoord[10].y = bottom; // Assign to mTexCoord.
        mTexCoord[11].x = right-mBorderFixed.x; // Assign to mTexCoord.
        mTexCoord[11].y = bottom; // Assign to mTexCoord.

        mTexCoord[12].x = right-mBorderFixed.x; // Assign to mTexCoord.
        mTexCoord[12].y = top; // Assign to mTexCoord.
        mTexCoord[13].x = right-mBorderFixed.x; // Assign to mTexCoord.
        mTexCoord[13].y = bottom; // Assign to mTexCoord.
        mTexCoord[14].x = right; // Assign to mTexCoord.
        mTexCoord[14].y = top; // Assign to mTexCoord.
        mTexCoord[15].x = right; // Assign to mTexCoord.
        mTexCoord[15].y = top; // Assign to mTexCoord.
        mTexCoord[16].x = right-mBorderFixed.x; // Assign to mTexCoord.
        mTexCoord[16].y = bottom; // Assign to mTexCoord.
        mTexCoord[17].x = right; // Assign to mTexCoord.
        mTexCoord[17].y = bottom; // Assign to mTexCoord.

        mGeomUVsOutOfDate = true;
    }
    //-----------------------------------------------------------------------------
} // namespace
