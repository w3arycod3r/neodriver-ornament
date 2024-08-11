/* File:      ard_utility.cpp
 * Author:    Garrett Carter
 * Purpose:   Arduino utility functions
 */

#include "ard_utility.h"
#include <stdint.h>

// ac_str must be large enough for the decimal digits and null terminator
// See U32_DEC_STR_MAX_BUFF_SIZE
void u32_to_dec_string(uint32_t u32_val, char* ac_str) {
    // Reference code: https://github.com/mpaland/printf/blob/master/printf.c
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