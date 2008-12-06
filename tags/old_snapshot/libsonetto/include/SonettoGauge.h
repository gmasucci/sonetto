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

#include <OgreOverlayElement.h>
#include <OgreOverlayElementFactory.h>
#include <OgreOverlayManager.h>

namespace Sonetto {

    class GaugeRenderable; // Forward declaration

    /** @brief Simple Gauge object.
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
        friend class GaugeRenderable;
    public:
        /// @brief Constructor.
        Gauge(const Ogre::String &name);
        /// @brief Destructor.
        virtual ~Gauge();
        /// @brief Initialise.
        void initialise(void);
        /// @brief Set the pointer to the max value for this gauge
        void setMaxValPtr(int * ptr);
        /// @brief Get the pointer to the max value for this gauge
        int *getMaxValPtr();
        /// @brief Set the pointer to the current value for this gauge
        void setCurValPtr(int * ptr);
        /// @brief Get the pointer to the current value for this gauge
        int *getCurValPtr();
        /// @brief Set Gauge Color (Left and Right side colors)
        void setGaugeColor(const Ogre::ColourValue &lcolor, const Ogre::ColourValue &rcolor);
        /// @brief Get the left gauge color.
        Ogre::ColourValue getLeftGaugeColor();
        /// @brief Get the right gauge color.
        Ogre::ColourValue getRightGaugeColor();
        /// @brief Set the Gauge's Back color.
        void setBackColor(const Ogre::ColourValue &color);
        /// @brief Get the Gauge's Back color.
        Ogre::ColourValue getBackColor();
        /// @brief Set the Gauge inclination (can be either positive or negative).
        void setGaugeInclination(const Ogre::Real incl);
        /// @brief Get the Gauge inclintation.
        Ogre::Real getGaugeInclination();
        /// @brief Set the Gauge size.
        void setGaugeSize(const Ogre::Real size);
        /// @brief Get the Gauge size.
        Ogre::Real getGaugeSize();
        /// @brief Set the Back distance.
        void setBackDistance(const Ogre::Real size);
        /// @brief Get the Back distance.
        Ogre::Real getBackDistance();
        /// @brief See OverlayElement.
        virtual const Ogre::String& getTypeName(void) const;
        /// @brief See Ogre::Renderable.
        void getRenderOperation(Ogre::RenderOperation& op);
        /// @brief Overridden from Ogre::OverlayElement.
        void setMaterialName(const Ogre::String& matName);
        /// @brief Overridden from Ogre::OverlayElement.
        void _updateRenderQueue(Ogre::RenderQueue* queue);
        /* /// @brief Visit Renderables

        void visitRenderables(Ogre::Renderable::Visitor* visitor, bool debugRenderables = false);
        */
        /// @brief Overridden from Ogre::OverlayElement.
        virtual void _update(void);
    protected:
        /// @brief Gauge Renderable
        GaugeRenderable *mGaugeRenderable;
        /// @brief Metrics Mode
        ScreenMetricsMode mScrMetricsMode;
        /// @brief Update Colors flag.
        bool mUpdateColor;
        /// @brief Aspect Ratio
        Ogre::Real mAspectRatio;
        /// @brief Inclination Level
        Ogre::Real mInclination;
        /// @brief Gauge Size
        Ogre::Real mGaugeSize;
        /// @brief Gauge Back Distance
        Ogre::Real mBackDistance;
        /** @brief Maximum Gauge Value
        @remarks
            This is a pointer to a value, the function will read
            from here to set the gauge appearence
        */
        int *mMaxVal;
        /** @brief Current Gauge Value
        @remarks
            This is a pointer to a value, the function will read
            from here to set the gauge appearence
        */
        int *mCurVal;
        /// @brief Left Gauge Color
        Ogre::ColourValue mLeftColor;
        /// @brief Right Gauge Color
        Ogre::ColourValue mRightColor;
        /// @brief Background Color Value
        Ogre::ColourValue mBgColor;
        /// @brief Gauge Background Render Operation.
        Ogre::RenderOperation mRenderOp;
        /// @brief Gauge Render Operation.
        Ogre::RenderOperation mGaugeRenderOp;
        /// @brief Internal method for setting up geometry, called by Ogre::OverlayElement::update.
        virtual void updatePositionGeometry(void);
        virtual void updateTextureGeometry(void);
        /// @brief Called to update the colours.
        virtual void updateColors(void);
        /// @brief Typename for identification.
        static Ogre::String msTypeName;
    };

    /** @brief Class for rendering the border of a Window.
     *  @remarks
     *  We need this because we have to render twice, once with the inner window's repeating
     *  material (handled by superclass) and once for the border's separate material.
     */
    class SONETTO_EXPORT GaugeRenderable : public Ogre::Renderable
    {
    protected:
        Gauge *mpParent;
    public:
        /// @brief Constructed with pointers to parent.
        GaugeRenderable(Gauge* parent) : mpParent(parent)
        {
            mUseIdentityProjection = true;
            mUseIdentityView = true;
        }
        const Ogre::MaterialPtr& getMaterial(void) const
        {
            return mpParent->mpMaterial;
        }
        void getRenderOperation(Ogre::RenderOperation& op) { op = mpParent->mGaugeRenderOp; }
        void getWorldTransforms(Ogre::Matrix4* xform) const { mpParent->getWorldTransforms(xform); }
        const Ogre::Quaternion& getWorldOrientation(void) const { return Ogre::Quaternion::IDENTITY; }
        const Ogre::Vector3& getWorldPosition(void) const { return Ogre::Vector3::ZERO; }
        unsigned short getNumWorldTransforms(void) const { return 1; }
        Ogre::Real getSquaredViewDepth(const Ogre::Camera* cam) const { return mpParent->getSquaredViewDepth(cam); }
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
        /// @brief See OverlayElementFactory
        Ogre::OverlayElement* createOverlayElement(const Ogre::String& instanceName)
        {
            return new Sonetto::Gauge(instanceName);
        }
        /// @brief See OverlayElementFactory
        const Ogre::String& getTypeName(void) const
        {
            static Ogre::String name = "Gauge";
            return name;
        }
    };

}; // namespace Sonetto

#endif // __SONETTO_GAUGE__
