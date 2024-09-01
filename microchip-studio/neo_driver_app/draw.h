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

/********************************** PROTOTYPES **********************************/
// Drawing functions
void draw_char(char c_char, uint32_t u32_color, int8_t i8_x, int8_t i8_y);
void draw_char_cntr(char c_char, uint32_t u32_color);
void draw_value(uint32_t u32_val, uint32_t u32_maxVal);
void draw_value_binary(uint32_t u32_val);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* DRAW_H */