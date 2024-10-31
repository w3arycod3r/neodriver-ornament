/* File:      utility.c
 * Author:    Garrett Carter
 * Purpose:   Utility functions
 */

#include "utility.h"
#include <stdint.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include <Arduino.h>
#include <wiring_analog.h>

static uint16_t au16_pow10[] = { 1, 10, 100, 1000, 10000 }; // Powers of 10 used for B2D

// ac_str must be large enough for the decimal digits and null terminator
// See U32_DEC_STR_MAX_BUFF_SIZE
// Reference code: https://github.com/mpaland/printf/blob/master/printf.c
void u32_to_dec_string(uint32_t u32_val, char* ac_str) {
    uint8_t u8_idx = 0;

    // Convert binary to decimal
    do {
        char c_digit = (char)(u32_val % 10);
        ac_str[u8_idx++] = '0' + c_digit;
        u32_val /= 10;
    } while (u32_val);

    // Add null terminator
    ac_str[u8_idx] = '\0';

    // The digits were placed in reverse by above algorithm
    rev_string(ac_str);
}

uint8_t util_strlen(char* ac_str) {
    uint8_t u8_len = 0;

    while (ac_str[u8_len]) {
        u8_len++;
    }

    return u8_len;
}

void rev_string(char* ac_str) {

    uint8_t u8_len = util_strlen(ac_str);
    if (u8_len < 2) {
        // Nothing to reverse...
        return;
    }
    
    char c_tmp;
    uint8_t u8_i, u8_j;

    // Swap chars from opposite ends of string, in place, until the indices meet in the middle
    for (u8_i = 0, u8_j = u8_len - 1; u8_i < u8_j; u8_i++, u8_j--) {
        c_tmp = ac_str[u8_i];
        ac_str[u8_i] = ac_str[u8_j];
        ac_str[u8_j] = c_tmp;
    }
}

void util_memset(uint8_t* ptr, uint8_t val, uint8_t num_bytes) {
    
    // Decrement loop index gives a smaller code size, according to Atmel app note.
    do {
        *ptr = val;
        ptr++;
        num_bytes--;
    } while (num_bytes);
}

// https://www.nongnu.org/avr-libc/user-manual/FAQ.html#faq_softreset
void soft_reset() {

    // Enable the watchdog timer in "reset" mode, not "interrupt" mode.
    wdt_enable(WDTO_15MS);

    // Wait for the watchdog to reset the processor
    while (1) {}
}

// Increment a u16 counter in EEPROM with "saturate"
// Saturate the counter at max value, don't wrap around
void inc_sat_eep_cntr_u16(uint16_t* eep_addr) {
    uint16_t u16_eepCounter;

    u16_eepCounter = eeprom_read_word(eep_addr);

    if (u16_eepCounter != 0xFFFF) {
        u16_eepCounter++;
        eeprom_update_word(eep_addr, u16_eepCounter);
    }
}

// dig = 0 is LS digit
uint8_t extract_digit(uint16_t u16_val, uint8_t u8_dig) {
    return u16_val / au16_pow10[u8_dig] % 10 ;
}

// Read 1.1V reference against VCC
uint16_t read_vcc_mv() {

    #ifdef DEBUG_BATT_LVL_EN
        return SPOOF_BATT_LVL;
    #endif

    // Calculate Vcc (in mV)
    // Unsigned (32-bit) division, since all values are positive
    return ((uint32_t)(ADC_MAX_VALUE * ADC_INT_1V1_REF_VOLTAGE * MILLIVOLTS_PER_VOLT) / adc_read(VCC_AN_MEAS_CH, VCC_AN_MEAS_REF));
}

// Return 1 to 5 corresponding to current battery level: 1 => empty, 5 => full.
uint8_t get_batt_level() {
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

// Shift in upper num_bits from data_in, into data, from the right
// num_bits should be <= 8
void shift_in_from_right(uint8_t* data, const uint8_t data_in, const uint8_t num_bits)
{
    if (num_bits == 0)
    {
        return;
    }
    
    // A bit mask with num_bits of 1's in lower positions
    uint8_t bitmask = (1 << num_bits) - 1;
    // Handle 8 bits specially, overflows the above left shift..
    if (num_bits == 8)
    {
        bitmask = 0xFF;
    }
    
    *data <<= num_bits;
    *data |= ((data_in >> (8-num_bits)) & bitmask);
}

uint32_t map(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
