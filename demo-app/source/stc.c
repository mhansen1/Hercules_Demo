/** @file stc.c 
*   @brief STC Driver Implmentation File
*   @date 14.September.2009
*   @version 1.00.000
*
*   (c) Texas Instruments 2009, All rights reserved.
*/

#include "stc.h"
#include "HL_system.h"
#include "HL_sci.h"
#include "sl_api.h"
#include "app_sys_core.h"
#include "HL_sys_core.h"


extern void sciSend_32bitdata(sciBASE_t *sci, unsigned int data);
extern void _coreBackupStackPointer_(void);
extern void _coreRestoreStackPointer_(void);
/* indicates that a fault was just injected. */
uint32    myFaultInjected = 0x00000000;
/* data storage to keep the register set */
uint32_t            myCoreRegisterSet[30];

void stcStartSelfTest(void)
{
    SL_STC_Config               stcSelfTestConfig;  /* STC Configuration */
    SL_STC_FailInfo stcFailInfo;
    myFaultInjected = 0x12345678;
    /*cpuSelfTest(STC_INTERVAL, STC_MAX_TIMEOUT, TRUE);*/
    stcSelfTestConfig.stcClockDiv = 2; /* STC Clock divider = 1 */
    stcSelfTestConfig.intervalCount = 1; /* One interval only */
    stcSelfTestConfig.restartInterval0 = TRUE; /* Start from interval 0 */
    stcSelfTestConfig.timeoutCounter = 0xFFFFFFFF; /* Timeout counter*/
    /* save the core register set */
    _CoreRegisterSetSave_(&myCoreRegisterSet[0]);
    /* save the stacks of tha banked registers */
    _CoreBankRegisterSetSave_(&myCoreRegisterSet[16]);
    SL_SelfTest_STC(STC1_RUN, TRUE, &stcSelfTestConfig);
    _coreEnableVfp_();
    _coreEnableEventBusExport_();
    _coreEnableIrqVicOffset_();
    _enable_interrupt_();
    SL_SelfTest_Status_STC(&stcFailInfo);
    if (stcFailInfo.stResult == ST_PASS)
    {	//return TRUE;
        sciSend_32bitdata(sciREG1, 1);
    }
    else
    {
        //return FALSE;
        sciSend_32bitdata(sciREG1, 0);
    }
    /* restore stack pointers */
    _CoreBankRegisterSetRestore_(&myCoreRegisterSet[16]);
    myFaultInjected = 0x00000000;
}


