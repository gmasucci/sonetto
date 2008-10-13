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

#include "SonettoEventObject.h"

namespace Sonetto {
    EventObject::EventObject   (const Ogre::String &name,
                                Ogre::SceneNode *parent,
                                Ogre::SceneManager *manager,
                                float height,
                                float actRadius,
                                float colRadius,
                                float speed,
                                bool noVisibleEntity,
                                const Ogre::String &modelname) :
                                mEventState(ES_NORMAL),
                                mIgnoreCollisions(false),
                                mName(name),
                                mModelName(modelname),
                                mInvisible(noVisibleEntity),
                                mEntity(NULL),
                                mParent(parent),
                                mSceneManager(manager),
                                mTargetPosition(Ogre::Vector3(0.0f,0.0f,0.0f)),
                                mHeight(height),
                                mActRadius(actRadius),
                                mColRadius(colRadius),
                                mSpeed(speed)
    {
        mNode = mParent->createChildSceneNode(mName + "_node");
        if(!mInvisible)
        {
            mEntity = mSceneManager->createEntity(mName + "_entity", mModelName);
            mNode->attachObject(mEntity);
        }
    }
    EventObject::~EventObject()
    {
        mParent->removeChild(mNode);
        if(!mInvisible)
        {
            mNode->detachObject(mEntity);
            mSceneManager->destroyEntity(mEntity);
        }
        mSceneManager->destroySceneNode(mNode->getName());
    }
    void EventObject::update(float deltatime)
    {
    }
    const Ogre::String & EventObject::getName(void) const
    {
        return mName;
    }
    void EventObject::changeModel(const Ogre::String & modelname)
    {
        mModelName = modelname;
        if(!mInvisible)
        {
            mNode->detachObject(mEntity);
            mSceneManager->destroyEntity(mEntity);
            mEntity = mSceneManager->createEntity(mName + "_entity", mModelName);
            mParent->attachObject(mEntity);
        }
    }
    void EventObject::setPosition(const Ogre::Vector3 &pos)
    {
        mNode->setPosition(pos);
    }
    void EventObject::setPosition(float x, float y, float z)
    {
        mNode->setPosition(x,y,z);
    }
    const Ogre::Vector3 & EventObject::getPosition(void) const
    {
        return mNode->getPosition();
    }
    void EventObject::setOrientation(const Ogre::Quaternion & orientation)
    {
        mNode->setOrientation(orientation);
    }
    void EventObject::setOrientation(float w, float x, float y, float z)
    {
        mNode->setOrientation(w,x,y,z);
    }
    const Ogre::Quaternion & EventObject::getOrientation(void) const
    {
        return mNode->getOrientation();
    }
    void EventObject::setState(EventState state)
    {
        mEventState = state;
    }
    const EventState EventObject::getState(void) const
    {
        return mEventState;
    }
    void EventObject::setIgnoreCollision(bool mode)
    {
        mIgnoreCollisions = mode;
    }
    const bool EventObject::getIgnoreCollision(void) const
    {
        return mIgnoreCollisions;
    }
    bool EventObject::isInvisible(void) const
    {
        return mInvisible;
    }
    void EventObject::setParent(Ogre::SceneNode * parent)
    {
        mParent->removeChild(mNode);
        mParent = parent;
        mParent->addChild(mNode);
    }
    const Ogre::Entity * EventObject::getEntity(void) const
    {
        return mEntity;
    }
    Ogre::SceneNode * EventObject::getNode(void)
    {
        return mNode;
    }
    const Ogre::SceneNode * EventObject::getParent(void) const
    {
        return mParent;
    }
    const Ogre::SceneManager * EventObject::getSceneManager(void) const
    {
        return mSceneManager;
    }
}; // namespace
