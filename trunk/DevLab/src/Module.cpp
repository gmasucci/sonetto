/*-----------------------------------------------------------------------------
This source file is part of Sonetto RPG Engine.

Copyright (C) 2007,2008 Arthur Carvalho de Souza Lima, Guilherme PrÃ¡ Vieira


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

#include <Ogre.h>
#include "Module.h"

namespace Sonetto {
    void Module::enter() {
        mKernel->mWindow->removeAllViewports();
        // Create the scene manager for this module.
        mSceneMan = mKernel->mRoot->createSceneManager(Ogre::ST_GENERIC);
        mCamera = mSceneMan->createCamera(Ogre::StringUtil::BLANK);
        mKernel->mViewport = mKernel->mWindow->addViewport(mCamera);
        mKernel->mViewport->setBackgroundColour(Ogre::ColourValue(0,0,0));
        mCamera->setAspectRatio(Ogre::Real(mKernel->mViewport->getActualWidth()) / Ogre::Real(mKernel->mViewport->getActualHeight()));
    }

    void Module::exit() {
        mKernel->mWindow->removeAllViewports();
        mSceneMan->clearScene();
        mSceneMan->destroyCamera(mCamera);
        mKernel->mRoot->destroySceneManager(mSceneMan);
        mSceneMan = NULL;
    }

    void Module::halt() {
        mKernel->mWindow->removeAllViewports();
    }

    void Module::wakeup() {
        mKernel->mWindow->removeAllViewports();
        mKernel->mViewport = mKernel->mWindow->addViewport(mCamera);
        mKernel->mViewport->setBackgroundColour(Ogre::ColourValue(0,0,0));
        mCamera->setAspectRatio(Ogre::Real(mKernel->mViewport->getActualWidth()) / Ogre::Real(mKernel->mViewport->getActualHeight()));
    }
};
