#pragma once

#include <stdint.h>
#include "CircularBuffer.h"
#include "AudioObject.h"
#include "../AudioParameter/AudioParameterInt.h"
#include "../AudioParameter/AudioParameterFloat.h"
#include "../AudioParameter/AudioParameterBool.h"
#include "../Utilities/AudioPreference.h"

class DigitalDelayLine : public AudioObject
{
public:
	DigitalDelayLine(unsigned int bufferLength, unsigned int location = 0) : AudioObject(4)
	{
		digitalDelayLine.CreateBuffer(bufferLength, location);

		bypass 		= new AudioParameterBool  ("0x00", "Bypass",		false);
		time 		= new AudioParameterInt   ("0x01", "Time", 			1,		(bufferLength - 1),		(bufferLength - 1));
		feedback 	= new AudioParameterFloat ("0x02", "Feedback", 		0.0f,	1.0f,	0.0f);
		mix 		= new AudioParameterFloat ("0x03", "Mix", 			0.0f,	1.0f,	0.0f);

		addParameter(bypass);
		addParameter(time);
		addParameter(feedback);
		addParameter(mix);
	}

	~DigitalDelayLine()
	{
		Release();
	}

	void Process(float* buffer, uint32_t audio_block_size);
	void Reset();
	void Release();

private:
	CircularBuffer<float> digitalDelayLine;
	AudioParameterBool* bypass;
	AudioParameterInt* time;
	AudioParameterFloat* feedback;
	AudioParameterFloat* mix;
};

void DigitalDelayLine::Process(float* buffer, uint32_t audio_block_size)
{
	if (!(bypass->value))
	{
		for(unsigned int index = 0; index < audio_block_size; index++)
		{
			float drySignal = buffer[index];
			float wetSignal = digitalDelayLine.ReadBufferInterpolation((float)time->value, 0);
			digitalDelayLine.WriteBuffer(drySignal + wetSignal * (feedback->value));
			buffer[index] = wetSignal * (mix->value) + drySignal * (1 - (mix->value));
		}
	}
}

void DigitalDelayLine::Reset()
{
	digitalDelayLine.FlushBuffer();

	bypass->resetToDefaultValue();
	time->resetToDefaultValue();
	feedback->resetToDefaultValue();
	mix->resetToDefaultValue();
}

void DigitalDelayLine::Release()
{
	digitalDelayLine.~CircularBuffer();

	delete bypass;
	bypass = NULL;

	delete time;
	time = NULL;

	delete feedback;
	feedback = NULL;

	delete mix;
	mix = NULL;

	AudioObject::Release();
}
