#ifndef SONETTOSTRMANAGER_H_INCLUDED
#define SONETTOSTRMANAGER_H_INCLUDED

#include <OgreResourceManager.h>
#include "SonettoSTR.h"

namespace Sonetto
{
    class STRManager : public Ogre::ResourceManager, public Ogre::Singleton<STRManager>
    {
    protected:
        // must implement this from ResourceManager's interface
        Ogre::Resource *createImpl(const Ogre::String &name, Ogre::ResourceHandle handle,
            const Ogre::String &group, bool isManual, Ogre::ManualResourceLoader *loader,
            const Ogre::NameValuePairList *createParams);

    public:

        STRManager();
        virtual ~STRManager();

        virtual STRDataPtr load(const Ogre::String &name, const Ogre::String &group);

        static STRManager &getSingleton();
        static STRManager *getSingletonPtr();

    };
} // namespace

#endif // SONETTOSTRMANAGER_H_INCLUDED
