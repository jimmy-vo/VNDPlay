#include "Bluetooth_ATCMD.h"
#include "Bluetooth_Layout.h"
#include "Bluetooth.h"
#include "Timing_Manage.h"
#include "Dev_USART.h"
#include "TFTService_Dialog.h"
#include "Bluetooth_Event.h"

static char * Bluetooth_AT_BaudStr[7] = 
{
	"9600,",
	"19200,",
	"38400,",
	"57600,",	
	"115200,",
	"230400,",
	"460800,",
};

char Bluetooth_ATBuff[TERMINAL_BUFFERSIZE];	
	

static uint32_t Bluetooh_AT_CheckString(char *Buffer, char *String)
{
	uint32_t cnt=0;
	while (*String != '\0')
	{
		if (*Buffer != *String) return 0;
		else
		{
			String++;
			Buffer++;
			cnt++;
		}
	}
	return cnt;
}


static uint32_t Bluetooh_AT_FindString(char *Buffer, char *String, uint16_t size, uint16_t timeout)
{
	uint64_t zero;
	uint32_t length = 0, index=0;
	
	//check for lenght
	while (String[++length] != '\0');
	if (length+1>size) size = length+1;
	
	if (timeout)
	{
		zero = Get_msTick();
		while (!Check_msTick(zero, timeout))
		{
			while (index < size - length)
				{
				if (Bluetooh_AT_CheckString (&Buffer[index++], String))
					return index;
			}
			index = 0;
		}
		return 0;
	}
	else
	{
		while (index < size - length)
		{
				if (Bluetooh_AT_CheckString (&Buffer[index++], String))
					return index;
		}
		return 0;		
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
//// GLOBAL
//
uint8_t Bluetooth_AT_SetupUART(uint32_t BAUDRATE, uint16_t STOPBIT, uint16_t PARITY)
{
	char *baudrate, Stopbit, parity, pos=0;	
		
	switch (BAUDRATE)
	{
		case 9600				: ////////////////////////////////////////////////
										baudrate = 	Bluetooth_AT_BaudStr[0];
										break;
		case 19200				: ////////////////////////////////////////////////
										baudrate = 	Bluetooth_AT_BaudStr[1];
										break;
		case 38400			: ////////////////////////////////////////////////
										baudrate = 	Bluetooth_AT_BaudStr[2];
										break;
		case 57600			: ////////////////////////////////////////////////
										baudrate = 	Bluetooth_AT_BaudStr[3];
										break;
		case 115200			: ////////////////////////////////////////////////
										baudrate = 	Bluetooth_AT_BaudStr[4];
										break;
		case 230400			: ////////////////////////////////////////////////
										baudrate = 	Bluetooth_AT_BaudStr[5];
										break;
		case 460800			: ////////////////////////////////////////////////
										baudrate = 	Bluetooth_AT_BaudStr[6];
										break;
		default         : ////////////////////////////////////////////////
										baudrate = 	Bluetooth_AT_BaudStr[0];
										break;
	}	
	
	switch (STOPBIT)
	{
		case USART_StopBits_2	: Stopbit = '1'; break;
		default								: Stopbit = '0'; break;
	}	
	switch (PARITY)
	{
		case USART_Parity_Odd		: parity = '1'; break;
		case USART_Parity_Even	: parity = '2'; break;
		default									: parity = '0'; break;
	}
	/////////////////////////////////////////////////////////////////////////
	
	do 		Bluetooth_ATBuff[pos++] = *(baudrate);
	while (*baudrate++ != ',');	
	Bluetooth_ATBuff[pos++] = Stopbit;
	Bluetooth_ATBuff[pos++] = ',';	
	Bluetooth_ATBuff[pos++] = parity;	
	Bluetooth_ATBuff[pos++] = '\0';
	
	/////////////////////////////////////////////////////////////////////////
	Bluetooth_PrepareRX(&Bluetooth_ATBuff[pos], TERMINAL_BUFFERSIZE - pos);
	Bluetooth_SendString("AT+UART=");
	Bluetooth_SendString(Bluetooth_ATBuff);
	Bluetooth_SendString("\r\n");
	
	if 			(Bluetooh_AT_FindString(&Bluetooth_ATBuff[pos], "OK\r\n",0,500))				return Bluetooth_ATConfirm(1, 0);
	else if (Bluetooth_ATBuff[pos] == 0)																						return Bluetooth_ATConfirm(0, "  No response");
	else if (Bluetooh_AT_FindString(&Bluetooth_ATBuff[pos], "ERROR:(0)\r\n",0,0))		return Bluetooth_ATConfirm(0, "  AT command error");
	else if (Bluetooh_AT_FindString(&Bluetooth_ATBuff[pos], "ERROR:(12)\r\n",0,0))	return Bluetooth_ATConfirm(0, "  Invalid baud rate");
	else if (Bluetooh_AT_FindString(&Bluetooth_ATBuff[pos], "ERROR:(13)\r\n",0,0))	return Bluetooth_ATConfirm(0, "  Invalid stop bit");
	else if (Bluetooh_AT_FindString(&Bluetooth_ATBuff[pos], "ERROR:(14)\r\n",0,0))	return Bluetooth_ATConfirm(0, "  Invalid parity bit");
	else 																																						return Bluetooth_ATConfirm(0, &Bluetooth_ATBuff[pos]);
		
}


char* Bluetooth_AT_GetInfo(void)
{
	uint16_t cnt = 0, pos = 0;	
					
	Bluetooth_PrepareRX(Bluetooth_ATBuff,  TERMINAL_BUFFERSIZE);
		
	Bluetooth_SendString("AT+VERSION?\r\n"); 		 	Bluetooh_AT_FindString(Bluetooth_ATBuff,"VERSION",TERMINAL_BUFFERSIZE, 80);
	Bluetooth_SendString("AT+ADDR?\r\n");					Bluetooh_AT_FindString(Bluetooth_ATBuff,"ADDR",TERMINAL_BUFFERSIZE, 80);
	Bluetooth_SendString("AT+CLASS?\r\n");				Bluetooh_AT_FindString(Bluetooth_ATBuff,"CLASS",TERMINAL_BUFFERSIZE, 80);
	Bluetooth_SendString("AT+ROLE?\r\n");					Bluetooh_AT_FindString(Bluetooth_ATBuff,"ROLE",TERMINAL_BUFFERSIZE, 80);
	Bluetooth_SendString("AT+NAME?\r\n");					Bluetooh_AT_FindString(Bluetooth_ATBuff,"NAME",TERMINAL_BUFFERSIZE, 80);
	Bluetooth_SendString("AT+PSWD?\r\n");					Bluetooh_AT_FindString(Bluetooth_ATBuff,"PSWD",TERMINAL_BUFFERSIZE, 80);
	Bluetooth_SendString("AT+STATE?\r\n");				Bluetooh_AT_FindString(Bluetooth_ATBuff,"STATE",TERMINAL_BUFFERSIZE, 80);
	Bluetooth_SendString("AT+UART?\r\n");					Bluetooh_AT_FindString(Bluetooth_ATBuff,"UART",TERMINAL_BUFFERSIZE, 80);
	Bluetooth_SendString("AT+INQM?\r\n");					Bluetooh_AT_FindString(Bluetooth_ATBuff,"INQM",TERMINAL_BUFFERSIZE, 80);
	Bluetooth_SendString("AT+CMODE?\r\n");				Bluetooh_AT_FindString(Bluetooth_ATBuff,"CMODE",TERMINAL_BUFFERSIZE, 80);
	
//	Bluetooth_SendString("AT+RMAAD\r\n"); 		 		Bluetooh_AT_FindString(Bluetooth_ATBuff,"RMAAD",TERMINAL_BUFFERSIZE, 80);
	
	for (cnt = 0; cnt < TERMINAL_BUFFERSIZE; cnt++)
	{			
		cnt+=Bluetooh_AT_CheckString(&Bluetooth_ATBuff[cnt], "OK\r\n");
		cnt+=Bluetooh_AT_CheckString(&Bluetooth_ATBuff[cnt], "\r");
		Bluetooth_ATBuff[pos++] = Bluetooth_ATBuff[cnt];
	}
	Bluetooth_ATBuff[pos] = '\0'; 
	
	return Bluetooth_ATBuff;
}

uint8_t Bluetooth_AT_SetName(char *Name)
{
	char pos=0;	
	
	
	do Bluetooth_ATBuff[pos++] = *(Name);
	while (*(Name++) != '\0');	
	
	Bluetooth_PrepareRX(&Bluetooth_ATBuff[pos], TERMINAL_BUFFERSIZE-pos);
	Bluetooth_SendString("AT+NAME=");
	Bluetooth_SendString(Bluetooth_ATBuff);
	Bluetooth_SendString("\r\n");
	
	if (Bluetooh_AT_FindString(&Bluetooth_ATBuff[pos], "OK\r\n",0,500))							return Bluetooth_ATConfirm(1, 0);
	else if (Bluetooth_ATBuff[pos] == 0)																						return Bluetooth_ATConfirm(0, "  No response");
	else if (Bluetooh_AT_FindString(&Bluetooth_ATBuff[pos], "ERROR:(0)\r\n",0,0))		return Bluetooth_ATConfirm(0, "  AT command error");
	else if (Bluetooh_AT_FindString(&Bluetooth_ATBuff[pos], "ERROR:(3)\r\n",0,0))		return Bluetooth_ATConfirm(0, "  Too long length of device name (more than 32 bytes)");
	else if (Bluetooh_AT_FindString(&Bluetooth_ATBuff[pos], "ERROR:(4)\r\n",0,0))		return Bluetooth_ATConfirm(0, "  No device name");
	else 																																						return Bluetooth_ATConfirm(0, &Bluetooth_ATBuff[pos]);
}


uint8_t Bluetooth_AT_SetPasskey(char *Passkey)
{
	char pos=0;	
	
	do Bluetooth_ATBuff[pos++] = *(Passkey);
	while (*(Passkey++) != '\0');	
	
	Bluetooth_PrepareRX(&Bluetooth_ATBuff[pos], TERMINAL_BUFFERSIZE-pos);
	Bluetooth_SendString("AT+PSWD=");
	Bluetooth_SendString(Bluetooth_ATBuff);
	Bluetooth_SendString("\r\n");
	
	if (Bluetooh_AT_FindString(&Bluetooth_ATBuff[pos], "OK\r\n",0,500))							return Bluetooth_ATConfirm(1, 0);
	else if (Bluetooth_ATBuff[pos] == 0)																						return Bluetooth_ATConfirm(0, "  No response");
	else if (Bluetooh_AT_FindString(&Bluetooth_ATBuff[pos], "ERROR:(0)\r\n",0,0))		return Bluetooth_ATConfirm(0, "  AT command error");
	else if (Bluetooh_AT_FindString(&Bluetooth_ATBuff[pos], "ERROR:(2)\r\n",0,0))		return Bluetooth_ATConfirm(0, "  Passkey write error");
	else if (Bluetooh_AT_FindString(&Bluetooth_ATBuff[pos], "ERROR:(F)\r\n",0,0))		return Bluetooth_ATConfirm(0, "  The length of Passkey is 0.");
	else if (Bluetooh_AT_FindString(&Bluetooth_ATBuff[pos], "ERROR:(10)\r\n",0,0))	return Bluetooth_ATConfirm(0, "  Too long length of passkey (more than 16 bytes)");
	else 																																						return Bluetooth_ATConfirm(0, &Bluetooth_ATBuff[pos]);
}

uint8_t Bluetooth_AT_SetRole(uint8_t Role)
{
	Bluetooth_PrepareRX(Bluetooth_ATBuff, TERMINAL_BUFFERSIZE);
	
	if (Role==0)
		Bluetooth_SendString("AT+ROLE=0\r\n");
	else if (Role==1)
		Bluetooth_SendString("AT+ROLE=1\r\n");
	else if (Role==2)
		Bluetooth_SendString("AT+ROLE=2\r\n");
		
	if 			(Bluetooh_AT_FindString(Bluetooth_ATBuff, "OK\r\n",0,500))				return Bluetooth_ATConfirm(1, 0);
	else if (*Bluetooth_ATBuff == 0)																					return Bluetooth_ATConfirm(0, "  No response");
	else if (Bluetooh_AT_FindString(Bluetooth_ATBuff, "ERROR:(0)\r\n",0,0))		return Bluetooth_ATConfirm(0, "  AT command error");
	else if (Bluetooh_AT_FindString(Bluetooth_ATBuff, "ERROR:(11)\r\n",0,0))	return Bluetooth_ATConfirm(0, "  Invalid module role");
	else 																																			return Bluetooth_ATConfirm(0, Bluetooth_ATBuff);
}

uint8_t Bluetooth_AT_SetMode(uint8_t Mode)
{
	Bluetooth_PrepareRX(Bluetooth_ATBuff, TERMINAL_BUFFERSIZE);
	
	if (Mode==0)
		Bluetooth_SendString("AT+CMODE=0\r\n");
	else if (Mode==1)
		Bluetooth_SendString("AT+CMODE=1\r\n");
	else if (Mode==2)
		Bluetooth_SendString("AT+CMODE=2\r\n");
		
	if 			(Bluetooh_AT_FindString(Bluetooth_ATBuff, "OK\r\n",0,500))				return Bluetooth_ATConfirm(1, 0);
	else if (*Bluetooth_ATBuff == 0)																					return Bluetooth_ATConfirm(0, "  No response");
	else if (Bluetooh_AT_FindString(Bluetooth_ATBuff, "ERROR:(0)\r\n",0,0))		return Bluetooth_ATConfirm(0, "  AT command error");
	else 																																			return Bluetooth_ATConfirm(0, Bluetooth_ATBuff);
}

uint8_t Bluetooth_AT_InitSPP(void)
{
	Bluetooth_PrepareRX(Bluetooth_ATBuff, TERMINAL_BUFFERSIZE);
	
	Bluetooth_SendString("AT+INIT\r\n");
	
	if 			(Bluetooh_AT_FindString(Bluetooth_ATBuff, "OK\r\n",0,500))					return Bluetooth_ATConfirm(1, 0);
	else if (Bluetooh_AT_FindString(Bluetooth_ATBuff, "ERROR:(17)\r\n",0,0))		return Bluetooth_ATConfirm(1, 0);
	else if (*Bluetooth_ATBuff == 0)																						return Bluetooth_ATConfirm(0, "  No response");
	else if (Bluetooh_AT_FindString(Bluetooth_ATBuff, "ERROR:(0)\r\n",0,0))			return Bluetooth_ATConfirm(0, "  AT command error");
	else 																																				return Bluetooth_ATConfirm(0, Bluetooth_ATBuff);
}


uint8_t Bluetooth_AT_CancelInquire(void)
{
	Bluetooth_PrepareRX(Bluetooth_ATBuff, TERMINAL_BUFFERSIZE);
	
	Bluetooth_SendString("AT+INQC\r\n");
	
	if 			(Bluetooh_AT_FindString(Bluetooth_ATBuff, "OK\r\n",0,500))					return Bluetooth_ATConfirm(1, 0);
	else if (*Bluetooth_ATBuff == 0)																						return Bluetooth_ATConfirm(0, "  No response");
	else if (Bluetooh_AT_FindString(Bluetooth_ATBuff, "ERROR:(0)\r\n",0,0))			return Bluetooth_ATConfirm(0, "  AT command error");
	else 																																				return Bluetooth_ATConfirm(0, Bluetooth_ATBuff);
}

uint8_t Bluetooth_AT_SetInquireMode(uint8_t Mode, uint8_t MaxDevice,   uint8_t second)
{
	uint8_t pos = 0;
	
	// MODE
	if (Mode) 	Bluetooth_ATBuff[pos++] = '1';
	else				Bluetooth_ATBuff[pos++] = '0';
	
	Bluetooth_ATBuff[pos++] = ',';
	
	// Num of Device
	if (MaxDevice<=9)	
	{
		if (MaxDevice<1)	Bluetooth_ATBuff[pos++] = '1';
		else 							Bluetooth_ATBuff[pos++] = MaxDevice+48;
	}
	else
	{
		if (MaxDevice>=99)
		{
			Bluetooth_ATBuff[pos++] = '9';
			Bluetooth_ATBuff[pos++] = '9';
		}
		else
		{			
			Bluetooth_ATBuff[pos++] = MaxDevice/9 + 48;
			Bluetooth_ATBuff[pos++] = MaxDevice%9 + 48;
		}
	}
	
	Bluetooth_ATBuff[pos++] = ',';
	
	// timeout
	second = second/1.28;
	if (second>48) 				second = 48;
	else if (second<=0)		second = 1;
	
	if (second<=9)	
	{
		if (second<1)	Bluetooth_ATBuff[pos++] = '1';
		else 						Bluetooth_ATBuff[pos++] = second+48;
	}
	else
	{
		if (second>48)
		{
			Bluetooth_ATBuff[pos++] = '4';
			Bluetooth_ATBuff[pos++] = '8';
		}
		else
		{			
			Bluetooth_ATBuff[pos++] = MaxDevice/9 + 48;
			Bluetooth_ATBuff[pos++] = MaxDevice%9 + 48;
		}
	}	
	Bluetooth_ATBuff[pos++] = '\0';
	
	
	// send and check	
	Bluetooth_PrepareRX(&Bluetooth_ATBuff[pos],  TERMINAL_BUFFERSIZE-pos);	
	Bluetooth_SendString("AT+INQM=");
	Bluetooth_SendString(Bluetooth_ATBuff);
	Bluetooth_SendString("\r\n");
	
	if 			(Bluetooh_AT_FindString(&Bluetooth_ATBuff[pos], "OK\r\n",0,500))				return Bluetooth_ATConfirm(1, 0);
	else if (Bluetooth_ATBuff[pos] == 0)																						return Bluetooth_ATConfirm(0, "  No response");
	else if (Bluetooh_AT_FindString(&Bluetooth_ATBuff[pos], "ERROR:(0)\r\n",0,0))		return Bluetooth_ATConfirm(0, "  AT command error");
	else if (Bluetooh_AT_FindString(&Bluetooth_ATBuff[pos], "ERROR:(18)\r\n",0,0))	return Bluetooth_ATConfirm(0, "  Invalid inquire mode");
	else if (Bluetooh_AT_FindString(&Bluetooth_ATBuff[pos], "ERROR:(B)\r\n",0,0))		return Bluetooth_ATConfirm(0, "  Too length of devices");
	else if (Bluetooh_AT_FindString(&Bluetooth_ATBuff[pos], "ERROR:(19)\r\n",0,0))	return Bluetooth_ATConfirm(0, "  Too long inquire time");
	else 																																						return Bluetooth_ATConfirm(0, &Bluetooth_ATBuff[pos]);
}

uint8_t Bluetooth_AT_Inquire(uint8_t second)
{
	uint16_t cnt=0, addr = 0, device = 0, size, offset;
	
	Bluetooth_PrepareRX(Bluetooth_ATBuff, TERMINAL_BUFFERSIZE);
	
	Bluetooth_SendString("AT+INQ\r\n");	
	
	//reset device list
	for (device = 0; device < Bluetooth_DeviceMax; device++)
		for (addr = 0; addr < Bluetooth_AddrSize; addr++)		
			Bluetooth_DeviceList[device].State = Bluetooth_DeviceNone;
	
	device = addr = 0;
	
	//check input
	if (Bluetooh_AT_FindString(Bluetooth_ATBuff, "OK\r\n",0,(second+1)*1000))						return Bluetooth_ATConfirm(0, "  No device found");	 
	else
	{
		size = Bluetooh_AT_FindString(Bluetooth_ATBuff, "OK\r\n",TERMINAL_BUFFERSIZE,0);	
		if (size !=0 ) 
		{	
			while ((cnt <size) && (device< Bluetooth_DeviceMax))
			{
				offset = 0;
				while ((offset == 0) 		&& (cnt <size))		
					offset = Bluetooh_AT_CheckString(&Bluetooth_ATBuff[cnt++], "+INQ:");
				
				if (offset)
				{
					cnt += offset -1 ;
					Bluetooth_DeviceList[device].State = Bluetooth_DeviceAvailable;
					while ((!Bluetooh_AT_CheckString(&Bluetooth_ATBuff[cnt], ",")) && (cnt <size))
					{
						if (addr < Bluetooth_AddrSize)
						{
							if(Bluetooth_ATBuff[cnt] == ':')		
								Bluetooth_DeviceList[device].Address[addr++] = ',';	
							else if (Bluetooth_ATBuff[cnt] != '\0')
								Bluetooth_DeviceList[device].Address[addr++] = Bluetooth_ATBuff[cnt];		
							else					
								Bluetooth_DeviceList[device].Address[addr++] =  '\0';				
						}
						cnt++;
					}
					device ++;
					addr = 0;
				}
			}
			return Bluetooth_ATConfirm(1, 0);
		} 	
		else if (Bluetooh_AT_FindString(Bluetooth_ATBuff, "ERROR:(0)\r\n",0,0))			return Bluetooth_ATConfirm(0, "  AT command error");	
		else if (*Bluetooth_ATBuff == 0)																						return Bluetooth_ATConfirm(0, "  No response");
		else 																																				return Bluetooth_ATConfirm(0, Bluetooth_ATBuff);
	}
}

uint8_t Bluetooth_AT_Scan(uint8_t timeout)
{	
		if (Bluetooth_AT_InitSPP()) 																			// Init SPP			
			if (Bluetooth_AT_SetInquireMode(0,Bluetooth_DeviceMax,timeout)) // Init Inquire mode
				return (Bluetooth_AT_Inquire(timeout));														// Search device
				
	return (Bluetooth_ATConfirm(0,"  Scan fail"));														// Search fail
}

uint8_t Bluetooth_AT_GetDeviceName(uint8_t device)
{	
	uint8_t cnt = 0, pos = 0;
	
	if (Bluetooth_DeviceList[device].State)
	{		
		Bluetooth_PrepareRX(Bluetooth_ATBuff, TERMINAL_BUFFERSIZE);
		
		Bluetooth_SendString("AT+RNAME?");
		Bluetooth_SendString(Bluetooth_DeviceList[device].Address);
		Bluetooth_SendString("\r\n");
				
		if (Bluetooh_AT_FindString(Bluetooth_ATBuff, "OK\r\n",TERMINAL_BUFFERSIZE,4000))
		{
			while (pos < Bluetooth_NameSize-1)
			{
				cnt+=Bluetooh_AT_CheckString(&Bluetooth_ATBuff[cnt], "+RNAME:");
				cnt+=Bluetooh_AT_CheckString(&Bluetooth_ATBuff[cnt], "\r\n");
				cnt+=Bluetooh_AT_CheckString(&Bluetooth_ATBuff[cnt], "OK\r\n");
				Bluetooth_DeviceList[device].Name[pos++] = Bluetooth_ATBuff[cnt++];
			}
			
			if (pos)
			{
				Bluetooth_DeviceList[device].Name[pos] = '\0';
				return	Bluetooth_ATConfirm(1, 0);	
			}
		}
		else
		{
			for (cnt = 0; cnt < Bluetooth_AddrSize; cnt++)
				Bluetooth_DeviceList[device].Name[cnt] = Bluetooth_DeviceList[device].Address[cnt++];
			Bluetooth_DeviceList[device].Name[cnt] = '\0';
			
			
			if (Bluetooh_AT_FindString(Bluetooth_ATBuff, "ERROR:(1A)\r\n",0,0))							return	Bluetooth_ATConfirm(1, 0);	
			else if 			(Bluetooh_AT_FindString(Bluetooth_ATBuff, "ERROR:(0)\r\n",0,0))		return	Bluetooth_ATConfirm(0, "  AT command error");	
			else if (*Bluetooth_ATBuff == 0)																								return 	Bluetooth_ATConfirm(0, "  No response");
			else if (Bluetooh_AT_FindString(Bluetooth_ATBuff, "ERROR:(5)\r\n",0,0))						Bluetooth_ATConfirm(1, "  Bluetooth address: NAP is too long.");	
			else if (Bluetooh_AT_FindString(Bluetooth_ATBuff, "ERROR:(6)\r\n",0,0))						Bluetooth_ATConfirm(1, "  Bluetooth address: UAP is too long.");	
			else if (Bluetooh_AT_FindString(Bluetooth_ATBuff, "ERROR:(7)\r\n",0,0))						Bluetooth_ATConfirm(1, "  Bluetooth address: LAP is too long.");	
			else 																																						return	Bluetooth_ATConfirm(0, Bluetooth_ATBuff);	
		}
	}
	return	Bluetooth_ATConfirm(1, 0);	
}

uint8_t Bluetooth_AT_DeviceCheckAuthenticated(uint8_t device)
{		
	if (Bluetooth_DeviceList[device].State)
	{		
		Bluetooth_PrepareRX(Bluetooth_ATBuff, TERMINAL_BUFFERSIZE);
		
		Bluetooth_SendString("AT+FSAD=");
		Bluetooth_SendString(Bluetooth_DeviceList[device].Address);
		Bluetooth_SendString("\r\n");
				
		if 			(Bluetooh_AT_FindString(Bluetooth_ATBuff, "OK\r\n",0,4000))				return	Bluetooth_ATConfirm(++Bluetooth_DeviceList[device].State, 0);	
		else if (Bluetooh_AT_FindString(Bluetooth_ATBuff, "FAIL\r\n",0,0))				return	Bluetooth_ATConfirm(1, 0);	
		else if (*Bluetooth_ATBuff == 0)																					return 	Bluetooth_ATConfirm(0, "  No response");
		else if (Bluetooh_AT_FindString(Bluetooth_ATBuff, "ERROR:(0)\r\n",0,0))		return	Bluetooth_ATConfirm(0, "  AT command error");	
		else if (Bluetooh_AT_FindString(Bluetooth_ATBuff, "ERROR:(5)\r\n",0,0))			Bluetooth_ATConfirm(0, "  Bluetooth address: NAP is too long.");	
		else if (Bluetooh_AT_FindString(Bluetooth_ATBuff, "ERROR:(6)\r\n",0,0))			Bluetooth_ATConfirm(0, "  Bluetooth address: UAP is too long.");	
		else if (Bluetooh_AT_FindString(Bluetooth_ATBuff, "ERROR:(7)\r\n",0,0))			Bluetooth_ATConfirm(0, "  Bluetooth address: LAP is too long.");	
		else 																																			return	Bluetooth_ATConfirm(0, Bluetooth_ATBuff);	
	}
	return	Bluetooth_ATConfirm(0,  "  Not in range");	
}


uint8_t Bluetooth_AT_DevicePair(uint8_t device, uint8_t timeout)
{		
	uint8_t pos =0;
	
	Bluetooth_ATBuff[pos++] = ',';
	if (timeout>99) 				timeout = 99;
	else if (timeout<=0)		timeout = 1;

	if (timeout<=9)	Bluetooth_ATBuff[pos++] = timeout+48;
	else
	{		
		Bluetooth_ATBuff[pos++] = timeout/9 + 48;
		Bluetooth_ATBuff[pos++] = timeout%9 + 48;
	}	
	Bluetooth_ATBuff[pos++] = '\0';
	
	Bluetooth_PrepareRX(&Bluetooth_ATBuff[pos], TERMINAL_BUFFERSIZE - pos);
	Bluetooth_SendString("AT+PAIR=");
	Bluetooth_SendString(Bluetooth_DeviceList[device].Address);
	Bluetooth_SendString(Bluetooth_ATBuff);
	Bluetooth_SendString("\r\n");
			
	if 			(Bluetooh_AT_FindString(&Bluetooth_ATBuff[pos], "OK\r\n",0,(timeout+1)*1000))				return	Bluetooth_ATConfirm(1, "  Pair succeed!");	
	else if (*Bluetooth_ATBuff == 0)																														return 	Bluetooth_ATConfirm(0, "  No response");
	else if (Bluetooh_AT_FindString(&Bluetooth_ATBuff[pos], "FAIL\r\n",0,0))										return	Bluetooth_ATConfirm(0, "  Pair timeout!");	
	else if (Bluetooh_AT_FindString(&Bluetooth_ATBuff[pos], "ERROR:(0)\r\n",0,0))								return	Bluetooth_ATConfirm(0, "  AT command error");	
	else if (Bluetooh_AT_FindString(&Bluetooth_ATBuff[pos], "ERROR:(5)\r\n",0,0))								Bluetooth_ATConfirm(0, "  Bluetooth address: NAP is too long.");	
	else if (Bluetooh_AT_FindString(&Bluetooth_ATBuff[pos], "ERROR:(6)\r\n",0,0))								Bluetooth_ATConfirm(0, "  Bluetooth address: UAP is too long.");	
	else if (Bluetooh_AT_FindString(&Bluetooth_ATBuff[pos], "ERROR:(7)\r\n",0,0))								Bluetooth_ATConfirm(0, "  Bluetooth address: LAP is too long.");	
																																															return	Bluetooth_ATConfirm(0, Bluetooth_ATBuff);	
}

uint8_t Bluetooth_AT_DeviceBind(uint8_t device)
{		
	Bluetooth_PrepareRX(Bluetooth_ATBuff, TERMINAL_BUFFERSIZE);
	
	Bluetooth_SendString("AT+BIND=");
	Bluetooth_SendString(Bluetooth_DeviceList[device].Address);
	Bluetooth_SendString("\r\n");
		
	if 			(Bluetooh_AT_FindString(Bluetooth_ATBuff, "OK\r\n",0,4000))				return	Bluetooth_ATConfirm(1, 0);	
	else if (*Bluetooth_ATBuff == 0)																					return 	Bluetooth_ATConfirm(0, "  No response");
	else if (Bluetooh_AT_FindString(Bluetooth_ATBuff, "ERROR:(0)\r\n",0,0))		return	Bluetooth_ATConfirm(0, "  AT command error");	
	else if (Bluetooh_AT_FindString(Bluetooth_ATBuff, "FAIL\r\n",0,0))				return	Bluetooth_ATConfirm(0, "  AT bind Fail");	
	else if (Bluetooh_AT_FindString(Bluetooth_ATBuff, "ERROR:(5)\r\n",0,0))			Bluetooth_ATConfirm(0, "  Bluetooth address: NAP is too long.");	
	else if (Bluetooh_AT_FindString(Bluetooth_ATBuff, "ERROR:(6)\r\n",0,0))			Bluetooth_ATConfirm(0, "  Bluetooth address: UAP is too long.");	
	else if (Bluetooh_AT_FindString(Bluetooth_ATBuff, "ERROR:(7)\r\n",0,0))			Bluetooth_ATConfirm(0, "  Bluetooth address: LAP is too long.");	
																																					return	Bluetooth_ATConfirm(0, Bluetooth_ATBuff);	
}

uint8_t Bluetooth_AT_DeviceLink(uint8_t device)
{		
	Bluetooth_PrepareRX(Bluetooth_ATBuff, TERMINAL_BUFFERSIZE);
	
	Bluetooth_SendString("AT+LINK=");
	Bluetooth_SendString(Bluetooth_DeviceList[device].Address);
	Bluetooth_SendString("\r\n");
		
	if 			(Bluetooh_AT_FindString(Bluetooth_ATBuff, "OK\r\n",0,4000))				return	Bluetooth_ATConfirm(1, "  Connected!");	
	else if (Bluetooh_AT_FindString(Bluetooth_ATBuff, "ERROR:(0)\r\n",0,0))		return	Bluetooth_ATConfirm(0, "  AT command error");	
	else if (*Bluetooth_ATBuff == 0)																					return 	Bluetooth_ATConfirm(0, "  No response");
	else if (Bluetooh_AT_FindString(Bluetooth_ATBuff, "FAIL\r\n",0,0))				return	Bluetooth_ATConfirm(0, "  AT Link Fail");	
	else if (Bluetooh_AT_FindString(Bluetooth_ATBuff, "ERROR:(5)\r\n",0,0))			Bluetooth_ATConfirm(0, "  Bluetooth address: NAP is too long.");	
	else if (Bluetooh_AT_FindString(Bluetooth_ATBuff, "ERROR:(6)\r\n",0,0))			Bluetooth_ATConfirm(0, "  Bluetooth address: UAP is too long.");	
	else if (Bluetooh_AT_FindString(Bluetooth_ATBuff, "ERROR:(7)\r\n",0,0))			Bluetooth_ATConfirm(0, "  Bluetooth address: LAP is too long.");	
	 																																			return	Bluetooth_ATConfirm(0, Bluetooth_ATBuff);	
}
