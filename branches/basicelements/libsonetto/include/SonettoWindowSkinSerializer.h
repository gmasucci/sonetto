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

#ifndef SONETTO_WindowSkinSerializer_H
#define SONETTO_WindowSkinSerializer_H

#include <OgreSerializer.h>
#include "SonettoPrerequisites.h"

namespace Sonetto
{
    struct MaterialPass
    {
        // Material Flags:
        // 0x1: Use Separate Blending
        // 0x2: Use Alpha to Coverage (Alpha Rejection)
        // 0x4,   0x8,   0x10,  0x20,  0x40,   0x80: Unused
        // 0x100, 0x200, 0x400, 0x800, 0x1000, 0x2000, 0x4000, 0x8000: Unused
        uint32 flags0;
        uint32 sceneBlendSource;
        uint32 sceneBlendDest;
        uint32 sceneBlendAlphaSource;
        uint32 sceneBlendAlphaDest;
        uint32 alphaRejectFunction;
        uint32 alphaRejectValue;
        uint32 numTextureUnit; // Number of Texture Units in this material
    };
    struct MaterialTextureUnit
    {
        uint32 textureType; // Texture Type
        uint32 configFlags; // 0x1 Is Alpha, 0x2 Use Extended Color Operation
        uint32 textureCoordSet; // Set the texcoord id
        // Color Opration
        uint32 colorOperation;
        uint32 colorOperationEx;
        uint32 colorOpExSrc1;
        uint32 colorOpExSrc2;
        float colorOpColorArgR1;
        float colorOpColorArgG1;
        float colorOpColorArgB1;
        float colorOpColorArgA1;
        float colorOpColorArgR2;
        float colorOpColorArgG2;
        float colorOpColorArgB2;
        float colorOpColorArgA2;
        float colorOpManualBlend;
        // Alpha Operation
        uint32 alphaOperationEx;
        uint32 alphaOpExSrc1;
        uint32 alphaOpExSrc2;
        float alphaOpAlphaArg1;
        float alphaOpAlphaArg2;
        float alphaOpManualBlend;
        // Texture Filtering Options
        uint32 textureFilteringMin;
        uint32 textureFilteringMag;
    };

    class WindowSkin;

	class SONETTO_API WindowSkinSerializer : Ogre::Serializer
	{
	public:
		WindowSkinSerializer() {}
		virtual ~WindowSkinSerializer() {}

		void exportWindowSkin(const WindowSkin *pWindowSkin, const Ogre::String &fileName);
        void importWindowSkin(Ogre::DataStreamPtr &stream, WindowSkin *pDest);

	};
} // namespace

#endif // SONETTO_WindowSkinSerializer_H
