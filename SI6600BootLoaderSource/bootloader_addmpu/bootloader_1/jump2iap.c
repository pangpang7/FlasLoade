

__asm void jump2iap(void)
{
      
	    LDR     R0, =0xa003f804	    
			LDR     R1, [R0, #0]
	    BX      R1
	    NOP
	
//	    LDR     R0, =0xa003f801  //main page #254 start addr
//			BX      R0
	
}





