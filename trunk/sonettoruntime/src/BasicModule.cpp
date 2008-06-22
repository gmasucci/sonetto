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
#include "BasicModule.h"

using namespace Ogre;
using namespace Sonetto;

void BasicModule::enter()
{
    PlayerInput *player;
    Module::enter();

    // Let's select a nice background colour for our viewport
    mKernel->getViewport()->setBackgroundColour(ColourValue(0.95f,0.95f,0.95f));

    // And setup the first controller
    player = mKernel->getInputMan()->getPlayer(0);
    // player->setEnabled(true);
    // player->configureBtn(Sonetto::BTN_CROSS,Sonetto::InputSource(true,Sonetto::IST_BUTTON,2));
}

void BasicModule::update(Ogre::Real deltatime)
{
    /*if (player->getButtonState(Sonetto::BTN_CROSS) == Sonetto::KS_PRESS)
    {
        std::cout << "Fear the cross, demon..!\n";
    }*/
}
