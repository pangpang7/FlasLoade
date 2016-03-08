/**************************************************************************//**
 * @file     ARMCM0plus.h
 * @brief    CMSIS Core Peripheral Access Layer Header File for
 *           ARMCM0plus Device Series
 * @version  V2.02
 * @date     10. September 2014
 *
 * @note
 *
 ******************************************************************************/
/* Copyright (c) 2014 ARM LIMITED

   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   - Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   - Neither the name of ARM nor the names of its contributors may be used
     to endorse or promote products derived from this software without
     specific prior written permission.
   *
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
   ---------------------------------------------------------------------------*/


#ifndef ARMCM0plus_H
#define ARMCM0plus_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
  FALSE = 0, TRUE  = !FALSE
}
bool;
	
/*-------------------------  Define clocks     --------------------------------*/
#define __HSI             ( 16000000UL)
#define __XTAL            ( 16000000UL)    /* Oscillator frequency             */

#if   defined SYSCLK_FREQ_4MHz
   #define __SYSTEM_CLOCK     4000000UL
#elif defined SYSCLK_FREQ_8MHz
   #define __SYSTEM_CLOCK     8000000UL
#elif defined SYSCLK_FREQ_16MHz
   #define __SYSTEM_CLOCK    16000000UL
#elif defined SYSCLK_FREQ_24MHz
   #define __SYSTEM_CLOCK    24000000UL
#else 
   #define __SYSTEM_CLOCK    (1*__XTAL)
#endif	
		
	

/* -------------------------  Interrupt Number Definition  ------------------------ */
typedef enum IRQn
{
/* -------------------  Cortex-M0+ Processor Exceptions Numbers  ------------------ */
  NonMaskableInt_IRQn           = -14,      /*!<  2 Non Maskable Interrupt          */
  HardFault_IRQn                = -13,      /*!<  3 HardFault Interrupt             */



  SVCall_IRQn                   =  -5,      /*!< 11 SV Call Interrupt               */

  PendSV_IRQn                   =  -2,      /*!< 14 Pend SV Interrupt               */
  SysTick_IRQn                  =  -1,      /*!< 15 System Tick Interrupt           */

/* ----------------------  ARMCM0 Specific Interrupt Numbers  --------------------- */
   DMAC_IRQn            =0,
   P0_IRQn              =1,    
   TIMER1_IRQn          =2,
   UART1_IRQn      	    =3,
   SPI1_IRQn            =4,
   I2C1_IRQn            =5,
   RAM_IRQn             =6,
   ADC_IRQn             =7,
   VC_IRQn              =8,
   LVD_IRQn             =9,
   RTC_IRQn             =10,
   BASETIMER_IRQn       =11,
   P1P2_IRQn            =12,
   P3P4_IRQn            =13,
   P5P6_IRQn            =14,
   TIMER2_IRQn          =15,
   TIMER3_IRQn          =16,
   TIMER4_IRQn          =17,
   UART2_IRQn           =18,
   UART3_IRQn           =19,
   UART4_IRQn           =20,
   UART5_IRQn           =21,
   UART6_IRQn           =22,
   SPI2_IRQn            =23,
   SPI3_IRQn            =24,
   SPI4_IRQn            =25,
   I2C2_IRQn            =26,
   PCA12_IRQn           =27,
   PCA34_IRQn           =28,
   WDT_IRQn             =29,
   LCD_IRQn             =30,
   SCI7816_IRQn         =31
} IRQn_Type;


/* ================================================================================ */
/* ================      Processor and Core Peripheral Section     ================ */
/* ================================================================================ */

/* --------  Configuration of the Cortex-M0+ Processor and Core Peripherals  ------ */
#define __CM0PLUS_REV             0x0000    /*!< Core revision r0p0                              */
#define __MPU_PRESENT             0         /*!< MPU present or not                              */
#define __VTOR_PRESENT            0         /*!< VTOR present or not                             */
#define __NVIC_PRIO_BITS          2         /*!< Number of Bits used for Priority Levels         */
#define __Vendor_SysTickConfig    0         /*!< Set to 1 if different SysTick Config is used    */

#include "core_cm0plus.h"                   /* Processor and core peripherals                    */
#include "system_SI6600.h" //#include "system_ARMCM0plus.h"              /* System Header                                     */


/* ================================================================================ */
/* ================       Device Specific Peripheral Section       ================ */
/* ================================================================================ */

/* -------------------  Start of section using anonymous unions  ------------------ */
#if   defined (__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined (__ICCARM__)
  #pragma language=extended
#elif defined (__GNUC__)
  /* anonymous unions are enabled by default */
#elif defined (__TMS470__)
  /* anonymous unions are enabled by default */
#elif defined (__TASKING__)
  #pragma warning 586
#elif defined (__CSMC__)
  /* anonymous unions are enabled by default */
#else
  #warning Not supported compiler type
#endif



//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*!< SI6600 Standard Peripheral Library old types  */
typedef unsigned           char u8;
typedef unsigned short     int u16;
typedef unsigned           int u32;

typedef int32_t  s32;
typedef int16_t  s16;
typedef int8_t   s8;

typedef const int32_t sc32;  /*!< Read Only */
typedef const int16_t sc16;  /*!< Read Only */
typedef const int8_t sc8;   /*!< Read Only */

typedef __IO int32_t  vs32;
typedef __IO int16_t  vs16;
typedef __IO int8_t   vs8;

typedef __I int32_t vsc32;  /*!< Read Only */
typedef __I int16_t vsc16;  /*!< Read Only */
typedef __I int8_t vsc8;   /*!< Read Only */

typedef const uint32_t uc32;  /*!< Read Only */
typedef const uint16_t uc16;  /*!< Read Only */
typedef const uint8_t uc8;   /*!< Read Only */

typedef __IO uint32_t  vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t  vu8;

typedef __I uint32_t vuc32;  /*!< Read Only */
typedef __I uint16_t vuc16;  /*!< Read Only */
typedef __I uint8_t vuc8;   /*!< Read Only */

#define		      BIT0	          0x00000001
#define		      BIT1	          0x00000002
#define		      BIT2	          0x00000004
#define		      BIT3	          0x00000008
#define		      BIT4	          0x00000010
#define		      BIT5	          0x00000020
#define		      BIT6	          0x00000040
#define		      BIT7	          0x00000080
                                 
#define		      BIT8	          0x00000100
#define		      BIT9	          0x00000200
#define		      BIT10	          0x00000400
#define		      BIT11	          0x00000800
#define		      BIT12	          0x00001000
#define		      BIT13	          0x00002000
#define		      BIT14	          0x00004000
#define		      BIT15	          0x00008000
                                 
#define		      BIT16	          0x00010000
#define		      BIT17	          0x00020000
#define		      BIT18	          0x00040000
#define		      BIT19	          0x00080000
#define		      BIT20	          0x00100000
#define		      BIT21	          0x00200000
#define		      BIT22	          0x00400000
#define		      BIT23	          0x00800000
                                 
#define		      BIT24	          0x01000000
#define		      BIT25	          0x02000000
#define		      BIT26	          0x04000000
#define		      BIT27	          0x08000000
#define		      BIT28	          0x10000000
#define		      BIT29	          0x20000000
#define		      BIT30	          0x40000000
#define		      BIT31	          0x80000000

/////////////////////////////////////////////////////////////
//------------------------------------------PERI_CLK0 
#define   PERI_CLK0_PASSWORD    BIT31
#define   PERI_CLK0_SM4         BIT30
#define   PERI_CLK0_RNG         BIT29
#define   PERI_CLK0_AES         BIT28
#define   PERI_CLK0_DES         BIT27
#define   PERI_CLK0_ADC         BIT26
#define   PERI_CLK0_BUZZER      BIT25
#define   PERI_CLK0_PSR         BIT24

#define   PERI_CLK0_CRC         BIT23
#define   PERI_CLK0_DMA         BIT22	
#define   PERI_CLK0_FLASH       BIT21	
#define   PERI_CLK0_PCA4        BIT20	
#define   PERI_CLK0_PCA3        BIT19	
#define   PERI_CLK0_PCA2        BIT18	
#define   PERI_CLK0_PCA1        BIT17	  
#define   PERI_CLK0_TIMER4      BIT16
	
#define   PERI_CLK0_TIMER3      BIT15	
#define   PERI_CLK0_TIMER2      BIT14	
#define   PERI_CLK0_TIMER1      BIT13	
#define   PERI_CLK0_SCI7816     BIT12	
#define   PERI_CLK0_I2C2        BIT11	
#define   PERI_CLK0_I2C1        BIT10	
#define   PERI_CLK0_SPI4        BIT9	   
#define   PERI_CLK0_SPI3        BIT8	

#define   PERI_CLK0_SPI2        BIT7	
#define   PERI_CLK0_SPI1        BIT6	
#define   PERI_CLK0_UART6       BIT5	
#define   PERI_CLK0_UART5       BIT4	
#define   PERI_CLK0_UART4       BIT3	
#define   PERI_CLK0_UART3       BIT2	
#define   PERI_CLK0_UART2       BIT1
#define   PERI_CLK0_UART1       BIT0

//------------------------------------------PERI_CLK1                        
#define   PERI_CLK1_BOOT            BIT6
#define   PERI_CLK1_LCD             BIT5
#define   PERI_CLK1_WDT             BIT4
#define   PERI_CLK1_BASETIMER       BIT3
#define   PERI_CLK1_RTC             BIT2
#define   PERI_CLK1_GPIO            BIT1
#define   PERI_CLK1_BKRAM           BIT0
/////////////////////////////////////////////////////////////
//------------------------------------------PERI_RST0 
#define   PERI_RST0_PASSWORD    BIT31
#define   PERI_RST0_SM4         BIT30
#define   PERI_RST0_RNG         BIT29
#define   PERI_RST0_AES         BIT28
#define   PERI_RST0_DES         BIT27
#define   PERI_RST0_ADC         BIT26
#define   PERI_RST0_BUZZER      BIT25
#define   PERI_RST0_PSR         BIT24

#define   PERI_RST0_CRC         BIT23
#define   PERI_RST0_DMA         BIT22	
#define   PERI_RST0_FLASH       BIT21	
#define   PERI_RST0_PCA4        BIT20	
#define   PERI_RST0_PCA3        BIT19	
#define   PERI_RST0_PCA2        BIT18	
#define   PERI_RST0_PCA1        BIT17	  
#define   PERI_RST0_TIMER4      BIT16
	
#define   PERI_RST0_TIMER3      BIT15	
#define   PERI_RST0_TIMER2      BIT14	
#define   PERI_RST0_TIMER1      BIT13	
#define   PERI_RST0_SCI7816     BIT12	
#define   PERI_RST0_I2C2        BIT11	
#define   PERI_RST0_I2C1        BIT10	
#define   PERI_RST0_SPI4        BIT9	   
#define   PERI_RST0_SPI3        BIT8	

#define   PERI_RST0_SPI2        BIT7	
#define   PERI_RST0_SPI1        BIT6	
#define   PERI_RST0_UART6       BIT5	
#define   PERI_RST0_UART5       BIT4	
#define   PERI_RST0_UART4       BIT3	
#define   PERI_RST0_UART3       BIT2	
#define   PERI_RST0_UART2       BIT1
#define   PERI_RST0_UART1       BIT0

//------------------------------------------PERI_RST1                        
#define   PERI_RST1_BOOT            BIT6
#define   PERI_RST1_LCD             BIT5
#define   PERI_RST1_WDT             BIT4
#define   PERI_RST1_BASETIMER       BIT3
#define   PERI_RST1_RTC             BIT2
#define   PERI_RST1_GPIO            BIT1
#define   PERI_RST1_BKRAM           BIT0     
                                                   
//////////////////////////////////////////////////////////////port cfg
#define  PORT_CFG_GPIO           0x00
#define  PORT_CFG_UART           0x01
#define  PORT_CFG_irDA           0x02
#define  PORT_CFG_SSIM           0x03
#define  PORT_CFG_SSIS           0x04
#define  PORT_CFG_I2C            0x05
#define  PORT_CFG_7816           0x05
#define  PORT_CFG_TMG            0x06
#define  PORT_CFG_PCA            0x07
//////////////////////////////////////////////////////////////
#define NVIC_INTR_EN_DMAC_IRQ            BIT0
#define NVIC_INTR_EN_P0_IRQ              BIT1
#define NVIC_INTR_EN_TIMER1_IRQ          BIT2
#define NVIC_INTR_EN_UART1_IRQ       	   BIT3
#define NVIC_INTR_EN_SPI1_IRQ            BIT4
#define NVIC_INTR_EN_I2C1_IRQ            BIT5
#define NVIC_INTR_EN_RAM_IRQ             BIT6
#define NVIC_INTR_EN_ADC_IRQ             BIT7
#define NVIC_INTR_EN_VC_IRQ              BIT8
#define NVIC_INTR_EN_LVD_IRQ             BIT9
#define NVIC_INTR_EN_RTC_IRQ             BIT10
#define NVIC_INTR_EN_BASETIMER_IRQ       BIT11
#define NVIC_INTR_EN_P1P2_IRQ            BIT12
#define NVIC_INTR_EN_P3P4_IRQ            BIT13
#define NVIC_INTR_EN_P5P6_IRQ            BIT14
#define NVIC_INTR_EN_TIMER2_IRQ          BIT15
#define NVIC_INTR_EN_TIMER3_IRQ          BIT16
#define NVIC_INTR_EN_TIMER4_IRQ          BIT17
#define NVIC_INTR_EN_UART2_IRQ           BIT18
#define NVIC_INTR_EN_UART3_IRQ           BIT19
#define NVIC_INTR_EN_UART4_IRQ           BIT20
#define NVIC_INTR_EN_UART5_IRQ           BIT21
#define NVIC_INTR_EN_UART6_IRQ           BIT22
#define NVIC_INTR_EN_SPI2_IRQ            BIT23
#define NVIC_INTR_EN_SPI3_IRQ            BIT24
#define NVIC_INTR_EN_SPI4_IRQ            BIT25
#define NVIC_INTR_EN_I2C2_IRQ            BIT26
#define NVIC_INTR_EN_PCA12_IRQ           BIT27
#define NVIC_INTR_EN_PCA34_IRQ           BIT28
#define NVIC_INTR_EN_WDT_IRQ             BIT29
#define NVIC_INTR_EN_LCD_IRQ             BIT30
#define NVIC_INTR_EN_SCI7816_IRQ         BIT31

#define UART_1_TX_dmaID                  0x00
#define UART_1_RX_dmaID                  0x01
#define UART_2_TX_dmaID                  0x02
#define UART_2_RX_dmaID                  0x03
#define UART_3_TX_dmaID                  0x04
#define UART_3_RX_dmaID                  0x05
#define UART_4_TX_dmaID                  0x06
#define UART_4_RX_dmaID                  0x07
#define UART_5_TX_dmaID                  0x08
#define UART_5_RX_dmaID                  0x09
#define UART_6_TX_dmaID                  0x0a
#define UART_6_RX_dmaID                  0x0b
#define I2C_1_TX_dmaID                   0x0c
#define I2C_1_RX_dmaID                   0x0d
#define I2C_2_TX_dmaID                   0x0e
#define I2C_2_RX_dmaID                   0x0f
#define SSIM_1_TX_dmaID                  0x10
#define SSIM_1_RX_dmaID                  0x11
#define SSIM_2_TX_dmaID                  0x12
#define SSIM_2_RX_dmaID                  0x13
#define SSIM_3_TX_dmaID                  0x14
#define SSIM_3_RX_dmaID                  0x15
#define SSIM_4_TX_dmaID                  0x16
#define SSIM_4_RX_dmaID                  0x17
#define SSIS_1_TX_dmaID                  0x18
#define SSIS_1_RX_dmaID                  0x19
#define SSIS_2_TX_dmaID                  0x1a
#define SSIS_2_RX_dmaID                  0x1b
#define SSIS_3_TX_dmaID                  0x1c
#define SSIS_3_RX_dmaID                  0x1d
#define SSIS_4_TX_dmaID                  0x1e
#define SSIS_4_RX_dmaID                  0x1f


typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;

typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

typedef enum {ERROR = 1, SUCCESS = !ERROR} ErrorStatus;


/* ================================================================================ */
/* ================              Peripheral memory map             ================ */
/* ================================================================================ */
#define UART_1_BASEADDR       (0x40000000)
#define UART_2_BASEADDR       (0x40000400)
#define UART_3_BASEADDR       (0x40000800)	
#define I2C_1_BASEADDR        (0x40000C00)	
#define SSIM_1_BASEADDR       (0x40001000)
#define SSIM_2_BASEADDR       (0x40001400)		
#define SSIS_1_BASEADDR       (0x40001800)	
#define SSIS_2_BASEADDR       (0x40001C00)
#define HS_MUX_BASEADDR       (0x40002000)
#define SCI7816_BASEADDR      (0x40002800)
#define TIMER_1_BASEADDR      (0x40002C00)					
#define TIMER_2_BASEADDR      (0x40002C14)			
#define TIMER_3_BASEADDR      (0x40002C28)			
#define TIMER_4_BASEADDR      (0x40002C3C)	
	
#define RNG_BASEADDR          (0x40003000)	
#define LCD_BASEADDR          (0x40003400)
#define UART_4_BASEADDR       (0x40010000)
#define UART_5_BASEADDR       (0x40010400)
#define UART_6_BASEADDR       (0x40010800)
#define I2C_2_BASEADDR        (0x40010C00)
#define SSIM_3_BASEADDR       (0x40011000)
#define SSIM_4_BASEADDR       (0x40011400)
#define SSIS_3_BASEADDR       (0x40011800) 
#define SSIS_4_BASEADDR       (0x40011C00)
#define RTC_BASEADDR          (0x40012000)
#define WDT_BASEADDR          (0x40012400)
#define BASETIMER_BASEADDR    (0x40012800)
		
#define PORT_0_BASEADDR       (0x40012C00)
#define PORT_1_BASEADDR       (0x40012C40)
#define PORT_2_BASEADDR       (0x40012C80)
#define PORT_3_BASEADDR       (0x40012CC0)
#define PORT_4_BASEADDR       (0x40012D00)
#define PORT_5_BASEADDR       (0x40012D40)
#define PORT_6_BASEADDR       (0x40012D80)
#define PCA_1_BASEADDR        (0x40013000)
#define PCA_2_BASEADDR        (0x40013050)				  
#define PCA_3_BASEADDR        (0x400130A0)					
#define PCA_4_BASEADDR        (0x400130F0)
#define ANALOG_CTRL_BASEADDR  (0x40013400)
#define EFLASH_CTRL_BASEADDR  (0x40020000)
#define PMU_CTRL_BASEADDR     (0x40020400)		
#define DMAC_BASEADDR         (0x40020C00)
//channel 0 register		
    #define DMAC_SAR0           *((volatile unsigned int *)(DMAC_BASEADDR+0x000))	
	  #define DMAC_DAR0           *((volatile unsigned int *)(DMAC_BASEADDR+0x008))
	  #define DMAC_LLP0           *((volatile unsigned int *)(DMAC_BASEADDR+0x010))			
    #define DMAC_CTL0_L         *((volatile unsigned long*)(DMAC_BASEADDR+0x018))	
    #define DMAC_CTL0_H         *((volatile unsigned long*)(DMAC_BASEADDR+0x01c))			
	  #define DMAC_CFG0           *((volatile unsigned long long*)(DMAC_BASEADDR+0x040))			
		#define DMAC_CFG0_L         *((volatile unsigned long*)(DMAC_BASEADDR+0x040))
		#define DMAC_CFG0_H         *((volatile unsigned long*)(DMAC_BASEADDR+0x044))			
		
//common register	
    #define DMAC_CFG              *((volatile unsigned long *)(DMAC_BASEADDR+0x398))	
    #define DMAC_CH_EN            *((volatile unsigned long *)(DMAC_BASEADDR+0x3a0))	

	  #define DMAC_STATUS_TFR       *((volatile unsigned long *)(DMAC_BASEADDR+0x2e8))
	  #define DMAC_STATUS_BLK       *((volatile unsigned long *)(DMAC_BASEADDR+0x2f0))
	  #define DMAC_STATUS_SRCTRAN   *((volatile unsigned long *)(DMAC_BASEADDR+0x2f8))
	  #define DMAC_STATUS_DSTTRAN   *((volatile unsigned long *)(DMAC_BASEADDR+0x300))
	  #define DMAC_STATUS_ERR       *((volatile unsigned long *)(DMAC_BASEADDR+0x308))
			
    #define DMAC_CLR_TFR          *((volatile unsigned long *)(DMAC_BASEADDR+0x338))	
    #define DMAC_CLR_BLK          *((volatile unsigned long *)(DMAC_BASEADDR+0x340))
    #define DMAC_CLR_SRCTRAN      *((volatile unsigned long *)(DMAC_BASEADDR+0x348))
    #define DMAC_CLR_DSTTRAN      *((volatile unsigned long *)(DMAC_BASEADDR+0x350))
    #define DMAC_CLR_ERR          *((volatile unsigned long *)(DMAC_BASEADDR+0x358))
	   				
	  #define DMAC_MASK_TFR        *((volatile unsigned long *)(DMAC_BASEADDR+0x310))
	  #define DMAC_MASK_BLK        *((volatile unsigned long *)(DMAC_BASEADDR+0x318))
	  #define DMAC_MASK_SRCTRAN    *((volatile unsigned long *)(DMAC_BASEADDR+0x320))
	  #define DMAC_MASK_DSTTRAN    *((volatile unsigned long *)(DMAC_BASEADDR+0x328))
	  #define DMAC_MASK_ERR        *((volatile unsigned long *)(DMAC_BASEADDR+0x330))	
			
#define HS_MUX_BASEADDR      (0x40002000)
		#define HS0_CFG            *((volatile unsigned int  *)(HS_MUX_BASEADDR+0x00))
		#define HS1_CFG            *((volatile unsigned int  *)(HS_MUX_BASEADDR+0x04))
		#define HS2_CFG            *((volatile unsigned int  *)(HS_MUX_BASEADDR+0x08))
		#define HS3_CFG            *((volatile unsigned int  *)(HS_MUX_BASEADDR+0x0c))
#define AES_BASEADDR          (0x40021C00)					
#define CRC_BASEADDR          (0x40021000)			

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//#define ANALOG_CTRL_BASEADDR  (0x40013400)
	  #define  SYSTEM_CTRL0		      *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0x00))
		#define  SYSTEM_CTRL1		      *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0x04))
		#define  PMU_CTRL    		      *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0x08))
		#define  IRC16M_CTRL 		      *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0x0C))
		#define  X32M_CTRL			      *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0x10))
		#define  IRC32K_CTRL 		      *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0x14))
		#define  X32K_CTRL 		        *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0x18))
		#define  RESETFLAG   		      *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0x20))
		#define  BGR_CTRL    		      *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0x28))
		#define  LVD_CTRL0   		      *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0x2C))
		#define  LVD_CTRL1   		      *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0x30))
		#define  VC_CTRL0    		      *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0x34))
		#define  VC_CTRL1    		      *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0x38))
		#define  PERI_CLK_CTRL0       *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0x40))
		#define  PERI_CLK_CTRL1	      *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0x44))
		#define  PERI_RST_CTRL0	      *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0x48))
		#define  PERI_RST_CTRL1	      *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0x4C))
		#define  CLK_TRIM_CTRL		    *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0x50))
		#define  CLK_CALIB_CNT    	  *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0x54))
		#define  CLK_REFER_CNT  	    *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0x58))
		#define  ADC_CTRL0   		      *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0x60))
		#define  ADC_CTRL1   		      *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0x64))
		#define  ADC_CTRL2   		      *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0x68))
		#define  ADC_RESULT_ACC       *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0x6C))
		#define  DEBUG_ACTIVE         *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0x70))
		#define  BUZZER               *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0x74))
		#define  TICK_TIMER           *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0x7C))
		

		#define  ADC_RESULT0  		    *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0x80))
		#define  ADC_RESULT1  		    *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0x84))			
		#define  ADC_RESULT2  		    *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0x88))
		#define  ADC_RESULT3  		    *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0x8C))
		#define  ADC_RESULT4  		    *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0x90))	
		#define  ADC_RESULT5  		    *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0x94))
		#define  ADC_RESULT6  	  	  *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0x98))
		#define  ADC_RESULT7  	  	  *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0x9C))
		#define  BACKUP_RAM0  	  	  *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0xC0))	
		#define  BACKUP_RAM1  	  	  *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0xC4))		
		#define  BACKUP_RAM2  	  	  *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0xC8))
		#define  BACKUP_RAM3  	  	  *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0xCC))
		#define  BACKUP_RAM4  	  	  *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0xD0))		
		#define  BACKUP_RAM5  	  	  *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0xD4))
		#define  BACKUP_RAM6  	  	  *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0xD8))
		#define  BACKUP_RAM7  	  	  *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0xDC))
		#define  BACKUP_RAM8  	  	  *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0xE0))
		#define  BACKUP_RAM9  	  	  *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0xE4))
		#define  BACKUP_RAMA  	  	  *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0xE8))
		#define  BACKUP_RAMB  	  	  *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0xEC))
		#define  BACKUP_RAMC  		    *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0xF0))
		#define  BACKUP_RAMD  		    *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0xF4))	
		#define  BACKUP_RAME  		    *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0xF8))
		#define  BACKUP_RAMF  		    *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0xFC))
		#define  LOCK_SFR     		    *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0x100))
		#define  BOOT0		    	      *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0x104))
		#define  BOOT1		            *((volatile unsigned int  *)(ANALOG_CTRL_BASEADDR+0x108))
		

			
/* ================================================================================ */
/* ================            UART                                ================ */
/* ================================================================================ */
typedef struct
{
    __IO u32  RBR;          
	//__IO u32  THR;         
	//__IO u32  DLL;         
	//__IO u32  DLH;         
		__IO u32  IER;         
		__IO u32  IIR;         
  //__IO u32  FCR;         
		__IO u32  LCR;         
		__IO u32  MCR;         
		__IO u32  LSR;
		__IO u32  MSR; 
			   u32  RESERVED0[24];         
		__IO u32  USR;           
		__IO u32  TFL;  
		__IO u32  RFL;        		        
}UART_TypeDef;

#define THR    RBR
#define DLL    RBR
#define DLH    IER
#define FCR    IIR

#define UART_1               ((UART_TypeDef *) UART_1_BASEADDR)
#define UART_2               ((UART_TypeDef *) UART_2_BASEADDR)
#define UART_3               ((UART_TypeDef *) UART_3_BASEADDR)
#define UART_4               ((UART_TypeDef *) UART_4_BASEADDR)
#define UART_5               ((UART_TypeDef *) UART_5_BASEADDR)
#define UART_6               ((UART_TypeDef *) UART_6_BASEADDR)	

///********************  UART IER register  **************************************/
#define  UART_IER_PTIME         (0x80)       /*Programmable THRE Interrupt Mode Enable*/
#define  UART_IER_EDSSI         (0x08)       /*Enable Modem Status Interrupt*/
#define  UART_IER_ELSI          (0x04)       /*Enable Receiver Line Status Interrupt*/
#define  UART_IER_ETBEI         (0x02)       /*Enable Transmit Holding Register Empty Interrupt */
#define  UART_IER_ERBFI         (0x01)       /*Enable Received Data Available Interrupt */

///********************  Interrupt Identity Register  ****************************/


///********************  FIFO Control Register  **********************************/
#define  UART_FCR_RT_1char          (0x00)       /*RCVR Trigger,1 character in the FIFO*/
#define  UART_FCR_RT_qurfull        (0x20)       /*RCVR Trigger,FIFO 1/4 full*/
#define  UART_FCR_RT_hlffull        (0x40)       /*RCVR Trigger,FIFO 1/2 full*/
#define  UART_FCR_RT_2lsfull        (0xc0)       /*RCVR Trigger,FIFO 2 less than full*/
#define  UART_FCR_TET_fifoept       (0x00)       /*TX Empty Trigger,FIFO empty*/
#define  UART_FCR_TET_2char         (0x20)       /*TX Empty Trigger,2 characters in the FIFO*/
#define  UART_FCR_TET_qurfull       (0x40)       /*TX Empty Trigger,FIFO 1/4 full*/
#define  UART_FCR_TET_hlffull       (0xc0)       /*TX Empty Trigger,FIFO 1/2 full*/
#define  UART_FCR_DMAM_mode0        (0x00)       /*DMA Mode,mode 0*/
#define  UART_FCR_DMAM_mode1        (0x00)       /*DMA Mode,mode 1*/
#define  UART_FCR_XFIFOR            (0x04)       /*DMA Mode,mode 0*/
#define  UART_FCR_RFIFOR            (0x02)       /*DMA Mode,mode 0*/
#define  UART_FCR_FIFOE             (0x01)       /*DMA Mode,mode 0*/

///********************  LCR Register           **********************************/
//#define	 UART_LCR_DLAB		         BIT7		     //Divisor Latch Access Bit.
//#define	 UART_LCR_BreakCtrl		     BIT6		     //Break (or BC) R/W Break Control Bit.
//#define	 UART_LCR_StickParity	     BIT5		     //parity bit is transmitted and checked as logic 0
//#define	 UART_LCR_EvenParitySel    BIT4		     //EPS R/W Even Parity Select
//#define	 UART_LCR_ParityEnable		 BIT3		 //PEN R/W Parity Enable
//#define	 UART_LCR_STOP_1bit		     BIT2		 //STOP R/W Number of stop bits
//#define	 UART_LCR_STOP_1p5bit
//#define	 UART_LCR_DLS_5bit	    UART_1_LCR &=~(BIT1+BIT0);             //00 5bit
//#define	 UART_LCR_DLS_6bit	    UART_1_LCR &=(~BIT1)|(BIT0);					 //01 6bit
//#define	 UART_LCR_DLS_7bit	    UART_1_LCR &=(~BIT0)|(BIT1);					 //10 7bit
//#define	 UART_LCR_DLS_8bit	    UART_1_LCR |=BIT1+ BIT0;					     //11 8bit


/////********************  Modem Control Register  **********************************/
////UART_1_MCR   Modem Control Register
//#define	 UART_MCR_SIREnable         BIT6   //6 SIRE R/W SIR Mode Enable.
//#define	 UART_MCR_AutoFlwCtrl       BIT5   //5 AFCE R/W Auto Flow Control Enable.
//#define	 UART_MCR_LoopBack          BIT4   //4 LoopBack R/W LoopBack Bit.
//#define	 UART_MCR_OUT2              BIT3   //3 OUT2 R/W OUT2.
//#define	 UART_MCR_OUT1              BIT2   //2 OUT1 R/W OUT1
//#define	 UART_MCR_RTS               BIT1   //1 RTS R/W Request to Send.
//#define	 UART_MCR_DTR               BIT0   //0 DTR R/W Data Terminal Ready


///********************  LSR Register  **********************************/
#define	 UART_LSR_RecvErr             BIT7  //Receiver FIFO Error bit
#define	 UART_LSR_TxEmpty             BIT6  //Transmitter Empty bit.
#define	 UART_LSR_THREmpty            BIT5  //Transmit Holding Register Empty
#define	 UART_LSR_BreakIntr           BIT4  //Break Interrupt bit.
#define	 UART_LSR_FrameErr            BIT3  //Framing Error bit.
#define	 UART_LSR_ParityErr           BIT2  //Parity Error bit.
#define	 UART_LSR_OverrunErr          BIT1  //Overrun error bit.
#define	 UART_LSR_RecvDataRdy         BIT0  //Data Ready bit. receiver contains at least 1 character in RBR or recv FIFO

/* ================================================================================ */
/* ================            I2C                                 ================ */
/* ================================================================================ */
typedef struct
{
	__IO u32 CON;            
	__IO u32 TAR;            
	__IO u32 SAR;            
	__IO u32 HS_MADDR;       
	__IO u32 DATA_CMD;       
	__IO u32 SS_HCNT;        
	__IO u32 SS_LCNT;        
	__IO u32 FS_HCNT;        
	__IO u32 FS_LCNT;        
	__IO u32 HS_HCNT;        
	__IO u32 HS_LCNT;        
	__IO u32 INTR_STAT;      
	__IO u32 INTR_MASK;      
	__IO u32 RAW_INTR_STAT;  
	__IO u32 RX_TL;          
	__IO u32 TX_TL;          
	__IO u32 CLR_INTR;       
	__IO u32 CLR_RX_UNDER;   
	__IO u32 CLR_RX_OVER;    
	__IO u32 CLR_TX_OVER;    
	__IO u32 CLR_RD_REQ;     
	__IO u32 CLR_TX_ABRT;    
	__IO u32 CLR_RX_DONE;    
	__IO u32 CLR_ACTIVITY;   
	__IO u32 CLR_STOP_DET;   
	__IO u32 CLR_START_DET;  
	__IO u32 CLR_GEN_CALL;   
	__IO u32 ENABLE;         
	__IO u32 STATUS;         
	__IO u32 TXFLR;          
	__IO u32 RXFLR;          
	__IO u32 TX_ABRT_SOURCE; 
	__IO u32 DMA_CR;         
	__IO u32 DMA_TDLR;       
	__IO u32 DMA_RDLR;       
	__IO u32 SDA_SETUP; 	    
	__IO u32 ACK_GENERAL_CALL; 
	__IO u32 ENABLE_STATUS;    
	__IO u32 FS_SPKLEN;        
	__IO u32 HS_SPKLEN;
	     u32 RESERVED0[21];       
	__IO u32 PARAM_1; 	         
	__IO u32 COMP_VERSION;     
	__IO u32 COMP_TYPE;        
        
}I2C_TypeDef;
#define I2C_1               ((I2C_TypeDef *) I2C_1_BASEADDR)
#define I2C_2               ((I2C_TypeDef *) I2C_2_BASEADDR)

///********************  IC_INTR_STAT/IC_INTR_MASK/IC_RAW_INTR_STAT Register           **********************************/
#define	   I2C_INTR_RXUNDER      BIT0	     //read EmptyRxFIFO
#define	   I2C_INTR_RXOVER       BIT1	     //recv Over RxFIFOdepth
#define	   I2C_INTR_RXFULL       BIT2
#define	   I2C_INTR_TXOVER       BIT3	     //fill Over TxFIFOdepth
#define	   I2C_INTR_TXEMPTY      BIT4
#define	   I2C_INTR_RDREQ        BIT5	     //slv mode,a master is attempting to read data
#define	   I2C_INTR_TXABRT       BIT6
#define	   I2C_INTR_RXDONE       BIT7	     //slv mode,after recv NACK,Slv_transmit
#define	   I2C_INTR_ACTIVITY     BIT8	     //
#define	   I2C_INTR_STOPDET      BIT9	     //
#define	   I2C_INTR_STARTDET     BIT10	   //Indicate Re-/Start Det,MST or SLV mode 
#define	   I2C_INTR_GENCALL      BIT11	   //
#define	   I2C_INTR_RESTARTDET   BIT12	   //slv mode,Re-start Det Indicated (SAR addrerred,and IC_SLV_RESTART_DET_EN = 1)
#define	   I2C_INTR_MSTONHOLD    BIT13	   //

///********************  IC_DATA_CMD           **********************************/
#define		I2C_DATACMD_Read       BIT8
#define		I2C_DATACMD_Stop       BIT9
#define		I2C_DATACMD_Restart    BIT10


/* ================================================================================ */
/* ================            SSI                                 ================ */
/* ================================================================================ */
typedef struct
{
	__IO u32 CTRLR0;    
	__IO u32 CTRLR1;    
	__IO u32 SSIENR;    
	__IO u32 MWCR;      
	__IO u32 SER;       
	__IO u32 BAUDR;     
	__IO u32 TXFTLR;    
	__IO u32 RXFTLR;    
	__IO u32 TXFLR;     
	__IO u32 RXFLR;     
	__IO u32 SR;        
	__IO u32 IMR;       
	__IO u32 ISR;       
	__IO u32 RISR;      
	__IO u32 TXOICR;    
	__IO u32 RXOICR;    
	__IO u32 RXUICR;    
	__IO u32 MSTICR;    
	__IO u32 ICR;       
	__IO u32 DMACR;     
	__IO u32 DMATDLR;   
	__IO u32 DMARDLR;   
	__IO u32 IDR;    
	__IO u32 VERSION_ID;
	__IO u32 DR_LOW;    
	__IO u32 DR_HIGH; 
	  	 u32 RESERVED0[34];
	__IO u32 RX_SAMPLE; 
	__IO u32 RSVD_0;    
	__IO u32 RSVD_1;    
	__IO u32 RSVD_2;               
}SSI_TypeDef;

#define SSIM_1               ((SSI_TypeDef *) SSIM_1_BASEADDR)
#define SSIM_2               ((SSI_TypeDef *) SSIM_2_BASEADDR)
#define SSIM_3               ((SSI_TypeDef *) SSIM_3_BASEADDR)
#define SSIM_4               ((SSI_TypeDef *) SSIM_4_BASEADDR)

#define SSIS_1               ((SSI_TypeDef *) SSIS_1_BASEADDR)
#define SSIS_2               ((SSI_TypeDef *) SSIS_2_BASEADDR)
#define SSIS_3               ((SSI_TypeDef *) SSIS_3_BASEADDR)
#define SSIS_4               ((SSI_TypeDef *) SSIS_4_BASEADDR)
/* ================================================================================ */
/* ================            SCI7816                             ================ */
/* ================================================================================ */
typedef struct
{   
	__IO u32 CON0;         
	__IO u32 MODE;           
	__IO u32 STATUS;          
	__IO u32 FIFO_STATUS;     
	__IO u32 FIFO_DEPTH;     
	__IO u32 DATA;            
	__IO u32 EDC_DATA;       
	__IO u32 SBDR;            
	__IO u32 ETU_CNT;        
	__IO u32 CLK_RST;         
}SCI7816_TypeDef;

#define SCI7816               ((SCI7816_TypeDef *) SCI7816_BASEADDR)

///********************  STATUS Register           **********************************/
#define	   STATUS_ETU_CNT     BIT4	   //eut_cnt int flg / etu_cnt int enable
#define	   STATUS_TX_ERR      BIT3	   //eut_cnt int flg / etu_cnt int enable
#define	   STATUS_TX_FIN      BIT2	   //eut_cnt int flg / etu_cnt int enable
#define	   STATUS_RX_ERR      BIT1	   //eut_cnt int flg / etu_cnt int enable
#define	   STATUS_RX_FIN      BIT0	   //eut_cnt int flg / etu_cnt int enable

///********************  INT_EN Register           **********************************/
#define	   INTEN_ETU_CNT     BIT8	   //eut_cnt int flg / etu_cnt int enable
#define	   INTEN_TX_ERR      BIT7	   //eut_cnt int flg / etu_cnt int enable
#define	   INTEN_TX_FIN      BIT6	   //eut_cnt int flg / etu_cnt int enable
#define	   INTEN_RX_ERR      BIT5	   //eut_cnt int flg / etu_cnt int enable
#define	   INTEN_RX_FIN      BIT4	   //eut_cnt int flg / etu_cnt int enable


///* ================================================================================ */
///* ================            TIMER + BASETIMER                   ================ */
///* ================================================================================ */
typedef struct
{
    __IO u32  LOADCOUNT;
		__IO u32  CURVAL;
		__IO u32  CTRL;
		__IO u32  EOI; 
		__IO u32  INTRSTATUS;
}TIMER_TypeDef;

#define TIMER_1               ((TIMER_TypeDef *)   TIMER_1_BASEADDR)
#define TIMER_2               ((TIMER_TypeDef *)   TIMER_2_BASEADDR)
#define TIMER_3               ((TIMER_TypeDef *)   TIMER_3_BASEADDR)
#define TIMER_4               ((TIMER_TypeDef *)   TIMER_4_BASEADDR)
#define BASETIMER             ((TIMER_TypeDef *) BASETIMER_BASEADDR)

#define   TIMER_1_LOADCOUNT2 *((volatile unsigned int  *)(  TIMER_1_BASEADDR+0xb0))
#define   TIMER_2_LOADCOUNT2 *((volatile unsigned int  *)(  TIMER_1_BASEADDR+0xb4))
#define   TIMER_3_LOADCOUNT2 *((volatile unsigned int  *)(  TIMER_1_BASEADDR+0xb8))
#define   TIMER_4_LOADCOUNT2 *((volatile unsigned int  *)(  TIMER_1_BASEADDR+0xbc))
#define BASETIMER_LOADCOUNT2 *((volatile unsigned int  *)(BASETIMER_BASEADDR+0xb0))

#define TIMERS_INT_STAT      *((volatile unsigned int  *)(  TIMER_1_BASEADDR+0xa0))
#define TIMERS_EOI           *((volatile unsigned int  *)(  TIMER_1_BASEADDR+0xa4))
#define TIMERS_GATE          *((volatile unsigned int  *)(  TIMER_1_BASEADDR+0xd0))  //(bit1,bit0: ct,gate)*4
	
#define BTIMERS_INT_STAT     *((volatile unsigned int  *)(BASETIMER_BASEADDR+0xa0))
#define BTIMERS_EOI          *((volatile unsigned int  *)(BASETIMER_BASEADDR+0xa4))				
#define BTIMERS_GATE         *((volatile unsigned int  *)(BASETIMER_BASEADDR+0xd0))  //bit1,bit0: ct,gate	

/* ================================================================================ */
/* ================            RNG                                 ================ */
/* ================================================================================ */

/* ================================================================================ */
/* ================            CRC                                 ================ */
/* ================================================================================ */
typedef struct
{
    __IO u32  CRC_CTRL;
    __IO u32  CRC_RESULT;
    __IO u32  CRC_RAND;
    __IO u32  CRC_FLAG;
    __IO u32  CRC_DATA;
    __IO u32  CRC_DATA16;
}CRC_TypeDef;

#define CRC               ((CRC_TypeDef *)   CRC_BASEADDR)

/* ================================================================================ */
/* ================            DES                                 ================ */
/* ================================================================================ */
typedef struct
{
    __IO u32  DES_CTRL;
    __IO u32  DES_RAND;
    __IO u32  DES_IV_L;
    __IO u32  DES_IV_H;
    __IO u32  DES_DATA_L;
    __IO u32  DES_DATA_H;
    __IO u32  DES_KEY1_L;
    __IO u32  DES_KEY1_H;
    __IO u32  DES_KEY2_L;
    __IO u32  DES_KEY2_H;
    __IO u32  DES_KEY3_L;
    __IO u32  DES_KEY3_H;
}DES_TypeDef;

#define DES              ((DES_TypeDef *)   DES_BASEADDR)
/* ================================================================================ */
/* ================            SM4                                 ================ */
/* ================================================================================ */
typedef struct
{
    __IO u32  SM4_CTRL;
    __IO u32  SM4_RAND1;
    __IO u32  SM4_RAND2;
    __IO u32  SM4_DATA1;
    __IO u32  SM4_DATA2;
    __IO u32  SM4_DATA3;
    __IO u32  SM4_DATA4;
    __IO u32  SM4_MKEY1;
    __IO u32  SM4_MKEY2;
    __IO u32  SM4_MKEY3;
    __IO u32  SM4_MKEY4;
    __IO u32  SM4_SKEY1;
    __IO u32  SM4_SKEY2;
    __IO u32  SM4_SKEY3;
    __IO u32  SM4_SKEY4;
}SM4_TypeDef;

#define SM4              ((SM4_TypeDef *)   SM4_BASEADDR)
		
/* ================================================================================ */
/* ================            LCD                                 ================ */
/* ================================================================================ */
typedef struct
{
    __IO u32 LCDRAM0L;        
	  __IO u32 LCDRAM1L;
		__IO u32 LCDRAM2L;	
		__IO u32 LCDRAM3L;
		__IO u32 LCDRAM4L;        
	  __IO u32 LCDRAM5L;
		__IO u32 LCDRAM6L;	
		__IO u32 LCDRAM7L;
    __IO u32 LCDRAM0H;        
	  __IO u32 LCDRAM1H;
		__IO u32 LCDRAM2H;	
		__IO u32 LCDRAM3H;
		__IO u32 LCDRAM4H;        
	  __IO u32 LCDRAM5H;
		__IO u32 LCDRAM6H;	
		__IO u32 LCDRAM7H;
		__IO u32 LCDCR;
	
}LCD_TypeDef;
#define LCD                  ((LCD_TypeDef *)    LCD_BASEADDR)
/* ================================================================================ */
/* ================            RTC                                ================ */
/* ================================================================================ */
typedef struct
{
    __IO u32 SIXTEENOFS;        
	  __IO u32 SECOND;           
	  __IO u32 MINUTE;           
	  __IO u32 HOUR;             
	  __IO u32 DAY;              
	  __IO u32 MONTH;            
	  __IO u32 YEARL;            
	  __IO u32 YEARH;            
	  __IO u32 SIXTEENOFS_DIS;   
	  __IO u32 SECOND_DIS;       
	  __IO u32 MINUTE_DIS;       
	  __IO u32 HOUR_DIS;         		
	  __IO u32 DAY_DIS;          
    __IO u32 MONTH_DIS;         
    __IO u32 YEARH_DIS;         
    __IO u32 YEARL_DIS;         		
	  __IO u32 ADJUST_INC;       
	  __IO u32 ADJUST_DEC;       
	  __IO u32 FORMAT;           
	  __IO u32 INTERVAL;
    __IO u32 EIGHT0;
    __IO u32 EIGHT1;		
	  __IO u32 CONFIG0;          
	  __IO u32 CONFIG1;          
	  __IO u32 PHASE;            
	  __IO u32 HOLD; 
    __IO u32 RTC_RESET;		
	  __IO u32 RTCINTERRUPT;        		        
}RTC_TypeDef;
#define RTC               ((RTC_TypeDef *) RTC_BASEADDR)
/* ================================================================================ */
/* ================            WDT                                 ================ */
/* ================================================================================ */
typedef struct
{       
    __IO u32 CR;           
		__IO u32 TORR;        
		__IO u32 CCVR; 		    
	  __IO u32 CRR; 		      
		__IO u32 STAT;         
		__IO u32 EOI;			          		        
}WDT_TypeDef;
#define WDT               ((WDT_TypeDef *) WDT_BASEADDR)

/* ================================================================================ */
/* ================            GPIO                                ================ */
/* ================================================================================ */

typedef struct
{
    __IO u32  Pn;
		__IO u32  Pn_DIR;
		__IO u32  Pn_PULSE_SEL;
		__IO u32  Pn_EDGE_SEL; 
		__IO u32  Pn_IE; 
		__IO u32  Pn_IFG;
		__IO u32  Pn_PH; 
		__IO u32  Pn_DS;
		__IO u32  Pn_OPENDRAIN;
		__IO u32  Pn_SEL0;
		__IO u32  Pn_SEL1;
		__IO u32  Pn_SEL2;       		        
}PORT_TypeDef;

#define PORT_0               ((PORT_TypeDef *) PORT_0_BASEADDR)
#define PORT_1               ((PORT_TypeDef *) PORT_1_BASEADDR)
#define PORT_2               ((PORT_TypeDef *) PORT_2_BASEADDR)
#define PORT_3               ((PORT_TypeDef *) PORT_3_BASEADDR)
#define PORT_4               ((PORT_TypeDef *) PORT_4_BASEADDR)
#define PORT_5               ((PORT_TypeDef *) PORT_5_BASEADDR)
#define PORT_6               ((PORT_TypeDef *) PORT_6_BASEADDR)

/* ================================================================================ */
/* ================            PCA                                 ================ */
/* ================================================================================ */
typedef struct
{
	__IO u32 PCA_CCON;         
	__IO u32 PCA_CMOD;         
	__IO u32 PCA_CH;           
	__IO u32 PCA_CL;           
	__IO u32 PCA_CCAPM0;       
	__IO u32 PCA_CCAPM1;      
	__IO u32 PCA_CCAPM2;       
	__IO u32 PCA_CCAPM3;       
	__IO u32 PCA_CCAPM4;       
	__IO u32 PCA_CCAP0H;       
	__IO u32 PCA_CCAP0L;       
	__IO u32 PCA_CCAP1H;       
	__IO u32 PCA_CCAP1L;       
	__IO u32 PCA_CCAP2H;       
	__IO u32 PCA_CCAP2L;       
	__IO u32 PCA_CCAP3H;       
	__IO u32 PCA_CCAP3L;       
	__IO u32 PCA_CCAP4H;       
	__IO u32 PCA_CCAP4L;       
	__IO u32 PCA_CCAPO;             
        
}PCA_TypeDef;
#define PCA_1               ((PCA_TypeDef *) PCA_1_BASEADDR)
#define PCA_2               ((PCA_TypeDef *) PCA_2_BASEADDR)
#define PCA_3               ((PCA_TypeDef *) PCA_3_BASEADDR)
#define PCA_4               ((PCA_TypeDef *) PCA_4_BASEADDR)

#define     PCA_CCAPMx_CaptRisEdge        0x20
#define     PCA_CCAPMx_CaptFalEdge        0x10
#define     PCA_CCAPMx_CaptRisFalEdge     0x30
#define     PCA_CCAPMx_SoftwareTimer      0x48
#define     PCA_CCAPMx_HighSpeedOut       0x4c
#define     PCA_CCAPMx_8BitPWMOut         0x42
#define     PCA_CCAPMx_WDT                0x48
/* ================================================================================ */
/* ================            FLASH                                ================ */
/* ================================================================================ */

typedef struct
{
    __IO u32  TNVS;
    __IO u32  TPGS;
    __IO u32  TPROG;
    __IO u32  TNVH;
    __IO u32  TRCV;
    __IO u32  TERASE;
    __IO u32  TME;
    __IO u32  TNVH1;			
    __IO u32  FLASH_CTRL;
    __IO u32  FLASH_FLAG;
    __IO u32  REMAP_SEL;
         u32 RESERVED0[1];
			
    __IO u32  PL_SEQ0;
    __IO u32  PL_SEQ1;
    __IO u32  MAIN_PL0;
    __IO u32  MAIN_PL1;
    __IO u32  MAIN_PL2;
    __IO u32  MAIN_PL3;
    __IO u32  MAIN_PL4;
    __IO u32  MAIN_PL5;
    __IO u32  MAIN_PL6;
    __IO u32  MAIN_PL7;
    __IO u32  INFO_PL;
         u32  RESERVED1[2];		
    __IO u32  RAM_INTR_FLAG;		        
}FLASH_TypeDef;

#define FLASH               ((FLASH_TypeDef *) EFLASH_CTRL_BASEADDR)

/* ================================================================================ */


/* --------------------  End of section using anonymous unions  ------------------- */
#if   defined (__CC_ARM)
  #pragma pop
#elif defined (__ICCARM__)
  /* leave anonymous unions enabled */
#elif defined (__GNUC__)
  /* anonymous unions are enabled by default */
#elif defined (__TMS470__)
  /* anonymous unions are enabled by default */
#elif defined (__TASKING__)
  #pragma warning restore
#elif defined (__CSMC__)
  /* anonymous unions are enabled by default */
#else
  #warning Not supported compiler type
#endif




//=====================================================================================
#define SET_BIT(REG, BIT)     ((REG) |= (BIT))

#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))

#define READ_BIT(REG, BIT)    ((REG) & (BIT))

#define CLEAR_REG(REG)        ((REG) = (0x0))

#define WRITE_REG(REG, VAL)   ((REG) = (VAL))

#define READ_REG(REG)         ((REG))

#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

#ifdef __cplusplus
}
#endif

#endif  /* ARMCM0plus_H */
