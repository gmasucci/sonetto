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

#include "TESTHeroObject.h"

namespace Sonetto
{
    HeroObject::HeroObject( const Ogre::String & name,
                            Ogre::SceneNode * parent,
                            Ogre::SceneManager * manager,
                            float height,float actRadius,
                            float colRadius,
                            bool noVisibleEntity,
                            const Ogre::String & modelname ) :
                            EventObject(name, parent, manager, height,
                                actRadius, colRadius, noVisibleEntity,
                                modelname),
                            mHeroSpeed(0.0f)
    {
    }
    HeroObject::~HeroObject()
    {
    }
    void HeroObject::update(float deltatime,const EventVector &events)
    {
        /*Ogre::Vector3 directionFinal, sourceDirection;

        // this will represent how many the player has pressed the Analog Stick, and we use this to move the hero.
        float inputMagnitude = mMovementInput.normalise();

        directionFinal.x = ((-mMovementInput.x) * mBaseRight.x) + ((-mMovementInput.y) * mBaseFront.x);
        directionFinal.y = 0.0f;
        directionFinal.z = ((-mMovementInput.x) * mBaseRight.z) + ((-mMovementInput.y) * mBaseFront.z);

        // Get the Orientation from initial direction.
        sourceDirection = getOrientation() * Ogre::Vector3::UNIT_X;
        // Ignore the pitch difference angle.
        sourceDirection.y = 0.0f;
        // Normalise the vectors.
        sourceDirection.normalise();
        directionFinal.normalise();

        // Ensure that the hero will not move while the controller is in the "dead zone".
        if(inputMagnitude < 0.3f)
        {
            inputMagnitude = 0.0f;
        } else if(inputMagnitude > 1.0f) {
            inputMagnitude = 1.0f;
        }
        Ogre::Vector3 interpFDir;
        if(inputMagnitude > 0.3f)
        {
            // interpFDir = (sourceDirection * (0.025f - deltatime)) + (directionFinal * deltatime);
            interpFDir = (sourceDirection * (1.0f - (deltatime*25.0f))) + (directionFinal * (deltatime*25.0f));
        } else {
            interpFDir = sourceDirection;
        }
        Ogre::Quaternion finalOrientation = sourceDirection.getRotationTo(interpFDir);

        // Set the new orientation relative to current one.
        setOrientation(finalOrientation * getOrientation());

        moveObject(directionFinal * (mHeroSpeed * inputMagnitude));*/

        EventObject::update(deltatime,events);
        // mMovementInput = Ogre::Vector2::ZERO;
    }
    void HeroObject::setHeroSpeed(float speed)
    {
        mHeroSpeed = speed;
    }
    const float HeroObject::getHeroSpeed(void) const
    {
        return mHeroSpeed;
    }
    void HeroObject::setBaseDirection(const Ogre::Vector3 &dir)
    {
    }
    void HeroObject::setBaseDirection(float x, float y, float z)
    {
    }
    const Ogre::Vector3 & HeroObject::getBaseDirection(void) const
    {
    }
    const Ogre::Vector3 & HeroObject::getBaseDirectionRight(void) const
    {
    }
}; // namespace
