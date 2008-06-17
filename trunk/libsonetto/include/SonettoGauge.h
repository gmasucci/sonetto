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
#ifndef __SONETTO_GAUGE__
#define __SONETTO_GAUGE__

#include "SonettoMain.h"

#include "OGRE/OgreOverlayElement.h"
#include "OGRE/OgreOverlayElementFactory.h"
#include "OGRE/OgreOverlayManager.h"

namespace Sonetto {

    class SONETTO_EXPORT GaugeRenderable; // Forward declaration
    /** Simple Gauge object.
    @remarks
        Gauge is an Overlay Element not an Overlay Container,
        as it cannot hold objects inside it.
    @par
        Note that Height does not affect the gauge size,
        use setGaugeSize() instead
    @par
        Also, note that this gauge in special does not accept materials,
        only Vertex Colors.
    */
    class SONETTO_EXPORT Gauge : public Ogre::OverlayContainer
    {
        friend class SONETTO_EXPORT GaugeRenderable;
    public:
        /// Construct SONETTO_EXPORTor.
        Gauge(const Ogre::String &name);
        /// Destruct SONETTO_EXPORTor.
        virtual ~Gauge();
        /// Initialise.
        void initialise(void);
        /// Set the pointer to the max value for this gauge
        void setMaxValPtr(int * ptr);
        /// Get the pointer to the max value for this gauge
        int * getMaxValPtr();
        /// Set the pointer to the current value for this gauge
        void setCurValPtr(int * ptr);
        /// Get the pointer to the current value for this gauge
        int * getCurValPtr();
        /// Set Gauge Color (Left and Right side colors)
        void setGaugeColor(const Ogre::ColourValue &lcolor, const Ogre::ColourValue &rcolor);
        /// Get the left gauge color.
        Ogre::ColourValue getLeftGaugeColor();
        /// Get the right gauge color.
        Ogre::ColourValue getRightGaugeColor();
        /// Set the Gauge's Back color.
        void setBackColor(const Ogre::ColourValue &color);
        /// Get the Gauge's Back color.
        Ogre::ColourValue getBackColor();
        /// Set the Gauge inclination (can be either positive or negative).
        void setGaugeInclination(const Ogre::Real incl);
        /// Get the Gauge inclintation.
        Ogre::Real getGaugeInclination();
        /// Set the Gauge size.
        void setGaugeSize(const Ogre::Real size);
        /// Get the Gauge size.
        Ogre::Real getGaugeSize();
        /// Set the Back distance.
        void setBackDistance(const Ogre::Real size);
        /// Get the Back distance.
        Ogre::Real getBackDistance();
        /// See OverlayElement.
        virtual const Ogre::String& getTypeName(void) const;
        /// See Renderable.
        void getRenderOperation(Ogre::RenderOperation& op);
        /// Overridden from OverlayElement.
        void setMaterialName(const Ogre::String& matName);
        /// Overridden from OverlayElement.
        void _updateRenderQueue(Ogre::RenderQueue* queue);
        /// Visit Renderables
        void visitRenderables(Ogre::Renderable::Visitor* visitor, bool debugRenderables = false);
        /// Overridden from OverlayElement.
        virtual void _update(void);
    protected:
        /// Gauge Renderable
        GaugeRenderable * mGaugeRenderable;
        /// Metrics Mode
        ScreenMetricsMode mScrMetricsMode;
        /// Update Colors flag.
        bool mUpdateColor;
        /// Aspect Ratio
        Ogre::Real mAspectRatio;
        /// Inclination Level
        Ogre::Real mInclination;
        /// Gauge Size
        Ogre::Real mGaugeSize;
        /// Gauge Back Distance
        Ogre::Real mBackDistance;
        /** Maximun Gauge Value
        @remarks
            This is a pointer to a value, the function will read
            from here to set the gauge appearence
        */
        int * mMaxVal;
        /** Current Gauge Value
        @remarks
            This is a pointer to a value, the function will read
            from here to set the gauge appearence
        */
        int * mCurVal;
        /// Left Gauge Color
        Ogre::ColourValue mLeftColor;
        /// Right Gauge Color
        Ogre::ColourValue mRightColor;
        /// Background Color Value
        Ogre::ColourValue mBgColor;
        /// Gauge Background Render Operation.
        Ogre::RenderOperation mRenderOp;
        /// Gauge Render Operation.
        Ogre::RenderOperation mGaugeRenderOp;
        /// internal method for setting up geometry, called by OverlayElement::update.
        virtual void updatePositionGeometry(void);
        virtual void updateTextureGeometry(void);
        /// Called to update the colours.
        virtual void updateColors(void);
        /// Typename for identification.
        static Ogre::String msTypeName;
    };

    /** class SONETTO_EXPORT for rendering the border of a Window.
     *  @remarks
     *  We need this because we have to render twice, once with the inner window's repeating
     *  material (handled by superclass SONETTO_EXPORT) and once for the border's separate material.
     */
    class SONETTO_EXPORT GaugeRenderable : public Ogre::Renderable
    {
    protected:
        Gauge * mpParent;
    public:
        /// Construct SONETTO_EXPORTed with pointers to parent.
        GaugeRenderable(Gauge* parent) : mpParent(parent)
        {
            mUseIdentityProjection = true;
            mUseIdentityView = true;
        }
        const Ogre::MaterialPtr& getMaterial(void) const
        {
            return mpParent->mpMaterial;
        }
        void getRenderOperation(Ogre::RenderOperation& op)
        {
            op = mpParent->mGaugeRenderOp;
        }
        void getWorldTransforms(Ogre::Matrix4* xform) const
        {
            mpParent->getWorldTransforms(xform);
        }
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

    class SONETTO_EXPORT GaugeFactory : public Ogre::OverlayElementFactory
    {
    public:
        /** See OverlayElementFactory */
        Ogre::OverlayElement* createOverlayElement(const Ogre::String& instanceName)
        {
            return new Sonetto::Gauge(instanceName);
        }
        /** See OverlayElementFactory */
        const Ogre::String& getTypeName(void) const
        {
            static Ogre::String name = "Gauge";
            return name;
        }
    };

}; // namespace Sonetto

#endif // __SONETTO_GAUGE__
