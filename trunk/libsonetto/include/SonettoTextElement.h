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

#include <OgreOverlayElement.h>

#include "SonettoFont.h"

namespace Sonetto
{

    #define DEFAULT_INITIAL_CHARS           16
    #define NEW_LINE_CHARACTER              '\n'
    #define SPACE_CHARACTER                 ' '
    #define TEXT_COMMAND                    0x5C
    #define TEXT_COMMAND_OPEN               '['
    #define TEXT_COMMAND_CLOSE              ']'
    #define TEXT_COLOR_COMMAND              'c'
    #define TEXT_COLOR_COMMAND_MAX_DIGITS   4
    #define TEXT_COLOR_COMMAND_HEX_FLAG     '#'
    #define TEXT_SPEED_COMMAND              's'
    #define TEXT_SPEED_COMMAND_UP           'U'
    #define TEXT_SPEED_COMMAND_DOWN         'D'
    #define TEXT_SPEED_COMMAND_DEFAULT      'N'
    #define TEXT_ITEM_COMMAND               'i'

    /** Enum containing the Text States. */
    enum TextState
    {
        /// The text is not initialised.
        TS_NOT_INITIALISED,
        /// The text is initialised, and ready to be shown.
        TS_INITIALISED,
        /// The text is currently beign animated.
        TS_ANIMATION_ACTIVE,
        /// The text is currently paused.
        TS_ANIMATION_PAUSED,
        /// The text animation has finished.
        TS_ANIMATION_FINISHED,
        /// The text is static and is beign shown.
        TS_STATIC
    };

    class TextIconSetRenderable; // Forward declaration

    class SONETTO_EXPORT TextElement : public Ogre::OverlayElement
    {
    friend class TextIconSetRenderable;
    public:
        /// Constructor.
        TextElement(const Ogre::String& name);
        /// Destructor.
        virtual ~TextElement();

        /// Object initialisation function.
        virtual void initialise(void);

        /** Set the TextElement message
        @remarks
            This will automatically reset the current text animation, if any.
        @param
            text Pointer to a String containing the message to show.
        */
        virtual void setCaption(const Ogre::String & text); // From OverlayElement

        /** Set the TextElement message.
        @see
            TextElement::setCaption()
        @param
            text Pointer to a String containing the message to show.
        */
        void setMessage(const Ogre::String & text);

        /** Set the TextElement message.
        @see
            TextElement::setCaption()
        @param
            text Pointer to a String containing the message to show.
        */
        void setMessage(const Ogre::String * text);

        /** Set the font for this TextElement.
        @param
            font The pointer to a Sonetto::Font.
        */
        void setFont(FontPtr font);
        /** Get the current font set to this object.
        @return
            A FontPtr containing a pointer to a Sonetto::Font.
        */
        FontPtr getFont();

        /** Set the Animation Settings configuration for this object.
        @remarks
            You also may set each configuration individually using their respective functions.
            Note that you can change this configuration anytime, but be aware that it may cause errors.
        @param
            isAnim Set wether this TextElement will show letter-by-letter animation.
        @param
            useFade Enable or disable fade in, if isAnim is enablet, this will fade in every letter.
        @param
            useFOut Enable or disable fade out, will fade out text when animation has finished.
        @param
            skipWS If true, the animation will skip whitespaces.
        @param
            anmSpd Set the Letter-by-letter Animation Speed.
        @param
            fdSpd Set the Fade In speed.
        @param
            fdOutSpd Set the Fade Out speed.
        */
        void setAnimSettings(   bool isAnim, bool useFade, bool useFOut,
                                bool skipWS, Ogre::Real anmSpd, Ogre::Real fdSpd,
                                Ogre::Real fdOutSpd );

        /// Set wether this TextElement will show letter-by-letter animation.
        void setAnimated(bool isAnim);

        /// Get if this TextElement is animated.
        bool getAnimated();

        /// Enable or disable fade in, if isAnim is enablet, this will fade in every letter.
        void setUseFade(bool useFade);

        /// Get if this object use Fade In.
        bool getUseFade();

        /// Enable or disable fade out, will fade out text when animation has finished.
        void setUseFadeOut(bool useFOut);

        /// Get if this object use Fade Out.
        bool getUseFadeOut();

        /// Set the Animation speed.
        void setAnimSpeed(Ogre::Real anmSpd);

        /// Get Animation speed.
        bool getAnimSpeed();

        /// Set the Fade In speed.
        void setFadeSpeed(Ogre::Real fdSpd);

        /// Get Fade In speed.
        Ogre::Real getFadeSpeed();

        /// Set the Fade Out speed.
        void setFadeOutSpeed(Ogre::Real fdOutSpd);

        /// Get Fade Out speed.
        Ogre::Real getFadeOutSpeed();

        /** Set the text size for this object.
        @remarks
            The size value is related to the MetricsMode,
            if you're using SMM_RELATIVE, 1.0f will be the size of the entire screen.
            You may calculate the correct size by dividing the wanted size in pixels by
            the screen resolution, for instance, 640x480, you may calculate like this:
            x: x / 640.0f and for y:  y / 480.0f.
        @param
            size The size value.
        */
        void setTextSize(Ogre::Vector2 size);

        /** Get the current text size.
        @return
            The current X and Y size of the text.
        */
        Ogre::Vector2 getTextSize();

        /** Set the space between lines.
        @remarks
            This is the space between lines,
            a value of 1.0f will be the exact size of the text,
            values smaller than 1.0f will make the text closer.
        */
        void setLineDistance(Ogre::Real size);

        /** Get the space between lines.
        @return
            The space between lines, relative to the text size.
        */
        Ogre::Real getLineDistance();

        /** Force animation to end.
        @remarks
            This will make the entire text show at once
            without animation or fade.
        */
        void forceAnimEnd();

        /** Force animation to restart.
        @remarks
            This will force the letter-by-letter animation to
            restart, but will retain the current text and configurations.
        */
        void forceAnimReset();

        /// Return this object TypeName, for Ogre internal use.
        virtual const Ogre::String& getTypeName(void) const; // See OverlayElement.

        /// Return this object RenderOperation, for Ogre internal use.
        virtual void getRenderOperation(Ogre::RenderOperation& op); // See Renderable.

        /// Set the material name.
        virtual void setMaterialName(const Ogre::String& matName); // Overridden from OverlayElement.

        /** Set the Icon Set for this text element.
        @remarks
            You must set this up before showing the text.
        @param
            Pointer to the IconSet.
        */
        void setIconSet(IconSet * icn);

        /** Get the Icon Set.
        @return
            A pointer to the Icon Set.
        */
        IconSet * getIconSet();

        /** Set the text base alpha.
        @remarks
            This will set the text final alpha, does not affect the fade in nor the fade out.
        */
        void setAlpha(Ogre::Real alpha);

        /** Get the current base text alpha.
        @remarks
            This will get the final alpha, not the current fade alpha.
        */
        Ogre::Real getAlpha();

        /** Set the text color.
        @remarks
            This will set the text standard text color,
            this color is stored inside the Sonetto::Font.
        */
        void setColor(size_t colorID);

        /** Get the current text color.
        @remarks
            this will only return the current text standard color,
            not the ones modifyed by the string commands.
        */
        size_t getColor();

        /** Calculate the Text Dimensions on the screen.
        @remarks
            This function will calculate the screen size of this
            text, taking in account all the configurations of its Font.
        @remarks
            Note that the result depends on the Screen Metrics Mode.
        @return
            The text screen dimensions.
        */
        Ogre::Vector2 getTextDimensions();

        /** Set the Screen Metrics Mode for this object.
        @param
            smm The desired ScreenMetricsMode.
        */
        void setScrMetricsMode(ScreenMetricsMode smm);

        /** Get the Screen Metrics Mode.
        @return
            The current Screen Metrics Mode.
        */
        ScreenMetricsMode getScrMetricsMode();

        /** Get the specified string lenght on screen.
        @param
            line Pointer to a char array containing a line
        @return
            The line lenght, in screen coordinates.
        */
        Ogre::Real getLineLenght(const char * line);

        /** Get the largest line on a string.
        @param
            str A string array.
        @return
            The and array containing the largest line in the string.
        */
        const char * getLargestLine(const char * str);

        /** Get the lenght of the largest line on a string.
        @remarks
            This functions uses the getLineLenght and getLargestLine internally.
        @param
            str The String to be calculated.
        @return
            The largest line lenght, in screen coordinates.
        */
        Ogre::Real getLargestLineLenght(const char * str);

        /** Get the heigh of the text.
        @return
            The string height on screen coordinates.
        */
        Ogre::Real getStringHeight(const char * str);

        /** Start Text Animation.
        @remarks
            This function will start the text animation.
        @remarks
            This function only works for animated text.
        */
        void startAnimation();

        /** Stop Text Animation.
        @remarks
            This will stop the text animation,
            upon stopping, it will reset the text,
            if you call startAnimation() it will start over,
            you may want to use pauseAnimation() and resumeAnimation()
            to pause and resume the text animation.
        */
        void stopAnimation();

        /// Pause Text Animation.
        void pauseAnimation();

        /// Resume Text Animation.
        void resumeAnimation();

        /// Show text element, only on elements that have no animation.
        void showText();

        /// Hide text element, only on elements that have no animation.
        void hideText();

        /** Get the Text State
        @return
            Return the current text state.
        */
        TextState getTextState();

        /** Update function.
        @remarks
            This function is called internally by Ogre, you should not call this.
        */
        virtual void _update(void); // Overridden from OverlayElement.

    protected:
        /// Check if the object has been initialised already.
        bool mInitialised;
        /// Type Name, for Ogre internal use.
        static Ogre::String msTypeName;

        /// Actual text.
        Ogre::String mText;

        /// Fade In list, for letter-by-letter animation.
        std::vector<Ogre::Real> mFadeList;

        /// Text Base Alpha.
        Ogre::Real mAlpha;

        /// Render Operation.
        Ogre::RenderOperation mRenderOp;

        /// Render Operation, for Icon Set.
        Ogre::RenderOperation mIconSetRenderOp;

        /// Icon Set Renderable
        TextIconSetRenderable * mpIconSetRenderable;

        /// Icon Set Pointer.
        IconSet * mpIconSet;

        /// Font Pointer.
        FontPtr mFontPtr;

        /// Current Text State
        TextState mTextState;

        /// Current memory allocation size for characters.
        size_t mCharAllocSize;

        /// Current memory allocation size for icons.
        size_t mIconAllocSize;

        /// Number of printable characters in the string.
        size_t mCharStringSize;

        /// Number of icons in the string.
        size_t mIconStringSize;

        /// Current String Size, Number of printable character plus number of icons.
        size_t mTotalStringSize;

        /// Current cursor position on text animation.
        size_t mStrCursorPosition;

        /// Frame Time.
        Ogre::Real mTimeSinceLastFrame;

        /// Text Animation Difference
        Ogre::Real mDifference;

        /// Viewport Aspect Coefficient.
        Ogre::Real mViewportAspectCoef;

        /// Screen Metrics Mode.
        ScreenMetricsMode mScrMetricsMode;

        /// Current Text Size.
        Ogre::Vector2 mTextSize;

        /// Current distance between lines.
        Ogre::Real mLineSize;

        /// Current cursor Color Value, used for printing texts with more than one color.
        Ogre::ColourValue mCurTextColor;

        /// Is Animated: 0 - Show all at once, 1 - Show letter by letter.
        bool mIsAnimated;
        /// Use Fade In: 0 - Don't use fade in, 1 - Use fade in.
        bool mUseFadeIn;
        /// Use Fade Out: 0 - Disappear at once, 1 - Fade the whole text out.
        bool mUseFadeOut;
        /// Skip White Spaces: 0 - Don't skip, 1 - Skip. If enablet, the animation will skip whitespaces.
        bool mSkipWhiteSpaces;

        /// Speed wich the text will appear on the screen.
        Ogre::Real mAnimSpeed;
        /// Default Speed wich the text will appear on the screen.
        Ogre::Real mDefAnimSpeed;
        /// Speed wich the text will fade on the screen.
        Ogre::Real mFadeSpeed;
        /// Speed wich the text will fade on the screen.
        Ogre::Real mFadeOutSpeed;

        /// Default TexColor
        size_t mDefaultColor;

        /** Allocate Fade List.
        @remarks
            Allocate a fade list for fade in animation.
        @param
            size The size of the string.
        @param
            value Initial value for every character in the fade list.
        */
        void allocateFadeList(size_t size, Ogre::Real value = 0.0f);

        /** Animate function.
        @remarks
            Function used to manage the text animation.
        */
        void animate();

        /** Allocate Memory.
        @remarks
            Internal method to allocate memory, only reallocates when necessary.
        @param
            num_chars Number of characters to check allocation.
            @param
            num_chars Number of icons to check allocation.
        */
        void checkMemoryAllocation(size_t num_chars,size_t num_icons);

        /** Get String Size.
        @remarks
            This will calculate the total string size, skipping all commands, zero characters and newlines.
        @param
            a_str String to calculate.
        @param
            max_len Maximum lenght to test the string, zero to test everything.
        */
        size_t getStrSize(Ogre::String &a_str, size_t max_len = 0);

        /** Get the number of characters in the string.
        @param
            a_str String to calculate.
        @param
            max_len Maximum lenght to test the string, zero to test everything.
        */
        size_t getNumStringChars(Ogre::String &a_str, size_t max_len = 0);

        /** Get the number of icons in the string.
        @param
            a_str String to calculate.
        @param
            max_len Maximum lenght to test the string, zero to test everything.
        */
        size_t getNumStringIcons(Ogre::String &a_str, size_t max_len = 0);

        /** Parse a text command.
        @param
            itr Iterator to the string containing the command.
        @param
            countonly If true will only count the number of character the command contains without advancing the original iterator.
        @return
            if
        */
        size_t parseTextCommand(std::string::iterator &itr,
                                size_t &offset,
                                Ogre::Real &txtPosX,
                                Ogre::Real txtPosY,
                                Ogre::Real zvalue,
                                Ogre::Real square[8],
                                float ** pVert,
                                bool countonly = false);

        size_t getValueTillClose(std::string::iterator &itr, size_t & offset, size_t max = TEXT_COLOR_COMMAND_MAX_DIGITS);

        Ogre::ColourValue getColorFromString(std::string::iterator &itr, size_t & offset);

        virtual void updatePositionGeometry(void) {}
        virtual void updateTextureGeometry(void) {}
    };
    /** Class for rendering the text Icon Set.
    @remarks
    We need this because we have to render twice, once with the text Font material,
    other with the Icon Set material.
    */
    class SONETTO_EXPORT TextIconSetRenderable : public Ogre::Renderable
    {
    protected:
        TextElement *mpParent;
    public:
        /// Constructed with pointers to parent.
        TextIconSetRenderable(TextElement* parent) : mpParent(parent)
        {
            mUseIdentityProjection = true;
            mUseIdentityView = true;
        }
        /// Get the Icon Set material.
        const Ogre::MaterialPtr& getMaterial(void) const
        {
            return mpParent->mpIconSet->iconMaterial;
        }
        /// Get the Render Operation.
        void getRenderOperation(Ogre::RenderOperation& op)
        {
            op = mpParent->mIconSetRenderOp;
        }
        /// Get the World Transforms.
        void getWorldTransforms(Ogre::Matrix4* xform) const
        {
            mpParent->getWorldTransforms(xform);
        }
        /// Get the World Orientation.
        const Ogre::Quaternion& getWorldOrientation(void) const { return Ogre::Quaternion::IDENTITY; }
        /// Get the World Position.
        const Ogre::Vector3& getWorldPosition(void) const { return Ogre::Vector3::ZERO; }
        /// Get the Number of World Transforms.
        unsigned short getNumWorldTransforms(void) const
        {
            return 1;
        }
        /// Get the Squared View Depth.
        Ogre::Real getSquaredViewDepth(const Ogre::Camera* cam) const
        {
            return mpParent->getSquaredViewDepth(cam);
        }
        /// Get the Lithgs affecting this element, in this case, none.
        const Ogre::LightList& getLights(void) const
        {
            // N/A, panels are not lit
            static Ogre::LightList ll;
            return ll;
        }
        /// Get the Polygon Mode.
        bool getPolygonModeOverrideable(void) const
        {
            return mpParent->getPolygonModeOverrideable();
        }
    };
} // namespace
#endif // SONETTOTEXTELEMENT_H_INCLUDED
