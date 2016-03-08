#include "SI6600.h"
#include "sysclk.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////
void GPIO_init(void)
{
  //BOOT0 = 0x1fff7; 
	PORT_0->Pn_DIR  =0xffff;  
	PORT_1->Pn_DIR  =0xffff;
	PORT_2->Pn_DIR  =0xffff;
	PORT_3->Pn_DIR  =0xffff;
	PORT_4->Pn_DIR  =0xffff;
	PORT_5->Pn_DIR  =0xffff;
	PORT_6->Pn_DIR  =0xffff;
	
  PORT_0->Pn_PH  = 0x7fff;		
	PORT_1->Pn_PH  = 0xffff;	
	PORT_2->Pn_PH  = 0xffff;	
	PORT_3->Pn_PH  = 0xffff;	
	PORT_4->Pn_PH  = 0xffff;	
	PORT_5->Pn_PH  = 0xffff;	
	PORT_6->Pn_PH  = 0xfeff;	
	
	PORT_0->Pn_SEL0 =0xc000;  //P014(XIN)+P015(XOUT)
	PORT_6->Pn_SEL0 =0x180;   //P67(XIN)+P68(XOUT)
}
void gpio_port_init(PORT_TypeDef * PORTx)	       
{
	PORTx->Pn_PH  = 0x00;
	PORTx->Pn_DIR = 0x00;
  PORTx->Pn     = 0x00;
}

void Enable_IRC32K(u8 Speed_sel)     //°üÀ¨stable_clk_sel,IRC32K_Trim,and 38K_32K_sel
{                          //0(32.768k)  1(38.5k) 
//	u8 IRC32K_Trim =0x00;  //32.6         36
//	u8 IRC32K_Trim =5;     //35.4         39.6
//	u8 IRC32K_Trim =10;    //38.9         43.9
//	u8 IRC32K_Trim =15;    //43.3         49.3
//	u8 IRC32K_Trim =20;    //48.5         56.5
//	u8 IRC32K_Trim =25;    //55.6         66.2
//	u8 IRC32K_Trim =31;    //66.7         82.9
	
	u8 IRC32K_Trim =2;  //
	IRC32K_CTRL =0xc0+(IRC32K_Trim<<1);
	if(Speed_sel ==38)		
      IRC32K_CTRL  &= ~BIT0;         //IRC32K sel 38.5K
	else if(Speed_sel ==32)
		  IRC32K_CTRL  |=  BIT0;         //IRC32K sel 32.768K
	
	SYSTEM_CTRL0 |=  BIT2;		         //IRC32K Enable
	while(!(IRC32K_CTRL&BIT8));        //stable
}

void Enable_IRC16M(void)             //IRC16M_Trim(2bit´Öµ÷ + 6bitÏ¸µ÷)  //Enabel and Stable,default;  but don't switch
{
  IRC16M_CTRL  = 0x55;               //IRC16M_Trim	
	SYSTEM_CTRL0 |=  BIT0;		      
	while(!(IRC16M_CTRL&BIT11));       //stable
}
void Enable_X32M(void)               //Current_driver,stable_clk_sel,stable
{
  X32M_CTRL |= 0x1F;
	SYSTEM_CTRL0 |=  BIT1;		      
	while(!(X32M_CTRL&BIT5));          //stable

}
void Enable_X32K(void)     
{
  X32K_CTRL |= 0xa;
	SYSTEM_CTRL0 |=  BIT3;		      
	while(!(X32K_CTRL&BIT6));          //stable
}
void Enable_IRC40K(void)     
{
  SYSTEM_CTRL0 |= BIT8;
}

void Sysclk_switch(u8 sysclk_src)
{
  SYSTEM_CTRL0 &=  ~(BIT4+BIT5);
  SYSTEM_CTRL0 |=   (sysclk_src<<4);	
}

void Disable_IRC32K(void)           //Disable 
{
  SYSTEM_CTRL0 &=  ~BIT2;	
}
void Disable_IRC16M(void)  
{
  SYSTEM_CTRL0 &=  ~BIT0;	
}  
void Disable_X32M(void)
{
  SYSTEM_CTRL0 &=  ~BIT1;	
}
void Disable_X32K(void)
{
  SYSTEM_CTRL0 &=  ~BIT3;	
}
void Disable_IRC40K(void)
{
  SYSTEM_CTRL1 &=  ~BIT8;	
}

void CLK_out(u8 Port_Pin,u8 gpio_cfg)
{
}

/*********************************************************************
LowSpeed_CLKSrc_Sel
LP_uart,LP_Timer,LCD,RTC,WDT
#define   LOWSPDCLK_SRC_IRC32K           0x00
#define   LOWSPDCLK_SRC_X32K             0x01
#define   LOWSPDCLK_SRC_IRC40K           0x02

**********************************************************************/
void RTC_Src_Sel(u8 LowSpeedClk_Src_ID)
{
  SYSTEM_CTRL1 &=  ~BIT2;	
	SYSTEM_CTRL1 |=  (LowSpeedClk_Src_ID<<2);
}
void LPUART_Src_Sel(u8 LowSpeedClk_Src_ID)
{
  SYSTEM_CTRL1 &=  ~BIT7;	
	SYSTEM_CTRL1 |=  (LowSpeedClk_Src_ID<<7);	
}
void LPTIMER_Src_Sel(u8 LowSpeedClk_Src_ID)
{
  SYSTEM_CTRL1 &=  ~BIT3;	
	SYSTEM_CTRL1 |=  (LowSpeedClk_Src_ID<<3);	
}
void LCD_Src_Sel(u8 LowSpeedClk_Src_ID)
{
  SYSTEM_CTRL1 &=  ~BIT6;	
	SYSTEM_CTRL1 |=  (LowSpeedClk_Src_ID<<6);	
}

void WDT_Src_Sel(u8 LowSpeedClk_Src_ID)
{
	SYSTEM_CTRL1 &=~(BIT4+BIT5);
	SYSTEM_CTRL1 |= ((~LowSpeedClk_Src_ID)<<4);
}

