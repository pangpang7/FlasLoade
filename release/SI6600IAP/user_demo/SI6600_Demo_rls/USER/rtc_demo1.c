/*
rtc_demo1.c 
rtc 1s 中断 PORT1翻转，uart输出时间
*/

///////////////////////////////////////////////////////////////////////////////////////
#include "SI6600.h"
#include "sysclk.h"

u8  rtc_flg,io_flg;
u8 rtc_string[]= "\nrtc Time:";
u8 MCU_Reset_string[] = "\nMCU reset!";
u8 rtc_time[7]= {0x20,0x15,0x07,0x02,0x21,0x20,0x00};
u8 rtc_time_string[14];

void delay(u32 idly)
{
  while(idly--);
}

void UART1_port_init(void)	       //P16+P17    ; P36+P35  rx+tx 
{
//---------------------------------//P16+P17  
	PORT_1->Pn_SEL1 &= ~(0x3f<<18);	                            //clr 0
	PORT_1->Pn_SEL1 |=  (1<<21) + (1<<18);
	
	PORT_1->Pn_DIR  &= ~(0x03<<6);		                            
	PORT_1->Pn_DIR  |=  (0x01<<6);	//2'b01
}
void Uart_Init(void)
{
	UART_1->MCR = 0x00;
	UART_1->LCR = 0x80;	
	UART_1->DLL = 0x68;		    //(16)/9600/16=0x68	  
	UART_1->DLH = 0x00;
	UART_1->LCR = 0x00;

	UART_1->LCR = 0x03;       //data frame size ...
	UART_1->FCR = 0x00;	      //FIFO  disable
	 
	UART_1->IER = 0x01;		    //en Tx and Rx Intr	
}
void RTC_cfg(void)
{
	RTC->INTERVAL = 0x41;   	//1s interrupt
	RTC->YEARH    = rtc_time[0];
	RTC->YEARL    = rtc_time[1];
	RTC->MONTH    = rtc_time[2];
	RTC->DAY      = rtc_time[3];
	RTC->HOUR     = rtc_time[4];
	RTC->MINUTE   = rtc_time[5];
	RTC->SECOND   = rtc_time[6];
	NVIC->ISER[0] |=NVIC_INTR_EN_RTC_IRQ; 
	
  RTC->HOLD = 0x00;         //Run RTC
}
void SendDatatoUart(u8 sData)
{
	UART_1->THR=sData;
	 while(!(UART_1->LSR&UART_LSR_THREmpty));
	 while(!(UART_1->LSR&UART_LSR_TxEmpty));
}
void Uart_Send_String(u8 *Pdata)
{
	while((*Pdata) != '\0')
	{
		SendDatatoUart(*Pdata);
		Pdata++;	
	}
}
void Uart_Send_time_string14(void) 
{
	u8 i;
	for(i=0;i<7;i++)
	{
	  rtc_time_string[i*2+0]=((rtc_time[i]&0xf0)>>4) +48;
		rtc_time_string[i*2+1]=((rtc_time[i]&0x0f)) +48;
	}

	for(i=0;i<14;i++)
	{
	  SendDatatoUart(rtc_time_string[i]);
	}
}
void system_init(void)	
{
//   Enable_IRC32K(32);
//   RTC_Src_Sel(LOWSPDCLK_SRC_IRC32K);	

Enable_X32K();
RTC_Src_Sel(LOWSPDCLK_SRC_X32K);		
	
//	IRC32K_CTRL  |= 0x01;         //IRC32K sel 32.768K
//	SYSTEM_CTRL0 |= 0x04;		      //IRC32K Enable
//	SYSTEM_CTRL1 &= ~0x04;		    //Sel IRC32K  [2]	rtc clock select
//	while(!(IRC32K_CTRL&0x100));  //stable
}

//===========================================================================================
int main(void)
{
	u8 i;
	rtc_flg = 0;
	io_flg  = 0;
	GPIO_init();
	
	
	PORT_0->Pn_PH  = 0x00;
	PORT_1->Pn_PH  = 0x00;
	
	PORT_0->Pn_DIR = 0x00;
	PORT_1->Pn_DIR = 0x00;
	for(i=0;i<8;i++)
	{
	  PORT_0->Pn = 0x01<<i;
		delay(100000);
		delay(100000);
		delay(100000);
	}
	PORT_0->Pn = 0x00;
	
	system_init();
  UART1_port_init();
	Uart_Init();
  RTC_cfg();
  Uart_Send_String(MCU_Reset_string);	
	while(1)
	{			
	  if(rtc_flg)
	  {
			rtc_time[0] = RTC->YEARH_DIS;
			rtc_time[1] = RTC->YEARL_DIS;
			rtc_time[2] = RTC->MONTH_DIS;
			rtc_time[3] = RTC->DAY_DIS;
			rtc_time[4] = RTC->HOUR_DIS;
			rtc_time[5] = RTC->MINUTE_DIS;
			rtc_time[6] = RTC->SECOND_DIS;
			
			Uart_Send_String(rtc_string);
			Uart_Send_time_string14();
	    rtc_flg = 0;
	  }
	}
}

void RTC_IRQHandler(void)
{
	rtc_flg=1;
  RTC->RTCINTERRUPT |= 0x01;  //clr intr 
	if(io_flg)
	  {
	     io_flg = 0;
			 PORT_1->Pn = 0x00;
	  }
	else 
		{
	     io_flg = 1;
			 PORT_1->Pn = 0xff;
	  }
}

