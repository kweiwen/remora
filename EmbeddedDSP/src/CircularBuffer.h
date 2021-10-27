#pragma once

#ifndef doLinearInterpolation
#define doLinearInterpolation (1)
#endif

#ifndef doHermitInterpolation
#define doHermitInterpolation (2)
#endif

#ifndef doLargrangeInterpolation
#define doLargrangeInterpolation (3)
#endif

typedef struct circularBuffer CircularBuffer;
struct circularBuffer
{
    float* _buffer;
    unsigned int _writeIndex;
    unsigned int _bufferLength;
    unsigned int _wrapMask;
    unsigned int _location;
    unsigned int _interpolation;

    void(*CreateBuffer)(CircularBuffer* self, unsigned int bufferLength, unsigned int location);
    void(*FlushBuffer)(CircularBuffer* self);
    float(*ReadBuffer)(CircularBuffer* self, float delayInSamples);
    float(*ReadBufferInterpolation)(CircularBuffer* self, float delayInFractionalSamples);
    void(*WriteBuffer)(CircularBuffer* self, float input);
};

CircularBuffer* CircularBuffer_NEW(void* address);
void CircularBuffer_DEL(CircularBuffer* self);
