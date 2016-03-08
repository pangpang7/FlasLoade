/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : hw_config.h
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : Hardware Configuration & Setup
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_lib.h"
#include "platform_config.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define PTDO GPIOB 
#define TDO GPIO_Pin_11

#define PTMS GPIOE
#define TMS GPIO_Pin_15

#define PTDI GPIOE
#define TDI GPIO_Pin_13

#define PTCK GPIOE
#define TCK GPIO_Pin_11

#define PRST GPIOE
#define RST GPIO_Pin_9

#define NSS TMS
#define SCK TCK
#define MISO TDO
#define MOSI TDI

#define PNSS PTMS
#define PSCK PTCK
#define PMISO PTDO
#define PMOSI PTDI

// SWCLK Pin                 
#define PORT_SWCLK           PTCK			
#define PIN_SWCLK            TCK

// SWDIO Pin                  
#define PORT_SWDIO            PTDI	
#define PIN_SWDIO             TDI

#define SO_SetBits(Signal) \
	P##Signal##->BSRR = Signal;
	

#define SO_ResetBits(Signal) \
	P##Signal##->BRR = Signal;

static __inline void SO_ResetClock()
{
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	PTCK->BRR = TCK;
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
}

static __inline void SO_SetClock()
{
	__nop();
	__nop();
	__nop();
	__nop();
	__nop(); 
	PTCK->BSRR = TCK;
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
}

#define SO_WriteBits(Signal,BitVal) \
	if (BitVal != Bit_RESET)	\
	{													\
	  P##Signal##->BSRR = Signal;		\
	}													\
	else											\
	{													\
	  P##Signal##->BRR = Signal;		\
	}

static __inline u8 SI_GetBits()
{
  if ((PTDO->IDR & TDO) != (u32)Bit_RESET)
  {
    return (u8)Bit_SET;
  }
  else
  {
    return (u8)Bit_RESET;
  }
}


/* Exported define -----------------------------------------------------------*/
#define HAPPY_OFF 0
#define HAPPY_ON_1S 1
#define HAPPY_BLINK_1 2
#define HAPPY_BLINK_2 3
/* Exported functions ------------------------------------------------------- */

void Set_System(void);
void Set_USBClock(void);
void Enter_LowPowerMode(void);
void Leave_LowPowerMode(void);
void USB_Interrupts_Config(void);
void GPIO_Configuration(void);
void EXTI_Configuration(void);
void Get_SerialNum(void);
void UART_Configuration(void);
u32 UART_send(u32 lg,u8* data);
u32 UART_rev(u32 lg,u8* data);
void SYSTICK_Configuration(void);
void systick_proc(void);
u32 GetSysTick(void);
void SetHappyLight(u32 mode);
void KEY_INT(void);
void ComDogFeed(void);
bool GetComDogDead(void);
bool FLASH_Earse(u32 StartAddr,u32 EndAddr);
bool FLASH_Program(u32 StartAddr,u8 * data, u32 lg);
void DelayMs(u32 ms);
bool GetKeyDown(void);
void ClearKeyDown(void);
void Key_Confirm(void);
void UART_SetTimeOut(u32 ms);
void STC_IO_Config_Unused(void);
void STC_IO_Config_Used_for_STC(void);
void STC_IO_Config_Used_for_SWD(void);
#define os_dly_wait(ms) DelayMs(ms)
#endif  /*__HW_CONFIG_H*/

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
