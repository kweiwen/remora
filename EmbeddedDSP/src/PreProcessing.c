#include "PreProcessing.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sru.h>

static void __process__(PreProcessing* self, float* buffer, uint32_t audio_block_size);

static void __process__(PreProcessing* self, float* buffer, uint32_t audio_block_size)
{
	for(int i = 0; i < audio_block_size; i++)
	{
		if(fabsf(buffer[i]) > self->peak)
		{
			self->peak = fabsf(buffer[i]);
		}
		buffer[i] *= self->gain;
	}
}

PreProcessing* PreProcessing_NEW(void* address)
{
	if (address == NULL) return NULL;
	PreProcessing* self = address;

	self->gain = 1;
	self->peak = 0;
	self->Process = __process__;

	return self;
}

void PreProcessing_DEL(PreProcessing* self)
{

}

