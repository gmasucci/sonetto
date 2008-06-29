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
#ifndef SONETTO_TAILEDWINDOW_H
#define SONETTO_TAILEDWINDOW_H

#include "SonettoMain.h"

#include <OgreOverlayContainer.h>
#include <OgreVector2.h>
#include "SonettoMain.h"
#include "SonettoWindowSkin.h"

namespace Sonetto
{

    class SONETTO_EXPORT TailedBorderRenderable;

    #define TAILED_NUM_VERTEX 66

    class SONETTO_EXPORT TailedWindow : public Ogre::OverlayContainer
    {
        friend class TailedBorderRenderable;
    public:
        /// @brief Constructor.
        TailedWindow(const Ogre::String& name);
        /// @brief Destructor.
        virtual ~TailedWindow();
        /// @brief Initialise.
        virtual void initialise (void);
        /** @brief Set the WindowSkin this element will use.
         *
         *  @remarks    The WindowSkin contain the basic appearence of a Window.
         *  Without it, the window will not appear in the screen,
         *  you may change the WindowSkin anytime.
         *  @param wskin a WindowSkin pointer.
         */
        void setWindowSkin(WindowSkinPtr pwskin);
        /// @brief Get this element WindowSkin.
        WindowSkinPtr getWindowSkin(void);
        /** @brief Set this element WindowType
         *
         *  @remarks
         *  Menu - For Menus
         *  Main - For Message Boxes and orther.
         *  Enemy - For Enemy Windows
         *  System - For System Messages
         */
        void setWindowType(WindowType wt);
        /// @brief Get this element Window Type.
        WindowType getWindowType(void);
        /// @brief Set Window Alpha.
        void setWindowAlpha(Ogre::Real alpha);
        /// @brief Get Window Alpha.
        Ogre::Real getWindowAlpha(void);
        /// @brief Set Border Alpha.
        void setBorderAlpha(Ogre::Real alpha);
        /// @brief Get Border Alpha.
        Ogre::Real getBorderAlpha(void);
        /** @brief Show or Hide the TailedWindow Tail, set it's direction and it's position.
        *
        * @param visible The tail visibility, false for invisible and true for visible.
        * @param dir The tail direction, false is down, true is up.
        * @param dir The tail position, range from 0.0 to 1.0, from one side of the TailedWindow to the other.
        */
        void setWindowTail(bool visible, bool dir, float pos);
        /** @brief Show or Hide the TailedWindow Tail.
        *
        * @param visible The tail visibility, false for invisible and true for visible.
        */
        void setTailVisibility(bool visible);
        /// @brief Return whether the tail is visible or not.
        bool isVisible();
        /** @brief Set tail direction.
        *
        * @param dir The tail direction, false is down, true is up.
        */
        void setTailDirection(bool dir);
        /// @brief Return the tail direction.
        bool getTailDirection();
        /** @brief Set tail position.
        *
        * @param dir The tail position, range from 0.0 to 1.0, from one side of the TailedWindow to the other.
        */
        void setTailPosition(float pos);
        /// @brief Return the tail position.
        float getTailPosition();
        /// @brief Tells this element how to interpret the position and dimension values it is given.
        void setScrMetricsMode(ScreenMetricsMode smm);
        /// @brief Get this element metrics mode.
        ScreenMetricsMode getScrMetricsMode(void);
        /// @brief See Ogre::OverlayElement.
        virtual const Ogre::String& getTypeName(void) const;
        /// @brief See Ogre::Renderable.
        void getRenderOperation(Ogre::RenderOperation& op);
        /// @brief Overridden from Ogre::OverlayContainer.
        void _updateRenderQueue(Ogre::RenderQueue* queue);
        /* /// Visit Renderables (only for Ogre 1.5.0 and up)

        void visitRenderables(Ogre::Renderable::Visitor* visitor, bool debugRenderables = false);
        */
        /// @brief Overridden from Ogre::OverlayContainer.
        void _update(void);
    protected:
        /// @brief Internal method for setting up geometry, called by Ogre::OverlayElement::update.
        virtual void updatePositionGeometry(void);
        /// @brief Called to update the texture coords.
        virtual void updateTextureGeometry(void);
        /// @brief Called to update the border colors.
        void updateBorderColors(void);
        /// @brief Called to update the background colors.
        void updateWindowColors(void);
        /// @brief Called to calculate the window's vertexes
        void calculatePosition(void);
        /// @brief Called to calculate the window's background texture coordinates
        void calculateTexCoord(void);
        /// @brief This Element's WindowSkin.
        WindowSkinPtr mpWindowSkin;
        /// @brief This Element's Window Type.
        WindowType mWindowType;
        /// @brief Window Alpha.
        Ogre::Real mWindowAlpha;
        /// @brief Border Alpha.
        Ogre::Real mBorderAlpha;
        /// @brief Whether updating the window colors is needed.
        bool mUpdateWindowColors;
        /// @brief Whether updating the border colors is needed.
        bool mUpdateBorderColors;
        /// @brief Window Border Renderable Pointer.
        TailedBorderRenderable * mpBorderRenderable;
        /// @brief Tail Visibility
        bool mTailVisible;
        /// @brief Tail Direction
        bool mTailDirection;
        /// @brief Tail Position
        float mTailPosition;
        /// @brief Window vertex coordinates.
        Ogre::Vector2 mPosCoord[TAILED_NUM_VERTEX];
        /// @brief Background texture coordinates.
        Ogre::Vector2 mTexCoord[TAILED_NUM_VERTEX];
        /// @brief This Element's Screen Metrics Mode.
        ScreenMetricsMode mScrMetricsMode;
        /// @brief Viewport Aspect Ratio.
        Ogre::Real mAspectRatio;
        /// @brief Window Render Operation.
        Ogre::RenderOperation mWindowRenderOp;
        /// @brief Border Render Operation.
        Ogre::RenderOperation mBorderRenderOp;
        /// @brief This Object's Type Name.
        static Ogre::String msTypeName;
        /** @brief Linear interpolation, interpolate between two values
        *
        * @param a The initial value.
        * @param b The destiny value.
        * @param t The time/weight value. Ranges from 0.0 to 1.0.
        */
        float lerp(float a, float b, float t)
        {
            return a + t * (b - a);
        }
    };
    /** @brief Class for rendering the border of a Window.
     *  @remarks
     *  We need this because we have to render twice, once with the inner window's repeating
     *  material (handled by superclass) and once for the border's separate material.
     */
    class SONETTO_EXPORT TailedBorderRenderable : public Ogre::Renderable {
    protected:
        TailedWindow * mpParent;
    public:
        /// @brief Constructed with pointers to parent.
        TailedBorderRenderable(TailedWindow* parent) : mpParent(parent)
        {
            mUseIdentityProjection = true;
            mUseIdentityView = true;
        }
        const Ogre::MaterialPtr& getMaterial(void) const
        {
            return mpParent->mpWindowSkin->mBorderMaterial;
        }
        void getRenderOperation(Ogre::RenderOperation& op)
        {
            op = mpParent->mBorderRenderOp;
        }
        void getWorldTransforms(Ogre::Matrix4* xform) const
        {
            mpParent->getWorldTransforms(xform);
        }
        const Ogre::Quaternion& getWorldOrientation(void) const { return Ogre::Quaternion::IDENTITY; }
        const Ogre::Vector3& getWorldPosition(void) const { return Ogre::Vector3::ZERO; }
        unsigned short getNumWorldTransforms(void) const
        {
            return 1;
        }
        Ogre::Real getSquaredViewDepth(const Ogre::Camera* cam) const
        {
            return mpParent->getSquaredViewDepth(cam);
        }
        const Ogre::LightList& getLights(void) const
        {
            // N/A, panels are not lit
            static Ogre::LightList ll;
            return ll;
        }
        bool getPolygonModeOverrideable(void) const
        {
            return mpParent->getPolygonModeOverrideable();
        }
    };
} // namespace
#endif //SONETTO_TAILEDWINDOW_H
