#include <iostream>
#include <AudioParameterFloat.h>
#include <CircularBuffer.h>
#include <DigitalDelayLine.h>
#include <Oscillator.h>

int main()
{
	std::cout << "Hello World" << std::endl;

	AudioParameterFloat test("0x00", "note", 69);
	Oscillator osc();

	return 0;
}