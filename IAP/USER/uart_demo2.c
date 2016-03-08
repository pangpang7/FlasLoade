/*
//SI6600 Demo ����:
uart �жϽ������ݣ���󽫽��յ������ݷ���UART��
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////
//uart_demo2.c

#include "SI6600.h"
#include "sysclk.h"
#include "UartSlave.h"

void delay(u32 idly)
{
   while(idly--);
}

int main(void)
{
	//Enable_IRC16M();
	UART1_port_init(PORT_1);
	UART_Init(UART_1);
	UartSlaveInit();
	
	
	while(1)
	{
		UartSlaveProtocolProc();
	}
}


