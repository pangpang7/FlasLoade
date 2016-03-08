/***********************************************************************/
/*  This file is part of the ARM Toolchain package                     */
/*  Copyright KEIL ELEKTRONIK GmbH 2003 - 2010                         */
/***********************************************************************/
/*                                                                     */
/*  FlashDev.C:  Flash Programming Functions adapted                   */
/*               for ST Microelectronics STM32F10x Flash               */
/*                                                                     */
/***********************************************************************/

#include "..\FlashOS.H"        // FlashOS Structures

typedef volatile unsigned char  vu8;
typedef volatile unsigned long  vu32;
typedef volatile unsigned short vu16;

#define M8(adr)  (*((vu8  *) (adr)))
#define M16(adr) (*((vu16 *) (adr)))
#define M32(adr) (*((vu32 *) (adr)))

// Peripheral Memory Map
#define IWDG_BASE       0x40003000
#define FLASH_BASE      0x00000000  //0x40022000

#define IWDG            ((IWDG_TypeDef *) IWDG_BASE)
#define FLASH           ((FLASH_TypeDef*) FLASH_BASE)

// FLASH BANK size
#define BANK1_SIZE      0x00080000      // Bank1 Size = 512kB

// Independent WATCHDOG
typedef struct {
  vu32 KR;
  vu32 PR;
  vu32 RLR;
  vu32 SR;
} IWDG_TypeDef;

// Flash Registers
typedef struct {
  vu32 ACR;                                     // offset  0x000
  vu32 KEYR;                                    // offset  0x004
  vu32 OPTKEYR;                                 // offset  0x008
  vu32 SR;                                      // offset  0x00C
  vu32 CR;                                      // offset  0x010
  vu32 AR;                                      // offset  0x014
  vu32 RESERVED0[1];
  vu32 OBR;                                     // offset  0x01C
  vu32 WRPR;                                    // offset  0x020
#ifdef STM32F10x_1024
  vu32 RESERVED1[8];
  vu32 KEYR2;                                   // offset  0x044
  vu32 RESERVED2[1];
  vu32 SR2;                                     // offset  0x04C
  vu32 CR2;                                     // offset  0x050
  vu32 AR2;                                     // offset  0x054
#endif
} FLASH_TypeDef;


// Flash Keys
#define RDPRT_KEY       0x5AA5
#define FLASH_KEY1      0x45670123
#define FLASH_KEY2      0xCDEF89AB

// Flash Control Register definitions
#define FLASH_PG        0x00000001
#define FLASH_PER       0x00000002
#define FLASH_MER       0x00000004
#define FLASH_OPTPG     0x00000010
#define FLASH_OPTER     0x00000020
#define FLASH_STRT      0x00000040
#define FLASH_LOCK      0x00000080
#define FLASH_OPTWRE    0x00000100

// Flash Status Register definitions
#define FLASH_BSY       0x00000001
#define FLASH_PGERR     0x00000004
#define FLASH_WRPRTERR  0x00000010
#define FLASH_EOP       0x00000020


unsigned long base_adr;


/*
 *  Initialize Flash Programming Functions
 *    Parameter:      adr:  Device Base Address
 *                    clk:  Clock Frequency (Hz)
 *                    fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */


int Init (unsigned long adr, unsigned long clk, unsigned long fnc) {

//  base_adr = adr & ~(BANK1_SIZE - 1);          // Align to Size Boundary
//
//  // Zero Wait State
//  FLASH->ACR  = 0x00000000;
//
//  // Unlock Flash    
//  FLASH->KEYR  = FLASH_KEY1;
//  FLASH->KEYR  = FLASH_KEY2;
//#ifdef STM32F10x_1024
//  FLASH->KEYR2 = FLASH_KEY1;                    // Flash bank 2
//  FLASH->KEYR2 = FLASH_KEY2;
//#endif
//
//  // Test if IWDG is running (IWDG in HW mode)
//  if ((FLASH->OBR & 0x04) == 0x00) {
//    // Set IWDG time out to ~32.768 second
//    IWDG->KR  = 0x5555;                         // Enable write access to IWDG_PR and IWDG_RLR     
//    IWDG->PR  = 0x06;                           // Set prescaler to 256  
//    IWDG->RLR = 4095;                           // Set reload value to 4095
//  }

  return (0);
}



int UnInit (unsigned long fnc) {
//
//  // Lock Flash
//  FLASH->CR  |=  FLASH_LOCK;
//#ifdef STM32F10x_1024
//  FLASH->CR2 |=  FLASH_LOCK;                    // Flash bank 2
//#endif
//
//  return (0);
//}
//#endif
//
//#ifdef FLASH_OPT
//int UnInit (unsigned long fnc) {
//
//  // Lock Flash & Option Bytes
//  FLASH->CR &= ~FLASH_OPTWRE;
//  FLASH->CR |=  FLASH_LOCK;

  return (0);
}



/*
 *  Erase complete Flash Memory
 *    Return Value:   0 - OK,  1 - Failed
 */
int EraseChip (void) {
(*(volatile unsigned int *)(0x40020020)) = 0x03;

(*(volatile unsigned int *)(0x00000000)) = 0x66000001;

/*(*(volatile unsigned int *)(0x40012C00+0x18)) = 0x00;	
(*(volatile unsigned int *)(0x40012C00+0x04)) = 0x00;
(*(volatile unsigned int *)(0x40012C00+0x00)) = 0x00;
(*(volatile unsigned int *)(0x40012C00+0x00)) = 0x02;*/	
//	(*(volatile unsigned int *)(0x20000600)) = 0x00;	
//(*(volatile unsigned int *)(0x20000604)) = 0x04;




  return (0);                                   // Done
}





/*
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */

#ifdef FLASH_MEM
int EraseSector (unsigned long adr) {

#ifdef STM32F10x_1024
  if (adr < (base_adr + BANK1_SIZE)) {          // Flash bank 2
#endif
    FLASH->CR  |=  FLASH_PER;                   // Page Erase Enabled 
    FLASH->AR   =  adr;                         // Page Address
    FLASH->CR  |=  FLASH_STRT;                  // Start Erase

    while (FLASH->SR  & FLASH_BSY) {
      IWDG->KR = 0xAAAA;                        // Reload IWDG
    }

    FLASH->CR  &= ~FLASH_PER;                   // Page Erase Disabled 
#ifdef STM32F10x_1024
  }
  else {                                        // Flash bank 2
    FLASH->CR2 |=  FLASH_PER;
    FLASH->AR2  =  adr;
    FLASH->CR2 |=  FLASH_STRT;

    while (FLASH->SR2 & FLASH_BSY) {
      IWDG->KR = 0xAAAA;
    }

    FLASH->CR2 &= ~FLASH_PER;
  }
#endif

  return (0);                                   // Done
}
#endif



/*  
 *  Blank Check Checks if Memory is Blank
 *    Parameter:      adr:  Block Start Address
 *                    sz:   Block Size (in bytes)
 *                    pat:  Block Pattern
 *    Return Value:   0 - OK,  1 - Failed
 */


int BlankCheck (unsigned long adr, unsigned long sz, unsigned char pat) {
  return (1);                                   // Always Force Erase
}



/*
 *  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 */

int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf)
{
  //int j;

  (*(volatile unsigned int *)(0x40020020)) = 0x01;
 
  /*for(j=0;j<sz;)//256word
	{ 
	   (*(volatile unsigned int *)(adr+j))=*((unsigned int *)buf);
	   buf=buf+4;
	   j=j+4;
	}*/
  while (sz) 
   {

    //FLASH->CR |=  FLASH_OPTPG;                  // Option Byte Programming Enabled

    (*(volatile unsigned int *)(adr))=*((unsigned int *)buf);
	adr += 4;
    buf += 4;
    sz  -= 4;
	}    
  return (0);                                   // Done
}


//#ifdef FLASH_OPT
//int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf) {
//
//  sz = (sz + 1) & ~1;                           // Adjust size for Half Words
//  
//  while (sz) {
//
//    FLASH->CR |=  FLASH_OPTPG;                  // Option Byte Programming Enabled
//
//    M16(adr) = *((unsigned short *)buf);        // Program Half Word
//    while (FLASH->SR & FLASH_BSY) {
//      IWDG->KR = 0xAAAA;                        // Reload IWDG
//    }
//
//    FLASH->CR &= ~FLASH_OPTPG;                  // Option Byte Programming Disabled
//
//    // Check for Errors
//    if (FLASH->SR & (FLASH_PGERR | FLASH_WRPRTERR)) {
//      FLASH->SR |= FLASH_PGERR | FLASH_WRPRTERR;
//      return (1);                               // Failed
//    }
//
//    // Go to next Half Word
//    adr += 2;
//    buf += 2;
//    sz  -= 2;
//   
//  }
//
//  return (0);                                   // Done
//}
//#endif
