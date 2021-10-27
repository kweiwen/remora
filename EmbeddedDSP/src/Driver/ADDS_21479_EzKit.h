#pragma once

#include "../AudioPreference.h"

/* Processing Sample Rate */
#define SAMPLE_RATE (SR)

/* Block Size per Audio Channel */
#define NUM_SAMPLES (BLOCK_SIZE)

/* Number of stereo channels */
#define NUM_RX_SLOTS 2
#define NUM_TX_SLOTS 2

/* Size of each channel */
#define RX_BLOCK_SIZE (NUM_SAMPLES * NUM_RX_SLOTS)
#define TX_BLOCK_SIZE (NUM_SAMPLES * NUM_TX_SLOTS)

#define SPIB_MODE (CPHASE | CLKPL)

#define PCI  0x00080000
#define OFFSET 0x00080000
#define OFFSET_MASK 0x7FFFF

/* DSP Version */
#ifndef DSP_GENERAL_VERSION
#define DSP_GENERAL_VERSION	(0000001)
#endif

#ifndef UPGRADE_BUFFER_SIZE
#define UPGRADE_BUFFER_SIZE (400000)
#endif

#ifndef UART_BUFFER_SIZE
#define UART_BUFFER_SIZE (8)
#endif

#ifndef UART_BUFFER_MASK
#define UART_BUFFER_MASK (UART_BUFFER_SIZE - 1)
#endif

/* Global Variables for Audio Processing */
extern volatile int count;
extern volatile int isProcessing;
extern volatile int inputReady;
extern volatile int buffer_cntr;

/* Global Variables for SPI Communication */
extern char upgrade_buffer[UPGRADE_BUFFER_SIZE];
extern unsigned int upgrade_size;
extern unsigned int upgrade_index;
extern unsigned int upgrade_flag;
extern unsigned int upgrade_rate;
extern unsigned int upgrade_result;

/* Global Variables for UART Communication */
extern char uart_buffer[UART_BUFFER_SIZE];
extern volatile int commandReady;
extern volatile int isCommandProcessing;
extern volatile int uart_buffer_cntr;
extern volatile int uart_buffer_ready;

extern volatile int timer_isr_count;   /* low priority counter */
