/* File:      anim_blk.c
 * Author:    Garrett Carter
 * Purpose:   Animation functions, blocking
 */

#include "anim_blk.h"
#include <stdint.h>
#include <neo_pixel_slim.h>
#include <Arduino.h>
#include <draw.h>
#include <utility.h>

// Flash the voltage with 3 digits of precision
void anim_blk_voltage(uint16_t u16_mv) {

    for (uint8_t u8_dig = 3; u8_dig >= 1; u8_dig--) // Digits 3 to 1
    {
        uint8_t u8_digVal = extract_digit(u16_mv, u8_dig);
        anim_blk_color_count(np_rgb_to_pack(  0, 255,   0), 150, u8_digVal);
        delay_msec(150 * u8_digVal);

        np_clear();
        np_show();
        delay_msec(300);
    }
}

// Quickly flash red battery icon to indicate low battery condition
void anim_blk_low_batt() {

    np_clear();
    for (uint8_t u8_i = 0; u8_i < 4; u8_i++)
    {
        draw_char_cent(BATT_ICON_DEAD, COLOR_RED);
        np_show();
        delay_msec(200);

        np_clear();
        np_show();
        delay_msec(200);
    }
}

// Quickly flash a short char sequence, without scrolling. Blocking animation.
void anim_blk_flash_chars(char* msg) {

    np_clear();
    uint8_t i = 0;
    char c;
    while (c = msg[i++]) {

        draw_char_cent(c, COLOR_BLUE);
        np_show();
        delay_msec(300);

        np_clear();
        np_show();
        delay_msec(300);

    }
    
}

// Fill pixels one after another with a color, with a wait in between frames.
void anim_blk_color_wipe(uint32_t u32_color, uint16_t u16_wait) {
    uint8_t u8_numPixels = np_get_length();

    for (uint8_t u8_pixIdx = 0; u8_pixIdx < u8_numPixels; u8_pixIdx++) { // For each pixel ... 
        np_set_pix_color_pack(u8_pixIdx, u32_color);  //  Set pixel's color (in RAM)
        np_show();                               //  Update pixels to match
        delay_msec(u16_wait);                              //  Pause for a moment
    }
}

// Fill pixels with a color, from 0 to count, with a wait in between frames.
void anim_blk_color_count(uint32_t u32_color, uint16_t u16_wait, uint16_t u16_count) {

    for (uint8_t u8_pixIdx = 0; u8_pixIdx < u16_count; u8_pixIdx++) { //  For count # of pixels          
        np_set_pix_color_pack(u8_pixIdx, u32_color); //  Set pixel's color (in RAM)
        np_show();                              //  Update pixels to match
        delay_msec(u16_wait);                             //  Pause for a moment
    }
}

// "Print" a u32 integer to the display, in decimal.
// Flash each decimal digit in sequence. Useful for debugging or printing statistics.
// Blocking animation.
void anim_blk_print_dec_u32(uint32_t u32_val) {

    char ac_str[U32_DEC_STR_MAX_BUFF_SIZE];
    u32_to_dec_string(u32_val, ac_str);
    anim_blk_flash_chars(ac_str);

}
