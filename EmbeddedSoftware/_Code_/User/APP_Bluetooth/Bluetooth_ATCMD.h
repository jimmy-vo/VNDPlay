#include "BoardConfig.h"




char* Bluetooth_AT_GetInfo(void);
uint8_t Bluetooth_AT_SetupUART(uint32_t BAUDRATE, uint16_t STOPBIT, uint16_t PARITY);
uint8_t Bluetooth_AT_SetName(char *Name);
uint8_t Bluetooth_AT_SetPasskey(char *Passkey);
uint8_t Bluetooth_AT_SetRole(uint8_t Role);
uint8_t Bluetooth_AT_SetMode(uint8_t Mode);
uint8_t Bluetooth_AT_Scan(uint8_t timeout);
uint8_t Bluetooth_AT_GetDeviceName(uint8_t device);
uint8_t Bluetooth_AT_DeviceCheckAuthenticated(uint8_t device);
uint8_t Bluetooth_AT_DevicePair(uint8_t device, uint8_t timeout);
uint8_t Bluetooth_AT_DeviceBind(uint8_t device);
uint8_t Bluetooth_AT_DeviceLink(uint8_t device);











