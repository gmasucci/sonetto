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
    Ogre::String TailedWindow::msTypeName = "TailedWindow";
#define POSITION_BINDING 0
#define COLOUR_BINDING 1
#define TEXCOORD_BINDING 2
    TailedWindow::TailedWindow(const Ogre::String& name): Ogre::OverlayContainer(name),
            mUpdateBorderColors(true),
            mUpdateBackgroundColors(true),
            mBorderRenderable(0),
            mBorderSize(Ogre::Vector2(24/480.0f,24/480.0f)),
            mBackgroundColor(Ogre::ColourValue(1.0f,1.0f,1.0f,1.0f)),
            mBorderColor(Ogre::ColourValue(1.0f,1.0f,1.0f,1.0f)),
            mTileMode(true),
            mTiling(Ogre::Vector2(7.5f,7.5f)),
            mTailVisible(true),
            mTailDirection(false),
            mTailPosition(0.1f),
            mTexCoord0(0),
            mScrMetricsMode(SMM_RELATIVE_ASPECT_ADJUSTED),
            mAspectRatio(1.3333333333333333333333333333333f)
    {
        //mScrMetricsMode = SMM_RELATIVE_ASPECT_ADJUSTED;
    }
    TailedWindow::~TailedWindow()
    {
        delete mRenderOp.vertexData;
        delete mRenderOp2.vertexData;
        delete mBorderRenderable;
    }
    void TailedWindow::initialise (void)
    {
        bool init = !mInitialised;
        OverlayContainer::initialise();
        if (init)
        {
            //-------------------
            // TailedWindow background
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
            mRenderOp.vertexData->vertexCount = TAILED_NUM_VERTEX;
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
            // TailedWindow border
            //----------------------
            mBorderRenderable = new TailedBorderRenderable(this);
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
            mRenderOp2.vertexData->vertexCount = TAILED_NUM_VERTEX;

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
    void TailedWindow::setScrMetricsMode(ScreenMetricsMode smm)
    {
        mScrMetricsMode = smm;
    }
    void TailedWindow::setBorderSize(const Ogre::Real sizeX, const Ogre::Real sizeY)
    {
        mBorderSize.x = sizeX;
        mBorderSize.y = sizeY;
    }
    void TailedWindow::setBorderSize(const Ogre::Real size)
    {
        mBorderSize = size;
    }
    const Ogre::Vector2 TailedWindow::getBorderSize()
    {
        return mBorderSize;
    }
    void TailedWindow::setTexCoords(WindowTexCoord * texcoord)
    {
        mTexCoord0 = texcoord;
    }
    void TailedWindow::setTileMode(bool mode)
    {
        mTileMode = mode;
    }
    bool TailedWindow::getTileMode()
    {
        return mTileMode;
    }
    void TailedWindow::setTiling(const Ogre::Real tileX, const Ogre::Real tileY)
    {
        mTiling.x = tileX;
        mTiling.y = tileY;
    }
    void TailedWindow::setTiling(const Ogre::Vector2 tile)
    {
        mTiling = tile;
    }
    Ogre::Vector2 TailedWindow::getTiling()
    {
        return mTiling;
    }
    void TailedWindow::setBackgroundColor(const Ogre::ColourValue& col)
    {
        mBackgroundColor = col;
    }
    void TailedWindow::setBackgroundColor(const Ogre::Real r,const Ogre::Real g,const Ogre::Real b,const Ogre::Real a)
    {
        mBackgroundColor.r = r;
        mBackgroundColor.g = g;
        mBackgroundColor.b = b;
        mBackgroundColor.a = a;
    }
    const Ogre::ColourValue& TailedWindow::getBackgroundColor()
    {
        return mBackgroundColor;
    }
    void TailedWindow::setBorderColor(const Ogre::ColourValue& col)
    {
        mBorderColor = col;
    }
    void TailedWindow::setBorderColor(const Ogre::Real r,const Ogre::Real g,const Ogre::Real b,const Ogre::Real a)
    {
        mBorderColor.r = r;
        mBorderColor.g = g;
        mBorderColor.b = b;
        mBorderColor.a = a;
    }
    const Ogre::ColourValue& TailedWindow::getBorderColor()
    {
        return mBorderColor;
    }
    void TailedWindow::setWindowTail(bool visible, bool dir, float pos)
    {
        mTailVisible = visible;
        mTailDirection = dir;
        mTailPosition = pos;
    }
    void TailedWindow::setTailVisibility(bool visible)
    {
        mTailVisible = visible;
    }
    bool TailedWindow::isVisible()
    {
        return mTailVisible;
    }
    void TailedWindow::setTailDirection(bool dir)
    {
        mTailDirection = dir;
    }
    bool TailedWindow::getTailDirection()
    {
        return mTailDirection;
    }
    void TailedWindow::setTailPosition(float pos)
    {
        mTailPosition = pos;
    }
    float TailedWindow::getTailPosition()
    {
        return mTailPosition;
    }
    void TailedWindow::setBorderMaterialName(const Ogre::String& name)
    {
        mBorderMaterialName = name;
        mpBorderMaterial = Ogre::MaterialManager::getSingleton().getByName(name);
        mpBorderMaterial->load();
        // Set some prerequisites to be sure
        mpBorderMaterial->setLightingEnabled(false);
        mpBorderMaterial->setDepthCheckEnabled(false);
    }
    const Ogre::String& TailedWindow::getBorderMaterialName(void) const
    {
        return mBorderMaterialName;
    }
    const Ogre::String& TailedWindow::getTypeName(void) const
    {
        return msTypeName;
    }
    void TailedWindow::getRenderOperation(Ogre::RenderOperation &op)
    {
        op = mRenderOp;
    }
    void TailedWindow::setMaterialName(const Ogre::String& matName)
    {
        Ogre::OverlayContainer::setMaterialName(matName);
    }
    void TailedWindow::_updateRenderQueue(Ogre::RenderQueue * queue)
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
    void TailedWindow::visitRenderables(Renderable::Visitor* visitor, bool debugRenderables)
    {
        visitor->visit(mBorderRenderable, 0, false);
        OverlayContainer::visitRenderables(visitor, debugRenderables);
    }
    void TailedWindow::_update(void)
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

    void TailedWindow::updatePositionGeometry(void)
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
    void TailedWindow::updateTextureGeometry(void)
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
        *pTex++ = mTexCoord0->top_left.left;
        *pTex++ = mTexCoord0->top_left.top;
        // Texcoord 1
        *pTex++ = mTexCoord1[0].x;
        *pTex++ = mTexCoord1[0].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->top_left.left;
        *pTex++ = mTexCoord0->top_left.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord1[1].x;
        *pTex++ = mTexCoord1[1].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->top_left.right;
        *pTex++ = mTexCoord0->top_left.top;
        // Texcoord 1
        *pTex++ = mTexCoord1[2].x;
        *pTex++ = mTexCoord1[2].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->top_left.right;
        *pTex++ = mTexCoord0->top_left.top;
        // Texcoord 1
        *pTex++ = mTexCoord1[3].x;
        *pTex++ = mTexCoord1[3].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->top_left.left;
        *pTex++ = mTexCoord0->top_left.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord1[4].x;
        *pTex++ = mTexCoord1[4].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->top_left.right;
        *pTex++ = mTexCoord0->top_left.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord1[5].x;
        *pTex++ = mTexCoord1[5].y;

        // Texcoord 0
        *pTex++ = mTexCoord0->top.left;
        *pTex++ = mTexCoord0->top.top;
        // Texcoord 1
        *pTex++ = mTexCoord1[6].x;
        *pTex++ = mTexCoord1[6].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->top.left;
        *pTex++ = mTexCoord0->top.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord1[7].x;
        *pTex++ = mTexCoord1[7].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->top.right;
        *pTex++ = mTexCoord0->top.top;
        // Texcoord 1
        *pTex++ = mTexCoord1[8].x;
        *pTex++ = mTexCoord1[8].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->top.right;
        *pTex++ = mTexCoord0->top.top;
        // Texcoord 1
        *pTex++ = mTexCoord1[9].x;
        *pTex++ = mTexCoord1[9].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->top.left;
        *pTex++ = mTexCoord0->top.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord1[10].x;
        *pTex++ = mTexCoord1[10].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->top.right;
        *pTex++ = mTexCoord0->top.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord1[11].x;
        *pTex++ = mTexCoord1[11].y;

        // Texcoord 0
        *pTex++ = mTexCoord0->top_right.left;
        *pTex++ = mTexCoord0->top_right.top;
        // Texcoord 1
        *pTex++ = mTexCoord1[12].x;
        *pTex++ = mTexCoord1[12].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->top_right.left;
        *pTex++ = mTexCoord0->top_right.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord1[13].x;
        *pTex++ = mTexCoord1[13].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->top_right.right;
        *pTex++ = mTexCoord0->top_right.top;
        // Texcoord 1
        *pTex++ = mTexCoord1[14].x;
        *pTex++ = mTexCoord1[14].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->top_right.right;
        *pTex++ = mTexCoord0->top_right.top;
        // Texcoord 1
        *pTex++ = mTexCoord1[15].x;
        *pTex++ = mTexCoord1[15].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->top_right.left;
        *pTex++ = mTexCoord0->top_right.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord1[16].x;
        *pTex++ = mTexCoord1[16].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->top_right.right;
        *pTex++ = mTexCoord0->top_right.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord1[17].x;
        *pTex++ = mTexCoord1[17].y;

        // Texcoord 0
        *pTex++ = mTexCoord0->left.left;
        *pTex++ = mTexCoord0->left.top;
        // Texcoord 1
        *pTex++ = mTexCoord1[18].x;
        *pTex++ = mTexCoord1[18].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->left.left;
        *pTex++ = mTexCoord0->left.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord1[19].x;
        *pTex++ = mTexCoord1[19].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->left.right;
        *pTex++ = mTexCoord0->left.top;
        // Texcoord 1
        *pTex++ = mTexCoord1[20].x;
        *pTex++ = mTexCoord1[20].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->left.right;
        *pTex++ = mTexCoord0->left.top;
        // Texcoord 1
        *pTex++ = mTexCoord1[21].x;
        *pTex++ = mTexCoord1[21].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->left.left;
        *pTex++ = mTexCoord0->left.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord1[22].x;
        *pTex++ = mTexCoord1[22].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->left.right;
        *pTex++ = mTexCoord0->left.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord1[23].x;
        *pTex++ = mTexCoord1[23].y;

        // Texcoord 0
        *pTex++ = mTexCoord0->center.left;
        *pTex++ = mTexCoord0->center.top;
        // Texcoord 1
        *pTex++ = mTexCoord1[24].x;
        *pTex++ = mTexCoord1[24].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->center.left;
        *pTex++ = mTexCoord0->center.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord1[25].x;
        *pTex++ = mTexCoord1[25].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->center.right;
        *pTex++ = mTexCoord0->center.top;
        // Texcoord 1
        *pTex++ = mTexCoord1[26].x;
        *pTex++ = mTexCoord1[26].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->center.right;
        *pTex++ = mTexCoord0->center.top;
        // Texcoord 1
        *pTex++ = mTexCoord1[27].x;
        *pTex++ = mTexCoord1[27].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->center.left;
        *pTex++ = mTexCoord0->center.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord1[28].x;
        *pTex++ = mTexCoord1[28].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->center.right;
        *pTex++ = mTexCoord0->center.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord1[29].x;
        *pTex++ = mTexCoord1[29].y;

        // Texcoord 0
        *pTex++ = mTexCoord0->right.left;
        *pTex++ = mTexCoord0->right.top;
        // Texcoord 1
        *pTex++ = mTexCoord1[30].x;
        *pTex++ = mTexCoord1[30].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->right.left;
        *pTex++ = mTexCoord0->right.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord1[31].x;
        *pTex++ = mTexCoord1[31].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->right.right;
        *pTex++ = mTexCoord0->right.top;
        // Texcoord 1
        *pTex++ = mTexCoord1[32].x;
        *pTex++ = mTexCoord1[32].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->right.right;
        *pTex++ = mTexCoord0->right.top;
        // Texcoord 1
        *pTex++ = mTexCoord1[33].x;
        *pTex++ = mTexCoord1[33].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->right.left;
        *pTex++ = mTexCoord0->right.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord1[34].x;
        *pTex++ = mTexCoord1[34].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->right.right;
        *pTex++ = mTexCoord0->right.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord1[35].x;
        *pTex++ = mTexCoord1[35].y;

        // Texcoord 0
        *pTex++ = mTexCoord0->bottom_left.left;
        *pTex++ = mTexCoord0->bottom_left.top;
        // Texcoord 1
        *pTex++ = mTexCoord1[36].x;
        *pTex++ = mTexCoord1[36].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->bottom_left.left;
        *pTex++ = mTexCoord0->bottom_left.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord1[37].x;
        *pTex++ = mTexCoord1[37].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->bottom_left.right;
        *pTex++ = mTexCoord0->bottom_left.top;
        // Texcoord 1
        *pTex++ = mTexCoord1[38].x;
        *pTex++ = mTexCoord1[38].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->bottom_left.right;
        *pTex++ = mTexCoord0->bottom_left.top;
        // Texcoord 1
        *pTex++ = mTexCoord1[39].x;
        *pTex++ = mTexCoord1[39].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->bottom_left.left;
        *pTex++ = mTexCoord0->bottom_left.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord1[40].x;
        *pTex++ = mTexCoord1[40].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->bottom_left.right;
        *pTex++ = mTexCoord0->bottom_left.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord1[41].x;
        *pTex++ = mTexCoord1[41].y;

        // Texcoord 0
        *pTex++ = mTexCoord0->bottom.left;
        *pTex++ = mTexCoord0->bottom.top;
        // Texcoord 1
        *pTex++ = mTexCoord1[42].x;
        *pTex++ = mTexCoord1[42].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->bottom.left;
        *pTex++ = mTexCoord0->bottom.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord1[43].x;
        *pTex++ = mTexCoord1[43].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->bottom.right;
        *pTex++ = mTexCoord0->bottom.top;
        // Texcoord 1
        *pTex++ = mTexCoord1[44].x;
        *pTex++ = mTexCoord1[44].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->bottom.right;
        *pTex++ = mTexCoord0->bottom.top;
        // Texcoord 1
        *pTex++ = mTexCoord1[45].x;
        *pTex++ = mTexCoord1[45].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->bottom.left;
        *pTex++ = mTexCoord0->bottom.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord1[46].x;
        *pTex++ = mTexCoord1[46].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->bottom.right;
        *pTex++ = mTexCoord0->bottom.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord1[47].x;
        *pTex++ = mTexCoord1[47].y;

        // Texcoord 0
        *pTex++ = mTexCoord0->bottom_right.left;
        *pTex++ = mTexCoord0->bottom_right.top;
        // Texcoord 1
        *pTex++ = mTexCoord1[48].x;
        *pTex++ = mTexCoord1[48].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->bottom_right.left;
        *pTex++ = mTexCoord0->bottom_right.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord1[49].x;
        *pTex++ = mTexCoord1[49].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->bottom_right.right;
        *pTex++ = mTexCoord0->bottom_right.top;
        // Texcoord 1
        *pTex++ = mTexCoord1[50].x;
        *pTex++ = mTexCoord1[50].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->bottom_right.right;
        *pTex++ = mTexCoord0->bottom_right.top;
        // Texcoord 1
        *pTex++ = mTexCoord1[51].x;
        *pTex++ = mTexCoord1[51].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->bottom_right.left;
        *pTex++ = mTexCoord0->bottom_right.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord1[52].x;
        *pTex++ = mTexCoord1[52].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->bottom_right.right;
        *pTex++ = mTexCoord0->bottom_right.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord1[53].x;
        *pTex++ = mTexCoord1[53].y;
        if(!mTailDirection)
        {
        // Texcoord 0
        *pTex++ = mTexCoord0->tail_down.left;
        *pTex++ = mTexCoord0->tail_down.top;
        // Texcoord 1
        *pTex++ = mTexCoord1[54].x;
        *pTex++ = mTexCoord1[54].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->tail_down.left;
        *pTex++ = mTexCoord0->tail_down.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord1[55].x;
        *pTex++ = mTexCoord1[55].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->tail_down.right;
        *pTex++ = mTexCoord0->tail_down.top;
        // Texcoord 1
        *pTex++ = mTexCoord1[56].x;
        *pTex++ = mTexCoord1[56].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->tail_down.right;
        *pTex++ = mTexCoord0->tail_down.top;
        // Texcoord 1
        *pTex++ = mTexCoord1[57].x;
        *pTex++ = mTexCoord1[57].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->tail_down.left;
        *pTex++ = mTexCoord0->tail_down.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord1[58].x;
        *pTex++ = mTexCoord1[58].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->tail_down.right;
        *pTex++ = mTexCoord0->tail_down.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord1[59].x;
        *pTex++ = mTexCoord1[59].y;

        // Texcoord 0
        *pTex++ = mTexCoord0->bottom.left;
        *pTex++ = mTexCoord0->bottom.top;
        // Texcoord 1
        *pTex++ = mTexCoord1[60].x;
        *pTex++ = mTexCoord1[60].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->bottom.left;
        *pTex++ = mTexCoord0->bottom.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord1[61].x;
        *pTex++ = mTexCoord1[61].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->bottom.right;
        *pTex++ = mTexCoord0->bottom.top;
        // Texcoord 1
        *pTex++ = mTexCoord1[62].x;
        *pTex++ = mTexCoord1[62].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->bottom.right;
        *pTex++ = mTexCoord0->bottom.top;
        // Texcoord 1
        *pTex++ = mTexCoord1[63].x;
        *pTex++ = mTexCoord1[63].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->bottom.left;
        *pTex++ = mTexCoord0->bottom.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord1[64].x;
        *pTex++ = mTexCoord1[64].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->bottom.right;
        *pTex++ = mTexCoord0->bottom.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord1[65].x;
        *pTex++ = mTexCoord1[65].y;
        } else {
        // Texcoord 0
        *pTex++ = mTexCoord0->tail_up.left;
        *pTex++ = mTexCoord0->tail_up.top;
        // Texcoord 1
        *pTex++ = mTexCoord1[54].x;
        *pTex++ = mTexCoord1[54].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->tail_up.left;
        *pTex++ = mTexCoord0->tail_up.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord1[55].x;
        *pTex++ = mTexCoord1[55].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->tail_up.right;
        *pTex++ = mTexCoord0->tail_up.top;
        // Texcoord 1
        *pTex++ = mTexCoord1[56].x;
        *pTex++ = mTexCoord1[56].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->tail_up.right;
        *pTex++ = mTexCoord0->tail_up.top;
        // Texcoord 1
        *pTex++ = mTexCoord1[57].x;
        *pTex++ = mTexCoord1[57].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->tail_up.left;
        *pTex++ = mTexCoord0->tail_up.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord1[58].x;
        *pTex++ = mTexCoord1[58].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->tail_up.right;
        *pTex++ = mTexCoord0->tail_up.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord1[59].x;
        *pTex++ = mTexCoord1[59].y;

        // Texcoord 0
        *pTex++ = mTexCoord0->top.left;
        *pTex++ = mTexCoord0->top.top;
        // Texcoord 1
        *pTex++ = mTexCoord1[60].x;
        *pTex++ = mTexCoord1[60].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->top.left;
        *pTex++ = mTexCoord0->top.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord1[61].x;
        *pTex++ = mTexCoord1[61].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->top.right;
        *pTex++ = mTexCoord0->top.top;
        // Texcoord 1
        *pTex++ = mTexCoord1[62].x;
        *pTex++ = mTexCoord1[62].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->top.right;
        *pTex++ = mTexCoord0->top.top;
        // Texcoord 1
        *pTex++ = mTexCoord1[63].x;
        *pTex++ = mTexCoord1[63].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->top.left;
        *pTex++ = mTexCoord0->top.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord1[64].x;
        *pTex++ = mTexCoord1[64].y;
        // Texcoord 0
        *pTex++ = mTexCoord0->top.right;
        *pTex++ = mTexCoord0->top.bottom;
        // Texcoord 1
        *pTex++ = mTexCoord1[65].x;
        *pTex++ = mTexCoord1[65].y;
        }

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
        *pTex++ = mTexCoord0->top_left.left;
        *pTex++ = mTexCoord0->top_left.top;
        // Texcoord 0
        *pTex++ = mTexCoord0->top_left.left;
        *pTex++ = mTexCoord0->top_left.bottom;
        // Texcoord 0
        *pTex++ = mTexCoord0->top_left.right;
        *pTex++ = mTexCoord0->top_left.top;
        // Texcoord 0
        *pTex++ = mTexCoord0->top_left.right;
        *pTex++ = mTexCoord0->top_left.top;
        // Texcoord 0
        *pTex++ = mTexCoord0->top_left.left;
        *pTex++ = mTexCoord0->top_left.bottom;
        // Texcoord 0
        *pTex++ = mTexCoord0->top_left.right;
        *pTex++ = mTexCoord0->top_left.bottom;

        // Texcoord 0
        *pTex++ = mTexCoord0->top.left;
        *pTex++ = mTexCoord0->top.top;
        // Texcoord 0
        *pTex++ = mTexCoord0->top.left;
        *pTex++ = mTexCoord0->top.bottom;
        // Texcoord 0
        *pTex++ = mTexCoord0->top.right;
        *pTex++ = mTexCoord0->top.top;
        // Texcoord 0
        *pTex++ = mTexCoord0->top.right;
        *pTex++ = mTexCoord0->top.top;
        // Texcoord 0
        *pTex++ = mTexCoord0->top.left;
        *pTex++ = mTexCoord0->top.bottom;
        // Texcoord 0
        *pTex++ = mTexCoord0->top.right;
        *pTex++ = mTexCoord0->top.bottom;

        // Texcoord 0
        *pTex++ = mTexCoord0->top_right.left;
        *pTex++ = mTexCoord0->top_right.top;
        // Texcoord 0
        *pTex++ = mTexCoord0->top_right.left;
        *pTex++ = mTexCoord0->top_right.bottom;
        // Texcoord 0
        *pTex++ = mTexCoord0->top_right.right;
        *pTex++ = mTexCoord0->top_right.top;
        // Texcoord 0
        *pTex++ = mTexCoord0->top_right.right;
        *pTex++ = mTexCoord0->top_right.top;
        // Texcoord 0
        *pTex++ = mTexCoord0->top_right.left;
        *pTex++ = mTexCoord0->top_right.bottom;
        // Texcoord 0
        *pTex++ = mTexCoord0->top_right.right;
        *pTex++ = mTexCoord0->top_right.bottom;

        // Texcoord 0
        *pTex++ = mTexCoord0->left.left;
        *pTex++ = mTexCoord0->left.top;
        // Texcoord 0
        *pTex++ = mTexCoord0->left.left;
        *pTex++ = mTexCoord0->left.bottom;
        // Texcoord 0
        *pTex++ = mTexCoord0->left.right;
        *pTex++ = mTexCoord0->left.top;
        // Texcoord 0
        *pTex++ = mTexCoord0->left.right;
        *pTex++ = mTexCoord0->left.top;
        // Texcoord 0
        *pTex++ = mTexCoord0->left.left;
        *pTex++ = mTexCoord0->left.bottom;
        // Texcoord 0
        *pTex++ = mTexCoord0->left.right;
        *pTex++ = mTexCoord0->left.bottom;

        // Texcoord 0
        *pTex++ = mTexCoord0->center.left;
        *pTex++ = mTexCoord0->center.top;
        // Texcoord 0
        *pTex++ = mTexCoord0->center.left;
        *pTex++ = mTexCoord0->center.bottom;
        // Texcoord 0
        *pTex++ = mTexCoord0->center.right;
        *pTex++ = mTexCoord0->center.top;
        // Texcoord 0
        *pTex++ = mTexCoord0->center.right;
        *pTex++ = mTexCoord0->center.top;
        // Texcoord 0
        *pTex++ = mTexCoord0->center.left;
        *pTex++ = mTexCoord0->center.bottom;
        // Texcoord 0
        *pTex++ = mTexCoord0->center.right;
        *pTex++ = mTexCoord0->center.bottom;

        // Texcoord 0
        *pTex++ = mTexCoord0->right.left;
        *pTex++ = mTexCoord0->right.top;
        // Texcoord 0
        *pTex++ = mTexCoord0->right.left;
        *pTex++ = mTexCoord0->right.bottom;
        // Texcoord 0
        *pTex++ = mTexCoord0->right.right;
        *pTex++ = mTexCoord0->right.top;
        // Texcoord 0
        *pTex++ = mTexCoord0->right.right;
        *pTex++ = mTexCoord0->right.top;
        // Texcoord 0
        *pTex++ = mTexCoord0->right.left;
        *pTex++ = mTexCoord0->right.bottom;
        // Texcoord 0
        *pTex++ = mTexCoord0->right.right;
        *pTex++ = mTexCoord0->right.bottom;

        // Texcoord 0
        *pTex++ = mTexCoord0->bottom_left.left;
        *pTex++ = mTexCoord0->bottom_left.top;
        // Texcoord 0
        *pTex++ = mTexCoord0->bottom_left.left;
        *pTex++ = mTexCoord0->bottom_left.bottom;
        // Texcoord 0
        *pTex++ = mTexCoord0->bottom_left.right;
        *pTex++ = mTexCoord0->bottom_left.top;
        // Texcoord 0
        *pTex++ = mTexCoord0->bottom_left.right;
        *pTex++ = mTexCoord0->bottom_left.top;
        // Texcoord 0
        *pTex++ = mTexCoord0->bottom_left.left;
        *pTex++ = mTexCoord0->bottom_left.bottom;
        // Texcoord 0
        *pTex++ = mTexCoord0->bottom_left.right;
        *pTex++ = mTexCoord0->bottom_left.bottom;

        // Texcoord 0
        *pTex++ = mTexCoord0->bottom.left;
        *pTex++ = mTexCoord0->bottom.top;
        // Texcoord 0
        *pTex++ = mTexCoord0->bottom.left;
        *pTex++ = mTexCoord0->bottom.bottom;
        // Texcoord 0
        *pTex++ = mTexCoord0->bottom.right;
        *pTex++ = mTexCoord0->bottom.top;
        // Texcoord 0
        *pTex++ = mTexCoord0->bottom.right;
        *pTex++ = mTexCoord0->bottom.top;
        // Texcoord 0
        *pTex++ = mTexCoord0->bottom.left;
        *pTex++ = mTexCoord0->bottom.bottom;
        // Texcoord 0
        *pTex++ = mTexCoord0->bottom.right;
        *pTex++ = mTexCoord0->bottom.bottom;

        // Texcoord 0
        *pTex++ = mTexCoord0->bottom_right.left;
        *pTex++ = mTexCoord0->bottom_right.top;
        // Texcoord 0
        *pTex++ = mTexCoord0->bottom_right.left;
        *pTex++ = mTexCoord0->bottom_right.bottom;
        // Texcoord 0
        *pTex++ = mTexCoord0->bottom_right.right;
        *pTex++ = mTexCoord0->bottom_right.top;
        // Texcoord 0
        *pTex++ = mTexCoord0->bottom_right.right;
        *pTex++ = mTexCoord0->bottom_right.top;
        // Texcoord 0
        *pTex++ = mTexCoord0->bottom_right.left;
        *pTex++ = mTexCoord0->bottom_right.bottom;
        // Texcoord 0
        *pTex++ = mTexCoord0->bottom_right.right;
        *pTex++ = mTexCoord0->bottom_right.bottom;
        if(!mTailDirection)
        {
        // Texcoord 0
        *pTex++ = mTexCoord0->tail_down.left;
        *pTex++ = mTexCoord0->tail_down.top;
        // Texcoord 0
        *pTex++ = mTexCoord0->tail_down.left;
        *pTex++ = mTexCoord0->tail_down.bottom;
        // Texcoord 0
        *pTex++ = mTexCoord0->tail_down.right;
        *pTex++ = mTexCoord0->tail_down.top;
        // Texcoord 0
        *pTex++ = mTexCoord0->tail_down.right;
        *pTex++ = mTexCoord0->tail_down.top;
        // Texcoord 0
        *pTex++ = mTexCoord0->tail_down.left;
        *pTex++ = mTexCoord0->tail_down.bottom;
        // Texcoord 0
        *pTex++ = mTexCoord0->tail_down.right;
        *pTex++ = mTexCoord0->tail_down.bottom;

        // Texcoord 0
        *pTex++ = mTexCoord0->bottom.left;
        *pTex++ = mTexCoord0->bottom.top;
        // Texcoord 0
        *pTex++ = mTexCoord0->bottom.left;
        *pTex++ = mTexCoord0->bottom.bottom;
        // Texcoord 0
        *pTex++ = mTexCoord0->bottom.right;
        *pTex++ = mTexCoord0->bottom.top;
        // Texcoord 0
        *pTex++ = mTexCoord0->bottom.right;
        *pTex++ = mTexCoord0->bottom.top;
        // Texcoord 0
        *pTex++ = mTexCoord0->bottom.left;
        *pTex++ = mTexCoord0->bottom.bottom;
        // Texcoord 0
        *pTex++ = mTexCoord0->bottom.right;
        *pTex++ = mTexCoord0->bottom.bottom;
        } else {
        // Texcoord 0
        *pTex++ = mTexCoord0->tail_up.left;
        *pTex++ = mTexCoord0->tail_up.top;
        // Texcoord 0
        *pTex++ = mTexCoord0->tail_up.left;
        *pTex++ = mTexCoord0->tail_up.bottom;
        // Texcoord 0
        *pTex++ = mTexCoord0->tail_up.right;
        *pTex++ = mTexCoord0->tail_up.top;
        // Texcoord 0
        *pTex++ = mTexCoord0->tail_up.right;
        *pTex++ = mTexCoord0->tail_up.top;
        // Texcoord 0
        *pTex++ = mTexCoord0->tail_up.left;
        *pTex++ = mTexCoord0->tail_up.bottom;
        // Texcoord 0
        *pTex++ = mTexCoord0->tail_up.right;
        *pTex++ = mTexCoord0->tail_up.bottom;

        // Texcoord 0
        *pTex++ = mTexCoord0->top.left;
        *pTex++ = mTexCoord0->top.top;
        // Texcoord 0
        *pTex++ = mTexCoord0->top.left;
        *pTex++ = mTexCoord0->top.bottom;
        // Texcoord 0
        *pTex++ = mTexCoord0->top.right;
        *pTex++ = mTexCoord0->top.top;
        // Texcoord 0
        *pTex++ = mTexCoord0->top.right;
        *pTex++ = mTexCoord0->top.top;
        // Texcoord 0
        *pTex++ = mTexCoord0->top.left;
        *pTex++ = mTexCoord0->top.bottom;
        // Texcoord 0
        *pTex++ = mTexCoord0->top.right;
        *pTex++ = mTexCoord0->top.bottom;
        }
        vbuf->unlock();
    }

    void TailedWindow::updateBorderColors(void)
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

    void TailedWindow::updateBackgroundColors(void)
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

    void TailedWindow::calculatePosition(void)
    {
        float left, right, top, bottom, tail;

        left = _getDerivedLeft();
        right = left + mWidth;
        top = _getDerivedTop();
        bottom =  top + mHeight;

        if(mTailVisible)
        {
            Ogre::Real start = left+mBorderSize.x;
            Ogre::Real end = right - (mBorderSize.x * 2);
            tail = lerp(start,end,mTailPosition);
        }

        mPosCoord[0].x = left;
        mPosCoord[0].y = top;
        mPosCoord[1].x = left;
        mPosCoord[1].y = top+mBorderSize.y;
        mPosCoord[2].x = left+mBorderSize.x;
        mPosCoord[2].y = top;
        mPosCoord[3].x = left+mBorderSize.x;
        mPosCoord[3].y = top;
        mPosCoord[4].x = left;
        mPosCoord[4].y = top+mBorderSize.y;
        mPosCoord[5].x = left+mBorderSize.x;
        mPosCoord[5].y = top+mBorderSize.y;
        if(mTailVisible)
        {
            if(!mTailDirection)
            {
                mPosCoord[6].x = left+mBorderSize.x;
                mPosCoord[6].y = top;
                mPosCoord[7].x = left+mBorderSize.x;
                mPosCoord[7].y = top+mBorderSize.y;
                mPosCoord[8].x = right-mBorderSize.x;
                mPosCoord[8].y = top;
                mPosCoord[9].x = right-mBorderSize.x;
                mPosCoord[9].y = top;
                mPosCoord[10].x = left+mBorderSize.x;
                mPosCoord[10].y = top+mBorderSize.y;
                mPosCoord[11].x = right-mBorderSize.x;
                mPosCoord[11].y = top+mBorderSize.y;
            } else {
                mPosCoord[6].x = left+mBorderSize.x;
                mPosCoord[6].y = top;
                mPosCoord[7].x = left+mBorderSize.x;
                mPosCoord[7].y = top+mBorderSize.y;
                mPosCoord[8].x = tail;
                mPosCoord[8].y = top;
                mPosCoord[9].x = tail;
                mPosCoord[9].y = top;
                mPosCoord[10].x = left+mBorderSize.x;
                mPosCoord[10].y = top+mBorderSize.y;
                mPosCoord[11].x = tail;
                mPosCoord[11].y = top+mBorderSize.y;
            }
        } else {
                mPosCoord[6].x = left+mBorderSize.x;
                mPosCoord[6].y = top;
                mPosCoord[7].x = left+mBorderSize.x;
                mPosCoord[7].y = top+mBorderSize.y;
                mPosCoord[8].x = right-mBorderSize.x;
                mPosCoord[8].y = top;
                mPosCoord[9].x = right-mBorderSize.x;
                mPosCoord[9].y = top;
                mPosCoord[10].x = left+mBorderSize.x;
                mPosCoord[10].y = top+mBorderSize.y;
                mPosCoord[11].x = right-mBorderSize.x;
                mPosCoord[11].y = top+mBorderSize.y;
        }

        mPosCoord[12].x = right-mBorderSize.x;
        mPosCoord[12].y = top;
        mPosCoord[13].x = right-mBorderSize.x;
        mPosCoord[13].y = top+mBorderSize.y;
        mPosCoord[14].x = right;
        mPosCoord[14].y = top;
        mPosCoord[15].x = right;
        mPosCoord[15].y = top;
        mPosCoord[16].x = right-mBorderSize.x;
        mPosCoord[16].y = top+mBorderSize.y;
        mPosCoord[17].x = right;
        mPosCoord[17].y = top+mBorderSize.y;

        mPosCoord[18].x = left;
        mPosCoord[18].y = top+mBorderSize.y;
        mPosCoord[19].x = left;
        mPosCoord[19].y = bottom-mBorderSize.y;
        mPosCoord[20].x = left+mBorderSize.x;
        mPosCoord[20].y = top+mBorderSize.y;
        mPosCoord[21].x = left+mBorderSize.x;
        mPosCoord[21].y = top+mBorderSize.y;
        mPosCoord[22].x = left;
        mPosCoord[22].y = bottom-mBorderSize.y;
        mPosCoord[23].x = left+mBorderSize.x;
        mPosCoord[23].y = bottom-mBorderSize.y;

        mPosCoord[24].x = left+mBorderSize.x;
        mPosCoord[24].y = top+mBorderSize.y;
        mPosCoord[25].x = left+mBorderSize.x;
        mPosCoord[25].y = bottom-mBorderSize.y;
        mPosCoord[26].x = right-mBorderSize.x;
        mPosCoord[26].y = top+mBorderSize.y;
        mPosCoord[27].x = right-mBorderSize.x;
        mPosCoord[27].y = top+mBorderSize.y;
        mPosCoord[28].x = left+mBorderSize.x;
        mPosCoord[28].y = bottom-mBorderSize.y;
        mPosCoord[29].x = right-mBorderSize.x;
        mPosCoord[29].y = bottom-mBorderSize.y;

        mPosCoord[30].x = right-mBorderSize.x;
        mPosCoord[30].y = top+mBorderSize.y;
        mPosCoord[31].x = right-mBorderSize.x;
        mPosCoord[31].y = bottom-mBorderSize.y;
        mPosCoord[32].x = right;
        mPosCoord[32].y = top+mBorderSize.y;
        mPosCoord[33].x = right;
        mPosCoord[33].y = top+mBorderSize.y;
        mPosCoord[34].x = right-mBorderSize.x;
        mPosCoord[34].y = bottom-mBorderSize.y;
        mPosCoord[35].x = right;
        mPosCoord[35].y = bottom-mBorderSize.y;

        mPosCoord[36].x = left;
        mPosCoord[36].y = bottom-mBorderSize.y;
        mPosCoord[37].x = left;
        mPosCoord[37].y = bottom;
        mPosCoord[38].x = left+mBorderSize.x;
        mPosCoord[38].y = bottom-mBorderSize.y;
        mPosCoord[39].x = left+mBorderSize.x;
        mPosCoord[39].y = bottom-mBorderSize.y;
        mPosCoord[40].x = left;
        mPosCoord[40].y = bottom;
        mPosCoord[41].x = left+mBorderSize.x;
        mPosCoord[41].y = bottom;
        if(mTailVisible)
        {
            if(mTailDirection)
            {
                mPosCoord[42].x = left+mBorderSize.x;
                mPosCoord[42].y = bottom-mBorderSize.y;
                mPosCoord[43].x = left+mBorderSize.x;
                mPosCoord[43].y = bottom;
                mPosCoord[44].x = right-mBorderSize.x;
                mPosCoord[44].y = bottom-mBorderSize.y;
                mPosCoord[45].x = right-mBorderSize.x;
                mPosCoord[45].y = bottom-mBorderSize.y;
                mPosCoord[46].x = left+mBorderSize.x;
                mPosCoord[46].y = bottom;
                mPosCoord[47].x = right-mBorderSize.x;
                mPosCoord[47].y = bottom;
            } else {
                mPosCoord[42].x = left+mBorderSize.x;
                mPosCoord[42].y = bottom-mBorderSize.y;
                mPosCoord[43].x = left+mBorderSize.x;
                mPosCoord[43].y = bottom;
                mPosCoord[44].x = tail;
                mPosCoord[44].y = bottom-mBorderSize.y;
                mPosCoord[45].x = tail;
                mPosCoord[45].y = bottom-mBorderSize.y;
                mPosCoord[46].x = left+mBorderSize.x;
                mPosCoord[46].y = bottom;
                mPosCoord[47].x = tail;
                mPosCoord[47].y = bottom;
            }
        } else {
            mPosCoord[42].x = left+mBorderSize.x;
            mPosCoord[42].y = bottom-mBorderSize.y;
            mPosCoord[43].x = left+mBorderSize.x;
            mPosCoord[43].y = bottom;
            mPosCoord[44].x = right-mBorderSize.x;
            mPosCoord[44].y = bottom-mBorderSize.y;
            mPosCoord[45].x = right-mBorderSize.x;
            mPosCoord[45].y = bottom-mBorderSize.y;
            mPosCoord[46].x = left+mBorderSize.x;
            mPosCoord[46].y = bottom;
            mPosCoord[47].x = right-mBorderSize.x;
            mPosCoord[47].y = bottom;
        }

        mPosCoord[48].x = right-mBorderSize.x;
        mPosCoord[48].y = bottom-mBorderSize.y;
        mPosCoord[49].x = right-mBorderSize.x;
        mPosCoord[49].y = bottom;
        mPosCoord[50].x = right;
        mPosCoord[50].y = bottom-mBorderSize.y;
        mPosCoord[51].x = right;
        mPosCoord[51].y = bottom-mBorderSize.y;
        mPosCoord[52].x = right-mBorderSize.x;
        mPosCoord[52].y = bottom;
        mPosCoord[53].x = right;
        mPosCoord[53].y = bottom;

        if(mTailVisible)
        {
            if(!mTailDirection)
            {
                mPosCoord[54].x = tail;
                mPosCoord[54].y = bottom-mBorderSize.y;
                mPosCoord[55].x = tail;
                mPosCoord[55].y = bottom+mBorderSize.y;
                mPosCoord[56].x = tail+mBorderSize.x;
                mPosCoord[56].y = bottom-mBorderSize.y;
                mPosCoord[57].x = tail+mBorderSize.x;
                mPosCoord[57].y = bottom-mBorderSize.y;
                mPosCoord[58].x = tail;
                mPosCoord[58].y = bottom+mBorderSize.y;
                mPosCoord[59].x = tail+mBorderSize.x;
                mPosCoord[59].y = bottom+mBorderSize.y;

                mPosCoord[60].x = tail+mBorderSize.x;
                mPosCoord[60].y = bottom-mBorderSize.y;
                mPosCoord[61].x = tail+mBorderSize.x;
                mPosCoord[61].y = bottom;
                mPosCoord[62].x = right-mBorderSize.x;
                mPosCoord[62].y = bottom-mBorderSize.y;
                mPosCoord[63].x = right-mBorderSize.x;
                mPosCoord[63].y = bottom-mBorderSize.y;
                mPosCoord[64].x = tail+mBorderSize.x;
                mPosCoord[64].y = bottom;
                mPosCoord[65].x = right-mBorderSize.x;
                mPosCoord[65].y = bottom;
            } else {
                mPosCoord[54].x = tail;
                mPosCoord[54].y = top-mBorderSize.y;
                mPosCoord[55].x = tail;
                mPosCoord[55].y = top+mBorderSize.y;
                mPosCoord[56].x = tail+mBorderSize.x;
                mPosCoord[56].y = top-mBorderSize.y;
                mPosCoord[57].x = tail+mBorderSize.x;
                mPosCoord[57].y = top-mBorderSize.y;
                mPosCoord[58].x = tail;
                mPosCoord[58].y = top+mBorderSize.y;
                mPosCoord[59].x = tail+mBorderSize.x;
                mPosCoord[59].y = top+mBorderSize.y;

                mPosCoord[60].x = tail+mBorderSize.x;
                mPosCoord[60].y = top;
                mPosCoord[61].x = tail+mBorderSize.x;
                mPosCoord[61].y = top+mBorderSize.y;
                mPosCoord[62].x = right-mBorderSize.x;
                mPosCoord[62].y = top;
                mPosCoord[63].x = right-mBorderSize.x;
                mPosCoord[63].y = top;
                mPosCoord[64].x = tail+mBorderSize.x;
                mPosCoord[64].y = top+mBorderSize.y;
                mPosCoord[65].x = right-mBorderSize.x;
                mPosCoord[65].y = top+mBorderSize.y;
            }
        } else {
            mPosCoord[54].x = 0.0f;
            mPosCoord[54].y = 0.0f;
            mPosCoord[55].x = 0.0f;
            mPosCoord[55].y = 0.0f;
            mPosCoord[56].x = 0.0f;
            mPosCoord[56].y = 0.0f;
            mPosCoord[57].x = 0.0f;
            mPosCoord[57].y = 0.0f;
            mPosCoord[58].x = 0.0f;
            mPosCoord[58].y = 0.0f;
            mPosCoord[59].x = 0.0f;
            mPosCoord[59].y = 0.0f;

            mPosCoord[60].x = 0.0f;
            mPosCoord[60].y = 0.0f;
            mPosCoord[61].x = 0.0f;
            mPosCoord[61].y = 0.0f;
            mPosCoord[62].x = 0.0f;
            mPosCoord[62].y = 0.0f;
            mPosCoord[63].x = 0.0f;
            mPosCoord[63].y = 0.0f;
            mPosCoord[64].x = 0.0f;
            mPosCoord[64].y = 0.0f;
            mPosCoord[65].x = 0.0f;
            mPosCoord[65].y = 0.0f;
        }



        mGeomPositionsOutOfDate = true;
    }

    void TailedWindow::calculateTexCoord(void)
    {
        // TEMPORARY: Set up a test vertex mesh!
        /*
        0---2   3
        |  /   /|
        | /<->/ |
        |/   /  |
        1   4---5
        */

        float left, right, top, bottom, tail;

        Ogre::Vector2 mBorderFixed;



        if (!mTileMode) // If the texture mode is on stretch mode
        {
            left = 0.0f;
            right = mTiling.x;
            top = 0.0f;
            bottom =  mTiling.y;
            mBorderFixed.x = (mBorderSize.x / mWidth)*mTiling.x;
            mBorderFixed.y = (mBorderSize.y / mHeight)*mTiling.y;
        }
        else // Else if the texture mode is on tile mode
        {
            left = _getDerivedLeft() * mTiling.x;
            right = left + (mWidth * mTiling.x);
            top = _getDerivedTop() * mTiling.y;
            bottom =  top + (mHeight * mTiling.y);
            mBorderFixed.x = mBorderSize.x*mTiling.x;
            mBorderFixed.y = mBorderSize.y*mTiling.y;
        }

        if(mTailVisible)
        {
            Ogre::Real start = left+mBorderFixed.x;
            Ogre::Real end = right - (mBorderFixed.x * 2);
            tail = lerp(start,end,mTailPosition);
        }

        mTexCoord1[0].x = left;
        mTexCoord1[0].y = top;
        mTexCoord1[1].x = left;
        mTexCoord1[1].y = top+mBorderFixed.y;
        mTexCoord1[2].x = left+mBorderFixed.x;
        mTexCoord1[2].y = top;
        mTexCoord1[3].x = left+mBorderFixed.x;
        mTexCoord1[3].y = top;
        mTexCoord1[4].x = left;
        mTexCoord1[4].y = top+mBorderFixed.y;
        mTexCoord1[5].x = left+mBorderFixed.x;
        mTexCoord1[5].y = top+mBorderFixed.y;
        if(mTailVisible)
        {
            if(!mTailDirection)
            {
                mTexCoord1[6].x = left+mBorderFixed.x;
                mTexCoord1[6].y = top;
                mTexCoord1[7].x = left+mBorderFixed.x;
                mTexCoord1[7].y = top+mBorderFixed.y;
                mTexCoord1[8].x = right-mBorderFixed.x;
                mTexCoord1[8].y = top;
                mTexCoord1[9].x = right-mBorderFixed.x;
                mTexCoord1[9].y = top;
                mTexCoord1[10].x = left+mBorderFixed.x;
                mTexCoord1[10].y = top+mBorderFixed.y;
                mTexCoord1[11].x = right-mBorderFixed.x;
                mTexCoord1[11].y = top+mBorderFixed.y;
            } else {
                mTexCoord1[6].x = left+mBorderFixed.x;
                mTexCoord1[6].y = top;
                mTexCoord1[7].x = left+mBorderFixed.x;
                mTexCoord1[7].y = top+mBorderFixed.y;
                mTexCoord1[8].x = tail;
                mTexCoord1[8].y = top;
                mTexCoord1[9].x = tail;
                mTexCoord1[9].y = top;
                mTexCoord1[10].x = left+mBorderFixed.x;
                mTexCoord1[10].y = top+mBorderFixed.y;
                mTexCoord1[11].x = tail;
                mTexCoord1[11].y = top+mBorderFixed.y;
            }
        } else {
                mTexCoord1[6].x = left+mBorderFixed.x;
                mTexCoord1[6].y = top;
                mTexCoord1[7].x = left+mBorderFixed.x;
                mTexCoord1[7].y = top+mBorderFixed.y;
                mTexCoord1[8].x = right-mBorderFixed.x;
                mTexCoord1[8].y = top;
                mTexCoord1[9].x = right-mBorderFixed.x;
                mTexCoord1[9].y = top;
                mTexCoord1[10].x = left+mBorderFixed.x;
                mTexCoord1[10].y = top+mBorderFixed.y;
                mTexCoord1[11].x = right-mBorderFixed.x;
                mTexCoord1[11].y = top+mBorderFixed.y;
        }

        mTexCoord1[12].x = right-mBorderFixed.x;
        mTexCoord1[12].y = top;
        mTexCoord1[13].x = right-mBorderFixed.x;
        mTexCoord1[13].y = top+mBorderFixed.y;
        mTexCoord1[14].x = right;
        mTexCoord1[14].y = top;
        mTexCoord1[15].x = right;
        mTexCoord1[15].y = top;
        mTexCoord1[16].x = right-mBorderFixed.x;
        mTexCoord1[16].y = top+mBorderFixed.y;
        mTexCoord1[17].x = right;
        mTexCoord1[17].y = top+mBorderFixed.y;

        mTexCoord1[18].x = left;
        mTexCoord1[18].y = top+mBorderFixed.y;
        mTexCoord1[19].x = left;
        mTexCoord1[19].y = bottom-mBorderFixed.y;
        mTexCoord1[20].x = left+mBorderFixed.x;
        mTexCoord1[20].y = top+mBorderFixed.y;
        mTexCoord1[21].x = left+mBorderFixed.x;
        mTexCoord1[21].y = top+mBorderFixed.y;
        mTexCoord1[22].x = left;
        mTexCoord1[22].y = bottom-mBorderFixed.y;
        mTexCoord1[23].x = left+mBorderFixed.x;
        mTexCoord1[23].y = bottom-mBorderFixed.y;

        mTexCoord1[24].x = left+mBorderFixed.x;
        mTexCoord1[24].y = top+mBorderFixed.y;
        mTexCoord1[25].x = left+mBorderFixed.x;
        mTexCoord1[25].y = bottom-mBorderFixed.y;
        mTexCoord1[26].x = right-mBorderFixed.x;
        mTexCoord1[26].y = top+mBorderFixed.y;
        mTexCoord1[27].x = right-mBorderFixed.x;
        mTexCoord1[27].y = top+mBorderFixed.y;
        mTexCoord1[28].x = left+mBorderFixed.x;
        mTexCoord1[28].y = bottom-mBorderFixed.y;
        mTexCoord1[29].x = right-mBorderFixed.x;
        mTexCoord1[29].y = bottom-mBorderFixed.y;

        mTexCoord1[30].x = right-mBorderFixed.x;
        mTexCoord1[30].y = top+mBorderFixed.y;
        mTexCoord1[31].x = right-mBorderFixed.x;
        mTexCoord1[31].y = bottom-mBorderFixed.y;
        mTexCoord1[32].x = right;
        mTexCoord1[32].y = top+mBorderFixed.y;
        mTexCoord1[33].x = right;
        mTexCoord1[33].y = top+mBorderFixed.y;
        mTexCoord1[34].x = right-mBorderFixed.x;
        mTexCoord1[34].y = bottom-mBorderFixed.y;
        mTexCoord1[35].x = right;
        mTexCoord1[35].y = bottom-mBorderFixed.y;

        mTexCoord1[36].x = left;
        mTexCoord1[36].y = bottom-mBorderFixed.y;
        mTexCoord1[37].x = left;
        mTexCoord1[37].y = bottom;
        mTexCoord1[38].x = left+mBorderFixed.x;
        mTexCoord1[38].y = bottom-mBorderFixed.y;
        mTexCoord1[39].x = left+mBorderFixed.x;
        mTexCoord1[39].y = bottom-mBorderFixed.y;
        mTexCoord1[40].x = left;
        mTexCoord1[40].y = bottom;
        mTexCoord1[41].x = left+mBorderFixed.x;
        mTexCoord1[41].y = bottom;

        if(mTailVisible)
        {
            if(mTailDirection)
            {
                mTexCoord1[42].x = left+mBorderFixed.x;
                mTexCoord1[42].y = bottom-mBorderFixed.y;
                mTexCoord1[43].x = left+mBorderFixed.x;
                mTexCoord1[43].y = bottom;
                mTexCoord1[44].x = right-mBorderFixed.x;
                mTexCoord1[44].y = bottom-mBorderFixed.y;
                mTexCoord1[45].x = right-mBorderFixed.x;
                mTexCoord1[45].y = bottom-mBorderFixed.y;
                mTexCoord1[46].x = left+mBorderFixed.x;
                mTexCoord1[46].y = bottom;
                mTexCoord1[47].x = right-mBorderFixed.x;
                mTexCoord1[47].y = bottom;
            } else {
                mTexCoord1[42].x = left+mBorderFixed.x;
                mTexCoord1[42].y = bottom-mBorderFixed.y;
                mTexCoord1[43].x = left+mBorderFixed.x;
                mTexCoord1[43].y = bottom;
                mTexCoord1[44].x = tail;
                mTexCoord1[44].y = bottom-mBorderFixed.y;
                mTexCoord1[45].x = tail;
                mTexCoord1[45].y = bottom-mBorderFixed.y;
                mTexCoord1[46].x = left+mBorderFixed.x;
                mTexCoord1[46].y = bottom;
                mTexCoord1[47].x = tail;
                mTexCoord1[47].y = bottom;
            }
        } else {
            mTexCoord1[42].x = left+mBorderFixed.x;
            mTexCoord1[42].y = bottom-mBorderFixed.y;
            mTexCoord1[43].x = left+mBorderFixed.x;
            mTexCoord1[43].y = bottom;
            mTexCoord1[44].x = right-mBorderFixed.x;
            mTexCoord1[44].y = bottom-mBorderFixed.y;
            mTexCoord1[45].x = right-mBorderFixed.x;
            mTexCoord1[45].y = bottom-mBorderFixed.y;
            mTexCoord1[46].x = left+mBorderFixed.x;
            mTexCoord1[46].y = bottom;
            mTexCoord1[47].x = right-mBorderFixed.x;
            mTexCoord1[47].y = bottom;
        }

        mTexCoord1[48].x = right-mBorderFixed.x;
        mTexCoord1[48].y = bottom-mBorderFixed.y;
        mTexCoord1[49].x = right-mBorderFixed.x;
        mTexCoord1[49].y = bottom;
        mTexCoord1[50].x = right;
        mTexCoord1[50].y = bottom-mBorderFixed.y;
        mTexCoord1[51].x = right;
        mTexCoord1[51].y = bottom-mBorderFixed.y;
        mTexCoord1[52].x = right-mBorderFixed.x;
        mTexCoord1[52].y = bottom;
        mTexCoord1[53].x = right;
        mTexCoord1[53].y = bottom;

        if(mTailVisible)
        {
            if(!mTailDirection)
            {
                mTexCoord1[54].x = tail;
                mTexCoord1[54].y = bottom-mBorderFixed.y;
                mTexCoord1[55].x = tail;
                mTexCoord1[55].y = bottom+mBorderFixed.y;
                mTexCoord1[56].x = tail+mBorderFixed.x;
                mTexCoord1[56].y = bottom-mBorderFixed.y;
                mTexCoord1[57].x = tail+mBorderFixed.x;
                mTexCoord1[57].y = bottom-mBorderFixed.y;
                mTexCoord1[58].x = tail;
                mTexCoord1[58].y = bottom+mBorderFixed.y;
                mTexCoord1[59].x = tail+mBorderFixed.x;
                mTexCoord1[59].y = bottom+mBorderFixed.y;

                mTexCoord1[60].x = tail+mBorderFixed.x;
                mTexCoord1[60].y = bottom-mBorderFixed.y;
                mTexCoord1[61].x = tail+mBorderFixed.x;
                mTexCoord1[61].y = bottom;
                mTexCoord1[62].x = right-mBorderFixed.x;
                mTexCoord1[62].y = bottom-mBorderFixed.y;
                mTexCoord1[63].x = right-mBorderFixed.x;
                mTexCoord1[63].y = bottom-mBorderFixed.y;
                mTexCoord1[64].x = tail+mBorderFixed.x;
                mTexCoord1[64].y = bottom;
                mTexCoord1[65].x = right-mBorderFixed.x;
                mTexCoord1[65].y = bottom;
            } else {
                mTexCoord1[54].x = tail;
                mTexCoord1[54].y = top-mBorderFixed.y;
                mTexCoord1[55].x = tail;
                mTexCoord1[55].y = top+mBorderFixed.y;
                mTexCoord1[56].x = tail+mBorderFixed.x;
                mTexCoord1[56].y = top-mBorderFixed.y;
                mTexCoord1[57].x = tail+mBorderFixed.x;
                mTexCoord1[57].y = top-mBorderFixed.y;
                mTexCoord1[58].x = tail;
                mTexCoord1[58].y = top+mBorderFixed.y;
                mTexCoord1[59].x = tail+mBorderFixed.x;
                mTexCoord1[59].y = top+mBorderFixed.y;

                mTexCoord1[60].x = tail+mBorderFixed.x;
                mTexCoord1[60].y = top;
                mTexCoord1[61].x = tail+mBorderFixed.x;
                mTexCoord1[61].y = top+mBorderFixed.y;
                mTexCoord1[62].x = right-mBorderFixed.x;
                mTexCoord1[62].y = top;
                mTexCoord1[63].x = right-mBorderFixed.x;
                mTexCoord1[63].y = top;
                mTexCoord1[64].x = tail+mBorderFixed.x;
                mTexCoord1[64].y = top+mBorderFixed.y;
                mTexCoord1[65].x = right-mBorderFixed.x;
                mTexCoord1[65].y = top+mBorderFixed.y;
            }
        } else {
            mTexCoord1[54].x = 0.0f;
            mTexCoord1[54].y = 0.0f;
            mTexCoord1[55].x = 0.0f;
            mTexCoord1[55].y = 0.0f;
            mTexCoord1[56].x = 0.0f;
            mTexCoord1[56].y = 0.0f;
            mTexCoord1[57].x = 0.0f;
            mTexCoord1[57].y = 0.0f;
            mTexCoord1[58].x = 0.0f;
            mTexCoord1[58].y = 0.0f;
            mTexCoord1[59].x = 0.0f;
            mTexCoord1[59].y = 0.0f;

            mTexCoord1[60].x = 0.0f;
            mTexCoord1[60].y = 0.0f;
            mTexCoord1[61].x = 0.0f;
            mTexCoord1[61].y = 0.0f;
            mTexCoord1[62].x = 0.0f;
            mTexCoord1[62].y = 0.0f;
            mTexCoord1[63].x = 0.0f;
            mTexCoord1[63].y = 0.0f;
            mTexCoord1[64].x = 0.0f;
            mTexCoord1[64].y = 0.0f;
            mTexCoord1[65].x = 0.0f;
            mTexCoord1[65].y = 0.0f;
        }

        mGeomUVsOutOfDate = true;
    }

} // namespace
