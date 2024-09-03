/* File:      utility.h
 * Author:    Garrett Carter
 * Purpose:   Utility functions
 */

#ifndef UTILITY_H
#define UTILITY_H

#include <stdint.h>

// Allow compilation with C++ compiler
#ifdef __cplusplus
extern "C"{
#endif

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

// Set or clear the bitMask in reg and return the new value
#define bitSetMaskRet(reg, bitMask) ((reg) | (bitMask))
#define bitClearMaskRet(reg, bitMask) ((reg) & ~(bitMask))

// Set or clear the bit in reg and return the new value
#define bitSetRet(reg, b) ((reg) | (bit(b)))
#define bitClearRet(reg, b) ((reg) & ~(bit(b)))

#define BIT0 (1UL << 0)
#define BIT1 (1UL << 1)
#define BIT2 (1UL << 2)
#define BIT3 (1UL << 3)
#define BIT4 (1UL << 4)
#define BIT5 (1UL << 5)
#define BIT6 (1UL << 6)
#define BIT7 (1UL << 7)
#define BIT8 (1UL << 8)
#define BIT9 (1UL << 9)
#define BIT10 (1UL << 10)
#define BIT11 (1UL << 11)
#define BIT12 (1UL << 12)
#define BIT13 (1UL << 13)
#define BIT14 (1UL << 14)
#define BIT15 (1UL << 15)
#define BIT16 (1UL << 16)
#define BIT17 (1UL << 17)
#define BIT18 (1UL << 18)
#define BIT19 (1UL << 19)
#define BIT20 (1UL << 20)
#define BIT21 (1UL << 21)
#define BIT22 (1UL << 22)
#define BIT23 (1UL << 23)
#define BIT24 (1UL << 24)
#define BIT25 (1UL << 25)
#define BIT26 (1UL << 26)
#define BIT27 (1UL << 27)
#define BIT28 (1UL << 28)
#define BIT29 (1UL << 29)
#define BIT30 (1UL << 30)
#define BIT31 (1UL << 31)


#define BITS_IN_BYTE 8

// Perform ceil(x/y) in integer math
#define DIV_CEILING(x,y) (((x) + (y) - 1) / (y))

#define U32_DEC_MAX_DIGITS 10
#define U32_DEC_STR_MAX_BUFF_SIZE (U32_DEC_MAX_DIGITS+1)

// Battery level thresholds
#define BATT_LVL_THRESH_1_MV  (2750)
#define BATT_LVL_THRESH_2_MV  (3000)
#define BATT_LVL_THRESH_3_MV  (3100)
#define BATT_LVL_THRESH_4_MV  (3200)

/********************************** PROTOTYPES **********************************/
void u32_to_dec_string(uint32_t u32_val, char* ac_str);
uint8_t strlen(char* ac_str);
void rev_string(char* ac_str);
void memset(uint8_t*, uint8_t, uint8_t);
void soft_reset();
void inc_sat_eep_cntr_u16(uint16_t* eep_addr);
uint8_t extract_digit(uint16_t u16_val, uint8_t u8_dig);
uint16_t read_vcc_mv();
uint8_t get_batt_level();
void shift_in_from_right(uint8_t* data, const uint8_t data_in, const uint8_t num_bits);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* UTILITY_H */