#include <iostream>
#include <AudioParameterFloat.h>
#include <CircularBuffer.h>
#include <DigitalDelayLine.h>
#include <Oscillator.h>

#include "Vendor/MLD.hpp"
#include "Vendor/AudioFile.h"

void main()
{
	memoryld::memory_monitoring();

	DigitalDelayLine dl(32768, 0);
	dl.ParameterCtrl(0, 125);
	dl.ParameterCtrl(1, 63);
	dl.ParameterCtrl(2, 63);

	AudioFile<double> audioFile;
	audioFile.load("Extra/taipei-emperor-intro.wav");
	std::cout << "Before Processing..." << std::endl;
	audioFile.printSummary();

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
		for (int j = 0; j < channels; j++)
		{
			float currentSample = audioFile.samples[j][i];
			dl.Process(&currentSample, 1);
			buffer[j][i] = currentSample;
		}
	}

	bool ok = audioFile.setAudioBuffer(buffer);
	audioFile.save("Extra/render-output.wav", AudioFileFormat::Wave);
	std::cout << "After Processing..." << std::endl;
	audioFile.printSummary();
	
	dl.Release();

	memoryld::memory_check();
}