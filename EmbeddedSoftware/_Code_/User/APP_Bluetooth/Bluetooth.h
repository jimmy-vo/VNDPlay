#include "BoardConfig.h"


#define Bluetooth_DeviceMax 		12
#define Bluetooth_RXSize  			300
#define Bluetooth_AddrSize	 		(2+1+3)*2+3
#define Bluetooth_NameSize	 		20

typedef enum 
{
	Bluetooth_DeviceNone 			= 0,
	Bluetooth_DeviceAvailable = 1,
	Bluetooth_DevicePaired		= 2,
	Bluetooth_DeviceConnected	= 3,
}Bluetooth_DeviceState;

typedef struct
{
	char 										Address[Bluetooth_AddrSize];
	char 										Name[Bluetooth_NameSize];
	Bluetooth_DeviceState	 	State;
}Bluetooth_DEVICE;


typedef enum 
{
	Bluetooth_ATMODE_AT 			= 0, //First set the key as high level, 
	Bluetooth_ATMODE_COM 			= 1,
}Bluetooth_ATMODE;
uint8_t Bluetooth_SwitchMode (Bluetooth_ATMODE mode);


typedef enum 
{
	Bluetooth_CMODE_Manual 		= 0,
	Bluetooth_CMODE_Auto			= 1,
	Bluetooth_CMODE_Slaveloop	= 2,
}Bluetooth_CONNECTMODE;

typedef enum 
{
	Bluetooth_ROLE_Slave 			= 0,
	Bluetooth_ROLE_Master			= 1,
	Bluetooth_ROLE_Slaveloop	= 2,
}Bluetooth_ROLE;


void Bluetooth_Initialize(void);

uint8_t Bluetooth_SetPwrPin (uint8_t turn);
uint8_t Bluetooth_SetKeyPin (uint8_t turn);
uint8_t Bluetooth_GetPwrPin (void);
uint8_t Bluetooth_GetKeyPin (void);
uint8_t Bluetooth_GetSttPin (void);

void Bluetooth_Window (void);

void Bluetooth_PrepareRX (char * buff, uint32_t OUTPUTSIZE);
void Bluetooth_SendString (char* addr);
void Bluetooth_Send (char* addr, uint16_t byte);
void Bluetooth_SetBaud (uint32_t baud);
void Bluetooth_SetupUART (uint32_t BaudRate, uint16_t STOPBIT, uint16_t PARITY);

extern uint32_t 							Bluetooth_Baudrate;
extern uint16_t 							Bluetooth_StopBit ;
extern uint16_t 							Bluetooth_Parity 	;
extern Bluetooth_DEVICE 			Bluetooth_DeviceList[Bluetooth_DeviceMax];
extern Bluetooth_ATMODE  			Bluetooth_ATMode;
extern Bluetooth_CONNECTMODE 	Bluetooth_ConnectMode;
extern Bluetooth_ROLE				 	Bluetooth_Role;
