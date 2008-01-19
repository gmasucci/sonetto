#ifndef SONETTOWINDOWFACTORY_H_INCLUDED
#define SONETTOWINDOWFACTORY_H_INCLUDED

#include <OgreOverlayElementFactory.h>
#include "SonettoWindow.h"
#include "SonettoTailedWindow.h"
#include "SonettoSlimWindow.h"

namespace Sonetto
{
    using namespace Ogre;
    class WindowFactory: public OverlayElementFactory
    {
    public:
        /** See OverlayElementFactory */
        OverlayElement* createOverlayElement(const String& instanceName)
        {
            return new Sonetto::Window(instanceName);
        }
        /** See OverlayElementFactory */
        const String& getTypeName(void) const
        {
            static String name = "Window";
            return name;
        }
    };

    class TailedWindowFactory: public OverlayElementFactory
    {
    public:
        /** See OverlayElementFactory */
        OverlayElement* createOverlayElement(const String& instanceName)
        {
            return new Sonetto::TailedWindow(instanceName);
        }
        /** See OverlayElementFactory */
        const String& getTypeName(void) const
        {
            static String name = "TailedWindow";
            return name;
        }
    };

    class SlimWindowFactory: public OverlayElementFactory
    {
    public:
        /** See OverlayElementFactory */
        OverlayElement* createOverlayElement(const String& instanceName)
        {
            return new Sonetto::SlimWindow(instanceName);
        }
        /** See OverlayElementFactory */
        const String& getTypeName(void) const
        {
            static String name = "SlimWindow";
            return name;
        }
    };
} // namespace

#endif // SONETTOWINDOWFACTORY_H_INCLUDED
