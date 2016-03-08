;/*---------------------------------------------------------------------------------------------------------*/
;/*                                                                                                         */
;/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
;/*                                                                                                         */
;/*---------------------------------------------------------------------------------------------------------*/

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

                 GBLL    SEMIHOSTED
SEMIHOSTED       SETL    {FALSE}


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


Stack_Size      EQU     0x00000400

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000000

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset
                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; External Interrupts
                                                  ; maximum of 32 External Interrupts are possible
                DCD     BOD_IRQHandler  
                DCD     WDT_IRQHandler  
                DCD     EINT0_IRQHandler
                DCD     EINT1_IRQHandler
                DCD     GPAB_IRQHandler 
                DCD     GPCD_IRQHandler
                DCD     PWMA_IRQHandler 
                DCD     Default_Handler 
                DCD     TMR0_IRQHandler 
                DCD     TMR1_IRQHandler 
                DCD     TMR2_IRQHandler 
                DCD     TMR3_IRQHandler 
                DCD     UART0_IRQHandler
                DCD     UART1_IRQHandler
                DCD     SPI0_IRQHandler 
                DCD     SPI1_IRQHandler 
                DCD     Default_Handler 
                DCD     Default_Handler 
                DCD     Default_Handler 
                DCD     I2C_IRQHandler 
                DCD     Default_Handler 
                DCD     TIMER_IRQHandler
                DCD     BTIMER_IRQHandler 
                DCD     USBD_IRQHandler  
                DCD     PS2_IRQHandler  
                DCD     Default_Handler 
                DCD     Default_Handler
                DCD     Default_Handler 
                DCD     PWRWU_IRQHandler
                DCD     Default_Handler
                DCD     Default_Handler  
                DCD     RTC_IRQHandler                                   
                
                AREA    |.text|, CODE, READONLY              
                
; Reset Handler 
                
                ENTRY
                
Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  SystemInit
                      
                LDR     R0, =SystemInit
                BLX     R0
				
				;read user MSP
				LDR     R0,  =0xa0000000  
				LDR     R1,  [R0, #0]
				;save user MSP in R13
				MOV     R13, R1           
			   
			    ;read user code reset address
				LDR     R0, =0xa0000004
				LDR     R1, [R0, #0]

                ;set sfr_lock = 1
                ;LDR     R4, =0x40013500
				
				;set remap register = 1
			    LDR     R2, =0x40020060
                LDR     R3, =0x00000001				
                STR     R3, [R2]		
				
				;set sfr_lock = 1							
                ;STR     R3, [R4]
				
                BX      R1
				
				nop
				nop				
				
                ENDP
                
                
; Dummy Exception Handlers (infinite loops which can be modified)                
                
NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
    IF SEMIHOSTED
                LDR    R0, [R13, #24]        ; Get previous PC
                LDRH   R1, [R0]              ; Get instruction
                LDR    R2, =0xBEAB           ; The sepcial BKPT instruction
                CMP    R1, R2                ; Test if the instruction at previous PC is BKPT
                BNE    HardFault_Handler_Ret ; Not BKPT
        
                ADDS   R0, #4                ; Skip BKPT and next line
                STR    R0, [R13, #24]        ; Save previous PC
        
                BX     LR
HardFault_Handler_Ret
    ENDIF
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler           [WEAK]
                B       .
                ENDP

Default_Handler PROC

                EXPORT  BOD_IRQHandler            [WEAK]
                EXPORT  WDT_IRQHandler            [WEAK]
                EXPORT  EINT0_IRQHandler          [WEAK]
                EXPORT  EINT1_IRQHandler          [WEAK]
                EXPORT  GPAB_IRQHandler           [WEAK]
                EXPORT  GPCD_IRQHandler           [WEAK]
                EXPORT  PWMA_IRQHandler           [WEAK]
                EXPORT  TMR0_IRQHandler           [WEAK]
                EXPORT  TMR1_IRQHandler           [WEAK]
                EXPORT  TMR2_IRQHandler           [WEAK]
                EXPORT  TMR3_IRQHandler           [WEAK]
                EXPORT  UART0_IRQHandler          [WEAK]
                EXPORT  UART1_IRQHandler          [WEAK]
                EXPORT  SPI0_IRQHandler           [WEAK]
                EXPORT  SPI1_IRQHandler           [WEAK]
                EXPORT  I2C_IRQHandler            [WEAK]
                EXPORT  USBD_IRQHandler           [WEAK]
                EXPORT  PS2_IRQHandler            [WEAK]
                EXPORT  PWRWU_IRQHandler          [WEAK]
                EXPORT  RTC_IRQHandler            [WEAK]
					
				EXPORT	TIMER_IRQHandler          [WEAK]     ;EXT INTR 21
				EXPORT	BTIMER_IRQHandler         [WEAK]     ;EXT INTR 22
                
BOD_IRQHandler
WDT_IRQHandler
EINT0_IRQHandler
EINT1_IRQHandler
GPAB_IRQHandler
GPCD_IRQHandler
PWMA_IRQHandler
TMR0_IRQHandler
TMR1_IRQHandler
TMR2_IRQHandler
TMR3_IRQHandler
UART0_IRQHandler
UART1_IRQHandler
SPI0_IRQHandler
SPI1_IRQHandler
I2C_IRQHandler
USBD_IRQHandler
PS2_IRQHandler
PWRWU_IRQHandler
RTC_IRQHandler

TIMER_IRQHandler
BTIMER_IRQHandler

                B       .
                ENDP


                ALIGN


; User Initial Stack & Heap

                IF      :DEF:__MICROLIB
                
                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit
                
                ELSE
                
                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
__user_initial_stackheap

                LDR     R0, =  Heap_Mem
                LDR     R1, = (Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR

                ALIGN

                ENDIF

    IF SEMIHOSTED

                ALIGN

; int SH_DoCommand(int n32In_R0, int n32In_R1, int *pn32Out_R0);
; Input
;	R0,n32In_R0: semihost register 0
;	R1,n32In_R1: semihost register 1
; Output
;	R2,*pn32Out_R0: semihost register 0
; Return
;	0: No ICE debug
;	1: ICE debug
SH_DoCommand	
                EXPORT SH_DoCommand
                BKPT   0xAB                  ; Wait ICE or HardFault
                                     ; ICE will step over BKPT directly
                                     ; HardFault will step BKPT and the next line
                B      SH_ICE
SH_HardFault                         ; Captured by HardFault
                MOVS   R0, #0                ; Set return value to 0
                BX     lr                    ; Return
SH_ICE                               ; Captured by ICE
                ; Save return value
                CMP    R2, #0
                BEQ    SH_End
                STR    R0, [R2]              ; Save the return value to *pn32Out_R0
SH_End
                MOVS   R0, #1                ; Set return value to 1
                BX     lr                    ; Return

                ALIGN
    ENDIF
                END
