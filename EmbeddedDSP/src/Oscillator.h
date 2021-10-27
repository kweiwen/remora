#pragma once

#ifndef M_TWO_PI
#define M_TWO_PI (6.283185307179586476925286766559)
#endif

#ifndef M_PI
#define M_PI (3.1415926535897932384626433832795)
#endif

enum E_OSCILLATOR_TYPE
{
    E_SINE          = 0,
    E_TRIANGLE      = 1,
    E_SAWTOOTH      = 2,
    E_TRAPEZOID     = 3,
    E_SQUARE        = 4,
    E_PHASOR        = 5,
	E_HANN          = 6,
	E_PULSE 		= 7,
};


typedef struct oscillator Oscillator;
struct oscillator
{
    float currentSample;
    float currentAngle;
    float counter;

    float pulseWidth;

    float(*Process)(Oscillator* self, float frequency, float sampleRate, int model, float offset);
};

Oscillator* Oscillator_NEW(void* address);
void Oscillator_DEL(Oscillator* self);
