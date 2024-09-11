/* File:      anim.h
 * Author:    Garrett Carter
 * Purpose:   Animation "task" functions, non-blocking. These are called by neo_driver_app in a loop to render animations.
 */

#ifndef ANIM_H
#define ANIM_H

#include <stdint.h>
#include <neo_common.h>
#include <eep_data.h>

// Allow compilation with C++ compiler
#ifdef __cplusplus
extern "C"{
#endif

/********************************** DEFINES **********************************/
// Primaries
#define PRIM_ON_TIME_MSEC      (6000)           // "on" time between sleeps
#define PRIM_SLEEP_TIME        (WDT_8S)         // Time to sleep after this anim

// Colorwheel
#define COL_WHEEL_ON_TIME_MSEC (6000)
#define COL_WHEEL_SLEEP_TIME   (WDT_8S)

// Half
#define HALF_ON_TIME_MSEC      (6000)
#define HALF_SLEEP_TIME        (WDT_8S)

// Sparkle
#define SPK_STEP_MSEC          (65)             // Time between random pixel change
#define SPK_ON_TIME_MSEC       (4000)
#define SPK_SLEEP_TIME         (WDT_8S)

// Marquee
#define MARQUEE_ON_TIME_MSEC   (6000)
#define MARQUEE_SLEEP_TIME     (WDT_8S)

// Sine Gamma
#define SINE_ON_TIME_MSEC      (6000)
#define SINE_SLEEP_TIME        (WDT_8S)

// CoV
#define COV_STEP_MSEC          (1000)                   // "On" time for each base
#define COV_STEP_CNT           (4)                      // Number of steps (bases) per cycle
#define COV_BASES_CNT          (EEP_COV_DATA_NUM_BYTES * 4)     // 4 bases per byte
#define COV_SLEEP_TIME         (WDT_8S)

// Message Scroll
#define MSG_STEP_MSEC          (200)                    // Time to shift each char left by one pixel
#define MSG_SLEEP_TIME         (WDT_24S)

// Frames
#define FRAMES_SLEEP_TIME      (WDT_8S)

// Battery Level
#define BATT_LVL_STEP_MSEC    (400)
#define BATT_LVL_SLEEP_TIME   (WDT_8S)

/***************************** DEBUG DEFINE OVERRIDES *****************************/
// Debug CoV animation
#ifdef DEBUG_COV_ANIM_EN
    #define COV_BASES_CNT  10
    #define COV_STEP_MSEC  1000
    #define COV_SLEEP_TIME WDT_125MS
#endif

/*********************************** ENUMS ***********************************/
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


/********************************** PROTOTYPES **********************************/
// Animation "task" functions
void anim_off();
void anim_white();
void anim_primaries();
void anim_colorwheel_gamma();
void anim_half();
void anim_sparkle();
void anim_marquee();
void anim_sine_gamma();
void anim_cov_quar();
void anim_cov_char();
void anim_cov(uint8_t u8_pattType);
void anim_msg_1();
void anim_msg_2();
void anim_msg_3();
void anim_msg_4();
void anim_msg(const uint8_t* pu8_msg, uint32_t u32_color);
void anim_frames_1();
void anim_frames_2();
void anim_frames_3();
void anim_frames_4();
void anim_frames_5();
void anim_frames_6();
void anim_frames_7();
void anim_frames_8();
void anim_frames_9();
void anim_frames_10();
void anim_frames(FRAMES_CONFIG_T* pst_f);
void anim_batt_level();

// Animation control functions
void shuffle_anim_params();

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* ANIM_H */