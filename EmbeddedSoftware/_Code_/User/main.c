#include "INCLUDE.h"



int main (void)
{	
	////////////////////////////////////////////
	Initialize_Timer();
	Button_Initialize ();
	RGB_Initialize();
	ADC_Iinitialize();
	Bluetooth_Initialize();
  TFT_Init(90);
	TouchPanel_Initialize();
	////////////////////////////////////////////		
	Main_Window();
};
