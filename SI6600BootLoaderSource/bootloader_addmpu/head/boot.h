#ifndef __REG_H__
#define __REG_H__
			
			
#define PASSWORD_BASEADDR       (0x40002400)

#define PASSWORD0      *((volatile unsigned short *)(PASSWORD_BASEADDR+0x40))
#define PASSWORD1      *((volatile unsigned short *)(PASSWORD_BASEADDR+0x44))
#define PASSWORD2      *((volatile unsigned short *)(PASSWORD_BASEADDR+0x48))
#define PASSWORD3      *((volatile unsigned short *)(PASSWORD_BASEADDR+0x4c))
	
#define PASSWORD4      *((volatile unsigned short *)(PASSWORD_BASEADDR+0x50))
#define PASSWORD5      *((volatile unsigned short *)(PASSWORD_BASEADDR+0x54))
#define PASSWORD6      *((volatile unsigned short *)(PASSWORD_BASEADDR+0x58))
#define PASSWORD7      *((volatile unsigned short *)(PASSWORD_BASEADDR+0x5c))

#define BUSKEY0        *((volatile unsigned short *)(PASSWORD_BASEADDR+0x60))
#define BUSKEY1        *((volatile unsigned short *)(PASSWORD_BASEADDR+0x64))
#define BUSKEY2        *((volatile unsigned short *)(PASSWORD_BASEADDR+0x68))
#define BUSKEY3        *((volatile unsigned short *)(PASSWORD_BASEADDR+0x6c))
	
#define BUSKEY4        *((volatile unsigned short *)(PASSWORD_BASEADDR+0x70))
#define BUSKEY5        *((volatile unsigned short *)(PASSWORD_BASEADDR+0x74))
#define BUSKEY6        *((volatile unsigned short *)(PASSWORD_BASEADDR+0x78))
#define BUSKEY7        *((volatile unsigned short *)(PASSWORD_BASEADDR+0x7c))	

#define CAL0_F16M      *((volatile unsigned char *)(PASSWORD_BASEADDR+0x80))
#define CAL0_F8M       *((volatile unsigned char *)(PASSWORD_BASEADDR+0x81))	
#define CAL1_F4M       *((volatile unsigned char *)(PASSWORD_BASEADDR+0x84))
#define CAL1_F2M       *((volatile unsigned char *)(PASSWORD_BASEADDR+0x85))
#define CAL2           *((volatile unsigned short *)(PASSWORD_BASEADDR+0x88))
#define CAL3           *((volatile unsigned short *)(PASSWORD_BASEADDR+0x8c))

#define CAL4_CID0      *((volatile unsigned char *)(PASSWORD_BASEADDR+0x90))
#define CAL4_IRC38K    *((volatile unsigned char *)(PASSWORD_BASEADDR+0x91))
#define CAL5_CID1      *((volatile unsigned char *)(PASSWORD_BASEADDR+0x94))
#define CAL5_CID2      *((volatile unsigned char *)(PASSWORD_BASEADDR+0x95))
#define CAL6_CID3      *((volatile unsigned char *)(PASSWORD_BASEADDR+0x98))
#define CAL6_CID4      *((volatile unsigned char *)(PASSWORD_BASEADDR+0x99))
#define CAL7_CID5      *((volatile unsigned char *)(PASSWORD_BASEADDR+0x9c))	
#define CAL7_CID6      *((volatile unsigned char *)(PASSWORD_BASEADDR+0x9d))
	
#define GPIO_BASEADDR          (0x40012c00)

#define P1             *((volatile unsigned int *)(GPIO_BASEADDR+0x40))
#define P1_DIR         *((volatile unsigned int *)(GPIO_BASEADDR+0x44))
#define P6             *((volatile unsigned int *)(GPIO_BASEADDR+0x180))
#define P6_DIR         *((volatile unsigned int *)(GPIO_BASEADDR+0x184))
#define P6_PH          *((volatile unsigned int *)(GPIO_BASEADDR+0x198))

#define SYSCTRL_BASEADDR       (0x40013400)

#define SYSCTRL1       *((volatile unsigned int *)(SYSCTRL_BASEADDR+0x04))
#define BGR_SFR        *((volatile unsigned int *)(SYSCTRL_BASEADDR+0x28))
#define BOOT0_SFR      *((volatile unsigned int *)(SYSCTRL_BASEADDR+0x104))
#define BOOT1_SFR      *((volatile unsigned int *)(SYSCTRL_BASEADDR+0x108))

#define EFCTRL_BASEADDR        (0x40020000)

#define MAIN_PL0       *((volatile unsigned int *)(EFCTRL_BASEADDR+0x38))
#define MAIN_PL1       *((volatile unsigned int *)(EFCTRL_BASEADDR+0x3c))
#define MAIN_PL2       *((volatile unsigned int *)(EFCTRL_BASEADDR+0x40))
#define MAIN_PL3       *((volatile unsigned int *)(EFCTRL_BASEADDR+0x44))
#define MAIN_PL4       *((volatile unsigned int *)(EFCTRL_BASEADDR+0x48))
#define MAIN_PL5       *((volatile unsigned int *)(EFCTRL_BASEADDR+0x4c))
#define MAIN_PL6       *((volatile unsigned int *)(EFCTRL_BASEADDR+0x50))
#define MAIN_PL7       *((volatile unsigned int *)(EFCTRL_BASEADDR+0x54))	
#define INFO_PL        *((volatile unsigned int *)(EFCTRL_BASEADDR+0x58))
	
	
#define REMAP_SEL      *((volatile unsigned int *)(EFCTRL_BASEADDR+0x60))

#endif
