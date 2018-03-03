#include "TFTService_Dialog.h"
#include "TFT_SSD1289.h"
#include "TFTService_Draw.h"
#include "TFTService_Text.h"
#include "TFTService_Button.h"



static char *DialogButtonString[Dialog_Max] = 
{	
	" No ",
	" Yes ",
	" OK "
};


static void Dialog_RenderText (int16_t x, int16_t y, int16_t width, int16_t height, const char * Title, const char *Content)
{	
	///////////////////////////////////////////////////////////////////////////////
	//draw dialog
	TFT_DrawRect		(TFT_GREY 				, x-2	, y-2	, width +4,	height+4);
	TFT_DrawRect		(TFT_BLACK				, x-1	, y-1	, width +2,	height+2);
	TFT_DrawFullRect(DialogTHEMEColor , x		,	y		, width		,	height);		
	
	///////////////////////////////////////////////////////////////////////////////
	//draw title			
	
	TFT_DrawFullRect(DialogTitleBackColor , x, y, width,DialogTitleFONTY);	
	
	TFT_Text_Transparent(x, y, DialogTitleFONT, DialogTitleForeColor,  Title);
	TFT_Text_Transparent(x+1, y, DialogTitleFONT, DialogTitleForeColor,  Title);
	///////////////////////////////////////////////////////////////////////////////
	//draw Content	
	
	TFT_Text_Background_Limit(
															x, y+DialogTitleFONTY,
															width, height -DialogTitleFONTY, 
															DialogContentFONT, DialogContentForeColor, DialogContentBackColor, 
															Content
														);		
}

TFTSevice_Dialog TFTSevice_ShowDialogYesNo (int16_t x, int16_t y, int16_t width, int16_t height, const char * Title, const char *Content)//  void (*Yes)(void), void (*No)(void))
{
	TFTService_Button DialogArea;
	TFTService_Button DialogButton[Dialog_Max];
	TouchFinger Event;
	int16_t xtouch = 0, ytouch = 0;	
	TFTSevice_Dialog cnt = Dialog_No, Result = Dialog_Max;
	
	///////////////////////////////////////////////////////////////////////////////
	//Dialog	layout
	DialogArea.MinX = x ;
	DialogArea.MinY = y ;
	DialogArea.MaxX = x + width  -1 ;
	DialogArea.MaxY = y + height -1 ;
	
	///////////////////////////////////////////////////////////////////////////////
	//Button	layout
	DialogButton[Dialog_No].MaxX = x + width - DialogButtonSeparate;
	DialogButton[Dialog_No].MaxY = y + height - DialogButtonSeparate;
	DialogButton[Dialog_No].MinX = DialogButton[Dialog_No].MaxX - DialogButtonNoSize;
	DialogButton[Dialog_No].MinY = DialogButton[Dialog_No].MaxY - DialogButtonFONTY;	
	DialogButton[Dialog_No].Normal.ForeColor = Dialog_ForeColor_ButtonNormal;
	DialogButton[Dialog_No].Normal.BackColor = Dialog_BackColor_ButtonNormal;
	DialogButton[Dialog_No].Normal.Text = DialogButtonString[Dialog_No];
	DialogButton[Dialog_No].Selected.ForeColor = Dialog_ForeColor_ButtonSelected;
	DialogButton[Dialog_No].Selected.BackColor = Dialog_BackColor_ButtonSelected;
	DialogButton[Dialog_No].Selected.Text = DialogButtonString[Dialog_No];	
	DialogButton[Dialog_No].IsPressed = 0;
	DialogButton[Dialog_No].IsDisable = 0;
	
	DialogButton[Dialog_Yes].MaxX = DialogButton[Dialog_No].MinX - DialogButtonSeparate;
	DialogButton[Dialog_Yes].MaxY = DialogButton[Dialog_No].MaxY;
	DialogButton[Dialog_Yes].MinX = DialogButton[Dialog_Yes].MaxX - DialogButtonYesSize;
	DialogButton[Dialog_Yes].MinY = DialogButton[Dialog_Yes].MaxY - DialogButtonFONTY;	
	DialogButton[Dialog_Yes].Normal.ForeColor = Dialog_ForeColor_ButtonNormal;
	DialogButton[Dialog_Yes].Normal.BackColor = Dialog_BackColor_ButtonNormal;
	DialogButton[Dialog_Yes].Normal.Text = DialogButtonString[Dialog_Yes];
	DialogButton[Dialog_Yes].Selected.ForeColor = Dialog_ForeColor_ButtonSelected;
	DialogButton[Dialog_Yes].Selected.BackColor = Dialog_BackColor_ButtonSelected;
	DialogButton[Dialog_Yes].Selected.Text = DialogButtonString[Dialog_Yes];
	DialogButton[Dialog_Yes].IsPressed = 0;
	DialogButton[Dialog_Yes].IsDisable = 0;
	
	///////////////////////////////////////////////////////////////////////////////
	//Render
	while (TouchPanel_GetPoint(&xtouch, &ytouch) != TouchFinger_None);
	Dialog_RenderText (x, y, width, height,  Title, Content);
	TFTSevice_Button_08x16(&DialogButton[Dialog_No]);
	TFTSevice_Button_08x16(&DialogButton[Dialog_Yes]);			
	///////////////////////////////////////////////////////////////////////////////
	//wait user response
	while (Result == Dialog_Max)
	{
		Event = TouchPanel_GetPoint(&xtouch, &ytouch); //Pixel (x, y, RED) ;

			if (xtouch >= DialogArea.MinX &&  ytouch>=DialogArea.MinY && xtouch <= DialogArea.MaxX && ytouch <= DialogArea.MaxY)
			{
				switch (Event)
				{					
					case TouchFinger_None: break;
					case TouchFinger_Up: 					
																for(cnt = Dialog_No; cnt <= Dialog_Yes; cnt++)
																	if (TFTSevice_RegionEvent(&DialogButton[cnt], xtouch, ytouch, Event))
																	{
																		TFTSevice_Button_08x16(&DialogButton[cnt]);													
																		Result = cnt;
																	}									
																break;
					default:								
																for(cnt = Dialog_No; cnt <= Dialog_Yes; cnt++)
																	if (TFTSevice_RegionEvent(&DialogButton[cnt], xtouch, ytouch, Event))
																		TFTSevice_Button_08x16(&DialogButton[cnt]);											
																break;
				}
			}
			else
			{
				switch (Event)
				{					
					case TouchFinger_Up: 			
																Dialog_RenderText (x, y, width, height,  Title, Content);	
																TFTSevice_Button_08x16(&DialogButton[Dialog_No]);
																TFTSevice_Button_08x16(&DialogButton[Dialog_Yes]);				
																break;
					default:	break;
				}
			}
	}
	
	return Result;
}



TFTSevice_Dialog TFTSevice_ShowDialogOK (int16_t x, int16_t y, int16_t width, int16_t height, const char * Title, const char *Content)//  void (*Yes)(void), void (*No)(void))
{
	TFTService_Button DialogArea;
	TFTService_Button DialogButton;
	TFTSevice_Dialog Result = Dialog_Max;
	int16_t xtouch = 0, ytouch = 0;	
	TouchFinger Event;
	
	///////////////////////////////////////////////////////////////////////////////
	//Dialog	layout
	DialogArea.MinX = x ;
	DialogArea.MinY = y;
	DialogArea.MaxX = x + width -1 ;
	DialogArea.MaxY = y + height -1;
	
	///////////////////////////////////////////////////////////////////////////////
	//Button	layout	
	DialogButton.MinX = x + (width - DialogButtonOKSize)/2 ;
	DialogButton.MinY = y + height - DialogButtonSeparate - DialogButtonFONTY;
	DialogButton.MaxX = DialogButton.MinX + DialogButtonOKSize;
	DialogButton.MaxY = DialogButton.MinY + DialogButtonFONTY;	
	DialogButton.Normal.ForeColor = Dialog_ForeColor_ButtonNormal;
	DialogButton.Normal.BackColor = Dialog_BackColor_ButtonNormal;
	DialogButton.Normal.Text = DialogButtonString[Dialog_OK];
	DialogButton.Selected.ForeColor = Dialog_ForeColor_ButtonSelected;
	DialogButton.Selected.BackColor = Dialog_BackColor_ButtonSelected;
	DialogButton.Selected.Text = DialogButtonString[Dialog_OK];
	DialogButton.IsPressed = 0;
	DialogButton.IsDisable = 0;
	
	///////////////////////////////////////////////////////////////////////////////
	//Render
	while (TouchPanel_GetPoint(&xtouch, &ytouch) != TouchFinger_None);
	Dialog_RenderText (x, y, width, height,  Title, Content);	
	TFTSevice_Button_08x16(&DialogButton);
	///////////////////////////////////////////////////////////////////////////////
	//wait user response
	while (Result == Dialog_Max)
	{
		Event = TouchPanel_GetPoint(&xtouch, &ytouch); //Pixel (x, y, RED) ;

			if (xtouch >= DialogArea.MinX &&  ytouch>=DialogArea.MinY && xtouch <= DialogArea.MaxX && ytouch <= DialogArea.MaxY)
			{
				switch (Event)
				{					
					case TouchFinger_None: break;
					case TouchFinger_Up: 					
																if (TFTSevice_RegionEvent(&DialogButton, xtouch, ytouch, Event))
																{
																	TFTSevice_Button_08x16(&DialogButton);									
																	return Dialog_OK;
																}									
																break;
					default:								
																if (TFTSevice_RegionEvent(&DialogButton, xtouch, ytouch, Event))
																	TFTSevice_Button_08x16(&DialogButton);											
																break;
				}
			}
			else
			{
				switch (Event)
				{					
					case TouchFinger_Up:	return Dialog_OK;
					default:							break;
				}
			}
	}
	
	return Result;
}
