#pragma once

#include "FilterDesigner.h"
#include "Oscillator.h"

#ifndef NSECTIONS
#define NSECTIONS (1)
#endif

#ifndef NSTATE
#define NSTATE (2*NSECTIONS)
#endif

typedef struct modFilter ModFilter;
struct modFilter
{
	float cut_off;
	float resonance;
	float speed;
	float amount;
	float filter_type;
	float oscillator_type;
	float mix;

	float modulation;

	FilterDesigner fd;
	Oscillator osc;

	float state[NSTATE];
	float pm coeffs[5*NSECTIONS];

	float(*Process)(ModFilter* self, float input);
};

ModFilter* ModFilter_NEW(void* address);
void ModFilter_DEL(ModFilter* self);
