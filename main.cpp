#include <iostream>
#include <AudioParameterFloat.h>
#include <CircularBuffer.h>
#include <DigitalDelayLine.h>
#include <Oscillator.h>
#include <RangedAudioParameter.h>
#include <AudioParameterFloat.h>
#include <AudioObject.h>

#include "Vendor/MLD.hpp"
#include "Vendor/AudioFile.h"

void renderAlgorithm()
{
	DigitalDelayLine dl0(262144, 0);
	dl0.ParameterCtrl(1, 0);
	dl0.ParameterCtrl(2, 64);
	DigitalDelayLine dl1(262144, 0);
	dl1.ParameterCtrl(1, 70);
	dl1.ParameterCtrl(2, 80);

	unsigned int NodeSize = 2;

	AudioObject* audioNodes[8];
	audioNodes[0] = &dl0;
	audioNodes[1] = &dl1;

	AudioFile<double> audioFile;
	audioFile.load("Extra/taipei-emperor-intro-mono.wav");
	std::cout << "Before Processing..." << std::endl;
	audioFile.printSummary();
	std::cout << "" << std::endl;

	int channels = audioFile.getNumChannels();
	int numSamples = audioFile.getNumSamplesPerChannel();

	AudioFile<double>::AudioBuffer buffer;
	buffer.resize(channels);

	for (int i = 0; i < channels; i++)
	{
		buffer[i].resize(numSamples);
	}

	
	for (int i = 0; i < numSamples; i++)
	{
		for (int channel = 0; channel < channels; channel++)
		{
			float currentSample = audioFile.samples[channel][i];

			for (int index = 0; index < NodeSize; index++)
			{
				audioNodes[index]->Process(&currentSample, 1);
			}

			buffer[channel][i] = currentSample;
		}
	}

	bool ok = audioFile.setAudioBuffer(buffer);
	audioFile.save("Render/render-output.wav", AudioFileFormat::Wave);
	std::cout << "After Processing..." << std::endl;
	audioFile.printSummary();
	std::cout << "" << std::endl;

	dl0.Release();
	dl1.Release();
}

void memoryLeakDetector()
{
	memoryld::memory_monitoring();

	std::cout << "" << std::endl;
	DigitalDelayLine dl(32768, 0);
	dl.Release();

	memoryld::memory_check();
}

void main()
{
	renderAlgorithm();
	memoryLeakDetector();
}