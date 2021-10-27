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

	for (int i = 0; i < 128; i++)
	{
		test = i;
		std::cout << test.cc_value << std::endl;
		std::cout << test.value << std::endl;
	}

	return 0;
}