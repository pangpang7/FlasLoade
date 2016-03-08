/*
gpio_demo1.c
    gpio LEDÏÔÊ¾;
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////

#include "SI6600.h"
#include "sysclk.h"
u8 cnt=0;

void led_add(PORT_TypeDef * PORTx)
{
  PORTx->Pn=cnt++;
}

void delay(u32 idly)
{
    while(idly--);
}



//==================================================================================================
int main(void)
{
	GPIO_init();
	gpio_port_init(PORT_1);	
	while(1)
	{	  
	  delay(600000);
		led_add(PORT_1);
	}
}



