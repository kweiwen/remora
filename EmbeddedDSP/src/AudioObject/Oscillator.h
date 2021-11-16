#pragma once

#include "AudioObject.h"
#include "FunctionGenerator.h"
#include "../AudioParameter/AudioParameterInt.h"
#include "../AudioParameter/AudioParameterFloat.h"
#include "../AudioParameter/AudioParameterChoice.h"
#include "../Utilities/Utils.h"

class Oscillator : public AudioObject
{
public:
	Oscillator() : AudioObject(3)
	{
		int oscillatorChoiceList[5] = {E_SINE, E_SAWTOOTH, E_TRIANGLE, E_TRAPEZOID, E_SQUARE};

		volume		= new AudioParameterFloat	("0x00", "Volume",	0.0f, 1.0f, 0.5f);
		pitch		= new AudioParameterFloat	("0x01", "Pitch",	69);
		type		= new AudioParameterChoice	("0x02", "Type",	5, oscillatorChoiceList, 0);

		addParameter(volume);
		addParameter(pitch);
		addParameter(type);
	}
	~Oscillator()
	{
		Release();
	}

	void Process(float* buffer, uint32_t audio_block_size);
	void Reset();
	void Release();

private:
	FunctionGenerator wave;

	AudioParameterFloat* volume;
	AudioParameterFloat* pitch;
	AudioParameterChoice* type;
};

void Oscillator::Process(float* buffer, uint32_t audio_block_size)
{
	for(unsigned int index = 0; index < audio_block_size; index++)
	{
		buffer[index] = wave.Generate((int)(type->value), (float)(pitch->value)) * volume->value;
	}
}

void Oscillator::Reset()
{
	wave.FlushSample();

	volume->resetToDefaultValue();
	pitch->resetToDefaultValue();
	type->resetToDefaultValue();
}

void Oscillator::Release()
{
	wave.FlushSample();
	wave.~FunctionGenerator();

	delete volume;
	volume = NULL;

	delete pitch;
	pitch = NULL;

	delete type;
	type = NULL;

	AudioObject::Release();
}


