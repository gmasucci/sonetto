#ifndef FONTMANAGER_H_INCLUDED
#define FONTMANAGER_H_INCLUDED

#include <OgreResourceManager.h>
#include "SonettoFont.h"
namespace Sonetto
{
    using namespace Ogre;
    class FontManager : public Ogre::ResourceManager, public Ogre::Singleton<FontManager>
    {
    protected:

        // must implement this from ResourceManager's interface
        Ogre::Resource *createImpl(const Ogre::String &name, Ogre::ResourceHandle handle,
                                   const Ogre::String &group, bool isManual, Ogre::ManualResourceLoader *loader,
                                   const Ogre::NameValuePairList *createParams);

    public:

        FontManager();
        virtual ~FontManager();

        virtual FontPtr load(const Ogre::String &name, const Ogre::String &group);

        static FontManager &getSingleton();
        static FontManager *getSingletonPtr();
    };

} // namespace
#endif // FONTMANAGER_H_INCLUDED
