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
#ifndef SONETTO_EVENTOBJECT_H
#define SONETTO_EVENTOBJECT_H

#include <Ogre.h>
#include "Sonetto.h"

namespace Sonetto{

    enum EventState {
        ES_NORMAL,
        ES_LOCKED,
        ES_SCRIPT
    };
    /** Represents a Event Object inside the game.

        Usually is used in maps to represent the Player, NPCs and other objects
        such as event triggers and wrap gateways (used to change maps).
        They may and may not contain visible models, but always contain position, orientation
        and collision objects, note that this class contains collision tests in its update function
        so must be set up properly.
    */
    class SONETTO_EXPORT EventObject
    {
    public:
        EventObject(const Ogre::String & name,
                    Ogre::SceneNode * parent,
                    Ogre::SceneManager * manager,
                    bool noVisibleEntity = false,
                    const Ogre::String & modelname = "");
        virtual ~EventObject();

        /** Updates this Object.

        @param
            deltatime This is the value of 1.0f/FPS used for framerate-independent movement.
        */
        virtual void update(float deltatime);

        virtual const Ogre::String & getName(void) const;

        /** This function changes the actual object model

            note that this function change the actual entity internally.
        @param
            modelname The new model for this object.
        */
        virtual void changeModel(const Ogre::String & modelname);

        virtual void setPosition(const Ogre::Vector3 &pos);

        virtual void setPosition(float x = 0.0f, float y = 0.0f, float z = 0.0f);

        virtual const Ogre::Vector3 & getPosition(void) const;

        virtual void setOrientation(const Ogre::Quaternion &orientation);

        virtual void setOrientation(float w, float x, float y, float z);

        virtual const Ogre::Quaternion & getOrientation(void) const;

        /** Set the position to move this Object in the next frame.

            This functions move the object translating it from it's current location
            note that it does not move the object directly, it will be updated at the
            update function, the final position may not be the same as inputed in this
            function, as it will first pass through a collision test,
            also note that the movement value will reset at every frame.

        @param
            mov This it the desired position the character will move, translating from its
            current position, and will take collisions in consideration.
        */
        virtual void moveObject(const Ogre::Vector3 & mov);

        /// Set the state for this event.
        virtual void setState(EventState state);

        /// Get the state for this event.
        virtual const EventState getState(void) const;

        /** Set the current collision flag.

            When true, the event will move freely through the map,
            withouth taking collisions in consideration, this is used
            by the Script to create flying objects withouth interference of the map.
        */
        virtual void setIgnoreCollision(bool mode);

        /// Return the current collision flag.
        virtual const bool getIgnoreCollision(void) const;

        /// Return whether this object has a visible entity or not.
        virtual bool isInvisible(void) const;

        /// Set a new parent node for this object.
        virtual void setParent(Ogre::SceneNode * parent);

        /// Return a pointer to this object's entity.
        virtual const Ogre::Entity * getEntity(void) const;

        /// Return a pointer to this object's node.
        virtual Ogre::SceneNode * getNode(void);

        /// Return a pointer to this object's parent node.
        virtual const Ogre::SceneNode * getParent(void) const;

        /// Return a pointer to this object's scene manager.
        virtual const Ogre::SceneManager * getSceneManager(void) const;

    protected:
        /// This Event State
        EventState mEventState;
        /// Ignore Collisions Flag (Ghost / Fly Mode)
        bool mIgnoreCollisions;
        Ogre::String mName;
        Ogre::String mModelName;
        /** This Object flags, wheter has a visible model or not.
            (true for Triggers/Gateways, false for NPCs and Players)
        */
        bool mInvisible;
        /// This Object Entity. (Visible model)
        Ogre::Entity * mEntity;
        /// This Object Node. (Represent Position and Orientation)
        Ogre::SceneNode * mNode;
        /// This Object Node's Parent.
        Ogre::SceneNode * mParent;
        /// The SceneManager used to create and manage this object's objects.
        Ogre::SceneManager * mSceneManager;
        /// The desired position relative to current, reset at every frame.
        Ogre::Vector3 mTargetPosition;
    };
/*
    class SONETTO_EXPORT HeroObject : public EventObject
    {
        HeroObject( const Ogre::String & name,
                    Ogre::SceneNode * parent,
                    Ogre::SceneManager * manager,
                    bool noVisibleEntity = false,
                    const Ogre::String & modelname = "");
        virtual ~HeroObject();
    };
*/
}; // namespace


#endif // SONETTO_EVENTOBJECT_H
