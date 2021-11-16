#pragma once

#include "AudioObject.h"
#include "FunctionGenerator.h"
#include "CircularBuffer.h"
#include "../AudioParameter/AudioParameterInt.h"
#include "../AudioParameter/AudioParameterFloat.h"
#include "../AudioParameter/AudioParameterChoice.h"
#include "../Utilities/Utils.h"

class DopplerOctave : public AudioObject
{
public:
	DopplerOctave(unsigned int location = 0) : AudioObject(4)
	{
		/* The phase of two modulator is different, the volume of two pitch-shifted delay line will be compensated.
		 * Using Hann window to prevernt clicking sound.
		 */
		waveA1.cycle = 0.5f;
		waveB1.cycle = 0.0f;
		waveA2.cycle = 0.5f;
		waveB2.cycle = 0.0f;

		/* The base frequency is 20Hz, which is minimal audioable response of human hearing. */
		baseFrequency = 20;

		/* Larger base frequency could save the memory consuming, smaller base frequency would cost more memory.
		 * Depends on the implementation scenario.
		 */
		delayStretchFactor = SR / baseFrequency;

		frequency1 = ((powf(2, (12 / 12.0))) - 1) * -1 * baseFrequency;
		frequency2 = ((powf(2, (-12 / 12.0))) - 1) * -1 * baseFrequency;

		digitalDelayLineA1.CreateBuffer(delayStretchFactor, location);
		digitalDelayLineB1.CreateBuffer(delayStretchFactor, location);
		digitalDelayLineA2.CreateBuffer(delayStretchFactor, location);
		digitalDelayLineB2.CreateBuffer(delayStretchFactor, location);

		bypass			= new AudioParameterBool ("0x00", "bypass",					false);
		highPitchVol	= new AudioParameterFloat("0x01", "Higher Pitch Volume",	0.0f, 1.0f, 0.5f);
		lowPitchVol		= new AudioParameterFloat("0x02", "Lower Pitch Volume",		0.0f, 1.0f, 0.75f);
		mix				= new AudioParameterFloat("0x03", "Mix",					0.0f, 1.0f, 0.8f);

		addParameter(bypass);
		addParameter(highPitchVol);
		addParameter(lowPitchVol);
		addParameter(mix);
	}
	~DopplerOctave()
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
	FunctionGenerator waveA1;
	FunctionGenerator waveB1;
	FunctionGenerator waveA2;
	FunctionGenerator waveB2;

	AudioParameterBool* bypass;
	AudioParameterFloat* mix;
	AudioParameterFloat* highPitchVol;
	AudioParameterFloat* lowPitchVol;

	float baseFrequency;
	float delayStretchFactor;
	float frequency1;
	float frequency2;
};

void DopplerOctave::Process(float* buffer, uint32_t audio_block_size)
{
	if (!(bypass->value))
	{
		
		for (unsigned int index = 0; index < audio_block_size; index++)
		{
			float phasorA1 = waveA1.Generate(5, frequency1);
			float phasorB1 = waveB1.Generate(5, frequency1);
			float phasorA2 = waveA2.Generate(5, frequency2);
			float phasorB2 = waveB2.Generate(5, frequency2);

			float windowA1 = ((cosf(TWO_PI * phasorA1) * -1.0) + 1.0) / 2.0;
			float windowB1 = ((cosf(TWO_PI * phasorB1) * -1.0) + 1.0) / 2.0;
			float windowA2 = ((cosf(TWO_PI * phasorA2) * -1.0) + 1.0) / 2.0;
			float windowB2 = ((cosf(TWO_PI * phasorB2) * -1.0) + 1.0) / 2.0;

			float timeA1 = phasorA1 * delayStretchFactor;
			float timeB1 = phasorB1 * delayStretchFactor;			
			float timeA2 = phasorA2 * delayStretchFactor;
			float timeB2 = phasorB2 * delayStretchFactor;

			float phaseA1 = digitalDelayLineA1.ReadBufferInterpolation(timeA1, 1) * windowA1;
			float phaseB1 = digitalDelayLineB1.ReadBufferInterpolation(timeB1, 1) * windowB1;
			float phaseA2 = digitalDelayLineA2.ReadBufferInterpolation(timeA2, 1) * windowA2;
			float phaseB2 = digitalDelayLineB2.ReadBufferInterpolation(timeB2, 1) * windowB2;

			digitalDelayLineA1.WriteBuffer(buffer[index]);
			digitalDelayLineB1.WriteBuffer(buffer[index]);			
			digitalDelayLineA2.WriteBuffer(buffer[index]);
			digitalDelayLineB2.WriteBuffer(buffer[index]);

			float temp = (phaseA1 + phaseB1) * highPitchVol->value + (phaseA2 + phaseB2) * lowPitchVol->value;

			buffer[index] = temp * mix->value + buffer[index] * (1 - mix->value);
		}
	}
}

void DopplerOctave::Reset()
{
	waveA1.FlushSample();
	waveB1.FlushSample();	
	waveA2.FlushSample();
	waveB2.FlushSample();

	waveA1.cycle = 0.5f;
	waveA1.cycle = 0.0f;	
	waveA2.cycle = 0.5f;
	waveA2.cycle = 0.0f;

	bypass->resetToDefaultValue();
	highPitchVol->resetToDefaultValue();
	lowPitchVol->resetToDefaultValue();
	mix->resetToDefaultValue();
}

void DopplerOctave::Release()
{
	waveA1.FlushSample();
	waveA1.~FunctionGenerator();
	waveB1.FlushSample();
	waveB1.~FunctionGenerator();
	waveA2.FlushSample();
	waveA2.~FunctionGenerator();
	waveB2.FlushSample();
	waveB2.~FunctionGenerator();

	digitalDelayLineA1.~CircularBuffer();
	digitalDelayLineB1.~CircularBuffer();
	digitalDelayLineA2.~CircularBuffer();
	digitalDelayLineB2.~CircularBuffer();

	delete bypass;
	bypass = NULL;

	delete highPitchVol;
	highPitchVol = NULL;
	
	delete lowPitchVol;
	lowPitchVol = NULL;
	
	delete mix;
	mix = NULL;

	AudioObject::Release();
}
