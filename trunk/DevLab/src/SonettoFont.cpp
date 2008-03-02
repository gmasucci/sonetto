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

#include "SonettoFont.h"
#include "SonettoFontSerializer.h"

namespace Sonetto
{
    using namespace Ogre;

    Font::Font(Ogre::ResourceManager* creator, const Ogre::String &name,
                             Ogre::ResourceHandle handle, const Ogre::String &group, bool isManual,
                             Ogre::ManualResourceLoader *loader) :
            Ogre::Resource(creator, name, handle, group, isManual, loader)
    {
        /* If you were storing a pointer to an object, then you would set that pointer to NULL here.
        */



        /* For consistency with StringInterface, but we don't add any parameters here
        That's because the Resource implementation of StringInterface is to
        list all the options that need to be set before loading, of which
        we have none as such. Full details can be set through scripts.
        */
        createParamDictionary("Font");
    }
    Font::~Font()
    {
        unload();
    }
    void Font::loadImpl()
    {
        FontSerializer serializer;
        Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(mName, mGroup, true, this);
        serializer.importFont(stream, this);
        deleteMaterial();
        createMaterial();
    }
    void Font::unloadImpl()
    {
        /* If you were storing a pointer to an object, then you would check the pointer here,
        and if it is not NULL, you would destruct the object and set its pointer to NULL again.
        */
        glyphs.clear();
        mColorList.clear();
    }
    size_t Font::calculateSize() const
    {
        return 0;
    }
    void Font::calculateGlyphCoords()
    {
        glyphs.clear();
        for (unsigned long tp = 0; tp < num_glyphs; ++tp)
        {
            /*
            h0,v0 --- h1,v0
            |          |
            h0,v1 --- h1,v1
            */
            float texcoord_h_0 = glyph_coord_offset.x * (tp % glyph_tbl.x);
            float texcoord_h_1 = glyph_coord_offset.x * ((tp % glyph_tbl.x) + 1);
            float texcoord_v_0 = glyph_coord_offset.y * (tp / glyph_tbl.y);
            float texcoord_v_1 = glyph_coord_offset.y * ((tp / glyph_tbl.y) + 1);
            GlyphStruct glyphs_tmp;
            glyphs_tmp.texcoord0 = Ogre::Vector2(texcoord_h_0,texcoord_v_0);
            glyphs_tmp.texcoord1 = Ogre::Vector2(texcoord_h_1,texcoord_v_0);
            glyphs_tmp.texcoord2 = Ogre::Vector2(texcoord_h_0,texcoord_v_1);
            glyphs_tmp.texcoord3 = Ogre::Vector2(texcoord_h_1,texcoord_v_1);

            glyphs_tmp.x_offset = default_y_space;

            glyphs.push_back(glyphs_tmp);
        }
    }

    bool Font::createMaterial()
    {
        font_material = MaterialManager::getSingleton().create(
                            "mat_"+mName, mGroup);

        if (font_material.isNull())
        {
        }

        TextureUnitState *texture_unit;

        font_texture = TextureManager::getSingleton().load(font_texture_name, mGroup,Ogre::TEX_TYPE_2D, 0);
        // Manually load since we need to load to get alpha
        texture_unit = font_material->getTechnique(0)->getPass(0)->createTextureUnitState(font_texture_name);

        font_material->getTechnique(0)->getPass(0)->setCullingMode(CULL_NONE);

        // Clamp to avoid fuzzy edges
        texture_unit->setTextureAddressingMode( Ogre::TextureUnitState::TAM_CLAMP );
        // Allow min/mag filter, but no mip

        // Set up blending
        switch(font_blend_mode)
        {
            case Sonetto::FBM_TRANSPARENT_ALPHA:
                font_material->setSceneBlending( Ogre::SBT_TRANSPARENT_ALPHA );
            break;
            case Sonetto::FBM_TRANSPARENT_COLOR:
                font_material->setSceneBlending( Ogre::SBT_TRANSPARENT_COLOUR );
            break;
            case Sonetto::FBM_ADD:
                font_material->setSceneBlending( Ogre::SBT_ADD );
            break;
            case Sonetto::FBM_MODULATE:
                font_material->setSceneBlending( Ogre::SBT_MODULATE );
            break;
            case Sonetto::FBM_REPLACE:
                font_material->setSceneBlending( Ogre::SBT_REPLACE );
            break;
        }

        switch(font_alpha_reject_settings)
        {
            case Sonetto::SCF_ALWAYS_FAIL:
            font_material->getTechnique(0)->getPass(0)->setAlphaRejectSettings(Ogre::CMPF_ALWAYS_FAIL, alpha_rejection);
            break;
            case Sonetto::SCF_ALWAYS_PASS:
            font_material->getTechnique(0)->getPass(0)->setAlphaRejectSettings(Ogre::CMPF_ALWAYS_PASS, alpha_rejection);
            break;
            case Sonetto::SCF_LESS:
            font_material->getTechnique(0)->getPass(0)->setAlphaRejectSettings(Ogre::CMPF_LESS, alpha_rejection);
            break;
            case Sonetto::SCF_LESS_EQUAL:
            font_material->getTechnique(0)->getPass(0)->setAlphaRejectSettings(Ogre::CMPF_LESS_EQUAL, alpha_rejection);
            break;
            case Sonetto::SCF_EQUAL:
            font_material->getTechnique(0)->getPass(0)->setAlphaRejectSettings(Ogre::CMPF_EQUAL, alpha_rejection);
            break;
            case Sonetto::SCF_NOT_EQUAL:
            font_material->getTechnique(0)->getPass(0)->setAlphaRejectSettings(Ogre::CMPF_NOT_EQUAL, alpha_rejection);
            break;
            case Sonetto::SCF_GREATER_EQUAL:
            font_material->getTechnique(0)->getPass(0)->setAlphaRejectSettings(Ogre::CMPF_GREATER_EQUAL, alpha_rejection);
            break;
            case Sonetto::SCF_GREATER:
            font_material->getTechnique(0)->getPass(0)->setAlphaRejectSettings(Ogre::CMPF_GREATER, alpha_rejection);
            break;
        }

        switch(font_filtering_mode)
        {
            case Sonetto::FT_NONE:
            texture_unit->setTextureFiltering(Ogre::FO_NONE, Ogre::FO_NONE, Ogre::FO_NONE);
            break;
            case Sonetto::FT_POINT:
            texture_unit->setTextureFiltering(Ogre::FO_POINT, Ogre::FO_POINT, Ogre::FO_NONE);
            break;
            case Sonetto::FT_LINEAR:
            texture_unit->setTextureFiltering(Ogre::FO_LINEAR, Ogre::FO_LINEAR, Ogre::FO_NONE);
            break;
            case Sonetto::FT_ANISOTROPIC:
            texture_unit->setTextureFiltering(Ogre::FO_ANISOTROPIC, Ogre::FO_ANISOTROPIC, Ogre::FO_NONE);
            break;
        }
        /*font_material->setSceneBlending( Ogre::SBT_TRANSPARENT_ALPHA );
        font_material->getTechnique(0)->getPass(0)->setAlphaRejectSettings(Ogre::CMPF_ALWAYS_PASS, alpha_rejection);
        texture_unit->setTextureFiltering(Ogre::FO_LINEAR, Ogre::FO_LINEAR, Ogre::FO_NONE);*/

        return true;
    }
    bool Font::deleteMaterial()
    {
        if (!font_material.isNull())
        {
            MaterialManager::getSingleton().remove( font_material->getHandle());
            font_material.setNull();
        }

        if (!font_texture.isNull())
        {
            TextureManager::getSingleton().remove( font_texture->getHandle());
            font_texture.setNull();
        }

        return true;
    }
    void Font::setTextureName( const Ogre::String &name)
    {
        font_texture_name = name;
    }

    const Ogre::String &Font::getTextureName()
    {
        return font_texture_name;
    }
    void Font::setUpFont(unsigned long x, unsigned long y, float default_space, const Ogre::String &name)
    {
        glyph_tbl.x = x;
        glyph_tbl.y = y;
        default_y_space = default_space;
        num_glyphs = x * y;

        glyph_coord_offset.x = (1.0 / x);
        glyph_coord_offset.y = (1.0 / y);
        setTextureName(name);
        calculateGlyphCoords();
        deleteMaterial();
        createMaterial();
    }
} // namespace
