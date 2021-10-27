#pragma once

#include "CircularBuffer.h"

typedef struct digitalDelayLine DigitalDelayLine;
struct digitalDelayLine
{
	float _time;
	float _mix;
	float _feedback;
	
	CircularBuffer _cb;

    float(*Process)(DigitalDelayLine* self, float input);
};

DigitalDelayLine* DigitalDelayLine_NEW(void* address, unsigned int bufferLength, unsigned int location);
void DigitalDelayLine_DEL(DigitalDelayLine* self);
