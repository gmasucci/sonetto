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
#ifndef __SONETTO_COUNTER__
#define __SONETTO_COUNTER__

#include "SonettoMain.h"

#include <OGRE/OgreOverlayElement.h>
#include <OGRE/OgreOverlayElementFactory.h>
#include "SonettoMain.h"

namespace Sonetto {
    /** @brief Counter object for showing Integer values.
    @remarks
        Please note that setDimensions() function will
        be each Digit, not the entire object.
    */
    class SONETTO_EXPORT Counter : public Ogre::OverlayElement
    {
    public:
        /// @brief Constructor.
        Counter(const Ogre::String &name);
        /// @brief Destructor.
        virtual ~Counter();
        /// @brief Initialisation function.
        virtual void initialise(void);
        /** @brief Set this element flags.
        @remarks
            You must use bitwise operators to set the desired flags,
            note that the previous configuration will be overriden by this operation.
            To set the flags you must write something like this:
            setFlags(Sonetto::Counter::FLAG_SHOW_MAXIMUM_VALUE | Sonetto::Counter::FLAG_COLOR_NUMBERS);

            FLAG_SHOW_MAXIMUM_VALUE and FLAG_COLOR_NUMBERS are on by default.
        */
        void setFlags(unsigned char flags);
        /// @brief Return the current flag configuration.
        unsigned char getFlags();
        /** @brief Set the number of maximum digits this object will display.
        @remarks
            Note that the number of digits if the flag FLAG_SHOW_MAXIMUM_VALUE
            is on is the double plus one (for the separators)
            as it will show the maximum value too.
        */
        void setNumDigits(unsigned char num);
        /// @brief Return the number of digits this object will display.
        unsigned char getNumDigits();
        /** @brief Set the percentage threshold.
        @remarks
            If the current value is n% below the maximum value,
            the number will be shown in the specified color.
        */
        void setThreshold(char percentage);
        /// @brief Return the current threshold for this object.
        char getThreshold();
        /** @brief Set the current value pointer for this counter.
        @remarks
            This is the Current Value Pointer.
            The object will read from here every frame, so
            do not delete the original variable while this
            is pointer set.
        */
        void setCurValPtr(int * ptr);
        /// @brief Get the Current Value Pointer.
        int * getCurValPtr();
        /** @brief Set the maximum value pointer for this counter.
        @remarks
            This is the Maximum Value Pointer.
            The object will read from here every frame, so
            do not delete the original variable while this
            is pointer set.
        */
        void setMaxValPtr(int * ptr);
        /// @brief Get the Maximum Value Pointer.
        int * getMaxValPtr();

        /*  Those functions are temporary!
            Later, they will be replaced by
            a "System".
        */
        /// @brief Set the inclination level.
        void setInclination(Ogre::Real incl);
        /// @brief Set the Object colors.
        void setColors(const Ogre::ColourValue &normal,const Ogre::ColourValue &below,const Ogre::ColourValue &zero);
        /// @brief Set this object's material name.
        void setMaterialName(const Ogre::String& matName); // Overridden from OverlayElement.
        /* End of temporary functions */

        /// @brief Tells this element how to interpret the position and dimension values it is given.
        void setScrMetricsMode(ScreenMetricsMode smm);
        /// @brief Get this element metrics mode.
        ScreenMetricsMode getScrMetricsMode(void);
        /// @brief Get the object type name.
        virtual const Ogre::String& getTypeName(void) const; // See OverlayElement.
        /// @brief Return the Render Operation for this object.
        void getRenderOperation(Ogre::RenderOperation& op); // See Renderable.
        /// @brief Update Function
        void _update(void); // Overridden from OverlayElement.
        /// @brief Enum used to set the flags for this object
        enum CounterFlags {
            /// @brief Show Maximum Value (separated by an '/')
            FLAG_SHOW_MAXIMUM_VALUE = 0x01,
            /// @brief Show Zeroes in front of numbers. ( e.g: 0001 )
            FLAG_SHOW_ZEROES = 0x2,
            /// @brief Color numbers when current value is below n% of maximum value.
            FLAG_COLOR_NUMBERS = 0x04
        };
    private:
        /// @brief Digit Aspect data.
        Ogre::Real t_left, b_left, t_right, b_right, top, bottom;
        /// @brief Viewport Aspect Ratio.
        Ogre::Real mViewportAspect;
        /// @brief Counter Inclination Level.
        Ogre::Real mInclination;
        /// @brief Space allocated in memory for this object.
        unsigned char mAllocSize;
        /// @brief This Element's Screen Metrics Mode.
        ScreenMetricsMode mScrMetricsMode;
        /// @brief Object Type Name.
        static Ogre::String msTypeName;
        /// @brief Render Operation.
        Ogre::RenderOperation mRenderOp;
        /** @brief Counter configuration flags.
        @remarks
            The bits stands for the folowing flags:
            0x01 - Show Maximum Value (separated by an '/')
            0x02 - Show Zeroes in front of numbers. ( e.g: 0001 )
            0x04 - Color numbers when current value is below n% of maximum value.
        */
        unsigned char mFlags;
        /// @brief Maximum number of digits to show.
        unsigned char mNumDigits;
        /** @brief Threshold percentage.
        @remarks
             Values below n% of the maximun value will be shown in a different color.
             The percentage ranges from 0 to 100.
        */
        char mThreshold;
        /// @brief Pointer to a (int) value to display.
        int * mCurrentValue;
        /// @brief Pointer to a Maximum value to use as basis for calculations.
        int * mMaxValue;
        /// @brief Normal counter color.
        Ogre::ColourValue mNormalColor;
        /// @brief Value below threshold color.
        Ogre::ColourValue mBelowThrColor;
        /// @brief Zero value color.
        Ogre::ColourValue mZeroValColor;
        /// @brief Allocate the memory to hold the digits.
        void allocateMemory(char numchar);
        /// @brief Unused, Inherited function.
        virtual void updatePositionGeometry(); // Inherited function
        /// @brief Unused, Inherited function.
        virtual void updateTextureGeometry(); // Inherited function
        /// @brief Simple Power of Function.
        int simplePow(int a, char b);
        /// @brief Return the specified digit.
        char getDigit(int value, char idx);
        /// @brief Specialized function to draw the digits on screen.
        void drawDigit(float *& pvert, Ogre::RGBA *& pcolor, char digit, Ogre::RGBA color, float offset, float depth);
    };

    class SONETTO_EXPORT CounterFactory : public Ogre::OverlayElementFactory
    {
    public:
        /// @brief See OverlayElementFactory
        Ogre::OverlayElement* createOverlayElement(const Ogre::String& instanceName)
        {
            return new Sonetto::Counter(instanceName);
        }

        /// @brief See OverlayElementFactory
        const Ogre::String& getTypeName(void) const
        {
            static Ogre::String name = "Counter";
            return name;
        }
    };

} // namespace Sonetto

#endif // __SONETTO_COUNTER__
