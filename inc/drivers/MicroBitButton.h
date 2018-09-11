/*
The MIT License (MIT)

Copyright (c) 2016 British Broadcasting Corporation.
This software is provided by Lancaster University by arrangement with the BBC.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#ifndef MICROBIT_BUTTON_H
#define MICROBIT_BUTTON_H

#include "mbed.h"
#include "MicroBitConfig.h"
#include "MicroBitComponent.h"
#include "MicroBitEvent.h"

#define MICROBIT_PIN_BUTTON_A                   P0_17
#define MICROBIT_PIN_BUTTON_B                   P0_26
#define MICROBIT_PIN_BUTTON_C                   P0_16
#define MICROBIT_PIN_BUTTON_D                   P0_21
#define MICROBIT_PIN_BUTTON_E                   P0_22
#define MICROBIT_PIN_BUTTON_F                   P0_23
#define MICROBIT_PIN_BUTTON_G                   P0_0
#define MICROBIT_PIN_BUTTON_H                   P0_30
#define MICROBIT_PIN_BUTTON_RESET               P0_19

#define MICROBIT_BUTTON_EVT_DOWN                1
#define MICROBIT_BUTTON_EVT_UP                  2
#define MICROBIT_BUTTON_EVT_CLICK               3
#define MICROBIT_BUTTON_EVT_LONG_CLICK          4
#define MICROBIT_BUTTON_EVT_HOLD                5
#define MICROBIT_BUTTON_EVT_DOUBLE_CLICK        6

#define MICROBIT_BUTTON_LONG_CLICK_TIME         1000
#define MICROBIT_BUTTON_HOLD_TIME               1500

#define MICROBIT_BUTTON_STATE                   1
#define MICROBIT_BUTTON_STATE_HOLD_TRIGGERED    2
#define MICROBIT_BUTTON_STATE_CLICK             4
#define MICROBIT_BUTTON_STATE_LONG_CLICK        8

#define MICROBIT_BUTTON_SIGMA_MIN               0
#define MICROBIT_BUTTON_SIGMA_MAX               12
#define MICROBIT_BUTTON_SIGMA_THRESH_HI         8
#define MICROBIT_BUTTON_SIGMA_THRESH_LO         2
#define MICROBIT_BUTTON_DOUBLE_CLICK_THRESH     50

enum MicroBitButtonEventConfiguration
{
    MICROBIT_BUTTON_SIMPLE_EVENTS,
    MICROBIT_BUTTON_ALL_EVENTS
};


/**
  * Class definition for MicroBit Button.
  *
  * Represents a single, generic button on the device.
  */
class MicroBitButton : public MicroBitComponent
{
    PinName name;                                           // mbed pin name for this button.
    DigitalIn pin;                                          // The mbed object looking after this pin at any point in time (may change!).

    unsigned long downStartTime;                            // used to store the current system clock when a button down event occurs
    uint8_t sigma;                                          // integration of samples over time. We use this for debouncing, and noise tolerance for touch sensing
    MicroBitButtonEventConfiguration eventConfiguration;    // Do we want to generate high level event (clicks), or defer this to another service.

    public:

    /**
      * Constructor.
      *
      * Create a software representation of a button.
      *
      * @param name the physical pin on the processor that should be used as input.
      *
      * @param id the ID of the new MicroBitButton object.
      *
      * @param eventConfiguration Configures the events that will be generated by this MicroBitButton instance.
      *                           Defaults to MICROBIT_BUTTON_ALL_EVENTS.
      *
      * @param mode the configuration of internal pullups/pulldowns, as defined in the mbed PinMode class. PullNone by default.
      *
      * @code
      * buttonA(MICROBIT_PIN_BUTTON_A, MICROBIT_ID_BUTTON_A);
      * @endcode
      */
    MicroBitButton(PinName name, uint16_t id, MicroBitButtonEventConfiguration eventConfiguration = MICROBIT_BUTTON_ALL_EVENTS, PinMode mode = PullNone);

    /**
      * Tests if this Button is currently pressed.
      *
      * @code
      * if(buttonA.isPressed())
      *     display.scroll("Pressed!");
      * @endcode
      *
      * @return 1 if this button is pressed, 0 otherwise.
      */
    int isPressed();

    /**
      * Changes the event configuration used by this button to the given MicroBitButtonEventConfiguration.
      *
      * All subsequent events generated by this button will then be informed by this configuraiton.
      *
      * @param config The new configuration for this button. Legal values are MICROBIT_BUTTON_ALL_EVENTS or MICROBIT_BUTTON_SIMPLE_EVENTS.
      *
      * Example:
      * @code
      * // Configure a button to generate all possible events.
      * buttonA.setEventConfiguration(MICROBIT_BUTTON_ALL_EVENTS);
      *
      * // Configure a button to suppress MICROBIT_BUTTON_EVT_CLICK and MICROBIT_BUTTON_EVT_LONG_CLICK events.
      * buttonA.setEventConfiguration(MICROBIT_BUTTON_SIMPLE_EVENTS);
      * @endcode
      */
    void setEventConfiguration(MicroBitButtonEventConfiguration config);

    /**
      * periodic callback from MicroBit system timer.
      *
      * Check for state change for this button, and fires various events on a state change.
      */
    virtual void systemTick();

    /**
      * Destructor for MicroBitButton, where we deregister this instance from the array of fiber components.
      */
    ~MicroBitButton();
};

#endif
