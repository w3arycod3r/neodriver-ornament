/* File:      anim.c
 * Author:    Garrett Carter
 * Purpose:   Animation "task" functions, non-blocking. These are called by neo_driver_app in a loop to render animations.
 */

#include "anim.h"
#include <stdint.h>
#include <avr/pgmspace.h>
#include <neo_pixel_slim.h>
#include <prng.h>
#include <draw.h>
#include <utility.h>
// TODO: Can we remove this dependency?
#include <neo_driver_app.h>

/************************ GLOBAL RAM VARS DEFINITIONS ************************/
// Below are shared with neo_driver_app
bool b_animReset = true;               // Used by mode logic to inform animations to reset
bool b_animCycleComplete = false;      // Signal from animations to mode logic
uint8_t u8_nextSleepTime = WDT_16MS;   // Data from terminating animations to mode logic

/****************************** FLASH CONSTANTS ******************************/
// NOTE: If these are defined in the header file instead, they get placed in flash twice!
// PROGMEM arrays should be defined in .cpp/.c files!

// Message Strings
static const uint8_t PROGMEM sz_msg1[] = "W3ARYCOD3R ";
static const uint8_t PROGMEM sz_msg2[] = "MERRY XMAS! ";
static const uint8_t PROGMEM sz_msg3[] = "HAPPY NEW YEAR! ";
static const uint8_t PROGMEM sz_msg4[] = "u 4 8 15 16 23 42 ";    // Lost "numbers"
// const uint8_t PROGMEM sz_msg4[] = "2021 FTW! ";


// Frame ASCII sequences
static const uint8_t PROGMEM sz_frames1[] = "gh";            // Pacman    ( 12   : gh  )
static const uint8_t PROGMEM sz_frames2[] = "\"";            // Ghost     ( 1    : "   )
static const uint8_t PROGMEM sz_frames3[] = "jklmnopq";      // Starburst ( 12345678 : jklmnopq)
static const uint8_t PROGMEM sz_frames4[] = "rs";            // Frog      ( 12   :  rs )
static const uint8_t PROGMEM sz_frames5[] = "`a";            // Turbine   ( 12   :  `a )
static const uint8_t PROGMEM sz_frames6[] = "b/-\\";         // Spinner
static const uint8_t PROGMEM sz_frames6_alt[] = "\\-/b";     // Spinner ALT
static const uint8_t PROGMEM sz_frames7[] = "ef";            // DNA       ( 12    :  ef    )
static const uint8_t PROGMEM sz_frames8[] = ")*+,.";         // Snowfall  ( 12345 :  )*+,. )
static const uint8_t PROGMEM sz_frames9[] = ":@=[=;";        // Field     ( 12345  :  :;=@[  ) [143532]
static const uint8_t PROGMEM sz_frames10[] = "]_cdcit";      // Ball      ( 123467 :  ]_cdit )
static const uint8_t PROGMEM sz_frames10_alt[] = "ticdc_]";  // Ball ALT

/************************ GLOBAL RAM VARS DEFINITIONS ************************/

// Frame sequence config data
// These are currently stored in RAM, because we change some params at runtime, for "randomization"
// See the struct definition FRAMES_CONFIG_T for details on the fields.

// Sequence 1 - Pacman
FRAMES_CONFIG_T st_sequence1 = {
    200,
    COLOR_YELLOW,
    (uint8_t*)sz_frames1,
    FRAMES_MODE_SHIFT,

    // STATIC params
    0,

    // SHIFT params
    2,
    -4,
    0,
    1,
    0
};

// Sequence 2 - Ghost
FRAMES_CONFIG_T st_sequence2 = {
    250,
    COLOR_TEAL,
    (uint8_t*)sz_frames2,
    FRAMES_MODE_SHIFT,

    // STATIC params
    0,

    // SHIFT params
    1,
    -4,
    0,
    1,
    0
};

// Sequence 3 - Starburst
FRAMES_CONFIG_T st_sequence3 = {
    100,
    COLOR_WHEEL,
    (uint8_t*)sz_frames3,
    FRAMES_MODE_STATIC,

    // STATIC params
    1,

    // SHIFT params
    0,
    0,
    0,
    0,
    0
};

// Sequence 4 - Frog
FRAMES_CONFIG_T st_sequence4 = {
    250,
    COLOR_GREEN,
    (uint8_t*)sz_frames4,
    FRAMES_MODE_SHIFT,

    // STATIC params
    0,

    // SHIFT params
    1,
    -4,
    0,
    1,
    0
};

// Sequence 5 - Turbine
FRAMES_CONFIG_T st_sequence5 = {
    250,
    COLOR_WHEEL,
    (uint8_t*)sz_frames5,
    FRAMES_MODE_STATIC,

    // STATIC params
    7,

    // SHIFT params
    0,
    0,
    0,
    0,
    0
};

// Sequence 6 - Spinner
FRAMES_CONFIG_T st_sequence6 = {
    250,
    COLOR_WHEEL,
    (uint8_t*)sz_frames6,
    FRAMES_MODE_STATIC,

    // STATIC params
    7,

    // SHIFT params
    0,
    0,
    0,
    0,
    0
};

// Sequence 7 - DNA
FRAMES_CONFIG_T st_sequence7 = {
    250,
    COLOR_WHEEL,
    (uint8_t*)sz_frames7,
    FRAMES_MODE_STATIC,

    // STATIC params
    6,

    // SHIFT params
    0,
    0,
    0,
    0,
    0
};

// Sequence 8 - Snowfall
FRAMES_CONFIG_T st_sequence8 = {
    300,
    COLOR_TEAL,
    (uint8_t*)sz_frames8,
    FRAMES_MODE_STATIC,

    // STATIC params
    4,

    // SHIFT params
    0,
    0,
    0,
    0,
    0
};

// Sequence 9 - Field
FRAMES_CONFIG_T st_sequence9 = {
    400,
    COLOR_WHEEL,
    (uint8_t*)sz_frames9,
    FRAMES_MODE_STATIC,

    // STATIC params
    5,

    // SHIFT params
    0,
    0,
    0,
    0,
    0
};

// Sequence 10 - Ball
FRAMES_CONFIG_T st_sequence10 = {
    150,
    COLOR_YELLOW,
    (uint8_t*)sz_frames10,
    FRAMES_MODE_STATIC,

    // STATIC params
    1,

    // SHIFT params
    0,
    0,
    0,
    0,
    0
};

/******************************** FUNCTIONS ********************************/

// All NeoPixels off
void anim_off() {
    np_clear();
}

void anim_white() {
    np_fill_all(0xFFFFFF);
}

// Cycle through primary colors (red, green, blue), full brightness.
void anim_primaries() {
    static uint16_t u16_startTime;
    static uint8_t u8_direction;        // Direction of the anim. (0, 1 : CW, CCW)
    uint32_t u32_color;
    uint16_t u16_currTime = millis();
    uint16_t u16_currTimeMod = u16_currTime;
    uint8_t  u8_numPixels = np_get_length();

    // Reset static vars to starting values
    if (b_animReset) {
        b_animReset = false;

        u16_startTime = u16_currTime;
        u8_direction = prng_upper(2); // 0 or 1
    }

    // For unsigned n-bit int i: -(i) = (2^n - i)
    // Negating this value will cause it to decrease instead of increase with time
    if (u8_direction) { u16_currTimeMod = -u16_currTimeMod; }

    // Generate time-modulated animation
    for (uint8_t u8_pixIdx = 0; u8_pixIdx < u8_numPixels; u8_pixIdx++) {
        uint8_t u8_pixIdxMod = u8_pixIdx;

        // Time modulated pixel index
        // Tick every 100 mSec
        u8_pixIdxMod = ((u16_currTimeMod / 100) + u8_pixIdxMod) % u8_numPixels;

        // Approx 1/3 R,G,B at any one time
        // Where does our modulated pixel index fall on the color wheel?
        if      (u8_pixIdxMod < u8_numPixels/3)    u32_color = 0xFF0000; // Red
        else if (u8_pixIdxMod < 2*u8_numPixels/3)  u32_color = 0x00FF00; // Green
        else                                       u32_color = 0x0000FF; // Blue
        np_set_pix_color_pack(u8_pixIdx, u32_color);
    }

    // Signal mode logic of cycle completion
    if (u16_currTime - u16_startTime > PRIM_ON_TIME_MSEC) {
        b_animCycleComplete = true;

        u8_nextSleepTime = PRIM_SLEEP_TIME;
    }
}

// Color wheel using gamma-corrected values.
void anim_colorwheel_gamma() {
    static uint16_t u16_startTime;
    static uint8_t u8_direction;        // Direction of the anim. (0, 1 : CW, CCW)
    uint16_t u16_currTime = millis();
    uint16_t u16_currTimeMod = u16_currTime;
    uint8_t  u8_numPixels = np_get_length();

    // Reset static vars to starting values
    if (b_animReset) {
        b_animReset = false;

        u16_startTime = u16_currTime;
        u8_direction = prng_upper(2); // 0 or 1
    }

    // For unsigned n-bit int i: -(i) = (2^n - i)
    // Negating this value will cause it to decrease instead of increase with time
    if (u8_direction) { u16_currTimeMod = -u16_currTimeMod; }

    // Generate time-modulated animation
    for (uint8_t u8_pixIdx = 0; u8_pixIdx < u8_numPixels; u8_pixIdx++) {
        np_set_pix_color_pack(u8_pixIdx, np_get_gamma_32(np_hsv_to_pack_hue((u16_currTimeMod * 50) + u8_pixIdx * 65536L / u8_numPixels)));
    }

    // Signal mode logic of cycle completion
    if (u16_currTime - u16_startTime > COL_WHEEL_ON_TIME_MSEC) {
        b_animCycleComplete = true;

        u8_nextSleepTime = COL_WHEEL_SLEEP_TIME;
    }
}

// Cycle with half the pixels on, half off at any given time.
void anim_half() {
    static uint16_t u16_startTime;
    static uint8_t u8_direction;        // Direction of the anim. (0, 1 : CW, CCW)
    uint16_t u16_currTime = millis();
    uint16_t u16_currTimeMod = u16_currTime;
    uint8_t  u8_numPixels = np_get_length();

    // Reset static vars to starting values
    if (b_animReset) {
        b_animReset = false;

        u16_startTime = u16_currTime;
        u8_direction = prng_upper(2); // 0 or 1
    }

    // For unsigned n-bit int i: -(i) = (2^n - i)
    // Negating this value will cause it to decrease instead of increase with time
    if (u8_direction) { u16_currTimeMod = -u16_currTimeMod; }

    // Generate time-modulated animation
    for (uint8_t u8_pixIdx = 0; u8_pixIdx < u8_numPixels; u8_pixIdx++) {

        // Time modulated pixel index
        uint8_t u8_pixIdxMod = u16_currTimeMod/4 + u8_pixIdx * 256 / u8_numPixels;
        u8_pixIdxMod = (u8_pixIdxMod >> 7) * 255;                     // ON or OFF

        np_set_pix_color_pack(u8_pixIdx, u8_pixIdxMod * 0x010000);
    }

    // Signal mode logic of cycle completion
    if (u16_currTime - u16_startTime > HALF_ON_TIME_MSEC) {
        b_animCycleComplete = true;

        u8_nextSleepTime = HALF_SLEEP_TIME;
    }
}

// Sparkles. Randomly turns on ONE pixel at a time.
void anim_sparkle() {
    static uint16_t u16_startTime;
    static uint16_t u16_lastTime;
    static uint8_t  u8_pixIdx;
    static uint8_t  u8_red;
    static uint8_t  u8_green;
    static uint8_t  u8_blue;
    uint16_t u16_currTime = millis();
    uint8_t  u8_numPixels = np_get_length();

    // Reset static vars to starting values
    if (b_animReset) {
        b_animReset = false;

        u16_startTime = u16_currTime;
        u16_lastTime = u16_currTime;
        u8_pixIdx = 0;

        // Pick a bright enough random color
        do {
            u8_red = prng_upper(256);
            u8_green = prng_upper(256);
            u8_blue = prng_upper(256);
        } while ((u8_red < 130) && (u8_green < 130) && (u8_blue < 130));
    }
    
    // Iterate animation once per step
    if (u16_currTime - u16_lastTime > SPK_STEP_MSEC) {
        u16_lastTime = u16_currTime;

        np_clear();                             // Clear pixels

        uint8_t u8_newPixIdx;
        do { u8_newPixIdx = prng_upper(u8_numPixels); }  // Pick a new random pixel
        while (u8_newPixIdx == u8_pixIdx);           // but not the same as last time

        u8_pixIdx = u8_newPixIdx;                    // Save new random pixel index
        np_set_pix_color(u8_pixIdx, u8_red, u8_green, u8_blue);
    }

    // Signal mode logic of cycle completion
    if (u16_currTime - u16_startTime > SPK_ON_TIME_MSEC) {
        b_animCycleComplete = true;

        u8_nextSleepTime = SPK_SLEEP_TIME;
    }
}

// Simple on-or-off "marquee" animation w/ about 50% of pixels lit at once.
void anim_marquee() {
    static uint16_t u16_startTime;
    static uint8_t u8_direction;        // Direction of the anim. (0, 1 : CW, CCW)
    uint16_t u16_currTime = millis();
    uint16_t u16_currTimeMod = u16_currTime;
    uint8_t  u8_numPixels = np_get_length();

    // Reset static vars to starting values
    if (b_animReset) {
        b_animReset = false;

        u16_startTime = u16_currTime;
        u8_direction = prng_upper(2); // 0 or 1
    }

    // For unsigned n-bit int i: -(i) = (2^n - i)
    // Negating this value will cause it to decrease instead of increase with time
    if (u8_direction) { u16_currTimeMod = -u16_currTimeMod; }

    // Generate time-modulated animation
    for (uint8_t u8_pixIdx = 0; u8_pixIdx < u8_numPixels; u8_pixIdx++) {

        // Time modulated pixel index
        uint8_t u8_pixIdxMod = (u16_currTimeMod/4 + u8_pixIdx * 256 / u8_numPixels) & 0xFF;
        u8_pixIdxMod = ((u8_pixIdxMod >> 6) & 1) * 255;

        np_set_pix_color_pack(u8_pixIdx, u8_pixIdxMod * 0x000100L);
    }

    // Signal mode logic of cycle completion
    if (u16_currTime - u16_startTime > MARQUEE_ON_TIME_MSEC) {
        b_animCycleComplete = true;

        u8_nextSleepTime = MARQUEE_SLEEP_TIME;
    }
}

// Sine wave with gamma correction.
void anim_sine_gamma() {
    static uint16_t u16_startTime;
    static uint8_t u8_direction;        // Direction of the anim. (0, 1 : CW, CCW)
    uint16_t u16_currTime = millis();
    uint16_t u16_currTimeMod = u16_currTime;
    uint8_t  u8_numPixels = np_get_length();

    // Reset static vars to starting values
    if (b_animReset) {
        b_animReset = false;

        u16_startTime = u16_currTime;
        u8_direction = prng_upper(2); // 0 or 1
    }

    // For unsigned n-bit int i: -(i) = (2^n - i)
    // Negating this value will cause it to decrease instead of increase with time
    if (u8_direction) { u16_currTimeMod = -u16_currTimeMod; }

    // Generate time-modulated animation
    for (uint8_t u8_pixIdx = 0; u8_pixIdx < u8_numPixels; u8_pixIdx++) {

        // Time modulated pixel index
        uint8_t u8_pixIdxMod = np_get_sine_8((u16_currTimeMod/4 + u8_pixIdx * 512 / u8_numPixels) & 0xFF);
        u8_pixIdxMod = np_get_gamma_8(u8_pixIdxMod);

        np_set_pix_color_pack(u8_pixIdx, u8_pixIdxMod * 0x000100L);
    }

    // Signal mode logic of cycle completion
    if (u16_currTime - u16_startTime > SINE_ON_TIME_MSEC) {
        b_animCycleComplete = true;

        u8_nextSleepTime = SINE_SLEEP_TIME;
    }
}


// Blink out the SARS-CoV-2 RNA sequence in a "quarter" pattern
void anim_cov_quar() {
    anim_cov(0);
}

// Blink out the SARS-CoV-2 RNA sequence using the charset
void anim_cov_char() {
    anim_cov(1);
}

/*!
 @brief             Supporting function for the CoV sequence animations
 @param u8_pattType 0 = Use "quarter" pattern,
                    1 = Use the charset to represent sequence letters
*/
void anim_cov(uint8_t u8_pattType) {
    static uint16_t u16_baseCnt = 0;
    static uint16_t u16_lastTime;
    static uint8_t  u8_stepCount;

    uint16_t u16_currTime = millis();

    // New transition into this anim, reset vars to starting values
    if (b_animReset) {
        b_animReset = false;

        // Don't reset baseCnt. Retain previous location in the sequence.

        u16_lastTime  = u16_currTime;
        u8_stepCount = 0;
    }

    // Map time elapsed 0-COV_STEP_MSEC (msec) to sine index 0-255
    uint8_t u8_cycle = (u16_currTime - u16_lastTime)*255L / COV_STEP_MSEC;

    // Shift sine wave right by 64 to get off-on-off cycle
    uint8_t u8_cBright = np_get_gamma_8(np_get_sine_8(u8_cycle - 64));

    np_clear();
    uint8_t u8_numPixels = np_get_length();
    uint8_t u8_quarter = u8_numPixels/4;                // Quarter of pixels

    // Render the quarter "slice" (or the char) for the current base, with sine-modulated brightness
    switch (read_cov_base(u16_baseCnt))
    {
    case 0: // A (Grn)
        if      (u8_pattType == 0) { np_fill(u8_cBright * 0x000100L, 0, u8_quarter); }  // Force 32-bit mult
        else if (u8_pattType == 1) { draw_char_cent('A', u8_cBright * 0x000100L); }
        break;
    case 1: // C (Blu)
        if      (u8_pattType == 0) { np_fill(u8_cBright * 0x000001L, u8_quarter, u8_quarter); }
        else if (u8_pattType == 1) { draw_char_cent('C', u8_cBright * 0x000001L); }
        break;
    case 2: // G (Yel)
        if      (u8_pattType == 0) { np_fill(u8_cBright * 0x010100L, 2*u8_quarter, u8_quarter); }
        else if (u8_pattType == 1) { draw_char_cent('G', u8_cBright * 0x010100L); }
        break;
    case 3: // U (Red)
        if      (u8_pattType == 0) { np_fill(u8_cBright * 0x010000L, 3*u8_quarter, 0); }
        else if (u8_pattType == 1) { draw_char_cent('U', u8_cBright * 0x010000L); }
        break;
    }

    // Current step complete
    if (u16_currTime - u16_lastTime > COV_STEP_MSEC) {

        // Prepare for next step. Increment base counter, wrap around if necessary
        u16_baseCnt++;
        if (u16_baseCnt == COV_BASES_CNT) { u16_baseCnt = 0; }
        u16_lastTime = u16_currTime;

        u8_stepCount++;

        // We have played enough steps, so signal cycle completion to the mode logic
        if (u8_stepCount == COV_STEP_CNT) {
            b_animCycleComplete = true;

            u8_nextSleepTime = COV_SLEEP_TIME;
        }
    }
}

// Message string 1
void anim_msg_1() {
    anim_msg(sz_msg1, COLOR_PURPLE);
}

// Message string 2
void anim_msg_2() {
    anim_msg(sz_msg2, COLOR_WHEEL);
}

// Message string 3
void anim_msg_3() {
    anim_msg(sz_msg3, COLOR_WHEEL);
}

// Message string 4
void anim_msg_4() {
    anim_msg(sz_msg4, COLOR_GREEN);
}

/*!
 @brief            Supporting function for the message scroll animations.
 @param pu8_msg    Starting address for a null-terminated C-String with the message, stored in flash.
 @param u32_color  Solid color to use for the characters. Use COLOR_WHEEL for a smooth color cycling anim.
*/
void anim_msg(const uint8_t* pu8_msg, uint32_t u32_color) {
    static uint8_t  u8_charCnt; // Counts 0 to LEN-1, index into char array
    static uint8_t  u8_cIn;     // Incoming char
    static uint8_t  u8_cOut;    // Outgoing char
    static int8_t   i8_xIn;
    static int8_t   i8_xOut;
    static uint16_t u16_lastTime;
    static bool     b_readyToShutdown;

    uint16_t u16_currTime = millis();

    // New transition into this anim, reset all vars to starting values
    if (b_animReset) {
        b_animReset = false;
        u8_charCnt = 0;
        u8_cIn  = pgm_read_byte(&pu8_msg[0]);
        u8_cOut = ' ';
        i8_xIn  = 5;
        i8_xOut = 0;
        u16_lastTime  = u16_currTime;
        b_readyToShutdown = false;
    }

    // Iterate animation once per step
    if (u16_currTime - u16_lastTime >= MSG_STEP_MSEC) {
        u16_lastTime = u16_currTime;

        if (i8_xIn == 0) {     // Incoming char has reached middle
            u8_cOut = u8_cIn;  // Incoming becomes outgoing

            // Fetch next char
            u8_charCnt++;
            u8_cIn = pgm_read_byte(&pu8_msg[u8_charCnt]);

            // Reached null term, current cycle complete
            if (u8_cIn == 0) {

                b_readyToShutdown = true;
            }

            // Reset positions
            i8_xIn  = 6;
            i8_xOut = 0;
        }

        i8_xIn--; i8_xOut--;  // Shift both chars left
    }

    // Pick color to use
    uint32_t u32_c;
    if (u32_color == COLOR_WHEEL) { u32_c = np_get_gamma_32(np_hsv_to_pack_hue(u16_currTime*10)); } // Smooth time-modulated color wheel
    else                          { u32_c = u32_color; }                                            // Solid color from calling func

    // Render characters at current pos, no vertical offset
    np_clear();
    draw_char(u8_cIn,  u32_c, i8_xIn, 0);
    draw_char(u8_cOut, u32_c, i8_xOut, 0);

    // Signal mode logic of cycle completion
    if (b_readyToShutdown) {
        b_readyToShutdown = false;
        b_animCycleComplete = true;

        u8_nextSleepTime = MSG_SLEEP_TIME;
    }
}

// Frames sequence 1
void anim_frames_1() {
    anim_frames(&st_sequence1);
}

// Frames sequence 2
void anim_frames_2() {
    anim_frames(&st_sequence2);
}

// Frames sequence 3
void anim_frames_3() {
    anim_frames(&st_sequence3);
}

// Frames sequence 4
void anim_frames_4() {
    anim_frames(&st_sequence4);
}

// Frames sequence 5
void anim_frames_5() {
    anim_frames(&st_sequence5);
}

// Frames sequence 6
void anim_frames_6() {
    anim_frames(&st_sequence6);
}

// Frames sequence 7
void anim_frames_7() {
    anim_frames(&st_sequence7);
}

// Frames sequence 8
void anim_frames_8() {
    anim_frames(&st_sequence8);
}

// Frames sequence 9
void anim_frames_9() {
    anim_frames(&st_sequence9);
}

// Frames sequence 10
void anim_frames_10() {
    anim_frames(&st_sequence10);
}

/*!
 @brief         Supporting function for the frames scroll animations.
 @param pst_f   Pointer to the structure containing frames configuration data.
*/
void anim_frames(FRAMES_CONFIG_T* pst_f) {
    static bool     b_readyToShutdown;
    static uint8_t  u8_frameIdx;     // Counts 0 to LEN-1, index into frames array
    static uint8_t  u8_frameShiftSync;
    static uint8_t  u8_seqRepeatCnt; // Counts how many frame seq's we have played
    static uint8_t  u8_currFrame;
    static uint16_t u16_lastFrameTime;
    static int8_t   i8_x;
    static int8_t   i8_y;

    uint16_t u16_currTime = millis();

    // New transition into this anim, reset static vars to starting values
    if (b_animReset) {

        b_animReset = false;
        b_readyToShutdown = false;
        u8_frameIdx = 0;
        u8_frameShiftSync = 0;
        u8_seqRepeatCnt = 0;
        u8_currFrame  = pgm_read_byte(&(pst_f->pu8_frames[0]));
        u16_lastFrameTime  = u16_currTime;

        // Set initial X and Y position of sprite
        if (pst_f->u8_mode == FRAMES_MODE_SHIFT) { i8_x = pst_f->i8_startPosX; i8_y = pst_f->i8_startPosY; }
        else                                     { i8_x =  0; i8_y = 0; }

    }

    // Time for frame update
    if (u16_currTime - u16_lastFrameTime >= pst_f->u16_frameStep_msec) {
        u16_lastFrameTime = u16_currTime;

        // Fetch next frame
        u8_frameIdx++;
        u8_currFrame = pgm_read_byte(&(pst_f->pu8_frames[u8_frameIdx]));

        // Reached null term, current frame cycle complete
        if (u8_currFrame == 0) {

            u8_seqRepeatCnt++;

            // We have repeated the seq the specified number of times
            if ((pst_f->u8_mode == FRAMES_MODE_STATIC) &&
                (u8_seqRepeatCnt == pst_f->u8_seqRepeatCnt)) {
                b_readyToShutdown = true;

            // Prepare for next frame seq
            } else {
                u8_frameIdx = 0;
                u8_currFrame = pgm_read_byte(&(pst_f->pu8_frames[0]));
            }
        }

        // Handle shift
        if (pst_f->u8_mode == FRAMES_MODE_SHIFT) {

            // Count frames and sync shift to frame transition
            u8_frameShiftSync++;
            if (u8_frameShiftSync == pst_f->u8_frameShiftSync) {
                u8_frameShiftSync = 0;

                // Update X and Y position
                i8_x += pst_f->i8_shiftStepX;
                i8_y += pst_f->i8_shiftStepY;
            }
            // We are offscreen, so animation is complete
            if ( (i8_x <= -5) || (i8_x >= 5) || (i8_y <= -5) || (i8_y >= 5) ) {
                b_readyToShutdown = true;
            }
        }
        
    }

    // Pick color to use
    uint32_t u32_color;
    if (pst_f->u32_color == COLOR_WHEEL) {
        u32_color = np_get_gamma_32(np_hsv_to_pack_hue(u16_currTime*10)); // Smooth time-modulated color wheel
    } else {
        u32_color = pst_f->u32_color; // Solid color from spec
    }

    // Render frame
    np_clear();
    draw_char(u8_currFrame, u32_color, i8_x, i8_y);

    // Signal mode logic of cycle completion
    if (b_readyToShutdown) {
        b_readyToShutdown = false;
        b_animCycleComplete = true;

        u8_nextSleepTime = FRAMES_SLEEP_TIME;
    }
}

void anim_batt_level() {
    static uint16_t u16_lastStepTime;
    static uint8_t u8_battLevel;
    static uint8_t u8_stepLevel;
    static bool    b_readyToShutdown;
    uint16_t u16_currTime = millis();

    // Reset static vars to starting values
    if (b_animReset) {
        b_animReset = false;

        u16_lastStepTime = u16_currTime;
        u8_battLevel = get_batt_level();
        u8_stepLevel = 1;
        b_readyToShutdown = false;
    }

    // Step the animation
    if (u16_currTime - u16_lastStepTime > BATT_LVL_STEP_MSEC) {
        u16_lastStepTime = u16_currTime;

        if (u8_stepLevel != u8_battLevel) {
            u8_stepLevel++;

        // Anim complete, power down
        } else {
            
            b_readyToShutdown = true;
        }
    }

    np_clear();

    // Render current frame
    switch (u8_stepLevel)
    {
    case 1:
        draw_char_cent(BATT_ICON_LVL_1, COLOR_RED);
        break;
    case 2:
        draw_char_cent(BATT_ICON_LVL_2, COLOR_YELLOW);
        break;
    case 3:
        draw_char_cent(BATT_ICON_LVL_3, COLOR_YELLOW);
        break;
    case 4:
        draw_char_cent(BATT_ICON_LVL_4, COLOR_GREEN);
        break;
    case 5:
        draw_char_cent(BATT_ICON_LVL_5, COLOR_GREEN);
        break;
    }

    // Signal mode logic of cycle completion
    if (b_readyToShutdown) {
        b_readyToShutdown = false;
        b_animCycleComplete = true;

        u8_nextSleepTime = BATT_LVL_SLEEP_TIME;
    }
}

// Randomly shuffle animation parameters
void shuffle_anim_params() {
    
    uint8_t u8_direction = prng_upper(2); // 0 or 1

    if (u8_direction) {

        // Reverse ghost direction
        st_sequence2.i8_startPosX = -st_sequence2.i8_startPosX;
        st_sequence2.i8_shiftStepX = -st_sequence2.i8_shiftStepX;

        // Reverse frog direction
        st_sequence4.i8_startPosX = -st_sequence4.i8_startPosX;
        st_sequence4.i8_shiftStepX = -st_sequence4.i8_shiftStepX;

        // Reverse spinner direction
        st_sequence6.pu8_frames = sz_frames6_alt;
        
        // Reverse ball direction
        st_sequence10.pu8_frames = sz_frames10_alt;

    } else {

        // Restore spinner direction
        st_sequence6.pu8_frames = sz_frames6;

        // Restore ball direction
        st_sequence10.pu8_frames = sz_frames10;
    }
}
