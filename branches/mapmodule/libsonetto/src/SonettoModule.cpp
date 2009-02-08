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

#include "SonettoModule.h"
#include "SonettoKernel.h"

namespace Sonetto
{
    // ----------------------------------------------------------------------
    void Module::initialize()
    {
        Kernel *kernel = Kernel::getSingletonPtr();

        if (kernel->getRenderWindow()->getNumViewports() > 0)
        {
            kernel->getRenderWindow()->removeAllViewports();
        }

        // Create the scene manager for this module.
        mSceneMan = Ogre::Root::getSingleton().
                createSceneManager(Ogre::ST_GENERIC);

        mCamera = mSceneMan->createCamera(Ogre::StringUtil::BLANK);
        mViewport = kernel->getRenderWindow()->addViewport(mCamera);
        mCamera->setAspectRatio(kernel->mAspectRatio);
        setBgColor(mBgColor);

        mOverlay = Ogre::OverlayManager::getSingleton().create(mOverlayName);
        mOverlay->show();
    }
    // ----------------------------------------------------------------------
    void Module::update()
    {
        mCamera->setAspectRatio(Kernel::getSingletonPtr()->mAspectRatio);
    }
    // ----------------------------------------------------------------------
    void Module::deinitialize()
    {
        Kernel * kernel = Kernel::getSingletonPtr();
        mOverlay->clear();
        kernel->mOverlayMan->destroy(mOverlay);

        if (kernel->getRenderWindow()->getNumViewports() != 0)
            kernel->getRenderWindow()->removeAllViewports();

        mViewport = NULL;

        mSceneMan->clearScene();
        mSceneMan->destroyCamera(mCamera);
        Ogre::Root::getSingleton().destroySceneManager(mSceneMan);
        mSceneMan = NULL;
    }
    // ----------------------------------------------------------------------
    void Module::halt()
    {
        Kernel * kernel = Kernel::getSingletonPtr();
        if (kernel->getRenderWindow()->getNumViewports() != 0)
            kernel->getRenderWindow()->removeAllViewports();
        mViewport = NULL;
    }
    // ----------------------------------------------------------------------
    void Module::resume()
    {
        Kernel * kernel = Kernel::getSingletonPtr();
        if (kernel->getRenderWindow()->getNumViewports() != 0)
            kernel->getRenderWindow()->removeAllViewports();
        mViewport = kernel->getRenderWindow()->addViewport(mCamera);
        setBgColor(mBgColor);
        mCamera->setAspectRatio(kernel->mAspectRatio);

    }
    // ----------------------------------------------------------------------
    void Module::setBgColor(const Ogre::ColourValue &col)
    {
        mBgColor = col;
        mViewport->setBackgroundColour(mBgColor);
    }
    // ----------------------------------------------------------------------
} // namespace
