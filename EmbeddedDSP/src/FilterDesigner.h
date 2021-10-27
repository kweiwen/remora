#pragma once

#ifndef TWO_PI
#define TWO_PI (6.283185307179586476925286766559)
#endif

#ifndef EULER
#define EULER (2.71828182845904523536)
#endif

enum E_FILTER_TYPE
{
	E_FLAT = 0,
	E_LOW_PASS_2 = 1,
	E_BAND_PASS = 2,
	E_HIGH_PASS_2 = 3,
	E_LOW_PASS_1 = 4,
	E_HIGH_PASS_1 = 5,
	E_ALL_PASS_1 = 6,
	E_ALL_PASS_2 = 7,
	E_PEAK = 8,
	E_PARAMETRIC = 9,
	E_BAND_REJECT = 10,
	E_LOW_SHELF = 11,
	E_HIGH_SHELF = 12,
};

typedef struct filterDesigner FilterDesigner;
struct filterDesigner
{
	// numerator of transfer function
	float b0;
	float b1;
	float b2;

	// denominator of transfer function
	float a0;
	float a1;
	float a2;

	unsigned int model;

	float omega;
	float sine_omega;
	float cosine_omega;
	float gain;
	float q;
	float slope;
	float alpha;

	void(*SetParameter)(FilterDesigner* self, float cut_off, float sample_rate, float Q, float slope, float magnitude);
	void(*SetCoefficients)(FilterDesigner* self);
	float*(*GetCoefficients)(FilterDesigner* self);
};

FilterDesigner* FilterDesigner_NEW(void* address);
void FilterDesigner_DEL(FilterDesigner* self);
