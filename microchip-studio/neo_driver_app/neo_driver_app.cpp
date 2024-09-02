/* File:      neo_driver_app.cpp
 * Author:    Garrett Carter
 * Purpose:   Main NeoPixel driver program.
 */

// Define below symbol to compile the eep_data_write program
// Defined by the "eep_data_write" config in Microchip Studio
// When not defined, compile the neo_driver_app
#ifndef COMPILE_EEP_DATA_WRITE

/********************************* INCLUDES **********************************/
#include <Arduino.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include <neo_pixel_slim.h>
#include <multi_button.h>
#include <neo_common.h>
#include <ard_utility.h>
#include <neo_driver_app.h>
#include <prng.h>
#include <draw.h>
#include <anim_blk.h>
#include <anim.h>

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
static void startup_seed_prng();

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


/************************ GLOBAL RAM VARS DEFINITIONS ************************/

// uint16_t u16_frameStep_msec;  // "on" time for each frame in the sequence.
// uint32_t u32_color;           // Color. Set to 0 for dynamic effect.
// uint8_t* pu8_frames;          // Pointer to the frame sequence array stored in flash.
// uint8_t  u8_mode;             // Mode of operation. A value from _FRAMES_MODE_T.

// // STATIC mode params
// uint8_t  u8_seqRepeatCnt;     // Number of times to repeat the frame seq for each anim cycle in STATIC mode.

// // SHIFT mode params
// uint8_t  u8_frameShiftSync;   // Number of frames between shifts, in SHIFT mode.
// int8_t   i8_startPosX;        // Init X,Y position. Should start onscreen.
// int8_t   i8_startPosY;        // 
// int8_t   i8_shiftStepX;       // The X shift to perform on each shift step. Can be pos, neg, or zero.
// int8_t   i8_shiftStepY;       // The Y shift to perform on each shift step. Can be pos, neg, or zero.

// Frame sequence config data
// These are currently stored in RAM, because we change some params at runtime, for "randomization"

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

// Misc vars
static uint8_t u8_anim = 0;                   // Index of current anim in table
static uint8_t u8_mode = SYS_MODE_ANIM_SEL;   // Current mode
static uint16_t au16_pow10[] = { 1, 10, 100, 1000, 10000 }; // Powers of 10 used for B2D
static bool b_animReset = true;               // Used to inform animations to reset
static bool b_animCycleComplete = false;      // Signal from animations to mode logic
static uint32_t u32_randSeed;
static volatile uint8_t u8_wdtCounter = 0;    // Used in WDT ISR
static uint8_t u8_nextSleepTime = WDT_16MS;   // Data from terminating animations to mode logic
static volatile bool b_pinChangeWake = false; // Signal from pin change ISR to mode logic

// Array of function pointers, in order of cycle
static void (*apfn_renderFunc[])(void) {
    //anim_off,
    //anim_white,
    anim_primaries, anim_colorwheel_gamma,
    // anim_half,
    anim_sparkle,
    // anim_marquee, anim_sine_gamma,
    // anim_cov_quar,
    // anim_cov_char,
    anim_msg_1,
    anim_msg_2,
    anim_msg_3,
    anim_msg_4,
    anim_frames_1,
    anim_frames_2,
    anim_frames_3,
    anim_frames_4,
    anim_frames_5,
    anim_frames_6,
    anim_frames_7,
    anim_frames_8,
    anim_frames_9,
    anim_frames_10,
    anim_batt_level,
};

/****************************** GLOBAL OBJECTS *******************************/
static multiButton o_leftBtn, o_rightBtn;

/****************************** SETUP FUNCTION *******************************/
void setup() {
    // Disable unused peripherals to save power in ACTIVE mode

    // The below assignments should be equivalent to RESET values
    // TCCR1 = 0;                                                     // Disable TIMER1
    // GTCCR = 0;                                                     // Disable TIMER1
    // bitClearMask( TIMSK, _BV(OCIE1A) | _BV(OCIE1B) | _BV(TOIE1) ); // Disable all TIMER1 ints
    // PLLCSR = 0;                                                    // Disable PLL
    // USICR = 0;                                                     // Disable USI

    ACSR = _BV(ACD);                                               // Disable analog comparator
    bitSetMask( DIDR0, _BV(ADC2D) );                               // Disable digital input buffer on ADC2
    bitSetMask( PRR, _BV(PRTIM1) | _BV(PRUSI) );                   // Disable TIMER1, USI clocks

    // Read EEPROM settings data
    uint8_t u8_rAnim = eeprom_read_byte(EEP_SETT_ANIM);

    // Validate animation number from EEPROM
    if (u8_rAnim >= ANIM_CNT) {
        u8_rAnim = 0;
        eeprom_update_byte(EEP_SETT_ANIM, u8_rAnim);
    }

    // Update power-on counter
    inc_sat_eep_cntr_u16(EEP_SETT_NUM_POWER_ON);

    startup_seed_prng();
    
    // Update state using EEPROM data
    u8_anim = u8_rAnim;

    // Setup I/O
    bitSetMask(DDRB, IO_NP_ENABLE);               // Set as o/p
    bitSetMask(PORTB, IO_NP_ENABLE);              // Enable MOSFET for NeoPixel power
    delay_msec(5);                                // Time for MOSFET to switch on
    bitClearMask(DDRB, IO_SW_LEFT | IO_SW_RIGHT); // Set as i/p
    bitSetMask(PORTB, IO_SW_LEFT | IO_SW_RIGHT);  // Enable pull-ups

    // Init and clear NeoPixels
    np_init();
    np_show();
    np_set_brightness(BRIGHT_INIT);

    // Init Vars

    // Debug code
    #if DEBUG_ADC_VAL == 1
        while(1) {
            delay_msec(250);
            //draw_value(adc_read(IO_POT_ADC_CH, IO_POT_ADC_REF), 1023);
            draw_value(read_vcc_mv(), 3500);
            np_show();
        }
    #endif

    #if DEBUG_RAND_SEED == 1
        draw_value_binary(u32_randSeed ^ read_vcc_mv());
        np_show();
        delay_msec(3000);
    #endif
    

}

/********************************* MAIN LOOP *********************************/
void loop() {

    // Set brightness from potentiometer value
    uint16_t u16_potVal = adc_read(IO_POT_ADC_CH, IO_POT_ADC_REF);
    uint8_t u8_bright = np_get_gamma_8(map(u16_potVal, 0, 1023, BRIGHT_MIN, 255)); // Scale value
    np_set_brightness(u8_bright);

    // Monitor vcc for low batt condition
    check_batt(read_vcc_mv());

    // Update "random" counter based on human interface 
    if ( (!bitReadMask(PINB, IO_SW_LEFT)) || (!bitReadMask(PINB, IO_SW_RIGHT)) ) {
        u32_randSeed++;
    }

    mode_logic();

    if ( (u8_mode == SYS_MODE_ANIM_SEL) || (u8_mode == SYS_MODE_ANIM_SHUFF) ) {
        (*apfn_renderFunc[u8_anim])();    // Render one frame in current anim
        np_show();                   // and update the NeoPixels to show it
    }
    if (u8_mode == SYS_MODE_PIX_ADJ) {
        np_fill_all(0xFF0000);
        np_show();
    }

    #if (DEBUG_SOFT_RESET_ON_INTERVAL_EN == 1)
    if (millis() > (DEBUG_SOFT_RESET_INTERVAL_SEC * 1000))
    {
        soft_reset();
    }
    #endif /* DEBUG_SOFT_RESET_ON_INTERVAL_EN */

}

static void randomize_anim() {
    uint8_t u8_newAnim;

    // Pick a new random animation, different from the current one
    do { u8_newAnim = prng_upper(ANIM_CNT); } while (u8_newAnim == u8_anim);

    u8_anim = u8_newAnim;
    
    // Randomly shuffle animation parameters
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

static void mode_logic() {
    static uint8_t u8_animCycleCount = 0;

    // Handle power down after an animation cycle
    if (b_animCycleComplete) {
        b_animCycleComplete = false;
        u8_animCycleCount++;

        // We have completed an anim cycle, update the cycle counter in EEPROM
        inc_sat_eep_cntr_u16(EEP_SETT_NUM_CYCLES);

        wd_enable(u8_nextSleepTime); // Value set by the terminating animation
        shutdown();

        // Don't randomize if we woke with switch
        if (u8_mode == SYS_MODE_ANIM_SHUFF && !b_pinChangeWake) {
            randomize_anim();
        }
        b_pinChangeWake = false;
        b_animReset = true;
    }

    // Switch to shuffle mode after playing a few cycles of the manually selected anim
    if (u8_mode == SYS_MODE_ANIM_SEL && u8_animCycleCount == ANIM_MANUAL_CYCLES-1) {
        u8_mode = SYS_MODE_ANIM_SHUFF;
    }

    // Update button events
    uint8_t u8_lEvent = o_leftBtn.check(bitReadMask(PINB, IO_SW_LEFT));
    uint8_t u8_rEvent = o_rightBtn.check(bitReadMask(PINB, IO_SW_RIGHT));

    // Respond to left button events
    switch (u8_lEvent)
    {
    // Left Click
    case 1:

        if (u8_mode == SYS_MODE_ANIM_SEL || u8_mode == SYS_MODE_ANIM_SHUFF)
        {
            u8_mode = SYS_MODE_ANIM_SEL;
            u8_animCycleCount = 0;

            if (u8_anim) { u8_anim--; }               // Go to prior anim
            else         { u8_anim = ANIM_CNT - 1; }  // or "wrap around" to last anim

            b_animReset = true;

            eeprom_update_byte(EEP_SETT_ANIM, u8_anim);
            store_rand_seed();
        }

        break;
    // Left Double Click
    case 2:

        break;
    // Left Hold
    case 3:

        break;
    // Left Long Hold
    case 4:

        if (u8_mode == SYS_MODE_ANIM_SEL || u8_mode == SYS_MODE_ANIM_SHUFF) {
            u8_mode = SYS_MODE_PIX_ADJ;
        }
        else if (u8_mode == SYS_MODE_PIX_ADJ) {
            
            // Reset statistics
            // EEPROM.put(EEP_SETT_NUM_CYCLES, (const uint16_t)(0));
            // EEPROM.put(EEP_SETT_NUM_POWER_ON, (const uint16_t)(0));
            // anim_flash_chars("SR");

            // Print statistics
            uint16_t u16_val;
            
            u16_val = eeprom_read_word(EEP_SETT_NUM_CYCLES);
            anim_flash_chars("CYCL-CNT");
            anim_print_dec_u32((uint32_t)(u16_val));

            u16_val = eeprom_read_word(EEP_SETT_NUM_POWER_ON);
            anim_flash_chars("PWR-CNT");
            anim_print_dec_u32((uint32_t)(u16_val));

            // Return to usual mode
            u8_mode = SYS_MODE_ANIM_SEL;
        }


        break;
    }

    // Respond to right button events
    switch (u8_rEvent)
    {
    // Right Click
    case 1:

        if (u8_mode == SYS_MODE_ANIM_SEL || u8_mode == SYS_MODE_ANIM_SHUFF)
        {
            u8_mode = SYS_MODE_ANIM_SEL;
            u8_animCycleCount = 0;

            if (u8_anim < (ANIM_CNT - 1)) { u8_anim++;   }  // Advance to next anim
            else                          { u8_anim = 0; }  // or "wrap around" to start

            b_animReset = true;

            eeprom_update_byte(EEP_SETT_ANIM, u8_anim);
            store_rand_seed();
        }

        break;
    // Right Double Click
    case 2:

        break;
    // Right Hold
    case 3:

        break;
    // Right Long Hold
    case 4:
        if (u8_mode == SYS_MODE_ANIM_SEL || u8_mode == SYS_MODE_ANIM_SHUFF) {
            u8_mode = SYS_MODE_PIX_ADJ;
        }
        else if (u8_mode == SYS_MODE_PIX_ADJ) {
            u8_mode = SYS_MODE_ANIM_SEL;
        }

        break;
    }
}

/***************************** RENDER FUNCTIONS ******************************/

// All NeoPixels off
static void anim_off() {
    np_clear();
}

static void anim_white() {
    np_fill_all(0xFFFFFF);
}

// Cycle through primary colors (red, green, blue), full brightness.
static void anim_primaries() {
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
static void anim_colorwheel_gamma() {
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
static void anim_half() {
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
static void anim_sparkle() {
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
static void anim_marquee() {
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
static void anim_sine_gamma() {
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
static void anim_cov_quar() {
    anim_cov(0);
}

// Blink out the SARS-CoV-2 RNA sequence using the charset
static void anim_cov_char() {
    anim_cov(1);
}

/*!
 @brief             Supporting function for the CoV sequence animations
 @param u8_pattType 0 = Use "quarter" pattern,
                    1 = Use the charset to represent sequence letters
*/
static void anim_cov(uint8_t u8_pattType) {
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
static void anim_msg_1() {
    anim_msg(sz_msg1, 0xFF00FF);
}

// Message string 2
static void anim_msg_2() {
    anim_msg(sz_msg2);
}

// Message string 3
static void anim_msg_3() {
    anim_msg(sz_msg3);
}

// Message string 4
static void anim_msg_4() {
    anim_msg(sz_msg4, COLOR_GREEN);
}

/*!
 @brief            Supporting function for the message scroll animations.
 @param pu8_msg    Starting address for a null-terminated C-String with the message, stored in flash.
 @param u32_color  Solid color to use for the characters. Leave at default 0 for a smooth color cycling anim.
*/
static void anim_msg(const uint8_t* pu8_msg, uint32_t u32_color) {
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
    if (u32_color == 0) { u32_c = np_get_gamma_32(np_hsv_to_pack_hue(u16_currTime*10)); } // Smooth time-modulated color wheel
    else                { u32_c = u32_color; }                                                  // Solid color from calling func

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
static void anim_frames_1() {
    anim_frames(&st_sequence1);
}

// Frames sequence 2
static void anim_frames_2() {
    anim_frames(&st_sequence2);
}

// Frames sequence 3
static void anim_frames_3() {
    anim_frames(&st_sequence3);
}

// Frames sequence 4
static void anim_frames_4() {
    anim_frames(&st_sequence4);
}

// Frames sequence 5
static void anim_frames_5() {
    anim_frames(&st_sequence5);
}

// Frames sequence 6
static void anim_frames_6() {
    anim_frames(&st_sequence6);
}

// Frames sequence 7
static void anim_frames_7() {
    anim_frames(&st_sequence7);
}

// Frames sequence 8
static void anim_frames_8() {
    anim_frames(&st_sequence8);
}

// Frames sequence 9
static void anim_frames_9() {
    anim_frames(&st_sequence9);
}

// Frames sequence 10
static void anim_frames_10() {
    anim_frames(&st_sequence10);
}

/*!
 @brief         Supporting function for the frames scroll animations.
 @param pst_f   Pointer to the structure containing frames configuration data.
*/
static void anim_frames(FRAMES_CONFIG_T* pst_f) {
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

static void anim_batt_level() {
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

/*********************************** ISR's ***********************************/

// Left or Right Switch
ISR(PCINT0_vect) {

    // Signal to mode logic
    b_pinChangeWake = true;

    // If we woke up with a switch, we don't want any additional WDT sleeps.
    u8_wdtCounter = 0;
}

// Watchdog timer
ISR(WDT_vect) {

    // Disable WDT interrupt
    bitClear(WDTCR, WDIE);
}

/***************************** SUPPORT FUNCTIONS *****************************/

// Return 1 to 5 corresponding to current battery level: 1 => empty, 5 => full.
static uint8_t get_batt_level() {
    uint16_t u16_batt_mv = read_vcc_mv();

    // Determine battery level based on voltage ranges
    if (u16_batt_mv < BATT_LVL_THRESH_1_MV) {
        return 1;
    } else if ( (u16_batt_mv >= BATT_LVL_THRESH_1_MV) && (u16_batt_mv < BATT_LVL_THRESH_2_MV) ) {
        return 2;
    } else if ( (u16_batt_mv >= BATT_LVL_THRESH_2_MV) && (u16_batt_mv < BATT_LVL_THRESH_3_MV) ) {
        return 3;
    } else if ( (u16_batt_mv >= BATT_LVL_THRESH_3_MV) && (u16_batt_mv < BATT_LVL_THRESH_4_MV) ) {
        return 4;
    } else {
        return 5;
    }
}

// Check battery level and respond to a low battery condition persisting for some time.
static void check_batt(uint16_t u16_batt_mv) {
    static uint16_t u16_onTimeAcc = 0;
    static uint16_t u16_lastScan = 0;
    uint16_t u16_currTime = millis();
    uint8_t u8_statusFlags;
    bool b_shutdown = false;

    u8_statusFlags = eeprom_read_byte(EEP_SETT_FLAGS);

    // Has the low battery condition been previously detected?
    if (bitReadMask(u8_statusFlags, STAT_FLG_BATT_DEAD))
    {
        // Has battery level risen above the charge threshold?
        if (u16_batt_mv >= BATT_CHG_THRESH_MV)
        {
            // Clear Flag and continue
            bitClearMask(u8_statusFlags, STAT_FLG_BATT_DEAD);
            eeprom_update_byte(EEP_SETT_FLAGS, u8_statusFlags);

        } else {
            // Shutdown
            b_shutdown = true;
        }
    }
    
    // Timer Running
    if (u16_batt_mv < BATT_DISCHG_THRESH_MV)
    {
        if (u16_lastScan)  u16_onTimeAcc += (u16_currTime - u16_lastScan);

        u16_lastScan = u16_currTime;
    }
    // Timer Reset
    else
    {
        u16_onTimeAcc = u16_lastScan = 0;
    }
    // Timer Done
    if (u16_onTimeAcc > BATT_TON_MSEC)
    {
        b_shutdown = true;
    }

    // Shutdown
    if (b_shutdown)
    {
        // Set the battery dead flag in EEPROM
        bitSetMask(u8_statusFlags, STAT_FLG_BATT_DEAD);
        eeprom_update_byte(EEP_SETT_FLAGS, u8_statusFlags);

        anim_low_batt();
        shutdown();

        u16_onTimeAcc = u16_lastScan = 0; // Timer Reset
    }
    

}

static void enable_pc_ints() {

    GIMSK |= _BV(PCIE);                 // Enable Pin Change Interrupts
    PCMSK |= _BV(PCINT0) | _BV(PCINT1); // Enable Specific Pins
}

static void disable_pc_ints() {

    PCMSK = 0x00;                       // Disable All Pins
    GIMSK &= ~(_BV(PCIE));              // Disable Pin Change Interrupts
}

// Turn off LEDs and send ATtiny to sleep, waiting for interrupt (WDT or pin change) to wake
static void shutdown() {

    np_clear();
    np_show();

    //DIDR0 |= (_BV(ADC2D));               // Disable digital input buffer for ADC2
    bitClearMask(DDRB, IO_NP_DATA          // Hi-Z to save power
                      |IO_NP_ENABLE);      // Hi-Z to turn off MOSFET
    bitSetMask(PORTB, IO_NP_DATA);         // Pull-up
    bitClearMask(PORTB, IO_NP_ENABLE);     // No Pull-up, there is external pull-down

    ADCSRA &= ~(_BV(ADEN));                // Disable ADC

    enable_pc_ints();                      // Wake-up Source
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();

    //sleep_bod_disable();                 // ATtiny85 Revision C or newer only
    sei();                                 // Global Interrupt Enable
    sleep_cpu();
    // Exec PC or WDT ISR, then return here

    // Handle additional WDT sleeps, if needed
    while (u8_wdtCounter > 0) {

        _wd_hw_enable(WDT_8S);
        //sleep_bod_disable();             // ATtiny85 Revision C or newer only
        sei();                             // Global Interrupt Enable
        sleep_cpu();
        // Exec PC or WDT ISR, then return here

        // Counter will be set to 0 if we woke by pin change.
        if (u8_wdtCounter == 0) { break; }

        u8_wdtCounter--;
    }
    
    sleep_disable();
    disable_pc_ints();                     // Don't want these during normal operation

    bitSetMask(DDRB, IO_NP_DATA            // Restore for NeoPixel Lib
                    |IO_NP_ENABLE);
    bitSetMask(PORTB, IO_NP_ENABLE);       // Enable power for NeoPixels
    ADCSRA |= _BV(ADEN);                   // Enable ADC

    // Reset the button state machines
    o_leftBtn.reset();
    o_rightBtn.reset();
}

// dig = 0 is LS digit
static uint8_t extract_digit(uint16_t u16_val, uint8_t u8_dig) {
    return u16_val / au16_pow10[u8_dig] % 10 ;
}


// Read 1.1V reference against VCC
static uint16_t read_vcc_mv() {

    #if DEBUG_BATT_LVL == 1
        return SPOOF_BATT_LVL;
    #endif

    // Calculate Vcc (in mV)
    // Unsigned division, since all values are positive
    return ((uint32_t)(ADC_MAX_VALUE * ADC_INT_1V1_REF_VOLTAGE * MILLIVOLTS_PER_VOLT) / adc_read(VCC_AN_MEAS_CH, VCC_AN_MEAS_REF));
}

// Extract base data from sequence table stored in EEPROM
static uint8_t read_cov_base(uint16_t u16_baseNum) {

    // 0th base is encoded in the 2 most significant bits
    uint8_t u8_pos = 3 - u16_baseNum % 4;
    uint8_t u8_data = eeprom_read_byte((uint8_t*)(EEP_COV_DATA_START_ADDR + u16_baseNum/4));

    return (u8_data >> (2 * u8_pos)) & 0b00000011;
}

// External interface for WDT. Call with one of the WDT_XXX constants. Sets up WDT software and hardware.
static void wd_enable(uint8_t u8_timeout) {

    // Setup our software counter.
    if (u8_timeout > WDT_8S) {
        u8_wdtCounter = u8_timeout - WDT_8S; // Calculate 8 second repeat count
        u8_timeout = WDT_8S;                 // Set hardware for 8 seconds
    } else {
        u8_wdtCounter = 0;
    }

    _wd_hw_enable(u8_timeout);
}

// Sets up WDT hardware. Enable WDT interrupt and set prescale value.
static void _wd_hw_enable(uint8_t u8_timeout) {

    //  FUSE: WDTON=1   WDT in Safety level 1, initially disabled.
    // We are cycling between these two states:
    // WDE  WDIE   Watchdog Timer State    Action on Time-out
    // 0    0      Stopped                 None
    // 0    1      Running                 Interrupt
    wdt_reset();
    WDTCR = ((1 << WDIE) | ((u8_timeout & 0x8) << 2) | (u8_timeout & 0x7));
}

static void store_rand_seed() {

    eeprom_update_dword(EEP_SETT_RSEED, u32_randSeed);
}

static void startup_seed_prng() {

    uint8_t adc_read_ctr = 8;
    uint8_t adc_rand_bits = 0;
    
    // Do consecutive reads of the ADC to get some entropy, compose a seed from the LSB of each read
    do {
        if (bitRead(adc_read(RAND_AN_MEAS_CH, RAND_AN_MEAS_REF), 0))
        {
            bitSet(adc_rand_bits, 0);
        }
        adc_rand_bits <<= 1;

        // Decrement loop index gives a smaller code size, according to Atmel app note.
        adc_read_ctr--;
    } while (adc_read_ctr);
    
    // Read the seed from EEPROM, increment it with the ADC-derived bits, store it back
    u32_randSeed = eeprom_read_dword(EEP_SETT_RSEED);
    u32_randSeed += adc_rand_bits;
    store_rand_seed();

    prng_seed(u32_randSeed);

}

#endif