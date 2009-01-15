/*-----------------------------------------------------------------------------
Copyright (c) 2009, Sonetto Project Developers
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1.  Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
2.  Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
3.  Neither the name of the Sonetto Project nor the names of its contributors
    may be used to endorse or promote products derived from this software
    without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
-----------------------------------------------------------------------------*/

#include "SonettoFontSerializer.h"
#include "SonettoFont.h"

#include <OgreMaterialManager.h>
#include <OgreTechnique.h>
#include <OgreTextureManager.h>

namespace Sonetto
{
    // ----------------------------------------------------------------------
    FontSerializer::FontSerializer()
    {
    }
    // ----------------------------------------------------------------------
    FontSerializer::~FontSerializer()
    {
    }
    // ----------------------------------------------------------------------
    void FontSerializer::exportFont(const Font *pFont, const Ogre::String &fileName)
    {
        // Open/create the file
		mpfFile = fopen(fileName.c_str(), "wb");
		if (!mpfFile)
		{
		    // Throw an error if the file was not found, or was not possible to read
		    SONETTO_THROW("A file was not found!");
		}
		fwrite(&Font::mFourCC, sizeof(uint32), 1, mpfFile);

		fwrite(&pFont->mVersion, sizeof(uint32), 1, mpfFile);
		fwrite(&pFont->mEncode, sizeof(uint32), 1, mpfFile);
		fwrite(&pFont->mVerticalOffsetTop, sizeof(float), 1, mpfFile);
		fwrite(&pFont->mVerticalOffsetBottom, sizeof(float), 1, mpfFile);
		fwrite(&pFont->mHorizontalScale, sizeof(float), 1, mpfFile);

		saveString(pFont->mIName);

		if(pFont->mMaterial.isNull())
            SONETTO_THROW("Material does not exist");

		Ogre::Pass * pass = pFont->mMaterial->getTechnique(0)->getPass(0);

		bool has_separate_blend = pass->hasSeparateSceneBlending();

        uint32 mat_scene_blend_source = (uint32)pass->getSourceBlendFactor();
        uint32 mat_scene_blend_dest = (uint32)pass->getDestBlendFactor();
        uint32 mat_scene_blend_source_a = (uint32)pass->getSourceBlendFactorAlpha();
        uint32 mat_scene_blend_dest_a = (uint32)pass->getDestBlendFactorAlpha();

        fwrite(&has_separate_blend, sizeof(bool), 1, mpfFile);
        fwrite(&mat_scene_blend_source, sizeof(uint32), 1, mpfFile);
        fwrite(&mat_scene_blend_dest, sizeof(uint32), 1, mpfFile);
        fwrite(&mat_scene_blend_source_a, sizeof(uint32), 1, mpfFile);
        fwrite(&mat_scene_blend_dest_a, sizeof(uint32), 1, mpfFile);

        uint32 mat_alpha_reject_func = (uint32)pass->getAlphaRejectFunction();
        uint8 mat_alpha_reject_val = (uint8)pass->getAlphaRejectValue();
        bool map_alpha_reject_atc = pass->isAlphaToCoverageEnabled();

        fwrite(&mat_alpha_reject_func, sizeof(uint32), 1, mpfFile);
        fwrite(&mat_alpha_reject_val, sizeof(uint8), 1, mpfFile);
        fwrite(&map_alpha_reject_atc, sizeof(bool), 1, mpfFile);

        Ogre::TextureUnitState * texunit = pass->getTextureUnitState(0);

        Ogre::TextureUnitState::UVWAddressingMode tex_address_mode_uvw = texunit->getTextureAddressingMode();

        uint32 tex_address_mode_u = (uint32)tex_address_mode_uvw.u;
        uint32 tex_address_mode_v = (uint32)tex_address_mode_uvw.v;
        uint32 tex_address_mode_w = (uint32)tex_address_mode_uvw.w;

        uint32 tex_filtering_min = (uint32)texunit->getTextureFiltering(Ogre::FT_MIN);
        uint32 tex_filtering_mag = (uint32)texunit->getTextureFiltering(Ogre::FT_MAG);
        Ogre::ColourValue tex_border_color = texunit->getTextureBorderColour();

        fwrite(&tex_address_mode_u,sizeof(uint32), 1, mpfFile);
        fwrite(&tex_address_mode_v,sizeof(uint32), 1, mpfFile);
        fwrite(&tex_address_mode_w,sizeof(uint32), 1, mpfFile);
        fwrite(&tex_filtering_min,sizeof(uint32), 1, mpfFile);
        fwrite(&tex_filtering_mag,sizeof(uint32), 1, mpfFile);
        fwrite(tex_border_color.ptr(),sizeof(float)*4, 1, mpfFile);

        uint32 colsize = (uint32)pFont->mColorList.size();
        fwrite(&colsize,sizeof(uint32), 1, mpfFile);

        for(int32 i = 0; i != colsize; ++i)
        {
            fwrite(pFont->mColorList[i].ptr(),sizeof(float)*4, 1, mpfFile);
        }

        for(uint32 i = 0; i != 256; ++i)
        {
            FontGlyph glyph = pFont->mGlyph[i];
            fwrite(&glyph, sizeof(FontGlyph), 1,mpfFile);
        }

        Ogre::Image * tex = pFont->mFontImage;

        size_t uWidth = tex->getWidth();
        size_t uHeight = tex->getHeight();
        size_t uDepth = tex->getDepth();
		size_t eFormat = (uint32)tex->getFormat();
		size_t numFaces = tex->getNumFaces();
		size_t numMipMaps = tex->getNumMipmaps();

		fwrite(&uWidth, sizeof(size_t), 1, mpfFile);
		fwrite(&uHeight, sizeof(size_t), 1, mpfFile);
		fwrite(&uDepth, sizeof(size_t), 1, mpfFile);
		fwrite(&eFormat, sizeof(size_t), 1, mpfFile);
		fwrite(&numFaces, sizeof(size_t), 1, mpfFile);
		fwrite(&numMipMaps, sizeof(size_t), 1, mpfFile);

		size_t texdatasize = Ogre::Image::calculateSize(numMipMaps,
                                                        numFaces,
                                                        uWidth,
                                                        uHeight,
                                                        uDepth,
                                                        (Ogre::PixelFormat)eFormat);

		fwrite(tex->getData(),texdatasize, 1,mpfFile);
    }
    // ----------------------------------------------------------------------
    void FontSerializer::importFont(Ogre::DataStreamPtr &stream, Font *pDest)
    {
        uint32 fccomp;
        stream->read(&fccomp, sizeof(uint32));
        if(fccomp != Font::mFourCC)
            SONETTO_THROW("Invalid Font File");
        stream->read(&pDest->mVersion, sizeof(uint32));
        stream->read(&pDest->mEncode, sizeof(uint32));
        stream->read(&pDest->mVerticalOffsetTop, sizeof(float));
        stream->read(&pDest->mVerticalOffsetBottom, sizeof(float));
        stream->read(&pDest->mHorizontalScale, sizeof(float));

        pDest->mIName = loadString(stream);

        // setup material
        pDest->mMaterial = Ogre::MaterialManager::getSingleton().create(pDest->mIName+"_mat",pDest->getGroup());

        if(pDest->mMaterial.isNull())
            SONETTO_THROW("Unable to create material for font.");

        Ogre::Pass * pass = pDest->mMaterial->getTechnique(0)->getPass(0);

        bool has_separate_blend;
        uint32 mat_scene_blend_source;
        uint32 mat_scene_blend_dest;
        uint32 mat_scene_blend_source_a;
        uint32 mat_scene_blend_dest_a;
        stream->read(&has_separate_blend, sizeof(bool));
        stream->read(&mat_scene_blend_source, sizeof(uint32));
        stream->read(&mat_scene_blend_dest, sizeof(uint32));
        stream->read(&mat_scene_blend_source_a, sizeof(uint32));
        stream->read(&mat_scene_blend_dest_a, sizeof(uint32));

        if(has_separate_blend)
        {
            pass->setSeparateSceneBlending( (Ogre::SceneBlendFactor)mat_scene_blend_source,
                                            (Ogre::SceneBlendFactor)mat_scene_blend_dest,
                                            (Ogre::SceneBlendFactor)mat_scene_blend_source_a,
                                            (Ogre::SceneBlendFactor)mat_scene_blend_dest_a);
        } else {
            pass->setSceneBlending((Ogre::SceneBlendFactor)mat_scene_blend_source,(Ogre::SceneBlendFactor)mat_scene_blend_dest);
        }
        uint32 mat_alpha_reject_func;
        uint8 mat_alpha_reject_val;
        bool map_alpha_reject_atc;
        stream->read(&mat_alpha_reject_func, sizeof(uint32));
        stream->read(&mat_alpha_reject_val, sizeof(uint8));
        stream->read(&map_alpha_reject_atc, sizeof(bool));

        pass->setAlphaRejectSettings((Ogre::CompareFunction) mat_alpha_reject_func, mat_alpha_reject_val, map_alpha_reject_atc);

        pass->setDepthCheckEnabled(false);
        pass->setDepthWriteEnabled(false);
        pass->setLightingEnabled(false);

        Ogre::TextureUnitState * tex_unit = pass->createTextureUnitState();

        uint32 tex_address_mode_u;
        uint32 tex_address_mode_v;
        uint32 tex_address_mode_w;
        uint32 tex_filtering_min;
        uint32 tex_filtering_mag;
        Ogre::ColourValue tex_border_color;
        stream->read(&tex_address_mode_u, sizeof(uint32));
        stream->read(&tex_address_mode_v, sizeof(uint32));
        stream->read(&tex_address_mode_w, sizeof(uint32));
        stream->read(&tex_filtering_min, sizeof(uint32));
        stream->read(&tex_filtering_mag, sizeof(uint32));
        stream->read(tex_border_color.ptr(), sizeof(float) * 4);

        tex_unit->setTextureAddressingMode( (Ogre::TextureUnitState::TextureAddressingMode) tex_address_mode_u,
                                            (Ogre::TextureUnitState::TextureAddressingMode) tex_address_mode_v,
                                            (Ogre::TextureUnitState::TextureAddressingMode) tex_address_mode_w);
        tex_unit->setTextureFiltering((Ogre::FilterOptions)tex_filtering_min, (Ogre::FilterOptions)tex_filtering_mag, Ogre::FO_NONE);
        tex_unit->setTextureBorderColour(tex_border_color);

        uint32 numcol = 0;
        stream->read(&numcol, sizeof(uint32));
        for(uint32 l = 0; l != numcol; ++l)
        {
            std::cout << "reading color #"<<l<<"...\n";
            Ogre::ColourValue color;
            stream->read(color.ptr(), sizeof(float) * 4);
            pDest->mColorList.push_back(color);
            std::cout << "color #"<<l<<" read complete...\n";
        }
        std::cout << "all color values have been read correctly...\n";
        std::cout << "reading font glyph list...\n";
        for(uint16 i = 0; i != 256; ++i)
        {
            std::cout << "reading glyph #"<<(int)i<<"...\n";
            FontGlyph glyph;
            stream->read(&glyph, sizeof(FontGlyph));
            pDest->mGlyph.push_back(glyph);
            std::cout << "glyph #"<<(int)i<<" read complete...\n";
        }
        std::cout << "all font glyph have been read correctly...\n";
        // size_t width, size_t height, size_t depth, size_t pixel format
        size_t txt_width, txt_height, txt_depth, txt_pixelformat, txt_faces, txt_mipmaps = 0;
        stream->read(&txt_width, sizeof(size_t));
        stream->read(&txt_height, sizeof(size_t));
        stream->read(&txt_depth, sizeof(size_t));
        stream->read(&txt_pixelformat, sizeof(size_t));
        stream->read(&txt_faces, sizeof(size_t));
        stream->read(&txt_mipmaps, sizeof(size_t));

        std::cout <<
        "Loading Font Texture Data...\n"
        "Texture Width: "<<txt_width<<"\n"
        "Texture Height: "<<txt_height<<"\n"
        "Texture Depth: "<<txt_depth<<"\n"
        "Texture Pixel Format: "<<txt_pixelformat<<"\n"
        "Texture Faces: "<<txt_faces<<"\n"
        "Texture Mipmaps: "<<txt_mipmaps<<"\n";

        pDest->mFontImage = new Ogre::Image();

        size_t totalimagesize = Ogre::Image::calculateSize(txt_mipmaps, txt_faces, txt_width, txt_height, txt_depth, (Ogre::PixelFormat)txt_pixelformat);

        std::cout << "Current position at file: "<<stream->tell()<<"\n"
        "Target Image Size: "<<totalimagesize<<"\n"
        "Remaining File Size: "<<stream->size()<<"\n";

        unsigned char * imgbuffer = new unsigned char [totalimagesize];

        stream->read(imgbuffer, totalimagesize);

        pDest->mFontImage->loadDynamicImage(imgbuffer, txt_width, txt_height, txt_depth, (Ogre::PixelFormat)txt_pixelformat, true, txt_faces, txt_mipmaps);

        Ogre::TexturePtr texture = Ogre::TextureManager::getSingleton().loadImage(pDest->mIName+"_tex",pDest->getGroup(), *pDest->mFontImage, Ogre::TEX_TYPE_2D, 0);

        tex_unit->setTextureName(texture->getName(), Ogre::TEX_TYPE_2D);
    }
    // ----------------------------------------------------------------------
    std::string FontSerializer::loadString(Ogre::DataStreamPtr& stream)
    {
        uint16 strsize;
        stream->read(&strsize, sizeof(uint16));
        char * stringbuffer = new char[strsize+1];
        stream->read((char*)stringbuffer, strsize);
        stringbuffer[strsize] = '\0';
        std::string rstring = stringbuffer;
        delete[] stringbuffer;
        return rstring;
    }
    // ----------------------------------------------------------------------
    void FontSerializer::saveString(const std::string &str)
    {
        uint16 strsize = str.size();
        fwrite(&strsize, sizeof(uint16), 1, mpfFile);
        fwrite(str.c_str(), strsize, 1, mpfFile);
    }
    // ----------------------------------------------------------------------
}; // namespace
