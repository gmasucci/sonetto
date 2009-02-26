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

#ifndef GENERICMAPMODULE_EVENT_H
#define GENERICMAPMODULE_EVENT_H

#include <map>
#include <vector>
#include <OgreEntity.h>
#include <SonettoVariable.h>
#include "GenericMapPrerequisites.h"

namespace GenericMapModule
{
    class Event
    {
    public:
        enum MeshSource
        {
            MES_NONE = 0,
            MES_NPC,
            MES_PARTY_MEMBER
        };

        Event(uint32 id,const Ogre::Vector3 &position = Ogre::Vector3::ZERO,
            const Ogre::Quaternion &rotation = Ogre::Quaternion::IDENTITY);

        virtual ~Event();

        virtual inline uint32 getID() const { return mID; }

        virtual inline Sonetto::VariableMap &getLocals() { return mLocals; }

        virtual void update() = 0;

        virtual inline Ogre::SceneNode *getSceneNode() const
                { return mSceneNode; }

        virtual inline Ogre::Entity *getEntity() const { return mEntity; }

        virtual inline bool getBlockEnabled() { return mBlockEnabled; }

        virtual inline void setCurrentTriangle(uint32 id) { mCurTriangle = id; }
        virtual inline uint32 getCurrentTriangle() const
                { return mCurTriangle; }

        virtual inline void setWalkSpeed(float speed) { mWalkSpeed = speed; }
        virtual inline float getWalkSpeed() const { return mWalkSpeed; }

        virtual inline void setHeight(float height) { mHeight = height; }
        virtual inline float getHeight() const { return mHeight; }

        virtual inline void setRadius(float radius) { mRadius = radius; }
        virtual inline float getRadius() const { return mRadius; }

    protected:
        virtual void setEntityMesh(MeshSource source,uint32 id);

        uint32 mID;

        Sonetto::VariableMap mLocals;

        Ogre::SceneNode *mSceneNode;
        Ogre::Entity *mEntity;

        MeshSource mCurMeshSource;
        uint32 mCurMeshID;

        bool mBlockEnabled;

        uint32 mCurTriangle;

        float mWalkSpeed;

        // <todo> Make possible to use other types of collision checking
        float mHeight;
        float mRadius;
    };

    typedef std::map<uint32,Event *> EventMap;
    typedef std::vector<Event *> EventVector;
} // namespace

#endif
