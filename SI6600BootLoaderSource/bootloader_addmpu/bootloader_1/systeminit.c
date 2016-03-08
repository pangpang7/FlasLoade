// unsigned int a = 22;
// unsigned int b = 33; 
#include "../head/boot.h"

extern void iap_func(void);
extern void jump2iap(void);
extern unsigned char iap_InitMPU(void);



volatile const unsigned char CodeCheckSum[] __attribute__((used)) __attribute__((at(0x7F8))) =
{
	0x5a,0x5a,0x5a,0x5a,
	0xa5,0xa5,0xa5,0xa5
};

void SystemInit(void)
{
	
	unsigned int rdata1;
	unsigned int rdata2;
	unsigned char i;
	unsigned int j;
	unsigned char iap_en;
	
	BOOT0_SFR = BOOT0_SFR & ~0x10000;  //disable swd

//////////////////////////////////////////////////////////////////
//    Fill Calibration Value
//////////////////////////////////////////////////////////////////
	CAL0_F16M = *((volatile unsigned char *)(0x000003c0));
	CAL0_F8M  = *((volatile unsigned char *)(0x000003c4));
	CAL1_F4M  = *((volatile unsigned char *)(0x000003c8));
	CAL1_F2M  = *((volatile unsigned char *)(0x000003cc));
	CAL2      = *((volatile unsigned short *)(0x000003d0));
	CAL3      = *((volatile unsigned short *)(0x000003d4));
	
	rdata1    = *((volatile unsigned char *)(0x000003d8)) & 0x07;
	rdata2    = BOOT1_SFR & 0x1f8;
	BOOT1_SFR = rdata1 | rdata2;
	
	rdata1    = *((volatile unsigned char *)(0x000003dc)) & 0x7e;
	rdata2    = BGR_SFR & 0x81;
	BGR_SFR   = rdata1 | rdata2;
	
	CAL4_IRC38K = *((volatile unsigned char *)(0x000003e0));
	
	CAL4_CID0   = *((volatile unsigned char *)(0x000003e8));
	CAL5_CID1   = *((volatile unsigned char *)(0x000003e9));
	CAL5_CID2   = *((volatile unsigned char *)(0x000003ea));
	CAL6_CID3   = *((volatile unsigned char *)(0x000003eb));
	CAL6_CID4   = *((volatile unsigned char *)(0x000003ec));
	CAL7_CID5   = *((volatile unsigned char *)(0x000003ed));
	CAL7_CID6   = *((volatile unsigned char *)(0x000003ee));
	
//////////////////////////////////////////////////////////////////
//    check busenc
//    if enable, fill busenc key, enable busenc
//////////////////////////////////////////////////////////////////
  rdata1 = *((volatile unsigned char *)(0x00000700));
	if(rdata1 == 0x55){  //busenc enable
		BUSKEY0 = *((volatile unsigned short *)(0x00000400));
		BUSKEY1 = *((volatile unsigned short *)(0x00000402));
		BUSKEY2 = *((volatile unsigned short *)(0x00000404));
		BUSKEY3 = *((volatile unsigned short *)(0x00000406));
		BUSKEY4 = *((volatile unsigned short *)(0x00000408));
		BUSKEY5 = *((volatile unsigned short *)(0x0000040a));
		BUSKEY6 = *((volatile unsigned short *)(0x0000040c));
		BUSKEY7 = *((volatile unsigned short *)(0x0000040e));
		
		//BOOT1_SFR = BOOT1_SFR | 0x200;  //enable busenc
	}
	else{  //busenc disable
		
	}
	
//////////////////////////////////////////////////////////////////
//    Page lock
//////////////////////////////////////////////////////////////////
	MAIN_PL0 = *((volatile unsigned int *)(0x00000500));
	MAIN_PL1 = *((volatile unsigned int *)(0x00000504));
	MAIN_PL2 = *((volatile unsigned int *)(0x00000508));
	MAIN_PL3 = *((volatile unsigned int *)(0x0000050c));
	MAIN_PL4 = *((volatile unsigned int *)(0x00000510));
	MAIN_PL5 = *((volatile unsigned int *)(0x00000514));
	MAIN_PL6 = *((volatile unsigned int *)(0x00000518));
	MAIN_PL7 = *((volatile unsigned int *)(0x0000051c));	
	INFO_PL  = INFO_PL & ~0x1;
	
//////////////////////////////////////////////////////////////////
//    Password
//////////////////////////////////////////////////////////////////
	PASSWORD0 = *((volatile unsigned short *)(0x00000600));
	PASSWORD1 = *((volatile unsigned short *)(0x00000602));
	PASSWORD2 = *((volatile unsigned short *)(0x00000604));
	PASSWORD3 = *((volatile unsigned short *)(0x00000606));
	PASSWORD4 = *((volatile unsigned short *)(0x00000608));
	PASSWORD5 = *((volatile unsigned short *)(0x0000060a));
	PASSWORD6 = *((volatile unsigned short *)(0x0000060c));
	PASSWORD7 = *((volatile unsigned short *)(0x0000060e));

	/*PASSWORD0 = 0xFFFF;
	PASSWORD1 = 0xFFFF;
	PASSWORD2 = 0xFFFF;
	PASSWORD3 = 0xFFFF;
	PASSWORD4 = 0xFFFF;
	PASSWORD5 = 0xFFFF;
	PASSWORD6 = 0xFFFF;
	PASSWORD7 = 0xFFFF;*/
//////////////////////////////////////////////////////////////////
//    disable LOCKUP reset
//    if LOCKUP enable, swd cannot connect when main block is 0xFF
//////////////////////////////////////////////////////////////////
	SYSCTRL1  = SYSCTRL1 & ~0x01;

//////////////////////////////////////////////////////////////////
//    Flag, for debug only
//////////////////////////////////////////////////////////////////
	//P1_DIR = 0x700;
	//P1 = 0xc0;
	
//////////////////////////////////////////////////////////////////
//    Check IAP
//    P6[1:0] == 2'b10, enter IAP code
//    others, enter user code
//////////////////////////////////////////////////////////////////
  P6_DIR = P6_DIR | 0x3;
	P6_PH  = P6_PH | 0x3;
  iap_en = 0;	
	
	for(i=0;i<0x10;i++){     //check P6[1:0] for 0x10 times
		for(j=0;j<0x1000;j++); //wait for a short time
		rdata1 = P6 & 0x3;
		if(rdata1 != 0x2){
			break;
		}
		if(i==0xf){
			iap_en = 1;
		}
	}
		
	if(iap_en == 1){  //P6[1:0]==2'b10 check ok, go to IAP
		
		iap_InitMPU();
    jump2iap();	
    
		
	}
	else {  //check error, go to bootloader main
		BOOT0_SFR = BOOT0_SFR | 0x10000;	  //enable swd
	}

	
}

