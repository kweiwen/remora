#include "FilterDesigner.h"
#include <stdlib.h>
#include <math.h>

static void __setParameter__(FilterDesigner* self, float cutOff, float sampleRate, float Q, float slope, float magnitude);
static void __setCoefficients__(FilterDesigner* self);
static float* __getCoefficients__(FilterDesigner* self);

static void __setParameter__(FilterDesigner* self, float cutOff, float sampleRate, float Q, float slope, float magnitude)
{
	self->omega = TWO_PI * cutOff / sampleRate;
	self->sine_omega = sinf(self->omega);
	self->cosine_omega = cosf(self->omega);
	self->gain = powf(10, (magnitude / 20));
	self->q = Q;
	self->slope = slope;
	__setCoefficients__(self);
}

static void __setCoefficients__(FilterDesigner* self)
{
	switch (self->model)
	{
	case E_FLAT:
		self->a0 = 1;
		self->a1 = 0;
		self->a2 = 0;
		self->b0 = 1;
		self->b1 = 0;
		self->b2 = 0;
		break;
	case E_LOW_PASS_1:
		self->a1 = powf(EULER, -self->omega);

		// numerator
		self->b0 = (1.0 - self->a1) * self->gain;
		self->b1 = 0.0;
		self->b2 = 0.0;

		// denominator
		self->a1 = -self->a1;
		self->a2 = 0.0;

		// set a0 into 1 after coefficients normalization
		self->a0 = 1;
		break;
	case E_LOW_PASS_2:
		self->alpha = self->sine_omega / (2 * self->q);

		// denominator normalization
		self->a0 = 1 + self->alpha;
		self->a1 = (-2 * self->cosine_omega) / self->a0;
		self->a2 = (1 - self->alpha) / self->a0;

		// numerator normalization
		self->b0 = (1 - self->cosine_omega) * self->gain / 2 / self->a0;
		self->b1 = (1 - self->cosine_omega) * self->gain / self->a0;
		self->b2 = (1 - self->cosine_omega) * self->gain / 2 / self->a0;

		// set a0 into 1 after coefficients normalization
		self->a0 = 1;
		break;
	case E_HIGH_PASS_1:
		break;
	case E_HIGH_PASS_2:
		self->alpha = self->sine_omega / (2 * self->q);

		// denominator normalization
		self->a0 = 1 + self->alpha;
		self->a1 = (-2 * self->cosine_omega) / self->a0;
		self->a2 = (1 - self->alpha) / self->a0;

		// numerator normalization
		self->b0 = (1 + self->cosine_omega) * self->gain / 2 / self->a0;
		self->b1 = -(1 + self->cosine_omega) * self->gain / self->a0;
		self->b2 = (1 + self->cosine_omega) * self->gain / 2 / self->a0;

		// set a0 into 1 after coefficients normalization
		self->a0 = 1;
		break;
	case E_ALL_PASS_1:
		break;
	case E_ALL_PASS_2:
		break;
	case E_PEAK:
		break;
	case E_PARAMETRIC:
		break;
	case E_BAND_PASS:
		self->alpha = self->sine_omega / (2 * self->q);

		// denominator normalization
		self->a0 = 1 + self->alpha;
		self->a1 = (-2 * self->cosine_omega) / self->a0;
		self->a2 = (1 - self->alpha) / self->a0;

		// numerator normalization
		self->b0 = self->alpha * self->gain / self->a0;
		self->b1 = 0.0;
		self->b2 = -self->alpha * self->gain / self->a0;

		// set a0 into 1 after coefficients normalization
		self->a0 = 1;
		break;
	case E_BAND_REJECT:
		break;
	case E_LOW_SHELF:
		break;
	case E_HIGH_SHELF:
		break;
	}
}

static float* __getCoefficients__(FilterDesigner* self)
{
	static float coefficients[6];

	// numerator of transfer function
	coefficients[0] = self->b0;
	coefficients[1] = self->b1;
	coefficients[2] = self->b2;

	// denominator of transfer function
	coefficients[3] = self->a0;
	coefficients[4] = self->a1;
	coefficients[5] = self->a2;

	return coefficients;
}

FilterDesigner* FilterDesigner_NEW(void* address)
{
	if (address == NULL) return NULL;
	FilterDesigner* self = address;

	// numerator of transfer function
	self->b0 = 0;
	self->b1 = 0;
	self->b2 = 0;

	// denominator of transfer function
	self->a0 = 0;
	self->a1 = 0;
	self->a2 = 0;

	self->omega = 0;
	self->sine_omega = 0;
	self->cosine_omega = 0;
	self->gain = 0;
	self->q = 0;
	self->slope = 0;
	self->alpha = 0;

	self->model = 0;

	self->SetParameter = __setParameter__;
	self->SetCoefficients = __setCoefficients__;
	self->GetCoefficients = __getCoefficients__;

	return self;
}


void FilterDesigner_DEL(FilterDesigner* self)
{

}
