/*********************************************************************************

 Copyright(c) 2012 Analog Devices, Inc. All Rights Reserved.

 This software is proprietary and confidential.  By using this software you agree
 to the terms of the associated Analog Devices License Agreement.

 *********************************************************************************/
/*
 * NAME:     SPORT1_isr.c (Block-based Talkthrough for 192 kHz fs)
 * PURPOSE:  Talkthrough framework for sending and receiving samples to the AD1939.
 * USAGE:    This file contains SPORT1 Interrupt Service Routine. Four buffers are used
 * 		     for this example: Two input buffers, and two output buffers.
*/


/*
 *  Here is the mapping between the SPORTS and the ADCs/DACs
 *  For AD1939
 *  ADC1 -> DSP : SPORT1A : TDM Channel 0,1
 *  ADC2 -> DSP : SPORT1A : TDM Channel 2,3
 *  DSP -> DAC1 : SPORT0A : TDM Channel 0,1
 *  DSP -> DAC2 : SPORT0A : TDM Channel 2,3
 *  DSP -> DAC3 : SPORT0B : TDM Channel 0,1
 *  DSP -> DAC4 : SPORT0B : TDM Channel 2,3
 */

#include "SPORT1_isr.h"
#include <stdint.h>    					/* Get definition of uint32_t. */
#include "ADDS_21479_EzKit.h"


/* Counter to choose which buffer to process */
volatile int buffer_cntr = 1;
/* Semaphore to indicate to main that a block is ready for processing */
volatile int inputReady = 0;
/* Semaphore to indicate to the isr that the processing has not completed before the
 * buffer will be overwritten.
 */
volatile int isProcessing = 0;

/* If the processing takes too long, the program will be stuck in this infinite loop. */
void ProcessingTooLong(void)
{
    while(1);
}

void TalkThroughISR(uint32_t iid, void* handlerArg)
{
    if(isProcessing)
        ProcessingTooLong();

    /* Increment the block pointer */
    buffer_cntr++;
    buffer_cntr %= 2;
    inputReady = 1;
}
