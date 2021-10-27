#pragma once
#include <stdint.h>

typedef struct preProcessing PreProcessing;
struct preProcessing
{
	float gain;
	float peak;

	void(*Process)(PreProcessing* self, float* buffer, uint32_t audio_block_size);
};

PreProcessing* PreProcessing_NEW(void* address);
void PreProcessing_DEL(PreProcessing* self);
