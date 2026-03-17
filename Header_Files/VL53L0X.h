#ifndef VL53L0X_H_
#define VL53L0X_H_

void I2C0_Init(void);
int I2C0_MasterBusy();
void VL53L0X_Config(uint8_t Address);
void VL53L0X_Start(uint8_t Address);
void I2C0_TransmitS(uint8_t Address, uint8_t Reg, uint8_t Data);
uint8_t I2C0_ReceiveS(uint8_t Address, uint8_t Reg);
float I2C0_Receive(uint8_t Address);
#endif /* VL53L0X_H_ */

