#line 1 "FlashPrg.c"
 
 
 
 
 
 
 
 
 

#line 1 "..\\FlashOS.H"














 



#line 25 "..\\FlashOS.H"




struct FlashSectors  {
  unsigned long   szSector;    
  unsigned long AddrSector;    
};



struct FlashDevice  {
   unsigned short     Vers;    
   char       DevName[128];    
   unsigned short  DevType;    
   unsigned long    DevAdr;    
   unsigned long     szDev;    
   unsigned long    szPage;    
   unsigned long       Res;    
   unsigned char  valEmpty;    

   unsigned long    toProg;    
   unsigned long   toErase;    

   struct FlashSectors sectors[512];
};




extern          int  Init        (unsigned long adr,   
                                  unsigned long clk,
                                  unsigned long fnc);
extern          int  UnInit      (unsigned long fnc);  
extern          int  BlankCheck  (unsigned long adr,   
                                  unsigned long sz,
                                  unsigned char pat);
extern          int  EraseChip   (void);               
extern          int  EraseSector (unsigned long adr);  
extern          int  ProgramPage (unsigned long adr,   
                                  unsigned long sz,
                                  unsigned char *buf);
extern unsigned long Verify      (unsigned long adr,   
                                  unsigned long sz,
                                  unsigned char *buf);
#line 12 "FlashPrg.c"

typedef volatile unsigned char  vu8;
typedef volatile unsigned long  vu32;
typedef volatile unsigned short vu16;
















typedef struct {
  vu32 KR;
  vu32 PR;
  vu32 RLR;
  vu32 SR;
} IWDG_TypeDef;


typedef struct {
  vu32 ACR;                                     
  vu32 KEYR;                                    
  vu32 OPTKEYR;                                 
  vu32 SR;                                      
  vu32 CR;                                      
  vu32 AR;                                      
  vu32 RESERVED0[1];
  vu32 OBR;                                     
  vu32 WRPR;                                    
#line 58 "FlashPrg.c"
} FLASH_TypeDef;








#line 75 "FlashPrg.c"








unsigned long base_adr;








 


int Init (unsigned long adr, unsigned long clk, unsigned long fnc) {






















  return (0);
}



int UnInit (unsigned long fnc) {


















  return (0);
}






 
int EraseChip (void) {
(*(volatile unsigned int *)(0x40020020)) = 0x03;

(*(volatile unsigned int *)(0x00000000)) = 0x66000001;




 	






  return (0);                                   
}









 


int EraseSector (unsigned long adr) {




    ((FLASH_TypeDef*) 0x00000000)->CR  |=  0x00000002;                   
    ((FLASH_TypeDef*) 0x00000000)->AR   =  adr;                         
    ((FLASH_TypeDef*) 0x00000000)->CR  |=  0x00000040;                  

    while (((FLASH_TypeDef*) 0x00000000)->SR  & 0x00000001) {
      ((IWDG_TypeDef *) 0x40003000)->KR = 0xAAAA;                        
    }

    ((FLASH_TypeDef*) 0x00000000)->CR  &= ~0x00000002;                   
#line 208 "FlashPrg.c"

  return (0);                                   
}










 


int BlankCheck (unsigned long adr, unsigned long sz, unsigned char pat) {
  return (1);                                   
}









 

int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf)
{
  int j;

  (*(volatile unsigned int *)(0x40020020)) = 0x01;
 
  




 
  while (sz) 
   {

    

    (*(volatile unsigned int *)(adr+j))=*((unsigned int *)buf);
	adr += 4;
    buf += 4;
    sz  -= 4;
	}    
  return (0);                                   
}


































