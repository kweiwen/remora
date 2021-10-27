#pragma once

#include "DigitalDelayLine.h"
#include "Oscillator.h"

typedef struct pitchShift PitchShift;
struct pitchShift
{
	DigitalDelayLine _dl1;
	DigitalDelayLine _dl2;
	Oscillator _osc1;
	Oscillator _osc2;
	float _window1;
	float _window2;
	float _phasor1;
	float _phasor2;
	float _pitch;
	float _mix;

	float(*Process)(PitchShift* self, float input);
};

PitchShift* PitchShift_NEW(void* address, unsigned int bufferLength, unsigned int location);
void PitchShift_DEL(PitchShift* self);
