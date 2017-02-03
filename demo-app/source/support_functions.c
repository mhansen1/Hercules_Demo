#include "HL_adc.h"
#include "HL_sci.h"
#include "sl_api.h"
#include "demo_priv.h"

/** @fn void adcGetSingleData(adcBASE_t *adc, unsigned group, adcData_t *data)
*   @brief Get single converted ADC value
*   @param[in] adc Pointer to ADC module:
*              - adcREG1: ADC1 module pointer
*              - adcREG2: ADC2 module pointer
*              - adcREG3: ADC3 module pointer
*   @param[in] group Hardware group of ADC module:
*              - adcGROUP0: ADC event group
*              - adcGROUP1: ADC group 1
*              - adcGROUP2: ADC group 2
*   @param[out] data Pointer to store ADC converted data
*
*/
void adcGetSingleData(adcBASE_t *adc, unsigned group, adcData_t *data)
{
    unsigned  buf;
    adcData_t *ptr = data; 

    /** -  Get conversion data and channel/pin id */
    buf        = adc->GxBUF[group].BUF0;
    ptr->value = (unsigned short)(buf & 0xFFFU);
    ptr->id    = (unsigned short)((buf >> 16U) & 0x1FU); // int to unsigned short

    adc->GxINTFLG[group] = 9U;

    /**   @note The function canInit has to be called before this function can be used.\n
    *           The user is responsible to initialize the message box.
    */
}

/** @fn void adcStartConversion_selChn(adcBASE_t *adc, unsigned channel, unsigned fifo_size, unsigned group)
*   @brief Starts an ADC conversion
*   @param[in] adc Pointer to ADC module:
*              - adcREG1: ADC1 module pointer
*              - adcREG2: ADC2 module pointer
*   @param[in] channel ADC channel to be selected for conversion
*   @param[in] fifo_size ADC fifo size to be configured.
*   @param[in] group Hardware group of ADC module:
*              - adcGROUP0: ADC event group
*              - adcGROUP1: ADC group 1
*              - adcGROUP2: ADC group 2
*
*   This function Starts the convesion of the ADC selected group for the selected channel
*
*/
void adcStartConversion_selChn(adcBASE_t *adc, unsigned channel, unsigned fifo_size, unsigned group)
{
    /** - Setup FiFo size */
    adc->GxINTCR[group] = fifo_size;

    /** - Start Conversion */
    adc->GxSEL[group] = 1 << channel;
}

/** @fn void sciSend_32bitdata(sciBASE_t *sci, unsigned int data)
*
*	Function used for send a 32bit data on to the SCI at the 
*   configured baud rate    
*/
void sciSend_32bitdata(sciBASE_t *sci, unsigned int data)
{
	unsigned char c_get ;
  	volatile int i = 0;
  	for( i = 8 ; i > 0 ; i-- )
  	{
 		c_get = (data>>28)&15 ;
		if( c_get > 9 )
			c_get += 7 ;
		c_get += 48 ;
		while ((sci->FLR & SCI_TX_INT) == 0) { /* wait */ };
		sci->TD = c_get;
		data = data<<4 ;
	}
}

void AutiInitMemories(void)
{
    BF_SET(sl_dmaREG->DMAPCR, DMA_PARCTRL_ECC_DIS, DMA_PARCTRL_ECC_EN_START,
            DMA_PARCTRL_ECC_EN_LENGTH);
    BF_SET(sl_canREG1->CTL, CAN_CTRL_SECDED_DIS, CAN_CTRL_SECDED_START,
            CAN_CTRL_SECDED_LENGTH);
    BF_SET(sl_canREG2->CTL, CAN_CTRL_SECDED_DIS, CAN_CTRL_SECDED_START,
            CAN_CTRL_SECDED_LENGTH);
    BF_SET(sl_canREG3->CTL, CAN_CTRL_SECDED_DIS, CAN_CTRL_SECDED_START,
            CAN_CTRL_SECDED_LENGTH);
    BF_SET(sl_canREG4->CTL, CAN_CTRL_SECDED_DIS, CAN_CTRL_SECDED_START,
            CAN_CTRL_SECDED_LENGTH);
    BF_SET(sl_mibspiREG1->PAR_ECC_CTRL, SPI_PAR_CTRL_ECC_DIS,
            SPI_PAR_CTRL_ECC_START, SPI_PAR_CTRL_ECC_LENGTH);
    BF_SET(sl_mibspiREG2->PAR_ECC_CTRL, SPI_PAR_CTRL_ECC_DIS,
            SPI_PAR_CTRL_ECC_START, SPI_PAR_CTRL_ECC_LENGTH);
    BF_SET(sl_mibspiREG3->PAR_ECC_CTRL, SPI_PAR_CTRL_ECC_DIS,
            SPI_PAR_CTRL_ECC_START, SPI_PAR_CTRL_ECC_LENGTH);
    BF_SET(sl_mibspiREG4->PAR_ECC_CTRL, SPI_PAR_CTRL_ECC_DIS,
            SPI_PAR_CTRL_ECC_START, SPI_PAR_CTRL_ECC_LENGTH);
    BF_SET(sl_mibspiREG5->PAR_ECC_CTRL, SPI_PAR_CTRL_ECC_DIS,
            SPI_PAR_CTRL_ECC_START, SPI_PAR_CTRL_ECC_LENGTH);
    BF_SET(sl_dmaREG->DMAPCR, DMA_PARCTRL_ECC_EN, DMA_PARCTRL_ECC_EN_START,
            DMA_PARCTRL_ECC_EN_LENGTH);
    SL_Init_Memory(RAMTYPE_DMA_RAM);
    BF_SET(sl_canREG1->CTL, CAN_CTRL_SECDED_EN, CAN_CTRL_SECDED_START,
            CAN_CTRL_SECDED_LENGTH);
    BF_SET(sl_canREG2->CTL, CAN_CTRL_SECDED_EN, CAN_CTRL_SECDED_START,
            CAN_CTRL_SECDED_LENGTH);
    BF_SET(sl_canREG3->CTL, CAN_CTRL_SECDED_EN, CAN_CTRL_SECDED_START,
            CAN_CTRL_SECDED_LENGTH);
    BF_SET(sl_canREG4->CTL, CAN_CTRL_SECDED_EN, CAN_CTRL_SECDED_START,
            CAN_CTRL_SECDED_LENGTH);
    SL_Init_Memory(RAMTYPE_DCAN1_RAM);
    SL_Init_Memory(RAMTYPE_DCAN2_RAM);
    SL_Init_Memory(RAMTYPE_DCAN3_RAM);
    SL_Init_Memory(RAMTYPE_DCAN4_RAM);
    BIT_SET(sl_mibspiREG1->GCR0, SPI_GCR0_RESET);
    BIT_SET(sl_mibspiREG1->MIBSPIE, SPI_MIBSPIE_MSPIENA);
    BF_SET(sl_mibspiREG1->PAR_ECC_CTRL, SPI_PAR_CTRL_ECC_EN,
            SPI_PAR_CTRL_ECC_START, SPI_PAR_CTRL_ECC_LENGTH);
    SL_Init_Memory(RAMTYPE_MIBSPI1_RAM);
    BIT_SET(sl_mibspiREG2->GCR0, SPI_GCR0_RESET);
    BIT_SET(sl_mibspiREG2->MIBSPIE, SPI_MIBSPIE_MSPIENA);
    BF_SET(sl_mibspiREG2->PAR_ECC_CTRL, SPI_PAR_CTRL_ECC_EN,
            SPI_PAR_CTRL_ECC_START, SPI_PAR_CTRL_ECC_LENGTH);
    SL_Init_Memory(RAMTYPE_MIBSPI2_RAM);
    BIT_SET(sl_mibspiREG3->GCR0, SPI_GCR0_RESET);
    BIT_SET(sl_mibspiREG3->MIBSPIE, SPI_MIBSPIE_MSPIENA);
    BF_SET(sl_mibspiREG3->PAR_ECC_CTRL, SPI_PAR_CTRL_ECC_EN,
            SPI_PAR_CTRL_ECC_START, SPI_PAR_CTRL_ECC_LENGTH);
    SL_Init_Memory(RAMTYPE_MIBSPI3_RAM);
    BIT_SET(sl_mibspiREG4->GCR0, SPI_GCR0_RESET);
    BIT_SET(sl_mibspiREG4->MIBSPIE, SPI_MIBSPIE_MSPIENA);
    BF_SET(sl_mibspiREG4->PAR_ECC_CTRL, SPI_PAR_CTRL_ECC_EN,
            SPI_PAR_CTRL_ECC_START, SPI_PAR_CTRL_ECC_LENGTH);
    SL_Init_Memory(RAMTYPE_MIBSPI4_RAM);
    BIT_SET(sl_mibspiREG5->GCR0, SPI_GCR0_RESET);
    BIT_SET(sl_mibspiREG5->MIBSPIE, SPI_MIBSPIE_MSPIENA);
    BF_SET(sl_mibspiREG5->PAR_ECC_CTRL, SPI_PAR_CTRL_ECC_EN,
            SPI_PAR_CTRL_ECC_START, SPI_PAR_CTRL_ECC_LENGTH);
    SL_Init_Memory(RAMTYPE_MIBSPI5_RAM);
}
