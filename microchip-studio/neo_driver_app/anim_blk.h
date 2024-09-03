/* File:      anim_blk.h
 * Author:    Garrett Carter
 * Purpose:   Animation functions, blocking
 */

#ifndef ANIM_BLK_H
#define ANIM_BLK_H

#include <stdint.h>

// Allow compilation with C++ compiler
#ifdef __cplusplus
extern "C"{
#endif

/********************************** DEFINES **********************************/

/********************************** PROTOTYPES **********************************/
// Blocking animations
void anim_blk_voltage(uint16_t u16_mv);
void anim_blk_low_batt();
void anim_blk_flash_chars(char* msg);
void anim_blk_color_wipe(uint32_t u32_color, uint16_t u16_wait);
void anim_blk_color_count(uint32_t u32_color, uint16_t u16_wait, uint16_t u16_count);
void anim_blk_print_dec_u32(uint32_t u32_val);


#ifdef __cplusplus
} // extern "C"
#endif

#endif /* ANIM_BLK_H */