#pragma once

#include <stdlib.h>
#include <math.h>

template <typename ValueType>
class CircularBuffer
{
public:
	CircularBuffer();
	CircularBuffer(unsigned int bufferLength, unsigned int location = 0);
	~CircularBuffer();

	void CreateBuffer(unsigned int bufferLength, unsigned int location);
	void FlushBuffer(void);
	ValueType ReadBuffer(unsigned int delayInSamples);
	float ReadBufferInterpolation(float delayInFractionalSamples);
	void WriteBuffer(ValueType input);

private:
	ValueType* _buffer;
	unsigned int _location;
	unsigned int _writeIndex;
	unsigned int _bufferLength;
	unsigned int _wrapMask;

	float doLinearInterpolation(float delayInFractionalSamples);
	float doHermitInterpolation(float delayInFractionalSamples);
	float doLargrangeInterpolation(float delayInFractionalSamples);
};

template <typename ValueType>
CircularBuffer<ValueType>::CircularBuffer()
{
	_buffer = NULL;
	_writeIndex = 0;
	_bufferLength = 0;
	_wrapMask = 0;
	_location = 0;
}

template <typename ValueType>
CircularBuffer<ValueType>::CircularBuffer(unsigned int bufferLength, unsigned int location)
{
	_buffer = NULL;
	_writeIndex = 0;
	_bufferLength = 0;
	_wrapMask = 0;
	_location = location;

	CreateBuffer(bufferLength, location);
}


#ifdef _WIN32
template <typename ValueType>
CircularBuffer<ValueType>::~CircularBuffer()
{
    delete[] _buffer;
    _buffer = NULL;
}
#else
template <typename ValueType>
CircularBuffer<ValueType>::~CircularBuffer()
{
	heap_free(_location, _buffer);
}
#endif

#ifdef _WIN32
template <typename ValueType>
void CircularBuffer<ValueType>::CreateBuffer(unsigned int bufferLength, unsigned int location)
{
    _bufferLength = (unsigned int)(powf(2, ceilf(logf((float)bufferLength) / logf(2))));
    _buffer = new ValueType[_bufferLength];
    _wrapMask = _bufferLength - 1;
    FlushBuffer();
}
#else
template <typename ValueType>
void CircularBuffer<ValueType>::CreateBuffer(unsigned int bufferLength, unsigned int location)
{
    _bufferLength = (unsigned int)(powf(2, ceilf(logf(bufferLength) / logf(2))));
    _buffer = (ValueType*)heap_calloc(location, _bufferLength, sizeof(ValueType));
    _wrapMask = _bufferLength - 1;
    FlushBuffer();
}
#endif


template <typename ValueType>
void CircularBuffer<ValueType>::FlushBuffer()
{
    for (unsigned int i = 0; i < _bufferLength; i++)
    {
        _buffer[i] = 0;
    }
}

template <typename ValueType>
void CircularBuffer<ValueType>::WriteBuffer(ValueType input)
{
    _buffer[_writeIndex++] = input;
    _writeIndex &= _wrapMask;
}

template <typename ValueType>
ValueType CircularBuffer<ValueType>::ReadBuffer(unsigned int delayInSamples)
{
    int readIndex = _writeIndex - (int)delayInSamples;
    readIndex &= _wrapMask;

    return _buffer[readIndex];
}

template <typename ValueType>
float CircularBuffer<ValueType>::ReadBufferInterpolation(float delayInFractionalSamples)
{
    return doLinearInterpolation(delayInFractionalSamples);
}

template <typename ValueType>
float CircularBuffer<ValueType>::doLinearInterpolation(float delayInFractionalSamples)
{
    float y1 = ReadBuffer((int)delayInFractionalSamples);
    float y2 = ReadBuffer((int)delayInFractionalSamples + 1);
    float fraction = delayInFractionalSamples - (int)delayInFractionalSamples;

    if (fraction >= 1.0) return y2;
    return fraction * y2 + (1 - fraction) * y1;
}

template <typename ValueType>
float CircularBuffer<ValueType>::doHermitInterpolation(float delayInFractionalSamples)
{
    int index = (int)delayInFractionalSamples;
    float xm1 = ReadBuffer(index - 1);;
    float x0 = ReadBuffer(index);
    float x1 = ReadBuffer(index + 1);
    float x2 = ReadBuffer(index + 2);

    float frac_pos = delayInFractionalSamples - (int)delayInFractionalSamples;;

    const float c = (x1 - xm1) * 0.5f;
    const float v = x0 - x1;
    const float w = c + v;
    const float a = w + v + (x2 - x0) * 0.5f;
    const float b_neg = w + a;
    return ((((a * frac_pos) - b_neg) * frac_pos + c) * frac_pos + x0);
}

template <typename ValueType>
float CircularBuffer<ValueType>::doLargrangeInterpolation(float delayInFractionalSamples)
{

    int n = 4;
    int index = (int)delayInFractionalSamples;
    int x[4] = { index - 1, index, index + 1, index + 2 };
    float y[4] = { ReadBuffer(index - 1), ReadBuffer(index), ReadBuffer(index + 1), ReadBuffer(index + 2) };

    float interpolation = 0;
    for (int i = 0; i < n; i++)
    {
        float temp = y[i];
        for (int j = 0; j < n; j++)
        {
            if (j != i)
            {
            	temp = temp * (delayInFractionalSamples - x[j]) / (float)(x[i] - x[j]);
            }
        }
        interpolation += temp;
    }
    return interpolation;
}
