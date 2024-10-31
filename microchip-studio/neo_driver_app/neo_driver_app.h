/* File:      neo_driver_app.h
 * Author:    Garrett Carter
 * Purpose:   Main NeoPixel driver program.
 */

/*  Code References:
    https://github.com/adafruit/Adafruit_NeoPixel/blob/master/examples/buttoncycler/buttoncycler.ino
    https://learn.adafruit.com/sipping-power-with-neopixels/demo-code
    https://github.com/adafruit/Adafruit_NeoPixel
    https://github.com/adafruit/Adafruit_NeoPixel/tree/master/examples/strandtest_wheel
*/

#ifndef NEO_DRIVER_APP_H
#define NEO_DRIVER_APP_H

/********************************* INCLUDES **********************************/
#include <stdint.h>
#include <avr/interrupt.h>
#include <debug.h>

// Allow compilation with C++ compiler
#ifdef __cplusplus
extern "C"{
#endif

/*********************************** ENUMS ***********************************/
typedef enum { SYS_MODE_ANIM_SEL,       // Play one selected animation.
               SYS_MODE_ANIM_SHUFF,     // "Shuffle mode". Play randomly selected animations in turn.
               SYS_MODE_PIX_ADJ,        // Adjust number of pixels.
} _SYS_MODE_T;

/********************************** STRUCTS **********************************/

/********************************** DEFINES **********************************/
// Watchdog timeouts (Actual delays will be slightly longer at 3.3V)
#define WDT_16MS   (0)
#define WDT_32MS   (1)
#define WDT_64MS   (2)
#define WDT_125MS  (3)
#define WDT_250MS  (4)
#define WDT_500MS  (5)
#define WDT_1S     (6)
#define WDT_2S     (7)
#define WDT_4S     (8)
#define WDT_8S     (9)  // Hardware maximum

// Multiples of 8 sec, software generated
#define WDT_16S    (10)
#define WDT_24S    (11)
#define WDT_32S    (12)
#define WDT_40S    (13)
#define WDT_48S    (14)
#define WDT_56S    (15)

// Battery params
#define BATT_TON_MSEC         (2000)    // "On" time for low battery timer
#define BATT_DISCHG_THRESH_MV (2500)    // Discharge threshold (Cutoff volt. from datasheet = 2.0V)
#define BATT_CHG_THRESH_MV    (3000)    // Charge threshold (Cutoff volt. from datasheet = 2.0V)

// Animation params
#define ANIM_CNT               (sizeof(apfn_renderFunc) / sizeof(apfn_renderFunc[0]))
#define ANIM_MANUAL_CYCLES     (1)  // Number of cycles to play in "manual" mode before going to shuffle mode


/***************************** DEBUG DEFINE OVERRIDES *****************************/

// Debug low battery
#ifdef DEBUG_LOW_BATT_EN
    #define BATT_TON_MSEC  (2000)
    #define BATT_DISCHG_THRESH_MV (1000)
#endif

// Debug batt level animation
#ifdef DEBUG_BATT_LVL_EN
    #define SPOOF_BATT_LVL (3200)
#endif

// Debug brightness
#ifdef DEBUG_BRIGHT_EN
    #define BRIGHT_MIN  (0)
#endif

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* NEO_DRIVER_APP_H */