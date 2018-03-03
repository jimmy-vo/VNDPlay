#include "Bluetooth_Event.h"
#include "Bluetooth.h"
#include "Bluetooth_Layout.h"
#include "TFTService_Dialog.h"
#include <stdio.h>
#include <stdlib.h>
#include "Input.h"
#include "TFTService_Draw.h"
#include "TFT_SSD1289.h"
#include "Bluetooth_ATCMD.h"
#include "Timing_Manage.h"
#include "TFTService_Keyboard.h"

uint8_t Bluetooth_ATConfirm(uint8_t RETURN, char* string)
{
	if (string) 
	{
		TFTSevice_ShowDialogOK(
														BTH_ATConfirm_POSX, BTH_ATConfirm_POSY, 
														BTH_ATConfirm_SIZEX, BTH_ATConfirm_SIZEY, 
														"  AT Command",
														string
													);
		Bluetooth_Render_Main();
	}
	return RETURN;
}

static uint8_t Bluetooth_Event_Connect (uint8_t cnt)
{	
	char *String;		
	if (Bluetooth_AT_DeviceCheckAuthenticated(cnt) == 1)
	{
		if (TFTSevice_ShowDialogYesNo(
																						BTH_ATConfirm_POSX, BTH_ATConfirm_POSY, 
																						BTH_ATConfirm_SIZEX, BTH_ATConfirm_SIZEY, 
																						Bluetooth_DeviceList[cnt].Name,
																						" Do you want to pair this device?")
																	)
						{
							if	(TFTService_Keyboard ("Bluetooth Passkey", &String))
							Bluetooth_AT_SetPasskey(String);
							Bluetooth_AT_DevicePair(cnt,15);
							Bluetooth_Render_Main();			
							return 1;
						}
						else 
						{
							Bluetooth_Render_Main();						
							return 0;	
						}
	}			
	else if (Bluetooth_AT_DeviceCheckAuthenticated(cnt) > 1)
	{
		if (TFTSevice_ShowDialogYesNo(
																						BTH_ATConfirm_POSX, BTH_ATConfirm_POSY, 
																						BTH_ATConfirm_SIZEX, BTH_ATConfirm_SIZEY, 
																						Bluetooth_DeviceList[cnt].Name,
																						" Do you want to connect this device?")
																					)
						{
							if (Bluetooth_AT_DeviceBind(cnt))
									Bluetooth_AT_DeviceLink(cnt);		
							Bluetooth_Render_Main();					
							return 1;
						}
						else 
						{
							Bluetooth_Render_Main();	
							return 0;	
						}
	}		
		
	return 0;	
}
//////////////////////////////////////////////////////
// SETTING window
static uint8_t Bluetooth_Event_SETTINGButton (BTH_UART cnt)
{
	char *String;	
	switch (cnt)
	{
		case BTH_SET_9600						: ////////////////////////////////////////////////
																	Bluetooth_Baudrate = 9600;
																	return 0;
		case BTH_SET_19200						: ////////////////////////////////////////////////
																	Bluetooth_Baudrate = 19200;
																	return 0;
		case BTH_SET_38400						: ////////////////////////////////////////////////
																	Bluetooth_Baudrate = 38400;
																	return 0;
		case BTH_SET_57600						: ////////////////////////////////////////////////
																	Bluetooth_Baudrate = 57600;
																	return 0;
		case BTH_SET_115200					: ////////////////////////////////////////////////
																	Bluetooth_Baudrate = 115200;
																	return 0;
		case BTH_SET_230400					: ////////////////////////////////////////////////
																	Bluetooth_Baudrate = 230400;
																	return 0;
		case BTH_SET_460800					: ////////////////////////////////////////////////
																	Bluetooth_Baudrate = 460800;
																	return 0;
		case BTH_SET_STOP1						: ////////////////////////////////////////////////
																	Bluetooth_StopBit	=	USART_StopBits_1;
																	return 0;
		case BTH_SET_STOP2						: ////////////////////////////////////////////////
																	Bluetooth_StopBit	=	USART_StopBits_2;
																	return 0;
		case BTH_SET_PARITYNone			: ////////////////////////////////////////////////
																	Bluetooth_Parity = USART_Parity_No;
																	return 0;
		case BTH_SET_PARITYOdd				: ////////////////////////////////////////////////
																	Bluetooth_Parity = USART_Parity_Odd;
																	return 0;
		case BTH_SET_PARITYEven			: ////////////////////////////////////////////////
																	Bluetooth_Parity = USART_Parity_Even;
																	return 0;
		case BTH_SET_ROLE_Slave			: ////////////////////////////////////////////////
																	Bluetooth_Role = Bluetooth_ROLE_Slave;
																	return 0;
		case BTH_SET_ROLE_Master			: ////////////////////////////////////////////////
																	Bluetooth_Role = Bluetooth_ROLE_Master;
																	return 0;
		case BTH_SET_ROLE_Slaveloop	: ////////////////////////////////////////////////
																	Bluetooth_Role = Bluetooth_ROLE_Slaveloop;
																	return 0;
		case BTH_SET_CMODE_Manual		: ////////////////////////////////////////////////
																	Bluetooth_ConnectMode = Bluetooth_CMODE_Manual;
																	return 0;
		case BTH_SET_CMODE_Auto			: ////////////////////////////////////////////////
																	Bluetooth_ConnectMode = Bluetooth_CMODE_Auto;
																	return 0;
		case BTH_SET_CMODE_Slaveloop	: ////////////////////////////////////////////////
																	Bluetooth_ConnectMode = Bluetooth_CMODE_Slaveloop;
																	return 0;
		case BTH_SET_Set_Name				: ////////////////////////////////////////////////
																	if	(TFTService_Keyboard ("Bluetooth Name", &String))
																		 Bluetooth_AT_SetName(String);
																	Bluetooth_Render_Main();
																	Bluetooth_Render_Setting();	
																	return 0;
		case BTH_SET_Set_Pass				: ////////////////////////////////////////////////
																	if	(TFTService_Keyboard ("Bluetooth Passkey", &String))
																		Bluetooth_AT_SetPasskey(String);
																	Bluetooth_Render_Main();
																	Bluetooth_Render_Setting();	
																	return 0;
		case BTH_SET_Ok							: ////////////////////////////////////////////////			
																	if (Bluetooth_AT_SetMode(Bluetooth_ConnectMode))	
																		if (Bluetooth_AT_SetRole(Bluetooth_Role))
																			if (Bluetooth_AT_SetupUART(Bluetooth_Baudrate, Bluetooth_StopBit, Bluetooth_Parity))
																				return 1;
																	Bluetooth_Render_Setting();																
																	return 0;
		case BTH_SET_Cancel					: ////////////////////////////////////////////////
																	return 0;
		default 											:	////////////////////////////////////////////////
																	return 1;										
	}
}


static uint8_t Bluetooth_Event_SETTINGProcess (void)
{	
	BTH_UART cnt;
	static int16_t x = 0, y = 0;
	
	while(1)
	{
		TouchFinger Event = TouchPanel_GetPoint(&x, &y); //TFT_SetPixelSafe (x, y, TFT_BLUE) ;
		
		if (
				x < BTH_UARTREGION_POSX ||
				x > BTH_UARTREGION_POSX + BTH_UARTREGION_SIZEX ||
				y < BTH_UARTREGION_POSY ||
				y > BTH_UARTREGION_POSY + BTH_UARTREGION_SIZEY 
			 )	{if (Event == TouchFinger_Up)Bluetooth_Render_Setting();	 }
		else
		{
			if (x >= Bluetooth_UART[BTH_SET_9600].MinX &&	x <= Bluetooth_UART[BTH_SET_9600].MaxX && y >= Bluetooth_UART[BTH_SET_9600].MinY && y <= Bluetooth_UART[BTH_SET_460800].MaxY)
			{
				switch (Event)
				{
					case TouchFinger_None	:	break;
					case TouchFinger_Up		:	break;				
					default:						
									for(cnt = BTH_SET_9600; cnt <= BTH_SET_460800; cnt++)
										if (TFTSevice_RegionEvent(&Bluetooth_UART[cnt], x, y, Event))
										{
											TFTSevice_Button_08x16(&Bluetooth_UART[cnt]);		
											if (Bluetooth_UART[cnt].IsPressed) Bluetooth_Event_SETTINGButton(cnt);
										}
									break;
				}
			}	
			else if (x >= Bluetooth_UART[BTH_SET_STOP1].MinX &&	x <= Bluetooth_UART[BTH_SET_STOP1].MaxX && y >= Bluetooth_UART[BTH_SET_STOP1].MinY && y <= Bluetooth_UART[BTH_SET_STOP2].MaxY)
			{
				switch (Event)
				{
					case TouchFinger_None	:	break;
					case TouchFinger_Up		:	break;				
					default:					
									for(cnt = BTH_SET_STOP1; cnt <= BTH_SET_STOP2; cnt++)
										if (TFTSevice_RegionEvent(&Bluetooth_UART[cnt], x, y, Event))
										{
											TFTSevice_Button_08x16(&Bluetooth_UART[cnt]);		
											if (Bluetooth_UART[cnt].IsPressed) Bluetooth_Event_SETTINGButton(cnt);
										}
									break;
				}
			}	
			else if (x >= Bluetooth_UART[BTH_SET_PARITYNone].MinX &&	x <= Bluetooth_UART[BTH_SET_PARITYNone].MaxX && y >= Bluetooth_UART[BTH_SET_PARITYNone].MinY && y <= Bluetooth_UART[BTH_SET_PARITYEven].MaxY)
			{
				switch (Event)
				{
					case TouchFinger_None	:	break;
					case TouchFinger_Up		:	break;				
					default:					
									for(cnt = BTH_SET_PARITYNone; cnt <= BTH_SET_PARITYEven; cnt++)
										if (TFTSevice_RegionEvent(&Bluetooth_UART[cnt], x, y, Event))
										{
											TFTSevice_Button_08x16(&Bluetooth_UART[cnt]);		
											if (Bluetooth_UART[cnt].IsPressed) Bluetooth_Event_SETTINGButton(cnt);
										}
									break;
				}
			}		
			else if (x >= Bluetooth_UART[BTH_SET_ROLE_Slave].MinX &&	x <= Bluetooth_UART[BTH_SET_ROLE_Slave].MaxX && y >= Bluetooth_UART[BTH_SET_ROLE_Slave].MinY && y <= Bluetooth_UART[BTH_SET_ROLE_Slaveloop].MaxY)
			{
				switch (Event)
				{
					case TouchFinger_None	:	break;
					case TouchFinger_Up		:	break;				
					default:					
									for(cnt = BTH_SET_ROLE_Slave; cnt <= BTH_SET_ROLE_Slaveloop; cnt++)
										if (TFTSevice_RegionEvent(&Bluetooth_UART[cnt], x, y, Event))
										{
											TFTSevice_Button_08x16(&Bluetooth_UART[cnt]);		
											if (Bluetooth_UART[cnt].IsPressed) Bluetooth_Event_SETTINGButton(cnt);
										}
									break;
				}
			}	
			else if (x >= Bluetooth_UART[BTH_SET_CMODE_Manual].MinX &&	x <= Bluetooth_UART[BTH_SET_CMODE_Manual].MaxX && y >= Bluetooth_UART[BTH_SET_CMODE_Manual].MinY && y <= Bluetooth_UART[BTH_SET_CMODE_Slaveloop].MaxY)
			{
				switch (Event)
				{
					case TouchFinger_None	:	break;
					case TouchFinger_Up		:	break;				
					default:					
									for(cnt = BTH_SET_CMODE_Manual; cnt <= BTH_SET_CMODE_Slaveloop; cnt++)
										if (TFTSevice_RegionEvent(&Bluetooth_UART[cnt], x, y, Event))
										{
											TFTSevice_Button_08x16(&Bluetooth_UART[cnt]);		
											if (Bluetooth_UART[cnt].IsPressed) Bluetooth_Event_SETTINGButton(cnt);
										}
									break;
				}
			}		
			else	
			{
				switch (Event)
				{
					case TouchFinger_None	:	break;
					case TouchFinger_Up		:	
									for(cnt = BTH_SET_Ok; cnt <= BTH_SET_Cancel; cnt++)
										if (TFTSevice_RegionEvent(&Bluetooth_UART[cnt], x, y, Event))
										{
											TFTSevice_Button_08x16(&Bluetooth_UART[cnt]);		
											if (Bluetooth_Event_SETTINGButton(cnt))		return 1;
											else if (cnt == BTH_SET_Cancel)  			return 0;
										}
										
									for(cnt = BTH_SET_Set_Name; cnt <= BTH_SET_Set_Pass; cnt++)
										if (TFTSevice_RegionEvent(&Bluetooth_UART[cnt], x, y, Event))
										{
											TFTSevice_Button_08x16(&Bluetooth_UART[cnt]);		
											Bluetooth_Event_SETTINGButton(cnt);
										}
									break;			
					default:					
									for(cnt = BTH_SET_Ok; cnt <= BTH_SET_Cancel; cnt++)
										if (TFTSevice_RegionEvent(&Bluetooth_UART[cnt], x, y, Event)) 	
											TFTSevice_Button_08x16(&Bluetooth_UART[cnt]);		
											
									for(cnt = BTH_SET_Set_Name; cnt <= BTH_SET_Set_Pass; cnt++)
										if (TFTSevice_RegionEvent(&Bluetooth_UART[cnt], x, y, Event))
											TFTSevice_Button_08x16(&Bluetooth_UART[cnt]);		
									break;	
				}
			}
		}
	}
}

//////////////////////////////////////////////////////
// Main window

static void Bluetooth_Event_SwitchPWR (void)
{
	switch (Bluetooth_GetPwrPin())
	{
		case 1:
						if (TFTSevice_ShowDialogYesNo(
																						BTH_SWITCHConfirm_POSX, BTH_SWITCHConfirm_POSY, 
																						BTH_SWITCHConfirm_SIZEX, BTH_SWITCHConfirm_SIZEY, 
																						"  HC05",
																						" Do you want to Turn Bluetooth Off?")
																					)	
						{							
							Bluetooth_SetKeyPin(0);
							Bluetooth_SetPwrPin(!Bluetooth_GetPwrPin()); 
						}
						break;
		case 0:
						if (TFTSevice_ShowDialogYesNo(
																						BTH_SWITCHConfirm_POSX, BTH_SWITCHConfirm_POSY, 
																						BTH_SWITCHConfirm_SIZEX, BTH_SWITCHConfirm_SIZEY, 
																						"  HC05",
																						" Do you want to Turn Bluetooth On?")
																					)																										
						{							
							Bluetooth_SetKeyPin(0);
							Bluetooth_SetPwrPin(!Bluetooth_GetPwrPin()); 
						}
						break;
	}
}



static uint8_t Bluetooth_Event_Button (BTH_BUTTON cnt)
{
	uint8_t device, line = 0 ;
	switch (cnt)
	{
		case BTH_Button_ATMode: ////////////////////////////////////////////////
														Bluetooth_SwitchMode((Bluetooth_ATMODE)(!Bluetooth_ATMode));
														Bluetooth_Render_Status();
														return 1;
		case BTH_Button_Scan	: ////////////////////////////////////////////////
														Bluetooth_Clear_Terminal();
														Bluetooth_Render_Main_Terminal(line++, "Scanning...");		
														if (Bluetooth_AT_Scan(4))				
														{												
															Bluetooth_Render_Main_Terminal(line++, "Getting Device name...");		
															for (device = 0; device < Bluetooth_DeviceMax; device ++)	
																Bluetooth_AT_GetDeviceName(device);		
																
															Bluetooth_Clear_Terminal();																
															for (device = 0; device < Bluetooth_DeviceMax; device ++)	
															{
																Bluetooth_DeviceChoice[device].IsDisable = !Bluetooth_DeviceList[device].State;		
																Bluetooth_DeviceChoice[device].IsPressed = 0;		
																TFTSevice_Button_08x16(&Bluetooth_DeviceChoice[device]);					
															}																
														}					
														return 1;
		case BTH_Button_Setting 	: ////////////////////////////////////////////////
														Bluetooth_Render_Setting();
														if (Bluetooth_Event_SETTINGProcess())		
														{	
															Bluetooth_Render_Main();															
															Bluetooth_Render_Main_Terminal(0xff, Bluetooth_AT_GetInfo());			
														}
														else 															
															Bluetooth_Render_Main();				
														return 1;
		case BTH_Button_Info	: ////////////////////////////////////////////////
														Bluetooth_Render_Main_Terminal(0xff, Bluetooth_AT_GetInfo());				
														return 1;
		case BTH_Button_Device	: ////////////////////////////////////////////////			
														Bluetooth_Clear_Terminal();					
														for (device = 0; device < Bluetooth_DeviceMax; device ++)	
														{
															Bluetooth_DeviceChoice[device].IsDisable = !Bluetooth_DeviceList[device].State;		
															Bluetooth_DeviceChoice[device].IsPressed = 0;		
															TFTSevice_Button_08x16(&Bluetooth_DeviceChoice[device]);					
														}			
														return 1;
		case BTH_Button_Power : ////////////////////////////////////////////////
														Bluetooth_Event_SwitchPWR();
														Bluetooth_Render_Main();	
														Bluetooth_Render_Status();
														return 1;
		case BTH_Button_Exit	: ////////////////////////////////////////////////
														return 0;
		default								: ////////////////////////////////////////////////
														return 1;
	}
}

//////////////////////////////////////////////////////
// Global Extern
uint8_t Bluetooth_TouchEvent_Process(void)
{	
	uint8_t cnt;	
	static int16_t x = 0, y = 0;	
	TouchFinger Event = TouchPanel_GetPoint(&x, &y); //TFT_SetPixelSafe (x, y, TFT_BLUE) ;	
	if (x >= BTH_TERMINAL_POSX )////////////////////////// Terminal region
	{
		switch (Event)
		{
			case TouchFinger_None	: ////////////////////////////////////////////////		
															return 1;
			case TouchFinger_Up		:	////////////////////////////////////////////////				
																
															for(cnt = 0; cnt < Bluetooth_DeviceMax; cnt++)
																if (TFTSevice_RegionEvent(&Bluetooth_DeviceChoice[cnt], x, y, Event)) 	
																{						
																		Bluetooth_Event_Connect(cnt);											
																		TFTSevice_Button_08x16(&Bluetooth_DeviceChoice[cnt]);	
																}																		
															return 1;
			default								:	////////////////////////////////////////////////						
																	
															for(cnt = 0; cnt < Bluetooth_DeviceMax; cnt++)
																if (TFTSevice_RegionEvent(&Bluetooth_DeviceChoice[cnt], x, y, Event)) 	
																	TFTSevice_Button_08x16(&Bluetooth_DeviceChoice[cnt]);	
															return 1;
		}
	}
	else //////////////////////////////////////////////////////////////////////////////////// Button Region
	{
		switch (Event)
		{

			case TouchFinger_None: 	return 1;
			case TouchFinger_Up:	////////////////////////////////////////////////								
							for(cnt = (BTH_BUTTON)BTH_Button_Min; cnt < (BTH_BUTTON)BTH_Button_Max; cnt++)
								if (TFTSevice_RegionEvent(&Bluetooth_Button[cnt], x, y, Event)) 		
								{					
									TFTSevice_Button_08x16(&Bluetooth_Button[cnt]);		
									return Bluetooth_Event_Button ((BTH_BUTTON)cnt);	
								}
			default						:	 /////////////////////////////////////////////////								
							for(cnt = (BTH_BUTTON)BTH_Button_Min; cnt < (BTH_BUTTON)BTH_Button_Max; cnt++)
								if (TFTSevice_RegionEvent(&Bluetooth_Button[cnt], x, y, Event)) 		
								{							
									TFTSevice_Button_08x16(&Bluetooth_Button[cnt]);		
								}
		}
	}
	return 1;
}
