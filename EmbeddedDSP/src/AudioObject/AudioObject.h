#pragma once

#include "../AudioParameter/AudioParameter.h"
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#include "../Utilities/Utils.h"

class AudioObject
{
public:
	AudioObject(unsigned int size)
	{
		if(size == 0)
		{
			ParameterSize = 0;
			LocalParameter = NULL;
		}
		else
		{
			ParameterSize = size;
			LocalParameter = new AudioParameter * [ParameterSize];
		}
	}

	virtual ~AudioObject()
	{
		Release();
	}

	void PrepareToProcess()
	{
	}

	/** to ensure derived classes will implement this function, the primary archtechure will be block process.

		====================================================

	  	To Process by Sample(gain as example):

		float gain = 1.2;

		void Process(float* buffer, uint32_t audio_block_size)
		{
			for(int index = 0; index < audio_block_size; index++)
	  		{
	  			buffer[index] = buffer[index] * gain;
	  		}
		}

		====================================================

		To Process by Block(circular convolution as example):

        # fftSize should be greater than or equal to audio_block_size
		unsigned int fftSize ;

        # convert time domain data into frequency domain data
		float hn[fftSize];
		Hn = fft(hn, fftSize);

        # FIFO circular buffer
		CircularBuffer xn(fftSize);

	    # array to place Xn
		float Xn[fftSize];

	    # array to place frequency domain data, Yn = Xn * Hn
		float Yn[fftSize];

	    # array to hold time domain data, converted from Yn.
	    float yn[fftSize];

		void Process(float* buffer, uint32_t audio_block_size)
		{
			for(int index = 0; index < audio_block_size; index++)
	  		{
	  		    #  xn appends one block of data in every process
				xn.write(buffer[index]);
			}
            # time0: 0  0  0  B0
	 	    # time1: 0  0  B0 B1
	        # time2: 0  B0 B1 B2
	        # time3: B0 B1 B2 B3
	        # time4: B1 B2 B3 B4

            # xn holds time domain, real number only, use rfft to transform xn into Xn
			Xn = rfft(xn.read(), fftSize);

			# Xn: complex number, size in fftSize
			# Hn: complex number, size in fftSize
			# multiply two array in same size
            # Yn: complex number, size in fftSize
			Yn = complex_multiply(Xn, Hn, fftSize);

			# adopt rfft method to convert frequency domain data into time domain, real number
			yn = irfft(Yn, fftSize);

			for(int index = 0; index < audio_block_size; index++)
			{
			    # yn contains fftSize data, but takes one block data from yn only
			    buffer[index] = yn[index];
			}
		}

		====================================================
	*/
	virtual void Process(double* buffer, uint32_t audio_block_size) = 0;

	/** to ensure derived class will implement this function, parameter and process buffer will be reset to default
	    value. We suggest any AudioObjects conatin BPM/Delay Time/Modulation... properties should restore all time variant
		memebr in order to align multiple phase at same time.
	*/
	virtual void Reset() = 0;

	/** to ensure derived class will implement this function. */
	virtual void Release();

	void ParameterCtrl(unsigned int index, unsigned int value);
	unsigned int ParameterDisp(unsigned int index);

protected:
	void addParameter(AudioParameter* parameter);

private:
	/** the total number of parameter */
	unsigned int ParameterSize;

	AudioParameter** LocalParameter;
};

void AudioObject::addParameter(AudioParameter* parameter)
{
	int index = Utils::HexsToInt(parameter->paramID);
	assert(index >= 0);
	assert(index < (int)ParameterSize);
	LocalParameter[index] = parameter;
}

void AudioObject::ParameterCtrl(unsigned int index, unsigned int value)
{
	*(LocalParameter[index]) = value;
}

unsigned int AudioObject::ParameterDisp(unsigned int index)
{
	return LocalParameter[index]->cc_value;
}

void AudioObject::Release()
{
	delete[] LocalParameter;
	LocalParameter = NULL;
}
