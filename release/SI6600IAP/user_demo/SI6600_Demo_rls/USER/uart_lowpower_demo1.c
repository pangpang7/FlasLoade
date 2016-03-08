/*
//SI6600 Demo 程序:
uart6 中断接收数据，随后将接收到的数据发回UART；
*/

//////////////////////////////////////////////////////////////////////////////////////////////////////
#include "SI6600.h"
#include "sysclk.h"

u8 RecvBuf[32] = {0};
u8 cnt=0;
u8 uart_rxflg=0;
void delay(u32 idly);
void Uart_Init(void);
void UART_6_Tx(u8 tdata);



//P5.2	UART6_RX  E16 P5[2]	E16	
//P5.3	UART6_TX  B19 P5[3]	B19	J212-26
void UART6_port_init(void)	       //P52+P53	   ; P40+P35  rx+tx
{
////---------------------------------P52+P53	 
	PORT_5->Pn_SEL1 &= ~(0x3f<<6);	                            //clr 0
	PORT_5->Pn_SEL1 |=  (PORT_CFG_UART<<9) + (PORT_CFG_UART<<6);
	
	PORT_5->Pn_DIR  &= ~(0x03<<2);		                            
	PORT_5->Pn_DIR  |=  (0x01<<2);			   //2'b01		   
}

void LED_port_init(PORT_TypeDef * PORTx)	       
{
	PORTx->Pn_DIR = 0x00;
  PORTx->Pn = 0x00;
}
void led_add(PORT_TypeDef * PORTx)
{
  PORTx->Pn=cnt++;
}


int main(void)
{

	IRC32K_CTRL  &= ~0x01;        //IRC32K sel 38.5K
	SYSTEM_CTRL0 |= 0x04;		      //IRC32K Enable
	SYSTEM_CTRL1 &= ~0x04;		    //Sel IRC32K  [2]	rtc clock select
	while(!(IRC32K_CTRL&0x100));  //stable
	
	UART6_port_init();
	Uart_Init();
	LED_port_init(PORT_0);
  NVIC->ISER[0] = NVIC_INTR_EN_UART6_IRQ;	 //uart1	
	UART_6_Tx(0x55);
	delay(600);
	while(1)
	{
	  
	  while(!uart_rxflg);
		  uart_rxflg = 0;
		if(RecvBuf[0] == 0x55)
		  UART_6_Tx(0x55);
	  delay(600000);
	  delay(600000);
    led_add(PORT_0);
	}

}
void Uart_Init(void)
{
	UART_6->MCR=0x00;
	UART_6->LCR = 0x80;	
	UART_6->DLL = 0x1;		//(38.5k)/9600/4=0x01	  32/4=8K=125us
	UART_6->DLH = 0x00;
	UART_6->LCR = 0;

	UART_6->LCR = 0x03;  //data frame size ...
	UART_6->FCR = 0x00;   // FIFO disable
	 
	UART_6->IER = 1;	  //en rx Intr	 
}

void UART_6_Tx(u8 tdata)
{
   UART_6->THR=tdata;
   while(!(UART_6->LSR&UART_LSR_THREmpty));
   while(!(UART_6->LSR&UART_LSR_TxEmpty));						 
}

void delay(u32 idly)
{
while(idly--);
}
void UART6_IRQHandler(void)		//uart1
{	

	if(UART_6->IIR == 0x04)	  //Receiver data available	INTR
	{
	 RecvBuf[0]=UART_6->RBR;
	 uart_rxflg=1;
	}

}

