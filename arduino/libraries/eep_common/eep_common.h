/* File:      eep_common.h
 * Author:    Garrett Carter
 * Purpose:   Common EEPROM def's for eep_data_write and neo_driver, so they don't get out of sync
 */

#ifndef EEP_COMMON_H
#define EEP_COMMON_H

// This lib depends on another
#include <ard_utility.h>

/********************************** DEFINES **********************************/

// EEPROM Addresses
#define EEP_SETT_ANIM   0
#define EEP_SETT_NPIX   1
#define EEP_SETT_RSEED  2   // Addr 2-5, 4 bytes
#define EEP_SETT_FLAGS  6   // 8 Status Flags

// Status Flags (bits in EEP_SETT_FLAGS)
#define STAT_FLG_BATT_DEAD  BIT0


// EEPROM Ranges & Sizes
#define EEP_SIZE_BYTES 512

#define EEP_CHAR_DATA_START_ADDR 7
#define EEP_CHAR_DATA_NUM_CHARS  7
#define EEP_CHAR_DATA_NUM_BYTES  425

// Comment out below line to skip writing the cov virus data, if you don't intend to use the animation
// #define EEP_COV_DATA_WRITE_ENABLE
#define EEP_COV_DATA_START_ADDR   432
// 160 Bytes of sequence are available
#define EEP_COV_DATA_NUM_BYTES   80

#endif