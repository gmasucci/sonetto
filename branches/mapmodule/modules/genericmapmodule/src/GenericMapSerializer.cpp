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

#include <OgreLogManager.h>
#include <OgreStringConverter.h>
#include <SonettoUtil.h>
#include <SonettoKernel.h>
#include "GenericMap.h"
#include "GenericMapModule.h"
#include "GenericMapSerializer.h"

namespace GenericMapModule
{
    // ----------------------------------------------------------------------
    // GenericMapModule::MapSerializer implementation.
    // ----------------------------------------------------------------------
    void MapSerializer::importMapFile(Map *map)
    {
        uint32 fccomp,version;

        mStream->read(&fccomp,sizeof(fccomp));
        if (fccomp != FourCC)
        {
            SONETTO_THROW("Invalid map file");
        }

        mStream->read(&version,sizeof(version));
        switch (version)
        {
            case VER_0_1:
                importMapFile0_1(map);
            break;

            default:
                SONETTO_THROW("Unsupported map serializer version");
            break;
        }

        if (version != VER_CURRENT)
        {
            Ogre::LogManager::getSingleton().logMessage("WARNING: "
               "The map `" + map->getName() + "' uses an old file "
               "format. You should upgrade it as soon as possible using "
               "GenericMapUpgrade tool. (loaded version " +
               Ogre::StringConverter::toString(version) + "; current is " +
               Ogre::StringConverter::toString(VER_CURRENT) + ".");
        }
    }
    // ----------------------------------------------------------------------
    void MapSerializer::importMapFile0_1(Map *map)
    {
        MapModule *module = static_cast<MapModule *>
                (Sonetto::Kernel::getSingleton().getActiveModule());
        Ogre::SceneManager *sceneMan = module->getSceneMan();
        Ogre::MeshManager *meshMan = Ogre::MeshManager::getSingletonPtr();
        uint32 sharedVertexCount,totalSharedIndices,subMeshCount;

        // <todo> Check if a resource size should be the size it takes in
        // system memory, hardware buffers or the sum of both (taking into
        // account both now)
        mTotalResourceSize = 0;

        // - Header serialization
        {
            std::cout << "\n// - Header serialization\n";
            map->_setMapName(Sonetto::Util::readString(mStream));
            std::cout << map->getMapName() << " (map->getMapName())\n";
            mTotalResourceSize += map->getMapName().size();

            // Skips debug info offset
            mStream->skip(sizeof(uint32));
        }

        // - Static Geometry Layer / Layer mesh serialization
        {
            std::cout << "\n// - Static Geometry Layer / Layer mesh serialization\n";
            Ogre::AxisAlignedBox aab;
            Ogre::MeshPtr mesh = meshMan->createManual("MAP_STATICGEOM_MESH",
                    "MAP_LOCAL");
            Ogre::StaticGeometry *staticGeom = sceneMan->
                    createStaticGeometry("MAP_STATICGEOM");
            Ogre::Entity *entity;
            Ogre::HardwareIndexBufferSharedPtr sharedIndexBuffer;
            size_t sharedIndexBufOffset = 0;

            // Hide static geometry (it is to be shown when the GenericMapModule want)
            staticGeom->setVisible(false);

            // Import bounds
            importMeshBounds(mesh);

            // Skip shared vertex declaration
            mStream->skip(sizeof(uint16));

            mStream->read(&sharedVertexCount,sizeof(sharedVertexCount));
            std::cout << sharedVertexCount << " (sharedVertexCount)\n";
            if (sharedVertexCount > 0)
            {
                mesh->sharedVertexData = OGRE_NEW Ogre::VertexData();

                // Rewind vertex declaration and vertex count for importVertexData()
                mStream->skip( -(sizeof(uint16) + sizeof(sharedVertexCount)) );
                importVertexData(mesh->sharedVertexData);
            }

            mStream->read(&totalSharedIndices,sizeof(totalSharedIndices));
            std::cout << totalSharedIndices << " (totalSharedIndices)\n";
            if (totalSharedIndices > 0)
            {
                sharedIndexBuffer =
                        Ogre::HardwareBufferManager::getSingleton().
                        createIndexBuffer(
                        Ogre::HardwareIndexBuffer::IT_16BIT,
                        totalSharedIndices,
                        Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
                mTotalResourceSize += (totalSharedIndices * 2);
            }

            mStream->read(&subMeshCount,sizeof(subMeshCount));
            std::cout << subMeshCount << " (subMeshCount)\n";
            for (size_t i = 0;i < subMeshCount;++i)
            {
                std::cout << "SubMesh " << i << " ---->\n";
                importSubMesh(mesh,sharedIndexBuffer,&sharedIndexBufOffset);
                std::cout << "<----\n";
            }

            entity = sceneMan->createEntity("MAP_STATICGEOM_ENTITY",
                    "MAP_STATICGEOM_MESH");
            staticGeom->addEntity(entity,Ogre::Vector3::ZERO);
            staticGeom->build();

            sceneMan->destroyEntity(entity);
            meshMan->remove(mesh->getHandle());

            map->_setStaticGeometry(staticGeom);
        }

        // - Static Geometry Layer / Layer walkmesh serialization
        {
            std::cout << "\n// - Static Geometry Layer / Layer walkmesh serialization\n";
            Walkmesh &walkmesh = map->_getWalkmesh();
            uint32 triangleCount;

            mStream->read(&triangleCount,sizeof(triangleCount));
            walkmesh.triangles.resize(triangleCount);
            std::cout << triangleCount << " (triangleCount)\n";
            for (size_t i = 0;i < triangleCount;++i)
            {
                Walkmesh::Triangle &triangle = walkmesh.triangles[i];

                // - First triangle vertex position
                mStream->read(&triangle.vertices[0].x,
                        sizeof(triangle.vertices[0].x));
                mStream->read(&triangle.vertices[0].y,
                        sizeof(triangle.vertices[0].y));
                mStream->read(&triangle.vertices[0].z,
                        sizeof(triangle.vertices[0].z));

                // - Second triangle vertex position
                mStream->read(&triangle.vertices[1].x,
                        sizeof(triangle.vertices[1].x));
                mStream->read(&triangle.vertices[1].y,
                        sizeof(triangle.vertices[1].y));
                mStream->read(&triangle.vertices[1].z,
                        sizeof(triangle.vertices[1].z));

                // - Third triangle vertex position
                mStream->read(&triangle.vertices[2].x,
                        sizeof(triangle.vertices[2].x));
                mStream->read(&triangle.vertices[2].y,
                        sizeof(triangle.vertices[2].y));
                mStream->read(&triangle.vertices[2].z,
                        sizeof(triangle.vertices[2].z));

                // - Edge links, ground type and battle background
                mStream->read(&triangle.edgeLinks[0],
                        sizeof(triangle.edgeLinks));
                mStream->read(&triangle.groundType,
                        sizeof(triangle.groundType));
                mStream->read(&triangle.battleBackground,
                        sizeof(triangle.battleBackground));
            }

            mTotalResourceSize += (triangleCount *
                    sizeof(Walkmesh::Triangle));
        }

        // - Sky Layer serialization
        {
            std::cout << "\n// - Sky Layer serialization\n";
            SkyEntityDataMap &skyEntities = map->_getSkyEntitiesData();
            uint32 entityCount;

            mStream->read(&entityCount,sizeof(entityCount));
            std::cout << entityCount << " (entityCount)\n";
            for (size_t i = 0;i < entityCount;++i)
            {
                Ogre::HardwareIndexBufferSharedPtr sharedIndexBuffer;
                uint32 id;
                size_t sharedIndexBufOffset = 0;

                mStream->read(&id,sizeof(id));
                if (skyEntities.find(id) != skyEntities.end())
                {
                    SONETTO_THROW("Sky entity ID already in use");
                }

                if (id == 0)
                {
                    SONETTO_THROW("Invalid sky entity ID");
                }

                SkyEntityData &entity = skyEntities[id];

                // - Initial position
                mStream->read(&entity.initialPosition.x,
                        sizeof(entity.initialPosition.x));
                mStream->read(&entity.initialPosition.y,
                        sizeof(entity.initialPosition.y));
                mStream->read(&entity.initialPosition.z,
                        sizeof(entity.initialPosition.z));
                std::cout << entity.initialPosition <<
                        " (entity.initialPosition)\n";

                // - Visibility
                mStream->read(&entity.initialVisibility,
                        sizeof(entity.initialVisibility));
                std::cout << entity.initialVisibility <<
                        " (entity.initialVisibility)\n";

                // - Mesh
                entity.mesh = meshMan->createManual("SKYLAYER_MESH_" +
                        Ogre::StringConverter::toString(id,3,'0'),
                        "MAP_LOCAL");

                // Import bounds
                importMeshBounds(entity.mesh);
                std::cout << entity.mesh->getBounds() <<
                        " (entity.mesh->getBounds())\n";

                // Skip shared vertex declaration
                mStream->skip(sizeof(uint16));

                mStream->read(&sharedVertexCount,sizeof(sharedVertexCount));
                std::cout << sharedVertexCount << " (sharedVertexCount)\n";
                if (sharedVertexCount > 0)
                {
                    entity.mesh->sharedVertexData = OGRE_NEW Ogre::VertexData();

                    // Rewind vertex declaration and
                    // vertex count for importVertexData()
                    mStream->skip( -(sizeof(uint16) +
                            sizeof(sharedVertexCount)) );
                    importVertexData(entity.mesh->sharedVertexData);
                }

                mStream->read(&totalSharedIndices,sizeof(totalSharedIndices));
                std::cout << totalSharedIndices << " (totalSharedIndices)\n";
                if (totalSharedIndices > 0)
                {
                    sharedIndexBuffer =
                            Ogre::HardwareBufferManager::getSingleton().
                            createIndexBuffer(
                            Ogre::HardwareIndexBuffer::IT_16BIT,
                            totalSharedIndices,
                            Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
                    mTotalResourceSize += (totalSharedIndices * 2);
                }

                mStream->read(&subMeshCount,sizeof(subMeshCount));
                std::cout << subMeshCount << " (subMeshCount)\n";
                for (size_t j = 0;j < subMeshCount;++j)
                {
                    std::cout << "SubMesh " << j << " ---->\n";
                    importSubMesh(entity.mesh,sharedIndexBuffer,
                            &sharedIndexBufOffset);
                    std::cout << "<----\n";
                }
            }

            mTotalResourceSize += (entityCount * sizeof(SkyEntityData));
        }

        // - Billboard Layer serialization
        {
            std::cout << "\n// - Billboard Layer serialization\n";
            BillboardSetDataMap &billboardSets = map->_getBillboardSetData();
            BillboardDataMap &billboards = map->_getBillboardData();
            uint32 billboardSetCount;

            mStream->read(&billboardSetCount,sizeof(billboardSetCount));
            std::cout << billboardSetCount << " (billboardSetCount)\n";
            for (size_t i = 0;i < billboardSetCount;++i)
            {
                uint32 id,billboardCount;

                // - Read billboard set ID
                mStream->read(&id,sizeof(id));
                std::cout << id << " (id)\n";
                if (billboardSets.find(id) != billboardSets.end())
                {
                    SONETTO_THROW("Billboard set ID already in use");
                }

                if (id == 0)
                {
                    SONETTO_THROW("Invalid billboard set ID");
                }

                BillboardSetData &billboardSet = billboardSets[id];

                // - Read material name
                billboardSet.materialName = Sonetto::Util::readString(mStream);
                std::cout << billboardSet.materialName <<
                        " (billboardSet.materialName)\n";

                // - Read position
                mStream->read(&billboardSet.position.x,
                        sizeof(billboardSet.position.x));
                mStream->read(&billboardSet.position.y,
                        sizeof(billboardSet.position.y));
                mStream->read(&billboardSet.position.z,
                        sizeof(billboardSet.position.z));
                std::cout << billboardSet.position <<
                        " (billboardSet.position)\n";

                // - Read default size
                mStream->read(&billboardSet.defaultSize.x,
                        sizeof(billboardSet.defaultSize.x));
                mStream->read(&billboardSet.defaultSize.y,
                        sizeof(billboardSet.defaultSize.y));
                std::cout << billboardSet.defaultSize <<
                        " (billboardSet.defaultSize)\n";

                // - Read pool size
                mStream->read(&billboardSet.poolSize,
                        sizeof(billboardSet.poolSize));

                mStream->read(&billboardCount,sizeof(billboardCount));
                std::cout << billboardCount << " (billboardCount)\n";
                for (size_t j = 0;j < billboardCount;++j)
                {
                    std::cout << "Billboard " << j << " ---->\n";
                    uint32 bid;

                    // - Read billboard ID
                    mStream->read(&bid,sizeof(bid));
                    std::cout << bid << " (bid)\n";
                    if (billboards.find(bid) != billboards.end())
                    {
                        SONETTO_THROW("Billboard ID already in use");
                    }

                    if (bid == 0)
                    {
                        SONETTO_THROW("Invalid billboard ID");
                    }

                    // Register ID with current billboard set
                    billboardSet.billboards.push_back(bid);

                    // Create structure to hold info
                    BillboardData &billboard = billboards[bid];

                    // - Read position
                    mStream->read(&billboard.position.x,
                            sizeof(billboard.position.x));
                    mStream->read(&billboard.position.y,
                            sizeof(billboard.position.y));
                    mStream->read(&billboard.position.z,
                            sizeof(billboard.position.z));
                    std::cout << billboard.position <<
                            " (billboard.position)\n";

                    // - Read size
                    mStream->read(&billboard.size.x,
                            sizeof(billboard.size.x));
                    mStream->read(&billboard.size.y,
                            sizeof(billboard.size.y));
                    std::cout << billboard.size <<
                            " (billboard.size)\n";

                    // - Read texture coordinates
                    mStream->read(&billboard.texCoord.left,
                            sizeof(billboard.texCoord.left));
                    mStream->read(&billboard.texCoord.top,
                            sizeof(billboard.texCoord.top));
                    mStream->read(&billboard.texCoord.right,
                            sizeof(billboard.texCoord.right));
                    mStream->read(&billboard.texCoord.bottom,
                            sizeof(billboard.texCoord.bottom));
                    std::cout << billboard.texCoord.left << " / " <<
                                 billboard.texCoord.top << " / " <<
                                 billboard.texCoord.right << " / " <<
                                 billboard.texCoord.bottom << " / " <<
                            " (billboard.texCoord)\n";

                    // - Read color tinting
                    mStream->read(&billboard.color.a,
                            sizeof(billboard.color.a));
                    mStream->read(&billboard.color.r,
                            sizeof(billboard.color.r));
                    mStream->read(&billboard.color.g,
                            sizeof(billboard.color.g));
                    mStream->read(&billboard.color.b,
                            sizeof(billboard.color.b));
                    std::cout << billboard.color << " (billboard.color)\n";
                    std::cout << "<----\n";
                }

                mTotalResourceSize += (billboardCount * sizeof(BillboardData));
            }

            mTotalResourceSize += (billboardSetCount * sizeof(BillboardSetData));
        }

        // - Dynamic Geometry Layer serialization
        {
            // Empty for now
        }

        // - Event Layer serialization
        {
            // <todo> Finish later
        }

        map->_setResourceSize(mTotalResourceSize);

        std::cout << "\n// - Finished map serialization\n\n";
    }
    // ----------------------------------------------------------------------
    void MapSerializer::importMeshBounds(Ogre::MeshPtr mesh)
    {
        Ogre::AxisAlignedBox aab;
        float minimum[3],maximum[3];

        mStream->read(&minimum[0],sizeof(minimum));
        aab.setMinimum(minimum[0],minimum[1],minimum[2]);

        mStream->read(&maximum[0],sizeof(maximum));
        aab.setMaximum(maximum[0],maximum[1],maximum[2]);

        mesh->_setBounds(aab);
        mesh->_setBoundingSphereRadius(std::max(maximum[0] - minimum[0],
                std::max(maximum[1] - minimum[1], maximum[2] - minimum[2]))
                / 2.0f);
    }
    // ----------------------------------------------------------------------
    void MapSerializer::importSubMesh(Ogre::MeshPtr mesh,
            Ogre::HardwareIndexBufferSharedPtr sharedIndexBuffer,
            size_t *offset)
    {
        bool useSharedVertexData;
        Ogre::SubMesh *subMesh = mesh->createSubMesh();

        subMesh->setMaterialName(Sonetto::Util::readString(mStream));

        mStream->read(&useSharedVertexData,sizeof(useSharedVertexData));
        std::cout << useSharedVertexData << " (useSharedVertexData)\n";

        if (useSharedVertexData) {
            subMesh->indexData->indexStart = *offset;
            subMesh->indexData->indexCount =
                    importIndexBuffer(sharedIndexBuffer,offset);
            std::cout << subMesh->indexData->indexCount <<
                    " (subMesh->indexData->indexCount)\n";
            std::cout << subMesh->indexData->indexStart <<
                    " (subMesh->indexData->indexStart)\n";
            subMesh->indexData->indexBuffer = sharedIndexBuffer;
        } else {
            uint32 indexCount;
            subMesh->useSharedVertices = false;
            subMesh->vertexData = OGRE_NEW Ogre::VertexData();
            Ogre::HardwareIndexBufferSharedPtr indexBuffer;

            importVertexData(subMesh->vertexData);

            mStream->read(&indexCount,sizeof(indexCount));
            indexBuffer = Ogre::HardwareBufferManager::getSingleton().
                    createIndexBuffer(Ogre::HardwareIndexBuffer::IT_16BIT,
                    indexCount,Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
            mTotalResourceSize += (indexCount * 2);

            // Rewind so that importIndexBuffer() can read the index count
            mStream->skip( -(sizeof(indexCount)) );

            subMesh->indexData->indexStart = 0;
            subMesh->indexData->indexCount = importIndexBuffer(indexBuffer,0);
            subMesh->indexData->indexBuffer = indexBuffer;
        }
    }
    // ----------------------------------------------------------------------
    void MapSerializer::importVertexData(Ogre::VertexData *data)
    {
        uint16 vertexDecl;
        uint32 vertexCount;
        size_t bytes;

        // - Read and generate vertex declaration
        mStream->read(&vertexDecl,sizeof(vertexDecl));
        if (!(vertexDecl & VDE_POSITION) || !(vertexDecl & VDE_NORMAL))
        {
            SONETTO_THROW("Map file vertex declaration does not "
                    "include positions or normals");
        }

        size_t offset = 0;

        data->vertexDeclaration->addElement(0,offset,Ogre::VET_FLOAT3,
                Ogre::VES_POSITION);
        offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

        data->vertexDeclaration->addElement(0,offset,Ogre::VET_FLOAT3,
                Ogre::VES_NORMAL);
        offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

        if (vertexDecl & VDE_DIFFUSE)
        {
            data->vertexDeclaration->addElement(0,offset,Ogre::VET_COLOUR_ARGB,
                    Ogre::VES_DIFFUSE);
            offset += Ogre::VertexElement::getTypeSize(Ogre::VET_COLOUR_ARGB);
        }

        if (vertexDecl & VDE_SPECULAR)
        {
            data->vertexDeclaration->addElement(0,offset,Ogre::VET_COLOUR_ARGB,
                    Ogre::VES_SPECULAR);
            offset += Ogre::VertexElement::getTypeSize(Ogre::VET_COLOUR_ARGB);
        }

        bool lastCoordSetReached = false;
        for (size_t i = 0;i < 8;++i)
        {
            if (vertexDecl & (VDE_TEXCOORD_1 << i)) {
                if (lastCoordSetReached)
                {
                    SONETTO_THROW("Map file vertex declaration's texture "
                            "coordinate flags contain gaps");
                }

                data->vertexDeclaration->addElement(0,offset,Ogre::VET_FLOAT2,
                        Ogre::VES_TEXTURE_COORDINATES,i);
                offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);
            } else {
                lastCoordSetReached = true;
            }
        }

        // - Read and upload vertex data to HardwareVertexBuffer
        mStream->read(&vertexCount,sizeof(vertexCount));
        data->vertexCount = vertexCount;
        bytes = (data->vertexDeclaration->getVertexSize(0) * vertexCount);

        Ogre::HardwareVertexBufferSharedPtr vBufPtr =
                Ogre::HardwareBufferManager::getSingleton().
                createVertexBuffer(data->vertexDeclaration->getVertexSize(0),
                vertexCount,
                Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
        mTotalResourceSize += bytes;

        // <todo> Check exceptions to retry; couldn't find documentation
        // about exceptions that could be thrown here and Ogre *doesn't seem*
        // to throw any indeed, but who knows..
        mStream->read(vBufPtr->lock(Ogre::HardwareBuffer::HBL_NORMAL),bytes);
        vBufPtr->unlock();

        data->vertexBufferBinding->setBinding(0,vBufPtr);
    }
    // ----------------------------------------------------------------------
    size_t MapSerializer::importIndexBuffer(
            Ogre::HardwareIndexBufferSharedPtr indexBuffer,
            size_t *offset)
    {
        uint32 triangleCount,bytes;

        mStream->read(&triangleCount,sizeof(triangleCount));

        // <todo> Check exceptions to retry; couldn't find documentation
        // about exceptions that could be thrown here and Ogre *doesn't seem*
        // to throw any indeed, but who knows..
        std::cout << triangleCount << " (triangleCount)\n";
        std::cout << *offset << " (offset)\n";
        bytes = (triangleCount * 3 * 2);
        mStream->read((char *)(indexBuffer->lock(
                Ogre::HardwareBuffer::HBL_NORMAL)) + *offset,bytes);
        indexBuffer->unlock();
        *offset += bytes;

        std::cout << mStream->tell() << " (mStream->tell())\n";

        return (triangleCount * 3);
    }
} // namspace
