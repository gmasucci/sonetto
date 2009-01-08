/*-----------------------------------------------------------------------------
Copyright (c) 2009, Sonetto Project Developers
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1.  Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
2.  Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
3.  Neither the name of the Sonetto Project nor the names of its contributors
    may be used to endorse or promote products derived from this software
    without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
-----------------------------------------------------------------------------*/

#ifndef SONETTO_STATICTEXTELEMENT_H
#define SONETTO_STATICTEXTELEMENT_H

#include "SonettoPrerequisites.h"
#include "SonettoFont.h"
#include <OgreOverlayElement.h>
#include <OgreOverlayElementFactory.h>

namespace Sonetto
{

    enum StringCommandType
    {
        SCT_NEWLINE = 0x0A,
        SCT_PRE_PROCESSING = 0x1A,
        SCT_POST_PROCESSING = 0x1B,
        SCT_WHITESPACE = 0x20
    };
    enum StringCommandId
    {
        // ----------------------
        // Pre Processing Opcodes
        // ----------------------
        // Hero Namme
        SCI_HERO_NAME,
        // Variable
        SCI_VARIABLE_INT, // Variable is Int Type
        SCI_VARIABLE_FLOAT, // Variable is Float Type
        // Party Status
        SCI_PARTY_GOLD, // Party Gold
        SCI_PARTY_HOURS, // Gameplay Hours
        SCI_PARTY_MINUTES, // Gameplay Minutes
        SCI_PARTY_SECONDS, // Gameplay Seconds
        // Item Name
        SCI_ITEM_NAME,
        // Item Quantity
        SCI_ITEM_QUANTITY,
        // (Hero Equipment Name
        SCI_HERO_WEAPON, // Hero's Weapon
        SCI_HERO_HAND, // Hero's Hand (Shield)
        SCI_HERO_HEAD, // Hero's Head (Helmet, Hats, etc.)
        SCI_HERO_BODY, // Hero's Body (Armor, Cloth)
        SCI_HERO_ACCESSORY1, // Hero's Accessory Slot 1
        SCI_HERO_ACCESSORY2, // Hero's Accessory Slot 2
        // Hero Attributes
        SCI_HERO_LEVEL, // Hero Current Level
        SCI_HERO_HP, // Hero Max HP
        SCI_HERO_CUR_HP, // Hero Current HP
        SCI_HERO_MP, // Hero Max MP
        SCI_HERO_CUR_MP, // Hero Current MP
        SCI_HERO_EXP, // Hero Current Exp
        SCI_HERO_EXP_NEXT_LEVEL, // Hero Required Exp Till Next Level

        SCI_HERO_STR, // Hero Strenght
        SCI_HERO_ATK, // Hero Attack (Combination of STR + ATK)
        SCI_HERO_INT, // Hero Intelligence
        SCI_HERO_DEF, // Hero Defence Strenght
        SCI_HERO_ACC, // Hero Accuracy
        SCI_HERO_EVA, // Hero Evade
        SCI_HERO_LCK, // Hero Luck
        // Item Attributes
        SCI_ITEM_HP,
        SCI_ITEM_MP,
        SCI_ITEM_ATK,
        SCI_ITEM_INT,
        SCI_ITEM_DEF,
        SCI_ITEM_ACC,
        SCI_ITEM_EVA,
        SCI_ITEM_LCK,
        // ----------------------
        // Post Processing Opcodes
        // ----------------------
        // Color Opcodes
        SCI_COLOR_CHANGE, // Change current text color
        SCI_COLOR_RESET, // Reset the current color to default
        // Text Animation Speed Opcodes
        SCI_SPEED_UP, // Speed up text animation speed
        SCI_SPEED_DOWN, // Speed down text animation speed
        SCI_SPEED_RESET, // Reset text speed to default speed
        SCI_SPEED_SET, // Set the text speed to the defined value
        // Skip Animation Opcodes
        SCI_SKIP_START, // Start skipping characters between this and SCI_SKIP_END
        SCI_SKIP_END, // Finish skipping animation.
        // Icon Opcodes
        SCI_ICON, // Show an icon.

        SCI_LAST // Allways keep this at last, used to check the number of opcodes.
    };

    class SONETTO_API StaticTextElement : public Ogre::OverlayElement
    {
    public:
        StaticTextElement(const Ogre::String& name);
        virtual ~StaticTextElement();

        /// Initialise static text element
        virtual void initialise(void);

        /// Allocate memory for text, allocated memory will only be freed when the element is deleted
        void allocateMemory(size_t numchars);

        virtual const Ogre::String& getTypeName(void) const;

        void getRenderOperation(Ogre::RenderOperation& op);

        Ogre::MaterialPtr & getMaterial(void) const;

        /// Internal method to update the element based on transforms applied.
        void _update(void);
    protected:
        bool mInitialized;
        size_t mStringSize;
        size_t mAllocatedSize;
    public:
        std::string mText;
        float mAlpha;
        Ogre::Vector2 mTextSize;
        /// Pointer to a pointer to the current font (so you don't need to dereference the database each frame)
        Font * mpFont;
    protected:
        /// Render operation
        Ogre::RenderOperation mRenderOp;
        /// Type name
        static Ogre::String msTypeName;

        virtual void updatePositionGeometry(){}
        virtual void updateTextureGeometry(){}
    };

    class SONETTO_API StaticTextElementFactory: public Ogre::OverlayElementFactory
    {
    public:
        /// @brief See OverlayElementFactory
        Ogre::OverlayElement* createOverlayElement(const Ogre::String& instanceName)
        {
            return new StaticTextElement(instanceName);
        }
        /// @brief See OverlayElementFactory
        const Ogre::String& getTypeName(void) const
        {
            static Ogre::String name = "StaticText";
            return name;
        }
    };

} // namespace

#endif // SONETTO_STATICTEXTELEMENT_H
