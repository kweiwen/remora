#pragma once

#include "AudioObject.h"
#include "CircularBuffer.h"
#include "../AudioParameter/AudioParameterInt.h"
#include "../AudioParameter/AudioParameterFloat.h"
#include "../AudioParameter/AudioParameterChoice.h"
#include "../Utilities/Utils.h"

class CrossConvolution : public AudioObject
{
public:
	CrossConvolution(unsigned int location = 0) : AudioObject(4)
	{
		digitalDelayLineA1.CreateBuffer(delayStretchFactor, location);
		digitalDelayLineB1.CreateBuffer(delayStretchFactor, location);
		digitalDelayLineA2.CreateBuffer(delayStretchFactor, location);
		digitalDelayLineB2.CreateBuffer(delayStretchFactor, location);

		bypass			= new AudioParameterBool ("0x00", "Bypass",					false);
		highPitchVol	= new AudioParameterFloat("0x01", "Higher Pitch Volume",	0.0f, 1.0f, 0.5f);
		lowPitchVol		= new AudioParameterFloat("0x02", "Lower Pitch Volume",		0.0f, 1.0f, 0.75f);
		mix				= new AudioParameterFloat("0x03", "Mix",					0.0f, 1.0f, 0.8f);

		addParameter(bypass);
		addParameter(highPitchVol);
		addParameter(lowPitchVol);
		addParameter(mix);
	}
	~CrossConvolution()
	{
		Release();
	}

	void Process(float* buffer, uint32_t audio_block_size);
	void Reset();
	void Release();

private:
	CircularBuffer<float> digitalDelayLineA1;
	CircularBuffer<float> digitalDelayLineB1;
	CircularBuffer<float> digitalDelayLineA2;
	CircularBuffer<float> digitalDelayLineB2;

	AudioParameterBool* bypass;
	AudioParameterFloat* mix;
	AudioParameterFloat* highPitchVol;
	AudioParameterFloat* lowPitchVol;

	float baseFrequency;
	float delayStretchFactor;
	float frequency1;
	float frequency2;
};

void CrossConvolution::Process(float* buffer, uint32_t audio_block_size)
{
	if (!(bypass->value))
	{
		for (unsigned int index = 0; index < audio_block_size; index++)
		{
			digitalDelayLineA1.WriteBuffer(buffer[index]);
			digitalDelayLineB1.WriteBuffer(buffer[index]);			
			digitalDelayLineA2.WriteBuffer(buffer[index]);
			digitalDelayLineB2.WriteBuffer(buffer[index]);
		}
	}
}

void CrossConvolution::Reset()
{
	bypass->resetToDefaultValue();
	highPitchVol->resetToDefaultValue();
	lowPitchVol->resetToDefaultValue();
	mix->resetToDefaultValue();
}

void CrossConvolution::Release()
{
	digitalDelayLineA1.~CircularBuffer();
	digitalDelayLineB1.~CircularBuffer();
	digitalDelayLineA2.~CircularBuffer();
	digitalDelayLineB2.~CircularBuffer();

	AudioObject::Release();
}
