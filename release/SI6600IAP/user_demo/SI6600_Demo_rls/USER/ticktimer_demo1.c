//SysTick 24λ��ʱ��,�ݼ�,
//4�����ƼĴ���
/*
  SysTick->CTRL;                         SysTick Control and Status Register 
  SysTick->LOAD;                         SysTick Reload Value Register       
  SysTick->VAL;                          SysTick Current Value Register      
  SysTick->CALIB;                        SysTick Calibration Register        
*/


//////////////////////////////////////////////////////////////////////////////////////////////
#include "SI6600.h"
#include "sysclk.h"
           
u8 ticktimer_intr_flg;
	    								   
void delay(u32 t)
{		
	u32 temp;	 
  SysTick->CTRL =0x0;	
	SysTick->LOAD =t;           //ʱ�����	  		 
	SysTick->VAL  =0x00;        //��ռ�����
	SysTick->CTRL =0x05 ;       //ʹ���ں�ʱ��,�����ж�,��ʼ����
	 
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��
	
	SysTick->CTRL=0;            //�رռ�����
	SysTick->VAL =0X00;         //��ռ�����	 
}
void ticktimer_intr(void)
{		
  SysTick->CTRL =0x0;	
	SysTick->LOAD =0x4000;      //ʱ�����	  		 
	SysTick->VAL  =0x00;        //��ռ�����
	SysTick->CTRL =0x07 ;       //ʹ���ں�ʱ��,ʹ���ж�,��ʼ����
}
void SysTick_Handler(void)    //������0ʱ�����ж�
{	
  if(ticktimer_intr_flg)
	{
	 ticktimer_intr_flg=0;
	 PORT_1->Pn=0x00;
	}
  else
	{
	 ticktimer_intr_flg=1;
	 PORT_1->Pn=0xff;
	}
	if(SysTick->CTRL&(0x01<<16))
	  SysTick->CTRL = SysTick->CTRL;
}

//=============================================================================================
int main(void)
{
  
	PORT_1->Pn_DIR = 0;
	PORT_1->Pn     = 0x00;
	
	
	//1.-------------------------------intr
	ticktimer_intr_flg=0;
	ticktimer_intr();
	while(1);
	
	
//	//2.-------------------------------polling	
//	while(1)
//	{
//		delay(0x4000);
//		PORT_1->Pn=0xff;
//		delay(0x4000);
//		PORT_1->Pn=0x00;
//	}
}

