#pragma once

#include "AudioObject.h"
#include "FunctionGenerator.h"
#include "FilterZ.h"

#include "../AudioParameter/AudioParameterBool.h"
#include "../AudioParameter/AudioParameterInt.h"
#include "../AudioParameter/AudioParameterFloat.h"
#include "../AudioParameter/AudioParameterChoice.h"
#include "../Utilities/Utils.h"

class ModulationFilter : public AudioObject
{
public:
	ModulationFilter() : AudioObject(8)
	{
		int lfoChoiceList[5] = {E_SINE, E_TRIANGLE, E_SAWTOOTH, E_TRAPEZOID, E_SQUARE };
		int filterChoiceList[3] = { E_LOW_PASS_2, E_BAND_PASS, E_HIGH_PASS_2 };

		bypass			=	new AudioParameterBool	("0x00", "Bypass",				false);
		filterFrequency =	new AudioParameterFloat	("0x01", "Filter Frequency",	250.0f, 2500.0f,			1200.0f);
		filterResoance	=	new AudioParameterFloat	("0x02", "Filter Resoance",		0.01f,	16.0f,				0.707f);
		filterModel		=	new AudioParameterChoice("0x03", "Filter Model",		3,		filterChoiceList,	0);
		lfoFrequency	=	new AudioParameterFloat	("0x04", "LFO Frequency",		1.0f,	10.0f,				8.0f);
		lfoDepth		=	new AudioParameterInt	("0x05", "LFO Depth",			100.0f,	1000.0f,			500.0f);
		lfoModel		=	new AudioParameterChoice("0x06", "LFO Model",			5,		lfoChoiceList,		0);
		mix				=	new AudioParameterFloat	("0x07", "Mix",					0.0f,	1.0f,				0.50f);

		addParameter(bypass);
		addParameter(filterFrequency);
		addParameter(filterResoance);
		addParameter(filterModel);
		addParameter(lfoFrequency);
		addParameter(lfoDepth);
		addParameter(lfoModel);
		addParameter(mix);
	}
	~ModulationFilter()
	{
		Release();
	}

	void Process(float* buffer, uint32_t audio_block_size);
	void Reset();
	void Release();

private:
	FunctionGenerator wave;

	AudioParameterBool* bypass;
	AudioParameterFloat* filterFrequency;
	AudioParameterFloat* filterResoance;
	AudioParameterChoice* filterModel;
	AudioParameterFloat* lfoFrequency;
	AudioParameterInt* lfoDepth;
	AudioParameterChoice* lfoModel;
	AudioParameterFloat* mix;

	CoefficientZ _coefficient;
	BiquadZ _biquad;
};

void ModulationFilter::Process(float* buffer, uint32_t audio_block_size)
{
	if (!(bypass->value))
	{
		_coefficient.model = filterModel->value;
		for(unsigned int index = 0; index < audio_block_size; index++)
		{
			float modulator = wave.Generate((int)(lfoModel->value), (float)(lfoFrequency->value)) * lfoDepth->value + filterFrequency->value;
			_coefficient.setParameter(modulator, filterResoance->value, 0.0f, 0.0f);
			float* coefficients = _coefficient.getCoefficients();
			_biquad._a1 = coefficients[1];
			_biquad._a2 = coefficients[2];
			_biquad._b0 = coefficients[3];
			_biquad._b1 = coefficients[4];
			_biquad._b2 = coefficients[5];

			buffer[index] = _biquad.ProcessSample(buffer[index]) * (mix->value) + buffer[index] * ((1 - mix->value));
		}
	}
}

void ModulationFilter::Reset()
{
	wave.FlushSample();

	bypass->resetToDefaultValue();
	filterFrequency->resetToDefaultValue();	
	filterResoance->resetToDefaultValue();
	filterModel->resetToDefaultValue();
	lfoFrequency->resetToDefaultValue();
	lfoDepth->resetToDefaultValue();
	lfoModel->resetToDefaultValue();
	mix->resetToDefaultValue();
}

void ModulationFilter::Release()
{
	wave.FlushSample();
	wave.~FunctionGenerator();

	delete bypass;
	bypass = NULL;

	delete filterFrequency;
	filterFrequency = NULL;

	delete filterResoance;
	filterResoance = NULL;

	delete filterModel;
	filterModel = NULL;

	delete lfoFrequency;
	lfoFrequency = NULL;

	delete lfoDepth;
	lfoDepth = NULL;

	delete lfoModel;
	lfoModel = NULL;

	delete mix;
	mix = NULL;

	AudioObject::Release();
}


