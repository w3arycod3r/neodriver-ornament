/* File:      multi_button.h
 * Author:    Garrett Carter
 * Purpose:   
 */

#ifndef MULTIBUTTON_H
#define MULTIBUTTON_H

#include <stdbool.h>
#include <stdint.h>

// Allow compilation with C++ compiler
#ifdef __cplusplus
extern "C"{
#endif

/* 4-Way Button:  Click, Double-Click, Press+Hold, and Press+Long-Hold Test Sketch

http://jmsarduino.blogspot.com/2009/10/4-way-button-click-double-click-hold.html

By Jeff Saltzman
Oct. 13, 2009

To keep a physical interface as simple as possible, this sketch demonstrates generating
four output events from a single push-button.

*/

/******************************* DEFINES ********************************/
#define MB_DEBOUNCE        20      // (Def 20) ms debounce period to prevent flickering when pressing or releasing the button
#define MB_DC_GAP          0       // (Def 250) (Set to 0 to disable DC) max ms between clicks for a double click event
#define MB_HOLD_TIME       1000    // (Def 1000) ms hold period: how long to wait for press+hold event
#define MB_LONG_HOLD_TIME  3000    // (Def 3000) ms long hold period: how long to wait for press+hold event

/******************************** STRUCTS *******************************/
typedef struct {

    bool b_buttonVal;           // value read from button
    bool b_buttonLast;          // buffered value of the button's previous state
    bool b_DCwaiting;           // whether we're waiting for a double click (down)
    bool b_DConUp;              // whether to register a double click on next release, or whether to wait and click
    bool b_singleOK;            // whether it's OK to do a single click
    uint16_t u16_downTime;      // time the button was pressed down
    uint16_t u16_upTime;        // time the button was released
    bool b_ignoreUp;            // whether to ignore the button release because the click+hold was triggered
    bool b_waitForUp;           // when held, whether to wait for the up event
    bool b_holdEventPast;       // whether or not the hold event happened already
    bool b_longHoldEventPast;   // whether or not the long hold event happened already    

} MULTIBUTTON_DATA_T;

typedef enum {
    MB_EVENT_NONE = 0,
    MB_EVENT_CLICK,             // Click:  rapid press and release
    MB_EVENT_DOUBLE_CLICK,      // Double-Click:  two clicks in quick succession
    MB_EVENT_HOLD,              // Press and Hold:  holding the button down
    MB_EVENT_LONG_HOLD          // Long Press and Hold:  holding the button for a long time
} MB_EVENT_T;

/****************************** PROTOTYPES ******************************/
MB_EVENT_T mb_check(MULTIBUTTON_DATA_T* mb_data, bool b_btnRead);
void mb_reset(MULTIBUTTON_DATA_T* mb_data);


#ifdef __cplusplus
} // extern "C"
#endif

#endif /* MULTIBUTTON_H */