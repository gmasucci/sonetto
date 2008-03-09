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
#include "SonettoWindowSkinSerializer.h"

namespace Sonetto {
    const long STREAM_OVERHEAD_SIZE = sizeof(uint16) + sizeof(uint32);
    WindowSkinSerializer::WindowSkinSerializer() {
        // Version number
        mVersion = "SntSYS000";
    }
    WindowSkinSerializer::~WindowSkinSerializer() {

    }
    void WindowSkinSerializer::exportWindowSkin(const WindowSkin *pWindowSkin, const Ogre::String &fileName, Ogre::Serializer::Endian endianMode = ENDIAN_NATIVE) {
        // Decide on endian mode
		determineEndianness(endianMode);

		mpfFile = fopen(fileName.c_str(), "wb");
		if (!mpfFile)
		{
			OGRE_EXCEPT(Exception::ERR_CANNOT_WRITE_TO_FILE,
				"Unable to open file " + fileName + " for writing",
				"WindowSkinSerializer::exportWindowSkin");
		}

		writeFileHeader();

		writeString(mWindowMaskTextureName);
		writeString(mWindowBgTextureName);
		writeString(mBorderTextureName);

		writeTexCoords(mWinTexCoord);
		writeVector2(mBorderDimensions);
		writeBools(mTileMode, 1);
		writeVector2(mTiling);

		writeVector2(mBorderDimensions);

		writeColorValue(mMenuColor);
		writeColorValue(mMenuBorderColor);

		writeColorValue(mMainColor);
		writeColorValue(mMainBorderColor);

		writeColorValue(mEnemyColor);
		writeColorValue(mEnemyBorderColor);

		writeColorValue(mSystemColor);
		writeColorValue(mSystemBorderColor);

		writeColorValue(mMenuBgTopLeft);
		writeColorValue(mMenuBgTopRight);
		writeColorValue(mMenuBgBottomLeft);
		writeColorValue(mMenuBgBottomRight);

		writeColorValue(mCursorLineColor);
    }
    void WindowSkinSerializer::importWindowSkin(Ogre::DataStreamPtr &stream, WindowSkin *pDest) {
        // Determine endianness (must be the first thing we do!)
		determineEndianness(stream);

		// Check header
        readFileHeader(stream);

        readString(mWindowMaskTextureName);
		readString(mWindowBgTextureName);
		readString(mBorderTextureName);

		readTexCoords(stream, mWinTexCoord);
		readVector2(stream, mBorderDimensions);
		readBools(stream, mTileMode, 1);
		readVector2(stream, mTiling);

		readVector2(stream, mBorderDimensions);

		readColorValue(stream, mMenuColor);
		readColorValue(stream, mMenuBorderColor);

		readColorValue(stream, mMainColor);
		readColorValue(stream, mMainBorderColor);

		readColorValue(stream, mEnemyColor);
		readColorValue(stream, mEnemyBorderColor);

		readColorValue(stream, mSystemColor);
		readColorValue(stream, mSystemBorderColor);

		readColorValue(stream, mMenuBgTopLeft);
		readColorValue(stream, mMenuBgTopRight);
		readColorValue(stream, mMenuBgBottomLeft);
		readColorValue(stream, mMenuBgBottomRight);

		readColorValue(stream, mCursorLineColor);
    }
    void WindowSkinSerializer::writeTexCoords(const WindowTexCoord& vec) {
        writeFloats(*vec.slim_left.left, 1);
        writeFloats(*vec.slim_left.right, 1);
        writeFloats(*vec.slim_left.top, 1);
        writeFloats(*vec.slim_left.bottom, 1);

        writeFloats(*vec.slim_center.left, 1);
        writeFloats(*vec.slim_center.right, 1);
        writeFloats(*vec.slim_center.top, 1);
        writeFloats(*vec.slim_center.bottom, 1);

        writeFloats(*vec.slim_right.left, 1);
        writeFloats(*vec.slim_right.right, 1);
        writeFloats(*vec.slim_right.top, 1);
        writeFloats(*vec.slim_right.bottom, 1);

        writeFloats(*vec.top_left.left, 1);
        writeFloats(*vec.top_left.right, 1);
        writeFloats(*vec.top_left.top, 1);
        writeFloats(*vec.top_left.bottom, 1);

        writeFloats(*vec.top.left, 1);
        writeFloats(*vec.top.right, 1);
        writeFloats(*vec.top.top, 1);
        writeFloats(*vec.top.bottom, 1);

        writeFloats(*vec.top_right.left, 1);
        writeFloats(*vec.top_right.right, 1);
        writeFloats(*vec.top_right.top, 1);
        writeFloats(*vec.top_right.bottom, 1);

        writeFloats(*vec.left.left, 1);
        writeFloats(*vec.left.right, 1);
        writeFloats(*vec.left.top, 1);
        writeFloats(*vec.left.bottom, 1);

        writeFloats(*vec.center.left, 1);
        writeFloats(*vec.center.right, 1);
        writeFloats(*vec.center.top, 1);
        writeFloats(*vec.center.bottom, 1);

        writeFloats(*vec.right.left, 1);
        writeFloats(*vec.right.right, 1);
        writeFloats(*vec.right.top, 1);
        writeFloats(*vec.right.bottom, 1);

        writeFloats(*vec.bottom_left.left, 1);
        writeFloats(*vec.bottom_left.right, 1);
        writeFloats(*vec.bottom_left.top, 1);
        writeFloats(*vec.bottom_left.bottom, 1);

        writeFloats(*vec.bottom.left, 1);
        writeFloats(*vec.bottom.right, 1);
        writeFloats(*vec.bottom.top, 1);
        writeFloats(*vec.bottom.bottom, 1);

        writeFloats(*vec.bottom_right.left, 1);
        writeFloats(*vec.bottom_right.right, 1);
        writeFloats(*vec.bottom_right.top, 1);
        writeFloats(*vec.bottom_right.bottom, 1);

        writeFloats(*vec.tail_up.left, 1);
        writeFloats(*vec.tail_up.right, 1);
        writeFloats(*vec.tail_up.top, 1);
        writeFloats(*vec.tail_up.bottom, 1);

        writeFloats(*vec.tail_down.left, 1);
        writeFloats(*vec.tail_down.right, 1);
        writeFloats(*vec.tail_down.top, 1);
        writeFloats(*vec.tail_down.bottom, 1);
    }
    void WindowSkinSerializer::writeVector2(const Ogre::Vector2& vec) {
        writeFloats(vec.ptr(), 2);
    }
    void WindowSkinSerializer::writeColorValue(const Ogre::ColourValue& vec) {
        writeFloats(vec.ptr(), 3);
    }
    void WindowSkinSerializer::readTexCoords(DataStreamPtr &stream, WindowTexCoord& vec) {
        readFloats(stream, *vec.slim_left.left, 1);
        readFloats(stream, *vec.slim_left.right, 1);
        readFloats(stream, *vec.slim_left.top, 1);
        readFloats(stream, *vec.slim_left.bottom, 1);

        readFloats(stream, *vec.slim_center.left, 1);
        readFloats(stream, *vec.slim_center.right, 1);
        readFloats(stream, *vec.slim_center.top, 1);
        readFloats(stream, *vec.slim_center.bottom, 1);

        readFloats(stream, *vec.slim_right.left, 1);
        readFloats(stream, *vec.slim_right.right, 1);
        readFloats(stream, *vec.slim_right.top, 1);
        readFloats(stream, *vec.slim_right.bottom, 1);

        readFloats(stream, *vec.top_left.left, 1);
        readFloats(stream, *vec.top_left.right, 1);
        readFloats(stream, *vec.top_left.top, 1);
        readFloats(stream, *vec.top_left.bottom, 1);

        readFloats(stream, *vec.top.left, 1);
        readFloats(stream, *vec.top.right, 1);
        readFloats(stream, *vec.top.top, 1);
        readFloats(stream, *vec.top.bottom, 1);

        readFloats(stream, *vec.top_right.left, 1);
        readFloats(stream, *vec.top_right.right, 1);
        readFloats(stream, *vec.top_right.top, 1);
        readFloats(stream, *vec.top_right.bottom, 1);

        readFloats(stream, *vec.left.left, 1);
        readFloats(stream, *vec.left.right, 1);
        readFloats(stream, *vec.left.top, 1);
        readFloats(stream, *vec.left.bottom, 1);

        readFloats(stream, *vec.center.left, 1);
        readFloats(stream, *vec.center.right, 1);
        readFloats(stream, *vec.center.top, 1);
        readFloats(stream, *vec.center.bottom, 1);

        readFloats(stream, *vec.right.left, 1);
        readFloats(stream, *vec.right.right, 1);
        readFloats(stream, *vec.right.top, 1);
        readFloats(stream, *vec.right.bottom, 1);

        readFloats(stream, *vec.bottom_left.left, 1);
        readFloats(stream, *vec.bottom_left.right, 1);
        readFloats(stream, *vec.bottom_left.top, 1);
        readFloats(stream, *vec.bottom_left.bottom, 1);

        readFloats(stream, *vec.bottom.left, 1);
        readFloats(stream, *vec.bottom.right, 1);
        readFloats(stream, *vec.bottom.top, 1);
        readFloats(stream, *vec.bottom.bottom, 1);

        readFloats(stream, *vec.bottom_right.left, 1);
        readFloats(stream, *vec.bottom_right.right, 1);
        readFloats(stream, *vec.bottom_right.top, 1);
        readFloats(stream, *vec.bottom_right.bottom, 1);

        readFloats(stream, *vec.tail_up.left, 1);
        readFloats(stream, *vec.tail_up.right, 1);
        readFloats(stream, *vec.tail_up.top, 1);
        readFloats(stream, *vec.tail_up.bottom, 1);

        readFloats(stream, *vec.tail_down.left, 1);
        readFloats(stream, *vec.tail_down.right, 1);
        readFloats(stream, *vec.tail_down.top, 1);
        readFloats(stream, *vec.tail_down.bottom, 1);
    }
    void WindowSkinSerializer::readVector2(DataStreamPtr &stream, Ogre::Vector2 &pDest) {
        readFloats(stream, pDest.ptr(), 2);
    }
    void WindowSkinSerializer::readColorValue(DataStreamPtr &stream, Ogre::ColourValue &pDest) {
        readFloats(stream, pDest.ptr(), 3);
    }
}; // namespace
