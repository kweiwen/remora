#pragma once

#include <stdint.h>
#include "CircularBuffer.h"
#include "AudioObject.h"
#include "../AudioParameter/AudioParameterInt.h"
#include "../AudioParameter/AudioParameterFloat.h"
#include "../AudioParameter/AudioParameterBool.h"
#include "../Utilities/AudioPreference.h"

class FeedbackDelayNetwork : public AudioObject
{
public:
	FeedbackDelayNetwork(unsigned int location = 0) : AudioObject(8)
	{
		_coefficient.model = E_LOW_PASS_1;

		wave1.cycle = 0;
		wave2.cycle = 0.25f;
		wave3.cycle = 0.50f;
		wave4.cycle = 0.75f;

		cb1.CreateBuffer(4096, location);
		cb2.CreateBuffer(4096, location);
		cb3.CreateBuffer(4096, location);
		cb4.CreateBuffer(4096, location);

		bypass				= new AudioParameterBool	("0x00", "Bypass",			false);
		reverbBrightness	= new AudioParameterFloat	("0x01", "Brightness",		150.0f,		5000.0f,	1500.0f);
		reverbDamping		= new AudioParameterFloat	("0x02", "Damping",			0.0f,		1.0f,		0.10f);
		reverbDecay			= new AudioParameterFloat	("0x03", "Decay",			0.0f,		1.0f,		0.75f);
		reverbSize			= new AudioParameterFloat	("0x04", "Size",			0.0f,		1.0f,		0.75f);
		lfoFrequency		= new AudioParameterFloat	("0x05", "LFO Frequency",	0.0f,		1.0f,		0.50f);
		lfoDepth			= new AudioParameterFloat	("0x06", "LFO Depth",		0.0f,		200.0f,		40.0f);
		mix					= new AudioParameterFloat	("0x07", "Mix",				0.0f,		1.0f,		0.75f);

		addParameter(bypass);
		addParameter(reverbBrightness);
		addParameter(reverbDamping);
		addParameter(reverbDecay);
		addParameter(reverbSize);
		addParameter(lfoFrequency);
		addParameter(lfoDepth);
		addParameter(mix);
	}

	~FeedbackDelayNetwork()
	{
		Release();
	}

	void Process(float* buffer, uint32_t audio_block_size);
	void Reset();
	void Release();

private:
	FunctionGenerator wave1;
	FunctionGenerator wave2;
	FunctionGenerator wave3;
	FunctionGenerator wave4;

	CircularBuffer<float> cb1;
	CircularBuffer<float> cb2;
	CircularBuffer<float> cb3;
	CircularBuffer<float> cb4;

	CoefficientZ _coefficient;
	BiquadZ _biquad1;
	BiquadZ _biquad2;
	BiquadZ _biquad3;
	BiquadZ _biquad4;

	AudioParameterBool* bypass;
	AudioParameterFloat* reverbBrightness;
	AudioParameterFloat* reverbDamping;
	AudioParameterFloat* reverbDecay;
	AudioParameterFloat* reverbSize;
	AudioParameterFloat* lfoFrequency;
	AudioParameterFloat* lfoDepth;
	AudioParameterFloat* mix;
};

void FeedbackDelayNetwork::Process(float* buffer, uint32_t audio_block_size)
{
	if (!(bypass->value))
	{
		for (unsigned int index = 0; index < audio_block_size; index++)
		{
			float drySignal = buffer[index];

			float modulation1 = wave1.Generate(0, lfoFrequency->value) * lfoDepth->value;
			float modulation2 = wave2.Generate(0, lfoFrequency->value) * lfoDepth->value;
			float modulation3 = wave3.Generate(0, lfoFrequency->value) * lfoDepth->value;
			float modulation4 = wave4.Generate(0, lfoFrequency->value) * lfoDepth->value;

			float feedback1 = cb1.ReadBuffer((2819.0f + modulation1) * reverbSize->value);
			float feedback2 = cb2.ReadBuffer((3343.0f + modulation2) * reverbSize->value);
			float feedback3 = cb3.ReadBuffer((3581.0f + modulation3) * reverbSize->value);
			float feedback4 = cb4.ReadBuffer((4133.0f + modulation4) * reverbSize->value);

			_coefficient.setParameter(reverbBrightness->value, 0.0f, 0.0f, 0.0f);
			float* coefficients = _coefficient.getCoefficients();
			_biquad1._a1 = coefficients[1]; _biquad1._a2 = coefficients[2]; _biquad1._b0 = coefficients[3]; _biquad1._b1 = coefficients[4]; _biquad1._b2 = coefficients[5];
			_biquad2._a1 = coefficients[1]; _biquad2._a2 = coefficients[2]; _biquad2._b0 = coefficients[3]; _biquad2._b1 = coefficients[4]; _biquad2._b2 = coefficients[5];
			_biquad3._a1 = coefficients[1]; _biquad3._a2 = coefficients[2]; _biquad3._b0 = coefficients[3]; _biquad3._b1 = coefficients[4]; _biquad3._b2 = coefficients[5];
			_biquad4._a1 = coefficients[1]; _biquad4._a2 = coefficients[2]; _biquad4._b0 = coefficients[3]; _biquad4._b1 = coefficients[4]; _biquad4._b2 = coefficients[5];

			float lpf1 = _biquad1.ProcessSample(feedback1);
			float lpf2 = _biquad2.ProcessSample(feedback2);
			float lpf3 = _biquad3.ProcessSample(feedback3);
			float lpf4 = _biquad4.ProcessSample(feedback4);

			float damp1 = (lpf1 - feedback1) * reverbDamping->value;
			float damp2 = (lpf2 - feedback2) * reverbDamping->value;
			float damp3 = (lpf3 - feedback3) * reverbDamping->value;
			float damp4 = (lpf4 - feedback4) * reverbDamping->value;

			float A = (damp1 + feedback1) * 0.5f * (reverbDecay->value * 0.25 + 0.75) + drySignal;
			float B = (damp2 + feedback2) * 0.5f * (reverbDecay->value * 0.25 + 0.75) + drySignal;
			float C = (damp3 + feedback3) * 0.5f * (reverbDecay->value * 0.25 + 0.75);
			float D = (damp4 + feedback4) * 0.5f * (reverbDecay->value * 0.25 + 0.75);

			float output1 = A + B + C + D;
			float output2 = A - B + C - D;
			float output3 = A + B - C - D;
			float output4 = A - B - C + D;

			cb1.WriteBuffer(output1);
			cb2.WriteBuffer(output2);
			cb3.WriteBuffer(output3);
			cb4.WriteBuffer(output4);

			buffer[index] = (A + B + C + D) * mix->value + drySignal * (1 - mix->value);
		}
	}
}

void FeedbackDelayNetwork::Reset()
{
	wave1.FlushSample();
	wave2.FlushSample();
	wave3.FlushSample();
	wave4.FlushSample();

	wave1.cycle = 0;
	wave2.cycle = 0.25f;
	wave3.cycle = 0.50f;
	wave4.cycle = 0.75f;

	cb1.FlushBuffer();
	cb2.FlushBuffer();
	cb3.FlushBuffer();
	cb4.FlushBuffer();

	bypass->resetToDefaultValue();
	reverbBrightness->resetToDefaultValue();
	reverbDamping->resetToDefaultValue();
	reverbDecay->resetToDefaultValue();
	reverbSize->resetToDefaultValue();
	lfoFrequency->resetToDefaultValue();
	lfoDepth->resetToDefaultValue();
	mix->resetToDefaultValue();
}

void FeedbackDelayNetwork::Release()
{
	wave1.FlushSample();
	wave1.~FunctionGenerator();	
	wave2.FlushSample();
	wave2.~FunctionGenerator();	
	wave3.FlushSample();
	wave3.~FunctionGenerator();	
	wave4.FlushSample();
	wave4.~FunctionGenerator();

	cb1.~CircularBuffer();
	cb2.~CircularBuffer();
	cb3.~CircularBuffer();
	cb4.~CircularBuffer();

	delete bypass;
	bypass = NULL;

	delete reverbBrightness;
	reverbBrightness = NULL;

	delete reverbDamping;
	reverbDamping = NULL;

	delete reverbDecay;
	reverbDecay = NULL;

	delete reverbSize;
	reverbSize = NULL;

	delete lfoFrequency;
	lfoFrequency = NULL;

	delete lfoDepth;
	lfoDepth = NULL;

	delete mix;
	mix = NULL;

	AudioObject::Release();
}