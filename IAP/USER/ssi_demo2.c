/*
ssi_demo2.c :MST Tx	;SLV Rx
*/

///////////////////////////////////////////////////////////////////////////////////////

#include "SI6600.h"

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
void SSIM3_port_init(void)	      
{
	//---------------------------------P53+P52+P51+P50(clk+in+out+cs)0100
  PORT_5->Pn_DIR   = 0x4;      
	PORT_5->Pn_SEL1  = 0x6db;          //0110 1101 1011

}

void SSIS4_port_init(void)	       
{
  //---------------------------------P63+P62+P61+P60(clk+out+in+cs)1011                         	
  PORT_6->Pn_DIR   = 0xb;     
	PORT_6->Pn_SEL1  = 0x924;		       //1001 0010 0100
}
void ssi_mst_init(SSI_TypeDef* SSIM_x)
{
	SSIM_x->SSIENR   = 0x00;
	SSIM_x->CTRLR0   = 0x0107;
	SSIM_x->CTRLR1   = 7; 
	SSIM_x->MWCR     = 0;   
	SSIM_x->BAUDR    = 0x64;  
	SSIM_x->TXFTLR   = 7;
	SSIM_x->RXFTLR   = 7;
	SSIM_x->IMR      = 0;
	SSIM_x->SER      = 1;

}
void ssi_slv_init(SSI_TypeDef* SSIS_x)
{
	SSIS_x->SSIENR   = 0x00;
	SSIS_x->CTRLR0   = 0x0207;
	SSIS_x->CTRLR1   = 7;  
	SSIS_x->MWCR     = 0;  	    
	SSIS_x->BAUDR    = 0x64;  
	SSIS_x->TXFTLR   = 7;
	SSIS_x->RXFTLR   = 7;
	SSIS_x->IMR      = 0;
	SSIS_x->SER      = 1;
	
	SSIS_x->SSIENR=0x01;
}

void ssimstTx_slvRx(SSI_TypeDef* SSIM_x,SSI_TypeDef* SSIS_x)
{  
	u8 i;
  //---------------------------------------------------mst tx 
  SSIM_x->SSIENR = 0x01;
	for(i=0;i<8;i++)
	{
		  	while(!(SSIM_x->SR&0x02));    //
	    SSIM_x->DR_LOW =  ssim_txbuf[i];
		 
	}	

	while(SSIM_x->SR&0x01);       //
  //delay(100000);

//  //---------------------------------------------------slv rx
	for(i=0;i<8;i++)
	{
	    ssis_rxbuf[i] = SSIS_x->DR_LOW ;
	}
  SSIM_x->SSIENR = 0;
	SSIS_x->SSIENR = 0;
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
//	ssimstTx_slvRx(SSIM_1,SSIS_2);


	SSIM3_port_init();
	SSIS4_port_init();

	ssi_mst_init(SSIM_3);
	ssi_slv_init(SSIS_4);
	ssimstTx_slvRx(SSIM_3,SSIS_4);
	
  //res=check_datap((u8 *)ssim_rxbuf,(u8 *)ssis_txbuf,8);	
	res=check_datap((u8 *)ssim_txbuf,(u8 *)ssis_rxbuf,8);	
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
											 
