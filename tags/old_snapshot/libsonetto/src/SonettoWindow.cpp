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

#include "SonettoKernel.h"
#include <OgreHardwareBufferManager.h>
#include <OgreMaterial.h>
#include <OgreMaterialManager.h>
#include <OgreOverlayManager.h>
#include <OgrePass.h>
#include <OgreStableHeaders.h>
#include <OgreStringConverter.h>
#include <OgreRoot.h>
#include <OgreRenderSystem.h>
#include <OgreTechnique.h>
#include "SonettoWindow.h"

namespace Sonetto {
    //-----------------------------------------------------------------------------
    // Sonetto::Window implementation
    //-----------------------------------------------------------------------------
    // Binding defines
    #define POSITION_BINDING 0
    #define COLOUR_BINDING 1
    #define TEXCOORD_BINDING 2
    //-----------------------------------------------------------------------------
    // Ogre object type name
    Ogre::String Window::msTypeName = "Window";
    //-----------------------------------------------------------------------------
    Window::Window(const Ogre::String &name) : Ogre::OverlayContainer(name),
    mpWindowSkin(0), mWindowType(WT_MENU), mWindowAlpha(1.0f), mBorderAlpha(1.0f),
    mUpdateWindowColors(true), mUpdateBorderColors(true), mpBorderRenderable(0),
    mScrMetricsMode(SMM_RELATIVE_ASPECT_ADJUSTED),
    mAspectRatio(1.0f)
    {}
    //-----------------------------------------------------------------------------
    Window::~Window()
    {
        if(mInitialised)
        {
            delete mWindowRenderOp.vertexData;
            delete mBorderRenderOp.vertexData;
            delete mpBorderRenderable;
        }
    }
    //-----------------------------------------------------------------------------
    void Window::initialise(void)
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
            mWindowRenderOp.vertexData->vertexCount = NUM_VERTEX;
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
            mpBorderRenderable           = new WindowBorderRenderable(this);
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
            mBorderRenderOp.vertexData->vertexCount = NUM_VERTEX;

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
    void Window::setWindowSkin(WindowSkinPtr pwskin)
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
    WindowSkinPtr Window::getWindowSkin(void)
    {
        return mpWindowSkin;
    }
    //-----------------------------------------------------------------------------
    void Window::setWindowType(WindowType wtype)
    {
        mWindowType         = wtype;
        mUpdateWindowColors = true;
        mUpdateBorderColors = true;
    }
    //-----------------------------------------------------------------------------
    WindowType Window::getWindowType(void)
    {
        return mWindowType;
    }
    //-----------------------------------------------------------------------------
    void Window::setWindowAlpha(Ogre::Real alpha)
    {
        mWindowAlpha        = alpha;
        mUpdateWindowColors = true;
    }
    //-----------------------------------------------------------------------------
    Ogre::Real Window::getWindowAlpha(void)
    {
        return mWindowAlpha;
    }
    //-----------------------------------------------------------------------------
    void Window::setBorderAlpha(Ogre::Real alpha)
    {
        mBorderAlpha        = alpha;
        mUpdateBorderColors = true;
    }
    //-----------------------------------------------------------------------------
    Ogre::Real Window::getBorderAlpha(void)
    {
        return mBorderAlpha;
    }
    //-----------------------------------------------------------------------------
    void Window::setScrMetricsMode(ScreenMetricsMode smm)
    {
        mScrMetricsMode         = smm;
        // We need to update the positions and UVs now.
        mGeomPositionsOutOfDate = true;
        mGeomUVsOutOfDate       = true;
    }
    //-----------------------------------------------------------------------------
    ScreenMetricsMode Window::getScrMetricsMode(void)
    {
        return mScrMetricsMode;
    }
    //-----------------------------------------------------------------------------
    const Ogre::String & Window::getTypeName(void) const
    {
        return msTypeName;
    }
    //-----------------------------------------------------------------------------
    void Window::getRenderOperation(Ogre::RenderOperation &op)
    {
        op = mWindowRenderOp;
    }
    //-----------------------------------------------------------------------------
    void Window::_updateRenderQueue(Ogre::RenderQueue * queue)
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
    // Only for Ogre 1.5.0
    /*
    void Window::visitRenderables(Renderable::Visitor* visitor, bool debugRenderables)
    {
        visitor->visit(mpBorderRenderable, 0, false);
        OverlayContainer::visitRenderables(visitor, debugRenderables);
    }
    */
    //-----------------------------------------------------------------------------
    void Window::_update(void)
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
    void Window::updatePositionGeometry(void)
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
            //*pPos++ = ((mPosCoord[i].x * (mAspect * 2)) - mAspect);
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
            //*pBorderPos++ = ((mPosCoord[i].x * (mAspect * 2)) - mAspect);
            *pBorderPos++ = (((mPosCoord[i].x * 2) - 1)/mAspect);
            *pBorderPos++ = -(mPosCoord[i].y * 2) + 1;
            *pBorderPos++ = zValue;
        }
        // Ok, we're all set up to the next frames, for now.
        vbuf->unlock();
    }
    //-----------------------------------------------------------------------------
    void Window::updateTextureGeometry(void)
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

        vbuf->unlock();
    }
    //-----------------------------------------------------------------------------
    void Window::updateWindowColors(void)
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
    void Window::updateBorderColors(void)
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
    void Window::calculatePosition(void)
    {
        float left, right, top, bottom;

        left = _getDerivedLeft();
        right = left + mWidth;
        top = _getDerivedTop();
        bottom =  top + mHeight;

        // Assign to mPosCoord.
        mPosCoord[0].x = left;
        // Assign to mPosCoord.
        mPosCoord[0].y = top;
        // Assign to mPosCoord.
        mPosCoord[1].x = left;
        // Assign to mPosCoord.
        mPosCoord[1].y = top+mpWindowSkin->mBorderDimensions.y;
        // Assign to mPosCoord.
        mPosCoord[2].x = left+mpWindowSkin->mBorderDimensions.x;
        // Assign to mPosCoord.
        mPosCoord[2].y = top;
        // Assign to mPosCoord.
        mPosCoord[3].x = left+mpWindowSkin->mBorderDimensions.x;
        // Assign to mPosCoord.
        mPosCoord[3].y = top;
        // Assign to mPosCoord.
        mPosCoord[4].x = left;
        // Assign to mPosCoord.
        mPosCoord[4].y = top+mpWindowSkin->mBorderDimensions.y;
        // Assign to mPosCoord.
        mPosCoord[5].x = left+mpWindowSkin->mBorderDimensions.x;
        // Assign to mPosCoord.
        mPosCoord[5].y = top+mpWindowSkin->mBorderDimensions.y;

                // Assign to mPosCoord.
                mPosCoord[6].x = left+mpWindowSkin->mBorderDimensions.x;
                // Assign to mPosCoord.
                mPosCoord[6].y = top;
                // Assign to mPosCoord.
                mPosCoord[7].x = left+mpWindowSkin->mBorderDimensions.x;
                // Assign to mPosCoord.
                mPosCoord[7].y = top+mpWindowSkin->mBorderDimensions.y;
                // Assign to mPosCoord.
                mPosCoord[8].x = right-mpWindowSkin->mBorderDimensions.x;
                // Assign to mPosCoord.
                mPosCoord[8].y = top;// Assign to mPosCoord.
                mPosCoord[9].x = right-mpWindowSkin->mBorderDimensions.x;
                // Assign to mPosCoord.
                mPosCoord[9].y = top;
                // Assign to mPosCoord.
                mPosCoord[10].x = left+mpWindowSkin->mBorderDimensions.x;
                // Assign to mPosCoord.
                mPosCoord[10].y = top+mpWindowSkin->mBorderDimensions.y;
                // Assign to mPosCoord.
                mPosCoord[11].x = right-mpWindowSkin->mBorderDimensions.x;
                // Assign to mPosCoord.
                mPosCoord[11].y = top+mpWindowSkin->mBorderDimensions.y;

        // Assign to mPosCoord.
        mPosCoord[12].x = right-mpWindowSkin->mBorderDimensions.x;
        // Assign to mPosCoord.
        mPosCoord[12].y = top;
        // Assign to mPosCoord.
        mPosCoord[13].x = right-mpWindowSkin->mBorderDimensions.x;
        // Assign to mPosCoord.
        mPosCoord[13].y = top+mpWindowSkin->mBorderDimensions.y;
        // Assign to mPosCoord.
        mPosCoord[14].x = right;
        // Assign to mPosCoord.
        mPosCoord[14].y = top;
        // Assign to mPosCoord.
        mPosCoord[15].x = right;
        // Assign to mPosCoord.
        mPosCoord[15].y = top;
        // Assign to mPosCoord.
        mPosCoord[16].x = right-mpWindowSkin->mBorderDimensions.x;
        // Assign to mPosCoord.
        mPosCoord[16].y = top+mpWindowSkin->mBorderDimensions.y;
        // Assign to mPosCoord.
        mPosCoord[17].x = right;
        // Assign to mPosCoord.
        mPosCoord[17].y = top+mpWindowSkin->mBorderDimensions.y;

        // Assign to mPosCoord.
        mPosCoord[18].x = left;
        // Assign to mPosCoord.
        mPosCoord[18].y = top+mpWindowSkin->mBorderDimensions.y;
        // Assign to mPosCoord.
        mPosCoord[19].x = left;
        // Assign to mPosCoord.
        mPosCoord[19].y = bottom-mpWindowSkin->mBorderDimensions.y;
        // Assign to mPosCoord.
        mPosCoord[20].x = left+mpWindowSkin->mBorderDimensions.x;
        // Assign to mPosCoord.
        mPosCoord[20].y = top+mpWindowSkin->mBorderDimensions.y;
        // Assign to mPosCoord.
        mPosCoord[21].x = left+mpWindowSkin->mBorderDimensions.x;
        // Assign to mPosCoord.
        mPosCoord[21].y = top+mpWindowSkin->mBorderDimensions.y;
        // Assign to mPosCoord.
        mPosCoord[22].x = left;
        // Assign to mPosCoord.
        mPosCoord[22].y = bottom-mpWindowSkin->mBorderDimensions.y;
        // Assign to mPosCoord.
        mPosCoord[23].x = left+mpWindowSkin->mBorderDimensions.x;
        // Assign to mPosCoord.
        mPosCoord[23].y = bottom-mpWindowSkin->mBorderDimensions.y;

        // Assign to mPosCoord.
        mPosCoord[24].x = left+mpWindowSkin->mBorderDimensions.x;
        // Assign to mPosCoord.
        mPosCoord[24].y = top+mpWindowSkin->mBorderDimensions.y;
        // Assign to mPosCoord.
        mPosCoord[25].x = left+mpWindowSkin->mBorderDimensions.x;
        // Assign to mPosCoord.
        mPosCoord[25].y = bottom-mpWindowSkin->mBorderDimensions.y;
        // Assign to mPosCoord.
        mPosCoord[26].x = right-mpWindowSkin->mBorderDimensions.x;
        // Assign to mPosCoord.
        mPosCoord[26].y = top+mpWindowSkin->mBorderDimensions.y;
        // Assign to mPosCoord.
        mPosCoord[27].x = right-mpWindowSkin->mBorderDimensions.x;
        // Assign to mPosCoord.
        mPosCoord[27].y = top+mpWindowSkin->mBorderDimensions.y;
        // Assign to mPosCoord.
        mPosCoord[28].x = left+mpWindowSkin->mBorderDimensions.x;
        // Assign to mPosCoord.
        mPosCoord[28].y = bottom-mpWindowSkin->mBorderDimensions.y;
        // Assign to mPosCoord.
        mPosCoord[29].x = right-mpWindowSkin->mBorderDimensions.x;
        // Assign to mPosCoord.
        mPosCoord[29].y = bottom-mpWindowSkin->mBorderDimensions.y;

        // Assign to mPosCoord.
        mPosCoord[30].x = right-mpWindowSkin->mBorderDimensions.x;
        // Assign to mPosCoord.
        mPosCoord[30].y = top+mpWindowSkin->mBorderDimensions.y;
        // Assign to mPosCoord.
        mPosCoord[30].y = top+mpWindowSkin->mBorderDimensions.y;
        // Assign to mPosCoord.
        mPosCoord[31].x = right-mpWindowSkin->mBorderDimensions.x;
        // Assign to mPosCoord.
        mPosCoord[31].y = bottom-mpWindowSkin->mBorderDimensions.y;
        // Assign to mPosCoord.
        mPosCoord[32].x = right;
        // Assign to mPosCoord.
        mPosCoord[32].y = top+mpWindowSkin->mBorderDimensions.y;
        // Assign to mPosCoord.
        mPosCoord[33].x = right;
        // Assign to mPosCoord.
        mPosCoord[33].y = top+mpWindowSkin->mBorderDimensions.y;
        // Assign to mPosCoord.
        mPosCoord[34].x = right-mpWindowSkin->mBorderDimensions.x;
        // Assign to mPosCoord.
        mPosCoord[34].y = bottom-mpWindowSkin->mBorderDimensions.y;
        // Assign to mPosCoord.
        mPosCoord[35].x = right;// Assign to mPosCoord.
        mPosCoord[35].y = bottom-mpWindowSkin->mBorderDimensions.y;

        // Assign to mPosCoord.
        mPosCoord[36].x = left;
        // Assign to mPosCoord.
        mPosCoord[36].y = bottom-mpWindowSkin->mBorderDimensions.y;
        // Assign to mPosCoord.
        mPosCoord[37].x = left;
        // Assign to mPosCoord.
        mPosCoord[37].y = bottom;
        // Assign to mPosCoord.
        mPosCoord[38].x = left+mpWindowSkin->mBorderDimensions.x;
        // Assign to mPosCoord.
        mPosCoord[38].y = bottom-mpWindowSkin->mBorderDimensions.y;
        // Assign to mPosCoord.
        mPosCoord[39].x = left+mpWindowSkin->mBorderDimensions.x;
        // Assign to mPosCoord.
        mPosCoord[39].y = bottom-mpWindowSkin->mBorderDimensions.y;
        // Assign to mPosCoord.
        mPosCoord[40].x = left;
        // Assign to mPosCoord.
        mPosCoord[40].y = bottom;
        // Assign to mPosCoord.
        mPosCoord[41].x = left+mpWindowSkin->mBorderDimensions.x;
        // Assign to mPosCoord.
        mPosCoord[41].y = bottom;

            // Assign to mPosCoord.
            mPosCoord[42].x = left+mpWindowSkin->mBorderDimensions.x;
            // Assign to mPosCoord.
            mPosCoord[42].y = bottom-mpWindowSkin->mBorderDimensions.y;
            // Assign to mPosCoord.
            mPosCoord[43].x = left+mpWindowSkin->mBorderDimensions.x;
            // Assign to mPosCoord.
            mPosCoord[43].y = bottom;
            // Assign to mPosCoord.
            mPosCoord[44].x = right-mpWindowSkin->mBorderDimensions.x;
            // Assign to mPosCoord.
            mPosCoord[44].y = bottom-mpWindowSkin->mBorderDimensions.y;
            // Assign to mPosCoord.
            mPosCoord[45].x = right-mpWindowSkin->mBorderDimensions.x;
            // Assign to mPosCoord.
            mPosCoord[45].y = bottom-mpWindowSkin->mBorderDimensions.y;
            // Assign to mPosCoord.
            mPosCoord[46].x = left+mpWindowSkin->mBorderDimensions.x;
            // Assign to mPosCoord.
            mPosCoord[46].y = bottom;// Assign to mPosCoord.
            mPosCoord[47].x = right-mpWindowSkin->mBorderDimensions.x;
            // Assign to mPosCoord.
            mPosCoord[47].y = bottom;

        // Assign to mPosCoord.
        mPosCoord[48].x = right-mpWindowSkin->mBorderDimensions.x;
        // Assign to mPosCoord.
        mPosCoord[48].y = bottom-mpWindowSkin->mBorderDimensions.y;
        // Assign to mPosCoord.
        mPosCoord[49].x = right-mpWindowSkin->mBorderDimensions.x;
        // Assign to mPosCoord.
        mPosCoord[49].y = bottom;
        // Assign to mPosCoord.
        mPosCoord[50].x = right;
        // Assign to mPosCoord.
        mPosCoord[50].y = bottom-mpWindowSkin->mBorderDimensions.y;
        // Assign to mPosCoord.
        mPosCoord[51].x = right;
        // Assign to mPosCoord.
        mPosCoord[51].y = bottom-mpWindowSkin->mBorderDimensions.y;
        // Assign to mPosCoord.
        mPosCoord[52].x = right-mpWindowSkin->mBorderDimensions.x;
        // Assign to mPosCoord.
        mPosCoord[52].y = bottom;
        // Assign to mPosCoord.
        mPosCoord[53].x = right;
        // Assign to mPosCoord.
        mPosCoord[53].y = bottom;

        mGeomPositionsOutOfDate = true;
    }
    //-----------------------------------------------------------------------------
    void Window::calculateTexCoord(void)
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
        // Assign to mTexCoord.
        mTexCoord[0].x = left;
        // Assign to mTexCoord.
        mTexCoord[0].y = top;
        // Assign to mTexCoord.
        mTexCoord[1].x = left;
        // Assign to mTexCoord.
        mTexCoord[1].y = top+mBorderFixed.y;
        // Assign to mTexCoord.
        mTexCoord[2].x = left+mBorderFixed.x;
        // Assign to mTexCoord.
        mTexCoord[2].y = top;
        // Assign to mTexCoord.
        mTexCoord[3].x = left+mBorderFixed.x;
        // Assign to mTexCoord.
        mTexCoord[3].y = top;
        // Assign to mTexCoord.
        mTexCoord[4].x = left;
        // Assign to mTexCoord.
        mTexCoord[4].y = top+mBorderFixed.y;
        // Assign to mTexCoord.
        mTexCoord[5].x = left+mBorderFixed.x;
        // Assign to mTexCoord.
        mTexCoord[5].y = top+mBorderFixed.y;

        // Assign to mTexCoord.
        mTexCoord[6].x = left+mBorderFixed.x;
        // Assign to mTexCoord.
        mTexCoord[6].y = top;
        // Assign to mTexCoord.
        mTexCoord[7].x = left+mBorderFixed.x;
        // Assign to mTexCoord.
        mTexCoord[7].y = top+mBorderFixed.y;
        // Assign to mTexCoord.
        mTexCoord[8].x = right-mBorderFixed.x;
        // Assign to mTexCoord.
        mTexCoord[8].y = top;
        // Assign to mTexCoord.
        mTexCoord[9].x = right-mBorderFixed.x;
        // Assign to mTexCoord.
        mTexCoord[9].y = top;
        // Assign to mTexCoord.
        mTexCoord[10].x = left+mBorderFixed.x;
        // Assign to mTexCoord.
        mTexCoord[10].y = top+mBorderFixed.y;
        // Assign to mTexCoord.
        mTexCoord[11].x = right-mBorderFixed.x;
        // Assign to mTexCoord.
        mTexCoord[11].y = top+mBorderFixed.y;

        // Assign to mTexCoord.
        mTexCoord[12].x = right-mBorderFixed.x;
        // Assign to mTexCoord.
        mTexCoord[12].y = top;
        // Assign to mTexCoord.
        mTexCoord[13].x = right-mBorderFixed.x;
        // Assign to mTexCoord.
        mTexCoord[13].y = top+mBorderFixed.y;
        // Assign to mTexCoord.
        mTexCoord[14].x = right;
        // Assign to mTexCoord.
        mTexCoord[14].y = top;
        // Assign to mTexCoord.
        mTexCoord[15].x = right;
        // Assign to mTexCoord.
        mTexCoord[15].y = top;
        // Assign to mTexCoord.
        mTexCoord[16].x = right-mBorderFixed.x;
        // Assign to mTexCoord.
        mTexCoord[16].y = top+mBorderFixed.y;
        // Assign to mTexCoord.
        mTexCoord[17].x = right;
        // Assign to mTexCoord.
        mTexCoord[17].y = top+mBorderFixed.y;

        // Assign to mTexCoord.
        mTexCoord[18].x = left;
        // Assign to mTexCoord.
        mTexCoord[18].y = top+mBorderFixed.y;
        // Assign to mTexCoord.
        mTexCoord[19].x = left;
        // Assign to mTexCoord.
        mTexCoord[19].y = bottom-mBorderFixed.y;
        // Assign to mTexCoord.
        mTexCoord[20].x = left+mBorderFixed.x;
        // Assign to mTexCoord.
        mTexCoord[20].y = top+mBorderFixed.y;
        // Assign to mTexCoord.
        mTexCoord[21].x = left+mBorderFixed.x;
        // Assign to mTexCoord.
        mTexCoord[21].y = top+mBorderFixed.y;
        // Assign to mTexCoord.
        mTexCoord[22].x = left;
        // Assign to mTexCoord.
        mTexCoord[22].y = bottom-mBorderFixed.y;
        // Assign to mTexCoord.
        mTexCoord[23].x = left+mBorderFixed.x;
        // Assign to mTexCoord.
        mTexCoord[23].y = bottom-mBorderFixed.y;

        // Assign to mTexCoord.
        mTexCoord[24].x = left+mBorderFixed.x;
        // Assign to mTexCoord.
        mTexCoord[24].y = top+mBorderFixed.y;
        // Assign to mTexCoord.
        mTexCoord[25].x = left+mBorderFixed.x;
        // Assign to mTexCoord.
        mTexCoord[25].y = bottom-mBorderFixed.y;
        // Assign to mTexCoord.
        mTexCoord[26].x = right-mBorderFixed.x;
        // Assign to mTexCoord.
        mTexCoord[26].y = top+mBorderFixed.y;
        // Assign to mTexCoord.
        mTexCoord[27].x = right-mBorderFixed.x;
        // Assign to mTexCoord.
        mTexCoord[27].y = top+mBorderFixed.y;
        // Assign to mTexCoord.
        mTexCoord[28].x = left+mBorderFixed.x;
        // Assign to mTexCoord.
        mTexCoord[28].y = bottom-mBorderFixed.y;
        // Assign to mTexCoord.
        mTexCoord[29].x = right-mBorderFixed.x;
        // Assign to mTexCoord.
        mTexCoord[29].y = bottom-mBorderFixed.y;

        // Assign to mTexCoord.
        mTexCoord[30].x = right-mBorderFixed.x;
        // Assign to mTexCoord.
        mTexCoord[30].y = top+mBorderFixed.y;
        // Assign to mTexCoord.
        mTexCoord[31].x = right-mBorderFixed.x;
        // Assign to mTexCoord.
        mTexCoord[31].y = bottom-mBorderFixed.y;
        // Assign to mTexCoord.
        mTexCoord[32].x = right;
        // Assign to mTexCoord.
        mTexCoord[32].y = top+mBorderFixed.y;
        // Assign to mTexCoord.
        mTexCoord[33].x = right;
        // Assign to mTexCoord.
        mTexCoord[33].y = top+mBorderFixed.y;
        // Assign to mTexCoord.
        mTexCoord[34].x = right-mBorderFixed.x;
        // Assign to mTexCoord.
        mTexCoord[34].y = bottom-mBorderFixed.y;
        // Assign to mTexCoord.
        mTexCoord[35].x = right;
        // Assign to mTexCoord.
        mTexCoord[35].y = bottom-mBorderFixed.y;

        // Assign to mTexCoord.
        mTexCoord[36].x = left;
        // Assign to mTexCoord.
        mTexCoord[36].y = bottom-mBorderFixed.y;
        // Assign to mTexCoord.
        mTexCoord[37].x = left;
        // Assign to mTexCoord.
        mTexCoord[37].y = bottom;
        // Assign to mTexCoord.
        mTexCoord[38].x = left+mBorderFixed.x;
        // Assign to mTexCoord.
        mTexCoord[38].y = bottom-mBorderFixed.y;
        // Assign to mTexCoord.
        mTexCoord[39].x = left+mBorderFixed.x;
        // Assign to mTexCoord.
        mTexCoord[39].y = bottom-mBorderFixed.y;
        // Assign to mTexCoord.
        mTexCoord[40].x = left;
        // Assign to mTexCoord.
        mTexCoord[40].y = bottom;
        // Assign to mTexCoord.
        mTexCoord[41].x = left+mBorderFixed.x;
        // Assign to mTexCoord.
        mTexCoord[41].y = bottom;

        // Assign to mTexCoord.
        mTexCoord[42].x = left+mBorderFixed.x;
        // Assign to mTexCoord.
        mTexCoord[42].y = bottom-mBorderFixed.y;
        // Assign to mTexCoord.
        mTexCoord[43].x = left+mBorderFixed.x;
        // Assign to mTexCoord.
        mTexCoord[43].y = bottom;
        // Assign to mTexCoord.
        mTexCoord[44].x = right-mBorderFixed.x;
        // Assign to mTexCoord.
        mTexCoord[44].y = bottom-mBorderFixed.y;
        // Assign to mTexCoord.
        mTexCoord[45].x = right-mBorderFixed.x;
        // Assign to mTexCoord.
        mTexCoord[45].y = bottom-mBorderFixed.y;
        // Assign to mTexCoord.
        mTexCoord[46].x = left+mBorderFixed.x;
        // Assign to mTexCoord.
        mTexCoord[46].y = bottom;
        // Assign to mTexCoord.
        mTexCoord[47].x = right-mBorderFixed.x;
        //Assign to mTexCoord.
        mTexCoord[47].y = bottom;
        // Assign to mTexCoord.

        mTexCoord[48].x = right-mBorderFixed.x;
        // Assign to mTexCoord.
        mTexCoord[48].y = bottom-mBorderFixed.y;
        // Assign to mTexCoord.
        mTexCoord[49].x = right-mBorderFixed.x;
        // Assign to mTexCoord.
        mTexCoord[49].y = bottom;
        // Assign to mTexCoord.
        mTexCoord[50].x = right;
        // Assign to mTexCoord.
        mTexCoord[50].y = bottom-mBorderFixed.y;
        // Assign to mTexCoord.
        mTexCoord[51].x = right;
        // Assign to mTexCoord.
        mTexCoord[51].y = bottom-mBorderFixed.y;
        // Assign to mTexCoord.
        mTexCoord[52].x = right-mBorderFixed.x;
        // Assign to mTexCoord.
        mTexCoord[52].y = bottom;
        // Assign to mTexCoord.
        mTexCoord[53].x = right;
        // Assign to mTexCoord.
        mTexCoord[53].y = bottom;

        mGeomUVsOutOfDate = true;
    }
    //-----------------------------------------------------------------------------
}; // namespace
