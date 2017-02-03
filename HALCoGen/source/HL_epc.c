/** @file HL_epc.c
*   @brief EPC Driver Implementation File
*   @date 20.May.2014
*   @version 04.00.00
*	This file contains APIs for the Error Profiling Controller Module.
*/

/* (c) Texas Instruments 2009-2013, All rights reserved. */

/* USER CODE BEGIN (0) */
/* USER CODE END */

#include "HL_epc.h"
#include "HL_sys_vim.h"
#include "HL_system.h"
#include "HL_reg_esm.h"

/* USER CODE BEGIN (1) */
/* USER CODE END */

/** @fn void epcEnableIP1ErrorGen(void)
*   @brief Enable ECC error generation for ECC errors on DMA Port A
*
*   Enable ECC error generation for ECC errors detected on DMA Port A master by the
*	CPU Interconnect Subsystem
*/
void epcEnableIP1ErrorGen(void)
{
	systemREG2->IP1ECCERREN = (systemREG2->IP1ECCERREN & 0xFFFFFFF0U) | 0xAU;

}

/** @fn void epcDisableIP1ErrorGen(void)
*   @brief Disable ECC error generation for ECC errors on DMA Port A
*
*   Disable ECC error generation for ECC errors detected on DMA Port A master by the
*	CPU Interconnect Subsystem
*/
void epcDisableIP1ErrorGen(void)
{
	systemREG2->IP1ECCERREN = (systemREG2->IP1ECCERREN & 0xFFFFFFF0U) | 0x5U;

}

/** @fn void epcEnableIP2ErrorGen(void)
*   @brief Enable ECC error generation for ECC errors on PS_SCR_M
*
*   Enable ECC error generation for ECC errors detected on PS_SCR_M master by the
*	CPU Interconnect Subsystem
*/
void epcEnableIP2ErrorGen(void)
{
	systemREG2->IP1ECCERREN = (systemREG2->IP1ECCERREN & 0xFFFFF0FFU) | 0xA00U;

}

/** @fn void epcDisableIP2ErrorGen(void)
*   @brief Disable ECC error generation for ECC errors on PS_SCR_M
*
*   Disable ECC error generation for ECC errors detected on PS_SCR_M master by the
*	CPU Interconnect Subsystem
*/
void epcDisableIP2ErrorGen(void)
{
	systemREG2->IP1ECCERREN = (systemREG2->IP1ECCERREN & 0xFFFFF0FFU) | 0x500U;

}

/** @fn void epcEnableSERREvent(void)
*   @brief Single (correctable) bit error event enable.
*
*   These bits (when enabled) cause EPC to
*	generate the serr_event if there is a correctable ECC fault address arrives from one of
*	the EPC-IP interface and the CAM has an empty entry.
*/
void epcEnableSERREvent(void)
{
	epcREG1->EPCCNTRL = (epcREG1->EPCCNTRL & 0xFFFFFFF0U) | 0xAU;
}

/** @fn void epcDisableSERREvent(void)
*   @brief Single (correctable) bit error event disable.
*
*   These bits (when enabled) cause EPC to
*	disable the serr_event generation.
*/
void epcDisableSERREvent(void)
{
	epcREG1->EPCCNTRL = (epcREG1->EPCCNTRL & 0xFFFFFFF0U) | 0x5U;
}

/** @fn void epcEnableInterrupt(void)
*   @brief CAM or FIFO full interrupt enable.
*
*   If this bit is set and CAM is full, CAM Full Interrupt
*	is generated.
*/
void epcEnableInterrupt(void)
{
	epcREG1->EPCCNTRL |= (1U<<24U);
}

/** @fn void epcDisableInterrupt(void)
*   @brief CAM or FIFO full interrupt disable.
*
*   Disables interrupt generation in case CAM is full.
*/
void epcDisableInterrupt(void)
{
	epcREG1->EPCCNTRL &= ~(1U<<24U);
}

/** @fn void epcCAMInit(void)
*   @brief Initializes CAM.
*
*  	CAM entries are cleared and available for future CAM usage.
*/
void epcCAMInit(void)
{
	int i;
	for(i=0U; i<8U; i++)
	{
		epcREG1->CAM_INDEX[i] = 0x05050505U;
	}
}

/** @fn void epcDiagnosticTest(void)
*   @brief CAM diagnostic enable.
*
*  	These bits (when enabled) allow the CPU to access the
*	CAM content to clear or set any entry (CAM index) or write any pattern to CAM
*	content.
*/
boolean epcDiagnosticTest(void)
{
	uint32 epccntrl_bk, camCont_bk, camIndex_bk;
	uint32 camAvailable;
	boolean status = true;

	/* Back up EPCCNTRL register */
	epccntrl_bk = epcREG1->EPCCNTRL;

	/* Back up CAM_CONTENT[0] and CAM_INDEX[0] registers */
	camCont_bk = epcREG1->CAM_CONTENT[0];
	camIndex_bk = epcREG1->CAM_INDEX[0];

	/* Enter CAM diagnostic mode and and enable Single (correctable) bit error event generation */
	epcREG1->EPCCNTRL = (epcREG1->EPCCNTRL & 0xFFFFF0F0U) | 0x0A0AU;

	/* Clear first CAM entry */
	epcREG1->CAM_INDEX[0] = (epcREG1->CAM_INDEX[0] & 0xFFFFFFF0) | 0x5;

	/* Identify the number of CAM entries available */
	camAvailable = epcREG1->CAMAVAILSTAT;

	/* New CAM Entry */
	epcREG1->CAM_CONTENT[0] = 0x1000;

	/* The number of CAM entries must reduce by 1 */
	if(((esmREG->SR1[0] & 0x10) == 0x10) && (epcREG1->CAMAVAILSTAT != (camAvailable - 1)) && epcCheckCAMEntry(0))
	{
		status = false;
	}


	/* Restore CAM_CONTENT and CAM_INDEX[0] registers */
	epcREG1->CAM_CONTENT[0] = camCont_bk;
	epcREG1->CAM_INDEX[0] = camIndex_bk;

	/* Disable CAM diagnostic mode and restore EPCCNTRL register */
	epcREG1->EPCCNTRL = epccntrl_bk;

	return status;
}

/** @fn void epcAddCAMEEntry(uint32 address)
*   @brief Add a new CAM Entry
*
*  	Allows you to write a new CAM entry, after checking if there are any available entries.
*/
boolean epcAddCAMEEntry(uint32 address)
{
	int i = 0;
	boolean status = false;

	if(epcREG1->CAMAVAILSTAT !=0)
	{

		for(i = 0;i<32;i++)
		{
			if(epcCheckCAMEntry(i) == true)
			{
				epcREG1->CAM_CONTENT[i] = address;
				status = true;
				break;
			}
		}
	}
	else
	{
		status = false;
	}

	return status;
}

/** @fn void epcCheckCAMEntry(uint32 CAMIndex)
*   @brief Checks if CAM entry is available.
*
*  	Checks if the CAM Entry is available and ready for future usage.
*/
boolean epcCheckCAMEntry(uint32 CAMIndex)
{
	/* Refer to the CAM Index Register and Bit group respectively */
	int i,j;

	boolean status = false;

	i = CAMIndex/4;
	j = (CAMIndex%4)*8;

	/* Check for availability of CAM Entry for future CAM usage. */
	if((epcREG1->CAM_INDEX[i] & (0xFU<<j)) == (0x5U<<j))
	{
		status = true;
	}
	else
	{
		status= false;
	}

	return status;

}

