#include "commandProcess.h"
#include "Driver/UART0_isr.h"
#include "Driver/ADDS_21479_EzKit.h"
//#include "../Utilities/Utils.h"

void handleCommandData()
{
	commandReady = 0;
	isCommandProcessing = 1;

	/** place handle command here! */
//	int command_1 = Utils::HexToInt(uart_buffer[0])*16 + Utils::HexToInt(uart_buffer[1]);
//	int command_2 = Utils::HexToInt(uart_buffer[2])*16 + Utils::HexToInt(uart_buffer[3]);
//	int command_3 = Utils::HexToInt(uart_buffer[4])*16 + Utils::HexToInt(uart_buffer[5]);
//	int command_4 = Utils::HexToInt(uart_buffer[6])*16 + Utils::HexToInt(uart_buffer[7]);
//
//	if(command_1 == 9)
//	{
//		audioObject[command_2]->ParameterCtrl(command_3, command_4);
//	}
//	else
//	{
//		result = audioObject[command_2]->ParameterDisp(command_3, command_4);
//		DBG(result);
//	}

	DBG(uart_buffer);

	isCommandProcessing = 0;
}
