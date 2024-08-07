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
#define DEBUG_COV_ANIM  0
#define DEBUG_LOW_BATT  0
#define DEBUG_BATT_LVL  0
#define DEBUG_ADC_VAL   0
#define DEBUG_RAND_SEED 0
#define DEBUG_BRIGHT    0

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

/******************************** PROTOTYPES *********************************/
// Utility functions
static void randomize_anim();
static void mode_logic();
static uint8_t get_batt_level();
static void check_batt(uint16_t u16_batt_mv);
static void enable_pc_ints();
static void disable_pc_ints();
static void shutdown();
static uint16_t read_vcc_mv();
static uint8_t read_cov_base(uint16_t u16_baseNum);
static uint8_t extract_digit(uint16_t u16_val, uint8_t u8_dig);
static void wd_enable(uint8_t u8_timeout);
static void _wd_hw_enable(uint8_t u8_timeout);
static void store_rand_seed();
static uint8_t eep_char_read_line(char c_char, uint8_t line);
static void inc_sat_eep_cntr_u16(uint8_t eep_addr);

// Drawing functions
static void draw_char(char c_char, uint32_t u32_color, int8_t i8_x=0, int8_t i8_y=0);
static void draw_value(uint32_t u32_val, uint32_t u32_maxVal);
static void draw_value_binary(uint32_t u32_val);


// Animation "task" functions
static void anim_off();
static void anim_white();
static void anim_primaries();
static void anim_colorwheel_gamma();
static void anim_half();
static void anim_sparkle();
static void anim_marquee();
static void anim_sine_gamma();
static void anim_cov_quar();
static void anim_cov_char();
static void anim_cov(uint8_t u8_pattType);
static void anim_msg_1();
static void anim_msg_2();
static void anim_msg_3();
static void anim_msg_4();
static void anim_msg(const uint8_t* pu8_msg, uint32_t u32_color=0);
static void anim_frames_1();
static void anim_frames_2();
static void anim_frames_3();
static void anim_frames_4();
static void anim_frames_5();
static void anim_frames_6();
static void anim_frames_7();
static void anim_frames_8();
static void anim_frames_9();
static void anim_frames_10();
static void anim_frames(FRAMES_CONFIG_T* pst_f);
static void anim_batt_level();

// Blocking animations
static void anim_voltage(uint16_t u16_mv);
static void anim_low_batt();
static void anim_flash_chars(char* msg);
static void anim_color_wipe(uint32_t u32_color, uint16_t u16_wait);
static void anim_color_count(uint32_t u32_color, uint16_t u16_wait, uint16_t u16_count);
static void anim_print_dec_u32(uint32_t u32_val);

// ISR's
ISR(PCINT0_vect);
ISR(WDT_vect);

/****************************** FLASH CONSTANTS ******************************/

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

#endif /* NEO_DRIVER_APP_H */