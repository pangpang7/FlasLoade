/*I2C test 
i2c_demo1  :mstRx_slvTx,polling
*/

//////////////////////////////////////////////////////////////////////////////////
#include "SI6600.h"

I2C_TypeDef *bI2CM_x,*bI2CS_x;
u8 i2c_txbuf[8],i2c_rxbuf[8];
u8 cnt,result; 

void delay(u32 idly)
{
   while(idly--);
}

void LED_port_init(PORT_TypeDef * PORTx)	      
{
	PORTx->Pn_DIR = 0x00;
  PORTx->Pn = 0x00;
}

void led_add(PORT_TypeDef * PORTx)
{
  PORTx->Pn=cnt++;
}

void I2C1_mst_port(void)      
{
	//---------------------------------          //P50++51
	PORT_5->Pn_SEL1      &= ~(0x3f);	                
	PORT_5->Pn_SEL1      |=  (PORT_CFG_I2C) + (PORT_CFG_I2C);
	
	PORT_5->Pn_DIR       &= ~(0x03);		      //output 
	PORT_5->Pn_OPENDRAIN |=  (0x03);	        //sdio to open-drain	
	
//	//---------------------------------          //P22+P23 sdio+clk(FPGA:B17+H16)
//	PORT_2->Pn_SEL1      &= ~(0x3f<<6);	                
//	PORT_2->Pn_SEL1      |=  (PORT_CFG_I2C<<9) + (PORT_CFG_I2C<<6);
//	
//	PORT_2->Pn_DIR       &= ~(0x03<<2);		      //output 
//	PORT_2->Pn_OPENDRAIN |=  (0x03<<2);	        //sdio to open-drain			
}

void I2C2_slv_port(void)             
{
  //---------------------------------         //P32+P33
	PORT_3->Pn_SEL1 &= ~(0x3f<<6);	                            //clr 0
	PORT_3->Pn_SEL1 |= (PORT_CFG_I2C<<9) + (PORT_CFG_I2C<<6);
	
	PORT_3->Pn_DIR  &= ~(0x3<<2);				
	PORT_3->Pn_DIR  |=  (0x2<<2);		           //2'b10   input 
	PORT_3->Pn_OPENDRAIN |= (0x03<<2);	       //sdio to open-drain	
	
  //---------------------------------         //P45+P46 ,sdio+clk(FPGA:D24+D25)
//	PORT_4->Pn_SEL1 &= ~(0x3f<<15);	                            //clr 0
//	PORT_4->Pn_SEL1 |= (PORT_CFG_I2C<<18) + (PORT_CFG_I2C<<15);
//	
//	PORT_4->Pn_DIR  &= ~(0x3<<5);				
//	PORT_4->Pn_DIR  |=  (0x2<<5);		           //2'b10   input 
//	PORT_4->Pn_OPENDRAIN |= (0x03<<5);	       //sdio to open-drain	
}

void i2c1_mst_cfg(void)
{
	bI2CM_x->ENABLE         = 0x00;
	bI2CM_x->CON            = 0x63;	    //mst
	bI2CM_x->TAR            = 0x66;  
	bI2CM_x->RX_TL          = 7; 	      //Rx_Intr_Trigger
	bI2CM_x->TX_TL          = 7; 
	bI2CM_x->FS_SPKLEN      = 1; 
	bI2CM_x->SS_HCNT        = 150; 
	bI2CM_x->SS_LCNT        = 150; 
	bI2CM_x->INTR_MASK      = 0; 	    
}

void i2c2_slv_cfg(void)
{
	bI2CS_x->ENABLE         = 0x00;
	bI2CS_x->CON            = 0x22;	    //slv
	bI2CS_x->SAR            = 0x66;    
	bI2CS_x->RX_TL          = 7; 	      //Rx_Intr_Trigger
	bI2CS_x->TX_TL          = 7; 
	bI2CS_x->FS_SPKLEN      = 1; 
	bI2CS_x->SS_HCNT        = 150; 
	bI2CS_x->SS_LCNT        = 150; 
	bI2CS_x->INTR_MASK      = 0; 	    
}

void mstRx_slvTx(void)
{
  u8 i;
  bI2CS_x->ENABLE         = 0x01;
  bI2CM_x->ENABLE         = 0x01;
  
  //----------------------------------
  for(i=0;i<7;i++)						   //7+1 byte write
     {	
			 (bI2CM_x->DATA_CMD) =0x0000|I2C_DATACMD_Read; 
			 while(!((bI2CS_x->RAW_INTR_STAT)&0x20));			//slv 收到transmit请求，再填充数据
			 bI2CS_x->DATA_CMD=i2c_txbuf[i];				      //清除请求再填数据，或先填数据在清请求
			 bI2CS_x->CLR_RD_REQ = bI2CS_x->CLR_RD_REQ;   //while(!bI2CS_x->CLR_RD_REQ);

			 while(!((bI2CM_x->STATUS)&0x08));				    //RFNE
				 i2c_rxbuf[i]= (u8)((bI2CM_x->DATA_CMD)&0xff);
		 }
		 
		 bI2CM_x->DATA_CMD  =0x000|I2C_DATACMD_Read|I2C_DATACMD_Stop;  //with stop
		 while(!((bI2CS_x->RAW_INTR_STAT)&0x20));
		 bI2CS_x->CLR_RD_REQ = bI2CS_x->CLR_RD_REQ;     //while(!bI2CS_x->CLR_RD_REQ);

		 bI2CS_x->DATA_CMD=i2c_txbuf[i]; 
		 while(!((bI2CM_x->STATUS)&0x08));
			 i2c_rxbuf[7]= (u8)((bI2CM_x->DATA_CMD)&0xff);
		 while((bI2CM_x->STATUS)&0x01);// 
  
  bI2CS_x->ENABLE         = 0x00;
  bI2CM_x->ENABLE         = 0x00;	
}

u8 compare_data(u8 *buf1,u8 *buf2)
{
  u8 i;
	for(i=0;i<8;i++)
	{
	 if(buf1[i] != buf2[i])
		return 1; 
	}
	return 0;
}



//====================================================================================================
int main(void)
{
  u8 i;
	LED_port_init(PORT_0);
	for(i=0;i<8;i++)
	{
	  i2c_txbuf[i]=0x80+i;
	  i2c_rxbuf[i]=0x90+i;
	}
	
  bI2CM_x=I2C_1;
	bI2CS_x=I2C_2;
  I2C1_mst_port();
  I2C2_slv_port();	
	i2c1_mst_cfg();
	i2c2_slv_cfg();
	
	result=1;
	mstRx_slvTx();	
	result= compare_data(i2c_txbuf,i2c_rxbuf);
	
  if(result)  //Error
	{
		  while(1)
			 {
				 delay(1000000); 
				 PORT_0->Pn=0xff;
				 delay(1000000); 
				 PORT_0->Pn=0x00;
			 }
	}
	else
	{
			while(1)
			 {
				 led_add(PORT_0);
				 delay(1000000);
			 }
	}
}
