/* File:      neo_driver_app.h
 * Author:    Garrett Carter
 * Purpose:   Main NeoPixel driver program.
 */

#ifndef NEO_DRIVER_APP_H
#define NEO_DRIVER_APP_H

/********************************* INCLUDES **********************************/
#include <stdint.h>
#include <avr/interrupt.h>

/*********************************** ENUMS ***********************************/
typedef enum { SYS_MODE_ANIM_SEL,       // Play one selected animation.
               SYS_MODE_ANIM_SHUFF,     // "Shuffle mode". Play randomly selected animations in turn.
               SYS_MODE_PIX_ADJ,        // Adjust number of pixels.
} _SYS_MODE_T;

typedef enum { FRAMES_MODE_STATIC,      // Sprite is stationary. Length determined by repeat count.
               FRAMES_MODE_SHIFT,       // Shift the sprite. Anim is terminated when sprite goes offscreen.
} _FRAMES_MODE_T;

/********************************** STRUCTS **********************************/
typedef struct {
    uint16_t u16_frameStep_msec;  // "on" time for each frame in the sequence.
    uint32_t u32_color;           // Color. Set to 0 for dynamic effect.
    const uint8_t* pu8_frames;    // Pointer to the frame sequence array stored in flash. Pointers are 2 bytes on this arch.
    uint8_t  u8_mode;             // Mode of operation. A value from _FRAMES_MODE_T.

    // STATIC mode params
    uint8_t  u8_seqRepeatCnt;     // Number of times to repeat the frame seq for each anim cycle in STATIC mode.
    
    // SHIFT mode params
    uint8_t  u8_frameShiftSync;   // Number of frames between shifts, in SHIFT mode.
    int8_t   i8_startPosX;        // Init X,Y position. Should start onscreen.
    int8_t   i8_startPosY;        // 
    int8_t   i8_shiftStepX;       // The X shift to perform on each shift step. Can be pos, neg, or zero.
    int8_t   i8_shiftStepY;       // The Y shift to perform on each shift step. Can be pos, neg, or zero.
    
} FRAMES_CONFIG_T;

/********************************** DEFINES **********************************/

// Watchdog timeouts (Actual delays will be slightly longer at 3.3V)
#define WDT_16MS   0
#define WDT_32MS   1
#define WDT_64MS   2
#define WDT_125MS  3
#define WDT_250MS  4
#define WDT_500MS  5
#define WDT_1S     6
#define WDT_2S     7
#define WDT_4S     8
#define WDT_8S     9  // Hardware maximum

// Multiples of 8 sec, software generated
#define WDT_16S    10
#define WDT_24S    11
#define WDT_32S    12
#define WDT_40S    13
#define WDT_48S    14
#define WDT_56S    15

// Battery params
#define BATT_TON_MSEC         2000    // "On" time for low battery timer
#define BATT_DISCHG_THRESH_MV 2500    // Discharge threshold (Cutoff volt. from datasheet = 2.0V)
#define BATT_CHG_THRESH_MV    3000    // Charge threshold (Cutoff volt. from datasheet = 2.0V)

#define BATT_LVL_THRESH_1_MV 2750
#define BATT_LVL_THRESH_2_MV 3000
#define BATT_LVL_THRESH_3_MV 3100
#define BATT_LVL_THRESH_4_MV 3200
#define BATT_LVL_STEP_MSEC   400
#define BATT_LVL_SLEEP_TIME  WDT_8S

#define BATT_ICON_LVL_1     '#'
#define BATT_ICON_LVL_2     '$'
#define BATT_ICON_LVL_3     '%'
#define BATT_ICON_LVL_4     '&'
#define BATT_ICON_LVL_5     '\''
#define BATT_ICON_DEAD      '('

// Animation params
#define ANIM_CNT (sizeof(apfn_renderFunc) / sizeof(apfn_renderFunc[0]))
#define ANIM_MANUAL_CYCLES     1              // Number of cycles to play in "manual" mode before going to shuffle mode

// Primaries
#define PRIM_ON_TIME_MSEC      6000           // "on" time between sleeps
#define PRIM_SLEEP_TIME        WDT_8S         // Time to sleep after this anim

// Colorwheel
#define COL_WHEEL_ON_TIME_MSEC 6000
#define COL_WHEEL_SLEEP_TIME   WDT_8S

// Half
#define HALF_ON_TIME_MSEC      6000
#define HALF_SLEEP_TIME        WDT_8S

// Sparkle
#define SPK_STEP_MSEC          65             // Time between random pixel change
#define SPK_ON_TIME_MSEC       4000
#define SPK_SLEEP_TIME         WDT_8S

// Marquee
#define MARQUEE_ON_TIME_MSEC   6000
#define MARQUEE_SLEEP_TIME     WDT_8S

// Sine Gamma
#define SINE_ON_TIME_MSEC      6000
#define SINE_SLEEP_TIME        WDT_8S

// CoV
#define COV_STEP_MSEC          1000                   // "On" time for each base
#define COV_STEP_CNT           4                      // Number of steps (bases) per cycle
#define COV_BASES_CNT         (EEP_COV_DATA_NUM_BYTES * 4)     // 4 bases per byte
#define COV_SLEEP_TIME         WDT_8S

// Message Scroll
#define MSG_STEP_MSEC          200                    // Time to shift each char left by one pixel
#define MSG_SLEEP_TIME         WDT_24S

// Frames
#define FRAMES_SLEEP_TIME      WDT_8S

/*
 *   ANIM_ROT_SEL - Rotation select for 5x5 Matrix - Increasing #, Turning clockwise
 *   Used by several text based animations
 *
 *   Value |  Input wires located at:
 *   ================================
 *   1     |  BOTTOM
 *   2     |  LEFT
 *   3     |  TOP
 *   4     |  RIGHT
 */
#define ANIM_ROT_SEL 3

/******************************* DEBUG PARAMS ********************************/
#define DEBUG_COV_ANIM  (0)
#define DEBUG_LOW_BATT  (0)
#define DEBUG_BATT_LVL  (0)
#define DEBUG_ADC_VAL   (0)
#define DEBUG_RAND_SEED (0)
#define DEBUG_BRIGHT    (0)

#define DEBUG_SOFT_RESET_ON_INTERVAL_EN (0)
#define DEBUG_SOFT_RESET_INTERVAL_SEC   (2)

// Debug CoV animation
#if DEBUG_COV_ANIM == 1
    #define COV_BASES_CNT  10
    #define COV_STEP_MSEC  1000
    #define COV_SLEEP_TIME WDT_125MS
#endif

// Debug low battery
#if DEBUG_LOW_BATT == 1
    #define BATT_TON_MSEC  2000
    #define BATT_DISCHG_THRESH_MV 1000
#endif

// Debug batt level animation
#if DEBUG_BATT_LVL == 1
    #define SPOOF_BATT_LVL 3200
#endif

// Debug brightness
#if DEBUG_BRIGHT == 1
    #define BRIGHT_MIN  0
#endif


#endif /* NEO_DRIVER_APP_H */