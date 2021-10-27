#include "Octave.h"
#include <stdlib.h>

static float __process__(Octave* self, float input);

static float __process__(Octave* self, float input)
{
	return (self->_ps1.Process(&self->_ps1, input) + self->_ps2.Process(&self->_ps2, input)) * 0.5 * self->_mix + input * (1 - self->_mix);
}

Octave* Octave_NEW(void* address, unsigned int bufferLength, unsigned int location)
{
	if (address == NULL) return NULL;
	Octave* self = address;

	self->Process = __process__;

	PitchShift_NEW(&self->_ps1, bufferLength, location);
	PitchShift_NEW(&self->_ps2, bufferLength, location);

	self->_mix = 1;
	self->_ps1._pitch = 12;
	self->_ps2._pitch = -12;

	return self;
}

void Octave_DEL(Octave* self)
{
	PitchShift_DEL(&self->_ps1);
	PitchShift_DEL(&self->_ps2);
}
