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
#ifndef __SONETTO_PLANE__
#define __SONETTO_PLANE__

#include "SonettoMain.h"

#include <OgreOverlayContainer.h>
#include "SonettoMain.h"

namespace Sonetto {
    /** OverlayElement representing a flat, single-material (or transparent) panel which can contain other elements.
    @remarks
        This class subclasses OverlayContainer because it can contain other elements. Like other
        containers, if hidden it's contents are also hidden, if moved it's contents also move etc.
        The panel itself is a 2D rectangle which is either completely transparent, or is rendered
        with a single material. The texture(s) on the panel can be tiled depending on your requirements.
    @par
        This component is suitable for backgrounds and grouping other elements. Note that because
        it has a single repeating material it cannot have a discrete border (unless the texture has one and
        the texture is tiled only once). For a bordered panel, see it's subclass BorderPanelOverlayElement.
    @par
        Note that the material can have all the usual effects applied to it like multiple texture
        layers, scrolling / animated textures etc. For multiple texture layers, you have to set
        the tiling level for each layer.
    */
    class SONETTO_EXPORT Plane : public Ogre::OverlayContainer
    {
    public:
        /** Constructor. */
        Plane(const Ogre::String& name);
        virtual ~Plane();

        /** Initialise */
        virtual void initialise(void);

        /** Sets the number of times textures should repeat.
        @param x The number of times the texture should repeat horizontally
        @param y The number of times the texture should repeat vertically
        @param layer The texture layer to specify (only needs to be altered if
            you're using a multi-texture layer material)
        */
        void setTiling(Ogre::Real x, Ogre::Real y, Ogre::ushort layer = 0);

        Ogre::Real getTileX(Ogre::ushort layer = 0) const;
        /** Gets the number of times the texture should repeat vertically.
        @param layer The texture layer to specify (only needs to be altered if
            you're using a multi-texture layer material)
        */
        Ogre::Real getTileY(Ogre::ushort layer = 0) const;

        /** Sets the texture coordinates for the panel. */
        void setUV(Ogre::Real u1, Ogre::Real v1, Ogre::Real u2, Ogre::Real v2);

        /** Get the uv coordinates for the panel*/
        void getUV(Ogre::Real& u1, Ogre::Real& v1, Ogre::Real& u2, Ogre::Real& v2) const;

        /** Sets whether this panel is transparent (used only as a grouping level), or
            if it is actually renderred.
        */
        void setTransparent(bool isTransparent);

        /** Returns whether this panel is transparent. */
        bool isTransparent(void) const;

        /** See OverlayElement. */
        virtual const Ogre::String& getTypeName(void) const;
        /** See Renderable. */
        void getRenderOperation(Ogre::RenderOperation& op);
        /** Overridden from OverlayElement */
        void setMaterialName(const Ogre::String& matName);

        void setScrMetricsMode(ScreenMetricsMode smm);
        ScreenMetricsMode getScrMetricsMode();

        /** Overridden from OverlayContainer */
        void _updateRenderQueue(Ogre::RenderQueue* queue);

        void _update(void);

        void setAlpha(Ogre::Real alpha);

    protected:
        bool mColorUpdate;
        // Flag indicating if this panel should be visual or just group things
        bool mTransparent;
        // Texture tiling
        Ogre::Real mTileX[OGRE_MAX_TEXTURE_LAYERS];
        Ogre::Real mTileY[OGRE_MAX_TEXTURE_LAYERS];
        size_t mNumTexCoordsInBuffer;
        Ogre::Real mU1, mV1, mU2, mV2;

        ScreenMetricsMode mScrMetricsMode;

        // Object Alpha Level.
        Ogre::Real mAlphaLevel;

        Ogre::RenderOperation mRenderOp;

        /// internal method for setting up geometry, called by OverlayElement::update
        virtual void updatePositionGeometry(void);

        /// Called to update the texture coords when layers change
        virtual void updateTextureGeometry(void);

        void updateColour(void);

        static Ogre::String msTypeName;

    };

}; // namespace
#endif // __SONETTO_PLANE__
