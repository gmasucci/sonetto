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
#include "SonettoFontSerializer.h"
#include "SonettoFont.h"

namespace Sonetto
{
    using namespace Ogre;
    const long STREAM_OVERHEAD_SIZE = sizeof(uint16) + sizeof(uint32);
    FontSerializer::FontSerializer()
    {
        // Version number
        mVersion = "SntFNT000";
    }

    FontSerializer::~FontSerializer()
    {
    }

    void FontSerializer::exportFont(const Font *pFont, const Ogre::String &fileName, Ogre::Serializer::Endian endianMode)
    {
        // Decide on endian mode
		determineEndianness(endianMode);

		mpfFile = fopen(fileName.c_str(), "wb");
		if (!mpfFile)
		{
			OGRE_EXCEPT(Exception::ERR_CANNOT_WRITE_TO_FILE,
				"Unable to open file " + fileName + " for writing",
				"FontSerializer::exportFont");
		}

        writeFileHeader();

        writeMain(pFont);

        for (size_t i = 0; i < pFont->glyphs.size()-1; ++i)
        {
            writeGlyph(pFont, i);
        }

        for (size_t i = 0; i < pFont->mColorList.size()-1; ++i)
        {
            writeColor(pFont, i);
        }

        fclose(mpfFile);
    }

    void FontSerializer::importFont(Ogre::DataStreamPtr &stream, Font *pDest)
    {
        // Determine endianness (must be the first thing we do!)
		determineEndianness(stream);

		// Check header
        readFileHeader(stream);


        unsigned short streamID;
        while(!stream->eof())
        {
            streamID = readChunk(stream);
            switch (streamID)
            {
                case FONT_MAIN:
                    readMain(stream, pDest);
                break;
                case FONT_GLYPH:
                    readGlyph(stream, pDest);
                break;
                case FONT_COLOR:
                    readColor(stream, pDest);
                break;
            }
        }
    }
    void FontSerializer::readMain(DataStreamPtr& stream, Font* pFont)
    {

        readInts(stream, &pFont->glyph_tbl.x,1);
        readInts(stream, &pFont->glyph_tbl.y,1);

        readFloats(stream, &pFont->default_y_space,1);

        readInts(stream, &pFont->num_glyphs,1);

        readFloats(stream, &pFont->glyph_coord_offset.x,1);
        readFloats(stream, &pFont->glyph_coord_offset.y,1);

        pFont->font_texture_name = readString(stream);

        std::cout<<"Texture: "<<pFont->font_texture_name.c_str()<<"\n";

        readShorts(stream, &pFont->font_blend_mode,1);

        readShorts(stream, &pFont->font_alpha_reject_settings,1);

        readShorts(stream, &pFont->alpha_rejection, 1);

        readShorts(stream, &pFont->font_filtering_mode,1);
    }
    void FontSerializer::readGlyph(Ogre::DataStreamPtr& stream, Font* pFont)
    {

        GlyphStruct tmp_glyph;
        // Read the texcoord0.x
        readFloats(stream, &tmp_glyph.texcoord0.x,1);
        // Read the texcoord0.y
        readFloats(stream, &tmp_glyph.texcoord0.y,1);

        // Read the texcoord1.x
        readFloats(stream, &tmp_glyph.texcoord1.x,1);
        // Read the texcoord1.y
        readFloats(stream, &tmp_glyph.texcoord1.y,1);

        // Read the texcoord2.x
        readFloats(stream, &tmp_glyph.texcoord2.x,1);
        // Read the texcoord2.y
        readFloats(stream, &tmp_glyph.texcoord2.y,1);

        // Read the texcoord3.x
        readFloats(stream, &tmp_glyph.texcoord3.x,1);
        // Read the texcoord3.y
        readFloats(stream, &tmp_glyph.texcoord3.y,1);

        // Read the x_offset
        readFloats(stream, &tmp_glyph.x_offset,1);

        // Add the glyph to the font glyph vector
        pFont->glyphs.push_back(tmp_glyph);
    }
    void FontSerializer::readColor(Ogre::DataStreamPtr& stream, Font* pFont)
    {
        Ogre::ColourValue tmp_color;

        readFloats(stream, &tmp_color.r,1);
        readFloats(stream, &tmp_color.g,1);
        readFloats(stream, &tmp_color.b,1);
        readFloats(stream, &tmp_color.a,1);
        // Add the glyph to the font glyph vector
        pFont->mColorList.push_back(tmp_color);
    }
    void FontSerializer::writeMain(const Font* pFont)
    {
        writeChunkHeader(FONT_MAIN, calcMainSize(pFont));

        writeInts(&pFont->glyph_tbl.x,1);
        writeInts(&pFont->glyph_tbl.y,1);

        writeFloats(&pFont->default_y_space,1);

        writeInts(&pFont->num_glyphs,1);

        writeFloats(&pFont->glyph_coord_offset.x,1);
        writeFloats(&pFont->glyph_coord_offset.y,1);

        writeString(pFont->font_texture_name);

        writeShorts(&pFont->font_blend_mode,1);

        writeShorts(&pFont->font_alpha_reject_settings,1);

        writeShorts(&pFont->alpha_rejection,1);

        writeShorts(&pFont->font_filtering_mode,1);

    }
    void FontSerializer::writeGlyph(const Font* pFont, size_t glyph_id)
    {
        writeChunkHeader(FONT_GLYPH, calcGlyphSize(pFont));

        writeFloats(&pFont->glyphs[glyph_id].texcoord0.x,1);
        writeFloats(&pFont->glyphs[glyph_id].texcoord0.y,1);

        writeFloats(&pFont->glyphs[glyph_id].texcoord1.x,1);
        writeFloats(&pFont->glyphs[glyph_id].texcoord1.y,1);

        writeFloats(&pFont->glyphs[glyph_id].texcoord2.x,1);
        writeFloats(&pFont->glyphs[glyph_id].texcoord2.y,1);

        writeFloats(&pFont->glyphs[glyph_id].texcoord3.x,1);
        writeFloats(&pFont->glyphs[glyph_id].texcoord3.y,1);

        writeFloats(&pFont->glyphs[glyph_id].x_offset,1);
    }
    void FontSerializer::writeColor(const Font* pFont, size_t color_id)
    {
        writeChunkHeader(FONT_COLOR, calcColorSize(pFont));
        writeFloats(&pFont->mColorList[color_id].r,1);
        writeFloats(&pFont->mColorList[color_id].g,1);
        writeFloats(&pFont->mColorList[color_id].b,1);
        writeFloats(&pFont->mColorList[color_id].a,1);
    }
    size_t FontSerializer::calcMainSize(const Font * pFont)
    {
        size_t size = STREAM_OVERHEAD_SIZE;

        // TODO: Finish this
        return size;
    }
    size_t FontSerializer::calcGlyphSize(const Font * pFont)
    {
        size_t size = STREAM_OVERHEAD_SIZE;

        size += sizeof(float) * 9;

        return size;
    }
    size_t FontSerializer::calcColorSize(const Font * pFont)
    {
        size_t size = STREAM_OVERHEAD_SIZE;

        size += sizeof(float) * 4;

        return size;
    }
} // namespace
