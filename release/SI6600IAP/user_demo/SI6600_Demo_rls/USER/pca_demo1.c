/*pca_demo1.c   PCA Capture test ,connect P310 to P311
PCA10,高速输出，CCAPH以0x20累加,
PCA11,捕获输入,由PCA10输入,上升沿触发中断,
主主循环查询捕获标志,一旦捕获到新的数据，将数据由UART_1输出,
9600 8 N 1 :0x3000 0x7000 0xB000 0xF000 0x3000 0x7000 0xB000 0xF000......
*/

///////////////////////////////////////////////////////////////////////////////////////
#include "SI6600.h"
#include "sysclk.h"

u8  pca_capt_flg;
u16 Capt_data;
PCA_TypeDef* PCA_x;


void pca_init6(void)  //connect P310 to P311
{
	//PCA1.0		B26 P3[10]	B26	J212-9
    PORT_3->Pn_DIR  &= ~(0x01<<10);
	  PORT_3->Pn_SEL2 &= ~(0x07<<6);
	  PORT_3->Pn_SEL2 |=  (0x07<<6);
	
	//PCA1.1		C18 P3[11]	C18	J212-11
    PORT_3->Pn_DIR  |=  (0x01<<11);  //input
	  PORT_3->Pn_PH   |=  (0x01<<11); 
	  PORT_3->Pn_SEL2 &= ~(0x07<<9);
	  PORT_3->Pn_SEL2 |=  (0x07<<9);
	
	
   PCA_x->PCA_CMOD  = 0x80;     //IDLE mode stop cnt,disable PCA ch4,sel PCAclksrc;
   //PCA_x->PCA_CMOD  |= 0x01;    //enable PCA CH/CL Overflow intr;
   PCA_x->PCA_CH     = 0x00;
	 PCA_x->PCA_CL     = 0x00;

		//------------------------------------PCA1.0 highspeed output
   PCA_x->PCA_CCAPM0 =  PCA_CCAPMx_HighSpeedOut;    //highspeed output 
   PCA_x->PCA_CCAPM0 |= 0x01;	                   //CH/CL cap/match intr 
   PCA_x->PCA_CCAP0L =  0x00;	                   //write CCAPxL first
   PCA_x->PCA_CCAP0H =  0x10;
 	  //------------------------------------PCA1.1 capture
   PCA_x->PCA_CCAPM1 =  PCA_CCAPMx_CaptRisEdge;   
   PCA_x->PCA_CCAPM1 |= 0x01;	                   //CH/CL cap/match intr  
   PCA_x->PCA_CCAP1L =  0x00;	                   //write CCAPxL first
   PCA_x->PCA_CCAP1H =  0x00;
	 
   PCA_x->PCA_CCON	 =  0x40;    //Run pca
}


void PCA12_IRQHandler(void)
{
	if((PCA_1->PCA_CCON)&0x01)  //PCA1.0
	{
		PCA_x->PCA_CCON   &=~0x01;		//clr intr cf0
		PCA_x->PCA_CCAP0L  = 0x00;
		PCA_x->PCA_CCAP0H += 0x20;
	}
	
	if((PCA_1->PCA_CCON)&0x02)  //PCA1.1
	{
		PCA_x->PCA_CCON &=~0x02;		//clr intr cf1
		Capt_data = (PCA_x->PCA_CCAP1H<<8) + PCA_x->PCA_CCAP1L;
		pca_capt_flg = 1;
	}

}
void UART1_port_init(void)	       //P16+P17    ; P36+P35  rx+tx 
{
//---------------------------------//P16+P17  
	PORT_1->Pn_SEL1 &= ~(0x3f<<18);	                            //clr 0
	PORT_1->Pn_SEL1 |=  (1<<21) + (1<<18);
	
	PORT_1->Pn_DIR  &= ~(0x03<<6);		                            
	PORT_1->Pn_DIR  |=  (0x01<<6);		   //2'b01
}
void Uart_Init(void)
{
	UART_1->MCR=0x00;
	UART_1->LCR = 0x80;	
	UART_1->DLL = 0x68;		 //9600@16M	  
	UART_1->DLH = 0x00;
	UART_1->LCR = 0x00;

	UART_1->LCR = 0x03;    //data frame size ...
	UART_1->FCR = 0x00;	   //FIFO  disable
	 
}
void SendDatatoUart(u16 sData)
{
	NVIC->ISER[0] = 0 ;
	UART_1->THR=(sData>>8)&0xff;
		 while(!(UART_1->LSR&UART_LSR_THREmpty));
	UART_1->THR=sData&0xff;
		 while(!(UART_1->LSR&UART_LSR_THREmpty));
	   while(!(UART_1->LSR&UART_LSR_TxEmpty));
	NVIC->ISER[0] |=  NVIC_INTR_EN_PCA12_IRQ ;
}
int main(void)
{
	pca_capt_flg=0; 
	PCA_x = PCA_1;
	
	UART1_port_init();
	Uart_Init();	
	pca_init6();
	NVIC->ISER[0] |=  NVIC_INTR_EN_PCA12_IRQ ;
	while(1)
	{
	  if(pca_capt_flg)
		{
		 pca_capt_flg = 0;
		 SendDatatoUart(Capt_data);
		}
	}

}



