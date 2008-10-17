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

#ifndef SONETTO_SCRIPTFILE_H
#define SONETTO_SCRIPTFILE_H

// Forward declarations
namespace Sonetto {
    class ScriptFile;
    class ScriptFilePtr;
}

#include <OgreSharedPtr.h>
#include <OgreResourceManager.h>
#include "SonettoMain.h"

namespace Sonetto
{
    typedef std::vector<size_t> LabelVector;
    typedef std::vector<char>   ScriptData;

    class SONETTO_EXPORT ScriptFile : public Ogre::Resource
    {
    public:
        ScriptFile(Ogre::ResourceManager *creator,const Ogre::String &name,
        Ogre::ResourceHandle handle,const Ogre::String &group,
        bool isManual = false,Ogre::ManualResourceLoader *loader = 0);

        virtual ~ScriptFile();

        LabelVector &_getLabels() { return mLabels; }

        ScriptData &_getScriptData() { return mScriptData; }

        size_t calculateSize() const;

    protected:
        // Ogre::Resource interface implementation
        void loadImpl();
        void unloadImpl();

        LabelVector mLabels;

        ScriptData mScriptData;
    };

    class SONETTO_EXPORT ScriptFilePtr : public Ogre::SharedPtr<ScriptFile>
    {
    public:
        ScriptFilePtr() : Ogre::SharedPtr<ScriptFile>() {}

        explicit ScriptFilePtr(ScriptFile *rep) : Ogre::SharedPtr<ScriptFile>(rep) {}

        ScriptFilePtr(const ScriptFilePtr &r) : Ogre::SharedPtr<ScriptFile>(r) {}

        ScriptFilePtr(const Ogre::ResourcePtr &r) : Ogre::SharedPtr<ScriptFile>()
        {
            // lock & copy other mutex pointer
            OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
                OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
                pRep = static_cast<ScriptFile*>(r.getPointer());
            pUseCount = r.useCountPointer();
            if (pUseCount)
            {
                ++(*pUseCount);
            }
        }

        /// Operator used to convert a ResourcePtr to a ScriptFilePtr
        ScriptFilePtr& operator=(const Ogre::ResourcePtr& r)
        {
            if (pRep == static_cast<ScriptFile*>(r.getPointer()))
                return *this;
            release();
            // lock & copy other mutex pointer
            OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
                OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
                pRep = static_cast<ScriptFile*>(r.getPointer());
            pUseCount = r.useCountPointer();
            if (pUseCount)
            {
                ++(*pUseCount);
            }
            return *this;
        }
    };
} // namespace

#endif // SONETTO_SCRIPTFILE_H
