/** @file pbist.c 
*   @brief PBIST Driver Inmplmentation File
*   @date 14.September.2009
*   @version 1.00.000
*
*   (c) Texas Instruments 2009, All rights reserved.
*/

#include "pbist.h"
#include "HL_sci.h"
#include "HL_system.h"
#include "sl_api.h"

/* following is the table of the RAM groups required for pBIST */
uint64 all2portmemories[] = {
        PBIST_RAMGROUP_36_CPGMAC_CPPI,
        PBIST_RAMGROUP_10_MIBSPI1,
        PBIST_RAMGROUP_11_MIBSPI2,
        PBIST_RAMGROUP_12_MIBSPI3,
        PBIST_RAMGROUP_22_MIBSPI4,
        PBIST_RAMGROUP_23_MIBSPI5,
        PBIST_RAMGROUP_06_DCAN1,
        PBIST_RAMGROUP_07_DCAN2,
        PBIST_RAMGROUP_19_DCAN3,
        PBIST_RAMGROUP_20_DCAN4,
        PBIST_RAMGROUP_26_FRAY_INBUF_OUTBUF,
        PBIST_RAMGROUP_13_NHET1,
        PBIST_RAMGROUP_24_NHET2,
        PBIST_RAMGROUP_08_DMA,
        PBIST_RAMGROUP_09_HTU1,
        PBIST_RAMGROUP_21_HTU2,
        };

extern unsigned int SubTask_Number;
extern void sciSend_32bitdata(sciBASE_t *sci, unsigned int data);

/** @fn void pbist_test(void)
*   @brief Function used to perform PBIST self test as per selected Sub Task.
*/
void pbist_test(void)
{
    boolean retVal;
    SL_PBIST_FailInfo failInfoPBISTOthers;/* PBIST Failure information for non-TCM memories */
    retVal = SL_SelfTest_PBIST(PBIST_EXECUTE, all2portmemories[SubTask_Number],
            PBISTALGO_MARCH13N_2PORT);
    while (TRUE != SL_SelfTest_Status_PBIST(&failInfoPBISTOthers))
        ;
    if (failInfoPBISTOthers.stResult == ST_PASS)
    {
        //return TRUE;
        sciSend_32bitdata(sciREG1, 1);

    }
    else
    {
        //  return FALSE;
        sciSend_32bitdata(sciREG1, 0);
    }
    SL_SelfTest_PBIST_StopExec();
}	

