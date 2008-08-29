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
#ifndef SONETTOTEXTELEMENT_H_INCLUDED
#define SONETTOTEXTELEMENT_H_INCLUDED

#include "SonettoMain.h"

#include <OgreOverlayElement.h>
#include <OgreControllerManager.h>
#include "SonettoMain.h"
#include "SonettoFontManager.h"

namespace Sonetto
{
    using namespace Ogre;

    class SONETTO_EXPORT TextElement : public OverlayElement
    {
    public:
        size_t deleteme; // Delete this later when the animate() function being implemented
        TextElement(const String& name);
        virtual ~TextElement();

        virtual void initialise(void);
        virtual void setCaption(const Ogre::String & text); // From OverlayElement
        void setMessage(const Ogre::String & text); // From OverlayElement
        void setMessage(const Ogre::String * text); // From OverlayElement

        //virtual void setCaption(const Ogre::String * text);

        void setFont(FontPtr font);
        FontPtr getFont();

        void setAnimSettings(bool isAnim, bool useFade, bool useFOut, Real anmSpd, Real fdSpd);

        void setTextSize(Real size);
        Real getTextSize();

        void forceAnimEnd();
        void forceAnimReset();

        virtual const String& getTypeName(void) const; // See OverlayElement.
        void getRenderOperation(RenderOperation& op); // See Renderable.
        void setMaterialName(const String& matName); // Overridden from OverlayElement.
        void _update(void); // Overridden from OverlayElement.

        // Aditional, ultility functions
        /// @brief Get the specified string lenght on screen
        Ogre::Real getLineLenght(const char * line);
        /// @brief Get the largest line on a string
        const char * getLargestLine(const char * str);
        /// @brief Get the lenght of the largest line on a string
        Ogre::Real getLargestLineLenght(const char * str);
        /// @brief Get the heigh of the text
        Ogre::Real getStringHeight(const char * str);
        /// @brief Return the dimensions of the text on the screen
        Ogre::Vector2 getTextDimensions();

        void setAlpha(Ogre::Real alpha);

        Ogre::Real getAlpha();

        void setColor(size_t colorID);

        size_t getColor();

    protected:
        // Member Variables
        static String msTypeName; // Type name

        // Text data
        std::string mCaption; // Actual text
        std::vector<Real> mFadeList; // Fade list
        Ogre::Real mFadeLevel;
        Ogre::Real mObjFade; // This is the maximum fade for the text, useful for fade in animation with windows.

        RenderOperation mRenderOp;// Render operation
        FontPtr mFontPtr; // Font pointer

        // Text settings
        size_t mAllocSize; // Memory allocation size
        size_t mStringSize; // String size (both string and event flags must be the same size)
        size_t mStrCursorPosition; // Current cursor position on text animation
        Real mTextSize; // Text size on the screen
        Real mNewLineSize; // Text new line size relative to the mTextSize
        ColourValue mCurTextColor; // The current text color, used for printing

        Ogre::Real mTimeSinceLastFrame;
        Ogre::Real mDifference;

        // Viewport settings
        Real mViewportAspectCoef; // Viewport aspect coefficient

        // Screen Metrics Mode
        ScreenMetricsMode mScrMetricsMode;

        // Animation settings
        bool mIsAnimated; // 0 - Show all at once, 1 - Show letter by letter
        bool mUseFadeIn; // 0 - Don't use fade in, 1 - Use fade in
        //bool mFadeInType; // 0 - by letters, 1 - whole
        bool mUseFadeOut; // 0 - Disappear at once, 1 - Fade the whole text out
        bool mSkipWhiteSpaces; // 0 - Don't skip, 1 - Skip
        bool mAnimationActive; // 0 - No animation beign performed, 1 - Animation is active

        Real mAnimSpeed; // Speed wich the text will appear on the screen
        Real mDefAnimSpeed; // Default Speed wich the text will appear on the screen
        Real mFadeSpeed; // Speed wich the text will fade on the screen

        size_t mDefaultColor; // Default Text Color ID

        // Member Functions
        void allocateFadeList(size_t size, float value = 0.0f); // Allocate the fade list
        void animate(); // Function used to manage the text animation
        void checkMemoryAllocation(size_t num_chars); // Internal method to allocate memory, only reallocates when necessary
        void phraseText(); // Phrase the text and generate the display text and event flags
        size_t getStrSize(std::string &a_str, size_t max_len = 0); // Return the string size (skipping the commands, zero characters and newlines)
        virtual void updatePositionGeometry(); // Inherited function
        virtual void updateTextureGeometry(); // Inherited function
    };

} // namespace
#endif // SONETTOTEXTELEMENT_H_INCLUDED
