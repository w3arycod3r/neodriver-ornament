/* File:      multi_button.c
 * Author:    Garrett Carter
 * Purpose:   
 */

#include "multi_button.h"
#include <stdbool.h>
#include <stdint.h>
#include <Arduino.h>

MB_EVENT_T mb_check(MULTIBUTTON_DATA_T* mb_data, bool b_btnRead)
{
    uint8_t u8_event = MB_EVENT_NONE;
    mb_data->b_buttonVal = b_btnRead;
    uint16_t u16_currTime = millis();

    // Button pressed down
    if (mb_data->b_buttonVal == false && mb_data->b_buttonLast == true && (u16_currTime - mb_data->u16_upTime) > MB_DEBOUNCE) {

        mb_data->u16_downTime = u16_currTime;
        mb_data->b_ignoreUp = false;
        mb_data->b_waitForUp = false;
        mb_data->b_singleOK = true;
        mb_data->b_holdEventPast = false;
        mb_data->b_longHoldEventPast = false;
        if ((u16_currTime - mb_data->u16_upTime) < MB_DC_GAP && mb_data->b_DConUp == false && mb_data->b_DCwaiting == true)  mb_data->b_DConUp = true;
        else  mb_data->b_DConUp = false;
        mb_data->b_DCwaiting = false;

    // Button released
    } else if (mb_data->b_buttonVal == true && mb_data->b_buttonLast == false && (u16_currTime - mb_data->u16_downTime) > MB_DEBOUNCE) {       
        
        if (!mb_data->b_ignoreUp) {

            mb_data->u16_upTime = u16_currTime;
            if (mb_data->b_DConUp == false) {
                mb_data->b_DCwaiting = true;
            } else {
                u8_event = MB_EVENT_DOUBLE_CLICK;
                mb_data->b_DConUp = false;
                mb_data->b_DCwaiting = false;
                mb_data->b_singleOK = false;
            }
        }
    }
    // Test for normal click u8_event: DCgap expired
    if ( mb_data->b_buttonVal == true && (u16_currTime - mb_data->u16_upTime) >= MB_DC_GAP && mb_data->b_DCwaiting == true
         && mb_data->b_DConUp == false && mb_data->b_singleOK == true && u8_event != 2) {

        u8_event = MB_EVENT_CLICK;
        mb_data->b_DCwaiting = false;
    }
    // Test for hold
    if (mb_data->b_buttonVal == false && (u16_currTime - mb_data->u16_downTime) >= MB_HOLD_TIME) {

        // Trigger "normal" hold
        if (!mb_data->b_holdEventPast) {

            u8_event = MB_EVENT_HOLD;
            mb_data->b_waitForUp = true;
            mb_data->b_ignoreUp = true;
            mb_data->b_DConUp = false;
            mb_data->b_DCwaiting = false;
            //u32_downTime = t;
            mb_data->b_holdEventPast = true;
        }
        // Trigger "long" hold
        if ((u16_currTime - mb_data->u16_downTime) >= MB_LONG_HOLD_TIME) {

            if (!mb_data->b_longHoldEventPast) {
                
                u8_event = MB_EVENT_LONG_HOLD;
                mb_data->b_longHoldEventPast = true;
            }
        }
    }

    mb_data->b_buttonLast = mb_data->b_buttonVal;
    return u8_event;
}

void mb_reset(MULTIBUTTON_DATA_T* mb_data)
{
    mb_data->b_buttonVal = true;      
    mb_data->b_buttonLast = true;     
    mb_data->b_DCwaiting = false;     
    mb_data->b_DConUp = false;        
    mb_data->b_singleOK = true;       
    mb_data->u16_downTime = 0;
    mb_data->u16_upTime = 0;  
    mb_data->b_ignoreUp = false;      
    mb_data->b_waitForUp = false;     
    mb_data->b_holdEventPast = false; 
    mb_data->b_longHoldEventPast = false;
}

