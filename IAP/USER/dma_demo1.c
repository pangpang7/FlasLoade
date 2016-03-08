/*
dam_demo1.c  sram  to uart	 (uart1 send data to uart2, dma mov sram data to uart1 tx buffer )
*/


///////////////////////////////////////////////////////////////////////////////////

#include "SI6600.h"
#include "sysclk.h"
#include "dma_avt.h"

#define SRAM_StartAddr1          (0x20000c00)  
#define SRAM_StartAddr2          (0x20000d00)  
#define FLSH_StartAddr1          (0x00010000)  
#define FLSH_StartAddr2          (0x00011000)  

u8   dma_intr_flg;
u8   uart2_recv_complete_flg,uart_cnt;
u8   uart_buf[32]={0};
u8   databuf[32]={0x02,0x00,0x01,0x05,0x00,0x07,0x00,0x08,
	                0x55,0xaa,0xff,0x00,0x01,0x02,0x04,0x08,
	                0x10,0x20,0x40,0x80,0x3c,0x69,0x78,0x1e,
	                0x34,0x78,0x34,0x12,0x91,0x11,0x10,0x10};
DMA_InitTypeDef  bDMA_InitStructure;
void delay(u32 idly);
void dma_ch0_config(void);								
void Led_port_init(void);	
u8   check_datap(u8 *p1,u8 * p2,u8 blen);

//-----------------------------------------------------------
void uart_senddata(void)
{
   u8 i;
	for(i=0;i<32;i++)
	{
	    UART_2->THR=databuf[i];
		while(!((UART_2->LSR)&UART_LSR_THREmpty));
	}
    while(!((UART_2->LSR)&UART_LSR_TxEmpty)); 
}

void peripheral_uart_init(void)	
{
	//UART1 port_init
	//P1.6	(UART1_RX  ),J404-5
	//P1.7	(UART1_TX  ),J404-6
	//---------------------------------//P16+P17  
	PORT_1->Pn_SEL1 &= ~(0x3f<<18);	                            //clr 0
	PORT_1->Pn_SEL1 |=  (PORT_CFG_UART<<21) + (PORT_CFG_UART<<18);
	
	PORT_1->Pn_DIR  &= ~(0x03<<6);		                            
	PORT_1->Pn_DIR  |=  (0x01<<6);		   //2'b01
	
	//UART2 port_init	
	//P4.0	UART2_RX  F23 P4[0]	F23	J212-13
	//P4.1	UART2_TX  E23 P4[1]	E23	J212-15
	//---------------------------------//P16+P17  
	PORT_4->Pn_SEL1 &= ~(0x3f);	                            //clr 0
	PORT_4->Pn_SEL1 |=  (PORT_CFG_UART<<3) + (PORT_CFG_UART);
	
	PORT_4->Pn_DIR  &= ~(0x03);		                            
	PORT_4->Pn_DIR  |=  (0x01);		   //2'b01	
	
	//set baudrate 9600
	UART_1->MCR  = 0x00;
	UART_1->LCR  = 0x80;	
	UART_1->DLL  = 0x68;
	UART_1->DLH  = 0x00;
	UART_1->LCR  = 0;
	
	UART_1->LCR |= 0x03;     //data length	
	UART_1->FCR  = 0x01;     //FIFO enable
	UART_1->IER  = 0;        //receive data int enable

	UART_2->MCR  = 0x00;
	UART_2->LCR  = 0x80;	
	UART_2->DLL  = 0x68;		 //9600
	UART_2->DLH  = 0x00;
	UART_2->LCR  = 0x00;

	UART_2->LCR |= 0x03;     //data frame size ...
	UART_2->FCR  = 0x01;     // FIFO enable	 
	UART_2->IER  = 0;		     //dis Intr	
	
	UART_2->IER |= 0x01;	//en recv-Intr	
	NVIC->ISER[0] |= NVIC_INTR_EN_UART2_IRQ;	
}

int main(void)
{
	u8 i,res=1;
	dma_intr_flg=0;	
	uart2_recv_complete_flg=0;
	uart_cnt=0;
	
	
  Led_port_init();
	for(i=0;i<32;i++)
	{
	  (*(volatile unsigned char *)(SRAM_StartAddr1+i))=databuf[i];  
	}	
//---------------------------------------------------------sram  to uart	
	HS0_CFG = UART_1_TX_dmaID;
  NVIC->ISER[0] |= NVIC_INTR_EN_DMAC_IRQ;	  //
	
  bDMA_InitStructure.DMA_SrcAddr    = SRAM_StartAddr1;
	bDMA_InitStructure.DMA_DstAddr    = UART_1_BASEADDR;  //UART_RBR/THR

	bDMA_InitStructure.DMA_IntrCmd    = IntrCmd_Enable;
	bDMA_InitStructure.DMA_DstTRWidth = DstTRWidth_8bit;
	bDMA_InitStructure.DMA_SrcTRWidth = SrcTRWidth_8bit;
	bDMA_InitStructure.DMA_DstInc     = DstInc_NOchange;
	bDMA_InitStructure.DMA_SrcInc     = SrcInc_inc;
	bDMA_InitStructure.DMA_DstMSIZE   = DstMSIZE_1byte;
	bDMA_InitStructure.DMA_SrcMSIZE   = SrcMSIZE_1byte;
	bDMA_InitStructure.DMA_TTFCType   = TTFCType_M2P;
	bDMA_InitStructure.DMA_BLOCKTS    = 0x20;            //32byte
	
	bDMA_InitStructure.DMA_DstHSSel   = DstHSSel_HwHandshak;
	bDMA_InitStructure.DMA_SrcHSSel   = SrcHSSel_HwHandshak;
	bDMA_InitStructure.DMA_SrcPer     = SrcPer_hs0;      //Memary per peripheral无效
	bDMA_InitStructure.DMA_DstPer     = DstPer_hs0;
	
  peripheral_uart_init();
	dma_ch0_config();
	
	while(!(uart2_recv_complete_flg));
	while(1)
	{
	  if(dma_intr_flg)	  //检测到dma结束
	  {
	     dma_intr_flg=0;
	     res=check_datap((u8 *)databuf,(u8 *)uart_buf,32);
	  }
		
		if(res)
		{
				PORT_0->Pn = 0xff;	    //error
		}
	  else
		{
			  PORT_0->Pn = i++;
			  delay(1000000);
		}
	}		
	
}
void DMAC_IRQHandler(void)
{	
	if(DMAC_STATUS_BLK & 0x01)     //check intr flag
	{		
	  
	  DMAC_CLR_BLK = 0x00000101;   //clear int flag
	  dma_intr_flg=1;
    }
}
void UART2_IRQHandler(void)      //DMAC_IRQHandler(void)
{	
  if(UART_2->IIR == 0xC4)	       //Receiver data available	INTR  1/2FIFO
	{
	  {
	    uart_buf[uart_cnt++]=UART_2->RBR;
	  } 
	  if(uart_cnt==32)
	  {
	  uart_cnt=0;
	  uart2_recv_complete_flg=1;
	  }  
	}
}
void dma_ch0_config(void)		     //dma1_1.c  sram  to sram	inc
{
  //set DMAC MASK REG
	DMAC_MASK_BLK = 0x00000101;		  //enable ch0 Block-Transaction intr	
	
  //ch0 set src/dst address	
	DMAC_SAR0 = bDMA_InitStructure.DMA_SrcAddr; 
	DMAC_DAR0 = bDMA_InitStructure.DMA_DstAddr;

	DMAC_CTL0_L = (bDMA_InitStructure.DMA_IntrCmd)         |
	              (bDMA_InitStructure.DMA_DstTRWidth<<1)   |
	              (bDMA_InitStructure.DMA_SrcTRWidth<<4)   |
	              (bDMA_InitStructure.DMA_DstInc<<7)       |
	              (bDMA_InitStructure.DMA_SrcInc<<9)       |
	              (bDMA_InitStructure.DMA_DstMSIZE<<11)    |
								(bDMA_InitStructure.DMA_SrcMSIZE<<14)    |
								(bDMA_InitStructure.DMA_TTFCType<<20);
	DMAC_CTL0_H = (bDMA_InitStructure.DMA_BLOCKTS);
	
	DMAC_CFG0_L = (bDMA_InitStructure.DMA_DstHSSel<<10)    |
	              (bDMA_InitStructure.DMA_SrcHSSel<<11);
								
	DMAC_CFG0_H = (bDMA_InitStructure.DMA_SrcPer<<8)       |
	              (bDMA_InitStructure.DMA_DstPer<<12);

	DMAC_CFG = 0x00000001;	       	//enable dma   
	
	DMAC_CH_EN = 0x00000101;        //enable dma channel
}
u8 check_datap(u8 *p1,u8 * p2,u8 blen)
{
   u8  i;
	for(i=0;i<blen;i++)
	{
		if(*(p1++) != *(p2++))
		{
		 return 1;
		}
	}
  return 0;
}

void delay(u32 idly)
{
    while(idly--);
}

void Led_port_init(void)
{
	PORT_0->Pn_PH  = 0x00;
  PORT_0->Pn_DIR = 0x00;
	PORT_0->Pn     = 0x00;
}





