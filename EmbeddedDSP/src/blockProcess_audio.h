#pragma once

#include "Driver/ADDS_21479_EzKit.h"

/* Interface Diagram:
 *
 * ------------------------------------------------------------------
 *                INPUT                          OUTPUT
 * ------------------------------------------------------------------
 * AD21479        fBlockA.Rx_L1(Pickup)          fBlockB.Tx2_L1(Line Out)
 *                fBlockA.Rx_R1(Microphone)      fBlockB.Tx2_R1(Speaker)
 * -------------------------------------------------------------------
 * ANDROID        fBlockA.Tx_L1                  fBlockB.Rx2_L1
 *                fBlockA.Tx_R1                  fBlockB.Rx2_R1
 * -------------------------------------------------------------------
 */


/* Define a structure to represent buffers for all 4 floating-point
 * data channels of the CS4272 */
typedef struct{
	float Rx_L1[NUM_SAMPLES];
	float Rx_R1[NUM_SAMPLES];
	float Tx_L1[NUM_SAMPLES];
	float Tx_R1[NUM_SAMPLES];
} ad21479_float_data_1;

typedef struct{
	float Rx2_L1[NUM_SAMPLES];
	float Rx2_R1[NUM_SAMPLES];
	float Tx2_L1[NUM_SAMPLES];
	float Tx2_R1[NUM_SAMPLES];
} ad21479_float_data_2;

void floatData(float *output, int *input, unsigned int instep, unsigned int length);
void fixData(int *output, float *input, unsigned int outstep, unsigned int length);
void clear_currentBuffer(void);
void clear_currentDMA(void);
void prepare_audioBlocks(void);
void process_audioBlocks(void);
void release_audioBlocks(void);
void handleCodecData(unsigned int blockIndex);
