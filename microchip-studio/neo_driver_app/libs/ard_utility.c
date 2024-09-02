/* File:      ard_utility.c
 * Author:    Garrett Carter
 * Purpose:   Arduino utility functions
 */

#include "ard_utility.h"
#include <stdint.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>

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

uint8_t strlen(char* ac_str) {
    uint8_t u8_len = 0;

    while (ac_str[u8_len]) {
        u8_len++;
    }

    return u8_len;
}

void rev_string(char* ac_str) {

    uint8_t u8_len = strlen(ac_str);
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

void memset(uint8_t* ptr, uint8_t val, uint8_t num_bytes) {
    
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