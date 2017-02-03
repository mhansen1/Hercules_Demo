#ifndef __HAL_ADC_H__
#define __HAL_ADC_H__

#include <sl_reg_adc.h>

#define ADC_12BIT_RESOLUTION 	   (uint32)(0x80000000U)
#define ADC_ENABLE_CALIBRATION 	   (uint32)(0x00000001U)
#define ADC_CALIB_BRIDGE_ENABLE    (uint32)(0x00000200U)
#define ADC_CALIB_HILO_ENABLE	   (uint32)(0x00000100U)
#define ADC_START_CALIBRARION      (uint32)(0x00010000U)
#define ADC_ENABLE_SELFTEST		   (uint32)(0x01000000U)
#define ADC_VALUE_MASK   		   (uint32)(0x00000FFFU)
#define ADC_GROUP_THR_INT_FLG	   (uint32)(0x00000001U)
#define ADC_GROUP_CONV_END         (uint32)(0x00000008U)
/*SAFETYMCUSW 340 S MR:19.7 <APPROVED> "Reason -  This is an advisory by MISRA.We accept this as a coding convention*/
#define ADC_GET_CHANNEL_ID(fifobuffer) (uint32)(((fifobuffer) & 0x001F0000U)>>16)
#endif /* __HAL_ADC_H__ */
