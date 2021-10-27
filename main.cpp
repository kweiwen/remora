#include <iostream>
#include <AudioParameterFloat.h>
#include <CircularBuffer.h>
#include <DigitalDelayLine.h>
#include <Oscillator.h>

#include "mld.hpp"


int main()
{
	memoryld::memory_monitoring();

	DigitalDelayLine dl(1024, 0);
	dl.Release();

	memoryld::memory_check();

	return 0;
}