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
#ifndef SONETTO_WINDOW_H_
#define SONETTO_WINDOW_H_
#include <OgreOverlayContainer.h>
#include <OgreVector2.h>
#include "SonettoMain.h"
#include "SonettoWindowSkin.h"

namespace Sonetto
{
    class BorderRenderable;
#define NUM_VERTEX 54
    //66
    class Window : public Ogre::OverlayContainer
    {
        friend class BorderRenderable;
    public:
        /// Constructor.
        Window(const Ogre::String& name);
        /// Destructor.
        virtual ~Window();
        /// Initialise
        virtual void initialise (void);
        /// Set the WindowSkin
        void setWindowSkin(WindowSkinPtr winSkin);
        /** Set this WindowType
        *
        *   Menu - For Menus
        *   Main - For Message Boxes and orther.
        *   Enemy - For Enemy Windows
        *   System - For System Messages
        */
        void setWindowType(WindowType wt);
        /// Get this Window Type
        WindowType getWindowType();
        /// Set Window Alpha
        void setWindowAlpha(Ogre::Real alpha);
        /// Get Window Alpha
        Ogre::Real getWindowAlpha();
        /// Set Border Alpha
        void setBorderAlpha(Ogre::Real alpha);
        /// Get Border Alpha
        Ogre::Real getBorderAlpha();
        /// Tells this element how to interpret the position and dimension values it is given.
        void setScrMetricsMode(ScreenMetricsMode smm);
        /** See OverlayElement. */
        virtual const Ogre::String& getTypeName(void) const;
        /** See Renderable. */
        void getRenderOperation(Ogre::RenderOperation& op);
        /** Overridden from OverlayContainer */
        void _updateRenderQueue(Ogre::RenderQueue* queue);
        /// Visit Renderables
        void visitRenderables(Ogre::Renderable::Visitor* visitor,
                              bool debugRenderables = false);
        /** Overridden from OverlayContaienr */
        void _update(void);
    protected:

        /// The WindowSkin for this Window
        WindowSkinPtr mWindowSkin;
        /// Window Type
        WindowType mWindowType;

        Ogre::Real mWindowAlpha;
        Ogre::Real mBorderAlpha;

        bool mUpdateBorderColors;
        bool mUpdateBackgroundColors;
        /// Border Renderable Pointer
        BorderRenderable * mBorderRenderable;
        /// Window vertex coordinates
        Ogre::Vector2 mPosCoord[NUM_VERTEX];
        /// Background coordinates
        Ogre::Vector2 mTexCoord1[NUM_VERTEX];
        /// Screen Metrics Mode
        ScreenMetricsMode mScrMetricsMode;
        /// Viewport Aspect Ratio
        Ogre::Real mAspectRatio;
        /// Background Render Operation
        Ogre::RenderOperation mRenderOp;
        /// Border Render Operation
        Ogre::RenderOperation mRenderOp2;
        /// Type Name
        static Ogre::String msTypeName;
        /// internal method for setting up geometry, called by OverlayElement::update
        virtual void updatePositionGeometry(void);
        /// Called to update the texture coords.
        virtual void updateTextureGeometry(void);
        /// Called to update the border colors.
        void updateBorderColors(void);
        /// Called to update the background colors.
        void updateBackgroundColors(void);
        /// Called to calculate the window's vertexes
        void calculatePosition(void);
        /// Called to calculate the window's background texture coordinates
        void calculateTexCoord(void);
    };
    /** Class for rendering the border of a Window.
    * @remarks
    * We need this because we have to render twice, once with the inner panel's repeating
    * material (handled by superclass) and once for the border's separate meterial.
    */
    class BorderRenderable : public Ogre::Renderable
    {
    protected:
        Window* mParent;
    public:
        /** Constructed with pointers to parent. */
        BorderRenderable(Window* parent) : mParent(parent)
        {
            mUseIdentityProjection = true;
            mUseIdentityView = true;
        }
        const Ogre::MaterialPtr& getMaterial(void) const
        {
            return mParent->mWindowSkin->mBorderMaterial;
        }
        void getRenderOperation(Ogre::RenderOperation& op)
        {
            op = mParent->mRenderOp2;
        }
        void getWorldTransforms(Ogre::Matrix4* xform) const
        {
            mParent->getWorldTransforms(xform);
        }
        unsigned short getNumWorldTransforms(void) const
        {
            return 1;
        }
        Ogre::Real getSquaredViewDepth(const Ogre::Camera* cam) const
        {
            return mParent->getSquaredViewDepth(cam);
        }
        const Ogre::LightList& getLights(void) const
        {
            // N/A, panels are not lit
            static Ogre::LightList ll;
            return ll;
        }
        bool getPolygonModeOverrideable(void) const
        {
            return mParent->getPolygonModeOverrideable();
        }
    };
} // namespace
#endif //SONETTO_WINDOW_H
