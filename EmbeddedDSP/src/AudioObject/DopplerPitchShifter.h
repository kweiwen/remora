#pragma once

#include "AudioObject.h"
#include "FunctionGenerator.h"
#include "CircularBuffer.h"
#include "../AudioParameter/AudioParameterInt.h"
#include "../AudioParameter/AudioParameterFloat.h"
#include "../AudioParameter/AudioParameterChoice.h"
#include "../Utilities/Utils.h"

class DopplerPitchShifter : public AudioObject
{
public:
	DopplerPitchShifter(unsigned int location = 0) : AudioObject(3)
	{
		/* The phase of two modulator is different, the volume of two pitch-shifted delay line will be compensated. 
		 * Using Hann window to prevernt clicking sound. 
		 */
		waveA.cycle = 0.5f;
		waveB.cycle = 0.0f;

		/* The base frequency is 20Hz, which is minimal audioable response of human hearing. */
		baseFrequency = 20;

		/* Larger base frequency could save the memory consuming, smaller base frequency would cost more memory. 
		 * Depends on the implementation scenario. 
		 */
		delayStretchFactor = SR / baseFrequency;

		digitalDelayLineA.CreateBuffer(delayStretchFactor, location);
		digitalDelayLineB.CreateBuffer(delayStretchFactor, location);

		bypass		= new AudioParameterBool	("0x00", "Bypass",	 false);
		pitch		= new AudioParameterFloat	("0x01", "Pitch",	-12.0f, 12.0f,	12.0f);
		mix			= new AudioParameterFloat	("0x02", "Mix",		 0.0f,	1.0f,	0.5f);

		addParameter(bypass);
		addParameter(pitch);
		addParameter(mix);
	}
	~DopplerPitchShifter()
	{
		Release();
	}

	void Process(float* buffer, uint32_t audio_block_size);
	void Reset();
	void Release();

private:
	CircularBuffer<float> digitalDelayLineA;
	CircularBuffer<float> digitalDelayLineB;
	FunctionGenerator waveA;
	FunctionGenerator waveB;

	AudioParameterBool* bypass;
	AudioParameterFloat* pitch;
	AudioParameterFloat* mix;

	float baseFrequency;
	float delayStretchFactor;
};

void DopplerPitchShifter::Process(float* buffer, uint32_t audio_block_size)
{
	if (!(bypass->value))
	{
		float frequency = ((powf(2, (pitch->value / 12.0))) - 1) * -1 * baseFrequency;
		for(unsigned int index = 0; index < audio_block_size; index++)
		{
			float phasorA = waveA.Generate(5, frequency);
			float phasorB = waveB.Generate(5, frequency);

			float windowA = ((cosf(TWO_PI * phasorA) * -1.0) + 1.0) / 2.0;
			float windowB = ((cosf(TWO_PI * phasorB) * -1.0) + 1.0) / 2.0;

			float timeA = phasorA * delayStretchFactor;
			float timeB = phasorB * delayStretchFactor;

			float phaseA = digitalDelayLineA.ReadBufferInterpolation(timeA, 1) * windowA;
			float phaseB = digitalDelayLineB.ReadBufferInterpolation(timeB, 1) * windowB;

			digitalDelayLineA.WriteBuffer(buffer[index]);
			digitalDelayLineB.WriteBuffer(buffer[index]);

			buffer[index] = (phaseA + phaseB) * mix->value + buffer[index] * (1 - mix->value);
		}
	}
}

void DopplerPitchShifter::Reset()
{
	waveA.FlushSample();
	waveB.FlushSample();

	waveA.cycle = 0.5f;
	waveA.cycle = 0.0f;

	bypass->resetToDefaultValue();
	pitch->resetToDefaultValue();
	mix->resetToDefaultValue();
}

void DopplerPitchShifter::Release()
{
	waveA.FlushSample();
	waveA.~FunctionGenerator();
	waveB.FlushSample();
	waveB.~FunctionGenerator();

	digitalDelayLineA.~CircularBuffer();
	digitalDelayLineB .~CircularBuffer();

	delete bypass;
	bypass = NULL;

	delete pitch;
	pitch = NULL;

	delete mix;
	mix = NULL;

	AudioObject::Release();
}
