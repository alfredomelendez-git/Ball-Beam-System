#include <stdint.h>
#include "UART.h"
void UART0_Init(void)
{
    SYSCTL_RCGCUART_R |= 0x01;
    while((SYSCTL_PRUART_R & 0x01) == 0);

    SYSCTL_RCGCGPIO_R |= 0x01;
    while((SYSCTL_PRGPIO_R & 0x01) == 0);

    GPIO_PORTA_AHB_AFSEL_R |= 0x02;                 //Funcion alterna PA0 PA1
    GPIO_PORTA_AHB_DEN_R |= 0x02;                   //Habilitar funciones digitales
    GPIO_PORTA_AHB_PCTL_R |= 0x00000010;            //Habilitar UART en PA4 PA5

    /*************Configurar Baud Rate**************/
    UART0_CTL_R &= ~0x01;                               // UART0 disable
    UART0_IBRD_R = 8;                                 // Integer part Baud divider (115200)
    UART0_FBRD_R = 44;                                  // Decimal part Baud divider
    UART0_LCRH_R = (UART0_LCRH_R & ~0x60) | (0x03<<5);  // UART_0 WLEN 8 bits Word Length
    UART0_LCRH_R = (UART0_LCRH_R & ~0x10) | (0x1<<4);   // UART_0 FEN Tx & Rx FIFO buffers are enabled
    UART0_CC_R = 0x05;                                  // UART_0 CS Alternate clock source
    UART0_CTL_R |= 0x01;                                // UART_0 enable
}

void SysTick_OneShot(uint32_t delay){
    NVIC_ST_RELOAD_R = (delay & 0x00FFFFFF) - 17;                                   //  Programar el valor de cuenta del SysTick (4,000,000)
    NVIC_ST_CURRENT_R = 0;                                                          //  Limpiar el registro STCURRENT al escribirle un valor cualquiera
    NVIC_ST_CTRL_R = 0x01;                                           //  Habilitar el SysTick con fuente de reloj de 4MHz
    NVIC_ST_RELOAD_R = 0;                                                           //  Limpiar el registro RELOAD para que el SysTick se apague cuando termine la cuenta
    while ((NVIC_ST_CTRL_R & 0x010000) == 0) {}                           //  Espera a que termine la cuenta del SysTick
}

void UART0_DataTransmit(char *data)
{
    while ((UART0_FR_R & 0x080) == 0);
    //SysTick_OneShot(1000000);                                        //  Retardo con el SysTick
    uint8_t i;
    for(i=0;i<10;i++)
    {
        UART0_DR_R = data[i];
    }
    UART0_DR_R = 10;                                                   // Carga de un dato a la FIFO de transmisión(newline)
    UART0_DR_R = 13;                                                   // Carga de un dato a la FIFO de transmisión (retorno)

}

