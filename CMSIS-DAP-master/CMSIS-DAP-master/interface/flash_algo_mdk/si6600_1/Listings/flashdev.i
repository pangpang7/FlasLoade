#line 1 "FlashDev.c"
 
 
 
 
 
 
 
 

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
#line 11 "FlashDev.c"

struct FlashDevice const FlashDevice  =  {
   (0x0100+1),             
   "SI6600",
   1,                     
   0x00000000,                 
   0x00040000,                 
   1024,                       
   0,                          
   0xFF,                       
   100,                        
   500,                        


   0x0800, 0x000000,           
   0xFFFFFFFF, 0xFFFFFFFF
};

















































































































 
