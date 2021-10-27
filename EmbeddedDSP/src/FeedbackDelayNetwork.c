#include "FeedbackDelayNetwork.h"
#include "stdlib.h"

#include <Filters.h>

static float __process__(FeedbackDelayNetwork* self, float input);

static float __process__(FeedbackDelayNetwork* self, float input)
{
	float mix = self->_mix;
	float preDelay = self->_preDelay;
	float brightness = self->_brightness;
	float damping = self->_damping;
	float decay = self->_decay;
	float size = self->_size;
	float speed = self->_speed;
	float depth = self->_depth;

	float drySignal = biquad(input, self->pre_coeffs, self->state_0, NSECTIONS);

    self->modulation_1 = self->osc_1.Process(&self->osc_1, speed, 44100, 0, 0);
    self->modulation_2 = self->osc_2.Process(&self->osc_2, speed, 44100, 0, 0.25 * TWO_PI);
    self->modulation_3 = self->osc_3.Process(&self->osc_3, speed, 44100, 0, 0.50 * TWO_PI);
    self->modulation_4 = self->osc_4.Process(&self->osc_4, speed, 44100, 0, 0.75 * TWO_PI);

    self->feedback_1 = self->_cb_1.ReadBufferInterpolation(&self->_cb_1, (2819.0f + self->modulation_1 * depth) *  size);
    self->feedback_2 = self->_cb_2.ReadBufferInterpolation(&self->_cb_2, (3343.0f + self->modulation_2 * depth) *  size);
    self->feedback_3 = self->_cb_3.ReadBufferInterpolation(&self->_cb_3, (3581.0f + self->modulation_3 * depth) *  size);
    self->feedback_4 = self->_cb_4.ReadBufferInterpolation(&self->_cb_4, (4133.0f + self->modulation_4 * depth) *  size);

//	self->fd.SetParameter(&self->fd, brightness, 44100, 0, 0, 0);

	// numerator of transfer function
	self->coeffs[0] = 0;
	self->coeffs[1] = self->fd.b1;
	self->coeffs[2] = self->fd.b0;

	// denominator of transfer function
	self->coeffs[3] = 0;
	self->coeffs[4] = -self->fd.a1;

    self->lpf_1 = biquad(self->feedback_1, self->coeffs, self->state_1, NSECTIONS);
    self->lpf_2 = biquad(self->feedback_2, self->coeffs, self->state_2, NSECTIONS);
    self->lpf_3 = biquad(self->feedback_3, self->coeffs, self->state_3, NSECTIONS);
    self->lpf_4 = biquad(self->feedback_4, self->coeffs, self->state_4, NSECTIONS);

    self->dampOutput_1 = (self->lpf_1 - self->feedback_1) * damping;
    self->dampOutput_2 = (self->lpf_2 - self->feedback_2) * damping;
    self->dampOutput_3 = (self->lpf_3 - self->feedback_3) * damping;
    self->dampOutput_4 = (self->lpf_4 - self->feedback_4) * damping;

    float A = (self->dampOutput_1 + self->feedback_1) * 0.5f * (decay * 0.25 + 0.75) + drySignal;
    float B = (self->dampOutput_2 + self->feedback_2) * 0.5f * (decay * 0.25 + 0.75) + drySignal;
    float C = (self->dampOutput_3 + self->feedback_3) * 0.5f * (decay * 0.25 + 0.75);
    float D = (self->dampOutput_4 + self->feedback_4) * 0.5f * (decay * 0.25 + 0.75);

    float output_1 = A + B + C + D;
    float output_2 = A - B + C - D;
    float output_3 = A + B - C - D;
    float output_4 = A - B - C + D;

    self->_cb_1.WriteBuffer(&self->_cb_1, output_1);
    self->_cb_2.WriteBuffer(&self->_cb_2, output_2);
    self->_cb_3.WriteBuffer(&self->_cb_3, output_3);
    self->_cb_4.WriteBuffer(&self->_cb_4, output_4);

//    return (output_1 + output_2 + output_3 + output_4) * 0.25f * mix + drySignal * (1 - mix);
    return (output_1 + output_2 + output_3 + output_4);
}


FeedbackDelayNetwork* FeedbackDelayNetwork_NEW(void* address, unsigned int location)
{
	if (address == NULL) return NULL;
	FeedbackDelayNetwork* self = address;


	self->_mix = 0.4f;
	self->_preDelay = 0.0f;
	self->_brightness = 1500;
	self->_damping = 0.0f;
	self->_decay = 0.80;
	self->_size = 1.0f;
	self->_speed = 0.5f;
	self->_depth = 40.0f;

	self->Process = __process__;

	CircularBuffer_NEW(&self->_cb_1);
	self->_cb_1.CreateBuffer(&self->_cb_1, 4096, location);
	CircularBuffer_NEW(&self->_cb_2);
	self->_cb_2.CreateBuffer(&self->_cb_2, 4096, location);
	CircularBuffer_NEW(&self->_cb_3);
	self->_cb_3.CreateBuffer(&self->_cb_3, 4096, location);
	CircularBuffer_NEW(&self->_cb_4);
	self->_cb_4.CreateBuffer(&self->_cb_4, 4096, location);

	Oscillator_NEW(&self->osc_1);
	Oscillator_NEW(&self->osc_2);
	Oscillator_NEW(&self->osc_3);
	Oscillator_NEW(&self->osc_4);

	FilterDesigner_NEW(&self->fd);
	self->fd.model = 4;
	self->fd.SetParameter(&self->fd, 1200, 44100, 1.5, 1, 0);

	self->coeffs[0] = self->fd.b2;
	self->coeffs[1] = self->fd.b1;
	self->coeffs[2] = self->fd.b0;

	self->coeffs[3] = -self->fd.a2;
	self->coeffs[4] = -self->fd.a1;

	self->pre_coeffs[0] = 0.9849995433380051;
	self->pre_coeffs[1] = -1.9699990866760102;
	self->pre_coeffs[2] = 0.9849995433380051;

	self->pre_coeffs[3] = -0.9702240460889019;
	self->pre_coeffs[4] = 1.9697741272631186;

	for (int i = 0; i < NSTATE; i++)
	{
		self->state_0[i] = 0.0;
		self->state_1[i] = 0.0;
		self->state_2[i] = 0.0;
		self->state_3[i] = 0.0;
		self->state_4[i] = 0.0;
	}


	return self;
}

void FeedbackDelayNetwork_DEL(FeedbackDelayNetwork* self)
{
	CircularBuffer_DEL(&self->_cb_1);
	CircularBuffer_DEL(&self->_cb_2);
	CircularBuffer_DEL(&self->_cb_3);
	CircularBuffer_DEL(&self->_cb_4);
}
