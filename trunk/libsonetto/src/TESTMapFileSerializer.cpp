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
#include "TESTMapFileSerializer.h"
#include "TESTMapFile.h"

namespace Sonetto{
    const long STREAM_OVERHEAD_SIZE = sizeof(Ogre::uint16) + sizeof(Ogre::uint32);
    MapFileSerializer::MapFileSerializer()
    {
        mVersion = "MapData0";
    }
    MapFileSerializer::~MapFileSerializer()
    {

    }
    void MapFileSerializer::exportMapFile(const MapFile *pDest, const Ogre::String &fileName, Endian endianMode)
    {
        determineEndianness(endianMode);
        mpfFile = fopen(fileName.c_str(), "wb");
		if (!mpfFile)
		{
		    std::cout<<"Cannot open file.\n";
		}
        writeFileHeader();
        writeString(pDest->mMapName);
        writeFloats(pDest->mAmbientColor.ptr(),4);
        writeFloats(pDest->mBackgroundColor.ptr(),4);
        writeBools(&pDest->mUseLight0,1);
        writeBools(&pDest->mUseLight1,1);
        writeObject(pDest->mLightDirection0);
        writeObject(pDest->mLightDirection1);
        writeFloats(pDest->mLightColor0.ptr(),4);
        writeFloats(pDest->mLightColor1.ptr(),4);
        writeFloats(pDest->mFogColor.ptr(),4);
        writeFloats(&pDest->mFogExpDensity,1);
        writeFloats(&pDest->mFogStart,1);
        writeFloats(&pDest->mFogEnd,1);
        writeInts(&pDest->mNumMapLayers,1);
        for( Ogre::uint32 start = 0; start < pDest->mNumMapLayers; ++start )
        {
            writeMapLayer(pDest->mMapLayer[start]);
        }
        fclose(mpfFile);
    }
    void MapFileSerializer::importMapFile(Ogre::DataStreamPtr &stream, MapFile *pDest)
    {
        determineEndianness(stream);
        readFileHeader(stream);
        pDest->mMapName = readString(stream);
        readFloats(stream,pDest->mAmbientColor.ptr(),4);
        readFloats(stream,pDest->mBackgroundColor.ptr(),4);
        readBools(stream,&pDest->mUseLight0,1);
        readBools(stream,&pDest->mUseLight1,1);
        readObject(stream,pDest->mLightDirection0);
        readObject(stream,pDest->mLightDirection1);
        readFloats(stream,pDest->mLightColor0.ptr(),4);
        readFloats(stream,pDest->mLightColor1.ptr(),4);
        readFloats(stream,pDest->mFogColor.ptr(),4);
        readFloats(stream,&pDest->mFogExpDensity,1);
        readFloats(stream,&pDest->mFogStart,1);
        readFloats(stream,&pDest->mFogEnd,1);
        readInts(stream,&pDest->mNumMapLayers,1);
        for( Ogre::uint32 start = 0; start < pDest->mNumMapLayers; ++start )
        {
            MapLayerFile tmpMapLayer;
            readMapLayer(stream, tmpMapLayer);
            pDest->mMapLayer.push_back(tmpMapLayer);
        }
    }
    void MapFileSerializer::writeMapLayer(const MapLayerFile &maplayer)
    {
        writeString(maplayer.mLayerName);
        writeString(maplayer.mParentLayerName);
        writeString(maplayer.mLayerModelName);
        writeObject(maplayer.mLayerPosition);
        writeObject(maplayer.mLayerOrientation);
        writeShorts(&maplayer.mLayerSettingsFlags,1);
        writeString(maplayer.mAnimationName);
        writeObject(maplayer.mAnimRotation);
        writeFloats(&maplayer.mAnimationSpeed,1);
        writeString(maplayer.mMaterialName);
    }
    void MapFileSerializer::readMapLayer(Ogre::DataStreamPtr& stream,MapLayerFile &pDest)
    {
        pDest.mLayerName = readString(stream);
        pDest.mParentLayerName = readString(stream);
        pDest.mLayerModelName = readString(stream);
        readObject(stream,pDest.mLayerPosition);
        readObject(stream,pDest.mLayerOrientation);
        readShorts(stream,&pDest.mLayerSettingsFlags,1);
        pDest.mAnimationName = readString(stream);
        readObject(stream,pDest.mAnimRotation);
        readFloats(stream,&pDest.mAnimationSpeed,1);
        pDest.mMaterialName = readString(stream);
    }
}; //namespace
