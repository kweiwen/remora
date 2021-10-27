#include "Oscillator.h"
#include <math.h>
#include <stdlib.h>

static float __process__(Oscillator* self, float frequency, float sampleRate, int model, float offset);

float __sine__(float angle, float offset)
{
	if(angle>M_PI)
	{
		angle = angle - M_TWO_PI;
	}

	float x2 = angle * angle;
	float numerator = -angle * (-11511339840 + x2 * (1640635920 + x2 * (-52785432 + x2 * 479249)));
	float denominator = 11511339840 + x2 * (277920720 + x2 * (3177720 + x2 * 18361));
    return numerator / denominator;
//    return sinf(angle + offset * M_TWO_PI);
}

float __triangle__(float angle, float offset)
{
    return (4 / M_TWO_PI) * asinf(sinf(angle + offset * M_TWO_PI));
}

float __sawtooth__(float angle, float offset)
{
    float param, output_data, intpart, fractpart;
    param = (angle / M_TWO_PI) + offset;
    fractpart = modff(param, &intpart);
    output_data = (fractpart - 0.5) * 2;
    return output_data;
}

float __sawtooth_inv__(float angle, float offset)
{
    float param, output_data, intpart, fractpart;
    param = (angle / M_TWO_PI) + offset;
    fractpart = modff(param, &intpart);
    output_data = ((fractpart * -1) + 0.5) * 2;
    return output_data;
}

float __trapezoid__(float angle, float offset)
{
    float output_data = 0;
    for (int index = 0; index < 16; index++)
    {
        int coefficeint = (index * 2) + 1;
        output_data = output_data + sinf(angle * coefficeint + offset * M_TWO_PI) / coefficeint;
    }
    return output_data;
}

float __square__(float angle, float offset)
{
    float output_data;
    if (sinf(angle + offset * M_TWO_PI)  > 0)
    {
        output_data = 1;
    }
    else if (sinf(angle + offset * M_TWO_PI) < 0)
    {
        output_data = -1;
    }
    else
    {
        output_data = 0;
    }
    return output_data;
}

float __phasor__(float angle, float offset)
{
    float param, intpart;
    param = (angle / M_TWO_PI) + offset;
    return modff(param, &intpart);
}

float __phasor_inv__(float angle, float offset)
{
    float param, intpart, output_data;
    param = (angle / M_TWO_PI) + offset;
    output_data = modff(param, &intpart);
    return ((output_data * -1) + 1);
}

float __hann__(float angle, float offset)
{
    return (cos(angle + offset * M_TWO_PI) * -1 + 1) * 0.5;
}

float __pulse__(float angle, float offset, float width)
{
	if(angle/M_TWO_PI <= width)
	{
		return -1;
	}
	else
	{
	    return 0;
	}
}


static float __process__(Oscillator* self, float frequency, float sampleRate, int model, float offset)
{
    switch (model)
    {
    case E_SINE:
        self->currentSample = __sine__(self->currentAngle, offset);
        break;
    case E_TRIANGLE:
        self->currentSample = __triangle__(self->currentAngle, offset);
        break;
    case E_SAWTOOTH:
        if (frequency < 0)
        {
            frequency = frequency * -1;
            self->currentSample = __sawtooth_inv__(self->currentAngle, offset);
        }
        else
        {
            self->currentSample = __sawtooth__(self->currentAngle, offset);
        }
        break;
    case E_TRAPEZOID:
        self->currentSample = __trapezoid__(self->currentAngle, offset);
        break;
    case E_SQUARE:
        self->currentSample = __square__(self->currentAngle, offset);
        break;
    case E_PHASOR:
    	if (frequency < 0)
		{
    		frequency = frequency * -1;
    		self->currentSample = __phasor_inv__(self->currentAngle, offset);
		}
    	else
    	{
    		self->currentSample = __phasor__(self->currentAngle, offset);
    	}
    	break;
    case E_HANN:
        self->currentSample = __hann__(self->currentAngle, offset);
    	break;
    case E_PULSE:
    	self->currentSample = __pulse__(self->currentAngle, offset, self->pulseWidth);
    	break;
    }
    float intpart;
    self->counter = self->counter + frequency / sampleRate;
    self->counter = modff(self->counter, &intpart);
    self->currentAngle = self->counter * M_TWO_PI;
    return self->currentSample;
}

Oscillator* Oscillator_NEW(void* address)
{
    if (address == NULL) return NULL;
    Oscillator* self = address;

    self->currentAngle = 0;
    self->currentSample = 0;
    self->counter = 0;

    self->pulseWidth = 0.1;

    self->Process = __process__;

    return self;
}

void Oscillator_DEL(Oscillator* self)
{

}
