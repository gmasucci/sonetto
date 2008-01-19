#include "SonettoSTRManager.h"

namespace Sonetto
{
    template<> STRManager *Ogre::Singleton<STRManager>::ms_Singleton = 0;
    STRManager *STRManager::getSingletonPtr()
    {
        return ms_Singleton;
    }
    STRManager &STRManager::getSingleton()
    {
        assert(ms_Singleton);
        return(*ms_Singleton);
    }
    STRManager::STRManager()
    {
        mResourceType = "STR";

        // low, because it will likely reference other resources
        mLoadOrder = 30.0f;

        // Register the ResourceManager with OGRE
        Ogre::ResourceGroupManager::getSingleton()._registerResourceManager(mResourceType, this);
    }
    STRManager::~STRManager()
    {
        // Unregister the ResourceManager from OGRE
        Ogre::ResourceGroupManager::getSingleton()._unregisterResourceManager(mResourceType);
    }
    STRDataPtr STRManager::load(const Ogre::String &name, const Ogre::String &group)
    {
        STRDataPtr strdata = getByName(name);

        if (strdata.isNull())
            strdata = create(name, group);

        strdata->load();
        return strdata;
    }
    Ogre::Resource *STRManager::createImpl(const Ogre::String &name, Ogre::ResourceHandle handle,
                                           const Ogre::String &group, bool isManual, Ogre::ManualResourceLoader *loader,
                                           const Ogre::NameValuePairList *createParams)
    {
        return new STRData(this, name, handle, group, isManual, loader);
    }
} // namespace
