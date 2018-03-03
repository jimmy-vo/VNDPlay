#include "BoardConfig.h"
#include "TFTService_Button.h"


#define BTH_Separate 									5
//////////////////////////////////////////////////////////////////////////////////
#define BTH_TITLE_POSX								1
#define BTH_TITLE_POSY								1

#define BTH_TITLE_SIZEX								319-BTH_TITLE_POSX*2
#define BTH_TITLE_SIZEY								35

//////////////////////////////////////////////////////////////////////////////////
	
#define BTH_BUTTON_FONT								ASCII_08x16 
#define BTH_BUTTON_FONTX							8
#define BTH_BUTTON_FONTY							16

#define BTH_BUTTON_SIZEX							BTH_BUTTON_FONTX * 8
#define BTH_BUTTON_SIZEY							BTH_BUTTON_FONTY 

#define BTH_BUTTON_POSX								BTH_Separate
#define BTH_BUTTON_POSY								BTH_TITLE_SIZEY + BTH_TITLE_POSY +  BTH_Separate

//////////////////////////////////////////////////////////////////////////////////
	
#define BTH_Set_FONT									ASCII_08x16 
#define BTH_Set_FONTX								8
#define BTH_Set_FONTY								16

#define BTH_Set_SIZEX								BTH_Set_FONTX * 6
#define BTH_Set_SIZEY								BTH_Set_FONTY 

//////////////////////////////////////////////////////////////////////////////////
	
#define BTH_Choice_FONT								ASCII_08x16 
#define BTH_Choice_FONTX							8
#define BTH_Choice_FONTY							16

#define BTH_Choice_SIZEX							BTH_Choice_FONTX * 11
#define BTH_Choice_SIZEY							BTH_Choice_FONTY 
//////////////////////////////////////////////////////////////////////////////////

#define BTH_TERMINAL_X								30
#define BTH_TERMINAL_Y								12

#define BTH_TERMINAL_FONT						  ASCII_08x16 
#define BTH_TERMINAL_FONTX						8
#define BTH_TERMINAL_FONTY						16

#define BTH_TERMINAL_POSX							BTH_BUTTON_POSX + BTH_BUTTON_SIZEX + BTH_Separate
#define BTH_TERMINAL_POSY							BTH_TITLE_POSY + BTH_TITLE_SIZEY +  BTH_Separate

#define BTH_TERMINAL_SIZEX						BTH_TERMINAL_FONTX * BTH_TERMINAL_X
#define BTH_TERMINAL_SIZEY						BTH_TERMINAL_FONTY * BTH_TERMINAL_Y

#define TERMINAL_BUFFERSIZE 					BTH_TERMINAL_SIZEX*BTH_TERMINAL_SIZEY/(BTH_TERMINAL_FONTX*BTH_TERMINAL_FONTY)
//////////////////////////////////////////////////////////////////////////////////

#define BTH_DEVICE_FONT						  ASCII_08x16 
#define BTH_DEVICE_FONTX						8
#define BTH_DEVICE_FONTY						16

#define BTH_DEVICE_POSX							BTH_TERMINAL_POSX
#define BTH_DEVICE_POSY							BTH_TERMINAL_POSY

#define BTH_DEVICE_SIZEX						BTH_TERMINAL_SIZEX
#define BTH_DEVICE_SIZEY						BTH_TERMINAL_SIZEY

//////////////////////////////////////////////////////////////////////////////////
	
#define BTH_STATUS_FONT								ASCII_08x16 
#define BTH_STATUS_FONTX							8
#define BTH_STATUS_FONTY							16

#define BTH_STATUS_POSX								BTH_TERMINAL_POSX 
#define BTH_STATUS_POSY								BTH_TERMINAL_POSY + BTH_TERMINAL_SIZEY + BTH_Separate 

//////////////////////////////////////////////////////////////////////////////////
	
#define BTH_UARTREGION_SIZEX					280
#define BTH_UARTREGION_SIZEY					200

#define BTH_UARTREGION_POSX						(319 - BTH_UARTREGION_SIZEX) / 2
#define BTH_UARTREGION_POSY						(239 - BTH_UARTREGION_SIZEY) / 2

#define BTH_SET_FONT									ASCII_08x16 
#define BTH_SET_FONTX								8
#define BTH_SET_FONTY								16

#define BTH_SET_SIZEX								BTH_BUTTON_FONTX * 8
#define BTH_SET_SIZEY								BTH_BUTTON_FONTY 

#define BTH_SET_POSX									BTH_UARTREGION_POSX + (BTH_UARTREGION_SIZEX - BTH_SET_SIZEX*4 - BTH_Separate*3)/2
#define BTH_SET_POSY									BTH_UARTREGION_POSY + BTH_SET_SIZEY*2 + BTH_Separate


//////////////////////////////////////////////////////////////////////////////////

#define BTH_EXITConfirm_SIZEX					200
#define BTH_EXITConfirm_SIZEY					100
#define BTH_EXITConfirm_POSX					(319 - BTH_EXITConfirm_SIZEX)/2
#define BTH_EXITConfirm_POSY					(239 - BTH_EXITConfirm_SIZEY)/2

//////////////////////////////////////////////////////////////////////////////////

#define BTH_SWITCHConfirm_SIZEX				200
#define BTH_SWITCHConfirm_SIZEY				100
#define BTH_SWITCHConfirm_POSX				(319 - BTH_SWITCHConfirm_SIZEX)/2
#define BTH_SWITCHConfirm_POSY				(239 - BTH_SWITCHConfirm_SIZEY)/2

//////////////////////////////////////////////////////////////////////////////////

#define BTH_ATConfirm_SIZEX						200
#define BTH_ATConfirm_SIZEY						100
#define BTH_ATConfirm_POSX						(319 - BTH_SWITCHConfirm_SIZEX)/2
#define BTH_ATConfirm_POSY						(239 - BTH_SWITCHConfirm_SIZEY)/2

//////////////////////////////////////////////////////////////////////////////////
// Color
//
#define	BTH_BACKGROUND											TFT_WHITE

#define	BTH_Theme_Title										TFT_BLACK
#define	BTH_Theme_UART_SETTING						TFT_DCYAN
#define	BTH_Theme_MAIN_CMD								TFT_NAVY
#define	BTH_Theme_MAIN_TERMINAL						TFT_BLACK
/////////////////////////

#define	BTH_ForeColor_UARTSETTING						TFT_RED
#define	BTH_BackColor_UARTSETTING						BTH_Theme_UART_SETTING

#define	BTH_ForeColor_ButtonNormal					TFT_BLACK
#define	BTH_BackColor_ButtonNormal					TFT_LGRAY

#define	BTH_ForeColor_ButtonSelected				TFT_BLACK
#define	BTH_BackColor_ButtonSelected				TFT_DGRAY
//////////////////////////


typedef enum
{	
	BTH_Button_Min 						= 0,
	
	BTH_Button_Power 					= 0,	
	BTH_Button_ATMode 				= 1,	
	BTH_Button_Setting		 				= 2,	
	BTH_Button_Info		 				= 3,	
	
	BTH_Button_Device					= 4,
	BTH_Button_Scan 					= 5,
		
	BTH_Button_Exit  					= 6,

	BTH_Button_Max 						= 7,
}BTH_BUTTON;



typedef enum
{
	BTH_SET_Min    			= 0,	
	
	BTH_SET_9600     		= 0,	
	BTH_SET_19200     		= 1,
	BTH_SET_38400     		= 2,
	BTH_SET_57600     		= 3,
	
	BTH_SET_115200     	= 4,
	BTH_SET_230400     	= 5,
	BTH_SET_460800    		= 6,
	
	BTH_SET_STOP1     				= 7,
	BTH_SET_STOP2    				= 8,	

	BTH_SET_PARITYNone  			= 9,	
	BTH_SET_PARITYOdd  			= 10,	
	BTH_SET_PARITYEven 			= 11,	
	
	BTH_SET_ROLE_Slave 			= 12,
	BTH_SET_ROLE_Master			= 13,
	BTH_SET_ROLE_Slaveloop		= 14,
	
	BTH_SET_CMODE_Manual 		= 15,
	BTH_SET_CMODE_Auto				= 16,
	BTH_SET_CMODE_Slaveloop	= 17,

	BTH_SET_Set_Name					= 18,
	BTH_SET_Set_Pass					= 19,
	
	
	BTH_SET_Ok       				= 20,		
	BTH_SET_Cancel     			= 21,

	BTH_SET_Max    					= 22,
}BTH_UART;

	
extern TFTService_Button Bluetooth_Button[BTH_Button_Max];
extern TFTService_Button Bluetooth_UART[BTH_SET_Max];
extern TFTService_Button Bluetooth_DeviceChoice[BTH_TERMINAL_Y];

void Bluetooth_InitLayout(void);
void Bluetooth_Render_Main(void);
void Bluetooth_Render_Status(void);
void Bluetooth_Render_Setting(void);
void Bluetooth_Render_Main_Terminal(uint8_t Line, char * INPUT);
void Bluetooth_Clear_Terminal(void);
