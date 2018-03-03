
//////////////////////////////////////////////////////////////////////////////////

#define Main_Background									TFT_GREEN
#define Main_Theme_Title								TFT_BLACK

#define	Main_Label_Button								TFT_RED
#define	Main_Theme_Button								TFT_DCYAN

#define	Main_ForeColor_ButtonNormal					TFT_BLACK
#define	Main_BackColor_ButtonNormal					TFT_LGRAY

#define	Main_ForeColor_ButtonSelected				TFT_BLACK
#define	Main_BackColor_ButtonSelected				TFT_DGRAY

#define Main_Separate 									8

//////////////////////////////////////////////////////////////////////////////////

#define Main_TITLE_POSX									1
#define Main_TITLE_POSY									1
#define Main_TITLE_SIZEX									320 - (Main_TITLE_POSX*2)
#define Main_TITLE_SIZEY									35


//////////////////////////////////////////////////////////////////////////////////
	
#define Main_FONT_Button							ASCII_08x16
#define Main_FONTSize_Button_X				8
#define Main_FONTSize_Button_Y				16
#define Main_Separate_Button					Main_Separate

#define Main_ButtonSIZE					Main_FONTSize_Button_X * 11
#define Main_ButtonPOSX					(320 - Main_ButtonSIZE*3 - Main_Separate_Button*4)/2
#define Main_ButtonPOSY					Main_TITLE_POSY + Main_TITLE_SIZEY + Main_Separate*3 + Main_FONTSize_Button_Y

void Main_Window (void);
