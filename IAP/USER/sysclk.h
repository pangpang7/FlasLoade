
#ifndef SI6600_SYSCLK_H
#define SI6600_SYSCLK_H

#include "SI6600.h"

//定义常用函数
#define   SYSTEMCLK_SRC_IRC16M           0x00
#define   SYSTEMCLK_SRC_IRC32K           0x01
#define   SYSTEMCLK_SRC_X32M             0x02
#define   SYSTEMCLK_SRC_X32K             0x03

#define   LOWSPDCLK_SRC_IRC32K           0x00
#define   LOWSPDCLK_SRC_X32K             0x01
#define   LOWSPDCLK_SRC_IRC40K           0x02

void GPIO_init(void);
void gpio_port_init(PORT_TypeDef * PORTx);
void Enable_IRC32K(u8 Speed_sel);     //包括stable_clk_sel,IRC32K_Trim,and 38K_32K_sel
void Enable_IRC16M(void);             //IRC16M_Trim(2bit粗调 + 6bit细调)  //Enabel and Stable,default;  but don't switch
void Enable_X32M(void);               //Current_driver,stable_clk_sel,stable
void Enable_X32K(void);               //
void Enable_IRC40K(void);             //

void Sysclk_switch(u8 sysclk_src);

void Disable_IRC32K(void);            //Disable 
void Disable_IRC16M(void);    
void Disable_X32M(void);
void Disable_X32K(void);
void Disable_IRC40K(void);

void CLK_out(u8 Port_Pin,u8 gpio_cfg);

void RTC_Src_Sel(u8 LowSpeedClk_Src_ID);  
void LPUART_Src_Sel(u8 LowSpeedClk_Src_ID);
void LPTIMER_Src_Sel(u8 LowSpeedClk_Src_ID);
void LCD_Src_Sel(u8 LowSpeedClk_Src_ID);
void WDT_Src_Sel(u8 LowSpeedClk_Src_ID);

#endif /* SYSTEM_ARMCM0plus_H */
