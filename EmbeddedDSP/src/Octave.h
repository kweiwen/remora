#include "PitchShift.h"

typedef struct octave Octave;
struct octave
{
	PitchShift _ps1;
	PitchShift _ps2;

	float _mix;

	float(*Process)(Octave* self, float input);
};

Octave* Octave_NEW(void* address, unsigned int bufferLength, unsigned int location);
void Octave_DEL(Octave* self);
