#include <iostream>
#include <AudioParameterFloat.h>
#include <CircularBuffer.h>
#include <DigitalDelayLine.h>
#include <Oscillator.h>
#include <RangedAudioParameter.h>
#include <AudioParameterFloat.h>
#include <AudioObject.h>
#include <DopplerPitchShifter.h>
#include <DopplerOctave.h>
#include <FilterZ.h>
#include <ModulationFilter.h>
#include <FeedbackDelayNetwork.h>
#include <CrossConvolution.h>
#include <UnitGain.h>

#include "Vendor/MLD.hpp"
#include "Vendor/AudioFile.h"

void renderAlgorithm()
{
	//DigitalDelayLine dl0(262144, 0);
	//dl0.ParameterCtrl(1, 0);
	//dl0.ParameterCtrl(2, 64);
	//DigitalDelayLine dl1(262144, 0);
	//dl1.ParameterCtrl(1, 70);
	//dl1.ParameterCtrl(2, 80);

	//unsigned int NodeSize = 2;

	//AudioObject* audioNodes[8];
	//audioNodes[0] = &dl0;
	//audioNodes[1] = &dl1;

	//DopplerOctave obj(0);
	//Oscillator obj;
	//ModulationFilter obj;
	//FeedbackDelayNetwork obj(0);
	//CrossConvolution obj(0);
	UnitGain obj(0);

	unsigned int NodeSize = 1;

	AudioObject* audioNodes[8];
	audioNodes[0] = &obj;

	AudioFile<double> audioFile;
	audioFile.load("Extra/taipei-emperor-intro-mono.wav");
	std::cout << "Before Processing..." << std::endl;
	audioFile.printSummary();
	std::cout << "" << std::endl;

	int channels = audioFile.getNumChannels();
	int numSamples = audioFile.getNumSamplesPerChannel();

	int blockSize = 128;
	int iteration = (numSamples / blockSize) + 1;

	AudioFile<double>::AudioBuffer buffer;
	buffer.resize(channels);

	for (int i = 0; i < channels; i++)
	{
		buffer[i].resize(iteration * blockSize);
	}

	//for (int i = 0; i < iteration; i++)
	//{
	//	for (int channel = 0; channel < channels; channel++)
	//	{
	//		double* currentFrame = &audioFile.samples[channel][i];

	//		for (int index = 0; index < NodeSize; index++)
	//		{
	//			audioNodes[index]->Process(currentFrame, 128);
	//		}

	//		//buffer[channel][i] = currentSample;
	//	}
	//	std::cout << i << std::endl;
	//}

	for (int i = 0; i < iteration; i++)
	{
		double* currentFrame = &audioFile.samples[0][i*blockSize];
		for (int index = 0; index < NodeSize; index++)
		{
			audioNodes[index]->Process(currentFrame, blockSize);
		}

		std::cout << "Current Iteration: " << i << std::endl;

		for (int j = 0; j < blockSize; j++)
		{
			//std::cout << currentFrame[j] << std::endl;
			buffer[0][i * blockSize + j] = currentFrame[j];
		}
	}

	bool ok = audioFile.setAudioBuffer(buffer);
	audioFile.save("Render/render-output.wav", AudioFileFormat::Wave);
	std::cout << "After Processing..." << std::endl;
	audioFile.printSummary();
	std::cout << "" << std::endl;

	obj.Release();
}

void memoryLeakDetector()
{
	memoryld::memory_monitoring();

	//std::cout << "" << std::endl;
	//DigitalDelayLine dl(32768, 0);
	//dl.Release();

	//Oscillator osc;
	//osc.Release();

	//DopplerPitchShifter ps(0);
	//ps.Release();

	//DopplerOctave oct(0);
	//oct.Release();

	memoryld::memory_check();
}

void main()
{
	renderAlgorithm();
	//memoryLeakDetector();

	//FilterDesigner lp;
	//lp.model = 1;
	//lp.setParameter(1200, 0.707, 0);

	//float* coefficient = lp.getCoefficients();

	//std::cout << coefficient[0] << std::endl;
	//std::cout << coefficient[1] << std::endl;
	//std::cout << coefficient[2] << std::endl;
	//std::cout << coefficient[3] << std::endl;
	//std::cout << coefficient[4] << std::endl;
	//std::cout << coefficient[5] << std::endl;
}