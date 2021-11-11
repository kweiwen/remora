#pragma once

#include <iostream>

namespace Utils
{

bool isHexValid(const char* input_data);
int HexToInt(char input_data);
int HexsToInt(const char* input_data);

float getFractionalPart(float input_data);

int sign(float input_data);
int sign(int input_data);

void floatData2Mem(float input_data, char* mem);
float mem2FloatData(char* input_data);

}


bool Utils::isHexValid(const char* input_data)
{
	if(input_data[4] != '\0')
		return false;
	if(input_data[0] != '0')
        return false;
	if((input_data[1] != 'x') && (input_data[1] != 'X'))
        return false;
    if(!(input_data[2] >= '0' && input_data[2] <= '9') && !(input_data[2] >= 'A' && input_data[2] <= 'F') && !(input_data[2] >= 'a' && input_data[2] <= 'f'))
        return false;
    if(!(input_data[3] >= '0' && input_data[3] <= '9') && !(input_data[3] >= 'A' && input_data[3] <= 'F') && !(input_data[3] >= 'a' && input_data[3] <= 'f'))
        return false;
    return true;
}

int Utils::HexToInt(char input_data)
{
	if (input_data >= '0' && input_data <= '9')
		return input_data - '0';
    if (input_data >= 'A' && input_data <= 'F')
        return input_data - 'A' + 10;
    if (input_data >= 'a' && input_data <= 'f')
    	return input_data - 'a' + 10;
    return -1;
}

int Utils::HexsToInt(const char* input_data)
{
	if(isHexValid(input_data))
	{
		int MSB = HexToInt(input_data[2]);
		int LSB = HexToInt(input_data[3]);
		return (MSB * 16 + LSB);
	}
	else
	{
		return -1;
	}
}

float Utils::getFractionalPart(float input_data)
{
	int INT = (int)input_data;
	return input_data - (float)INT;
}

int Utils::sign(float input_data)
{
	if(input_data >= 0)
	{
		return 1;
	}
	else
	{
		return -1;
	}
}

int Utils::sign(int input_data)
{
	if(input_data >= 0)
	{
		return 1;
	}
	else
	{
		return -1;
	}
}

void Utils::floatData2Mem(float input_data, char* mem)
{
	/* WARNING!
	 * this function only works in ADSP device, due to 32-bit char architecture */
    char* temp;

    temp = (char*)&input_data;

    mem[0] = *temp >> 0 & 0xFF;
    mem[1] = *temp >> 8 & 0xFF;
    mem[2] = *temp >> 16 & 0xFF;
    mem[3] = *temp >> 24 & 0xFF;
}

float Utils::mem2FloatData(char* input_data)
{
	/* WARNING!
	 * this function only works in ADSP device, due to 32-bit char architecture */
    unsigned int src = (input_data[0] << 0) + (input_data[1] << 8) + (input_data[2] << 16) + (input_data[3] << 24);
    float temp;
    std::memcpy(&temp, &src, sizeof temp);
    return temp;
}
