/** @file HL_sys_dma.c 
*   @brief DMA Driver Source File
*   @date 20.May.2014
*   @version 04.00.00
*
*   This file contains:
*   - API Functions
*   - Interrupt Handlers
*   .
*   which are relevant for the DMA driver.
*/

/* (c) Texas Instruments 2009-2013, All rights reserved. */


#include "HL_sys_dma.h"

/** @fn void dmaEnable(void)
*   @brief enables DMA module
*
*   This function brings DMA out of reset
*/
void dmaEnable(void)
{
  dmaREG->GCTRL  = 0x00010000U; /* enable dma      */
  dmaREG->GCTRL |= 0x00000300U; /* stop at suspend */
}

/** @fn void dmaDisable(void)
*   @brief disables DMA module
*
*   This function disables DMA module
*/
void dmaDisable(void)
{
	while((dmaREG->GCTRL & 0x00004000U) != 0U)
	{
	} /* Wait */

	/* Disable DMA module */
	dmaREG->GCTRL = 0U;
}

/** @fn void dmaSetCtrlPacket(uint32 channel)
*   @brief Set control packet
*
*   This function sets control packet
*/
void dmaSetCtrlPacket(dmaChannel_t channel, g_dmaCTRL g_dmaCTRLPKT)
{
    uint8 i,j;

    dmaRAMREG->PCP[channel].ISADDR  =  g_dmaCTRLPKT.SADD;

    dmaRAMREG->PCP[channel].IDADDR  =  g_dmaCTRLPKT.DADD;

    dmaRAMREG->PCP[channel].ITCOUNT = (g_dmaCTRLPKT.FRCNT << 16U) | g_dmaCTRLPKT.ELCNT;

    dmaRAMREG->PCP[channel].CHCTRL  = (g_dmaCTRLPKT.RDSIZE    << 14U) | (g_dmaCTRLPKT.WRSIZE    << 12U) | (g_dmaCTRLPKT.TTYPE << 8U)| \
                                      (g_dmaCTRLPKT.ADDMODERD << 3U ) | (g_dmaCTRLPKT.ADDMODEWR << 1U ) | (g_dmaCTRLPKT.AUTOINIT);

    dmaRAMREG->PCP[channel].CHCTRL |= (g_dmaCTRLPKT.CHCTRL << 16U);

    dmaRAMREG->PCP[channel].EIOFF   = (g_dmaCTRLPKT.ELDOFFSET << 16U) | (g_dmaCTRLPKT.ELSOFFSET);

    dmaRAMREG->PCP[channel].FIOFF   = (g_dmaCTRLPKT.FRDOFFSET << 16U) | (g_dmaCTRLPKT.FRSOFFSET);

    i = channel / 8U;          /* Find the register to write */
    j = channel % 8U;          /* Find the offset            */
    j = (uint8)7U - j;         /* Reverse the order          */
    j = j * 4U;                /* Find the bit position      */

    dmaREG->PAR[i] &= ~((uint32)0xFU << j);
    dmaREG->PAR[i] |= (g_dmaCTRLPKT.PORTASGN << j);
}

/** @fn void dmaSetChEnable(uint32 channel,uint32 type)
*   @brief Enable channel
*   @param[in] channel DMA channel
*   @param[in] type Type of triggering
*                    - DMA_HW: Enables the selected DMA channel for hardware triggering
*                    - DMA_SW: Enables the selected DMA channel for software triggering
*
*   This function enables the DMA channel for hardware or software triggering
*/
void dmaSetChEnable(dmaChannel_t channel, dmaTriggerType_t type)
{
	if(type == DMA_HW)
	{
		dmaREG->HWCHENAS = (uint32)1U << channel;
	}
    else
    {
    	dmaREG->SWCHENAS = (uint32)1U << channel;
    }
}

/** @fn void dmaReqAssign(uint32 channel,uint32 reqline)
*   @brief Assign DMA request lines to channels
*   @param[in] channel DMA channel
*   @param[in] reqline DMA request line
*
*   This function assigns DMA request lines to channels
*/
void dmaReqAssign(dmaChannel_t channel, dmaRequest_t reqline)
{
    uint8 i,j;

    i = channel / 4U;        /* Find the register to configure */
    j = channel % 4U;        /* Find the offset                */
    j = (uint8)3U - j;       /* reverse the byte order         */
    j = j * 8U;              /* find the bit location          */

    /* Mapping channel 'i' to request line 'j' */
    dmaREG->DREQASI[i] &= ~((uint32)0xFFU   << j);
    dmaREG->DREQASI[i] |=  ((uint32)reqline << j);
}

/** @fn void dmaSetPriority(uint32 channel, dmaPRIORITY_t priority)
*   @brief Assign Priority to the channel
*   @param[in] channel DMA channel
*   @param[in] priority Priority queue to which channel needs to be assigned
*                        - LOWPRIORITY : The selected channel will be assigned to low priority queue
*                        - HIGHPRIORITY: The selected channel will be assigned to high priority queue
*
*   This function assigns the selected priority to the selected channel
*/
void dmaSetPriority(dmaChannel_t channel, dmaPriorityQueue_t priority)
{
	if (priority == LOWPRIORITY)
	{
		dmaREG->CHPRIOR = (uint32)1U << channel;
	}
	else
	{
		dmaREG->CHPRIOS = (uint32)1U << channel;
	}
}

/** @fn void dmaEnableInterrupt(dmaChannel_t channel, dmaInterrupt_t inttype, dmaIntGroup_t group)
*   @brief Enable selected interrupt
*   @param[in] channel DMA channel
*   @param[in] inttype Interrupt to be enabled
*                        - FTC: Frame Transfer Complete Interrupt will be disabled for the selected channel
*                        - LFS: Last Frame Transfer Started Interrupt will be disabled for the selected channel
*                        - HBC: First Half Of Block Complete Interrupt will be disabled for the selected channel
*                        - BTC: Block transfer complete Interrupt will be disabled for the selected channel
*                        - BER: Bus Error Interrupt will be disabled for the selected channel
*   @param[in] group   Group to which the interrupt is routed to.
*                        - DMA_INTA : Group A
*						 - DMA_INTB : Group B (Do not use this in case of Lock-step device)
*
*   This function enables the selected interrupt for the selected channel
*/
void dmaEnableInterrupt(dmaChannel_t channel, dmaInterrupt_t inttype, dmaIntGroup_t group)
{
    dmaREG->GCHIENAS = (uint32)1U << channel;

    switch (inttype)
    {
    case FTC:   dmaREG->FTCINTENAS = (uint32)1U << channel;
				dmaREG->FTCMAP     = (dmaREG->FTCMAP & ~((uint32)1U << channel)) | ((uint32)group << channel);
                break;
    case LFS:   dmaREG->LFSINTENAS = (uint32)1U << channel;
                dmaREG->LFSMAP     = (dmaREG->LFSMAP & ~((uint32)1U << channel)) | ((uint32)group << channel);
                break;
    case HBC:   dmaREG->HBCINTENAS = (uint32)1U << channel;
                dmaREG->HBCMAP     = (dmaREG->HBCMAP & ~((uint32)1U << channel)) | ((uint32)group << channel);
                break;
    case BTC:   dmaREG->BTCINTENAS = (uint32)1U << channel;
                dmaREG->BTCMAP     = (dmaREG->BTCMAP & ~((uint32)1U << channel)) | ((uint32)group << channel);
                break;
    default :
                break;
    }
}
/** @fn void dmaDisableInterrupt(uint32 channel, dmaInterrupt_t inttype)
*   @brief Disable selected interrupt
*   @param[in] channel DMA channel
*   @param[in] inttype Interrupt to be disabled
*                       - FTC: Frame Transfer Complete Interrupt will be disabled for the selected channel
*                       - LFS: Last Frame Transfer Started Interrupt will be disabled for the selected channel
*                       - HBC: First Half Of Block Complete Interrupt will be disabled for the selected channel
*                       - BTC: Block transfer complete Interrupt will be disabled for the selected channel
*                       - BER: Bus Error Interrupt will be disabled for the selected channel
*
*   This function disables the selected interrupt for the selected channel
*/
void dmaDisableInterrupt(dmaChannel_t channel, dmaInterrupt_t inttype)
{
    switch (inttype)
    {
    case FTC:   dmaREG->FTCINTENAR = (uint32)1U << channel;
                break;
    case LFS:   dmaREG->LFSINTENAR = (uint32)1U << channel;
                break;
    case HBC:   dmaREG->HBCINTENAR = (uint32)1U << channel;
                break;
    case BTC:   dmaREG->BTCINTENAR = (uint32)1U << channel;
                break;
    default :
                break;
    }
}
/** @fn void dmaDefineRegion(dmaREGION_t region, uint32 start_add, uint32 end_add)
*   @brief Configure start and end address of the region
*   @param[in] region Memory Region
*                     - DMA_REGION0
*                     - DMA_REGION1
*                     - DMA_REGION2
*                     - DMA_REGION3
*   @param[in] start_add Start address of the the region
*   @param[in] end_add End address of the region
*
*   This function configure start and end address of the selected region
*/
void dmaDefineRegion(dmaMPURegion_t region, uint32 start_add, uint32 end_add)
{
	if(region < 4U)
	{
		dmaREG->DMAMPR_L[region].STARTADD = start_add;
		dmaREG->DMAMPR_L[region].ENDADD   = end_add;
	}
	else
	{
		dmaREG->DMAMPR_H[region - 4U].STARTADD = start_add;
		dmaREG->DMAMPR_H[region - 4U].ENDADD   = end_add;
	}
}

/** @fn void dmaEnableRegion(dmaREGION_t region, dmaRegionAccess_t access, dmaMPUInt_t intenable)
*   @brief Enable the selected region
*   @param[in] region Memory Region
*                     - DMA_REGION0
*                     - DMA_REGION1
*                     - DMA_REGION2
*                     - DMA_REGION3
*   @param[in] access Access permission of the selected region
*                      - FULLACCESS
*                      - READONLY
*                      - WRITEONLY
*                      - NOACCESS
*   @param[in] intenable Interrupt to be enabled or not
*                         - INTERRUPTA_ENABLE : Enable Group A interrupt for the selected region
*                         - INTERRUPTB_ENABLE : Enable Group B interrupt for the selected region (Do not use this in case of Lock-step device)
*                         - INTERRUPT_DISABLE : Disable interrupt for the selected region
*
*   This function enables the selected region with selected access permission with or without interrupt enable
*/
void dmaEnableRegion(dmaMPURegion_t region, dmaRegionAccess_t access, dmaMPUInt_t intenable)
{
	uint8 bitpos = 0U;

	if(region < 4U)
	{
		bitpos = region * 8U;
		dmaREG->DMAMPCTRL1 &= ~(uint32)((uint32)0xFFU << bitpos);


		dmaREG->DMAMPCTRL1 |= ((uint32)1U << bitpos)                /* Enable the region */
    		        	   |  ((uint32)access << (bitpos + 1U))     /* Set access permission for the region */
    		               |  ((uint32)intenable << (bitpos + 3U)); /* Enable or Disable interrupt */
	}
	else
	{
		bitpos = (region - 4U) * 8U;
		dmaREG->DMAMPCTRL2 &= ~((uint32)0xFFU << bitpos);


		dmaREG->DMAMPCTRL2 |= ((uint32)1U << bitpos)                /* Enable the region */
    		        	   |  ((uint32)access << (bitpos + 1U))     /* Set access permission for the region */
    		               |  ((uint32)intenable << (bitpos + 3U)); /* Enable or Disable interrupt */
	}
}

/** @fn void dmaDisableRegion(dmaREGION_t region)
*   @brief Disable the selected region
*   @param[in] region Memory Region
*                     - DMA_REGION0
*                     - DMA_REGION1
*                     - DMA_REGION2
*                     - DMA_REGION3
*
*   This function disables the selected region(no address checking done).
*/
void dmaDisableRegion(dmaMPURegion_t region)
{
	uint8 bitpos = 0U;

	if(region < 4U)
	{
		bitpos = region * 8U;
		dmaREG->DMAMPCTRL1 &= ~((uint32)1U << bitpos);
	}
	else
	{
		bitpos = (region - 4U) * 8U;
		dmaREG->DMAMPCTRL2 &= ~((uint32)1U << bitpos);
	}
}

/** @fn void dmaEnableECC(void)
*   @brief Enable ECC
*
*   This function enables ECC check
*/
void dmaEnableECC(void)
{
	dmaREG->DMAPCR = 0xAU;
}

/** @fn void dmaDisableECC(void)
*   @brief Disable ECC
*
*   This function disables ECC check
*/
void dmaDisableECC(void)
{
	dmaREG->DMAPCR = 0x5U;
}

/** @fn uint32 dmaGetReq(uint32 channel)
*   @brief Gets the request line number mapped to the selected channel
*   @param[in] channel DMA channel
*
*   This function returns the request line number mapped to the selected channel
*/
uint32 dmaGetReq(dmaChannel_t channel)
{
    uint8 i,j;

    i = channel / 4U;        /* Find the register to configure */
    j = channel % 4U;        /* Find the offset                */
    j = (uint8)3U - j;       /* reverse the byte order         */
    j = j * 8U;              /* find the bit location          */

    return ((dmaREG->DREQASI[i] >> j) & 0xFFU);
}

/** @fn boolean dmaIsChannelActive(dmaChannel_t channel)
*   @brief Gets the status of the DMA channel
*   @param[in] channel DMA channel
*
*   This function returns TRUE if the channel is currently being processed using one of the FIFOs.
*/
boolean dmaIsChannelActive(dmaChannel_t channel)
{
	boolean status;
	uint32 bitmask = (uint32)1U << channel;

	if((dmaREG->DMASTAT & bitmask) == 0U)
	{
		status = FALSE;
	}
	else
	{
		status = TRUE;
	}

	return status;
}

/** @fn boolean dmaIsBusy(void)
*   @brief Gets the status of the DMA bus
*
*   This function returns TRUE if DMA's external bus is busy in data transfers
*/
boolean dmaIsBusy(void)
{
	boolean status;

	if((dmaREG->GCTRL & 0x4000U) == 0U)
	{
		status = FALSE;
	}
	else
	{
		status = TRUE;
	}

	return status;
}
