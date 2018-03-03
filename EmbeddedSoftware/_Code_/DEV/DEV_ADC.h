#include "BoardConfig.h"

#define ADC_Average												1
#define ADC_BuffSize											5*ADC_Average
#define ADC_CoversionSize									5*ADC_Average
#define ADCX															ADC3

void ADC_Iinitialize(void);
uint16_t ADC_Get(uint8_t INDEX, uint8_t size);


