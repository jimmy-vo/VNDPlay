#include "BoardConfig.h"

#define Vreff 									3.3f
#define Rtop										15.0f				
#define Rbot										3.3f
#define	Resolution							4096

#define	VF_SlopeScale						1.005f
#define	VF_SlopeShift						0.4f

#define ADCtoVolt							 (Vreff*(Rtop+Rbot)/(Resolution*Rbot)) * VF_SlopeScale

#define Max_Loop_Detect  	20
//////////3.3 lv2 3.5 lv1
#define max_cell 															4.5f
#define min_cell 															3.5f
///////////////////////////////////////////////////////////////

void Voltage_ScreenUpdate(void);
float Get_Voltage(void);
uint16_t Cell_Detect(void);
uint8_t Battery_Check(void);

