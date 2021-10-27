#include "ModFilter.h"
#include <stdlib.h>
#include <Filters.h>

static float __process__(ModFilter* self, float input);

static float __process__(ModFilter* self, float input)
{
	self->modulation = self->osc.Process(&self->osc, self->speed, 44100, self->oscillator_type, 0) * self->amount + self->cut_off;
	self->fd.SetParameter(&self->fd, self->modulation, 44100, self->resonance, 0, 0);

	// numerator of transfer function
	self->coeffs[0] = self->fd.b2;
	self->coeffs[1] = self->fd.b1;
	self->coeffs[2] = self->fd.b0;

	// denominator of transfer function
	self->coeffs[3] = -self->fd.a2;
	self->coeffs[4] = -self->fd.a1;

	return biquad(input, self->coeffs, self->state, NSECTIONS) * self->mix + input * (1 - self->mix);
}

ModFilter* ModFilter_NEW(void* address)
{
	if (address == NULL) return NULL;
	ModFilter* self = address;

	Oscillator_NEW(&self->osc);

	FilterDesigner_NEW(&self->fd);
	self->fd.model = 1;
	self->fd.SetParameter(&self->fd, 1200, 44100, 1.5, 1, 0);

	self->amount = 500;
	self->cut_off = 1200;
	self->resonance = 1.5;
	self->oscillator_type = 0;
	self->speed = 20;
	self->mix = 0.5;

	for (int i = 0; i < NSTATE; i++)
	{
		self->state[i] = 0.0;
	}

	self->coeffs[0] = self->fd.b2;
	self->coeffs[1] = self->fd.b1;
	self->coeffs[2] = self->fd.b0;
	self->coeffs[3] = -self->fd.a2;
	self->coeffs[4] = -self->fd.a1;

	self->Process = __process__;

	return self;
}

void ModFilter_DEL(ModFilter* self)
{

}
