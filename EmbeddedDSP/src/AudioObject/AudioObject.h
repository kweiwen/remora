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

		unsigned int buffer_size = 1024;
		unsigned int block_size = 1024/audio_block_size;

		float coefficient[buffer_size];
		coefficient_z = fft(coefficient, buffer_size);

		CircularBuffer cb(buffer_size);
		float cb_z[buffer_size];

		CircularBuffer block(block_size);

		float temp_z[buffer_size];
		float temp[block_size][buffer_size];

		void Process(float* buffer, uint32_t audio_block_size)
		{
			block.write(buffer);

			for(int index = 0; index < audio_block_size; index++)
	  		{
				cb.write(buffer[index]);
				buffer[index] = 0;
			}

			cb_z = fft(cb.buffer, size);
			temp_z = multiply(cb_z, coefficient_z, size);
			temp[block.writeindex] = ifft(temp_z, size);

			for(int i = 0; i < audio_block_size; i++)
			{
				for(int j = 0; j < block_size; j++)
	  			{
					buffer[i] = buffer[i] + temp[j];
				}
			}
		}

		====================================================
	*/
	virtual void Process(float* buffer, uint32_t audio_block_size) = 0;

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
