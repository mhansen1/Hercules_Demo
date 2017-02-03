/**
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *
 *                 TEXAS INSTRUMENTS INCORPORATED PROPRIETARY INFORMATION
 *
 *                 Property of Texas Instruments, Unauthorized reproduction and/or distribution
 *                 is strictly prohibited.  This product  is  protected  under  copyright  law
 *                 and  trade  secret law as an  unpublished work.
 *                 (C) Copyright Texas Instruments.  All rights reserved.
 *
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *
 *  ------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         @file
 *         @version
 *         @component
 *         @module
 *         @generator
 *
 *         @brief
 *         @details
 *
 *
 *---------------------------------------------------------------------------------------------------------------------
 * @author
 *---------------------------------------------------------------------------------------------------------------------
 * Revision History
 *---------------------------------------------------------------------------------------------------------------------
 |Version       | Date        | Author           |    Change ID       | Description                         |
 |------------: |:-----------:|:----------------:|:------------------:|:------------------------------------|
 |0.0.1         | Apr 22, 2014     |                  |    00000000000     | Initial version created             |
 */

#ifndef DEMO_PRIV_H_
#define DEMO_PRIV_H_

/*SAFETYMCUSW 340 S MR:19.7 <APPROVED> "Reason -  This is an advisory by MISRA.We accept this as a coding convention*/
#define BIT(n)                  ((uint32)((uint32)1u <<(n)))

/*SAFETYMCUSW 340 S MR:19.7 <APPROVED> "Reason -  This is an advisory by MISRA.We accept this as a coding convention*/
#define BIT_SET(y, mask)        ((y) |=  (mask))

/*SAFETYMCUSW 340 S MR:19.7 <APPROVED> "Reason -  This is an advisory by MISRA.We accept this as a coding convention*/
#define BIT_CLEAR(y, mask)      ((y) &= ~(mask))

/*SAFETYMCUSW 340 S MR:19.7 <APPROVED> "Reason -  This is an advisory by MISRA.We accept this as a coding convention*/
#define BIT_FLIP(y, mask)       ((y) ^=  (mask))

/* Create a bitmask of length \a len.*/
/*SAFETYMCUSW 340 S MR:19.7 <APPROVED> "Reason -  This is an advisory by MISRA.We accept this as a coding convention*/
#define BIT_MASK(len)           (uint32)(BIT(len)-(uint32)1u)

/* Create a bitfield mask of length \a starting at bit \a start.*/
/*SAFETYMCUSW 340 S MR:19.7 <APPROVED> "Reason -  This is an advisory by MISRA.We accept this as a coding convention*/
#define BF_MASK(start, len)     (BIT_MASK(len)<<(start))

/* Prepare a bitmask for insertion or combining.*/
/*SAFETYMCUSW 340 S MR:19.7 <APPROVED> "Reason -  This is an advisory by MISRA.We accept this as a coding convention*/
#define BF_PREP(x, start, len)  (((x)&BIT_MASK(len)) << (start))

/*SAFETYMCUSW 79 S MR:19.4 <APPROVED> "Use of ternary operator is allowed" */
/*SAFETYMCUSW 340 S MR:19.7 <APPROVED> "Reason -  This is an advisory by MISRA.We accept this as a coding convention*/
#define GET_ESM_BIT_NUM(x)      ((x)<(uint32)32U? (BIT((x))): (BIT(((x)-((uint32)32U)))))
/* Extract a bitfield of length \a len starting at bit \a start from \a y.
 * y is the register
 * start is the starting bit of the field
 * len is the length of the field*/
/*SAFETYMCUSW 340 S MR:19.7 <APPROVED> "Reason -  This is an advisory by MISRA.We accept this as a coding convention*/
#define BF_GET(y, start, len)   ((uint32)(((uint32)((y)>>(start))) & BIT_MASK(len)))

/*Insert a new bitfield value \a x into \a y.
 * y is the register
 * x is the value of the field
 * start is the starting bit of the field
 * len is the length of the field*/
/*SAFETYMCUSW 340 S MR:19.7 <APPROVED> "Reason -  This is an advisory by MISRA.We accept this as a coding convention*/
#define BF_SET(y, x, start, len)    \
    ( (y) = ((y) & (~(BF_MASK((start),(len))))) | BF_PREP((x),(start),(len)))

#define ESM_GRP1_MASK (1U<<16U)
#define ESM_GRP2_MASK (2U<<16U)
#define ESM_GRP3_MASK (3U<<16U)

#endif /* DEMO_PRIV_H_ */
