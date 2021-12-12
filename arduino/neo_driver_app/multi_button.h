// File:    multiButton.h
// Purpose:	Class definition

#ifndef MULTIBUTTON_H
#define MULTIBUTTON_H

#include <Arduino.h>

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

// -------- DEFINES --------
#define MB_DEBOUNCE        20      // (Def 20) ms debounce period to prevent flickering when pressing or releasing the button
#define MB_DC_GAP          0       // (Def 250) (Set to 0 to disable DC) max ms between clicks for a double click event
#define MB_HOLD_TIME       1000    // (Def 1000) ms hold period: how long to wait for press+hold event
#define MB_LONG_HOLD_TIME  3000    // (Def 3000) ms long hold period: how long to wait for press+hold event

class multiButton {
public:
    uint8_t check(bool b_btnRead);

protected:
    bool b_buttonVal = true;      // value read from button
    bool b_buttonLast = true;     // buffered value of the button's previous state
    bool b_DCwaiting = false;     // whether we're waiting for a double click (down)
    bool b_DConUp = false;        // whether to register a double click on next release, or whether to wait and click
    bool b_singleOK = true;       // whether it's OK to do a single click
    uint16_t u16_downTime = 0;    // time the button was pressed down
    uint16_t u16_upTime = 0;      // time the button was released
    bool b_ignoreUp = false;      // whether to ignore the button release because the click+hold was triggered
    bool b_waitForUp = false;         // when held, whether to wait for the up event
    bool b_holdEventPast = false;     // whether or not the hold event happened already
    bool b_longHoldEventPast = false; // whether or not the long hold event happened already
};

#endif