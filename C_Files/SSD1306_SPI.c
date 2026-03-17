#include <stdint.h>
#include <SSD1306_SPI.h>


void PortH_Init(void)
{
    SYSCTL_RCGCGPIO_R |= 0x080;                 //Port H clock enable
    while((SYSCTL_PRGPIO_R & 0x080) == 0);

    GPIO_PORTH_AHB_DIR_R |= 0x07;               //PortH[0,1] as outputs
    GPIO_PORTH_AHB_DEN_R |= 0x07;               //PortH[0,1] digital functions enable
}

void SSI0_Init(void){

    /* Habilitación e inicialización del QSSI.*/

    SYSCTL_RCGCSSI_R |= 0x01;                                          // SSI0 clock enable
    while ((SYSCTL_PRSSI_R & 0x01) == 0) {};                           // Waits for clock stabilize

    SYSCTL_RCGCGPIO_R |= 0x01;                                        //  PortA clock enable
    while ((SYSCTL_PRGPIO_R & 0x01) == 0) {};                         //  Waits for clock stabilize

    GPIO_PORTA_AHB_AFSEL_R |= 0x3C;                                                 //  PortA[5:2] alt function enable

    GPIO_PORTA_AHB_PCTL_R = (GPIO_PORTA_AHB_PCTL_R & ~0x00F00000) | (15 << 20);     //  PortA[5] SSI0SSI0XDAT0 -> MISO
    GPIO_PORTA_AHB_PCTL_R = (GPIO_PORTA_AHB_PCTL_R & ~0x000F0000) | (15 << 16);     //  PortA[4] SSI0SSI0XDAT0 -> MOSI
    GPIO_PORTA_AHB_PCTL_R = (GPIO_PORTA_AHB_PCTL_R & ~0x0000F000) | (15 << 12);     //  PortA[3] SSI0Fss
    GPIO_PORTA_AHB_PCTL_R = (GPIO_PORTA_AHB_PCTL_R & ~0x00000F00) | (15 << 8);      //  PortA[2] SSI0Clk

    GPIO_PORTA_AHB_DEN_R |= 0x3C;                                                   //  PortA[4:2] enable digital functions
    //GPIO_PORTA_AHB_PUR_R |= 0x04;                                                   // Port A2 Pull-up resistor enable

    // 1. if initialize after RESET, make sure SSE (SSICR1) is '0' before changing to config.
    SSI0_CR1_R &= ~0x02;                                                      //  QSSI_0 operation Disabled

    SSI0_CR1_R &= ~0x04;                                                      //  QSSI_0 is configured as a master

    SSI0_CC_R = 0x00000000;                                                   //  QSSI_0 => System clock (based on clock source and divisor
                                                                              //factor programmed in RSCLKCFG)
    SSI0_CPSR_R = 0x04;                 //clock prescale config

    // 5. Seleccionar la configuración adecuada (SSICR0).
        // Para el cálculo de SCR:
            // SSInClk = SysClk / (CPSDVSR * (1 + SCR))
            // Asumiendo:   SSInClk = 4,000,000
            //              SysClk = 16,000,000
            //              CPSDVSR = 2
            // 4,000,000 = 16,000,000 / (2 * (1 + SCR))
            // Por lo tanto, SCR = 1
        // SCR = 1  => Tasa de reloj serial.
        // SPH = 0  => Fase del reloj -> Data is captured on the first clock edge transition.
        // SPO = 0  => Polaridad del reloj -> A steady state Low value is placed on the SSInClk pin.
        // FRF = 0  => Selección del formato de la trama -> Freescale SPI Frame Format
        // DSS = 7 => Selección del tamaño de los datos -> 8 bits
    SSI0_CR0_R = (0x0100 | 0x07) & ~(0x30 & 0x80 & 0x40);
    SSI0_CR1_R |= 0x02;                      // If first initialization after reset enable QSSI
    GPIO_PORTH_AHB_DATA_R |= 0x02;            //CS = 1
}


void SSI0_sendCommand (uint8_t command)
{
    //CS -> portH0  DC -> portH1
    GPIO_PORTH_AHB_DATA_R &= ~0x03;                           //CS = 0, DC = 0
    while ((SSI0_SR_R & 0x010) != 0);                       // SSI0 Waits bus until inactive status
    SSI0_DR_R = command;                                    // Load to transmission FIFO
    while((SSI0_SR_R & 0x010) != 0);
    GPIO_PORTH_AHB_DATA_R |= 0x01;                           //CS = 1 end transmission
}


void SSI0_sendData (uint8_t data)
{
    GPIO_PORTH_AHB_DATA_R &= ~0x01 ;                          //CS = 0, DC = 1
    GPIO_PORTH_AHB_DATA_R |= 0x02 ;
    while((SSI0_SR_R & 0x010) != 0);
    SSI0_DR_R = data;
    while((SSI0_SR_R & 0x010) != 0);
    GPIO_PORTH_AHB_DATA_R |= 0x01;                           //CS = 1 end transmission
}

void OLED_reset(void)
{
    uint8_t i;
    GPIO_PORTH_AHB_DATA_R &= ~0x04;       //Reset LOW
    for(i=0;i<100;i++);
    GPIO_PORTH_AHB_DATA_R |= 0x04;
    for(i=0;i<100;i++);
}

void OLED_init(void)
{
    SSI0_sendCommand(0xAE);             //Display off
    SSI0_sendCommand(0xA8);             //Set multiple ratio (height)
    SSI0_sendCommand(0x3F);             //Set 64 height
    SSI0_sendCommand(0x81);             //Set contrast
    SSI0_sendCommand(0x7F);             //Default contrast
    SSI0_sendCommand(0xA1);             //Segment remap,flip Horizontal
    SSI0_sendCommand(0xC8);             //Com scan direction
    SSI0_sendCommand(0xD3);             //Set display offset
    SSI0_sendCommand(0x00);             //No offset
    SSI0_sendCommand(0xD5);             //Set display clock divide ratio/osc freq
    SSI0_sendCommand(0x80);             //Default frequency
    SSI0_sendCommand(0xD9);             //Set pre-charge period
    SSI0_sendCommand(0x22);             //Pre-charge period
    SSI0_sendCommand(0xDB);             //Set VCOM select
    SSI0_sendCommand(0x20);             //Desired VCOM
    SSI0_sendCommand(0x8D);             //Charge Pump
    SSI0_sendCommand(0x14);             //Charge enable
    SSI0_sendCommand(0xAF);             //Display on
}



void OLED_CursorSet(uint8_t x,uint8_t y)
{
    SSI0_sendCommand(0xB0 | y);                 //page config (0xB-0xB7)
    SSI0_sendCommand(0x00 | (x & 0x0F));        //Low column
    SSI0_sendCommand(0x10 | ((x>>4)& 0x0F));    //High column
}



void OLED_Clear(void) {
    uint8_t page,column;
    for (page = 0; page < 8; page++) {
        // Selecciona la página
        OLED_CursorSet(0, page); // Columna 0, página actual

        // Llena todas las columnas de la página
        for (column = 0; column < 128; column++) {
            SSI0_sendData(0x00); // Send 0x00 to shut down
        }
    }
}
void OLED_8x16_P(uint8_t x, uint8_t page, const uint8_t *charMap) {
    uint8_t i;

    // Print superior half
    OLED_CursorSet(x, page);
    for (i = 0; i < 8; i++) {
        SSI0_sendData(charMap[i]);
    }

    // Print inferior half
    OLED_CursorSet(x, page + 1);
    for (i = 8; i < 16; i++) {
        SSI0_sendData(charMap[i]);
    }
}
