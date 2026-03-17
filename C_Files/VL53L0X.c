#include <stdint.h>
#include "VL53L0X.h"

#define SYSCTL_RCGCI2C_R            (*((volatile uint32_t *) 0x400FE620))
#define SYSCTL_RCGCGPIO_R           (*((volatile uint32_t *) 0x400FE608))
#define SYSCTL_PRI2C_R              (*((volatile uint32_t *) 0x400FEA20))
#define SYSCTL_PRGPIO_R             (*((volatile uint32_t *) 0x400FEA08))

#define GPIO_PORTB_AHB_AFSEL_R      (*((volatile uint32_t *) 0x40059420))
#define GPIO_PORTB_AHB_PCTL_R       (*((volatile uint32_t *) 0x4005952C))
#define GPIO_PORTB_AHB_DIR_R        (*((volatile uint32_t *) 0x40059400))
#define GPIO_PORTB_AHB_DEN_R        (*((volatile uint32_t *) 0x4005951C))
#define GPIO_PORTB_AHB_ODR_R        (*((volatile uint32_t *) 0x4005950C))

#define I2C0_MCR_R                  (*((volatile uint32_t *) 0x40020020))
#define I2C0_MTPR_R                 (*((volatile uint32_t *) 0x4002000C))
#define I2C0_MRIS_R                 (*((volatile uint32_t *) 0x40020014))
#define I2C0_MICR_R                 (*((volatile uint32_t *) 0x4002001C))
#define I2C0_MCS_R                  (*((volatile uint32_t *) 0x40020004))
#define I2C0_MSA_R                  (*((volatile uint32_t *) 0x40020000))
#define I2C0_MDR_R                  (*((volatile uint32_t *) 0x40020008))


#define RCGCI2C0    0x01
#define RCGCPORTB   0x02

void I2C0_Init(void)
{

    SYSCTL_RCGCI2C_R |= RCGCI2C0;
    while((SYSCTL_PRI2C_R & RCGCI2C0) == 0);          //enable clock I2C0

    SYSCTL_RCGCGPIO_R |= RCGCPORTB;
    while((SYSCTL_PRGPIO_R & RCGCPORTB) == 0);      //enable clock port B

    GPIO_PORTB_AHB_AFSEL_R |= 0x0C;                 //enable alternate function port B[2,3]
    GPIO_PORTB_AHB_ODR_R |= 0x08;                   //enable open drain for port B[2,3]
    GPIO_PORTB_AHB_DEN_R |= 0x0C;                   //Port[2,3]digital functions enable
    GPIO_PORTB_AHB_DIR_R |= 0x0C;                   //Port[2,3] as digital outputs
    GPIO_PORTB_AHB_PCTL_R |= 0x00002200;            //Port B[2,3] as I2C

    I2C0_MCR_R = 0x10;                              //I2C0 as Master mode
    I2C0_MTPR_R = 7;                                //Set speed as 100kbps
}

int I2C0_MasterBusy()
{
    int error = 0;
    while((I2C0_MRIS_R & 0x01) == 0);               //while master interrupt has not occurred
    I2C0_MICR_R |= 0x01;                            //clear master interrupt (RIS bit)
    if((I2C0_MCS_R & 0x02) == 0x02)                 //
    {
        error=1;                                    //transmission error
    }
    return error;
}

void I2C0_TransmitS(uint8_t Address, uint8_t Reg, uint8_t Data)
{
    I2C0_MSA_R = (Address <<1) & 0xFE;              //send Slave Address
    I2C0_MDR_R = Reg;                               //sends Register address
    I2C0_MCS_R = 0x02 | 0x01;                       //start & run conditions (Master transmit mode)
    I2C0_MasterBusy();
    I2C0_MDR_R = Data;
    I2C0_MCS_R = 0x04 | 0x01;                   //Stop & run condition
    I2C0_MasterBusy();
}

uint8_t I2C0_ReceiveS(uint8_t Address, uint8_t Reg)
{
    uint8_t dataR;
    while ((I2C0_MCS_R & 0x01) == 0x01);              //  I2C_0 => ¿Idle?
    //Master single transmit
    I2C0_MSA_R = (Address<<1) & 0xFE;                 //Send slave address
    I2C0_MDR_R = Reg;                                //Measurements register
    I2C0_MCS_R = 0x02 | 0x01;                         //Start & Run conditions (Master transmit mode)
    I2C0_MasterBusy();

    //Master data reception
    I2C0_MSA_R = (Address << 1) | 0x01;               //Send Address + ACK
    I2C0_MCS_R = 0x08 | 0x02 | 0x01;                    // ACK+Start+Run conditions (ready to receive data)
    I2C0_MasterBusy();

    dataR = (I2C0_MDR_R & 0xFF);
    I2C0_MCS_R = 0x04 | 0x01;
    I2C0_MasterBusy();
    return dataR;
    }
void VL53L0X_Config(uint8_t Address)
{
    I2C0_TransmitS(Address,0x89,0x01);
    I2C0_TransmitS(Address,0x88,0x00);
    I2C0_TransmitS(Address,0x80,0x01);
    I2C0_TransmitS(Address,0xFF,0x01);
    I2C0_TransmitS(Address,0x00,0x00);
    I2C0_ReceiveS(Address,0x91);
    I2C0_TransmitS(Address,0x91,0x3C);
    I2C0_TransmitS(Address,0x00,0x01);
    I2C0_TransmitS(Address,0xFF,0x00);
    I2C0_TransmitS(Address,0x80,0x00);
    I2C0_TransmitS(Address,0x8A,0x29);
    I2C0_ReceiveS(Address,0xC2);
    I2C0_ReceiveS(Address,0xC0);
    I2C0_TransmitS(Address,0x09,0x01);
    }



void VL53L0X_Start(uint8_t Address)
{
    I2C0_TransmitS(Address,0x80,0x01);
    I2C0_TransmitS(Address,0xFF,0x01);
    I2C0_TransmitS(Address,0x00,0x00);
    I2C0_TransmitS(Address,0x91,0x3C);
    I2C0_TransmitS(Address,0x00,0x01);
    I2C0_TransmitS(Address,0xFF,0x00);
    I2C0_TransmitS(Address,0x80,0x00);
    I2C0_TransmitS(Address,0x00,0x02);
}
uint8_t received_data[12];
float I2C0_Receive(uint8_t Address)
{
    while ((I2C0_MCS_R & 0x01) == 0x01);              //  I2C_0 => ¿Idle?
    //Master single transmit
    I2C0_MSA_R = (Address<<1) & 0xFE;                 //Send slave address
    I2C0_MDR_R = 0x14;                                //Measurements register
    I2C0_MCS_R = 0x02 | 0x01;                         //Start & Run conditions (Master transmit mode)
    I2C0_MasterBusy();

    //Master data reception
    I2C0_MSA_R = (Address << 1) | 0x01;               //Send Address + ACK
    I2C0_MCS_R = 0x08 | 0x02 | 0x01;                    // ACK+Start+Run conditions (ready to receive data)
    I2C0_MasterBusy();

    received_data[0] = (I2C0_MDR_R & 0xFF);

    int i;
    for(i=1;i<11;i++)
    {
        I2C0_MCS_R = 0x08 | 0x01;                     //ACK & RUN conditions Remains in Master Slave mode
        I2C0_MasterBusy();
        received_data[i] = (I2C0_MDR_R & 0xFF);                //Reads data from Register
    }

    I2C0_MCS_R = 0x08 | 0x01;                         //ACK & RUN conditions Remains in Master Reception mode
    I2C0_MasterBusy();
    received_data[11] = (I2C0_MDR_R & 0xFF);                   //Read last data from Register
    I2C0_MCS_R = 0x04 | 0x01;                         //Stop & run conditions (go to Idle)
    I2C0_MasterBusy();

    //Get distance data from raw data
    uint16_t Dist;
    Dist = ((received_data[10]&0xFF)<<8) | (received_data[11]&0xFF);    //Raw data conversion
    float Result;
    Result = Dist/4.0;                                //Divides per 4 when precision mode selected
    return Result;
}
