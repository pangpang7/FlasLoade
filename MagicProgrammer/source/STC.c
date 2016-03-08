#include "hw_config.h"
#include "STC.h"
#include "sw_lib.h"
#include "usb_type.h"
#include "spi.h"
#include "OfflineDownloadCtrl.h"
#include "SI6600BL.h"


//sbit StartDown11 = P3^5; zwh
//for test

u32 PageAddr;
u8 OUTIN_PACKET[100];
bool STC_pwd_authed;
bool STCIsConnected;

u16 control[16];
u8  STCcontrol[140];
u16 controlcnt[16];
u8  MassOrInfo;
extern u8 ArrayITX[64];
extern u8 ArrayIRX[64];

//unsigned int xdata temp_data[256] _at_ 0x440;	 zwh
unsigned int temp_data[256];
STC_CONFIG DwordCfg;

void STC_BL_Init(void)
{
	STC_pwd_authed=FALSE;
	STCIsConnected=FALSE;
}

void STC_Init(void)
{
	u16 i;
	SO_ResetBits(TMS);
	SO_ResetBits(TDI);
	SO_ResetClock();
	for(i=0;i<200;i++)
	{
		SO_SetClock();
		SO_ResetClock();

	}

	SO_SetBits(TDI);
	for(i=0;i<250;i++)
	{
		SO_SetClock();
		SO_ResetClock();

	}
	SO_ResetBits(TDI);
	SO_SetClock();
	SO_ResetClock();

	SO_SetBits(TDI);
	SO_SetClock();
	SO_ResetClock();

	SO_ResetBits(TDI);
	SO_SetClock();
	SO_ResetClock();

	SO_SetBits(TDI);
	SO_SetClock();
	SO_ResetClock();

	SO_SetBits(TDI);
	SO_SetClock();
	SO_ResetClock();

	SO_SetBits(TMS);

	SO_SetBits(TDI);
	SO_SetClock();
	SO_ResetClock();

	SO_ResetBits(TDI);
	SO_SetClock();
	SO_ResetClock();

	SO_SetBits(TDI);
	SO_SetClock();
	SO_ResetClock();

	SO_ResetBits(TDI);
	SO_SetClock();
	SO_ResetClock();

	SO_SetBits(TDI);
	SO_SetClock();
	SO_ResetClock();

	SO_SetBits(TDI);
	SO_SetClock();
	SO_ResetClock();

	SO_ResetBits(TMS);

	SO_SetBits(TDI);
	SO_SetClock();
	SO_ResetClock();

	SO_SetBits(TDI);
	SO_SetClock();
	SO_ResetClock();
}

void STC_Connect(void)
{
   	u16 RxPassword=0;

	if(STCIsConnected)
		return;

	STCIsConnected=TRUE;
    STC_IO_Config_Used_for_STC();

   	SO_ResetClock();
	SO_SetBits(TMS);

	SO_SetBits(RST);
	ArrayITX[0]=0;
	ArrayITX[1]=0;
	ArrayITX[2]=0;
	ArrayITX[3]=1;
	ArrayITX[4]=0;
	ArrayITX[5]=0;
	ArrayITX[6]=0;
	ArrayITX[7]=0;

	/*TimerOver=10;
   	TR2=1;
   	while(TimerOver);
	TR2=0; zwh*/
	DelayMs(10);

	SO_ResetBits(RST);

   	/*TimerOver=10;
   	TR2=1;
   	while(TimerOver);
	TR2=0; zwh*/
	DelayMs(10);

	SPI_TxRx(8,0x1979,&RxPassword);

	SO_ResetBits(TMS);

	/*TimerOver=10;
   	TR2=1;
   	while(TimerOver);
	TR2=0; zwh*/
	DelayMs(10);

	SO_SetBits(RST);

	/*TimerOver=50;
   	TR2=1;
   	while(TimerOver);
	TR2=0; zwh*/
	DelayMs(50);

	STC_Init();

}

void STC_DisConnect(void)
{
	u16 RxPassword=0;

	if(!STCIsConnected)
		return;

	STCIsConnected=FALSE;

	SO_ResetClock();
	SO_SetBits(TMS);

   	SO_SetBits(RST);
	ArrayITX[0]=0;
	ArrayITX[1]=0;
	ArrayITX[2]=0;
	ArrayITX[3]=1;
	ArrayITX[4]=0;
	ArrayITX[5]=0;
	ArrayITX[6]=0;
	ArrayITX[7]=0;


	SO_ResetBits(RST);

	/*TimerOver=10;
   	TR2=1;
   	while(TimerOver);
	TR2=0; zwh*/
	DelayMs(10);

	SPI_TxRx(8,0x0,&RxPassword);

	SO_SetBits(RST);

	STC_IO_Config_Unused();
}

void STC_MassEraseCmd(void)						   
{  	
	u8 k,l;


	control[0]=0x200; controlcnt[0]=1;		 
	control[1]=0x211; controlcnt[1]=1;
	control[2]=0x219; controlcnt[2]=3;
	
	control[3]=0x259; controlcnt[3]=5620; //2810
	control[4]=0x251; controlcnt[4]=25;   //20
	control[5]=0x211; controlcnt[5]=1;
	control[6]=0x202; controlcnt[6]=1;

	for(k=0;k<7;k++)
	{
		for(l=0;l<10;l++)
		STCcontrol[k*10+l] = (control[k]>>l) & 1;
	}
	STC_OP(0x00,0x0a28,0x0a28,7);

}

void STC_MassEraseCmdAll(void)						 
{
	u8 k,l;

	control[0]=0x200; controlcnt[0]=1;
	control[1]=0x291; controlcnt[1]=1;
	control[2]=0x299; controlcnt[2]=3;

	control[3]=0x2d9; controlcnt[3]=5620; //2810
	control[4]=0x2d1; controlcnt[4]=25;   //20
	control[5]=0x291; controlcnt[5]=1;
	control[6]=0x202; controlcnt[6]=1;

	for(k=0;k<7;k++)
	{
		for(l=0;l<10;l++)
			STCcontrol[k*10+l] = (control[k]>>l) & 1;
	}
	STC_OP(0x00,0x0a28,0x0a28,7);		

}

void STC_PageErase(void)						 
{
	u8 k,l;

	if(DwordCfg.MainOrInfo)
	{
		control[0]=0x200; controlcnt[0]=1;		 
		control[1]=0x201; controlcnt[1]=1;
		control[2]=0x209; controlcnt[2]=3;
		
		control[3]=0x249; controlcnt[3]=5620; //2810
		control[4]=0x241; controlcnt[4]=25;   //20
		control[5]=0x201; controlcnt[5]=1;
		control[6]=0x202; controlcnt[6]=1;	
	}
	else
	{
		control[0]=0x200; controlcnt[0]=1;
		control[1]=0x281; controlcnt[1]=1;
		control[2]=0x289; controlcnt[2]=3;
	
		control[3]=0x2c9; controlcnt[3]=5620; //2810
		control[4]=0x2c1; controlcnt[4]=25;   //20
		control[5]=0x281; controlcnt[5]=1;
		control[6]=0x202; controlcnt[6]=1;
	}

	for(k=0;k<7;k++)
	{
		for(l=0;l<10;l++)
			STCcontrol[k*10+l] = (control[k]>>l) & 1;
	}
	STC_OP(0x00,DwordCfg.addr,0x0a28,7);		

}	

void STC_ReferenceCellEraseCmd(void)
{
	u8 k,l;
	control[0]=0x200; controlcnt[0]=1;
	control[1]=0x000; controlcnt[1]=1;
	control[2]=0x044; controlcnt[2]=1;
	control[3]=0x0f6; controlcnt[3]=1;
	control[4]=0x2f6; controlcnt[4]=1;
	control[5]=0x0f6; controlcnt[5]=1;
	control[6]=0x044; controlcnt[6]=1;
	control[7]=0x000; controlcnt[7]=1;
	control[8]=0x001; controlcnt[8]=1;
	control[9]=0x009; controlcnt[9]=4;
	control[10]=0x049;controlcnt[10]=2850;
	control[11]=0x041;controlcnt[11]=4;
	control[12]=0x001;controlcnt[12]=1;
	control[13]=0x000;controlcnt[13]=1;

	for(k=0;k<14;k++)
	{
		for(l=0;l<10;l++)
		STCcontrol[k*10+l] = (control[k]>>l) & 1;
	}
	STC_OP(0x00,0x0000,0x0000,14);

}

void STC_OP(u8 Symbol,u32 addr,u32 dataa,u8 count)
{
 	u8 i,k; 
	u32 j;
	u32 temp_data = 0;

	temp_data = Symbol;
	for(i=0; i<3; i++)					  
	{
		ArrayITX[i] =  temp_data & 0x01;
		temp_data = temp_data >>1;
	}

	temp_data = addr;
	for(i=0; i<16; i++)					  
	{												 
		ArrayITX[i+3] = temp_data & 0x01;
		temp_data = temp_data >>1;
	}

	temp_data = dataa;
	for(i=0; i<32; i++)			  
	{
		ArrayITX[i+19] = (temp_data & 0x01);
		temp_data = temp_data >>1;
	}

	SO_SetBits(TMS);			         

	SO_SetClock();
	SO_ResetClock();

	SO_ResetBits(TMS);
	for(i=0; i<3; i++)		  
	{
		SO_WriteBits(TDI,(BitAction)ArrayITX[i]);
		SO_SetClock();
		SO_ResetClock();
	}

 	SO_SetBits(TMS);				     
	SO_SetClock();
	SO_ResetClock();

	SO_ResetBits(TMS);
	for(i=0; i<16; i++)		 
	{	
		SO_WriteBits(TDI,(BitAction)ArrayITX[i+3]);
		SO_SetClock();
		SO_ResetClock();
	}

	for(i=0; i<32; i++)
	{	
		SO_WriteBits(TDI,(BitAction)ArrayITX[i+19]);
		SO_SetClock();
		SO_ResetClock();
	}

	for(k=0;k<count;k++)			   
	{
		for(j=0;j<controlcnt[k];j++)    
		{
			 temp_data = k*10;
			 SO_WriteBits(TDI,(BitAction)STCcontrol[temp_data]);
			 SO_SetClock();
			 SO_ResetClock();
			 SO_WriteBits(TDI,(BitAction)STCcontrol[temp_data+1]);
			 SO_SetClock();
			 SO_ResetClock();
			 SO_WriteBits(TDI,(BitAction)STCcontrol[temp_data+2]);
			 SO_SetClock();
			 SO_ResetClock();
			 SO_WriteBits(TDI,(BitAction)STCcontrol[temp_data+3]);
			 SO_SetClock();
			 SO_ResetClock();
			 SO_WriteBits(TDI,(BitAction)STCcontrol[temp_data+4]);
			 SO_SetClock();
			 SO_ResetClock();
			 SO_WriteBits(TDI,(BitAction)STCcontrol[temp_data+5]);
			 SO_SetClock();
			 SO_ResetClock();
			 SO_WriteBits(TDI,(BitAction)STCcontrol[temp_data+6]);
			 SO_SetClock();
			 SO_ResetClock();
			 SO_WriteBits(TDI,(BitAction)STCcontrol[temp_data+7]);
			 SO_SetClock();
			 SO_ResetClock();
			 SO_WriteBits(TDI,(BitAction)STCcontrol[temp_data+8]);
			 SO_SetClock();
			 SO_ResetClock();
			 if((k==(count-1))&&(j==(controlcnt[k]-1)))
			 {
			 	SO_SetBits(TMS);
			 }
			 SO_WriteBits(TDI,(BitAction)STCcontrol[temp_data+9]);
			 SO_SetClock();
			 SO_ResetClock();			
		}
	}	 

	SO_ResetBits(TMS);
	SO_SetClock();
	SO_ResetClock();

	SO_ResetBits(TMS);
	SO_SetClock();
	SO_ResetClock();

	SO_SetBits(TMS);				
	SO_SetClock();
	SO_ResetClock();

	SO_ResetBits(TMS);
	SO_SetClock();
	SO_ResetClock();

	SO_SetClock();
	SO_ResetClock();
	
	SO_SetBits(TMS);				 
	SO_SetClock();
	SO_ResetClock();

	SO_ResetBits(TMS);
	SO_SetClock();
	SO_ResetClock();
}

void STC_Write_DWORD(void)
{
	u8 i,k; 
	u32 j;
	u32 temp_data = 0;
	u8 	cnt_ArrayITX = 0;

	//set symbol	
	if(DwordCfg.NormalOrBurst)
		temp_data = 0x00;
	else
		temp_data = 0x02;
		
	for(i=0; i<3; i++)					  
	{
		ArrayITX[cnt_ArrayITX++] =  temp_data & 0x01;
		temp_data = temp_data >>1;
	}
	
	if(DwordCfg.NormalOrBurst)	
	{
		// set addr in Normal mode
		temp_data = DwordCfg.addr;
		for(i=0; i<16; i++)					  
		{												 
			ArrayITX[cnt_ArrayITX++] = temp_data & 0x01;
			temp_data = temp_data >>1;
		}	
	}
	
	// set data
	temp_data = (((u32)(DwordCfg.data[3]))<<24)
				| (((u32)(DwordCfg.data[2]))<<16)
				| (((u32)(DwordCfg.data[1]))<<8)
				| (((u32)(DwordCfg.data[0]))<<0);

	for(i=0; i<32; i++)				  
	{
		ArrayITX[cnt_ArrayITX++] = (temp_data & 0x01);
		temp_data = temp_data >>1;
	}

 	//push over, now start to pop data from ArrayITX to STC bus
	cnt_ArrayITX = 0;
	
	SO_SetBits(TMS);			         

	SO_SetClock();
	SO_ResetClock();

	SO_ResetBits(TMS);
	for(i=0; i<3; i++)		  
	{
		SO_WriteBits(TDI,(BitAction)ArrayITX[cnt_ArrayITX++]);
		SO_SetClock();
		SO_ResetClock();
	}

	SO_SetBits(TMS);				     
	SO_SetClock();
	SO_ResetClock();

	SO_ResetBits(TMS);

	if(DwordCfg.NormalOrBurst)
	{
		for(i=0; i<16; i++)		 
		{	
			SO_WriteBits(TDI,(BitAction)ArrayITX[cnt_ArrayITX++]);
			SO_SetClock();
			SO_ResetClock();
		}
	}
	
	for(i=0; i<32; i++)
	{	
		SO_WriteBits(TDI,(BitAction)ArrayITX[cnt_ArrayITX++]);

	
		SO_SetClock();
		SO_ResetClock();
	}
	
	for(k=0;k<9;k++)			   
	{
		for(j=0;j<controlcnt[k];j++)    
		{
			 temp_data = k*10;
			 SO_WriteBits(TDI,(BitAction)STCcontrol[temp_data]);
			 SO_SetClock();
			 SO_ResetClock();
			 SO_WriteBits(TDI,(BitAction)STCcontrol[temp_data+1]);
			 SO_SetClock();
			 SO_ResetClock();
			 SO_WriteBits(TDI,(BitAction)STCcontrol[temp_data+2]);
			 SO_SetClock();
			 SO_ResetClock();
			 SO_WriteBits(TDI,(BitAction)STCcontrol[temp_data+3]);
			 SO_SetClock();
			 SO_ResetClock();
			 SO_WriteBits(TDI,(BitAction)STCcontrol[temp_data+4]);
			 SO_SetClock();
			 SO_ResetClock();
			 SO_WriteBits(TDI,(BitAction)STCcontrol[temp_data+5]);
			 SO_SetClock();
			 SO_ResetClock();
			 SO_WriteBits(TDI,(BitAction)STCcontrol[temp_data+6]);
			 SO_SetClock();
			 SO_ResetClock();
			 SO_WriteBits(TDI,(BitAction)STCcontrol[temp_data+7]);
			 SO_SetClock();
			 SO_ResetClock();
			 SO_WriteBits(TDI,(BitAction)STCcontrol[temp_data+8]);
			 SO_SetClock();
			 SO_ResetClock();
			 if((k==(9-1))&&(j==(controlcnt[k]-1)))
			 {
			 	SO_SetBits(TMS);
			 }
			 SO_WriteBits(TDI,(BitAction)STCcontrol[temp_data+9]);
			 SO_SetClock();
			 SO_ResetClock();			
		}
	}	 

	SO_ResetBits(TMS);
	SO_SetClock();
	SO_ResetClock();

	SO_ResetBits(TMS);
	SO_SetClock();
	SO_ResetClock();

	SO_SetBits(TMS);				
	SO_SetClock();
	SO_ResetClock();

	SO_ResetBits(TMS);
	SO_SetClock();
	SO_ResetClock();

	SO_SetClock();
	SO_ResetClock();
	
	SO_SetBits(TMS);				 
	SO_SetClock();
	SO_ResetClock();

	SO_ResetBits(TMS);
	SO_SetClock();
	SO_ResetClock();	
}

void STC_Read_DWORD(void)
{
	u8 i,k; 
	u32 j;
	u32 temp_data = 0;
	u32 temp_rd;
	u8 	cnt_ArrayITX = 0;
	
	//set symbol	
	if(DwordCfg.NormalOrBurst)
		temp_data = 0x01;
	else
		temp_data = 0x03;

	for(i=0; i<3; i++)					  
	{
		ArrayITX[cnt_ArrayITX++] = temp_data & 0x01;
		temp_data = temp_data>>1;
	}
		
	if(DwordCfg.NormalOrBurst)
	{
		// set addr in Normal mode
		temp_data = DwordCfg.addr;
		for(i=0; i<16; i++)					   
		{
			ArrayITX[cnt_ArrayITX++] =  temp_data & 0x01;
			temp_data = temp_data>>1;
		}
	}
	
	//push over, now start to pop data from ArrayITX to STC bus
	cnt_ArrayITX=0;

	SO_SetBits(TMS);
	SO_SetClock();
	SO_ResetClock();

	SO_ResetBits(TMS);
	for(i=0; i<3; i++)
	{
		SO_WriteBits(TDI,(BitAction)ArrayITX[cnt_ArrayITX++]);
		SO_SetClock();
		SO_ResetClock();
	}
	
	SO_SetBits(TMS);
	SO_SetClock();
	SO_ResetClock();

	SO_ResetBits(TMS);	

    if(DwordCfg.NormalOrBurst)
	{
		for(i=0; i<16; i++)
		{
			SO_WriteBits(TDI,(BitAction)ArrayITX[cnt_ArrayITX++]);
			SO_SetClock();
			SO_ResetClock();
		}
	}

	for(k=0;k<5;k++)
	{
		for(j=0;j<controlcnt[k];j++)
		{
			 temp_data = k*10;
			 SO_WriteBits(TDI,(BitAction)STCcontrol[temp_data]);
			 SO_SetClock();
			 SO_ResetClock();
			 SO_WriteBits(TDI,(BitAction)STCcontrol[temp_data+1]);
			 SO_SetClock();
			 SO_ResetClock();
			 SO_WriteBits(TDI,(BitAction)STCcontrol[temp_data+2]);
			 SO_SetClock();
			 SO_ResetClock();
			 SO_WriteBits(TDI,(BitAction)STCcontrol[temp_data+3]);
			 SO_SetClock();
			 SO_ResetClock();
			 SO_WriteBits(TDI,(BitAction)STCcontrol[temp_data+4]);
			 SO_SetClock();
			 SO_ResetClock();
			 SO_WriteBits(TDI,(BitAction)STCcontrol[temp_data+5]);
			 SO_SetClock();
			 SO_ResetClock();
			 SO_WriteBits(TDI,(BitAction)STCcontrol[temp_data+6]);
			 SO_SetClock();
			 SO_ResetClock();
			 SO_WriteBits(TDI,(BitAction)STCcontrol[temp_data+7]);
			 SO_SetClock();
			 SO_ResetClock();
			 SO_WriteBits(TDI,(BitAction)STCcontrol[temp_data+8]);
			 SO_SetClock();
			 SO_ResetClock();
			 if((k==(5-1))&&(j==(controlcnt[k]-1)))
			 {
			 	SO_SetBits(TMS);
			 }
			 SO_WriteBits(TDI,(BitAction)STCcontrol[temp_data+9]);
			 SO_SetClock();
			 SO_ResetClock();	
		}

	}

	SO_ResetBits(TMS);
	SO_SetClock();
	SO_ResetClock();

	SO_SetClock();
	SO_ResetClock();

	SO_SetClock();
	SO_ResetClock();

	for(i=0; i<31; i++)
	{
		SO_SetBits(TDI);
		ArrayITX[i]=SI_GetBits();  
		SO_SetClock();
		SO_ResetClock();
	}

	SO_SetBits(TMS);
	SO_SetBits(TDI);
	ArrayITX[i]=SI_GetBits();  		//bit32
	SO_SetClock();
	SO_ResetClock();

	SO_ResetBits(TMS);
	SO_SetClock();
	SO_ResetClock();

	SO_SetClock();
	SO_ResetClock();

	SO_SetBits(TMS);
	SO_SetClock();
	SO_ResetClock();

	SO_ResetBits(TMS);
	SO_SetClock();
	SO_ResetClock();

	temp_rd=0;

	temp_data = 0x01;
	for(i=0;i<32;i++)
	{			  
		if(ArrayITX[i])
		{
			temp_rd |= temp_data;
		}
		temp_data = (temp_data << 1);
	}

	DwordCfg.data[0]=temp_rd&0xff;
	DwordCfg.data[1]=(temp_rd>>8)&0xff;
	DwordCfg.data[2]=(temp_rd>>16)&0xff;
	DwordCfg.data[3]=(temp_rd>>24)&0xff;
}

void STC_Write(LPSTC_CONFIG pcfg)
{
	
	u32 i,k,l;

	if(pcfg->MainOrInfo)
	{
		control[0]=0x200; controlcnt[0]=1;
		control[1]=0x201; controlcnt[1]=1;
		control[2]=0x221; controlcnt[2]=1;
		control[3]=0x261; controlcnt[3]=1;
		control[4]=0x263; controlcnt[4]=6; //3
		control[5]=0x261; controlcnt[5]=1;
		control[6]=0x241; controlcnt[6]=2;
		control[7]=0x201; controlcnt[7]=1;
	   	control[8]=0x200; controlcnt[8]=1;
	}
	else
	{
		control[0]=0x280; controlcnt[0]=1;
		control[1]=0x281; controlcnt[1]=1;
		control[2]=0x2a1; controlcnt[2]=1;
		control[3]=0x2e1; controlcnt[3]=1;
		control[4]=0x2e3; controlcnt[4]=6; //3
		control[5]=0x2e1; controlcnt[5]=1;
		control[6]=0x2c1; controlcnt[6]=2;
		control[7]=0x281; controlcnt[7]=1;
	   	control[8]=0x280; controlcnt[8]=1;
	}

	for(k=0;k<9;k++)
	{
		for(l=0;l<10;l++)
		{
			STCcontrol[k*10+l] = (control[k]>>l) & 1;
		}
	}

 	//use normal mode to write first dword
	//recal addr
	//if(pcfg->MainOrInfo)
	//	readdr=pcfg->addr-0xA0000000;

	DwordCfg.addr=(pcfg->addr)>>2;
	DwordCfg.data=pcfg->data;
	DwordCfg.NormalOrBurst=TRUE;
	STC_Write_DWORD();
	
	
	for(i=1;i<(pcfg->cnt)>>2;i++)
	{
		DwordCfg.addr=((pcfg->addr)>>2)+i;
		DwordCfg.data=&(pcfg->data[i<<2]);
		DwordCfg.NormalOrBurst=FALSE;
		STC_Write_DWORD();
	}	
}

void STC_Read(LPSTC_CONFIG pcfg)
{
	u32 i,k,l;
	//u32 readdr;
	//set control signals
	if(pcfg->MainOrInfo)
	{
		control[0]=0x201; controlcnt[0]=2;
		control[1]=0x200; controlcnt[1]=1;
   		control[2]=0x203; controlcnt[2]=1;
		control[3]=0x207; controlcnt[3]=1;
		control[4]=0x203; controlcnt[4]=1;
	}
	else
	{
		control[0]=0x281; controlcnt[0]=2;
		control[1]=0x280; controlcnt[1]=1;
   		control[2]=0x283; controlcnt[2]=1;
		control[3]=0x287; controlcnt[3]=1;
		control[4]=0x283; controlcnt[4]=1;
	}

	for(k=0;k<5;k++)
	{
		for(l=0;l<10;l++)
		{
			STCcontrol[k*10+l] = (control[k]>>l) & 1;
		}
	}

 	//use normal mode to write first dword
	//recal addr
	//if(pcfg->MainOrInfo)
	//	readdr=pcfg->addr-0xA0000000;
	DwordCfg.addr=(pcfg->addr)>>2;
	DwordCfg.data=pcfg->data;
	DwordCfg.NormalOrBurst=TRUE;
	STC_Read_DWORD();
	
	
	for(i=1;i<(pcfg->cnt)>>2;i++)
	{
		DwordCfg.addr=((pcfg->addr)>>2)+i;
		DwordCfg.data=&(pcfg->data[i<<2]);
		DwordCfg.NormalOrBurst=FALSE;
		STC_Read_DWORD();
	}	
}

bool STC_PWDPageEarse()
{
	u32 tmp[2];
	STC_CONFIG tmpcfg;
	//pwd page earse
	DwordCfg.MainOrInfo=FALSE;
	DwordCfg.addr=INFO_PAGE_1_READDR>>2;
	STC_PageErase();
	//write a5a5 flag to the last 8 bytes of the pwd page earse
	tmp[0]=0x5a5a5a5a;
	tmp[1]=0xa5a5a5a5;
	tmpcfg.MainOrInfo=FALSE;
	tmpcfg.addr=0x7f8;
	tmpcfg.data=(u8*)tmp;
	tmpcfg.cnt=8;
	STC_Write(&tmpcfg);

	//verify a5a5 flag
	/*tmpcfg.MainOrInfo=FALSE;
	tmpcfg.addr=0x7f8;
	tmpcfg.data=(u8*)tmp;
	tmpcfg.cnt=8;
	STC_Read(&tmpcfg);

	if((tmp[0]==0x5a5a5a5a)&&(tmp[1]==0xa5a5a5a5))
		return TRUE;
	else 
		return FALSE;*/
	return TRUE;
}

bool STC_Restore()
{
	bool Res;
	//main mass earse
	STC_MassEraseCmd();
	// pwd page earse
	Res=STC_PWDPageEarse();

	//
	STC_pwd_authed=FALSE;

	return Res;
}

void STC_WritePWD(u8* data)
{
	//byte 0 - 3: max_quth_times
	//byte 4 - 19: pwd
	//byte 20 - 51: flash lock bit
	//byte 52 - 67: flash bus key
	//byte 68 - 71: flash bus key enable
	STC_CONFIG tmpcfg;
	u8 tmp[4];
	//pwd page earse
	STC_Restore();
	//check flash bus key , if all FFs, replace it with MCU chip id
	if(memcmpfixed(&data[52],0xFF,16))
	{
		STC_GetChipID(&data[52]);
	}
	//write all kinds of password
	tmpcfg.MainOrInfo=FALSE;
	tmpcfg.addr=PWD_MAX_AUTH_OFFSET;
	tmpcfg.data=&data[0];
	tmpcfg.cnt=PWD_MAX_AUTH_LEN;
	STC_Write(&tmpcfg);

	tmpcfg.MainOrInfo=FALSE;
	tmpcfg.addr=PWD_INFO_OFFSET;
	tmpcfg.data=&data[4];
	tmpcfg.cnt=PWD_LEN;
	STC_Write(&tmpcfg);

	tmpcfg.MainOrInfo=FALSE;
	tmpcfg.addr=FLASH_LOCK_BIT_OFFSET;
	tmpcfg.data=&data[20];
	tmpcfg.cnt=FLASH_LOCK_BIT_LEN;
	STC_Write(&tmpcfg);

	tmpcfg.MainOrInfo=FALSE;
	tmpcfg.addr=FLASH_BUS_KEY_OFFSET;
	tmpcfg.data=&data[52];
	tmpcfg.cnt=FLASH_BUS_KEY_LEN;
	STC_Write(&tmpcfg);

	tmpcfg.MainOrInfo=FALSE;
	tmpcfg.addr=FLASH_BUS_KEY_EN_OFFSET;
	tmpcfg.data=&data[68];
	tmpcfg.cnt=FLASH_BUS_KEY_EN_LEN;
	STC_Write(&tmpcfg);
	
	//write pwd flag
	tmp[0]=FISH_MAN;
	tmpcfg.MainOrInfo=FALSE;
	tmpcfg.addr=PWD_FLAG_OFFSET;
	tmpcfg.data=tmp;
	tmpcfg.cnt=PWD_FLAG_LEN;
	STC_Write(&tmpcfg);	

	STC_pwd_authed=TRUE;
}

bool STC_AUTHPWD(u8* pwd)
{
	STC_CONFIG tmpcfg;
	u8 tmp[PWD_LEN];
	u32 tmp32;
	u32 i;
	//read password
	tmpcfg.MainOrInfo=FALSE;
	tmpcfg.addr=PWD_INFO_OFFSET;
	tmpcfg.data=tmp;
	tmpcfg.cnt=PWD_LEN;
	STC_Read(&tmpcfg);
	
	if(memcmp(tmp,pwd,PWD_LEN)!=0)
	{
		//read max_auth_times
		tmpcfg.addr=PWD_MAX_AUTH_OFFSET;
		tmpcfg.data=tmp;
		tmpcfg.cnt=PWD_MAX_AUTH_LEN;
		STC_Read(&tmpcfg);

	   	if((tmp[0]!=0)&&(tmp[0]<32))//0 means 00
		{
			tmpcfg.addr=CUR_PWD_OFFSET;
			tmpcfg.data=(u8 *)&tmp32;
			tmpcfg.cnt=CUR_PWD_LEN;
			STC_Read(&tmpcfg);

 			for(i=0;i<32;i++)
			{
				if((tmp32&(1<<i)))
					break;
			}

			if(i>=tmp[0])
			{
				STC_Restore();
			}
			else
			{
				tmp32 &= ~(1<<i);
				tmpcfg.addr=CUR_PWD_OFFSET;
				tmpcfg.data=(u8 *)&tmp32;
				tmpcfg.cnt=CUR_PWD_LEN;
				STC_Write(&tmpcfg);
			}
		}
		else if(tmp[0]>=32)
		{
			//invalid value
			STC_Restore();
		}
		

		return FALSE;
	}
	STC_pwd_authed=TRUE;
	return TRUE;	
}

bool STC_CheckPWDValid(void)
{
	STC_CONFIG tmpcfg;
	u8 tmp[4];

	//read pwd flag 
	tmpcfg.MainOrInfo=FALSE;
	tmpcfg.addr=PWD_FLAG_OFFSET;
	tmpcfg.data=tmp;
	tmpcfg.cnt=PWD_FLAG_LEN;
	STC_Read(&tmpcfg);

	if(tmp[0]==FISH_MAN)
		return TRUE;
	else
		return FALSE;
}

u8 STC_GetMAXAUTHTimes(void)
{
	STC_CONFIG tmpcfg;
	u32 tmp32,i;
	u8 tmp[4];

	//read cur_auth_times
	tmpcfg.MainOrInfo=FALSE;
	tmpcfg.addr=CUR_PWD_OFFSET;
	tmpcfg.data=(u8*)&tmp32;
	tmpcfg.cnt=CUR_PWD_LEN;
	STC_Read(&tmpcfg);

	for(i=0;i<32;i++)
	{
		if((tmp32&(1<<i)))
			break;
	}

	//read max_auth_times
	tmpcfg.MainOrInfo=FALSE;
	tmpcfg.addr=PWD_MAX_AUTH_OFFSET;
	tmpcfg.data=tmp;
	tmpcfg.cnt=PWD_MAX_AUTH_LEN;
	STC_Read(&tmpcfg);

	return tmp[0]-i;
}

bool STC_RemovePWD(void)
{
	return STC_Restore();
}

bool STC_GetPwdAuthed(void)
{
	return STC_pwd_authed;
}

void STC_GetPID(u8* pid)
{
	STC_CONFIG tmpcfg;
	u8 tmp[4];
	//read PID
	tmpcfg.MainOrInfo=FALSE;
	tmpcfg.addr=MCU_PID_OFFSET;
	tmpcfg.data=tmp;
	tmpcfg.cnt=MCU_PID_LEN;
	STC_Read(&tmpcfg);

	pid[0]=tmp[0];
	pid[1]=tmp[1];
}
void STC_GetChipVersion(u8* version)
{
	STC_CONFIG tmpcfg;
	u8 tmp[4];
	//read VERSION
	tmpcfg.MainOrInfo=FALSE;
	tmpcfg.addr=MCU_REVISION_OFFSET;
	tmpcfg.data=tmp;
	tmpcfg.cnt=MCU_REVISION_LEN;
	STC_Read(&tmpcfg);

	*version=tmp[3];
}
void STC_GetChipID(u8* ChipID)
{
	STC_CONFIG tmpcfg;

	//read chip id
	tmpcfg.MainOrInfo=FALSE;
	tmpcfg.addr=MCU_CHIP_ID_OFFSET;
	tmpcfg.data=ChipID;
	tmpcfg.cnt=MCU_CHIP_ID_LEN;
	STC_Read(&tmpcfg);
}
void STC_GetFlashLockBit(u8* FlashLock)
{
	STC_CONFIG tmpcfg;

	//read flash lock bit
	tmpcfg.MainOrInfo=FALSE;
	tmpcfg.addr=FLASH_LOCK_BIT_OFFSET;
	tmpcfg.data=FlashLock;
	tmpcfg.cnt=FLASH_LOCK_BIT_LEN;
	STC_Read(&tmpcfg);
}

#ifdef STC_TEST
u8 test_wr[1024];
u8 test_rd[1024];
u8 error_no[256];
bool test_res;
void STC_Test(void)
{
	int i,j,k;
	STC_CONFIG tmpcfg;
	STC_Connect();

	STC_MassEraseCmd();

	for(i=0;i<256;i++)
		error_no[i]=0;
	k=0;

	for(j=0;j<256;j++)
	{
		for(i=0;i<1024;i++)
		{
			test_wr[i]=0xFF;
		}
		tmpcfg.MainOrInfo=TRUE;
		tmpcfg.addr=0x00000+(j)*1024;
		tmpcfg.data=test_wr;
		tmpcfg.cnt=1024;
		//STC_Write(&tmpcfg);
		tmpcfg.data=test_rd;
		STC_Read(&tmpcfg);
		test_res=TRUE;
		for(i=0;i<1024;i++)
		{
			if(test_wr[i]!=test_rd[i])
			{
				test_res=FALSE;
				error_no[k++]=j;
				break;
			}
		}
	}

	for(i=0;i<256;i++)
		error_no[i]=0;
	k=0;

	//STC_Connect();
	for(j=0;j<256;j++)
	{
		for(i=0;i<1024;i++)
		{
			test_wr[i]=j;
		}
		tmpcfg.MainOrInfo=TRUE;
		tmpcfg.addr=0x00000+(j)*1024;
		tmpcfg.data=test_wr;
		tmpcfg.cnt=1024;
		STC_Write(&tmpcfg);
		tmpcfg.data=test_rd;
		STC_Read(&tmpcfg);
		test_res=TRUE;
		for(i=0;i<1024;i++)
		{
			if(test_wr[i]!=test_rd[i])
			{
				test_res=FALSE;
				error_no[k++]=j;
				break;
			}
		}
	}

	for(i=0;i<256;i++)
		error_no[i]=0;
	k=0;

	//STC_Connect();
	for(j=0;j<256;j++)
	{
		for(i=0;i<1024;i++)
		{
			test_wr[i]=j;
		}
		tmpcfg.MainOrInfo=TRUE;
		tmpcfg.addr=0x00000+(j)*1024;
		tmpcfg.data=test_wr;
		tmpcfg.cnt=1024;
		//STC_Write(&tmpcfg);
		tmpcfg.data=test_rd;
		STC_Read(&tmpcfg);
		test_res=TRUE;
		for(i=0;i<1024;i++)
		{
			if(test_wr[i]!=test_rd[i])
			{
				test_res=FALSE;
				error_no[k++]=j;
				break;
			}
		}
	}
}

#endif




