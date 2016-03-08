/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : hw_config.c
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

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_lib.h"
#include "platform_config.h"
#include "hw_config.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"


/* Private typedef -----------------------------------------------------------*/
#define FLASH_PAGE_SIZE 0x800
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ErrorStatus HSEStartUpStatus;
volatile u32 SysTime;
volatile u32 ComDogTime;
u32 HappyMode;
bool HappyFlag;
bool Key_OD_Pre;
bool Key_OD_Det;
u32 Key_Delay;
u32 UartTimeOut;
bool gpio_reset_pin_is_input  = TRUE; //zwh
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void gpio_set_isp_pin(u8 state) {
    /*if (state) {
        LPC_GPIO_PORT->SET[ISPCTRL_PORT] = (1 << ISPCTRL_BIT);
    } else {
        LPC_GPIO_PORT->CLR[ISPCTRL_PORT] = (1 << ISPCTRL_BIT);
    }//zwh*/
}
/*******************************************************************************
* Function Name  : Set_System
* Description    : Configures Main system clocks & power.
* Input          : None.
* Return         : None.
*******************************************************************************/
void Set_System(void)
{
  /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/   
     
  /* RCC system reset(for debug purpose) */
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if(HSEStartUpStatus == SUCCESS)
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);
 
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
  
    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1); 

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /* On STICE the PLL output clock is fixed to 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

    /* Enable PLL */ 
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }

   /* Enable USART1, GPIOA, GPIOD and AFIO clocks RCC_APB2Periph_USART1 |*/
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA
                         | RCC_APB2Periph_AFIO | RCC_APB2Periph_USART1, ENABLE);

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
						   
  /* Configure the used GPIOs*/
  GPIO_Configuration();
  
  /* Configure the EXTI lines for Key and Tamper push buttons*/
  EXTI_Configuration();
  
  UART_Configuration();
  SYSTICK_Configuration();
}

/*******************************************************************************
* Function Name  : Set_USBClock
* Description    : Configures USB Clock input (48MHz).
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Set_USBClock(void)
{
  /* Select USBCLK source */
  RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);

  /* Enable USB clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
}

/*******************************************************************************
* Function Name  : Enter_LowPowerMode.
* Description    : Power-off system clocks and power while entering suspend mode.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Enter_LowPowerMode(void)
{
  /* Set the device state to suspend */
  bDeviceState = SUSPENDED;
}

/*******************************************************************************
* Function Name  : Leave_LowPowerMode.
* Description    : Restores system clocks and power while exiting suspend mode.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Leave_LowPowerMode(void)
{
  DEVICE_INFO *pInfo = &Device_Info;
  
  /* Set the device state to the correct state */
  if (pInfo->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
  else 
  {
    bDeviceState = ATTACHED;
  }
}

/*******************************************************************************
* Function Name  : USB_Interrupts_Config.
* Description    : Configures the USB interrupts.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void USB_Interrupts_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
 
  /* Set the Vector Table base address at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x00);
  
  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN_RX0_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* Enable the EXTI4 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
 }

void STC_IO_Config_Unused(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Configure STC/SPI IO */
  GPIO_InitStructure.GPIO_Pin = TMS;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(PTMS, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = TCK;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(PTCK, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = TDI;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(PTDI, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = TDO;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(PTDO, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = RST;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(PRST, &GPIO_InitStructure);	

  DelayMs(10);	
  //SO_SetBits(RST);
}

void STC_IO_Config_Used_for_STC(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Configure STC/SPI IO */
  GPIO_InitStructure.GPIO_Pin = TMS;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(PTMS, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = TCK;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(PTCK, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = TDI;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(PTDI, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = TDO;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(PTDO, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = RST;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(PRST, &GPIO_InitStructure);	
}

void STC_IO_Config_Used_for_SWD(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Configure SWD IO */
  GPIO_InitStructure.GPIO_Pin = PIN_SWCLK;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(PORT_SWCLK, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = PIN_SWDIO;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(PORT_SWDIO, &GPIO_InitStructure);

  /*config other unused pin to input & floating*/
  GPIO_InitStructure.GPIO_Pin = TMS;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(PTMS, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = TDO;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(PTDO, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = RST;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(PRST, &GPIO_InitStructure);	
}

/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC
                         , ENABLE);   
  
  /* Configure the KEY IO as Input PULL UP */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  /* Configure Enable/Disable USB IO as Input PULL UP */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Configure the LED IO */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  STC_IO_Config_Unused();

   /* Configure USART1 Tx (PA.09) as alternate function push-pull GPIO_Pin_9*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure USART1 Rx (PA.10) as input floating  GPIO_Pin_10*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
}
/*******************************************************************************
* Function Name : EXTI_Configuration.
* Description   : Configure the EXTI lines for Key and Tamper push buttons.
* Input         : None.
* Output        : None.
* Return value  : The direction value.
*******************************************************************************/
void EXTI_Configuration(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;
   
  /* Enable the AFIO Clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  
  
  /* Connect Key to the correspondent EXTI line */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource4);
  
  /* Configure Key EXTI line to generate an interrupt on rising & falling edges */  
  EXTI_InitStructure.EXTI_Line = EXTI_Line4;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Clear the Key EXTI line pending bit */
  EXTI_ClearITPendingBit(EXTI_Line4);
  
}


/*******************************************************************************
* Function Name  : Get_SerialNum.
* Description    : Create the serial number string descriptor.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Get_SerialNum(void)
{
  u32 Device_Serial0, Device_Serial1, Device_Serial2;
  
  Device_Serial0 = *(u32*)(0x1FFFF7E8);
  Device_Serial1 = *(u32*)(0x1FFFF7EC);
  Device_Serial2 = *(u32*)(0x1FFFF7F0);
  
  if(Device_Serial0 != 0)
  {
     CustomHID_StringSerial[2] = (u8)(Device_Serial0 & 0x000000FF);  
     CustomHID_StringSerial[4] = (u8)((Device_Serial0 & 0x0000FF00) >> 8);
     CustomHID_StringSerial[6] = (u8)((Device_Serial0 & 0x00FF0000) >> 16);
     CustomHID_StringSerial[8] = (u8)((Device_Serial0 & 0xFF000000) >> 24);  
     
     CustomHID_StringSerial[10] = (u8)(Device_Serial1 & 0x000000FF);  
     CustomHID_StringSerial[12] = (u8)((Device_Serial1 & 0x0000FF00) >> 8);
     CustomHID_StringSerial[14] = (u8)((Device_Serial1 & 0x00FF0000) >> 16);
     CustomHID_StringSerial[16] = (u8)((Device_Serial1 & 0xFF000000) >> 24); 
     
     CustomHID_StringSerial[18] = (u8)(Device_Serial2 & 0x000000FF);  
     CustomHID_StringSerial[20] = (u8)((Device_Serial2 & 0x0000FF00) >> 8);
     CustomHID_StringSerial[22] = (u8)((Device_Serial2 & 0x00FF0000) >> 16);
     CustomHID_StringSerial[24] = (u8)((Device_Serial2 & 0xFF000000) >> 24); 
  }   
}

void UART_Configuration(void)
{
  USART_InitTypeDef USART_InitStructure;
  USART_InitStructure.USART_BaudRate = 76800;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  /* Configure USART1 */
  USART_Init(USART2, &USART_InitStructure);

  USART_Init(USART1, &USART_InitStructure);

  /* Enable the USART1 */
  USART_Cmd(USART2, ENABLE);

  USART_Cmd(USART1, ENABLE);
}

void UART_SetTimeOut(u32 ms)
{
	UartTimeOut=ms;
}

u32 UART_send(u32 lg,u8* data)
{
	u32 tmptime;
	u32 i=0;
	volatile u8 tmp;

	tmptime=SysTime;
	

	for(i=0;i<lg;i++)
	{
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
	    {
			if(SysTime-tmptime>UartTimeOut)
				return i;
	    }

		/* Send one byte from USART1 to USART2 */
	   	USART_SendData(USART2, data[i]);

		
	
		if(i==lg-1)
		{
			/* Loop until the end of transmit */ 
		    while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
		    {
				if(SysTime-tmptime>UartTimeOut)
					return i;
		    }
		}
	}
	tmp = USART_ReceiveData(USART2)&0xff;
	return i;
	
}

u32 UART_rev(u32 lg,u8* data)
{
	u32 tmptime;
	u32 i=0;
	tmptime=SysTime;
	while(i<lg)
	{
    	/* Loop until the USART2 Receive Data Register is not empty */
    	while(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET)
    	{
			if(SysTime-tmptime>UartTimeOut)
				return i;
    	}

		data[i++] = USART_ReceiveData(USART2)&0xff;
	}
	return i;
}

u32 UARTtest_send(u32 lg,u8* data)
{
	u32 tmptime;
	u32 i=0;
	tmptime=SysTime;
	while(i<lg)
	{
		/* Send one byte from USART1 to USART2 */
	   	USART_SendData(USART1, data[i]);
	
		/* Loop until the end of transmit */ 
	    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
	    {
			if(SysTime-tmptime>3300)
				return i;
	    }
		
		i++;
	}
	return i;
	
}

u32 UARTtest_rev(u32 lg,u8* data)
{
	u32 tmptime;
	u32 i=0;
	tmptime=SysTime;
	while(i<lg)
	{
    	/* Loop until the USART2 Receive Data Register is not empty */
    	while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
    	{
			//if(SysTime-tmptime>3300)
			//	return i;
    	}

		data[i++] = USART_ReceiveData(USART1)&0xff;
	}
	return i;
}

void DelayMs(u32 ms)
{
	u32 tmp=GetSysTick();
	while(GetSysTick()-tmp<ms);
}

void SYSTICK_Configuration(void)
{
  SysTime=0;

  HappyFlag=FALSE;

  ComDogTime=5000;
	/* SysTick end of count event each 1ms with input clock equal to 9MHz (HCLK/8, default) */
  SysTick_SetReload(9000);

  /* Enable SysTick interrupt */
  SysTick_ITConfig(ENABLE);

  /* Enable the SysTick Counter */
  SysTick_CounterCmd(SysTick_Counter_Enable);

  
}

void HappyLight(void)
{
  if(HappyMode==HAPPY_BLINK_1)
  {
  	if(!((SysTime-1)&0x1FF))
		GPIO_WriteBit(GPIOB, GPIO_Pin_5,((SysTime-1)>>9)&0x01);
  }
  else if(HappyMode==HAPPY_BLINK_2)
  {
  	if(!((SysTime-1)&0x3F))
		GPIO_WriteBit(GPIOB, GPIO_Pin_5,((SysTime-1)>>6)&0x01);
  }
}

void ComDogFeed(void)
{
	ComDogTime=0;
}

bool GetComDogDead(void)
{
	if(ComDogTime>=5000)
		return TRUE;
	else
		return FALSE;
}

void systick_proc(void)
{
	SysTime++;
	ComDogTime++;
	if(!((SysTime-1)&0x1F))	 //32ms proc
	{
		HappyLight();
	}
}

u32 GetSysTick(void)
{
	return SysTime;
}

void SetHappyLight(u32 mode)
{
  u32 tmptime;
  HappyMode=mode;
  if(HappyMode==HAPPY_ON_1S)
  {
  	 HappyFlag=TRUE;
	 tmptime=GetSysTick();
  	 GPIO_ResetBits(GPIOB, GPIO_Pin_5);
	 while(GetSysTick()-tmptime<1000);
  	 GPIO_SetBits(GPIOB, GPIO_Pin_5);
  }
}



void KEY_INT(void)
{
	//add user code
	Key_OD_Pre=TRUE;
	Key_Delay=GetSysTick();

	/* Clear the Key EXTI line pending bit */
  	EXTI_ClearITPendingBit(EXTI_Line4);
}

void Key_Confirm(void)
{
	if((Key_OD_Pre)&&(!Key_OD_Det))
	{
		if(GetSysTick()-Key_Delay>50)//Nms delay
		{
			if(!GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4))
			{
				Key_OD_Det=TRUE;
			}
		}
	}
}

bool GetKeyDown(void)
{
	return Key_OD_Det;
}

void ClearKeyDown(void)
{
	Key_OD_Det=FALSE;
	Key_OD_Pre=FALSE;
}

bool FLASH_Earse(u32 StartAddr,u32 EndAddr)
{
  vu32 NbrOfPage;
  volatile FLASH_Status FLASHStatus;
  u32 EraseCounter;

  FLASHStatus=FLASH_COMPLETE;
  /* Unlock the Flash Program Erase controller */
  FLASH_Unlock();

  /* Define the number of page to be erased */
  NbrOfPage = (EndAddr - StartAddr) / FLASH_PAGE_SIZE + 1;

  /* Clear All pending flags */
  FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	

  /* Erase the FLASH pages */
  for(EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
  {
    FLASHStatus = FLASH_ErasePage(StartAddr + (FLASH_PAGE_SIZE * EraseCounter));
	if(FLASHStatus!=FLASH_COMPLETE)
		return FALSE;
  }

  return TRUE;
}

bool FLASH_Program(u32 StartAddr,u8 * data, u32 lg)
{
	int i;
	volatile FLASH_Status FLASHStatus;

	/* Unlock the Flash Program Erase controller */
  	FLASH_Unlock();

  	/* Clear All pending flags */
  	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);

	for(i=0,FLASHStatus=FLASH_COMPLETE;(i<lg/4)&&(FLASHStatus == FLASH_COMPLETE);i++)
	{
		FLASHStatus = FLASH_ProgramWord(StartAddr+i*4, *((u32*)&data[i*4]));
		if(FLASHStatus!=FLASH_COMPLETE)
			return FALSE;	
	}


  	return TRUE;
}
 
/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
