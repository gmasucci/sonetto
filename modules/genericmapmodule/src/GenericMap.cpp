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

#include <OgreResourceGroupManager.h>
#include <OgreDataStream.h>
#include <OgreStaticGeometry.h>
#include <SonettoKernel.h>
#include "GenericMap.h"
#include "GenericMapSerializer.h"

namespace GenericMapModule
{
    // ----------------------------------------------------------------------
    // GenericMapModule::Map implementation.
    // ----------------------------------------------------------------------
    Map::~Map()
    {
        unload();
    }
    // ----------------------------------------------------------------------
    size_t Map::calculateSize() const
    {
        return mResourceSize;
    }
    // ----------------------------------------------------------------------
    void Map::loadImpl()
    {
        Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::
                getSingleton().openResource(mName,mGroup,true,this);
        MapSerializer serializer(stream);
        serializer.importMapFile(this);
    }
    // ----------------------------------------------------------------------
    void Map::unloadImpl()
    {
        Ogre::MeshManager &meshMan = Ogre::MeshManager::getSingleton();

        // Clears map name
        mMapName.clear();

        // Destroys static geometry
        Sonetto::Kernel::getSingleton().getActiveModule()->
                getSceneMan()->destroyStaticGeometry(mStaticGeom);

        // Clears walkmesh triangles
        mWalkmesh.triangles.clear();

        // Destroys and clears Sky Entities data
        while (!mSkyEntitiesData.empty())
        {
            SkyEntityDataMap::iterator iter = mSkyEntitiesData.begin();

            meshMan.remove(iter->second.mesh->getHandle());
            mSkyEntitiesData.erase(iter);
        }

        // Clears Billboard Set data
        mBillboardSetData.clear();

        // Clears Billboard data
        mBillboardData.clear();

        // Resets resource size
        mResourceSize = 0;
    }
} // namespace
