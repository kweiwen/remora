#pragma once

#include "AudioObject.h"
#include "CircularBuffer.h"
#include "CooleyTukeyFFT.h"
#include "../AudioParameter/AudioParameterInt.h"
#include "../AudioParameter/AudioParameterFloat.h"
#include "../AudioParameter/AudioParameterChoice.h"
#include "../Utilities/Utils.h"

class UnitGain : public AudioObject
{
public:
	UnitGain(unsigned int location = 0) : AudioObject(2)
	{
		bypass		= new AudioParameterBool ("0x00", "Bypass",		false);
		polarity	= new AudioParameterBool ("0x01", "Polarity",	false);
		gain		= new AudioParameterFloat("0x02", "Gain",		0.0f, 1.0f, 0.8f);

		addParameter(bypass);
		addParameter(polarity);
		addParameter(gain);
	}
	~UnitGain()
	{
		Release();
	}

	void Process(double* buffer, uint32_t audio_block_size);
	void Reset();
	void Release();

private:
	AudioParameterBool* bypass;
	AudioParameterBool* polarity;
	AudioParameterFloat* gain;
};

void UnitGain::Process(double* buffer, uint32_t audio_block_size)
{
	float inverse;
	if (polarity->value)
	{
		inverse = -1.0f;
	}
	else
	{
		inverse = 1.0f;
	}

	if (!(bypass->value))
	{
		for (int index = 0; index < audio_block_size; index++)
		{

			buffer[index] = buffer[index] * gain->value * inverse;
		}
	}
}

void UnitGain::Reset()
{
	bypass->resetToDefaultValue();
	polarity->resetToDefaultValue();
	gain->resetToDefaultValue();
}

void UnitGain::Release()
{
	delete bypass;
	bypass = NULL;

	delete polarity;
	polarity = NULL;

	delete gain;
	gain = NULL;

	AudioObject::Release();
}
