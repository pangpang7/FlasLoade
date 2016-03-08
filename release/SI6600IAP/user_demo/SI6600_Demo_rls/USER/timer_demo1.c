/*
timer_demo1.c      ------------timer1 intr+pwm
P311  PWM output
P1    toggle
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

//TM1_toggle	C18 P3[11]	C18	??J212-11
void Timer1_port_init(void)
{
	PORT_3->Pn_SEL2 &= ~(0x7<<9);	                       
	PORT_3->Pn_SEL2 |=  (PORT_CFG_TMG<<9);
	
	PORT_3->Pn_DIR  &= ~(0x1<<11);			   
}

void Timer_init(void)
{
  TIMER_1->CTRL        = 0x00;    //disable 
	TIMER_1->CTRL       |= 0x02;    //user-defined count mode
	TIMER_1->CTRL       &= ~0x04;   //enable intr
	TIMER_1->CTRL       |= 0x08;    //pwm
	TIMER_1->LOADCOUNT   = 0xf000;   
	TIMER_1_LOADCOUNT2   = 0x1000;   

	TIMER_1->CTRL |=0x01;           //enable Timer
}


void GPIO_port_init(PORT_TypeDef * PORTx)	       
{
	PORTx->Pn_DIR = 0x00;
  PORTx->Pn = 0x00;
}

void led_add(PORT_TypeDef * PORTx)
{
  PORTx->Pn=cnt++;
}


//=============================================================================================
int main(void)
{
	timer_flg=0;
	GPIO_port_init(PORT_0);
	GPIO_port_init(PORT_1);
	Timer1_port_init();
	Timer_init();	
	NVIC->ISER[0] =NVIC_INTR_EN_TIMER1_IRQ;   
  while(1)
	{	  
	  delay(600000);
	  delay(600000);
    led_add(PORT_0);
	}

}

void TIMER1_IRQHandler(void)
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
  TIMER_1->EOI = TIMER_1->EOI;
}


											 
