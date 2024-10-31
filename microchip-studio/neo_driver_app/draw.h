/* File:      draw.h
 * Author:    Garrett Carter
 * Purpose:   Drawing functions
 */

#ifndef DRAW_H
#define DRAW_H

#include <stdint.h>

// Allow compilation with C++ compiler
#ifdef __cplusplus
extern "C"{
#endif

/********************************** DEFINES **********************************/

/*
 *   ANIM_ROT_SEL - Rotation select for 5x5 Matrix - Increasing #, Turning clockwise
 *   Used by the draw_char() function to correctly orient the characters.
 *
 *   Value |  Input wires located at:
 *   ================================
 *   1     |  BOTTOM
 *   2     |  LEFT
 *   3     |  TOP
 *   4     |  RIGHT
 */
#define ANIM_ROT_SEL 3

#define BATT_ICON_LVL_1     '#'
#define BATT_ICON_LVL_2     '$'
#define BATT_ICON_LVL_3     '%'
#define BATT_ICON_LVL_4     '&'
#define BATT_ICON_LVL_5     '\''
#define BATT_ICON_DEAD      '('

// 32-bit packed colors
#define COLOR_TEAL   0x00FFFFUL
#define COLOR_YELLOW 0xFFFF00UL
#define COLOR_WHITE  0xFFFFFFUL
#define COLOR_PURPLE 0xFF00FFUL
#define COLOR_RED    0xFF0000UL
#define COLOR_GREEN  0x00FF00UL
#define COLOR_BLUE   0x0000FFUL

#define COLOR_WHEEL  0x000000UL // Dynamic color wheel effect

/********************************** PROTOTYPES **********************************/
// Drawing functions
void draw_char(char c_char, uint32_t u32_color, int8_t i8_x, int8_t i8_y);
void draw_char_cent(char c_char, uint32_t u32_color);
void draw_value(uint32_t u32_val, uint32_t u32_maxVal);
void draw_value_binary(uint32_t u32_val);

// Drawing "support" functions
uint8_t read_cov_base(uint16_t u16_baseNum);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* DRAW_H */