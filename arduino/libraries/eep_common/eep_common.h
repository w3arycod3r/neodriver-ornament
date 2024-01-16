/* File:      eep_common.h
 * Author:    Garrett Carter
 * Purpose:   Common EEPROM def's for eep_data_write and neo_driver, so they don't get out of sync
 */

/********************************** DEFINES **********************************/

// EEPROM Addresses
#define EEP_SETT_ANIM   0
#define EEP_SETT_NPIX   1
#define EEP_SETT_RSEED  2   // Addr 2-5, 4 bytes
#define EEP_SETT_FLAGS  6   // 8 Status Flags
#define EEP_CHAR_START  7


// EEPROM Sizes
#define EEP_CHAR_NUM_B  425
#define EEP_COV_START   432
#define EEP_COV_NUM_B   80
