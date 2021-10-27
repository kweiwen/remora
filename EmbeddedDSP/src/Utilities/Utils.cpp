//#include "Utils.h"
//
//bool Utils::isHexValid(const char* input_data)
//{
//	if(input_data[4] != '\0')
//		return false;
//	if(input_data[0] != '0')
//        return false;
//	if((input_data[1] != 'x') && (input_data[1] != 'X'))
//        return false;
//    if(!(input_data[2] >= '0' && input_data[2] <= '9') && !(input_data[2] >= 'A' && input_data[2] <= 'F') && !(input_data[2] >= 'a' && input_data[2] <= 'f'))
//        return false;
//    if(!(input_data[3] >= '0' && input_data[3] <= '9') && !(input_data[3] >= 'A' && input_data[3] <= 'F') && !(input_data[3] >= 'a' && input_data[3] <= 'f'))
//        return false;
//    return true;
//}
//
//int Utils::HexToInt(char input_data)
//{
//	if (input_data >= '0' && input_data <= '9')
//		return input_data - '0';
//    if (input_data >= 'A' && input_data <= 'F')
//        return input_data - 'A' + 10;
//    if (input_data >= 'a' && input_data <= 'f')
//    	return input_data - 'a' + 10;
//    return -1;
//}
//
//int Utils::HexsToInt(const char* input_data)
//{
//	if(isHexValid(input_data))
//	{
//		int MSB = HexToInt(input_data[2]);
//		int LSB = HexToInt(input_data[3]);
//		return (MSB * 16 + LSB);
//	}
//	else
//	{
//		return -1;
//	}
//}
//
//float Utils::getFractionalPart(float input_data)
//{
//	int INT = (int)input_data;
//	return input_data - (float)INT;
//}
//
//int Utils::sign(float input_data)
//{
//	if(input_data >= 0)
//	{
//		return 1;
//	}
//	else
//	{
//		return -1;
//	}
//}
//
//int Utils::sign(int input_data)
//{
//	if(input_data >= 0)
//	{
//		return 1;
//	}
//	else
//	{
//		return -1;
//	}
//}
