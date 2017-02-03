#include "esm_demo.h"
#include "sl_api.h"

/* External Variables */
extern unsigned int SubTask_Number;

/* External Functions */
/* Global Variables */
unsigned int ESM_High_Int_Flag = 0;

extern void sciSend_32bitdata(sciBASE_t *sci, unsigned int data);

extern unsigned int SubTask_Number;
extern unsigned int Task_Number;

/** @enum testStatus
 *   @brief pass or fail status for the test performed
 *   This enumeration is used to provide alias names for the test pass/fail status:
 *     - PASS
 *     - Fail
 */

enum testStatus
{
    FAIL = 0x00000000, /**< Alias for FAIL */
    PASS = 0x00000001 /**< Alias for PASS */
};

/** @fn void esm_test(void)
 *   
 *   This function is called when a ESM test command is received. 
 * 	 In ESM, required channel test is selected based on the sub task   
 *  
 */
void esm_test(void)
{
    SL_CCMR5F_FailInfo failInfoCCMR5F; /* CCMR5 Self Test fail info */
    esmClearErrPin();

    switch (SubTask_Number)
    {
        case 0:
            break;
        case 1:
            /** - DCAN 1 ECC error Test */
            SL_SelfTest_CAN(CAN_ECC_TEST_MODE_2BIT_FAULT_INJECT, SL_DCAN1);

            /** - Check for corresponding ESM channel error */
            break;
        case 2:
            /** - DCAN 2 ECC error Test */
            SL_SelfTest_CAN(CAN_ECC_TEST_MODE_2BIT_FAULT_INJECT, SL_DCAN2);

            /** - Check for corresponding ESM channel error */
            break;
        case 3:
            /** - DCAN 4 Parity error Test */
            SL_SelfTest_CAN(CAN_ECC_TEST_MODE_2BIT_FAULT_INJECT, SL_DCAN3);

            /** - Check for corresponding ESM channel error */
            break;
        case 4:
            /** - HTU Parity error Test */
            /** - Check for corresponding ESM channel error */
            break;
        case 5:
            /** - MIBSPI 1 ECC error Test */
            SL_SelfTest_MibSPI(MIBSPI_ECC_TEST_MODE_2BIT_FAULT_INJECT, SL_MIBSPI1);

            /** - Check for corresponding ESM channel error */
            break;
        case 6:
            /** - MIBSPI 3 Parity error Test */
            SL_SelfTest_MibSPI(MIBSPI_ECC_TEST_MODE_2BIT_FAULT_INJECT, SL_MIBSPI3);

            /** - Check for corresponding ESM channel error */
            break;
        case 7:
            /** - MIBSPI 5 Parity error Test */
            SL_SelfTest_MibSPI(MIBSPI_ECC_TEST_MODE_2BIT_FAULT_INJECT, SL_MIBSPI5);

            /** - Check for corresponding ESM channel error */
            break;
        case 8:
            /** - MIBADC 1 Parity error Test */
            /** - Check for corresponding ESM channel error */
            break;
        case 9:
            /** - MIBADC 2 Parity error Test */
            /** - Check for corresponding ESM channel error */
            break;
        case 10:
            /** - NHET Memory Parity error Test */
            /** - Check for corresponding ESM channel error */
            break;
        case 11:
            /** - VIM RAM Parity error Test */
            /** - Check for corresponding ESM channel error */
            break;
        case 12:
            /** - Flexray Message RAM Parity error Test */
            /** - Check for corresponding ESM channel error */
            break;
        case 13:
            /** - FTU Parity error Test */
            /** - Check for corresponding ESM channel error */
            break;
        case 14:
            /** - Core Compare Module Compare fail error Test */
            SL_SelfTest_CCMR5F(CCMR5F_CPUCOMP_ERROR_FORCING_TEST_FAULT_INJECT,
                    TRUE, &failInfoCCMR5F);
            /** - Check for corresponding ESM channel error */
            break;
        case 15:
            /** - DMA Parity error Test */
            /** - Check for corresponding ESM channel error */
            break;
        case 16:
            /** - DMA MPU Violation error Test */
            /** - Check for corresponding ESM channel error */
            break;
        case 17:
            /** - FTU MPU Violation error Test */
            /** - Check for corresponding ESM channel error */
            break;
        case 18:
            /** - HTU MPU Violation error Test */
            /** - Check for corresponding ESM channel error */
            break;
        case 19:
            /** - Flash Single Bit Error detection Test */
            /** - Check for corresponding ESM channel error */
            break;
        case 20:
            SL_SelfTest_CCMR5F(CCMR5F_VIMCOMP_ERROR_FORCING_TEST_FAULT_INJECT,
                    TRUE, &failInfoCCMR5F);
            break;
        case 21:
            SL_SelfTest_CCMR5F(CCMR5F_PDCOMP_ERROR_FORCING_TEST_FAULT_INJECT,
                    TRUE, &failInfoCCMR5F);
            break;
        case 22:
            SL_SelfTest_CCMR5F(CCMR5F_INMCOMP_ERROR_FORCING_TEST_FAULT_INJECT,
                    TRUE, &failInfoCCMR5F);
        case 23:
            SL_SelfTest_CAN(CAN_ECC_TEST_MODE_2BIT_FAULT_INJECT, SL_DCAN4);
            break;
        case 24:
            SL_SelfTest_MibSPI(MIBSPI_ECC_TEST_MODE_2BIT_FAULT_INJECT, SL_MIBSPI2);
            break;
        case 25:
            SL_SelfTest_MibSPI(MIBSPI_ECC_TEST_MODE_2BIT_FAULT_INJECT, SL_MIBSPI4);
            break;
    }

}
/** @fn void esmClearErrPin(void)
 *   @brief function clears the ESM error pin
 */
void esmClearErrPin(void)
{
    /** - clear only when the error PIN are set	  	*/
    if (!esmREG ->EPSR)

        /** - clear the error using Error Key Register  */
        esmREG ->EKR = 0x5;

    /** - Wait till the error pin is reset		 	*/
    while (!esmREG ->EPSR)
        ;

    sciSend_32bitdata(sciREG1, 1U);
}

#if 0
/** @fn esmCheckstatus(unsigned int channel)
 *  @breif Check ESM channel status 
 *   
 */
void esmCheckstatus(unsigned int channel)
{

    if(esmREG->ESTATUS1[0] == (1 << channel))
    {
        /** - Clear the corresponding channel Group 1 ESM  error */
        esmREG->ESTATUS1[0] = (1 << channel);

        sciSend_32bitdata(sciREG1, PASS);
    }
    else if(esmREG->ESTATUS1[1] == (1 << channel)) // For CCMR4 compare
    {
        /** - Clear the corresponding channel Group 2 ESM  error */
        esmREG->ESTATUS1[1] = (1 << channel);

        sciSend_32bitdata(sciREG1, PASS);
    }
    else
    {
        sciSend_32bitdata(sciREG1, FAIL);
    }

}
#endif

