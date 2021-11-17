#pragma once

#include "../Utilities/AudioPreference.h"
#include "../Utilities/Utils.h"
#include <iostream>

enum E_WAVEFORM
{
	E_SINE 		= 0,
	E_TRIANGLE 	= 1,
	E_SAWTOOTH 	= 2,
	E_TRAPEZOID = 3,
	E_SQUARE 	= 4,
	E_PHASOR 	= 5,
	E_PULSE		= 6,
};

class FunctionGenerator
{
public:
	FunctionGenerator(float phase = 0.0f, float pulse_width = 0.5f)
	{
		offset = phase;
		width = pulse_width;
		_degree = 0;
		cycle = 0 + phase;
		_value = 0;
	}

	~FunctionGenerator()
	{
	}

	float Generate(unsigned int waveform, float frequency);
	void FlushSample();
	float offset;
	float width;
	float cycle;

private:
	float _degree;
	float _value;
};

float FunctionGenerator::Generate(unsigned int waveform, float frequency)
{
	cycle = cycle + (frequency * INV_SR);
	cycle = Utils::getFractionalPart(cycle);
	_degree = cycle * TWO_PI;

	switch(waveform)
	{
	case E_SINE:
		_value = sinf(_degree);
		break;
	case E_TRIANGLE:
		_value = 2 * INV_PI * asinf(sinf(_degree));
		break;
	case E_SAWTOOTH:
		_value = (cycle - 0.5f) * 2.0f;
	    break;
	case E_TRAPEZOID:
		_value = 0;
		for (int index = 0; index < 16; index++)
		{
			int coefficeint = (index * 2) + 1;
			_value = _value + sinf(_degree * coefficeint) / coefficeint;
		}
	    break;
	case E_SQUARE:
		if(cycle < width)
			_value = 1;
		else if(cycle > width)
			_value = -1;
		else
			_value = 0;
	    break;
	case E_PHASOR:
		if(frequency)
			_value = cycle;
		else
			_value = (cycle * -1.0f) + 1.0f;
		break;
	}
	return _value;
}

void FunctionGenerator::FlushSample()
{
	cycle = 0;
	_degree = 0;
	_value = 0;
}
