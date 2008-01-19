#ifndef SONETTOTEXTFACTORY_H_INCLUDED
#define SONETTOTEXTFACTORY_H_INCLUDED

#include <Ogre.h>
#include <OgrePrerequisites.h>
#include <OgreOverlayElement.h>
#include "SonettoTextElement.h"

namespace Sonetto
{
    using namespace Ogre;

    /** Factory for creating CustomTextElement instances. */
    class TextElementFactory: public OverlayElementFactory
    {
    public:
        /** See OverlayElementFactory */
        OverlayElement* createOverlayElement(const String& instanceName)
        {
            return new Sonetto::TextElement(instanceName);
        }
        /** See OverlayElementFactory */
        const String& getTypeName(void) const
        {
            static String name = "Text";
            return name;
        }
    };

} // namespace

#endif // SONETTOTEXTFACTORY_H_INCLUDED
