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
#ifndef SONETTOSTR_H_INCLUDED
#define SONETTOSTR_H_INCLUDED

#include "SonettoMain.h"

#include <vector>
#include <OgrePrerequisites.h>
#include <OgreResourceManager.h>

namespace Sonetto
{
    class SONETTO_EXPORT STRData : public Ogre::Resource
    {
    protected:
        /** Internal implementation of the meat of the 'load' action, only called if this
		*	resource is not being loaded from a ManualResourceLoader.
		*/
        void loadImpl();
        /** Internal implementation of the 'unload' action; called regardless of
		*	whether this resource is being loaded from a ManualResourceLoader.
		*/
        void unloadImpl();
        /** Calculate the size of a resource; this will only be called after 'load' */
        size_t calculateSize() const;

    public:
        /// The Vector containing all strings for this STR File
        std::vector<Ogre::String> mMessageList;
        /** Standard construct SONETTO_EXPORTor.
		@param creator Pointer to the ResourceManager that is creating this resource
		@param name The unique name of the resource
		@param group The name of the resource group to which this resource belongs
		@param isManual Is this resource manually loaded? If so, you should really
			populate the loader parameter in order that the load process
			can call the loader back when loading is required.
		@param loader Pointer to a ManualResourceLoader implementation which will be called
			when the Resource wishes to load (should be supplied if you set
			isManual to true). You can in fact leave this parameter null
			if you wish, but the Resource will never be able to reload if
			anything ever causes it to unload. Therefore provision of a proper
			ManualResourceLoader instance is strongly recommended.
		*/
        STRData(Ogre::ResourceManager *creator, const Ogre::String &name,
        Ogre::ResourceHandle handle, const Ogre::String &group, bool isManual = false,
        Ogre::ManualResourceLoader *loader = 0);
        /** Virtual destruct SONETTO_EXPORTor. Shouldn't need to be overloaded, as the resource
            deallocation code should reside in unload()
            @see
                STRData::unload()
        */
        virtual ~STRData();
        /** Get a message from the vector.
        *   @param msg_id The message ID.
        *   @return A pointer to the string.
        */
        const Ogre::String * getMessage(size_t msg_id);
        /** Change a message contained in this STR.
        *   @param msg The message to put in the STR.
        *   @param position The position of the message you want to replace.
        */
        void setMessage(Ogre::String& msg, size_t position);
        /** Add a new message to the STR.
        *   @remarks The message will be added at last.
        *   @param msg The message to put in the STR.
        *   @return The message ID.
        */
        size_t insertMessage(Ogre::String& msg);
        /** Insert a message at the specified position on the STR.
        *   @remarks This will change all ids after this one.
        *   @param msg The message to put in the STR.
        *   @param position The position where you want to put it.
        */
        void insertMessage(Ogre::String& msg, size_t position);
        /// Remove a message from the back of the vector.
        void removeMessage();
        /** Remove a message from a specified position at the STR.
        *   @remarks This will change all ids after this one.
        */
        void removeMessage(size_t position);
    };

    class SONETTO_EXPORT STRDataPtr : public Ogre::SharedPtr<STRData>
    {
    public:
        STRDataPtr() : Ogre::SharedPtr<STRData>() {}
        explicit STRDataPtr(STRData *rep) : Ogre::SharedPtr<STRData>(rep) {}
        STRDataPtr(const STRDataPtr &r) : Ogre::SharedPtr<STRData>(r) {}
        STRDataPtr(const Ogre::ResourcePtr &r) : Ogre::SharedPtr<STRData>()
        {
            // lock & copy other mutex pointer
            OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
                OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
                pRep = static_cast<STRData*>(r.getPointer());
            pUseCount = r.useCountPointer();
            if (pUseCount)
            {
                ++(*pUseCount);
            }
        }

        /// Operator used to convert a ResourcePtr to a STRDataPtr
        STRDataPtr& operator=(const Ogre::ResourcePtr& r)
        {
            if (pRep == static_cast<STRData*>(r.getPointer()))
                return *this;
            release();
            // lock & copy other mutex pointer
            OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
                OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
                pRep = static_cast<STRData*>(r.getPointer());
            pUseCount = r.useCountPointer();
            if (pUseCount)
            {
                ++(*pUseCount);
            }
            return *this;
        }
    };

} // namespace

#endif // SONETTOSTR_H_INCLUDED
