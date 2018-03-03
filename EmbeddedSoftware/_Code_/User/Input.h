#include "BoardConfig.h"

#define Joystick_Average						20
#define	Joystick_MAX								19

#define	Joystick_ADCMAX_LX					4096
#define	Joystick_OFFSET_LX					9

#define	Joystick_ADCMAX_LY					4096
#define	Joystick_OFFSET_LY					9

#define	Joystick_ADCMAX_RX					4096
#define	Joystick_OFFSET_RX					9

#define	Joystick_ADCMAX_RY					4096
#define	Joystick_OFFSET_RY					9

typedef enum
{
	A_Joystick 	= 0,
	
	A_Right 		= 1,
	A_Up 				= 2,
	A_Left 			= 3,
	A_Down 			= 4,
	
	B_Down 			= 5,
	B_Right 		= 6,
	B_Left 			= 7,
	B_Up 				= 8,
	
	B_Joystick	= 9,
}INPUT_BUTTON;


int16_t Joystick_AX(void);
int16_t Joystick_AY(void);
int16_t Joystick_BX(void);
int16_t Joystick_BY(void);

void Joystick_A(int16_t *x,int16_t *y);
void Joystick_B(int16_t *x,int16_t *y);

uint8_t ButtonA_Left		(void);
uint8_t ButtonA_Right		(void);
uint8_t ButtonA_Up			(void);
uint8_t ButtonA_Down		(void);
uint8_t ButtonA_Joystick(void);
uint8_t ButtonB_Left		(void);
uint8_t ButtonB_Right		(void);
uint8_t ButtonB_Up			(void);
uint8_t ButtonB_Down		(void);
uint8_t ButtonB_Joystick(void);	

void Button_Initialize (void);
void INPUT_ReadList (uint8_t * INPUT);
