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

#include <Ogre.h>
#include "SonettoModule.h"

namespace Sonetto {
    //-----------------------------------------------------------------------------
    // Sonetto::Module implementation.
    //-----------------------------------------------------------------------------
    Module::Module() :
            mSceneMan (NULL),
            mOverlay  (NULL),
            mCamera   (NULL)
    {
        mKernel = Kernel::getSingleton();
    }
    //-----------------------------------------------------------------------------
    void Module::enter()
    {
        if (mKernel->getRenderWindow()->getNumViewports() != 0)
            mKernel->getRenderWindow()->removeAllViewports();

        // Create the scene manager for this module.
        mSceneMan = mKernel->getOgreRoot()->createSceneManager(Ogre::ST_GENERIC);
        mCamera   = mSceneMan->createCamera(Ogre::StringUtil::BLANK);

        mKernel->setViewport(mKernel->getRenderWindow()->addViewport(mCamera));
        mKernel->getViewport()->setBackgroundColour(Ogre::ColourValue(0,0,0));
        mCamera->setAspectRatio(Ogre::Real(mKernel->getViewport()->getActualWidth()) /
                                Ogre::Real(mKernel->getViewport()->getActualHeight()));
    }
    //-----------------------------------------------------------------------------
    void Module::exit()
    {
        if (mKernel->getRenderWindow()->getNumViewports() != 0)
            mKernel->getRenderWindow()->removeAllViewports();

        mSceneMan->clearScene();
        mSceneMan->destroyCamera(mCamera);
        mKernel->getOgreRoot()->destroySceneManager(mSceneMan);
        mSceneMan = NULL;
    }
    //-----------------------------------------------------------------------------
    void Module::halt()
    {
        if (mKernel->getRenderWindow()->getNumViewports() != 0)
            mKernel->getRenderWindow()->removeAllViewports();
    }
    //-----------------------------------------------------------------------------
    void Module::wakeup()
    {
        if (mKernel->getRenderWindow()->getNumViewports() != 0)
            mKernel->getRenderWindow()->removeAllViewports();
        mKernel->setViewport(mKernel->getRenderWindow()->addViewport(mCamera));
        mKernel->getViewport()->setBackgroundColour(Ogre::ColourValue(0,0,0));
        mCamera->setAspectRatio(Ogre::Real(mKernel->getViewport()->getActualWidth()) /
                                Ogre::Real(mKernel->getViewport()->getActualHeight()));
    }
    //-----------------------------------------------------------------------------
};
