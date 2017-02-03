/** @file led_demo.c 
*   @brief led demo implementation File
*   @date 11.August.2009
*   @version 1.00.000
*
*   (c) Texas Instruments 2009, All rights reserved.
*/

#include "HL_het.h"
#include "HL_gio.h"
#include "HL_sci.h"
#include "HL_adc.h"

extern unsigned int 	SubTask_Number;
extern unsigned int 	Task_Number;
extern unsigned int 	task_data;

extern void sciSend_32bitdata(sciBASE_t *sci, unsigned int data);
extern unsigned int Get_Ambient_Light_Data(void);
uint32_t map_led_pins[12] = {0x02000000,0x00040000,0x20000000,0x00000001,0x80000000,
	                         0x00020000,0x00000010,0x08000000,0x00010000,0x00000004,
	                         0x00000020,0x00100000};
uint32_t map_led_pins_reve[12] = {0x02000000,0x00040000,0x20000000,0x00000001,0x80000000,
	                         0x00020000,0x08000000,0x00000020,0x00010000,0x00000004,
	                         0x00000020,0x00100000};
uint32_t map_leds_pwms[6] = {3,4,5,0,1,2};
uint32_t map_leds_pwms_reve[8] = {3,4,5,0,1,2,6,7};
	 

/** @fn void Running_LED_demo(void)
*   
*   This function is used for LED demo based on the SCI
*   command different LED's are turned on and a running 
*	LED demo is included
*/
void Running_LED_demo(void)
{
	unsigned int temp;		/** - Temporary Variable */
	unsigned int pwm;		/** - Variable to count the width */
	unsigned int count,i;		/** - Vaiable to toggle the pulse width*/

    /** - NHET is configured as Master
     *  - NHET is turned On */
   	hetREG1->GCR     	= 0x01000001;

	/** - PULL functinality is enabled */
	hetREG1->PULDIS	= 0x00000000;

	/** - This if statment is a placeholder for ArgoBoard/USBStick check */
	if(1)
	{
   		/** - Configure NHET[0,2,4,5,25,16,17,18,20,27,29,31] pins as output
   		 *  - These pins are connected to the LED's */
   		hetREG1->DIR   	= 0xAA178035;

		switch(SubTask_Number)
		{

			case 0:
					/** - Take all LEDs low */
					hetREG1->DOUT   	= 0x08110034; //0x0
					break;
			case 1:
					pwm = 0;
					count = 0;
					/** - Do the LED Demo Toggle, until another task selected
					 *  - The below loop generates a varying pulse width at
					 *    HET pins[0,2,4,5,15,16,17,18,20,27,29,31]
					 * 	  which helps in blinking LED demo */

					while((Task_Number == 9) && (SubTask_Number ==1))
					{

						/** - Set NHET Pins 29,31,0,17,18,25
						 * 	- Set NHET Pins  20,5,2,16,27,4
						 *  - This turns the white LEDs on and the RGB LEDs off*/
				hetREG1->DOUT   	= 0xAA170035;

						for(temp=0;temp<pwm;temp++);

						if((pwm == 0x800))
						{
							for(temp=0;temp<0x100000;temp++);
						}
						/** - Clr all NHETs (turns on RGB LEDs) */
						hetREG1->DOUT   	= 0x00000000;

						for(temp=0;temp<(0x800 - pwm);temp++);

						if((pwm == 0x00))
						{
							for(temp=0;temp<0x100000;temp++);
						}

						if(pwm == 0x800)
						{
							count = 1;

						}
						if(pwm == 0)
						{
							count = 0;
						}

						if(count != 0)
						{
							pwm--;
						}
						else
						{
							pwm++;
						}
					}

					/** - Clear HET pins end of the demo */
					hetREG1->DOUT   	= 0; //0x08110034;
					break;


			case 2:
					/** - Set NHET Pin 25 */
					hetREG1->DSET   	= 0x02000000;
					break;
			case 3:
					/** - Clear NHET Pin 25 */
					hetREG1->DCLR   	= 0x02000000;
					break;
			case 4:
					/** - Set NHET Pin 18 */
					hetREG1->DSET   	= 0x00040000;
					break;
			case 5:
					/** - Clear NHET Pin 18 */
					hetREG1->DCLR   	= 0x00040000;
					break;
			case 6:
					/** - Set NHET Pin 29 */
					hetREG1->DSET   	= 0x20000000;
					break;
			case 7:
					/** - Clear NHET Pin 29 */
					hetREG1->DCLR   	= 0x20000000;
					break;


			case 8:
					/** - Set NHET Pin 0 */
					hetREG1->DSET   	= 0x00000001;
					break;
			case 9:
					/** - Clear NHET Pin 0 */
					hetREG1->DCLR   	= 0x00000001;
					break;
			case 10:
					/** - Set NHET Pin 31 */
					hetREG1->DSET   	= 0x80000000;
					break;
			case 11:
					/** - Clear NHET Pin 31 */
					hetREG1->DCLR   	= 0x80000000;
					break;
			case 12:
					/** - Set NHET Pin 17 */
					hetREG1->DSET   	= 0x00020000;
					break;
			case 13:
					/** - Clear NHET Pin 17 */
					hetREG1->DCLR   	= 0x00020000;
					break;
			case 14:
					/** - Set NHET Pin 2 */
					hetREG1->DCLR   	= 0x00000004;
					break;
			case 15:
					/** - Clear NHET Pin 2 */
					hetREG1->DSET   	= 0x00000004;
					break;
			case 16:
					/** - Set NHET Pin 20 */
					hetREG1->DCLR   	= 0x00100000;
					break;
			case 17:
					/** - Clear NHET Pin 20 */
					hetREG1->DSET   	= 0x00100000;
					break;
			case 18:
					/** - Set NHET Pin 5 */
					hetREG1->DCLR   	= 0x00000020;
					break;
			case 19:
					/** - Clear NHET Pin 5 */
					hetREG1->DSET   	= 0x00000020;
					break;
			case 20:
					/** - Set NHET Pin 4 */
					hetREG1->DCLR   	= 0x00000010;
					break;
			case 21:
					/** - Clear NHET Pin 4 */
					hetREG1->DSET   	= 0x00000010;
					break;
			case 22:
					/** - Set NHET Pin 16 */
					hetREG1->DCLR   	= 0x00010000;
					break;
			case 23:
					/** - Clear NHET Pin 16 */
					hetREG1->DSET   	= 0x00010000;
					break;
			case 24:
					/** - Set NHET Pin 27 */
					hetREG1->DCLR   	= 0x08000000;
					break;
			case 25:
					/** - Clear NHET Pin 27 */
					hetREG1->DSET   	= 0x08000000;
					break;
			case 26:
					/** - Set NHET Pin 27 */
					hetREG1->DSET   	= 0x08000000;
					break;
			case 27:
					/** - Clear NHET Pin 27 */
					hetREG1->DCLR   	= 0x08000000;
					break;
			case 28:
					/** - Set NHET Pin 5 */
					hetREG1->DSET    	= 0x00000020;
					break;
			case 29:
					/** - Clear NHET Pin 5 */
					hetREG1->DCLR 	= 0x00000020;
					break;

			case 30:
					hetREG1->DIR   	= 0xAA178035;
					hetREG1->DOUT   	= 0;
					/* turn off all PWMs */
					for(i=0; i<8; i++){pwmStop(hetRAM1,i);}
					/* Reset the task number to reenter main loop */
					Task_Number = 0;
					break;

		}
	}

	else
	{
	}
}
/** @fn void Running_LED_demo_new(void)
*
*   This function is used for LED demo based on the SCI
*   command different LED's are turned on and a running
*	LED demo is included
*/
void Running_LED_demo_new(void)
{
	unsigned int pwm;		/** - Variable to count the width */
	unsigned led_no,on_off,brightness_level,i;

    /** - NHET is configured as Master 
     *  - NHET is turned On */    
   	hetREG1->GCR     	= 0x01000001; 
	
	/** - PULL functinality is enabled */
	hetREG1->PULDIS	= 0x00000000;
	 
	/** - This if statment is a placeholder for ArgoBoard/USBStick check */
	if(1)
	{
   		/** - Configure NHET[0,2,4,5,25,16,17,18,20,27,29,31] pins as output 
   		 *  - These pins are connected to the LED's */
   		hetREG1->DIR   	= 0xAA178035; 
   		
		switch(SubTask_Number)
		{

			case 0:
					hetREG1->DIR   	= 0xAA178035;
					hetREG1->DOUT   	= 0x08110034;
					/* turn off all PWMs */
					for(i=0; i<6; i++){pwmStop(hetRAM1,i);}
					/* Reset the task number to reenter main loop */
					Task_Number = 0;
	    	case 1:
                    /* - Start all 6 pwm channels */
                    led_no = (task_data & 0x0000FF00)>>8;
                    brightness_level = (task_data & 0x000000FF);
					pwm = 0x1000 - (10-brightness_level)*409;
					/* - Set PWM duty cycles as a percent of sensor data */
					pwmSetDuty(hetRAM1,map_leds_pwms[led_no], ((100*pwm)/0x1000));
					break;
					
					
			case 2:
					/* set different leds or clear different leds*/
					led_no = (task_data & 0x0000FF00)>>8;
					on_off = (task_data & 0x000000FF);
					if(led_no <= 5)
					{
					if(on_off == 1)
					{
					hetREG1->DSET   	= map_led_pins[led_no];
					}
					else
					{
					hetREG1->DCLR   	= map_led_pins[led_no];
					}
					}
					else
					{
					if(on_off == 1)
					{
					hetREG1->DCLR   	= map_led_pins[led_no];
					}
					else
					{
					hetREG1->DSET   	= map_led_pins[led_no];
					}
				
					}
					break;
					
										
			case 3:
					/* set different leds or clear different leds*/
				    hetREG1->DOUT   	= 0x08110034;
				    for(i=0; i<6; i++){pwmStop(hetRAM1,i);}
					led_no = (task_data & 0x0000FF00)>>8;
					brightness_level = (task_data & 0x000000FF);
					pwm = 0x1000 - (10-brightness_level)*409;
					if(brightness_level > 0)
					{
					pwmStart(hetRAM1,map_leds_pwms[led_no]);
					pwmSetDuty(hetRAM1,map_leds_pwms[led_no], ((100*pwm)/0x1000));
					}
					else
					{
					 pwmSetDuty(hetRAM1,map_leds_pwms[led_no], ((100*pwm)/0x1000));
					}
					break;
			/*For REV E boards*/

			case 4:
					/* set different leds or clear different leds*/
					led_no = (task_data & 0x0000FF00)>>8;
					on_off = (task_data & 0x000000FF);
					if(on_off == 1)
					{
					hetREG1->DSET   	= map_led_pins_reve[led_no];
					}
					else
					{
					hetREG1->DCLR   	= map_led_pins_reve[led_no];
					}
					break;
			case 5:
				/* set different leds or clear different leds*/
				hetREG1->DOUT   	= 0;
				for(i=0; i<8; i++){pwmStop(hetRAM1,i);}
				led_no = (task_data & 0x0000FF00)>>8;
				brightness_level = (task_data & 0x000000FF);
				pwm = 0x1000 - (10-brightness_level)*409;
				if(brightness_level > 0)
				{
				pwmStart(hetRAM1,map_leds_pwms_reve[led_no]);
				pwmSetDuty(hetRAM1,map_leds_pwms_reve[led_no], ((100*pwm)/0x1000));
				}
				else
				{
				 pwmSetDuty(hetRAM1,map_leds_pwms_reve[led_no], ((100*pwm)/0x1000));
				}
				break;

			case 6:
					hetREG1->DIR   	= 0xAA178035;
					hetREG1->DOUT   	= 0;
					/* turn off all PWMs */
					for(i=0; i<8; i++){pwmStop(hetRAM1,i);}
					/* Reset the task number to reenter main loop */
					Task_Number = 0;
	    	case 7:
                    /* - Start all 6 pwm channels */
                    led_no = (task_data & 0x0000FF00)>>8;
                    brightness_level = (task_data & 0x000000FF);
					pwm = 0x1000 - (10-brightness_level)*409;
					/* - Set PWM duty cycles as a percent of sensor data */
					pwmSetDuty(hetRAM1,map_leds_pwms_reve[led_no], ((100*pwm)/0x1000));
					break;


	}
	}
	else
	{
	}
}


/** @fn run_LED_StartUp(void)
*   
*   This function is called during the start up, LED's are turned on 
*   in a sequence to indicate the board is powerd up and ready to run
*   the demo software.  
*   
*/
void run_LED_StartUp(void)
{
   	int temp,delay;
   	
	/** - Delay Parameter */
	delay				= 0x200000;

    /** - NHET is configured as Master 
     *  - NHET is turned On */    
   	hetREG1->GCR     	= 0x01000001; 
	
	/** - PULL functinality is enabled */
	hetREG1->PULDIS	= 0x00000000;
	 
	/** - This if statment is a placeholder for ArgoBoard/USBStick check */
	if(1)
	{
   		/** - Configure NHET pins as output */
   		hetREG1->DIR   	= 0xAA178035; 
		/** - Turn all LEDs off */
		hetREG1->DOUT   	= 0x0;   //0x08110034;
		
		/** - Set only NHET[25,0] */ 
		hetREG1->DOUT   	= 0 | (1 << 25) | (1 << 0); //0x0A110035;
		/** - Simple Delay */
		for(temp=0;temp<delay;temp++);
		
		/** - Set only NHET[18,31] */
		hetREG1->DOUT   	= 0 | (1 << 31) | (1 << 18) ; //0x88150034;
		/** - Simple Dealy */
		for(temp=0;temp<delay;temp++);
		
		/** - Set only NHET[29,17] */
		hetREG1->DOUT   	= 0 | (1 << 29) | (1 << 17);   //0x28130034;
		/** - Simple Dealy */
		for(temp=0;temp<delay;temp++);

		/** - Set only NHET[31,18] */
		hetREG1->DOUT   	= 0 | (1 << 31) | ( 1 << 18);   //0x88150034;
		/** - Simple Dealy */
		for(temp=0;temp<delay;temp++);
		
		/** - Set only NHET[0,25] */
		hetREG1->DOUT   	= 0 | (1 << 25) | (1 << 0); //0x0A118035;
		/** - Simple Dealy */
		for(temp=0;temp<delay;temp++);

		/** - Set only NHET[0,25,17,18,29,31] */
		hetREG1->DOUT   	= 0 | (1<<25) | (1<<17) | (1<<18) | (1<<29) | (1 <<31) | (1 <<0); //0xAA178035;
		/** - Simple Dealy */
		for(temp=0;temp<delay;temp++);
		
		/** - Clr all NHET[0,25,17,18,29,31] pins */
		hetREG1->DOUT   	= 0; //0x08110034;
	}
	
	else
	{
	}
}


/** @fn void Running_Light_Balance(void)
*   
*   This function reads the light sensor data
*   and sets to light PWM to create a facimile
*	of an ambient light reaciton system
*/
void Running_Light_Balance(void)
{
	//unsigned int temp;		/** - Temporary Variable */
	unsigned int pwm, i;		/** - Variable to count the width */
    /** - NHET is configured as Master 
     *  - NHET is turned On */    
   	hetREG1->GCR     	= 0x01000001; 
	
	/** - PULL functinality is enabled */
	hetREG1->PULDIS	= 0x00000000;
	 
	/* - Start all 6 pwm channels */
	for(i=0; i<8; i++){pwmStart(hetRAM1,i);}
	 
	/** - This if statment is a placeholder for ArgoBoard/USBStick check */
	if(1)
	{
   		/** - Configure NHET pins as output 
   		 *  - These pins are connected to the LED's */
   		hetREG1->DIR   	= 0xAA178035; 
   		
		switch(SubTask_Number)
		{
			case 0:
				/* Send current PWM value */
				sciSend_32bitdata(sciREG1, (unsigned int)(0x1000 - Get_Ambient_Light_Data()));
				
				/* Maintain PWM loop while PC responds */
				SubTask_Number = 3;
				break;
				
			case 1:
				/*Send current Light Sensor Data */
				sciSend_32bitdata(sciREG1, Get_Ambient_Light_Data());
				
				/* Maintain PWM loop while PC responds */
				SubTask_Number = 3;
				break;
				
			case 2:
				hetREG1->DIR   	= 0xAA178035;
				hetREG1->DOUT   	= 0x08110034;
				/* turn off all PWMs */
				for(i=0; i<8; i++){pwmStop(hetRAM1,i);}
				/* Reset the task number to reenter main loop */
				Task_Number = 0;

				break;
				
			case 3:
				while((Task_Number == 16) && (SubTask_Number == 3))
				{
					/* Fetch light sensor data */
					pwm = Get_Ambient_Light_Data(); 
					
					/* Invert sensor data for LEDs */
					pwm = 0x1000 - pwm;
					
					/* - Set PWM duty cycles as a percent of sensor data */
					for(i=0; i<8; i++){pwmSetDuty(hetRAM1,i, ((100*pwm)/0x1000));}
				}
				break;	
		}
	}
}

/** @fn void SDC_alert(int input)
*   This function temporarialy turns on
* 	red or green LEDs to report status of
* 	SDC initalization   
*/
void SDC_alert(int input)
{
	int i;
	
	if(input == 1)       /* 1 means PASSED */
	{
		sciSend_32bitdata(sciREG1, 1U);
		hetREG1->DCLR   	= 0x00110000;
		for(i=0; i<0x2000000; i++);
		hetREG1->DSET	= 0x00110000;
		
	}
	else  /* 0 means FAILED */
	{
		sciSend_32bitdata(sciREG1, 0U);
		hetREG1->DCLR   	= 0x00000014;
		for(i=0; i<0x2000000; i++);
		hetREG1->DSET	= 0x00000014;
	}
}
