/* File:      ard_utility.h
 * Author:    Garrett Carter
 * Purpose:   Arduino utility functions
 */

#ifndef ARD_UTILITY_H
#define ARD_UTILITY_H

#include <stdint.h>

/********************************** DEFINES **********************************/
/* definition to expand macro then apply to pragma message */
// Usage example below
// #pragma message(VAR_NAME_VALUE(NOT_DEFINED))

#define VALUE_TO_STRING(x) #x
#define VALUE(x) VALUE_TO_STRING(x)
#define VAR_NAME_VALUE(var) #var "="  VALUE(var)

#define bitSetMask(reg, bitMask) ((reg) |= (bitMask))
#define bitClearMask(reg, bitMask) ((reg) &= ~(bitMask))
#define bitReadMask(reg, bitMask) ((reg) & (bitMask))

#define BIT0 (1UL << 0)
#define BIT1 (1UL << 1)
#define BIT2 (1UL << 2)
#define BIT3 (1UL << 3)
#define BIT4 (1UL << 4)
#define BIT5 (1UL << 5)
#define BIT6 (1UL << 6)
#define BIT7 (1UL << 7)

#define BITS_IN_BYTE 8

// Perform ceil(x/y) in integer math
#define DIV_CEILING(x,y) (((x) + (y) - 1) / (y))

#define U32_DEC_MAX_DIGITS 10
#define U32_DEC_STR_MAX_BUFF_SIZE (U32_DEC_MAX_DIGITS+1)

/********************************** PROTOTYPES **********************************/
void u32_to_dec_string(uint32_t u32_val, char* ac_str);
uint8_t strlen(char* ac_str);
void rev_string(char* ac_str);

#endif