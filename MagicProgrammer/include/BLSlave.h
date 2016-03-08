#ifndef __BLSlave_H
#define __BLSlave_H
#define NO_CMD (0x7E)

#define INIT_CON   (0x7F)
//Get the version and the allowed commands supported by the current version of the boot loader
#define GET_CMD    (0x00)  
//Get the BL version and the Read Protection status of the NVM
#define GET_VER_ROPS_CMD (0x01) 
//Get the chip ID
#define GET_ID_CMD (0x02)
//set the new baudrate
#define SET_SPEED_CMD (0x03)
//Read up to 256 bytes of memory starting from an address specified by the user
#define READ_CMD (0x11)
//Jump to an address specified by the user to execute (a loaded) code
#define GO_CMD (0x21)
//Write maximum 256 bytes to the RAM or the NVM starting from an address specified by the user
#define WRITE_CMD (0x31)
//Erase from one to all the NVM sectors
#define ERASE_CMD (0x43)
//Erase from one to all the NVM sectors
#define ERASE_EXT_CMD (0x44)
//Enable the write protection in a permanent way for some sectors
#define WRITE_PROTECT_CMD (0x63)
 //Disable the write protection in a temporary way for all NVM sectors
#define WRITE_TEMP_UNPROTECT_CMD (0x71)
 //Disable the write protection in a permanent way for all NVM sectors
#define WRITE_PERM_UNPROTECT_CMD (0x73)
 //Enable the readout protection in a permanent way
#define READOUT_PROTECT_CMD (0x82) 
 //Disable the readout protection in a temporary way
#define READOUT_TEMP_UNPROTECT_CMD (0x91) 
 //Disable the readout protection in a permanent way
#define READOUT_PERM_UNPROTECT_CMD (0x92)
//Set config to programmer
#define SET_PRO_CMD	 (0xA0)
//Get config and info from programmer
#define GET_PRO_CMD (0xA1)
//Set offline config to programmer
#define SET_OFFLINE_CMD (0xA2)
//Set password
#define SET_PWD (0xB0) 
//auth password
#define AUTH_PWD (0xB1)
//remove password 
#define REMOVE_PWD (0xB2)
//check if MCU have valid password 
#define CHK_PWD_VALID (0xB3) 

#define SUCCESS                     0x00 // No error 
#define ERROR_OFFSET                0x00 //error offset 

#define COM_ERROR_OFFSET            (ERROR_OFFSET + 0x00)
#define NO_CON_AVAILABLE            (COM_ERROR_OFFSET + 0x01)  // No serial port opened
#define COM_ALREADY_OPENED          (COM_ERROR_OFFSET + 0x02)  // Serial port already opened
#define CANT_OPEN_COM               (COM_ERROR_OFFSET + 0x03)  // Fail to open serial port
#define SEND_FAIL                   (COM_ERROR_OFFSET + 0x04)  // send over serial port fail
#define READ_FAIL                   (COM_ERROR_OFFSET + 0x05)  // Read from serial port fail

#define SYS_MEM_ERROR_OFFSET        (ERROR_OFFSET + 0x10)
#define CANT_INIT_BL                (SYS_MEM_ERROR_OFFSET + 0x01) // Fail to start system memory BL
#define UNREOGNIZED_DEVICE          (SYS_MEM_ERROR_OFFSET + 0x02) // Unreconized device
#define CMD_NOT_ALLOWED             (SYS_MEM_ERROR_OFFSET + 0x03) // Command not allowed
#define CMD_FAIL                    (SYS_MEM_ERROR_OFFSET + 0x04) // command failed

#define PROGRAM_ERROR_OFFSET        (ERROR_OFFSET + 0x20)
#define INPUT_PARAMS_ERROR          (PROGRAM_ERROR_OFFSET + 0x01)
#define INPUT_PARAMS_MEMORY_ALLOCATION_ERROR (PROGRAM_ERROR_OFFSET + 0x02)

typedef enum  {UNDEFINED=0x00, ACK=0x79, NACK=0x1F, DATA=0x55} ACKS;

typedef enum  {IDLE=0x00,CMD,SUB1,SUB2,SUB3,SUB4} STATS;

typedef enum  {ON_LINE=1,OFF_LINE=0} BL_MODE;

typedef enum  {TYPE_STC=0x00,TYPE_UART=0x01,TYPE_SWD=0x02} COM_TYPE;

typedef enum  {DTYPE_PRO=0x00,DTYPE_MCU_RAM=0X01,DTYPE_MCU_FLASH=0x02} DOWNLAOD_TYPE; 

void USBInit(void);
void USBOutCB(void);
void USBIn(void);
void RQ_init(void);
void RQ_Proc(void);

#endif
