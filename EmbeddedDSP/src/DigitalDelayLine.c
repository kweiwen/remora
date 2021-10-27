#include "DigitalDelayLine.h"
#include <stdlib.h>

static float __process__(DigitalDelayLine* self, float input);

static float __process__(DigitalDelayLine* self, float input)
{	
	float time = self->_time;
	float feedback = self->_feedback;
	float mix = self->_mix;

	float drySignal = input;
	float wetSignal = self->_cb.ReadBufferInterpolation(&self->_cb, time);

	self->_cb.WriteBuffer(&self->_cb, drySignal + wetSignal * feedback);

	return wetSignal * mix + drySignal * (1 - mix);
}


DigitalDelayLine* DigitalDelayLine_NEW(void* address, unsigned int bufferLength, unsigned int location)
{
	if (address == NULL) return NULL;
	DigitalDelayLine* self = address;

	self->_feedback = 0;
	self->_time = 0;
	self->_mix = 1;

	self->Process = __process__;

	CircularBuffer_NEW(&self->_cb);
	self->_cb._interpolation = 1;
	self->_cb.CreateBuffer(&self->_cb, bufferLength, location);

	return self;
}

void DigitalDelayLine_DEL(DigitalDelayLine* self)
{
	CircularBuffer_DEL(&self->_cb);
}
