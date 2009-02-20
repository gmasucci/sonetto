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

#ifndef GENERICMAPMODULE_MAPSERIALIZER_H
#define GENERICMAPMODULE_MAPSERIALIZER_H

#include <OgreSerializer.h>
#include <OgreDataStream.h>
#include <OgreHardwareIndexBuffer.h>
#include "GenericMapPrerequisites.h"

namespace GenericMapModule
{
    class MapSerializer : public Ogre::Serializer
    {
    public:
        static const uint32 FourCC = MKFOURCC('S','M','A','P');

        enum Version
        {
            VER_0_1     = 1,
            VER_CURRENT = VER_0_1
        };

        enum VertexDeclarationElement
        {
            VDE_POSITION   = 1,
            VDE_NORMAL     = 2,
            VDE_DIFFUSE    = 4,
            VDE_SPECULAR   = 8,
            VDE_TEXCOORD_1 = 16,
            VDE_TEXCOORD_2 = 32,
            VDE_TEXCOORD_3 = 64,
            VDE_TEXCOORD_4 = 128,
            VDE_TEXCOORD_5 = 256,
            VDE_TEXCOORD_6 = 512,
            VDE_TEXCOORD_7 = 1024,
            VDE_TEXCOORD_8 = 2048
        };

        MapSerializer(Ogre::DataStreamPtr &stream)
                : mStream(stream) {}
        virtual ~MapSerializer() {}

        void importMapFile(Map *map);

    protected:
        Ogre::DataStreamPtr mStream;

        size_t mTotalBuffersSize;

        void importMapFile0_1(Map *map);

        void importSubMesh(Ogre::MeshPtr mesh,
            Ogre::HardwareIndexBufferSharedPtr sharedIndexBuffer,
            size_t *offset);

        void importVertexData(Ogre::VertexData *data);

        size_t importIndexBuffer(
            Ogre::HardwareIndexBufferSharedPtr indexBuffer,
            size_t *offset);
    };
} // namespace

#endif
