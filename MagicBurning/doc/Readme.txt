1. USBHID protocol add unique header
0x02 0x00 0x00 0xAA ...

2. add extend config in BL_WRITE
a. set mode  (adr:0xDFFFF000-0xDFFFF003)
   BYTE 0: on line / off line
   BYTE 1: STC / UART

b. set UART config
   speedInBaud (adr:0xDFFFF004-0xDFFFF007) 
		9600/115200...
   others (adr:0xDFFFF008-0xDFFFF00B)
		nbBit (adr:0xDFFFF008)
		parity (adr:0xDFFFF009) 
		nbStopBit (adr:0xDFFFF010) 