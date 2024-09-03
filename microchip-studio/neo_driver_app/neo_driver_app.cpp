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
#include <utility.h>
#include <neo_driver_app.h>
#include <prng.h>
#include <draw.h>
#include <anim_blk.h>
#include <anim.h>
#include <eep_data.h>

/******************************** PROTOTYPES *********************************/
static void randomize_anim();
static void mode_logic();

static void check_batt();

static void store_rand_seed();
static void startup_seed_prng();

static void enable_pc_ints();
static void disable_pc_ints();
static void shutdown();
static void wd_enable(uint8_t u8_timeout);
static void _wd_hw_enable(uint8_t u8_timeout);

// ISR's
ISR(PCINT0_vect);
ISR(WDT_vect);

/****************************** FLASH CONSTANTS ******************************/
// NOTE: If these are defined in the header file instead, they get placed in flash twice!
// PROGMEM arrays should be defined in .cpp/.c files!

/************************ GLOBAL RAM VARS DEFINITIONS ************************/

// Shared with anim.c
extern bool b_animReset;           // Used by mode logic to inform animations to reset
extern bool b_animCycleComplete;   // Signal from animations to mode logic
extern uint8_t u8_nextSleepTime;   // Data from terminating animations to mode logic

// Misc vars
static uint8_t u8_anim = 0;                   // Index of current anim in table
static uint8_t u8_mode = SYS_MODE_ANIM_SEL;   // Current mode
static uint32_t u32_randSeed;
static volatile uint8_t u8_wdtCounter = 0;    // Used by WDT code to handle multiple sleeps
static volatile bool b_pinChangeWake = false; // Signal from pin change ISR to mode logic

// Array of animation "task" function pointers, in order of cycle
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
    #if DEBUG_ADC_VAL_EN == 1
        while(1) {
            delay_msec(250);
            //draw_value(adc_read(IO_POT_ADC_CH, IO_POT_ADC_REF), 1023);
            draw_value(read_vcc_mv(), 3500);
            np_show();
        }
    #endif

    #if DEBUG_RAND_SEED_EN == 1
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

    // Monitor Vcc for low batt condition
    check_batt();

    // Update "random" counter based on human interface 
    if ( (!bitReadMask(PINB, IO_SW_LEFT)) || (!bitReadMask(PINB, IO_SW_RIGHT)) ) {
        u32_randSeed++;
    }

    mode_logic();

    if ( (u8_mode == SYS_MODE_ANIM_SEL) || (u8_mode == SYS_MODE_ANIM_SHUFF) ) {
        (*apfn_renderFunc[u8_anim])();    // Render one frame in current anim
        np_show();                        // and update the NeoPixels to show it
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
    
    shuffle_anim_params();
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
            // anim_blk_flash_chars("SR");

            // Print statistics
            uint16_t u16_val;
            
            u16_val = eeprom_read_word(EEP_SETT_NUM_CYCLES);
            anim_blk_flash_chars("CYCL-CNT");
            anim_blk_print_dec_u32((uint32_t)(u16_val));

            u16_val = eeprom_read_word(EEP_SETT_NUM_POWER_ON);
            anim_blk_flash_chars("PWR-CNT");
            anim_blk_print_dec_u32((uint32_t)(u16_val));

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

// Check battery level and respond to a low battery condition persisting for some time.
static void check_batt() {
    static uint16_t u16_onTimeAcc = 0;
    static uint16_t u16_lastScan = 0;
    uint16_t u16_currTime = millis();
    uint16_t u16_batt_mv = read_vcc_mv();
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

        anim_blk_low_batt();
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