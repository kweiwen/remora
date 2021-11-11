#include "blockProcess_audio.h"

#include <sru.h>
#include <stdlib.h>
#include <stdio.h>

#include "Driver/audio_utilities.h"

#include "AudioObject/CircularBuffer.h"
#include "AudioObject/DigitalDelayLine.h"
#include "AudioObject/Oscillator.h"

unsigned int nodeSize = 2;
DigitalDelayLine dl0(SR * 3, 1);
DigitalDelayLine dl1(SR * 3, 2);
AudioObject* audioNodes[8];

// Structures to hold floating point data for each AD1939
ad21479_float_data_1 fBlockA;
ad21479_float_data_2 fBlockB;

// SPORT Ping/Pong Data buffers
extern int TxBlock_A0[];
extern int TxBlock_A1[];

extern int RxBlock_A0[];
extern int RxBlock_A1[];

extern int TxBlock_B0[];
extern int TxBlock_B1[];

extern int RxBlock_B0[];
extern int RxBlock_B1[];

//Pointer to the blocks
int *rx_block_pointer[2] = {RxBlock_A0, RxBlock_A1};
int *tx_block_pointer[2] = {TxBlock_A0, TxBlock_A1};
int *rx_block_pointer2[2] = {RxBlock_B0, RxBlock_B1};
int *tx_block_pointer2[2] = {TxBlock_B0, TxBlock_B1};

/* optimized function to convert the incoming fixed-point data to 32-bit floating-point format. */
/* This function assumes that the incoming fixed point data is in 1.31 format */
#pragma optimize_for_speed
#pragma section ("seg_int_code")  /* handler functions perform better in internal memory */
void floatData(float *output, int *input, unsigned int instep, unsigned int length)
{
    int i;

    for(i = 0; i < length; i++)
    {
        output[i] = __builtin_conv_RtoF(input[instep*i]);
    }
}

/* optimized function to convert the outgoing floating-point data to 1.31 fixed-point format. */
/* This function assumes that the outcoming fixed point data is in 1.31 format */
#pragma optimize_for_speed
#pragma section ("seg_int_code")  /* handler functions perform better in internal memory */
void fixData(int *output, float *input, unsigned int outstep, unsigned int length)
{
    int i;

    for(i = 0; i < length; i++)
    {
        output[outstep*i] = __builtin_conv_FtoR(input[i]);
    }
}

void clear_currentBuffer()
{
	clear_buffer(fBlockA.Rx_L1, NUM_SAMPLES);
	clear_buffer(fBlockA.Rx_R1, NUM_SAMPLES);

	clear_buffer(fBlockB.Tx2_L1, NUM_SAMPLES);
	clear_buffer(fBlockB.Tx2_R1, NUM_SAMPLES);

	clear_buffer(fBlockA.Tx_L1, NUM_SAMPLES);
	clear_buffer(fBlockA.Tx_R1, NUM_SAMPLES);

	clear_buffer(fBlockB.Rx2_L1, NUM_SAMPLES);
	clear_buffer(fBlockB.Rx2_R1, NUM_SAMPLES);
}

void clear_currentDMA()
{
	clear_buffer_int(RxBlock_A0, RX_BLOCK_SIZE);
	clear_buffer_int(RxBlock_A1, RX_BLOCK_SIZE);
	clear_buffer_int(TxBlock_A0, TX_BLOCK_SIZE);
	clear_buffer_int(TxBlock_A1, TX_BLOCK_SIZE);
	clear_buffer_int(RxBlock_B0, RX_BLOCK_SIZE);
	clear_buffer_int(RxBlock_B1, RX_BLOCK_SIZE);
	clear_buffer_int(TxBlock_B0, TX_BLOCK_SIZE);
	clear_buffer_int(TxBlock_B1, TX_BLOCK_SIZE);
}

void prepare_audioBlocks()
{
	audioNodes[0] = &dl0;
	audioNodes[1] = &dl1;

	audioNodes[0]->ParameterCtrl(2, 64);

	audioNodes[1]->ParameterCtrl(0, 16);
	audioNodes[1]->ParameterCtrl(1, 64);
	audioNodes[1]->ParameterCtrl(2, 64);
}

void processSample(float *buffer, unsigned int buffersize)
{
	for(int i = 0; i < buffersize; i++)
	{
		buffer[i] = buffer[i] * 10;
	}
}

void process_audioBlocks()
{
	SRU(HIGH, DAI_PB18_I);

	for(int i = 0; i < nodeSize; i++)
	{
		audioNodes[i]->Process(fBlockA.Rx_L1, NUM_SAMPLES);
	}

	processSample(fBlockA.Rx_L1, NUM_SAMPLES);
	copy_buffer(fBlockA.Rx_L1, fBlockB.Tx2_L1, NUM_SAMPLES);
	copy_buffer(fBlockA.Rx_L1, fBlockB.Tx2_R1, NUM_SAMPLES);

//	processSample(fBlockA.Rx_L1, NUM_SAMPLES);
//	processSample(fBlockA.Rx_R1, NUM_SAMPLES);

	copy_buffer(fBlockA.Rx_L1, fBlockA.Tx_L1, NUM_SAMPLES);
	copy_buffer(fBlockA.Rx_R1, fBlockA.Tx_R1, NUM_SAMPLES);

	copy_buffer(fBlockB.Rx2_L1, fBlockB.Tx2_L1, NUM_SAMPLES);
	copy_buffer(fBlockB.Rx2_R1, fBlockB.Tx2_R1, NUM_SAMPLES);

	SRU(LOW, DAI_PB18_I);
}

void release_audioBlocks()
{
}

/*
 * This function handles the Codec data in the following 3 steps...
 * 1. Converts all ADC data to 32-bit floating-point, and copies this
 * from the current RX DMA buffer into fBlockA & fBlockB
 * 2. Calls the audio processing function (processBlocks)
 * 3. Converts all DAC to 1.31 fixed point, and copies this from
 * 	  fBlockA & fBlockB into the current TX DMA buffer
 * */

void handleCodecData(unsigned int blockIndex)
{
    /* Clear the Block Ready Semaphore */
    inputReady = 0;
    /* Set the Processing Active Semaphore before starting processing */
    isProcessing = 1;

    /* Float ADC data from CS4272 */
	floatData(fBlockA.Rx_L1, rx_block_pointer[blockIndex]+0, NUM_RX_SLOTS, NUM_SAMPLES);
	floatData(fBlockA.Rx_R1, rx_block_pointer[blockIndex]+1, NUM_RX_SLOTS, NUM_SAMPLES);
	floatData(fBlockB.Rx2_L1, rx_block_pointer2[blockIndex]+0, NUM_RX_SLOTS, NUM_SAMPLES);
	floatData(fBlockB.Rx2_R1, rx_block_pointer2[blockIndex]+1, NUM_RX_SLOTS, NUM_SAMPLES);

	/* Place the audio processing algorithm here. */
	process_audioBlocks();

    /* Fix DAC data for CS4272 */
	fixData(tx_block_pointer[blockIndex]+0, fBlockA.Tx_L1, NUM_TX_SLOTS, NUM_SAMPLES);
	fixData(tx_block_pointer[blockIndex]+1, fBlockA.Tx_R1, NUM_TX_SLOTS, NUM_SAMPLES);
	fixData(tx_block_pointer2[blockIndex]+0, fBlockB.Tx2_L1, NUM_TX_SLOTS, NUM_SAMPLES);
	fixData(tx_block_pointer2[blockIndex]+1, fBlockB.Tx2_R1, NUM_TX_SLOTS, NUM_SAMPLES);

    /* Clear the Processing Active Semaphore after processing is complete */
    isProcessing = 0;
}
