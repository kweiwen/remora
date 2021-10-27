#pragma once
#include <stdint.h>    /* Get definition of uint32_t. */

void ProcessingTooLong(void);
void TalkThroughISR(uint32_t iid, void* handlerArg);
