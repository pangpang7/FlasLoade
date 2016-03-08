/*
wdt_demo1.c
*/

/////////////////////////////////////////////////////////////////////////////////////////////
#include "SI6600.h"
#include "sysclk.h"

void delay(u32 idly)
{
    while(idly--);
}

void init_wdt(void)
{
//	Enable_IRC32K(32);
//  WDT_Src_Sel(LOWSPDCLK_SRC_IRC32K);
//	
//	Enable_X32K();
//	WDT_Src_Sel(LOWSPDCLK_SRC_X32K);
	
	Enable_IRC40K();	
	WDT_Src_Sel(LOWSPDCLK_SRC_IRC40K);

  WDT->CR    = 0x0;     //32pclk,Generate a system reset.
	//WDT->CR   |= 0x02;    //intr(1st) + reset(2nd)
  WDT->TORR  = 0x07;  
  WDT->CR   |= 0x01;		//enable
	
}

//=================================================================================
int main(void)
{
  u8 i,j,cnt;
	GPIO_init();
	PORT_0->Pn_PH  = 0x00;
	PORT_0->Pn_DIR = 0x00;
	PORT_0->Pn     = 0x00;

	for(j=0;j<3;j++)
	{
		for(i=0;i<8;i++)
		{
			delay(100000);
			PORT_0->Pn = 1<<i;
		}
		
		for(i=0;i<8;i++)
		{
			delay(100000);
			PORT_0->Pn = 0x80>>i;
		}
	}

	PORT_0->Pn = 0x3;
	delay(200000);
	delay(200000);
	delay(200000);
	
	init_wdt(); 
	while(1)
	{
		WDT->CRR=0x76;
		PORT_0->Pn = cnt++;
		delay(10000);               //delay time < WDT TimeOut;
	}	
}
