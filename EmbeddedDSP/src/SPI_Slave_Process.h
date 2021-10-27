#pragma once
#include <stdint.h>    /* Get definition of uint32_t. */

#define DEBUG_SPI 0

void initSPIB_SRU(void);
void setupSPIB(void);
void SPIBTransferISR(uint32_t iid, void *handlerarg);

unsigned int check_version(int command, int head, int tail);
unsigned int declare_size(int command, int head, int tail);
void write_to_buffer(int command, int head, int tail);
unsigned int check_upgrade_result(int command, int head, int tail);
unsigned int check_buffer_size(int command, int head, int tail);
unsigned int start_to_upgrade(int command, int head, int tail);
unsigned int reset_upgrade_flags(int command, int head, int tail);
void software_reset(void);
