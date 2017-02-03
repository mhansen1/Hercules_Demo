/** @file HL_sys_main.c 
*   @brief Application main file
*   @date 20.May.2014
*   @version 04.00.00
*
*   This file contains an empty main function,
*   which can be used for the application.
*/

/* (c) Texas Instruments 2009-2013, All rights reserved. */

/* USER CODE BEGIN (0) */
/* USER CODE END */

/* Include Files */

#include "HL_sys_common.h"
#include "HL_system.h"

/* USER CODE BEGIN (1) */
#include "HL_sci.h"
#include "HL_adc.h"
#include "HL_het.h"
#include "HL_can.h"
#include "HL_mibspi.h"
#include "HL_sci.h"
#include "HL_rti.h"
#include "stc.h"
#include "esm_demo.h"
#include "swi_util.h"
#include "sl_config.h"
#include "esm_application_callback.h"
#include "sl_api.h"



unsigned int GlobalIP;
unsigned char receive_command[12];
unsigned int Task_Number;
unsigned int SubTask_Number;
unsigned int task_data;
unsigned int acyl_zero[3];
double acyl_zeroV[3];



#pragma DATA_ALIGN (StackModeSVC, 4)
volatile uint32 StackModeSVC[STACK_SIZE_MODE_SVC];

#pragma DATA_ALIGN (StackModeFIQ, 4)
volatile uint32 StackModeFIQ[STACK_SIZE_MODE_FIQ];

#pragma DATA_ALIGN (StackModeIRQ, 4)
volatile uint32 StackModeIRQ[STACK_SIZE_MODE_IRQ];

#pragma DATA_ALIGN (StackModeABT, 4)
volatile uint32 StackModeABT[STACK_SIZE_MODE_ABT];

#pragma DATA_ALIGN (StackModeUND, 4)
volatile uint32 StackModeUND[STACK_SIZE_MODE_UND];

#pragma DATA_ALIGN (StackModeSYS, 4)
volatile uint32 StackModeSYS[STACK_SIZE_MODE_SYS];


extern void run_LED_StartUp(void);
extern void Running_LED_demo(void);
extern void Running_Light_Balance(void);
extern void Digital_Level(void);
extern void Maze_Game(void);
extern void get_software_Version(void);
extern void get_hardware_Info(void);
extern void pbist_test(void);
extern void Ambient_Light_Sensor_demo(void);
extern void Temp_Sensor_demo(void);
extern void SDC_alert(int input);
extern void CAN_Demo();
extern void sciSend_32bitdata(sciBASE_t *sci, unsigned int data);
extern void adcStartConversion_selChn(adcBASE_t *adc, unsigned channel, unsigned fifo_size, unsigned group);
extern void adcGetSingleData(adcBASE_t *adc, unsigned group, adcData_t *data);
extern void EMAC_Init (uint8_t * macAddress);
extern void AutiInitMemories(void);
void EMAC_Webserver_demo();
void delay(void) {
	  static volatile unsigned int delayval;
	  delayval = 10000;   // 100000 are about 10ms
	  while(delayval--);
}
void small_delay(void) {
	  static volatile unsigned int delayval;
	  delayval = 10;
	  while(delayval--);
}
/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
/* USER CODE END */

void main(void)
{
/* USER CODE BEGIN (3) */
	run_LED_StartUp();
	swi_enable_fiq_irq();
	sciInit();
	sciReceive(sciREG1, 12, receive_command);
	hetInit();
	adcInit();
	gioInit();
	esmInit();
	esmClearErrPin();
	canInit();
	mibspiInit();
	AutiInitMemories();
    vimInit();
    SL_ESM_Init(ESM_ApplicationCallback);
	hetREG1->DIR   	= 0xAA178035;
	hetREG1->DOUT   	= 0x08110034;
	pwmStop(hetRAM1,pwm0);
	pwmStop(hetRAM1,pwm1);
	pwmStop(hetRAM1,pwm2);
	pwmStop(hetRAM1,pwm3);
	pwmStop(hetRAM1,pwm4);
	pwmStop(hetRAM1,pwm5);
	pwmStop(hetRAM1,pwm6);
	pwmStop(hetRAM1,pwm7);
	Task_Number = 0;
	SubTask_Number = 0;
	while(1)
	{
		switch(Task_Number)
		{
			case 1:
				/** Get Software Version */
				get_software_Version();
				
				/** Reset the Task Number */
				Task_Number = 0;
				break;
			case 2:
				/** Get Hardware Informations */
				get_hardware_Info();
				
				/** Reset the Task Number */  				
				Task_Number = 0;
				break;
			case 3:
				/** Run Pbist Test */
			    pbist_test();
			    AutiInitMemories();
		        vimInit();
		        SL_ESM_Init(ESM_ApplicationCallback);
				/** Reset the Task Number */  				
				Task_Number = 0;
				break;
			case 4:
				/** Run STC Test */
			    stcStartSelfTest();
				/** Enable IRQ and FIQ interrupt in Cortex R4 
				*   After STC test all Core registers have to be reinitialized */
				
				/** Reset the Task Number */  				
				Task_Number = 0;
				break;
			case 5:
				/** Run ESM Test */
				esm_test();
				
				/** Reset the Task Number */  				
				Task_Number = 0;
				break;
			case 6:
				/** Clear ESM pin if set */
				esmClearErrPin();
				
				/** Respond Back to PC with 0x00000001 */
				sciSend_32bitdata(sciREG1, 1);
				
				/** Reset the Task Number */
				Task_Number = 0;
				break;
			case 7:
				/** Perform Temprature Sensor Demo */
				Temp_Sensor_demo();
				
				/** Reset the Task Number */  				
				Task_Number = 0;
				break;
			case 8:
				/** Perform Ambient Light Sensor Demo */
				Ambient_Light_Sensor_demo();
				
				/** Reset the Task Number */  				
				Task_Number = 0;
				break;
			case 9:
				/** Perform LED Demo */
				Running_LED_demo();
				
				/** Respond Back to PC with 0x00000001 */
				sciSend_32bitdata(sciREG1, 1);
				
				/** Reset the Task Number */  				
				Task_Number = 0;
				break;
			case 10:
				/** Test SD Card */
				//SDC_alert(SDC_init());
				
				/** Reset the Task Number */
				Task_Number = 0;
				break;
			case 11:
                /* Run Loopback test and report result */
				/** Reset the Task Number */
				Task_Number = 0;
				break;
			case 12:
                /* Run OHCI and W2FC communication test and report result */
/*				if (!OHCI_W2FC_LoopBack())
					sciSend_32bitdata(scilinREG, 1U);
				else
					sciSend_32bitdata(scilinREG, 0U);
*/
				/** Reset the Task Number */
				Task_Number = 0;
				break;
			case 13:
  				/** Perform DCAN1, and DCAN2 external loopback Test */
                //canExtLoopTest(); //located in can12_loopback.c
  							
				/** Reset the Task Number */
				Task_Number = 0;
				break;
		/** - Configure EMIF for Sync and Async test */
		    case 14:
				//EMIF_Test();

				/** Reset the Task Number */
				Task_Number = 0;
				break;
		/** - Test main connectors */
		    case 15:
/*                if (!ExpConn_Test())
 					sciSend_32bitdata(scilinREG, 1U);
				else
					sciSend_32bitdata(scilinREG, 0U);
*/
 				/** Reset the Task Number */
				Task_Number = 0;
				break;
			case 16:
				Running_Light_Balance();
				break;
			case 17:
				CAN_Demo();
				Task_Number = 0;
				break;
			case 18:
				Digital_Level();
				Task_Number = 0;
				break;
			case 19:
				Maze_Game();
				Task_Number = 0;
				break;
		}
	}
/* USER CODE END */
}

/* USER CODE BEGIN (4) */
/* USER CODE END */
