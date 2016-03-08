/*
gpio_demo1.c
    gpio intr,Port_x,Pin_x,fall-dege
按键中断(port Intr),每一次按下，P0 加1
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////

#include "SI6600.h"
#include "sysclk.h"
PORT_TypeDef   *bPORTx;
u8 	bPINx; 
u8 cnt,intr_flg;

void delay(u32 idly)
{
    while(idly--);
}
/*
EDGE_SEL
端口P0.0 中断边沿选择配置寄存器
[1] = 1：外部端口上升沿产生中断； 
[1] = 0：外部端口上升沿不产生中断；
[0] = 1：外部端口下降沿产生中断； 
[0] = 0：外部端口下降沿不产生中断；

PULSE_SEL
端口P0.0 中断电平选择配置寄存器
[1] = 1：外部高电平产生中断； 
[1] = 0：外部高电平不产生中断；
[0] = 1：外部低电平产生中断； 
[0] = 0：外部低电平不产生中断；
*/


void gpioIntr_init(PORT_TypeDef * PORT_x,u8 PINx)
{
	  PORT_x->Pn           |=  (0x01<<PINx);
	  PORT_x->Pn_SEL0      &= ~(0x01<<PINx);
	  if(PINx<8)			
	      PORT_x->Pn_SEL1      &= ~(0x07<<(PINx*3));
		else
	      PORT_x->Pn_SEL2      &= ~(0x07<<((PINx-8)*3));
		
	  PORT_x->Pn_DIR       |= (0x01<<PINx);                  //input
	  PORT_x->Pn_PH        |= (0x01<<PINx);                  //pull-push
	  PORT_x->Pn_IFG       &= ~(0x01<<PINx);
	
	  PORT_x->Pn_PULSE_SEL  = 0x00;                          //Px.0, 
	  PORT_x->Pn_EDGE_SEL  |= (0x01<<(PINx<<1));             //Px.0, fall-edge
	
	  PORT_x->Pn_IE        |=  (0x01<<PINx);                 //Px.0, Enable intr
}


//==================================================================================================
int main(void)
{
	u32 i;
	intr_flg=0;
	cnt = 0x01;
	
	GPIO_init();
	gpio_port_init(PORT_0);	
	for(i=0;i<8;i++)
	{
		delay(200000);
	  PORT_0->Pn = 0x01<<i;
	}
  PORT_0->Pn = 0;
	
//////////////////////////////////////////////
	bPORTx = PORT_6;
	bPINx  = 0;
	
  gpioIntr_init(bPORTx,bPINx);
	NVIC->ISER[0] = NVIC_INTR_EN_P5P6_IRQ;
  	
	while(1)
	{
   if(intr_flg)
	 {
		 intr_flg=0;
	   delay(200000);
		 PORT_0->Pn=cnt++;
	 }
	}

}


void P5P6_IRQHandler(void)
{
	bPORTx->Pn_IFG &= ~(0x01<<bPINx);
	delay(1000);
	
	if(!(bPORTx->Pn&(0x01<<bPINx)))
	{
	    intr_flg =1;
	}
}



