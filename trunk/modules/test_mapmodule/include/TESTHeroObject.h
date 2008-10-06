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
#ifndef TEST_HEROOBJECT_H
#define TEST_HEROOBJECT_H

#include "SonettoEventObject.h"

// If it's the client, then import the library from the dll, export otherwise.
#if defined( WIN32 )
#   if defined( MODULE_DLL )
#        define MODULE_EXPORT __declspec( dllexport )
#   else
#       define MODULE_EXPORT __declspec( dllimport )
#   endif
#endif

namespace Sonetto
{
    class MODULE_EXPORT HeroObject : public EventObject
    {
    public:
        HeroObject( const Ogre::String & name,
                    Ogre::SceneNode * parent,
                    Ogre::SceneManager * manager,
                    CollisionManager * colmanager,
                    float height,float actRadius,
                    float colRadius,
                    bool noVisibleEntity = false,
                    const Ogre::String & modelname = "");
        virtual ~HeroObject();

        /// @see EventObject::update()
        virtual void update(float deltatime,const EventVector &events);

        /// Set the Hero's base movement speed.
        virtual void setHeroSpeed(float speed);

        /// Return the current hero base movement speed.
        virtual const float getHeroSpeed(void) const;

        /** Set the base direction to calculate the hero movement vector.

            This is used to calculate the event direction based on camera
            or other direction.
        */
        virtual void setBaseDirection(const Ogre::Vector3 &dir);

        /** Set the base direction to calculate the hero movement vector.

            This is used to calculate the event direction based on camera
            or other direction.
        */
        virtual void setBaseDirection(float x, float y, float z);

        /// Return the current base direction.
        virtual const Ogre::Vector3 & getBaseDirection(void) const;

        /// Return the current base direction right vector.
        virtual const Ogre::Vector3 & getBaseDirectionRight(void) const;

        virtual void setMovementInput(const Ogre::Vector2 &mov);

        virtual const Ogre::Vector2 & getMovementInput(void) const;

    private:
        /// Hero Base Movement Speed.
        float mHeroSpeed;
        float mOldInputMag;
        /// Character Movement Input. (From Player)
        Ogre::Vector2 mMovementInput;
        /// Base Movement Front Vector.
        Ogre::Vector3 mBaseFront;
        /// Base Movement Right Vector.
        Ogre::Vector3 mBaseRight;
        /// Movement Direction Vector.
        Ogre::Vector3 mDirection;


    };
}; // namespace

#endif // TEST_HEROOBJECT_H
