#pragma once

#include "CircularBuffer.h"
#include "AudioObject.h"
#include "../AudioParameter/AudioParameterInt.h"
#include "../AudioParameter/AudioParameterFloat.h"
#include "../AudioParameter/AudioParameterBool.h"

class DigitalDelayLine : public AudioObject
{
public:
	DigitalDelayLine(unsigned int bufferLength, unsigned int location = 0) : AudioObject(4)
	{
		digitalDelayLine.CreateBuffer(bufferLength, location);

		time 		= new AudioParameterInt   ("0x00", "time", 			1, 48000, 48000);
		feedback 	= new AudioParameterFloat ("0x01", "feedback", 		0.0f, 1.0f, 0.0f);
		mix 		= new AudioParameterFloat ("0x02", "mix", 			0.0f, 1.0f, 0.0f);
		bypass 		= new AudioParameterBool  ("0x03", "bypass",		false);

		addParameter(time);
		addParameter(feedback);
		addParameter(mix);
		addParameter(bypass);
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
	AudioParameterInt* time;
	AudioParameterFloat* feedback;
	AudioParameterFloat* mix;
	AudioParameterBool* bypass;
};

void DigitalDelayLine::Process(float* buffer, uint32_t audio_block_size)
{
	for(unsigned int index = 0; index < audio_block_size; index++)
	{
		float drySignal = buffer[index];
		float wetSignal = digitalDelayLine.ReadBufferInterpolation((float)time->value);
		digitalDelayLine.WriteBuffer(drySignal + wetSignal * (feedback->value));
		buffer[index] = wetSignal * (mix->value) * 20 + drySignal * (1 - (mix->value)) * 20;
	}
}

void DigitalDelayLine::Reset()
{
	digitalDelayLine.FlushBuffer();

	time->resetToDefaultValue();
	feedback->resetToDefaultValue();
	mix->resetToDefaultValue();
	bypass->resetToDefaultValue();
}

void DigitalDelayLine::Release()
{
	digitalDelayLine.~CircularBuffer();

	delete time;
	time = NULL;

	delete feedback;
	feedback = NULL;

	delete mix;
	mix = NULL;

	delete bypass;
	bypass = NULL;

	AudioObject::Release();
}
