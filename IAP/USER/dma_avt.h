#ifndef __DMASTRUCT_SITRI6600_H__
#define __DMASTRUCT_SITRI6600_H__

#include "SI6600.h"


typedef enum
{ 
  IntrCmd_Disable = 0,
  IntrCmd_Enable
}DMAIntrCmd_TypeDef;

typedef enum
{ 
  DstTRWidth_8bit  = 0,
  DstTRWidth_16bit,
  DstTRWidth_32bit
}DMADstTRWidth_TypeDef;

typedef enum
{ 
  SrcTRWidth_8bit  = 0,
  SrcTRWidth_16bit,
  SrcTRWidth_32bit
}DMASrcTRWidth_TypeDef;

typedef enum
{ 
  DstInc_inc = 0,
  DstInc_dec,
  DstInc_NOchange
}DMADstInc_TypeDef;

typedef enum
{ 
  SrcInc_inc = 0,
  SrcInc_dec,
	SrcInc_NOchange
}DMASrcInc_TypeDef;

typedef enum
{ 
  DstMSIZE_1byte = 0,
  DstMSIZE_4byte
}DMADstMSIZE_TypeDef;

typedef enum
{ 
  SrcMSIZE_1byte = 0,
  SrcMSIZE_4byte
}DMASrcMSIZE_TypeDef;

typedef enum
{ 
  TTFCType_M2M = 0,
  TTFCType_M2P,
	TTFCType_P2M,
	TTFCType_P2P
}DMATTFCType_TypeDef;

typedef enum
{ 
  DstPer_hs0 = 0,
  DstPer_hs1,
	DstPer_hs2,
	DstPer_hs3
}DMADstPer_TypeDef;

typedef enum
{ 
  SrcPer_hs0 = 0,
  SrcPer_hs1,
  SrcPer_hs2,
  SrcPer_hs3
}DMASrcPer_TypeDef;

typedef enum
{ 
  SrcHSSel_HwHandshak = 0,
  SrcHSSel_SwHandshak
}DMASrcHSSel_TypeDef;

typedef enum
{ 
  DstHSSel_HwHandshak = 0,
  DstHSSel_SwHandshak
}DMADstHSSel_TypeDef;


typedef struct
{
	u8  DMA_ID;
	u32 DMA_SrcAddr;
	u32 DMA_DstAddr;
	DMAIntrCmd_TypeDef      DMA_IntrCmd;
	DMADstTRWidth_TypeDef   DMA_DstTRWidth;
	DMASrcTRWidth_TypeDef   DMA_SrcTRWidth;
	DMADstInc_TypeDef       DMA_DstInc;
	DMASrcInc_TypeDef       DMA_SrcInc;
	DMADstMSIZE_TypeDef     DMA_DstMSIZE;
	DMASrcMSIZE_TypeDef     DMA_SrcMSIZE;
	DMATTFCType_TypeDef     DMA_TTFCType;
	u16 DMA_BLOCKTS;
	DMADstHSSel_TypeDef     DMA_DstHSSel;
	DMASrcHSSel_TypeDef     DMA_SrcHSSel;
	DMASrcPer_TypeDef       DMA_SrcPer;
	DMADstPer_TypeDef       DMA_DstPer; 
}DMA_InitTypeDef;

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
#endif
