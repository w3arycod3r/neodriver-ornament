// File:    multiButton.cpp
// Purpose:	Class functions

#include "multi_button.h"

/* 4-Way Button:  Click, Double-Click, Press+Hold, and Press+Long-Hold Test Sketch

http://jmsarduino.blogspot.com/2009/10/4-way-button-click-double-click-hold.html

By Jeff Saltzman
Oct. 13, 2009

To keep a physical interface as simple as possible, this sketch demonstrates generating
four output events from a single push-button.

1) Click:  rapid press and release
2) Double-Click:  two clicks in quick succession
3) Press and Hold:  holding the button down
4) Long Press and Hold:  holding the button for a long time
*/

uint8_t multiButton::check(bool b_btnRead)
{
    uint8_t u8_event = 0;
    b_buttonVal = b_btnRead;
    uint16_t u16_currTime = millis();

    // Button pressed down
    if (b_buttonVal == false && b_buttonLast == true && (u16_currTime - u16_upTime) > MB_DEBOUNCE) {

        u16_downTime = u16_currTime;
        b_ignoreUp = false;
        b_waitForUp = false;
        b_singleOK = true;
        b_holdEventPast = false;
        b_longHoldEventPast = false;
        if ((u16_currTime - u16_upTime) < MB_DC_GAP && b_DConUp == false && b_DCwaiting == true)  b_DConUp = true;
        else  b_DConUp = false;
        b_DCwaiting = false;

    // Button released
    } else if (b_buttonVal == true && b_buttonLast == false && (u16_currTime - u16_downTime) > MB_DEBOUNCE) {       
        
        if (!b_ignoreUp) {

            u16_upTime = u16_currTime;
            if (b_DConUp == false) {
                b_DCwaiting = true;
            } else {
                u8_event = 2;
                b_DConUp = false;
                b_DCwaiting = false;
                b_singleOK = false;
            }
        }
    }
    // Test for normal click u8_event: DCgap expired
    if ( b_buttonVal == true && (u16_currTime - u16_upTime) >= MB_DC_GAP && b_DCwaiting == true
         && b_DConUp == false && b_singleOK == true && u8_event != 2) {

        u8_event = 1;
        b_DCwaiting = false;
    }
    // Test for hold
    if (b_buttonVal == false && (u16_currTime - u16_downTime) >= MB_HOLD_TIME) {

        // Trigger "normal" hold
        if (!b_holdEventPast) {

            u8_event = 3;
            b_waitForUp = true;
            b_ignoreUp = true;
            b_DConUp = false;
            b_DCwaiting = false;
            //u32_downTime = t;
            b_holdEventPast = true;
        }
        // Trigger "long" hold
        if ((u16_currTime - u16_downTime) >= MB_LONG_HOLD_TIME) {

            if (!b_longHoldEventPast) {
                
                u8_event = 4;
                b_longHoldEventPast = true;
            }
        }
    }

    b_buttonLast = b_buttonVal;
    return u8_event;
}