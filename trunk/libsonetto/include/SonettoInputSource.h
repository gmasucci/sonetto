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

#ifndef SONETTO_INPUTSOURCE_H
#define SONETTO_INPUTSOURCE_H

#include "SonettoPrerequisites.h"

namespace Sonetto
{
    /** Physical input attachment

        Each Sonetto button or axis is mapped into a physical key, button or axis. Those
        physical attachments to keys, buttons and axis are represented by InputSources.
        InputSources determine how the InputManager should use the physical hardware, be it
        the keyboard or a joystick, to get its virtual buttons states and virtual axes values.
        Virtual buttons can be mapped into physical axes as well as virtual axes can be mapped
        into physical keys or buttons. This is possible to avoid somebody without a joystick
        being unable to play a game that uses analog sticks, for example.
    @see
        SourceType
    @see
        PlayerInput::configBtn()
    @see
        PlayerInput::configAxis()
    */
    class SONETTO_API InputSource
    {
        public:
            /** Describes a physical input attachment type

                When a button or axis is attached to a Sonetto::InputSource struct,
                this button's or axis' state will be based on one of these source
                types.
            @see
                Sonetto::InputSource
            */
            enum SourceType
            {
                /// Keyboard key
                IST_KEY,
                /// Joystick button
                IST_BUTTON,
                /// Joystick axis
                IST_AXIS
            };

            /** Input inversion

                Input inversion can be useful when your joystick uses peculiar axes settings.
                By default in Sonetto, analog sticks pointing upwards have a Y value of -1.0f,
                and analog sticks pointing left have an X value of -1.0f. You can invert the X
                and Y polarity, though, if your joystick works in a different way. The
                horizontal/vertical ordering of axes is also, by default, read as
                horizontal-vertical for the left analog stick and vertical-horizontal for the
                right analog stick. This can be changed by using order inversion too.
            */
            enum InvertInput
            {
                INV_NONE       = 0x00,
                INV_ORDER      = 0x01,
                INV_X_POLARITY = 0x02,
                INV_Y_POLARITY = 0x04,
            };

            /** Constructor

            @param
                aEnabled Whether the button attached to this source will be enabled or not. If not,
                the button will always be in a KS_NONE state.
            @param
                aType Defines the kind of input source
            @param
                aValue This argument depends on aType. If aType is set to IST_KEY, this value is
                an SDLKey. If it's set to IST_BUTTON, this is the button number in the joystick.
                And if it's set to IST_AXIS, this is a Sonetto::AXIS value.
            @param
                aInvert An OR'ed combination of InputSource::InvertInput's. Only used when aType
                is IST_AXIS.
            @param
                aAxisDeadzone Axis deadzone. Only used when aType is IST_AXIS and aValue is neither
                AX_LEFT nor AX_RIGHT. See axisDeadzone.
            @see
                http://www.libsdl.org/cgi/docwiki.cgi/SDLKey
            */
            InputSource(bool aEnabled = false,SourceType aType = IST_KEY,
                    uint16 aValue = 0,uint8 aInvert = INV_NONE,float aAxisDeadzone = 0.5f)
                    : enabled(aEnabled), type(aType), value(aValue),
                    invert(aInvert), axisDeadzone(aAxisDeadzone) {}

            /// Whether this source is enabled or not
            bool enabled;

            /// Source type (SourceType)
            uint8 type;

            /// Source value (depends on `type')
            uint16 value;

            /// Order and polarity inversions (InvertInput)
            uint8 invert;

            /** Button to axis mapping deadzone

                When a virtual button is mapped into a physical axis direction, the
                button will look pressed when the physical axis has got past this
                deadzone value. For example, a button mapped as AXE_LEFT_UP with an
                axisDeadzone value of 0.5f would look pressed when the left vertical
                axis had a value smaller than -0.5f (vertical axes range up to down
                as -1.0f to +1.0f).
            */
            float axisDeadzone;
    };
} // namespace

#endif
