#pragma once

#include "FilterDesigner.h"
#include "Oscillator.h"
#include "CircularBuffer.h"
#include "DigitalDelayLine.h"

#ifndef NSECTIONS
#define NSECTIONS (1)
#endif

#ifndef NSTATE
#define NSTATE (2*NSECTIONS)
#endif

typedef struct feedbackDelayNetwork FeedbackDelayNetwork;
struct feedbackDelayNetwork
{
	float _mix;
	float _preDelay;
	float _brightness;
	float _damping;
	float _decay;
	float _size;
	float _speed;
	float _depth;

	CircularBuffer _cb_1;
	CircularBuffer _cb_2;
	CircularBuffer _cb_3;
	CircularBuffer _cb_4;

	float feedback_1;
	float feedback_2;
	float feedback_3;
	float feedback_4;

	float lpf_1;
	float lpf_2;
	float lpf_3;
	float lpf_4;

	Oscillator osc_1;
	Oscillator osc_2;
	Oscillator osc_3;
	Oscillator osc_4;

	float modulation_1;
	float modulation_2;
	float modulation_3;
	float modulation_4;

	float dampOutput_1;
	float dampOutput_2;
	float dampOutput_3;
	float dampOutput_4;

	FilterDesigner fd;

	float state_0[NSTATE];
	float pm pre_coeffs[5*NSECTIONS];

	float state_1[NSTATE];
	float state_2[NSTATE];
	float state_3[NSTATE];
	float state_4[NSTATE];
	float pm coeffs[5*NSECTIONS];

	float(*Process)(FeedbackDelayNetwork* self, float input);
};

FeedbackDelayNetwork* FeedbackDelayNetwork_NEW(void* address, unsigned int location);
void FeedbackDelayNetwork_DEL(FeedbackDelayNetwork* self);
