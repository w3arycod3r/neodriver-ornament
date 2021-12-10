/* File:      ard_utility.h
 * Author:    Garrett Carter
 * Purpose:   Arduino utility functions
 */

#ifndef ARD_UTILITY_H
#define ARD_UTILITY_H

// Defines
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

#endif