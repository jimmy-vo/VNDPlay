#include "Bluetooth_Layout.h"
#include "Bluetooth.h"
#include <stdio.h>
#include <stdlib.h>
#include "TFT_SSD1289.h"
#include "TFTService_Text.h"
#include "TFTService_Draw.h"



static char *BthButtonString[BTH_Button_Max] = 
{		
	" Switch ",	
	" ATMode ",
	" Set..  ", 
	" Info   ",	
	" Device ",
	" Scan.. ",
	" Exit   ",
};

static char *BthUartString[BTH_SET_Max] = 
{	
	"   9600 ", "  19200 ", "  38400 ", "  57600 ", " 115200 ", " 230400 ", " 460800 ",	
	"  1-bit ", "  2-bit ",	
	"   None ", "    Odd ", "   Even ",		
	"  Slave ", " Master ", " Sla-Lo ",		
	" Manual ", "   Auto ", " Sla-Lo ",		
	"   Name ", "   Pass ",	
	"   Ok   ",	" Cancel ",
};

static char *BthStatusString[2][2] = 
{
	"   ON   ", "   OFF  ",
	" AT CMD ", " AT COM ",
};

TFTService_Button Bluetooth_Button[BTH_Button_Max];
TFTService_Button Bluetooth_UART[BTH_SET_Max];
TFTService_Button Bluetooth_DeviceChoice[BTH_TERMINAL_Y];


//////////////////////////////////////////////////////////////////////////////////
// Bluetooth Layout Init
//
void Bluetooth_InitLayout(void)
{		
	 uint8_t cnt;
	 uint16_t	 posx , posy;
	
	//Init Button
	posx = BTH_BUTTON_POSX; 
	posy = BTH_BUTTON_POSY; 
	for(cnt = (BTH_BUTTON)BTH_Button_Min; cnt < (BTH_BUTTON)BTH_Button_Max; cnt++)		
	{ 
			Bluetooth_Button[cnt].MinX = posx;
			Bluetooth_Button[cnt].MinY = posy;
			Bluetooth_Button[cnt].MaxX = Bluetooth_Button[cnt].MinX + BTH_BUTTON_SIZEX;
			Bluetooth_Button[cnt].MaxY = Bluetooth_Button[cnt].MinY + BTH_BUTTON_SIZEY;	
						
			posy = Bluetooth_Button[cnt].MaxY + BTH_Separate*2; 		
			
			if	(cnt == (BTH_BUTTON)BTH_Button_Info || cnt == (BTH_BUTTON)BTH_Button_Scan )	
			{				
				posy += BTH_BUTTON_SIZEY - BTH_Separate;
			}
			
			Bluetooth_Button[cnt].Normal.ForeColor 		= BTH_ForeColor_ButtonNormal;
			Bluetooth_Button[cnt].Normal.BackColor 		= BTH_BackColor_ButtonNormal;
			Bluetooth_Button[cnt].Normal.Text 				= BthButtonString[cnt];
			Bluetooth_Button[cnt].Selected.ForeColor 	= BTH_ForeColor_ButtonSelected;
			Bluetooth_Button[cnt].Selected.BackColor 	= BTH_BackColor_ButtonSelected;
			Bluetooth_Button[cnt].Selected.Text 			= BthButtonString[cnt];
			Bluetooth_Button[cnt].Disable.ForeColor 	= BTH_ForeColor_ButtonSelected;
			Bluetooth_Button[cnt].Disable.BackColor 	= BTH_BackColor_ButtonSelected;
			Bluetooth_Button[cnt].Disable.Text 				= BthButtonString[cnt];				
	}
	
	
	//Init UART
	posx = BTH_SET_POSX; 
	posy = BTH_SET_POSY; 
	for(cnt = (BTH_UART)BTH_SET_Min; cnt < (BTH_UART)BTH_SET_Max; cnt++)		
	{		
			if ((cnt == (BTH_UART)BTH_SET_STOP1)||(cnt == (BTH_UART)BTH_SET_ROLE_Slave)||(cnt == (BTH_UART)BTH_SET_CMODE_Manual))
			{
				posx += BTH_SET_SIZEX + BTH_Separate;
				posy = BTH_SET_POSY;
			}	
			else if (cnt == (BTH_UART)BTH_SET_PARITYNone)
			{
				posy +=  BTH_SET_SIZEY + BTH_Separate;
			}
			if 	(cnt == (BTH_UART)BTH_SET_Ok)	
			{				
				Bluetooth_UART[cnt].MaxX = BTH_UARTREGION_POSX + BTH_UARTREGION_SIZEX - BTH_SET_SIZEX - BTH_Separate;
				Bluetooth_UART[cnt].MaxY = BTH_UARTREGION_POSY + BTH_UARTREGION_SIZEY - BTH_Separate/2;
				Bluetooth_UART[cnt].MinX = Bluetooth_UART[cnt].MaxX - BTH_SET_SIZEX;
				Bluetooth_UART[cnt].MinY = Bluetooth_UART[cnt].MaxY - BTH_SET_SIZEY;
			}
			else if 	(cnt == (BTH_UART)BTH_SET_Cancel)	
			{				
				Bluetooth_UART[cnt].MaxX = BTH_UARTREGION_POSX + BTH_UARTREGION_SIZEX -  BTH_Separate/2;
				Bluetooth_UART[cnt].MaxY = BTH_UARTREGION_POSY + BTH_UARTREGION_SIZEY -  BTH_Separate/2;
				Bluetooth_UART[cnt].MinX = Bluetooth_UART[cnt].MaxX - BTH_SET_SIZEX;
				Bluetooth_UART[cnt].MinY = Bluetooth_UART[cnt].MaxY - BTH_SET_SIZEY;
			}
			else if (cnt == (BTH_UART)BTH_SET_Set_Name)
			{				
				Bluetooth_UART[cnt].MinX = Bluetooth_UART[BTH_SET_ROLE_Slave].MinX;
				Bluetooth_UART[cnt].MinY =  Bluetooth_UART[BTH_SET_PARITYOdd].MinY;
				Bluetooth_UART[cnt].MaxX = Bluetooth_UART[cnt].MinX + BTH_SET_SIZEX;
				Bluetooth_UART[cnt].MaxY = Bluetooth_UART[cnt].MinY + BTH_SET_SIZEY;	
			}
			else if (cnt == (BTH_UART)BTH_SET_Set_Pass)
			{				
				Bluetooth_UART[cnt].MinX = Bluetooth_UART[BTH_SET_CMODE_Manual].MinX;
				Bluetooth_UART[cnt].MinY = Bluetooth_UART[BTH_SET_PARITYOdd].MinY;
				Bluetooth_UART[cnt].MaxX = Bluetooth_UART[cnt].MinX + BTH_SET_SIZEX;
				Bluetooth_UART[cnt].MaxY = Bluetooth_UART[cnt].MinY + BTH_SET_SIZEY;	
			}
			else 
			{
				Bluetooth_UART[cnt].MinX = posx;
				Bluetooth_UART[cnt].MinY = posy;
				Bluetooth_UART[cnt].MaxX = Bluetooth_UART[cnt].MinX + BTH_SET_SIZEX;
				Bluetooth_UART[cnt].MaxY = Bluetooth_UART[cnt].MinY + BTH_SET_SIZEY;	
				
				posy = Bluetooth_UART[cnt].MaxY + BTH_Separate; 
			}

			Bluetooth_UART[cnt].Normal.ForeColor 		= BTH_ForeColor_ButtonNormal;
			Bluetooth_UART[cnt].Normal.BackColor 		= BTH_BackColor_ButtonNormal;
			Bluetooth_UART[cnt].Normal.Text 				= BthUartString[cnt];
			Bluetooth_UART[cnt].Selected.ForeColor 	= BTH_ForeColor_ButtonSelected;
			Bluetooth_UART[cnt].Selected.BackColor 	= BTH_BackColor_ButtonSelected;
			Bluetooth_UART[cnt].Selected.Text 			= BthUartString[cnt];
			Bluetooth_UART[cnt].Disable.ForeColor 		= BTH_ForeColor_ButtonSelected;
			Bluetooth_UART[cnt].Disable.BackColor 		= BTH_BackColor_ButtonSelected;
			Bluetooth_UART[cnt].Disable.Text 				= BthUartString[cnt];						
	}
	
	posx = BTH_DEVICE_POSX; 
	posy = BTH_DEVICE_POSY; 
	for(cnt = 0; cnt < BTH_TERMINAL_Y; cnt++)		
	{		
		Bluetooth_DeviceChoice[cnt].MinX = posx;
		Bluetooth_DeviceChoice[cnt].MinY = posy;
		Bluetooth_DeviceChoice[cnt].MaxX = Bluetooth_DeviceChoice[cnt].MinX + BTH_DEVICE_SIZEX;
		Bluetooth_DeviceChoice[cnt].MaxY = Bluetooth_DeviceChoice[cnt].MinY + BTH_DEVICE_FONTY;	
		
		posy = Bluetooth_DeviceChoice[cnt].MaxY;	

		Bluetooth_DeviceChoice[cnt].Normal.ForeColor 		= TFT_WHITE;
		Bluetooth_DeviceChoice[cnt].Normal.BackColor 		= BTH_Theme_MAIN_TERMINAL;
		Bluetooth_DeviceChoice[cnt].Normal.Text 				= Bluetooth_DeviceList[cnt].Name;
		Bluetooth_DeviceChoice[cnt].Selected.ForeColor 	= TFT_BLUE;
		Bluetooth_DeviceChoice[cnt].Selected.BackColor 	= BTH_Theme_MAIN_TERMINAL;
		Bluetooth_DeviceChoice[cnt].Selected.Text 			= Bluetooth_DeviceList[cnt].Name;
		Bluetooth_DeviceChoice[cnt].Disable.ForeColor 	= BTH_Theme_MAIN_TERMINAL;
		Bluetooth_DeviceChoice[cnt].Disable.BackColor 	= BTH_Theme_MAIN_TERMINAL;
		Bluetooth_DeviceChoice[cnt].Disable.Text 				= Bluetooth_DeviceList[cnt].Name;		
	}	
}


//////////////////////////////////////////////////////////////////////////////////
//  RENDER

void Bluetooth_Clear_Terminal(void)
{
	uint8_t cnt;
	TFT_DrawFullRect (BTH_Theme_MAIN_TERMINAL, BTH_TERMINAL_POSX, BTH_TERMINAL_POSY, BTH_TERMINAL_SIZEX, BTH_TERMINAL_SIZEY);
	
	for (cnt = 0; cnt < Bluetooth_DeviceMax; cnt ++)												
		Bluetooth_DeviceChoice[cnt].IsDisable = 1;	
}

void Bluetooth_Render_Main_Terminal(uint8_t Line, char * INPUT)
{
	if (INPUT)
	{
		if (Line >= BTH_TERMINAL_Y)
		{
			Bluetooth_Clear_Terminal();
			TFT_Text_Background_Limit(
																	BTH_TERMINAL_POSX,
																	BTH_TERMINAL_POSY,
																	BTH_TERMINAL_SIZEX, 
																	BTH_TERMINAL_SIZEY,
																	BTH_TERMINAL_FONT,	TFT_WHITE, BTH_Theme_MAIN_TERMINAL,
																	INPUT
																);	
		}																
		else					
			TFT_Text_Background_Limit(
																	BTH_TERMINAL_POSX,
																	BTH_TERMINAL_POSY + Line*BTH_TERMINAL_FONTY,
																	BTH_TERMINAL_SIZEX, 
																	BTH_TERMINAL_SIZEY - Line*BTH_TERMINAL_FONTY,
																	BTH_TERMINAL_FONT, TFT_WHITE, BTH_Theme_MAIN_TERMINAL,
																	INPUT
																);				
	}		
}

void Bluetooth_Render_Status (void)
{
	BTH_BUTTON cnt;
	
	Bluetooth_Clear_Terminal();
		
	Bluetooth_Button[BTH_Button_Power].Normal.Text 		= BthStatusString[0][!Bluetooth_GetPwrPin()];
	Bluetooth_Button[BTH_Button_Power].Selected.Text 	= BthStatusString[0][!Bluetooth_GetPwrPin()];
	
	if (!Bluetooth_GetKeyPin() || !Bluetooth_GetPwrPin())
	{
		Bluetooth_Button[BTH_Button_Setting].IsDisable 	= 1;
		Bluetooth_Button[BTH_Button_Info].IsDisable 		= 1;
		Bluetooth_Button[BTH_Button_Device].IsDisable		= 1;
		Bluetooth_Button[BTH_Button_Scan].IsDisable 		= 1;
		
		Bluetooth_Button[BTH_Button_ATMode].Normal.Text 	= BthStatusString[1][1];
		Bluetooth_Button[BTH_Button_ATMode].Selected.Text = BthStatusString[1][1];
	}
	else
	{
		Bluetooth_Button[BTH_Button_Setting].IsDisable 	= 0;
		Bluetooth_Button[BTH_Button_Info].IsDisable 		= 0;
		Bluetooth_Button[BTH_Button_Device].IsDisable		= 0;
		Bluetooth_Button[BTH_Button_Scan].IsDisable 		= 0;		
		
		Bluetooth_Button[BTH_Button_ATMode].Normal.Text 	= BthStatusString[1][0];
		Bluetooth_Button[BTH_Button_ATMode].Selected.Text = BthStatusString[1][0];
	}
	for (cnt = BTH_Button_Min; cnt < BTH_Button_Max; cnt ++)
		TFTSevice_Button_08x16(&Bluetooth_Button[cnt]);
}


void Bluetooth_Render_Setting(void)
{	
	uint8_t cnt;	
	// UART Region
	TFT_DrawRect 			(TFT_GREY				, BTH_UARTREGION_POSX-2 , BTH_UARTREGION_POSY-2 ,BTH_UARTREGION_SIZEX+4	, BTH_UARTREGION_SIZEY+4);
	TFT_DrawRect 			(TFT_BLACK			, BTH_UARTREGION_POSX-1 , BTH_UARTREGION_POSY-1 ,BTH_UARTREGION_SIZEX+2	, BTH_UARTREGION_SIZEY+2);
	TFT_DrawFullRect 	(BTH_BACKGROUND	, BTH_UARTREGION_POSX 	, BTH_UARTREGION_POSY 	,BTH_UARTREGION_SIZEX		, BTH_UARTREGION_SIZEY);
			
	
	TFT_DrawFullRect (BTH_Theme_Title, BTH_UARTREGION_POSX , BTH_UARTREGION_POSY ,BTH_UARTREGION_SIZEX, BTH_SET_FONTY);
	TFT_Text_Transparent(BTH_UARTREGION_POSX, BTH_UARTREGION_POSY, BTH_SET_FONT,	TFT_BLUE, 	"  HC05 Configuration");	
	TFT_Text_Transparent(BTH_UARTREGION_POSX+1, BTH_UARTREGION_POSY, BTH_SET_FONT,	TFT_BLUE, "  HC05 Configuration");	
	
	
	// Baudrate Region
	TFT_DrawFullRect(
										BTH_Theme_UART_SETTING, 
										Bluetooth_UART[BTH_SET_9600].MinX -2 , Bluetooth_UART[BTH_SET_9600].MinY - BTH_SET_FONTY- 2,
										BTH_SET_SIZEX + 4, BTH_SET_SIZEY*8 + BTH_Separate*6 + 4
									);
	TFT_Text_Transparent(
												Bluetooth_UART[BTH_SET_9600].MinX, Bluetooth_UART[BTH_SET_9600].MinY - BTH_SET_FONTY,
												ASCII_08x16,	BTH_ForeColor_UARTSETTING, "BAUDRATE"
											);	
							
	// STOP Region
	TFT_DrawFullRect(
										BTH_Theme_UART_SETTING, 
										Bluetooth_UART[BTH_SET_STOP1].MinX -2 , Bluetooth_UART[BTH_SET_STOP1].MinY - BTH_SET_FONTY- 2,
										BTH_SET_SIZEX  + 4, BTH_SET_SIZEY*3 + BTH_Separate + 4
									);
	TFT_Text_Transparent(
												Bluetooth_UART[BTH_SET_STOP1].MinX, Bluetooth_UART[BTH_SET_STOP1].MinY - BTH_SET_FONTY,
												ASCII_08x16,	BTH_ForeColor_UARTSETTING, "  STOP"
											);	
							
	// PARITY Region
	TFT_DrawFullRect(
										BTH_Theme_UART_SETTING, 
										Bluetooth_UART[BTH_SET_PARITYNone].MinX -2 , Bluetooth_UART[BTH_SET_PARITYNone].MinY - BTH_SET_FONTY- 2,
										BTH_SET_SIZEX  + 4, BTH_SET_SIZEY*4 + BTH_Separate*2 + 4
									);
	TFT_Text_Transparent(
												Bluetooth_UART[BTH_SET_PARITYNone].MinX, Bluetooth_UART[BTH_SET_PARITYNone].MinY - BTH_SET_FONTY,
												ASCII_08x16,	BTH_ForeColor_UARTSETTING, " PARITY"
											);	
	
	// Role Region
	TFT_DrawFullRect(
										BTH_Theme_UART_SETTING, 
										Bluetooth_UART[BTH_SET_ROLE_Slave].MinX -2 , Bluetooth_UART[BTH_SET_ROLE_Slave].MinY - BTH_SET_FONTY- 2,
										BTH_SET_SIZEX  + 4, BTH_SET_SIZEY*4 + BTH_Separate*2 + 4
									);
	TFT_Text_Transparent(
												Bluetooth_UART[BTH_SET_ROLE_Slave].MinX, Bluetooth_UART[BTH_SET_ROLE_Slave].MinY - BTH_SET_FONTY,
												ASCII_08x16,	BTH_ForeColor_UARTSETTING, "  ROLE"
											);	
											
	// PARITY Region
	TFT_DrawFullRect(
										BTH_Theme_UART_SETTING, 
										Bluetooth_UART[BTH_SET_CMODE_Manual].MinX -2 , Bluetooth_UART[BTH_SET_CMODE_Manual].MinY - BTH_SET_FONTY- 2,
										BTH_SET_SIZEX  + 4, BTH_SET_SIZEY*4 + BTH_Separate*2 + 4
									);
	TFT_Text_Transparent(
												Bluetooth_UART[BTH_SET_CMODE_Manual].MinX, Bluetooth_UART[BTH_SET_CMODE_Manual].MinY - BTH_SET_FONTY,
												ASCII_08x16,	BTH_ForeColor_UARTSETTING, " C-MODE"
											);	
											
											
	// PARITY name pass
	TFT_DrawFullRect(
										BTH_Theme_UART_SETTING, 
										Bluetooth_UART[BTH_SET_Set_Name].MinX -2 , Bluetooth_UART[BTH_SET_Set_Name].MinY - BTH_SET_FONTY- 2,
										BTH_SET_SIZEX * 2 + BTH_Separate  + 4, BTH_SET_SIZEY*2  + 4
									);
	TFT_Text_Transparent(
												Bluetooth_UART[BTH_SET_Set_Name].MinX, Bluetooth_UART[BTH_SET_Set_Name].MinY - BTH_SET_FONTY,
												ASCII_08x16,	BTH_ForeColor_UARTSETTING, "     CHANGE"
											);	
											
	for(cnt = (BTH_UART) BTH_SET_Min; cnt < (BTH_UART)BTH_SET_Max; cnt++)		
		Bluetooth_UART[cnt].IsPressed = 0;
	
	switch (Bluetooth_Baudrate)
	{
		case 9600				: ////////////////////////////////////////////////
										Bluetooth_UART[BTH_SET_9600].IsPressed = 1;
										break;
		case 19200				: ////////////////////////////////////////////////
										Bluetooth_UART[BTH_SET_19200].IsPressed = 1;
										break;
		case 38400			: ////////////////////////////////////////////////
										Bluetooth_UART[BTH_SET_38400].IsPressed = 1;
										break;
		case 57600			: ////////////////////////////////////////////////
										Bluetooth_UART[BTH_SET_57600].IsPressed = 1;
										break;
		case 115200			: ////////////////////////////////////////////////
										Bluetooth_UART[BTH_SET_115200].IsPressed = 1;
										break;
		case 230400			: ////////////////////////////////////////////////
										Bluetooth_UART[BTH_SET_230400].IsPressed = 1;
										break;
		case 460800			: ////////////////////////////////////////////////
										Bluetooth_UART[BTH_SET_460800].IsPressed = 1;
										break;
	}
	switch (Bluetooth_StopBit)
	{
		case USART_StopBits_1			: ////////////////////////////////////////////////
															Bluetooth_UART[BTH_SET_STOP1].IsPressed = 1;
															break;
		case USART_StopBits_2			: ////////////////////////////////////////////////
															Bluetooth_UART[BTH_SET_STOP2].IsPressed = 1;
															break;
	}
	
	switch (Bluetooth_Parity)
	{
		case USART_Parity_No		: ////////////////////////////////////////////////
															Bluetooth_UART[BTH_SET_PARITYNone].IsPressed = 1;
															break;
		case USART_Parity_Odd		: ////////////////////////////////////////////////
															Bluetooth_UART[BTH_SET_PARITYOdd].IsPressed = 1;
															break;
		case USART_Parity_Even	: ////////////////////////////////////////////////
															Bluetooth_UART[BTH_SET_PARITYEven].IsPressed = 1;
															break;
	}
	switch (Bluetooth_ConnectMode)
	{
		case Bluetooth_CMODE_Manual					: ////////////////////////////////////////////////
																				Bluetooth_UART[BTH_SET_CMODE_Manual].IsPressed = 1;
																				break;
		case Bluetooth_CMODE_Auto						: ////////////////////////////////////////////////
																				Bluetooth_UART[BTH_SET_CMODE_Auto].IsPressed = 1;
																				break;
		case Bluetooth_CMODE_Slaveloop			: ////////////////////////////////////////////////
																				Bluetooth_UART[BTH_SET_CMODE_Slaveloop].IsPressed = 1;
																				break;		
	}
	
	switch (Bluetooth_Role)
	{
		case Bluetooth_ROLE_Slave						: ////////////////////////////////////////////////
																				Bluetooth_UART[BTH_SET_ROLE_Slave].IsPressed = 1;
																				break;
		case Bluetooth_ROLE_Master					: ////////////////////////////////////////////////
																				Bluetooth_UART[BTH_SET_ROLE_Master].IsPressed = 1;
																				break;
		case Bluetooth_ROLE_Slaveloop				: ////////////////////////////////////////////////
																				Bluetooth_UART[BTH_SET_ROLE_Slaveloop].IsPressed = 1;
																				break;		
	}
	for(cnt = (BTH_UART) BTH_SET_Min; cnt < (BTH_UART)BTH_SET_Max; cnt++)		
		TFTSevice_Button_08x16(&Bluetooth_UART[cnt]);
}

void Bluetooth_Render_Main(void)
{
	uint8_t cnt;	
	
  TFT_Clear(BTH_BACKGROUND);	
	
	//Draw titile	
	TFT_DrawFullRect (BTH_Theme_Title, BTH_TITLE_POSX, BTH_TITLE_POSY, BTH_TITLE_SIZEX, BTH_TITLE_SIZEY);
	
	TFT_Text_Transparent(26, 10, ASCII_16x24,	TFT_BLUE, "Bluetooth Setting");
	TFT_Text_Transparent(27, 10, ASCII_16x24,	TFT_BLUE, "Bluetooth Setting");
	TFT_Text_Transparent(24, 10, ASCII_16x24,	TFT_BLUE, "Bluetooth Setting");
	TFT_Text_Transparent(25, 10, ASCII_16x24,	TFT_BLUE, "Bluetooth Setting");
	TFT_Text_Transparent(294, 3, ASCII_08x12,	TFT_WHITE, "VND");	
	TFT_Text_Transparent(295, 3, ASCII_08x12,	TFT_WHITE, "VND");	
	
	// Terminal Region
	TFT_DrawFullRect (BTH_Theme_MAIN_TERMINAL, BTH_TERMINAL_POSX, BTH_TERMINAL_POSY, BTH_TERMINAL_SIZEX, BTH_TERMINAL_SIZEY);
		
	// Button
	for(cnt = (BTH_BUTTON) BTH_Button_Min; cnt < (BTH_BUTTON)BTH_Button_Max; cnt++)		
		TFTSevice_Button_08x16(&Bluetooth_Button[cnt]);	
		
					
	// device	
	for(cnt = 0; cnt < Bluetooth_DeviceMax; cnt++)		
		if (Bluetooth_DeviceChoice[cnt].IsDisable == 0)
			TFTSevice_Button_08x16(&Bluetooth_DeviceChoice[cnt]);		
			
	Bluetooth_Render_Status();
}
