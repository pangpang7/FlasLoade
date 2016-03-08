/*
//SI6600 Demo 程序:
uart 中断接收数据，随后将接收到的数据发回UART；
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////
//uart_demo2.c

#include "SI6600.h"

u8 uart_rxflg;
u8 intr_cnt=0;
u8 RecvBuf[32] = {0};
void LED_port_init(PORT_TypeDef * PORTx)	       
{
	PORTx->Pn_DIR = 0x00;
  PORTx->Pn = 0x00;
}
void led_add(PORT_TypeDef * PORTx)
{
  //PORTx->Pn=cnt++;
}
void UART1_port_init(PORT_TypeDef * PORTx)	
{
////---------------------------------//P16+P17  
	PORTx->Pn_SEL1 &= ~(0x3f<<18);	             
	PORTx->Pn_SEL1 |=  (PORT_CFG_UART<<21) + (PORT_CFG_UART<<18);
	
	PORTx->Pn_DIR  &= ~(0x03<<6);		                            
	PORTx->Pn_DIR  |=  (0x01<<6);		

////---------------------------------P36+P35
//	PORTx->Pn_SEL1 &= ~(0x3f<<15);	                            //clr 0
//	PORTx->Pn_SEL1 |=  (PORT_CFG_UART<<18) + (PORT_CFG_UART<<15);
//	
//	PORTx->Pn_DIR  &= ~(0x03<<5);		                            
//	PORTx->Pn_DIR  |=  (0x02<<5);		   //2'b10	   note:xxxxxxxxxxxxxx	

}
void UART_Init(UART_TypeDef* UARTx)
{
  //config MCR
	UARTx->MCR=0x00;
	//set baudrate
	UARTx->LCR = 0x80;	
	UARTx->DLL = 0x68;		//9600 @16M 
	UARTx->DLH = 0x00;
	UARTx->LCR = 0;

	UARTx->LCR = 0x03;   //data frame size ...
	UARTx->FCR = 0x01;   // FIFO disable
	 
	UARTx->IER = UART_IER_ERBFI;	  //enable rx Intr	 
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
	intr_cnt=0;
  uart_rxflg = 0;
	UART1_port_init(PORT_1);
	LED_port_init(PORT_0);	
	UART_Init(UART_1);
	NVIC->ISER[0] = NVIC_INTR_EN_UART1_IRQ;	 //uart1
	while(1)
	{	 
    if(uart_rxflg)
    {
			uart_rxflg = 0;
		  UART_x_Tx(UART_1,RecvBuf[0]);
		}			
	  
	  delay(600000);
	  delay(600000);
    led_add(PORT_0);
	}
}
void UART1_IRQHandler(void)		//uart1
{	
	//intr_cnt++;
	if(UART_1->IIR == 0xC4)	  //Receiver data available	INTR
	{
	 RecvBuf[0]=UART_1->RBR;
	 uart_rxflg=1;
	}

}


