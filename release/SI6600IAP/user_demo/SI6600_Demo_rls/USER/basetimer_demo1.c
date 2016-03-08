/*
basetimertimer_demo1.c      ------------basetimertimer1 intr+pwm
P56 PWM output
P1      toggle
*/


///////////////////////////////////////////////////////////////////////////////////////
#include "SI6600.h"
#include "sysclk.h"
u8   timer_flg;
u32  cnt;


void delay(u32 idly)
{
   while(idly--);
}


void basetimer_port_init(void)
{
	//BTM_toggle	 P5[6]
	PORT_5->Pn_SEL1 &= ~(0x7<<18);	                       
	PORT_5->Pn_SEL1 |=  (0x5<<18);	
	PORT_5->Pn_DIR  &= ~(0x1<<6);	

		//P55 gate
//  PORT_5->Pn_SEL1 &= ~(0x7<<15);	                            //clr 0
//	PORT_5->Pn_SEL1 |=  (5<<15);	
//	PORT_5->Pn_DIR  |=  (0x1<<5);			   //6'b000000
	
//		//P54 clk_in
//  PORT_5->Pn_SEL1 &= ~(0x7<<12);	                            //clr 0
//	PORT_5->Pn_SEL1 |=  (5<<12);	
//	PORT_5->Pn_DIR  |=  (0x1<<4);			   //6'b000000
//	
//	//P211 IRC32K output
//  PORT_2->Pn_SEL2 &= ~(0x7<<9);	                            //clr 0
//	PORT_2->Pn_SEL2 |=  (6<<9);	
//	PORT_2->Pn_DIR  &= ~(0x1<<11);			   //6'b000000	
}

void basetimer_init(void)
{
  BASETIMER->CTRL        = 0x00;    //disable 
	BASETIMER->CTRL       |= 0x02;    //user-defined count mode
	BASETIMER->CTRL       &= ~0x04;   //enable intr
	
	
	BASETIMER->CTRL       |= 0x08;    //pwm
	BASETIMER->LOADCOUNT   = 0xf0;   
	BASETIMER_LOADCOUNT2   = 0x80;     
	
	BASETIMER->CTRL       |=0x01;           //enable Timer
}


void GPIO_port_init(PORT_TypeDef * PORTx)	       
{
	PORTx->Pn_PH  = 0x00;
	PORTx->Pn_DIR = 0x00;
  PORTx->Pn = 0x00;
}

void led_add(PORT_TypeDef * PORTx)
{
  PORTx->Pn=cnt++;
}
void basetimer_clk_cfg(void)
{
Enable_X32K();
LPTIMER_Src_Sel(LOWSPDCLK_SRC_X32K);	
	
//	SYSTEM_CTRL0 |= 0x04;   //[2]	   irc32k enable
//	SYSTEM_CTRL1 &= ~0x08;  //[3]	   basetimer clock select(	0 = irc32k)
//  while(!(IRC32K_CTRL & 0x100));
}

//=============================================================================================
int main(void)
{
	timer_flg=0;
	GPIO_init();
	
	GPIO_port_init(PORT_0);
	GPIO_port_init(PORT_1);
	basetimer_clk_cfg();
	basetimer_port_init();
	basetimer_init();	
	NVIC->ISER[0] =NVIC_INTR_EN_BASETIMER_IRQ;   
  while(1)
	{	  
	  delay(600000);
	  delay(600000);
    led_add(PORT_0);
	}

}

void BASETIMER_IRQHandler(void)
{
  
  if(timer_flg)
  {   
      timer_flg=0;
      PORT_1->Pn = 0x00;
  }
  else
  {
      timer_flg=1;
      PORT_1->Pn = 0xff;
  }
  BASETIMER->EOI = BASETIMER->EOI;
}


											 
