/*
gpio_demo1.c
    gpio LED??;
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////


#include "SI6600.h"

u8 cnt=0;


void led_add(PORT_TypeDef * PORTx)
{
  PORTx->Pn=cnt++;
}
			
void delay(unsigned int  idly)
{
    while(idly--);
}


unsigned char clock_1[16] = {0x5F,0x06,0x6B,0x2F,0x36,0x3d,0x7d,0x07,0x7f,0x3f,0x77,0x7c,0x5d,0x6e,0x79,0x71}; 
u8 clk[4][16]  ={  {3,0,1,1,2,3,3,1,3,3,3,2,3,0,3,3},
	                 {1,1,3,3,3,2,2,1,3,3,3,2,0,3,2,2},
									 {3,1,2,1,1,1,3,1,3,1,3,3,2,3,2,2},
									 {1,0,1,1,0,1,1,0,1,1,0,1,1,1,1,0}};
u8 YEAR1, YEAR1, YEAR2, YEAR3, YEAR4, MONL, MONH, DAYL, DAYH, HOURL, HOURH, MINL, MINH, SECL, SECH;

//           
//		 ___A___
//    |       |
//    | F     | B
//    |___G___|
//    |       |
//    | E     | C
//    |___D___|
//

//==================================================================================================
int main(void)
{ 
  u8 i=0;
	PORT_0->Pn_DIR  =0xffff;  //p015+P68

	PORT_1->Pn_DIR  =0xffff;
	PORT_2->Pn_DIR  =0xffff;
	PORT_3->Pn_DIR  =0xffff;
	PORT_4->Pn_DIR  =0xffff;
	PORT_5->Pn_DIR  =0xffff;
	PORT_6->Pn_DIR  =0xffff;
	
	PORT_0->Pn_SEL0 =0xc000;  //p015+P68
  PORT_0->Pn_PH = 0x7fff;		
	PORT_1->Pn_PH = 0xffff;	
	PORT_2->Pn_PH = 0xffff;	
	PORT_3->Pn_PH = 0xffff;	
	PORT_4->Pn_PH = 0xffff;	
	PORT_5->Pn_PH = 0xffff;	
	PORT_6->Pn_SEL0 =0x180;  //p015+P68
	PORT_6->Pn_PH = 0xfeff;	
 
 

  X32K_CTRL = 0x0a;
	SYSTEM_CTRL0 |=  BIT3;		  
	SYSTEM_CTRL1 |=  BIT2;	
	
//	RTC->INTERVAL = 0x4f;   	//1s interrupt
//	RTC->SECOND   = 0;
//  RTC->HOLD = 0x00;         //Run RTC

	RTC->HOLD = 0x00;								  //??RTC  

  RTC->INTERVAL  = 0X41;								  //??????
  
  RTC->YEARL 		= 0x15;								  //??????
  RTC->YEARH	 	= 0x20;
  RTC->MONTH 		= 0x12;
  RTC->DAY 			= 0x21;
  RTC->HOUR 		= 0x12;
  RTC->MINUTE 	= 0x34;
  RTC->SECOND 	= 0x56;


  PORT_1->Pn_SEL0 =0x0780; 
	PORT_2->Pn_SEL0 =0x000f; 
	PORT_3->Pn_SEL0 =0x0000; 
	PORT_4->Pn_SEL0 =0x0ff0; 
	PORT_5->Pn_SEL0 =0x00ff; 
	PORT_6->Pn_SEL0 =0x09ff; 

	SYSTEM_CTRL1 |=  BIT6;	
	
	LCD->LCDRAM0L = 0xffffffff;
	LCD->LCDRAM1L = 0xffffffff;
	LCD->LCDRAM2L = 0xffffffff;
	LCD->LCDRAM3L = 0xffffffff;
	LCD->LCDRAM4L = 0xffffffff;
	LCD->LCDRAM5L = 0xffffffff;
	LCD->LCDRAM6L = 0xffffffff;
	LCD->LCDRAM7L = 0xffffffff;
	LCD->LCDRAM0H = 0xffffffff;
	LCD->LCDRAM1H = 0xffffffff;
	LCD->LCDRAM2H = 0xffffffff;
	LCD->LCDRAM3H = 0xffffffff;
	LCD->LCDRAM4H = 0xffffffff;
	LCD->LCDRAM5H = 0xffffffff;
	LCD->LCDRAM6H = 0xffffffff;
	LCD->LCDRAM7H = 0xffffffff;	
	LCD->LCDCR =0x2db;	
	
	
	NVIC->ICPR[0] = 0x0000;
	NVIC->ISER[0] |= NVIC_INTR_EN_RTC_IRQ;
	
	PERI_CLK_CTRL0 = 0x0000;
	PERI_CLK_CTRL1 = 0x0024;	//RTC+LCD
	
//	SCB->SCR &=  ~0x04;   //sleep
//	SCB->SCR |=  0x04;   //deepsleep
//	__WFI();
	
	LCD->LCDRAM0L = 0x00000000;
	LCD->LCDRAM1L = 0x00000000;
	LCD->LCDRAM2L = 0x00000000;
	LCD->LCDRAM3L = 0x00000000;
	LCD->LCDRAM4L = 0x00000000;
	LCD->LCDRAM5L = 0x00000000;
	LCD->LCDRAM6L = 0x00000000;
	LCD->LCDRAM7L = 0x00000000;
	LCD->LCDRAM0H = 0x00000000;
	LCD->LCDRAM1H = 0x00000000;
	LCD->LCDRAM2H = 0x00000000;
	LCD->LCDRAM3H = 0x00000000;
	LCD->LCDRAM4H = 0x00000000;
	LCD->LCDRAM5H = 0x00000000;
	LCD->LCDRAM6H = 0x00000000;
	LCD->LCDRAM7H = 0x00000000;	

	while(1)
	{
		for(i=0;i<16;i++)
		{
	//显示第一个8	
			LCD->LCDRAM0L = clk[0][i];
			LCD->LCDRAM1L = clk[1][i];
			LCD->LCDRAM2L = clk[2][i];
			LCD->LCDRAM3L = clk[3][i];

	//显示第二个8			
			LCD->LCDRAM0L |= clk[0][i]<<2;
			LCD->LCDRAM1L |= clk[1][i]<<2;
			LCD->LCDRAM2L |= clk[2][i]<<2;
			LCD->LCDRAM3L |= clk[3][i]<<2;
			delay(1000000);
			delay(1000000);
			delay(1000000);
		}
	}

}


void LCD_IRQHandler(void)
{
	LCD->LCDCR &= ~BIT18;
}


void RTC_IRQHandler(void)
{
	RTC->RTCINTERRUPT |= 0x01;  //clr intr
}


