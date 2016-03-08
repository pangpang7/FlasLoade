/*
uart_demo1.c
uart 发送数据;
gpio 显示;

*/
//////////////////////////////////////////////////////////////////////////////////////////////////////
#include "SI6600.h"
#include "sysclk.h"

u32 cnt=0;

void LED_port_init(PORT_TypeDef * PORTx)	       
{
	PORTx->Pn_PH = 0x00;
	PORTx->Pn_DIR = 0x00;
  PORTx->Pn = 0x00;
}
void led_add(PORT_TypeDef * PORTx)
{
  PORTx->Pn=cnt++;
}
void UART1_port_init(PORT_TypeDef * PORTx)	       
{
//---------------------------------//P16+P17  
	PORTx->Pn_SEL1 &= ~(0x3f<<18);	                  
	PORTx->Pn_SEL1 |=  (1<<21) + (1<<18);
	
	PORTx->Pn_DIR  &= ~(0x03<<6);		                            
	PORTx->Pn_DIR  |=  (0x01<<6);		   

}
void UART_Init(UART_TypeDef* UARTx)
{
	UARTx->MCR = 0x00;
	UARTx->LCR = 0x80;	
	UARTx->DLL = 0x68;		// 9600@16M  
	UARTx->DLH = 0x00;
	UARTx->LCR = 0;

	UARTx->LCR = 0x03;    //data frame size ...
	UARTx->FCR = 0x00;    // FIFO disable
	 
	UARTx->IER = 0;	      //dis Intr	 
}

void UART_x_Tx(UART_TypeDef* UARTx,u8 tdata)
{
   UARTx->THR=tdata;
   while(!(UARTx->LSR&UART_LSR_THREmpty));
   while(!(UARTx->LSR&UART_LSR_TxEmpty));						 
}

void delay(u32 idly)
{
while(idly--);
}
int main(void)
{
  GPIO_init();
	LED_port_init(PORT_0);	
	
	UART1_port_init(PORT_1);
	UART_Init(UART_1);
	while(1)
	{	  
	  UART_x_Tx(UART_1,0x55);
	  delay(600000);
	  delay(600000);
    led_add(PORT_0);
	}
}



