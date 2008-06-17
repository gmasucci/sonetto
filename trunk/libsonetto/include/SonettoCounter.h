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
    /** Counter object for showing Integer values.
    @remarks
        Please note that setDimensions() function will
        be each Digit, not the entire object.
    */
    class SONETTO_EXPORT Counter : public Ogre::OverlayElement
    {
    public:
        /// Construct SONETTO_EXPORTor.
        Counter(const Ogre::String &name);
        /// Destruct SONETTO_EXPORTor.
        virtual ~Counter();
        /// Initialisation function.
        virtual void initialise(void);
        /** Set this element flags.
        @remarks
            You must use bitwise operators to set the desired flags,
            note that the previous configuration will be overriden by this operation.
            To set the flags you must write something like this:
            setFlags(Sonetto::Counter::FLAG_SHOW_MAXIMUM_VALUE | Sonetto::Counter::FLAG_COLOR_NUMBERS);

            FLAG_SHOW_MAXIMUM_VALUE and FLAG_COLOR_NUMBERS are on by default.
        */
        void setFlags(unsigned char flags);
        /// Return the current flag configuration.
        unsigned char getFlags();
        /** Set the number of maximum digits this object will display.
        @remarks
            Note that the number of digits if the flag FLAG_SHOW_MAXIMUM_VALUE
            is on is the double plus one (for the separators)
            as it will show the maximum value too.
        */
        void setNumDigits(unsigned char num);
        /// Return the number of digits this object will display.
        unsigned char getNumDigits();
        /** Set the percentage threshold.
        @remarks
            If the current value is n% below the maximum value,
            the number will be shown in the specified color.
        */
        void setThreshold(char percentage);
        /// Return the current threshold for this object.
        char getThreshold();
        /** Set the current value pointer for this counter.
        @remarks
            This is the Current Value Pointer.
            The object will read from here every frame, so
            do not delete the original variable while this
            is pointer set.
        */
        void setCurValPtr(int * ptr);
        /// Get the Current Value Pointer.
        int * getCurValPtr();
        /** Set the maximum value pointer for this counter.
        @remarks
            This is the Maximum Value Pointer.
            The object will read from here every frame, so
            do not delete the original variable while this
            is pointer set.
        */
        void setMaxValPtr(int * ptr);
        /// Get the Maximum Value Pointer.
        int * getMaxValPtr();

        /*  Those functions are temporary!
            Later, they will be replaced by
            a "System".
        */
        /// Set the inclination level.
        void setInclination(Ogre::Real incl);
        /// Set the Object colors.
        void setColors(const Ogre::ColourValue &normal,const Ogre::ColourValue &below,const Ogre::ColourValue &zero);
        /// Set this object's material name.
        void setMaterialName(const Ogre::String& matName); // Overridden from OverlayElement.
        /* End of temporary functions */

        /// Tells this element how to interpret the position and dimension values it is given.
        void setScrMetricsMode(ScreenMetricsMode smm);
        /// Get this element metrics mode.
        ScreenMetricsMode getScrMetricsMode(void);
        /// Get the object type name.
        virtual const Ogre::String& getTypeName(void) const; // See OverlayElement.
        /// Return the Render Operation for this object.
        void getRenderOperation(Ogre::RenderOperation& op); // See Renderable.
        /// Update Function
        void _update(void); // Overridden from OverlayElement.
        /// Enum used to set the flags for this object
        enum CounterFlags {
            /// Show Maximum Value (separated by an '/')
            FLAG_SHOW_MAXIMUM_VALUE = 0x01,
            /// Show Zeroes in front of numbers. ( e.g: 0001 )
            FLAG_SHOW_ZEROES = 0x2,
            /// Color numbers when current value is below n% of maximum value.
            FLAG_COLOR_NUMBERS = 0x04
        };
    private:
        /// Digit Aspect data.
        Ogre::Real t_left, b_left, t_right, b_right, top, bottom;
        /// Viewport Aspect Ratio.
        Ogre::Real mViewportAspect;
        /// Counter Inclination Level.
        Ogre::Real mInclination;
        /// Space allocated in memory for this object.
        unsigned char mAllocSize;
        /// This Element's Screen Metrics Mode.
        ScreenMetricsMode mScrMetricsMode;
        /// Object Type Name.
        static Ogre::String msTypeName;
        /// Render Operation.
        Ogre::RenderOperation mRenderOp;
        /** Counter configuration flags.
        @remarks
            The bits stands for the folowing flags:
            0x01 - Show Maximum Value (separated by an '/')
            0x02 - Show Zeroes in front of numbers. ( e.g: 0001 )
            0x04 - Color numbers when current value is below n% of maximum value.
        */
        unsigned char mFlags;
        /// Maximum number of digits to show.
        unsigned char mNumDigits;
        /** Threshold percentage.
        @remarks
             Values below n% of the maximun value will be shown in a different color.
             The percentage ranges from 0 to 100.
        */
        char mThreshold;
        /// Pointer to a (int) value to display.
        int * mCurrentValue;
        /// Pointer to a Maximum value to use as basis for calculations.
        int * mMaxValue;
        /// Normal counter color.
        Ogre::ColourValue mNormalColor;
        /// Value below threshold color.
        Ogre::ColourValue mBelowThrColor;
        /// Zero value color.
        Ogre::ColourValue mZeroValColor;
        /// Allocate the memory to hold the digits.
        void allocateMemory(char numchar);
        /// Unused, Inherited function.
        virtual void updatePositionGeometry(); // Inherited function
        /// Unused, Inherited function.
        virtual void updateTextureGeometry(); // Inherited function
        /// Simple Power of Function.
        int simplePow(int a, char b);
        /// Return the specified digit.
        char getDigit(int value, char idx);
        /// Specialized function to draw the digits on screen.
        void drawDigit(float *& pvert, Ogre::RGBA *& pcolor, char digit, Ogre::RGBA color, float offset, float depth);
    };

    class SONETTO_EXPORT CounterFactory : public Ogre::OverlayElementFactory
    {
    public:
        /** See OverlayElementFactory */
        Ogre::OverlayElement* createOverlayElement(const Ogre::String& instanceName)
        {
            return new Sonetto::Counter(instanceName);
        }
        /** See OverlayElementFactory */
        const Ogre::String& getTypeName(void) const
        {
            static Ogre::String name = "Counter";
            return name;
        }
    };

}; // namespace Sonetto

#endif // __SONETTO_COUNTER__
