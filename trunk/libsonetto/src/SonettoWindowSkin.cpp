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
    //-----------------------------------------------------------------------------
    // Sonetto::WindowSkin implementation
    //-----------------------------------------------------------------------------
    WindowSkin::WindowSkin(Ogre::ResourceManager* creator, const Ogre::String &name,
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
        createParamDictionary("WindowSkin");
    }
    //-----------------------------------------------------------------------------
    WindowSkin::~WindowSkin()
    {
        unload();
    }
    //-----------------------------------------------------------------------------
    void WindowSkin::loadImpl()
    {
        WindowSkinSerializer serializer;
        Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(mName, mGroup, true, this);
        serializer.importWindowSkin(stream, this);
        deleteMaterial();
        createMaterial();
    }
    //-----------------------------------------------------------------------------
    void WindowSkin::unloadImpl()
    {
    }
    //-----------------------------------------------------------------------------
    size_t WindowSkin::calculateSize() const
    {
        return 0;
    }
    //-----------------------------------------------------------------------------
    bool WindowSkin::createMaterial()
    {

        if(!mWindowMaterial.isNull() || !mBorderMaterial.isNull())
            return false;

        // Create the Window Material
        mWindowMaterial = Ogre::MaterialManager::getSingleton().create(mName+"_window", mGroup);
        // Now, create the Window Border Material
        mBorderMaterial = Ogre::MaterialManager::getSingleton().create(mName+"_border", mGroup);

        mCursorMaterial = Ogre::MaterialManager::getSingleton().create(mName+"_cursor", mGroup);

        mCursorLineMaterial = Ogre::MaterialManager::getSingleton().create(mName+"_cursor_line", mGroup);



        if(mWindowMaterial.isNull() || mBorderMaterial.isNull())
        {
            SONETTO_THROW("Error creating window skin");
        }



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
        winMaskTexture->setTextureFiltering(Ogre::FO_LINEAR, Ogre::FO_LINEAR, Ogre::FO_NONE);
        Ogre::TextureUnitState * winBgTexture = winMatPass->createTextureUnitState(mWindowBgTextureName);
        winBgTexture->setTextureCoordSet(1);
        winBgTexture->setTextureFiltering(Ogre::FO_LINEAR, Ogre::FO_LINEAR, Ogre::FO_NONE);
        winBgTexture->setTextureAddressingMode(Ogre::TextureUnitState::TAM_WRAP);

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
        borderMaskTexture->setTextureFiltering(Ogre::FO_LINEAR, Ogre::FO_LINEAR, Ogre::FO_NONE);

        // Cursor Material
        // Setup the material basics (Non-configurable Mandatory Data)
        mCursorMaterial->setLightingEnabled(false);
        mCursorMaterial->setDepthCheckEnabled(false);
        mCursorMaterial->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
        // We'll use the base technique for now, no shader support.
        Ogre::Technique * cursorMatTech = mCursorMaterial->getTechnique(0);
        Ogre::Pass * cursorMatPass = cursorMatTech->getPass(0);
        // Create the texture unit for mask and background textures
        Ogre::TextureUnitState * cursorTexture = cursorMatPass->createTextureUnitState(mCursorTextureName);
        cursorTexture->setTextureCoordSet(0);
        cursorTexture->setTextureFiltering(Ogre::FO_LINEAR, Ogre::FO_LINEAR, Ogre::FO_NONE);

        // Cursor Line Material
        // Setup the material basics (Non-configurable Mandatory Data)
        mCursorLineMaterial->setLightingEnabled(false);
        mCursorLineMaterial->setDepthCheckEnabled(false);
        mCursorLineMaterial->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
        // We'll use the base technique for now, no shader support.
        Ogre::Technique * cursorLineMatTech = mCursorLineMaterial->getTechnique(0);
        Ogre::Pass * cursorLineMatPass = cursorLineMatTech->getPass(0);
        // Create the texture unit for mask and background textures
        Ogre::TextureUnitState * cursorLineTexture = cursorLineMatPass->createTextureUnitState(mCursorLineTextureName);
        cursorLineTexture->setTextureCoordSet(0);
        cursorLineTexture->setTextureFiltering(Ogre::FO_LINEAR, Ogre::FO_LINEAR, Ogre::FO_NONE);

        return true;
    }
    //-----------------------------------------------------------------------------
    bool WindowSkin::deleteMaterial()
    {
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
    //-----------------------------------------------------------------------------
}; // namespace
