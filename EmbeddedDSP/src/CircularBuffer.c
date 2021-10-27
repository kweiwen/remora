#include "CircularBuffer.h"
#include <stdlib.h>
#include <math.h>

static void __createBuffer__(CircularBuffer* self, unsigned int bufferLength, unsigned int location);
static void __flushBuffer__(CircularBuffer* self);
static void __writeBuffer__(CircularBuffer* self, float input);
static float __readBuffer__(CircularBuffer* self, float delayInSamples);
static float __readBufferInterpolation__(CircularBuffer* self, float delayInFractionalSamples);
static float __doLinearInterpolation__(CircularBuffer* self, float delayInFractionalSamples);
static float __doHermitInterpolation__(CircularBuffer* self, float delayInFractionalSamples);
static float __doLargrangeInterpolation__(CircularBuffer* self, float delayInFractionalSamples);

static void __createBuffer__(CircularBuffer* self, unsigned int bufferLength, unsigned int location)
{
    self->_bufferLength = (unsigned int)(powf(2, ceilf(logf(bufferLength) / logf(2))));
    self->_location = location;
    if(self->_location==0)
    {
    	self->_buffer = calloc(self->_bufferLength, sizeof(float));
    }
    else
    {
        self->_buffer = heap_calloc(self->_location, self->_bufferLength, sizeof(float));
    }

    self->_wrapMask = self->_bufferLength - 1;
    __flushBuffer__(self);
}

static void __flushBuffer__(CircularBuffer* self)
{
    for (unsigned int i = 0; i < self->_bufferLength; i++)
    {
        self->_buffer[i] = 0;
    }
}

static void __writeBuffer__(CircularBuffer* self, float input)
{
    self->_buffer[self->_writeIndex++] = input;
    self->_writeIndex &= self->_wrapMask;
}

static float __readBuffer__(CircularBuffer* self, float delayInSamples)
{
    int readIndex = self->_writeIndex - (int)delayInSamples;
    readIndex &= self->_wrapMask;

    return self->_buffer[readIndex];
}

static float __readBufferInterpolation__(CircularBuffer* self, float delayInFractionalSamples)
{
	if(self->_interpolation == doLinearInterpolation)
	{
		return __doLinearInterpolation__(self, delayInFractionalSamples);
	}
	else if(self->_interpolation == doHermitInterpolation)
	{
		return __doHermitInterpolation__(self, delayInFractionalSamples);
	}
	else if(self->_interpolation == doLargrangeInterpolation)
	{
		return __doLargrangeInterpolation__(self, delayInFractionalSamples);
	}
	else
	{
		return __readBuffer__(self, delayInFractionalSamples);
	}
}

static float __doLinearInterpolation__(CircularBuffer* self, float delayInFractionalSamples)
{
    float y1 = __readBuffer__(self, (int)delayInFractionalSamples);
    float y2 = __readBuffer__(self, (int)delayInFractionalSamples + 1);
    float fraction = delayInFractionalSamples - (int)delayInFractionalSamples;

    if (fraction >= 1.0) return y2;
    return fraction * y2 + (1 - fraction) * y1;
}

static float __doHermitInterpolation__(CircularBuffer* self, float delayInFractionalSamples)
{
    int index = (int)delayInFractionalSamples;
    float xm1 = __readBuffer__(self, index - 1);;
    float x0 = __readBuffer__(self, index);
    float x1 = __readBuffer__(self, index + 1);
    float x2 = __readBuffer__(self, index + 2);

    float frac_pos = delayInFractionalSamples - (int)delayInFractionalSamples;;

    const float c = (x1 - xm1) * 0.5f;
    const float v = x0 - x1;
    const float w = c + v;
    const float a = w + v + (x2 - x0) * 0.5f;
    const float b_neg = w + a;
    return ((((a * frac_pos) - b_neg) * frac_pos + c) * frac_pos + x0);
}


static float __doLargrangeInterpolation__(CircularBuffer* self, float delayInFractionalSamples)
{

    int n = 4;
    int index = (int)delayInFractionalSamples;
    float x[4] = { index - 1, index, index + 1, index + 2 };
    float y[4] = { __readBuffer__(self, index - 1), __readBuffer__(self, index),__readBuffer__(self, index + 1),__readBuffer__(self, index + 2) };

    float interpolation = 0;
    for (int i = 0; i < n; i++)
    {
        float term = y[i];
        for (int j = 0; j < n; j++)
        {
            if (j != i)
            {
                term = term * (delayInFractionalSamples - x[j]) / (float)(x[i] - x[j]);
            }
        }
        interpolation += term;
    }
    return interpolation;
}

CircularBuffer* CircularBuffer_NEW(void* address)
{
    if (address == NULL) return NULL;
    CircularBuffer* self = address;

    self->_buffer = NULL;
    self->_writeIndex = 0;
    self->_bufferLength = 0;
    self->_wrapMask = 0;
    self->_interpolation = 0;

    self->CreateBuffer = __createBuffer__;
    self->FlushBuffer = __flushBuffer__;
    self->ReadBuffer = __readBuffer__;
    self->ReadBufferInterpolation = __readBufferInterpolation__;
    self->WriteBuffer = __writeBuffer__;

    return self;
}

void CircularBuffer_DEL(CircularBuffer* self)
{
    if(self->_location==0)
    {
        free(self->_buffer);
    }
    else
    {
        heap_free(self->_location, self->_buffer);
    }
}
