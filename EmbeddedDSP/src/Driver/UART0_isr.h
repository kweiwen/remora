#pragma once
#include <stdint.h>    /* Get definition of uint32_t. */

void xmitUARTmessage(char* xmit, int SIZE);
void initUART(void);
void UART0ISR(uint32_t iid, void *handlerArg);
void handleControlData(void);
void TimerISR(uint32_t iid, void* handlerArg);
void DBG(char* input_data);
