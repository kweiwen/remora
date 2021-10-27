#pragma once

namespace Utils
{

bool isHexValid(const char* input_data);
int HexToInt(char input_data);
int HexsToInt(const char* input_data);

float getFractionalPart(float input_data);

int sign(float input_data);
int sign(int input_data);

}
