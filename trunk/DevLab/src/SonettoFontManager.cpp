#include "SonettoFontManager.h"

namespace Sonetto
{
    using namespace Ogre;
    template<> FontManager *Ogre::Singleton<FontManager>::ms_Singleton = 0;

    FontManager *FontManager::getSingletonPtr()
    {
        return ms_Singleton;
    }

    FontManager &FontManager::getSingleton()
    {
        assert(ms_Singleton);
        return(*ms_Singleton);
    }

    FontManager::FontManager()
    {
        mResourceType = "Font";

        // low, because it will likely reference other resources
        mLoadOrder = 30.0f;

        // this is how we register the ResourceManager with OGRE
        Ogre::ResourceGroupManager::getSingleton()._registerResourceManager(mResourceType, this);
    }

    FontManager::~FontManager()
    {
        // and this is how we unregister it
        Ogre::ResourceGroupManager::getSingleton()._unregisterResourceManager(mResourceType);
    }

    FontPtr FontManager::load(const Ogre::String &name, const Ogre::String &group)
    {
        FontPtr fontf = getByName(name);

        if (fontf.isNull())
            fontf = create(name, group);

        fontf->load();
        return fontf;
    }

    Ogre::Resource *FontManager::createImpl(const Ogre::String &name, Ogre::ResourceHandle handle,
            const Ogre::String &group, bool isManual, Ogre::ManualResourceLoader *loader,
            const Ogre::NameValuePairList *createParams)
    {
        return new Font(this, name, handle, group, isManual, loader);
    }
} // namespace
