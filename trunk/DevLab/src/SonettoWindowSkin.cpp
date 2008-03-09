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
#include "SonettoWindowSkin.h"
#include "SonettoWindowSkinSerializer.h"

namespace Sonetto {

    void WindowSkin::loadImpl() {
        WindowSkinSerializer serializer;
        Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(mName, mGroup, true, this);
        serializer.importWindowSkin(stream, this);
        deleteMaterial();
        createMaterial();
    }

    void WindowSkin::unloadImpl() {

    }

    size_t WindowSkin::calculateSize() const
    {
        return 0;
    }

    bool WindowSkin::createMaterial() {

        if(!mWindowMaterial.isNull() || !mBorderMaterial.isNull())
            return false;

        // Create the Window Material
        mWindowMaterial = Ogre::MaterialManager::getSingleton().create(mName+"_window", mGroup);
        // Now, create the Window Border Material
        mBorderMaterial = Ogre::MaterialManager::getSingleton().create(mName+"_border", mGroup);

        if(mWindowMaterial.isNull() || mBorderMaterial.isNull())
            return false;

        // Window Material
        // Setup the material basics (Non-configurable Mandatory Data)
        mWindowMaterial->setLightingEnabled(false);
        mWindowMaterial->setDepthCheckEnabled(false);
        mWindowMaterial->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);

        // We'll use the base technique for now, no shader support.
        Ogre::Technique * winMatTech = mWindowMaterial->getTechnique(0);
        Ogre::Pass * winMatPass = winMatTech->getPass(0);
        // Create the texture unit for mask and background textures
        Ogre::TextureUnitState * winMaskTexture = winMatPass->createTextureUnitState(mWindowMaskTextureName);
        winMaskTexture->setTextureCoordSet(0);
        winMaskTextute->setTextureFiltering(Ogre::FO_LINEAR, Ogre::FO_LINEAR, Ogre::FO_NONE);
        Ogre::TextureUnitState * winBgTexture = winMatPass->createTextureUnitState(mWindowBgTextureName);
        winBgTexture->setTextureCoordSet(1);
        winBgTexture->setTextureFiltering(Ogre::FO_LINEAR, Ogre::FO_LINEAR, Ogre::FO_NONE);
        winBgTexture->setTextureAddressingMode(Ogre::TAM_WRAP);

        // Border Material
        // Setup the material basics (Non-configurable Mandatory Data)
        mBorderMaterial->setLightingEnabled(false);
        mBorderMaterial->setDepthCheckEnabled(false);
        mBorderMaterial->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
        // We'll use the base technique for now, no shader support.
        Ogre::Technique * borderMatTech = mBorderMaterial->getTechnique(0);
        Ogre::Pass * borderMatPass = borderMatTech->getPass(0);
        // Create the texture unit for mask and background textures
        Ogre::TextureUnitState * borderMaskTexture = borderMatPass->createTextureUnitState(mBorderTextureName);
        borderMaskTexture->setTextureCoordSet(0);
        borderMaskTextute->setTextureFiltering(Ogre::FO_LINEAR, Ogre::FO_LINEAR, Ogre::FO_NONE);
    }

    bool WindowSkin::deleteMaterial() {
        if(!mWindowMaterial.isNull())
        {
            Ogre::MaterialManager::getSingleton().remove( mWindowMaterial->getHandle() );
            mWindowMaterial.setNull();
        }
        if(!mBorderMaterial.isNull())
        {
            Ogre::MaterialManager::getSingleton().remove( mBorderMaterial->getHandle() );
            mBorderMaterial.setNull();
        }
        return true;
    }

}; // namespace
