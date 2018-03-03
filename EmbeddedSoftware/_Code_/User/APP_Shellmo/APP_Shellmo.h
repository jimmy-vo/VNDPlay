
//////////////////////////////////////////////////////////////////////////////////

#define Shellmo_Background								TFT_WHITE
#define Shellmo_Theme_Title								TFT_PURPLE


#define	Shellmo_ForeColor_ButtonNormal					TFT_BLACK
#define	Shellmo_BackColor_ButtonNormal					TFT_LGRAY

#define	Shellmo_ForeColor_ButtonSelected				TFT_BLACK
#define	Shellmo_BackColor_ButtonSelected				TFT_DGRAY

#define Shellmo_Separate 									7

//////////////////////////////////////////////////////////////////////////////////

#define Shellmo_TITLE_POSX									1
#define Shellmo_TITLE_POSY									1
#define Shellmo_TITLE_SIZEX									320 - (Shellmo_TITLE_POSX*2)
#define Shellmo_TITLE_SIZEY									35

//////////////////////////////////////////////////////////////////////////////////

#define Shellmo_J_SIZE 				100//80
#define Shellmo_JLPOSX 				Shellmo_Separate
#define Shellmo_JPOSY 				Shellmo_TITLE_SIZEY + Shellmo_Separate
#define Shellmo_JRPOSX 				320 - Shellmo_J_SIZE - Shellmo_Separate

//////////////////////////////////////////////////////////////////////////////////

#define Shellmo_J_MOUSESIZE 	6
#define Shellmo_J_MOUSESCALE 	(Shellmo_J_SIZE/2-Shellmo_J_MOUSESIZE-1)/9
#define Shellmo_JLMOUSESPOSX 	Shellmo_JLPOSX+Shellmo_J_SIZE/2
#define Shellmo_JRMOUSESPOSX 	Shellmo_JRPOSX+Shellmo_J_SIZE/2
#define Shellmo_JMOUSESPOSY 	Shellmo_JPOSY+Shellmo_J_SIZE/2

//////////////////////////////////////////////////////////////////////////////////

#define Shellmo_ButtonSIZE		14
#define Shellmo_ButtonAPOSX		Shellmo_Separate + Shellmo_ButtonSIZE*3
#define Shellmo_ButtonBPOSX		320 - Shellmo_ButtonSIZE*3 - Shellmo_Separate
#define Shellmo_ButtonPOSY		240 - Shellmo_ButtonSIZE*3 - Shellmo_Separate

//////////////////////////////////////////////////////////////////////////////////
	
#define Shellmo_FONT_Choice							ASCII_08x16 
#define Shellmo_FONTSize_Choice_X				8
#define Shellmo_FONTSize_Choice_Y				16
#define Shellmo_Separate_Choice					7

#define Shellmo_ChoiceSIZE					Shellmo_FONTSize_Choice_X*11
#define Shellmo_ChoicePOSX					320 /2  - Shellmo_ChoiceSIZE/2
#define Shellmo_ChoicePOSY					240 - Shellmo_FONTSize_Choice_Y*3 - Shellmo_Separate_Choice*3

void Shellmo_Window (void);
