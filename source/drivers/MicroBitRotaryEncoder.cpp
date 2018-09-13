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

#include "MicroBitConfig.h"
#include "MicroBitRotaryEncoder.h"
#include "MicroBitFiber.h"
#include "ErrorNo.h"

uint32_t MicroBitRotaryEncoder::getValue(){
    return nRotaryEncoderValue;
}

void MicroBitRotaryEncoder::setValue(uint32_t value){
    nRotaryEncoderValue = value;
}
void MicroBitRotaryEncoder::onCCW(){
    if(nRotaryEncoderValue > 0)
        nRotaryEncoderValue = nRotaryEncoderValue - 1;
}

void MicroBitRotaryEncoder::onCW(){
    nRotaryEncoderValue = nRotaryEncoderValue + 1;
    if(nRotaryEncoderValue > 9)
        nRotaryEncoderValue = 9;
}

void MicroBitRotaryEncoder::onRotated(){    
    uint32_t now = encoderTimer.read_ms();
    if(now - nEncoderTime < 200) 
        return;
    nEncoderTime = now;
    if(rotationDirection.read())
        onCCW();
    else
        onCW();
}

MicroBitRotaryEncoder::MicroBitRotaryEncoder() :
    rotaryEncoderInterrupt(P0_0),
    rotationDirection(P0_30)
{
    rotaryEncoderInterrupt.mode(PullUp);
    encoderTimer.start();
    rotaryEncoderInterrupt.fall(this, &MicroBitRotaryEncoder::onRotated);
}


