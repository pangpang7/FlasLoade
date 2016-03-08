/*
gpio_demo1.c
    gpio intr,Port_x,Pin_x,fall-dege
�����ж�(port Intr),ÿһ�ΰ��£�P0 ��1
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
�˿�P0.0 �жϱ���ѡ�����üĴ���
[1] = 1���ⲿ�˿������ز����жϣ� 
[1] = 0���ⲿ�˿������ز������жϣ�
[0] = 1���ⲿ�˿��½��ز����жϣ� 
[0] = 0���ⲿ�˿��½��ز������жϣ�

PULSE_SEL
�˿�P0.0 �жϵ�ƽѡ�����üĴ���
[1] = 1���ⲿ�ߵ�ƽ�����жϣ� 
[1] = 0���ⲿ�ߵ�ƽ�������жϣ�
[0] = 1���ⲿ�͵�ƽ�����жϣ� 
[0] = 0���ⲿ�͵�ƽ�������жϣ�
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



