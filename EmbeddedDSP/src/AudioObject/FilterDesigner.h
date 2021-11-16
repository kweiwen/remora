#pragma once

#include "../Utilities/AudioPreference.h"
#include "../Utilities/Utils.h"
#include <math.h>

enum E_FILTER_TYPE
{
	E_FLAT		  = 0,
	E_LOW_PASS_2  = 1,
	E_BAND_PASS   = 2,
	E_HIGH_PASS_2 = 3,
	E_LOW_PASS_1  = 4,
	E_HIGH_PASS_1 = 5,
	E_ALL_PASS_1  = 6,
	E_ALL_PASS_2  = 7,
	E_PEAK        = 8,
	E_PARAMETRIC  = 9,
	E_BAND_REJECT = 10,
	E_LOW_SHELF   = 11,
	E_HIGH_SHELF  = 12,
};

class FilterDesigner
{

public:
	FilterDesigner()
    {
		model = E_FLAT;
		omega = 0;
		sine_omega = 0;
		cosine_omega = 0;
		gain = 0;
		q = 0;
		slope = 0;
		alpha = 0;
    };

    ~FilterDesigner()
    {
    };

	void setParameter(float cut_off = 1200, float Q = 0.707, float slope = 0, float magnitude = 0);
    void setCoefficients();
	float* getCoefficients();
	int model;

private:
	double TWO_PI = 6.283185307179586476925286766559;
	double EULER = 2.71828182845904523536;

	// numerator of transfer function
	float b0 = 0;
	float b1 = 0;
	float b2 = 0;
	// denominator of transfer function
    float a0 = 0;
    float a1 = 0;
    float a2 = 0;

	float omega;
	float sine_omega;
	float cosine_omega;
	float gain;
	float q;
	float slope;
	float alpha;
};


void FilterDesigner::setParameter(float cut_off, float Q, float slope, float magnitude)
{
	omega = TWO_PI * cut_off / SR;
	sine_omega = sin(omega);
	cosine_omega = cos(omega);
	gain = pow(10, (magnitude / 20));
	q = Q;
	slope = slope;
	setCoefficients();
}

void FilterDesigner::setCoefficients()
{
	switch (model)
	{
	case E_FLAT:
		b0 = 1;
		b1 = 0;
		b2 = 0;
		a0 = 1;
		a1 = 0;
		a2 = 0;
		break;
	case E_LOW_PASS_1:
		b1 = pow(EULER, -omega);

		// numerator
		a0 = (1.0 - b1) * gain;
		a1 = 0.0;
		a2 = 0.0;

		// denominator
		b1 = -b1;
		b2 = 0.0;

		// set b0 into 1 after coefficients normalization 
		b0 = 1;
		break;
	case E_LOW_PASS_2:
		alpha = sine_omega / (2 * q);

		// denominator normalization
		b0 = 1 + alpha;
		b1 = (-2 * cosine_omega) / b0;
		b2 = (1 - alpha) / b0;

		// numerator normalization
		a0 = (1 - cosine_omega) * gain / 2 / b0;
		a1 = (1 - cosine_omega) * gain / b0;
		a2 = (1 - cosine_omega) * gain / 2 / b0;

		// set b0 into 1 after coefficients normalization 
		b0 = 1;
		break;
	case E_HIGH_PASS_1:
		b1 = pow(EULER, -omega);

		// numerator
		a0 = (1.0 + b1) * gain / 2;
		a1 = -(1.0 + b1) * gain / 2;
		a2 = 0.0;

		// denominator
		b1 = -b1;
		b2 = 0.0;

		// set b0 into 1 after coefficients normalization 
		b0 = 1;
		break;
	case E_HIGH_PASS_2:
		alpha = sine_omega / (2 * q);

		// denominator normalization
		b0 = 1 + alpha;
		b1 = (-2 * cosine_omega) / b0;
		b2 = (1 - alpha) / b0;

		// numerator normalization
		a0 = (1 + cosine_omega) * gain / 2 / b0;
		a1 = -(1 + cosine_omega) * gain / b0;
		a2 = (1 + cosine_omega) * gain / 2 / b0;

		// set b0 into 1 after coefficients normalization 
		b0 = 1;
		break;
	case E_ALL_PASS_1:
		b1 = pow(EULER, -omega);

		// numerator
		a0 = -b1 * gain;
		a1 = gain;
		a2 = 0.0;

		// denominator
		b1 = -b1;
		b2 = 0.0;

		// set b0 into 1 after coefficients normalization 
		b0 = 1;
		break;
	case E_ALL_PASS_2:
		alpha = sine_omega / (2 * q);

		// denominator normalization
		b0 = 1 + alpha;
		b1 = (-2 * cosine_omega) / b0;
		b2 = (1 - alpha) / b0;

		// numerator normalization
		a0 = (1 - alpha) * gain / b0;
		a1 = (-2 * cosine_omega) * gain / b0;
		a2 = (1 + alpha) * gain / b0;

		// set b0 into 1 after coefficients normalization 
		b0 = 1;
		break;
	case E_PEAK:
		break;
	case E_PARAMETRIC:
		break;
	case E_BAND_PASS:
		alpha = sine_omega / (2 * q);

		// denominator normalization
		b0 = 1 + alpha;
		b1 = (-2 * cosine_omega) / b0;
		b2 = (1 - alpha) / b0;

		// numerator normalization
		a0 = alpha * gain / b0;
		a1 = 0.0;
		a2 = -alpha * gain / b0;

		// set b0 into 1 after coefficients normalization 
		b0 = 1;
		break;
	case E_BAND_REJECT:
		break;
	case E_LOW_SHELF:
		break;
	case E_HIGH_SHELF:
		break;
	}
}

float* FilterDesigner::getCoefficients()
{
	static float coefficients[6];
	// numerator of transfer function
	coefficients[0] = a0;
	coefficients[1] = a1;
	coefficients[2] = a2;

	// denominator of transfer function
	coefficients[3] = b0;
	coefficients[4] = b1;
	coefficients[5] = b2;
	return coefficients;
}