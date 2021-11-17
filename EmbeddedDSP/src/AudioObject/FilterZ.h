#pragma once

#include "../Utilities/AudioPreference.h"
#include "../Utilities/Utils.h"
#include <math.h>

enum E_FILTER_TYPE
{
	E_FLAT		  = 0,
	E_LOW_PASS_1  = 1,
	E_LOW_PASS_2  = 2,
	E_HIGH_PASS_1 = 3,
	E_HIGH_PASS_2 = 4,
	E_BAND_PASS   = 5,
	E_BAND_REJECT = 6,
	E_ALL_PASS_1  = 7,
	E_ALL_PASS_2  = 8,
	E_PARAMETRIC  = 9,
	E_PEAK        = 10,
	E_LOW_SHELF   = 11,
	E_HIGH_SHELF  = 12,
};

class BiquadZ
{
public:
	BiquadZ()
	{
		ResetSample();
	}

	~BiquadZ()
	{

	}

	void ResetSample();
	float ProcessSample(float input_data);

	float _b0;
	float _b1;
	float _b2;

	float _a1;
	float _a2;

private:

	float _prev_input_1;
	float _prev_input_2;
	float _prev_output_1;
	float _prev_output_2;
};

float BiquadZ::ProcessSample(float input_data)
{
	float output_data = (_b0 * input_data) +
		(_b1 * _prev_input_1) +
		(_b2 * _prev_input_2) -
		(_a1 * _prev_output_1) -
		(_a2 * _prev_output_2);

	_prev_input_2 = _prev_input_1;
	_prev_input_1 = input_data;
	_prev_output_2 = _prev_output_1;
	_prev_output_1 = output_data;
	return output_data;
}

void BiquadZ::ResetSample()
{
	_b0 = 0.0f;
	_b1 = 0.0f;
	_b2 = 0.0f;

	_a1 = 0.0f;
	_a2 = 0.0f;

	_prev_input_1 = 0.0f;
	_prev_input_2 = 0.0f;
	_prev_output_1 = 0.0f;
	_prev_output_2 = 0.0f;
}

class CoefficientZ
{

public:
	CoefficientZ()
    {
		model = E_FLAT;
		_omega = 0;
		_sine_omega = 0;
		_cosine_omega = 0;
		_gain = 0;
		_q = 0;
		_slope = 0;
		_alpha = 0;
		_magnitude = 0;
		_A = 0;
    };

    ~CoefficientZ()
    {
    };

	void setParameter(float cut_off = 1200, float Q = 0.707, float _slope = 0, float _magnitude = 0);
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

	float _omega;
	float _sine_omega;
	float _cosine_omega;
	float _gain;
	float _q;
	float _slope;
	float _alpha;
	float _magnitude;
	float _A;
};

void CoefficientZ::setParameter(float cut_off, float Q, float slope, float magnitude)
{
	_omega = (TWO_PI * cut_off) / SR;
	_sine_omega = sin(_omega);
	_cosine_omega = cos(_omega);
	_magnitude = magnitude;
	_gain = powf(10, (magnitude / 20));
	_A = powf(10.0, (_magnitude / 40.0));
	_q = Q;
	_slope = slope;
	setCoefficients();
}

void CoefficientZ::setCoefficients()
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
		a1 = powf(EULER, -_omega);

		// numerator
		b0 = (1.0 - a1) * _gain;
		b1 = 0.0;
		b2 = 0.0;

		// denominator
		a1 = -a1;
		a2 = 0.0;

		// set a0 into 1 after coefficients normalization 
		a0 = 1;
		break;
	case E_LOW_PASS_2:
		_alpha = _sine_omega / (2 * _q);

		// denominator normalization
		a0 = 1 + _alpha;
		a1 = (-2 * _cosine_omega) / a0;
		a2 = (1 - _alpha) / a0;

		// numerator normalization
		b0 = (1 - _cosine_omega) * _gain / 2 / a0;
		b1 = (1 - _cosine_omega) * _gain / a0;
		b2 = (1 - _cosine_omega) * _gain / 2 / a0;

		// set b0 into 1 after coefficients normalization 
		a0 = 1;
		break;
	case E_HIGH_PASS_1:
		a1 = powf(EULER, -_omega);

		// numerator
		b0 = (1.0 + a1) * _gain / 2;
		b1 = -(1.0 + a1) * _gain / 2;
		b2 = 0.0;

		// denominator
		a1 = -a1;
		a2 = 0.0;

		// set b0 into 1 after coefficients normalization 
		a0 = 1;
		break;
	case E_HIGH_PASS_2:
		_alpha = _sine_omega / (2 * _q);

		// denominator normalization
		a0 = 1 + _alpha;
		a1 = (-2 * _cosine_omega) / a0;
		a2 = (1 - _alpha) / a0;

		// numerator normalization
		b0 = (1 + _cosine_omega) * _gain / 2 / a0;
		b1 = -(1 + _cosine_omega) * _gain / a0;
		b2 = (1 + _cosine_omega) * _gain / 2 / a0;

		// set b0 into 1 after coefficients normalization 
		a0 = 1;
		break;
	case E_ALL_PASS_1:
		a1 = powf(EULER, -_omega);

		// numerator
		b0 = -a1 * _gain;
		b1 = _gain;
		b2 = 0.0;

		// denominator
		a1 = -a1;
		b2 = 0.0;

		// set b0 into 1 after coefficients normalization 
		a0 = 1;
		break;
	case E_ALL_PASS_2:
		_alpha = _sine_omega / (2 * _q);

		// denominator normalization
		a0 = 1 + _alpha;
		a1 = (-2 * _cosine_omega) / a0;
		a2 = (1 - _alpha) / a0;

		// numerator normalization
		b0 = (1 - _alpha) * _gain / a0;
		b1 = (-2 * _cosine_omega) * _gain / a0;
		b2 = (1 + _alpha) * _gain / a0;

		// set b0 into 1 after coefficients normalization 
		a0 = 1;
		break;
	case E_PEAK:
		_alpha = _sine_omega / (2 * _q);

		// denominator normalization
		a0 = 1 + _alpha / _A;
		a1 = (-2 * _cosine_omega) / a0;
		a2 = (1 - _alpha / _A) / a0;

		// numerator normalization
		b0 = (1 + _alpha * _A) / a0;
		b1 = -(2 * _cosine_omega) / a0;
		b2 = (1 - _alpha * _A) / a0;

		// set b0 into 1 after coefficients normalization 
		a0 = 1;
		break;
	case E_PARAMETRIC:
		_alpha = _sine_omega / (2 * _A * _q);

		a0 = 1 + _alpha / _A;
		a1 = (-2 * _cosine_omega) / a0;
		a2 = (1 - _alpha / _A) / a0;

		b0 = (1 + _alpha * _A) / a0;
		b1 = -(2 * _cosine_omega) / a0;
		b2 = (1 - _alpha * _A) / a0;

		break;
	case E_BAND_PASS:
		_alpha = _sine_omega / (2 * _q);
		
		a0 = 1 + _alpha;
		a1 = (-2.0 * _cosine_omega) / a0;
		a2 = (1 - _alpha) / a0;

		b0 = (_alpha * _gain) / a0;
		b1 = 0.0;
		b2 = -(_alpha * _gain) / a0;

		// set b0 into 1 after coefficients normalization 
		a0 = 1;
		break;
	case E_BAND_REJECT:
		_alpha = _sine_omega / (2 * _q);

		a0 = 1 + _alpha;
		a1 = (-2.0 * _cosine_omega) / a0;
		a2 = (1.0 - _alpha) / a0;

		b0 = _gain / a0;
		b1 = (-2.0 * _cosine_omega * _gain) / a0;
		b2 = _gain / a0;

		// set b0 into 1 after coefficients normalization 
		a0 = 1;
		break;
	case E_LOW_SHELF:
		//_slope should be in the range 0 < _slope <= 2
		if (_slope >= 2.0)
		{
			_slope = 2.0;
		}
		else if(_slope <= 0.1)
		{
			_slope = 0.1;
		}

		_alpha = _sine_omega / 2 * sqrtf((_A + 1 / _A) * (1 / _slope - 1) + 2);

		a0 = (_A + 1) + (_A - 1) * _cosine_omega + 2 * sqrtf(_A) * _alpha;
		a1 = (-2 * ((_A - 1) + (_A + 1) * _cosine_omega)) / a0;
		a2 = ((_A + 1) + (_A - 1) * _cosine_omega - 2 * sqrtf(_A) * _alpha) / a0;

		b0 = (_A * ((_A + 1) - (_A - 1) * _cosine_omega + 2 * sqrtf(_A) * _alpha)) / a0;
		b1 = (2 * _A * ((_A - 1) - (_A + 1) * _cosine_omega)) / a0;
		b2 = (_A * ((_A + 1) - (_A - 1) * _cosine_omega - 2 * sqrtf(_A) * _alpha)) / a0;

		// set b0 into 1 after coefficients normalization 
		a0 = 1;
		break;
	case E_HIGH_SHELF:
		//_slope should be in the range 0 < _slope <= 2
		if (_slope >= 2.0)
		{
			_slope = 2.0;
		}
		else if (_slope <= 0.1)
		{
			_slope = 0.1;
		}

		_alpha = _sine_omega / 2 * sqrtf((_A + 1 / _A) * (1 / _slope - 1) + 2);

		a0 = (_A + 1) - (_A - 1) * _cosine_omega + 2 * sqrtf(_A) * _alpha;
		a1 = (2 * ((_A - 1) - (_A + 1) * _cosine_omega)) / a0;
		a2 = ((_A + 1) - (_A - 1) * _cosine_omega - 2 * sqrtf(_A) * _alpha) / a0;

		b0 = (_A * ((_A + 1) + (_A - 1) * _cosine_omega + 2 * sqrtf(_A) * _alpha)) / a0;
		b1 = (-2 * _A * ((_A - 1) + (_A + 1) * _cosine_omega)) / a0;
		b2 = (_A * ((_A + 1) + (_A - 1) * _cosine_omega - 2 * sqrtf(_A) * _alpha)) / a0;

		// set b0 into 1 after coefficients normalization 
		a0 = 1;
		break;
	}
}

float* CoefficientZ::getCoefficients()
{
	static float coefficients[6];
	// denominator of transfer function
	coefficients[0] = a0;
	coefficients[1] = a1;
	coefficients[2] = a2;

	// numerator of transfer function
	coefficients[3] = b0;
	coefficients[4] = b1;
	coefficients[5] = b2;
	return coefficients;
}