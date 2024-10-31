/* File:      neo_common.h
 * Author:    Garrett Carter
 * Purpose:   Common def's for eep_data_write and neo_driver_app, so they don't get out of sync
 */

#ifndef NEO_COMMON_H
#define NEO_COMMON_H

#include <Arduino.h>
#include <utility.h>
#include <avr/pgmspace.h>
#include <wiring_analog.h>

/********************************** DEFINES **********************************/

/*
 *  FUSE SETTINGS:
 *  EESAVE          ENABLE
 *  BODLEVEL        ENABLE at VCC = 1.8V
 *  CKDIV8          DISABLE
 *  SUT_CKSEL       INTRCOSC_8MHZ_6CK_14CK_64MS
 *  WDTON       1   WDT in Safety level 1, initially disabled.
 *
 *  LOW       0xE2
 *  HIGH      0xD6
 *  EXTENDED  0xFF
 * 
 *  fuses.txt = e2d6ff
 */

#define MATRIX_WIDTH_PIX    (5)
#define MATRIX_HEIGHT_PIX   (5)
#define MATRIX_NUM_PIX     (MATRIX_WIDTH_PIX * MATRIX_HEIGHT_PIX)

// I/O Mapping
#define IO_SW_LEFT       (BIT0)                       // PB0, AREF - Left switch
#define IO_SW_RIGHT      (BIT1)                       // PB1       - Right switch
#define IO_NP_ENABLE     (BIT2)                       // PB2, ADC1 - NeoPixel Enable
#define IO_NP_DATA       (BIT3)                       // PB3, ADC3 - NeoPixel Data
#define IO_POT_ADC_CH    (ADMUX_MUX_SE_ADC2_PB4)      // PB4, ADC2 - Brightness Potentiometer
#define IO_POT_ADC_REF   (ADMUX_REFS_VCC_AS_REF)      // PB4, ADC2 - Brightness Potentiometer
                                                      // PB5, ADC0 - Reset

// 

/*  Analog channel and reference for "random noise" measurement
    Could we get "random" read by using PB0 (AREF) as external ref to ADC?
    PB0 goes to left switch, with no external pullup.
    Could measure any constant channel, with the "random" reference.
    Probably need to do consecutive reads, and composite the least significant bits.
*/
#define RAND_AN_MEAS_CH  (IO_POT_ADC_CH)
#define RAND_AN_MEAS_REF (ADMUX_REFS_EXT_PB0_AREF)

// Pixel params
#define BRIGHT_MIN               (24)
#define BRIGHT_INIT              (8)
#define CONFIRM_BLINK_BRIGHT_LVL (50)     // Brightness used for EEP program confirm blinking


#endif /* NEO_COMMON_H */