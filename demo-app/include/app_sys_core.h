/** @file sys_core.h
*   @brief System Core Header File
*   @date 15.Mar.2012
*   @version 03.01.00
*   
*   This file contains:
*   - Core Interface Functions
*   .
*   which are relevant for the System driver.
*/

/* (c) Texas Instruments 2009-2012, All rights reserved. */

#ifndef __APP_SYS_CORE_H__
#define __APP_SYS_CORE_H__

#include "HL_sys_common.h"



/** @fn _CoreRegisterSetSave_(uint32_t* aRegisterSet)
*   @brief Store the Core register set in an array
*
*   This function saves the core registers in the array given in Parameter 0.
*/
void _CoreRegisterSetSave_(uint32* aRegisterSet);

/** @fn _CoreRegisterSetRestore_(uint32_t* aRegisterSet)
*   @brief reStore the Core register previously stored
*
*   This function restores the core registers out of the array given in Parameter 0.
*/
void _CoreRegisterSetRestore_(uint32* aRegisterSet);

void _CoreBankRegisterSetSave_(uint32* aRegisterSet);
void _CoreBankRegisterSetRestore_(uint32* aRegisterSet);


/** @fn uint32_t _mpuGetRegionType_(void)
*   @brief Returns the type of currently selected mpu region
*   @return Current type of selected mpu region
*
*   This function returns the type of currently selected mpu region.
*/
uint32_t _mpuGetRegionType_(void);

/** @fn uint32_t _mpuGetRegionPermission_(void)
*   @brief Returns permission of currently selected mpu region
*   @return Current type of selected mpu region
*
*   This function returns permission of currently selected mpu region.
*/
uint32_t _mpuGetRegionPermission_(void);

/** @fn void _mpuSetRegionSizeRegister_(uint32_t value)
*   @brief Set mpu region size register value
*
*   This function sets mpu region size register value.
*/
void _mpuSetRegionSizeRegister_(uint32_t value);

/** @fn void _mpuSetRegionSizeRegister_(uint32_t value)
*   @brief Set mpu region size register value
*
*   This function sets mpu region size register value.
*/
uint32_t _mpuGetRegionSizeRegister_();


#endif
