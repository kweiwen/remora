#pragma once

#include "AudioObject.h"
#include "CircularBuffer.h"
#include "CooleyTukeyFFT.h"
#include "../AudioParameter/AudioParameterInt.h"
#include "../AudioParameter/AudioParameterFloat.h"
#include "../AudioParameter/AudioParameterChoice.h"
#include "../Utilities/Utils.h"

class CrossConvolution : public AudioObject
{
public:
	CrossConvolution(unsigned int location = 0) : AudioObject(2)
	{
		inputBuffer.CreateBuffer(2048, location);
		outputBuffer.CreateBuffer(2048, location);

		fftSize = 2048;
		offset = fftSize - 128;

		data_td_real = new double[fftSize];
		data_td_imag = new double[fftSize];
		data_fd_real = new double[fftSize];
		data_fd_imag = new double[fftSize];

		impedance_1_td_real = new double[fftSize];
		impedance_1_td_imag = new double[fftSize];
		impedance_2_td_real = new double[fftSize];
		impedance_2_td_imag = new double[fftSize];

		impedance_1_fd_real = new double[fftSize];
		impedance_1_fd_imag = new double[fftSize];
		impedance_2_fd_real = new double[fftSize];
		impedance_2_fd_imag = new double[fftSize];

		for (int i = 0; i < fftSize; i++)
		{
			impedance_1_td_real[i] = impedance_1[i];
			impedance_2_td_real[i] = impedance_2[i];
			impedance_1_td_imag[i] = 0.0f;
			impedance_2_td_imag[i] = 0.0f;

			data_td_real[i] = 0.0f;
			data_td_imag[i] = 0.0f;
			data_fd_real[i] = 0.0f;
			data_fd_imag[i] = 0.0f;
		}

		instance.fft_normalize(impedance_1_td_real, impedance_1_td_imag, fftSize, impedance_1_fd_real, impedance_1_fd_imag);
		instance.fft_normalize(impedance_2_td_real, impedance_2_td_imag, fftSize, impedance_2_fd_real, impedance_2_fd_imag);

		bypass			= new AudioParameterBool ("0x00", "Bypass", false);
		mix				= new AudioParameterFloat("0x01", "Mix",	0.0f, 1.0f, 0.8f);

		addParameter(bypass);
		addParameter(mix);
	}
	~CrossConvolution()
	{
		Release();
	}

	void Process(double* buffer, uint32_t audio_block_size);
	void Reset();
	void Release();

private:
	CircularBuffer<double> inputBuffer;
	CircularBuffer<double> outputBuffer;

	AudioParameterBool* bypass;
	AudioParameterFloat* mix;

	int offset;
	int fftSize;

	double* data_td_real;
	double* data_td_imag;
	double* data_fd_real;
	double* data_fd_imag;

	double impedance_1[2048] =
	{
		#include "../../../RIRs/impedance_1.dat"
	};

	double impedance_2[2048] =
	{
		#include "../../../RIRs/impedance_2.dat"
	};

	double* impedance_1_td_real;
	double* impedance_1_td_imag;
	double* impedance_2_td_real;
	double* impedance_2_td_imag;

	double* impedance_1_fd_real;
	double* impedance_1_fd_imag;
	double* impedance_2_fd_real;
	double* impedance_2_fd_imag;

	twiddle instance;
};

void CrossConvolution::Process(double* buffer, uint32_t audio_block_size)
{
	if (!(bypass->value))
	{
		for (int i = 0; i < audio_block_size; i++)
		{
			inputBuffer.WriteBuffer(buffer[i]);
		}

		// copy input buffer to fft buffer
		for (int i = 0; i < fftSize; i++)
		{
			data_td_real[i] = inputBuffer.ReadBuffer(fftSize - i);
		}

		////convert to fd data
		instance.fft_normalize(data_td_real, data_td_imag, fftSize, data_fd_real, data_fd_imag);

		for (int i = 0; i < fftSize; i++)
		{
			auto real1 = data_fd_real[i] * impedance_1_fd_real[i] - data_fd_imag[i] * impedance_1_fd_imag[i];
			auto imag1 = data_fd_imag[i] * impedance_1_fd_real[i] + data_fd_real[i] * impedance_1_fd_imag[i];
			auto real2 = data_fd_real[i] * impedance_2_fd_real[i] - data_fd_imag[i] * impedance_2_fd_imag[i];
			auto imag2 = data_fd_imag[i] * impedance_2_fd_real[i] + data_fd_real[i] * impedance_2_fd_imag[i];

			data_fd_real[i] = real1 * mix->value + real1 * (1 - mix->value);
			data_fd_imag[i] = imag1 * mix->value + imag2 * (1 - mix->value);
		}

		//convert back to td data
		instance.ifft_normalize(data_fd_real, data_fd_imag, fftSize, data_td_real, data_td_imag);

		for (int i = 0; i < audio_block_size; i++)
		{
			buffer[i] = data_td_real[i + offset];
		}
	}
}

void CrossConvolution::Reset()
{
	bypass->resetToDefaultValue();
	mix->resetToDefaultValue();
}

void CrossConvolution::Release()
{
	inputBuffer.~CircularBuffer();
	outputBuffer.~CircularBuffer();

	delete bypass;
	bypass = NULL;

	delete mix;
	mix = NULL;

	AudioObject::Release();
}
