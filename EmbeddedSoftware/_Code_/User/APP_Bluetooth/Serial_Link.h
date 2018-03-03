
#include "Serial_Link_string.h"




char *byte_to_binary(int x, uint8_t size);

void SerialLink_INPUT_INTERRUPT (uint16_t INPUT);

void SerialLink_DMATX_Process (void);
void SerialLink_INPUT_Process (void);

void Initialize_SerialLink (uint16_t baud);
void SerialLink_SetBaud (uint16_t baud);
