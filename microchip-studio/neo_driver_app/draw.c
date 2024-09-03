/* File:      draw.c
 * Author:    Garrett Carter
 * Purpose:   Drawing functions
 */

#include "draw.h"
#include <stdint.h>
#include <stdbool.h>
#include <neo_pixel_slim.h>
#include <neo_common.h>
#include <avr/eeprom.h>
#include <eep_data.h>

/****************************** STATIC PROTOTYPES ******************************/
static uint8_t eep_char_read_line(char c_char, uint8_t line);

// Display an integer value (0-maxVal) on the LEDs
void draw_value(uint32_t u32_val, uint32_t u32_maxVal) {

    // Linearly scale the value -> number of pixels to fill
    uint8_t u8_scaledVal = map(u32_val, 0, u32_maxVal, 0, np_get_length());

    np_clear();
    if (u8_scaledVal != 0)  { np_fill(COLOR_PURPLE, 0, u8_scaledVal); }

}

// Represent each bit with an ON or OFF pixel, LSB first.
// Bit 0 corresponds to pixel 0, bit 1 to pixel 1, etc.
void draw_value_binary(uint32_t u32_val) {
    np_clear();

    // Represent each bit with an ON or OFF pixel, LSB first
    for (uint8_t u8_bit = 0; u8_bit < 32; u8_bit++) {

        // set_pix_color() will enforce pixel index bounds
        if (bitRead(u32_val, 0)) { np_set_pix_color_pack(u8_bit, COLOR_GREEN); }

        u32_val >>= 1;
    }
}

/*!
 @brief            Render an ASCII character onto a 5x5 NeoPixel matrix. Simply draws over the
                   current framebuffer. Destructive only to "on" bits in the font.
                   (X,Y) controls a shift of the char. (0,0) = no shift.
 @param c_char     ASCII character to render.
 @param u32_color  Color to use when setting bits.
 @param i8_x       X shift to perform. +X is right, -X is left. X <= -5 or X >= 5 is out of frame.
 @param i8_y       Y shift to perform. +Y is down, -Y is up. Y <= -5 or Y >= 5 is out of frame.
*/
void draw_char(char c_char, uint32_t u32_color, int8_t i8_x, int8_t i8_y) {

    // Enforce bounds of our ASCII char set
    if ((c_char < ASCII_START) || (c_char > ASCII_START + ASCII_NUM_CHARS - 1)) {
        return;
    }

    uint8_t au8_buffer[5]; // 5x5 framebuffer

    // Copy char data from EEPROM into framebuffer & perform X shift
    for (uint8_t u8_i = 0; u8_i < 5; u8_i++) {
        au8_buffer[u8_i] = eep_char_read_line(c_char, u8_i);

        if      (i8_x < 0) { au8_buffer[u8_i] <<= -i8_x; }   // Shift left
        else if (i8_x > 0) { au8_buffer[u8_i] >>=  i8_x; }   // Shift right
    }

    // Perform Y shift
    if (i8_y != 0) {
        bool shiftDir = true;                                    // True = Down, False = Up
        if (i8_y < 0) { shiftDir = false; i8_y = -i8_y; }        // Make y positive

        for (uint8_t u8_shift = 0; u8_shift < i8_y; u8_shift++)  // Perform y # of shifts
        {
            // Shift down
            if (shiftDir) {
                for (int8_t i8_i = 4; i8_i > 0; i8_i--) {        // i = 4..1
                    au8_buffer[i8_i] = au8_buffer[i8_i - 1];
                }
                au8_buffer[0] = 0;

            // Shift up
            } else {
                for (uint8_t u8_i = 0; u8_i < 4; u8_i++) {       // i = 0..3
                    au8_buffer[u8_i] = au8_buffer[u8_i + 1];
                }
                au8_buffer[4] = 0;
            }
        }
    }

    // Render the framebuffer
    for (uint8_t u8_row = 0; u8_row < 5; u8_row++) {      // Row 0..4, top to bottom
        uint8_t u8_line = au8_buffer[u8_row];             // Get line data from buffer

        for (uint8_t u8_col = 0; u8_col < 5; u8_col++) {  // Col 0..4, right to left
            uint8_t u8_pixIndex;

            // Calculate pixel index from (row,col)
            #if   ANIM_ROT_SEL == 1
                if (u8_col % 2 == 0) { u8_pixIndex = u8_col*5     + 4-u8_row; }
                else                 { u8_pixIndex = u8_col*5     +   u8_row; }
            #elif ANIM_ROT_SEL == 2
                if (u8_row % 2 == 0) { u8_pixIndex = (4-u8_row)*5 + 4-u8_col; }
                else                 { u8_pixIndex = (4-u8_row)*5 +   u8_col; }
            #elif ANIM_ROT_SEL == 3
                if (u8_col % 2 == 0) { u8_pixIndex = (4-u8_col)*5 +   u8_row; }
                else                 { u8_pixIndex = (4-u8_col)*5 + 4-u8_row; }
            #elif ANIM_ROT_SEL == 4
                if (u8_row % 2 == 0) { u8_pixIndex = u8_row*5     +   u8_col; }
                else                 { u8_pixIndex = u8_row*5     + 4-u8_col; }
            #endif


            if (u8_line & 0x01) {
                np_set_pix_color_pack(u8_pixIndex, u32_color); // Extract bit for (row,col)
            }

            u8_line >>= 1; // Next column
        }
    }
}

// Draw a character, centered (no X or Y shift)
void draw_char_cent(char c_char, uint32_t u32_color)
{
    draw_char(c_char, u32_color, 0, 0);
}

// Extract a line of a character from the compressed EEPROM char data
// line: 0-4, top-bottom
static uint8_t eep_char_read_line(char c_char, uint8_t line)
{
    uint16_t char_index = (c_char - ASCII_START);
    uint16_t bit_start_index = (char_index * MATRIX_NUM_PIX) + (MATRIX_WIDTH_PIX * line);
    // Byte index where beginning of line data is found
    uint16_t byte_start_index = bit_start_index / BITS_IN_BYTE;

    uint8_t* byte_start_addr = (uint8_t*)(EEP_CHAR_DATA_START_ADDR + byte_start_index);
    // Bit which is the first bit of the line data (left-most)
    uint16_t bit_within_byte_index = BITS_IN_BYTE - (bit_start_index % BITS_IN_BYTE) - 1;

    // Compose a u16 with first and second bytes, since the line data *may* span two bytes
    uint16_t u16_eep_read = ((eeprom_read_byte(byte_start_addr) << 8) | eeprom_read_byte(byte_start_addr+1));
    uint16_t bit_within_word_index = bit_within_byte_index + BITS_IN_BYTE;

    // Extract the 5-bit line data, place in lower 5-bits of the byte to be returned
    uint8_t line_data = (u16_eep_read >> (bit_within_word_index - MATRIX_WIDTH_PIX + 1)) & 0x1F;

    return line_data;
    
}

// Extract base data from sequence table stored in EEPROM
uint8_t read_cov_base(uint16_t u16_baseNum) {

    // 0th base is encoded in the 2 most significant bits
    uint8_t u8_pos = 3 - u16_baseNum % 4;
    uint8_t u8_data = eeprom_read_byte((uint8_t*)(EEP_COV_DATA_START_ADDR + u16_baseNum/4));

    return (u8_data >> (2 * u8_pos)) & 0b00000011;
}
