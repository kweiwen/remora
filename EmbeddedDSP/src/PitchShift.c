#include "PitchShift.h"
#include "Oscillator.h"
#include <math.h>
#include <stdlib.h>

static float __process__(PitchShift* self, float input);

static float __process__(PitchShift* self, float input)
{
	float speed = ((powf(2, (self->_pitch / 12.0))) - 1) * -20.0;
	self->_phasor1 = self->_osc1.Process(&self->_osc1, speed, 44100, 5, 0);
	self->_phasor2 = self->_osc2.Process(&self->_osc2, speed, 44100, 5, 0.5);
	self->_window1 = ((cosf(M_TWO_PI * self->_phasor1) * -1) + 1) / 2.0;
	self->_window2 = ((cosf(M_TWO_PI * self->_phasor2) * -1) + 1) / 2.0;

	self->_dl1._time = self->_phasor1 * 2205;
	self->_dl2._time = self->_phasor2 * 2205;

	float Phase_A = self->_dl1.Process(&self->_dl1, input) * self->_window1;
	float Phase_B = self->_dl1.Process(&self->_dl2, input) * self->_window2;

	return (Phase_A + Phase_B) * self->_mix + input * (1 - self->_mix);
}

PitchShift* PitchShift_NEW(void* address, unsigned int bufferLength, unsigned int location)
{
	if (address == NULL) return NULL;
	PitchShift* self = address;

	self->_window1 = 0;
	self->_window2 = 0;
	self->_phasor1 = 0;
	self->_phasor2 = 0;
	self->_pitch = 12;
	self->_mix = 1;

	self->Process = __process__;

	DigitalDelayLine_NEW(&self->_dl1, bufferLength, location);
	DigitalDelayLine_NEW(&self->_dl2, bufferLength, location);

	Oscillator_NEW(&self->_osc1);
	Oscillator_NEW(&self->_osc2);

	return self;
}

void PitchShift_DEL(PitchShift* self)
{
	DigitalDelayLine_DEL(&self->_dl1);
	DigitalDelayLine_DEL(&self->_dl2);
}
