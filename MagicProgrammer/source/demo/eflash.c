/*
flash 擦写

*/
//////////////////////////////////////////////////////////////////////////////////////////////////////
#include "SI6600.h"

u32 cnt=0,tmp32;

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



void delay(u32 idly)
{
while(idly--);
}

void flash_er_wr(void)
{
//page erase
FLASH_CTRL= 0x02;
(*(volatile unsigned int *)(0x00020000)) = 0x55555555;	 //该页任意地址写任意值将擦除该页
 
//program
FLASH_CTRL = 0x01;
(*(volatile unsigned int *)(0x00020000)) = 0x55555555;	 

 tmp32 = (*(volatile unsigned int *)(0x00020000));
}
int main(void)
{
	IRC16M_CTRL = 0x60;  //4M	
	LED_port_init(PORT_0);
	flash_er_wr();
	
	while(1)
	{	 
    if(tmp32 == 0x55555555)	
		{			
      led_add(PORT_0);
		}
		else
		{
		  PORT_0->Pn = 0xffff;
		}
		delay(100000);
	}
}



