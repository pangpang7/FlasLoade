/*
ssi_demo2.c :MST TxRx	;SLV RxTx
*/

///////////////////////////////////////////////////////////////////////////////////////

#include "SI6600.h"
#include "sysclk.h"

void ssim_Tx(void);
void ssim_TxN(void);
void delay(u32 idly);

u16  ssim_txbuf[8];
u16  ssim_rxbuf[8];
u16  ssis_txbuf[8];
u16  ssis_rxbuf[8];

void SSIM1_port_init(void)	      
{
	//---------------------------------P14+P13+P12+P11(clk+in+out+cs)0100
	PORT_1->Pn_SEL0  &= ~(0x0f<<1);
	PORT_1->Pn_PH    &= ~(0x0f<<1);
	PORT_1->Pn_SEL1 &= ~(0xfff<<3);	                          
	PORT_1->Pn_SEL1 |= (PORT_CFG_SSIM<<12) + (PORT_CFG_SSIM<<9)+(PORT_CFG_SSIM<<6) + (PORT_CFG_SSIM<<3);
	
	PORT_1->Pn_DIR  &= ~(0x0f<<1);		                            
	PORT_1->Pn_DIR  |=  (0x04<<1);	
	
//	//---------------------------------P40+P41+P42+P43
//	PORT_4->Pn_SEL1 &= ~(0xfff);	                          
//	PORT_4->Pn_SEL1 |= (PORT_CFG_SSIM<<9) + (PORT_CFG_SSIM<<6)+(PORT_CFG_SSIM<<3) + (PORT_CFG_SSIM);
//	
//	PORT_4->Pn_DIR  &= ~(0x0f);		                            
//	PORT_4->Pn_DIR  |=  (0x04);	
}

void SSIM3_port_init(void)	      
{
	//---------------------------------P53+P52+P51+P50(clk+in+out+cs)0100
  PORT_5->Pn_DIR   = 0x4;      
	PORT_5->Pn_SEL1  = 0x6db;          //0110 1101 1011
  //---------------------------------P63+P62+P61+P60(clk+out+in+cs)1011                         	
  PORT_6->Pn_DIR   = 0xb;     
	PORT_6->Pn_SEL1  = 0x924;		       //1001 0010 0100
}
//void SSIM3_port_init(void)	      
//{

//	PORT_5->Pn = 0xffff;
//	
//  PORT_6->Pn_DIR   |=  (0x0f);
//  PORT_6->Pn_PH    |=  (0x0f);	
//	//---------------------------------P53+P52+P51+P50(clk+in+out+cs)0100
//	PORT_5->Pn_SEL0  &= ~(0x0f);	
//  PORT_5->Pn_DIR   |=  (0x0f);		
//	PORT_5->Pn_PH    |=  (0x0f);	
//	  delay(10);
//	
//	
//	PORT_5->Pn_PH    &= ~(0x0B);	
//  PORT_5->Pn_DIR   &= ~(0x0B);
//	
//	
//	                                               //1001 0010 0100
//	PORT_5->Pn_SEL1 |= (0xfff);	                   //0110 1101 1011       
//	PORT_5->Pn_SEL1 &= ~0x924;
// while(1);	
//////==============================================================================================	
//	                                   //P63+P62+P61+P60(clk+out+in+cs)1011
//	PORT_6->Pn_SEL0  &= ~(0x0f);	                            	

////	PORT_6->Pn_PH    |=  (0x0f);
////	  delay(10);
//	PORT_6->Pn_PH    &=  ~(0x02);
//	
//	PORT_6->Pn_DIR   |=  (0x0f);
//  PORT_6->Pn_DIR   &= ~(0x04);	
//	
//	PORT_6->Pn_SEL1 |= (0xfff);	                   //0110 1101 1011       
//	PORT_6->Pn_SEL1 &= ~0x924;
////	while(1);

//		
//}
void SSIS2_port_init(void)	       
{
	//-----------------------------------P23+P22+P21+P20(clk+out+in+cs)1011
	
	PORT_2->Pn_SEL0  &= ~(0x0f);
	PORT_2->Pn_PH    &= ~(0x0f);
	PORT_2->Pn_SEL1 &= ~(0xfff);	                       
	PORT_2->Pn_SEL1 |= (PORT_CFG_SSIS<<9) + (PORT_CFG_SSIS<<6)+(PORT_CFG_SSIS<<3) + (PORT_CFG_SSIS);
	
	PORT_2->Pn_DIR  &= ~(0x0f);		                            
	PORT_2->Pn_DIR  |=  (0x0b);	
////---------------------------------P44+P45+P46+P47
//	PORT_4->Pn_SEL1 &= ~(0xfff<<12);	                       
//	PORT_4->Pn_SEL1 |= (PORT_CFG_SSIS<<21) + (PORT_CFG_SSIS<<18)+(PORT_CFG_SSIS<<15) + (PORT_CFG_SSIS<<12);
//	
//	PORT_4->Pn_DIR  &= ~(0x0f<<4);		                            
//	PORT_4->Pn_DIR  |=  (0x0b<<4);	
}
void SSIS4_port_init(void)	       
{
	//-----------------------------------P63+P62+P61+P60(clk+out+in+cs)1011
	PORT_6->Pn_SEL0  &= ~(0x0f);
	PORT_6->Pn_DIR   &= ~(0x0f);		                            
	PORT_6->Pn_DIR   |=  (0x0b);	

	PORT_6->Pn_PH    &= ~(0x0f);
	PORT_6->Pn_PH    |=  (0x0b);
	PORT_6->Pn_SEL1  &= ~(0xfff);	                       
	PORT_6->Pn_SEL1  |= (PORT_CFG_SSIS<<9) + (PORT_CFG_SSIS<<6)+(PORT_CFG_SSIS<<3) + (PORT_CFG_SSIS);
	
while(1);

}
void ssi_mst_init(SSI_TypeDef* SSI_x)
{
	SSI_x->SSIENR   = 0x00;
	SSI_x->CTRLR0   = 0x0007;
	SSI_x->CTRLR1   = 7; 
	SSI_x->MWCR     = 0;   
	SSI_x->BAUDR    = 0x64;  
	SSI_x->TXFTLR   = 7;
	SSI_x->RXFTLR   = 7;
	SSI_x->IMR      = 0;
	SSI_x->SER      = 1;

}
void ssi_slv_init(SSI_TypeDef* SSI_x)
{
	SSI_x->SSIENR   = 0x00;
	SSI_x->CTRLR0   = 0x0007;
	SSI_x->CTRLR1   = 7;  
	SSI_x->MWCR     = 0;  	    
	SSI_x->BAUDR    = 0x64;  
	SSI_x->TXFTLR   = 7;
	SSI_x->RXFTLR   = 7;
	SSI_x->IMR      = 0;
	SSI_x->SER      = 1;
	
	//SSI_x->SSIENR=0x01;
}

void ssim_TxRx(void)
{  
	u8 i;
	//---------------------------------------------------

	for(i=0;i<8;i++)
	{
     SSIS_2->DR_LOW = ssis_txbuf[i];
	}
  //---------------------------------------------------mst tx 
  SSIM_1->SSIENR = 0x01;
	for(i=0;i<8;i++)
	{
	    SSIM_1->DR_LOW =  ssim_txbuf[i];
	}	
	while(!(SSIM_1->SR&0x08));    //
	while(SSIM_1->SR&0x01);       //
	for(i=0;i<8;i++)
	{
	    ssim_rxbuf[i] = SSIM_1->DR_LOW ;
	}
  //---------------------------------------------------
	for(i=0;i<8;i++)
	{
	    ssis_rxbuf[i] = SSIS_2->DR_LOW ;
	}
	SSIM_1->SSIENR = 0;
	SSIS_2->SSIENR = 0;
}
void ssim_TxRx34(void)
{  
	u8 i;
	//---------------------------------------------------
	SSIS_4->SSIENR=0x01;
	for(i=0;i<8;i++)
	{
     SSIS_4->DR_LOW = ssis_txbuf[i];
	}
  //---------------------------------------------------mst tx 
  SSIM_3->SSIENR = 0x01;
	for(i=0;i<8;i++)
	{
	    SSIM_3->DR_LOW =  ssim_txbuf[i];
	}	
	while(!(SSIM_3->SR&0x08));    //
	while(SSIM_3->SR&0x01);       //
	for(i=0;i<8;i++)
	{
	    ssim_rxbuf[i] = SSIM_3->DR_LOW ;
	}
  //---------------------------------------------------
	for(i=0;i<8;i++)
	{
	    ssis_rxbuf[i] = SSIS_4->DR_LOW ;
	}
	SSIM_3->SSIENR = 0;
	SSIS_4->SSIENR = 0;
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



//=======================================================================================
int main(void)
{
	u8 res=1;
	u8 i=0;	
	
	PORT_0->Pn_DIR = 0x00;
	PORT_0->Pn = 0x00;
	for(i=0;i<8;i++)
	{
	   ssim_txbuf[i]=0x10 + i;
     ssim_rxbuf[i]=0x20 + i;
     ssis_txbuf[i]=0x30 + i;
     ssis_rxbuf[i]=0x40 + i;
	}
//	SSIM1_port_init();
//	SSIS2_port_init();	
//	ssi_mst_init(SSIM_1);
//	ssi_slv_init(SSIS_2);
//	ssim_TxRx();


	SSIM3_port_init();
//	SSIS4_port_init();

	
	ssi_slv_init(SSIS_4);
	ssi_mst_init(SSIM_3);
	ssim_TxRx34();
	
  res=check_datap((u8 *)ssim_rxbuf,(u8 *)ssis_txbuf,8);	
	res+=check_datap((u8 *)ssim_txbuf,(u8 *)ssis_rxbuf,8);	
	while(1)
	{	  
		if(res)
		{
				PORT_0->Pn = 0xff;	  //error
		}
	  else
		{
			 PORT_0->Pn = i++;
			 delay(1000000);
		}
	}
}

void delay(u32 idly)
{
while(idly--);
}
											 
