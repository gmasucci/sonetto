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

#ifndef SONETTO_MAIN_H_
#define SONETTO_MAIN_H_

#include <al.h>
#include <vorbis/vorbisfile.h>

namespace Sonetto {
    enum ScreenMetricsMode {
        SMM_RELATIVE,
        SMM_RELATIVE_ASPECT_ADJUSTED
    };

    struct TexCoord {
        TexCoord():left(0.0f),right(0.0f),top(0.0f),bottom(0.0f) { }
        float left;
        float right;
        float top;
        float bottom;
    };

    struct WindowTexCoord {
        TexCoord slim_left;
        TexCoord slim_center;
        TexCoord slim_right;
        TexCoord top_left;
        TexCoord top;
        TexCoord top_right;
        TexCoord left;
        TexCoord center;
        TexCoord right;
        TexCoord bottom_left;
        TexCoord bottom;
        TexCoord bottom_right;
        TexCoord tail_up;
        TexCoord tail_down;
    };

    /** @brief Structure used to store glyph data.
     *
     * This structure is used to store the texture coordinates and horizontal offset for each character.
     */
    struct GlyphStruct {
        /// This is the top-left texture UV coordinate for this glyph.
        Ogre::Vector2 texcoord0;
        /// This is the top-right texture UV coordinate for this glyph.
        Ogre::Vector2 texcoord1;
        /// This is the bottom-left texture UV coordinate for this glyph.
        Ogre::Vector2 texcoord2;
        /// This is the bottom-right texture UV coordinate for this glyph.
        Ogre::Vector2 texcoord3;
        /// This is the horizontal offset, the horizontal size for this glyph.
        float x_offset;
    };

    /** @brief Stores the number of glyphs in the font texture.
     *
     * This structure is used to store the texture coordinates and horizontal offset for each character.
     */
    struct GlyphTable {
        /** @brief Default constructor.
        * @param nx is the number of horizontal glyphs in the texture.
        * @param ny is the number of vertical glyphs in the texture.
        */
        GlyphTable(unsigned long nx = 0, unsigned long ny = 0) : x(nx),y(ny) {}
        /// Number of horizontal glyphs in the texture.
        Ogre::uint32 x;
        /// Number of vertical glyphs in the texture.
        Ogre::uint32 y;
    };

    /** @brief ???.
     *
     * Need to redo this, ignore.
     */
    struct GlyphHeader {
        /// Default Constructor.
        GlyphHeader() {}
        /// Number of glyphs per line and number of lines.
        GlyphTable glyph_tbl;
        /// Default Vertical Space for each new line.
        float default_y_space;
        /// Total number of Glyphs
        Ogre::uint32 num_glyphs;
        // Offset value for Glyph Mapping
        Ogre::Vector2 glyph_coord_offset;
    };

    /** @brief Store the color change events in text (deprecated).
     *
     * Need to remove this, ignore.
     */
    struct ColorTextEvent {
        size_t flag_position;
        size_t color_id;
    };

    enum FontChunkHeader
    {
        FONT_MAIN = 0xF0AA,
        FONT_GLYPH = 0xF0BB,
        FONT_COLOR = 0xF0FF
    };

    enum FontBlendMode
    {
        FBM_TRANSPARENT_ALPHA = 0x1010,
        FBM_TRANSPARENT_COLOR = 0x2010,
        FBM_ADD = 0x2020,
        FBM_MODULATE  = 0x20A0,
        FBM_REPLACE  = 0x3000
    };

    enum CompareFunction
    {
        SCF_ALWAYS_FAIL = 0x1000,
        SCF_ALWAYS_PASS = 0x10A0,
        SCF_LESS = 0x20A0,
        SCF_LESS_EQUAL = 0x20B0,
        SCF_EQUAL = 0x40A0,
        SCF_NOT_EQUAL = 0x40F0,
        SCF_GREATER_EQUAL = 0xA0A0,
        SCF_GREATER = 0xA0F0
    };

    enum FilterType
    {
        FT_NONE = 0x0000,
        FT_POINT = 0x1111,
        FT_LINEAR = 0x2222,
        FT_ANISOTROPIC = 0x3333
    };

    struct MusicInfo {
        std::string  filename;

        bool         loopEnabled;
        ogg_uint64_t loopBegin;
        ogg_uint64_t loopEnd;
    };

    struct MusicStream {
        size_t         musicID;

        ALuint         sourceID;
        OggVorbis_File oggStream;
        int            bitstream;
        char           activeBuffer;
        ALuint         bufferIDs[2];

        float          maxVolume;
        char           fade;      // -1 = Fade out, 0 = No Fading, +1 = Fade in
        float          fadeSpd;
    };

    struct SoundInfo {
        ALuint       bufferID;
        bool         loopEnabled;
        ogg_uint64_t loopBegin;
        ogg_uint64_t loopEnd;

        float        volume;
        char         fade;         // -1 = Fade out, 0 = No Fading, +1 = Fade in
        float        fadeSpd;
    };

    struct SoundSource {
        ALuint           soundID;
        ALuint           sourceID;

        Ogre::SceneNode *parentNode;
    };
}
; // namespace

#endif // SONETTO_MAIN_H_
