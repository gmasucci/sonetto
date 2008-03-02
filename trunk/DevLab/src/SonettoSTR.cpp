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
#include "SonettoSTR.h"
#include "SonettoSTRSerializer.h"

namespace Sonetto
{
    STRData::STRData(Ogre::ResourceManager* creator, const Ogre::String &name,
                    Ogre::ResourceHandle handle, const Ogre::String &group, bool isManual,
                    Ogre::ManualResourceLoader *loader) :
    Ogre::Resource(creator, name, handle, group, isManual, loader)
    {
        /* For consistency with StringInterface, but we don't add any parameters here
        That's because the Resource implementation of StringInterface is to
        list all the options that need to be set before loading, of which
        we have none as such. Full details can be set through scripts.
        */
        createParamDictionary("STR");
    }
    STRData::~STRData()
    {
        unload();
    }
    void STRData::loadImpl()
    {
        STRSerializer serializer;
        Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(mName, mGroup, true, this);
        serializer.importSTR(stream, this);
    }
    void STRData::unloadImpl()
    {
        mMessageList.clear();
    }
    size_t STRData::calculateSize() const
    {
        return 0;
    }
    const Ogre::String * STRData::getMessage(size_t msg_id)
    {
        return &mMessageList[msg_id];
    }
    void STRData::setMessage(Ogre::String& msg, size_t position)
    {
        mMessageList.erase(mMessageList.begin()+position);
        mMessageList.insert(mMessageList.begin()+position, msg);

    }
    size_t STRData::insertMessage(Ogre::String& msg)
    {
        mMessageList.push_back(msg);

        return mMessageList.size()-1;
    }
    void STRData::insertMessage(Ogre::String& msg, size_t position)
    {
        mMessageList.insert(mMessageList.begin()+position, msg);
    }
    void STRData::removeMessage()
    {
        mMessageList.pop_back();
    }
    void STRData::removeMessage(size_t position)
    {
        mMessageList.erase(mMessageList.begin()+position);
    }
} // namespaces
