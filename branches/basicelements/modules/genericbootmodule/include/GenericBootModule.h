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

#ifndef BOOTMODULE_GENERICBOOTMODULE_H
#define BOOTMODULE_GENERICBOOTMODULE_H

#include "SonettoBootModule.h"
#include "SonettoFont.h"
#include "SonettoPrerequisites.h"

namespace BootModule
{
    class GenericBootModule : public Sonetto::BootModule
    {
    public:
        GenericBootModule();
        ~GenericBootModule();

        void initialize();
        void update();
        void deinitialize();

        void halt();
        void resume();
    };

    // Temporary ----------------------------------------------------
    class ManualFontLoader  : public Ogre::ManualResourceLoader
    {
    public:
        ManualFontLoader(){}
        virtual ~ManualFontLoader(){}

        void loadResource(Ogre::Resource *resource)
        {
            Sonetto::Font * font = static_cast<Sonetto::Font *>(resource);
            font->mIName = "baar_sophia";
            font->mVersion = Sonetto::SFV_VER_1_0;
            font->mEncode = Sonetto::FE_ASCII;
            font->mVerticalOffsetTop = 24.0f / 128.0f;
            font->mVerticalOffsetBottom = 103.0f / 128.0f;
            font->mHorizontalScale = 1.0f;

            font->mColorList.push_back(Ogre::ColourValue(1.0f,1.0f,1.0f,1.0f));
            font->mColorList.push_back(Ogre::ColourValue(1.0f,0.0f,0.0f,1.0f));
            font->mColorList.push_back(Ogre::ColourValue(0.0f,1.0f,0.0f,1.0f));
            font->mColorList.push_back(Ogre::ColourValue(0.0f,0.0f,1.0f,1.0f));

            generateFontTexCoords(font);

            Ogre::ResourceGroupManager * resourcegroup = Ogre::ResourceGroupManager::getSingletonPtr();
            resourcegroup->createResourceGroup("TEMP_FONT_GENERATION_GROUP");
            resourcegroup->addResourceLocation("temp","FileSystem","TEMP_FONT_GENERATION_GROUP");
            resourcegroup->declareResource("baar_sophia.material","Material","TEMP_FONT_GENERATION_GROUP");
            resourcegroup->declareResource("baarsophia1024.dds","Texture","TEMP_FONT_GENERATION_GROUP");
            resourcegroup->initialiseResourceGroup("TEMP_FONT_GENERATION_GROUP");
            resourcegroup->loadResourceGroup("TEMP_FONT_GENERATION_GROUP");

            std::cout<<"Parsing material should start now\n";
            Ogre::MaterialManager::getSingleton().load("baar_sophia.material","TEMP_FONT_GENERATION_GROUP");
            std::cout<<"Parsing should be finished by now, or not\n";

            font->mMaterial = Ogre::MaterialManager::getSingleton().getByName("baar_sophia");
            font->mFontImage = new Ogre::Image();
            font->mFontImage->load("baarsophia1024.dds", "TEMP_FONT_GENERATION_GROUP");
        }
        void generateFontTexCoords(Sonetto::Font * font)
        {
            float glyphcoord = 1.0f / 16.0f;

            for(size_t i = 0; i != 256; ++i)
            {
                Sonetto::FontGlyph glyph;
                float texcoord_h_0 = glyphcoord * (i % 16);
                float texcoord_h_1 = glyphcoord * ((i % 16) + 1);
                float texcoord_v_0 = glyphcoord * (i / 16);
                float texcoord_v_1 = glyphcoord * ((i / 16) + 1);
                glyph.mTopLeft = Ogre::Vector2(texcoord_h_0,texcoord_v_0);
                glyph.mTopRight = Ogre::Vector2(texcoord_h_1,texcoord_v_0);
                glyph.mBottomLeft = Ogre::Vector2(texcoord_h_0,texcoord_v_1);
                glyph.mBottomRight = Ogre::Vector2(texcoord_h_1,texcoord_v_1);

                glyph.mLeftOffset = 4.0f / 128.0f;
                glyph.mRightOffset = 66.0f / 128.0f;

                font->mGlyph.push_back(glyph);
            }
        }
    };
    // Temporary ----------------------------------------------------


} // namespace

#endif
