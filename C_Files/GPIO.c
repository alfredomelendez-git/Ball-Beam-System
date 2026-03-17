#include <stdint.h>
#include "GPIO.h"

void PortF_Init(void)
{
    SYSCTL_RCGCGPIO_R |= 0x020;              //PortF clock enable
    while((SYSCTL_PRGPIO_R & 0x020) == 0);
    GPIO_PORTF_DIR_R &= ~0x0C;              //Port F2 & F3 as digital inputs
    GPIO_PORTF_PUR_R |= 0x0C;               //Port F2 & F3 Pull-Up Resistor enable
    GPIO_PORTF_DEN_R |= 0x0C;               //Port F2 & F3 digital functions enable
    GPIO_PORTF_IS_R &= ~0x0C;              //Port F2 & F3 interrupt by edge
    GPIO_PORTF_IBE_R &= ~0x0C;              //Port F2 & F3 one edge event
    GPIO_PORTF_IEV_R &= ~0x0C;              //Port F2 & F3 falling edge event enable
    GPIO_PORTF_ICR_R |= 0x0C;               //Interrupt flag cleared
    GPIO_PORTF_IM_R |= 0x0C;                //Interrupt for Port F2&F3 unmasked
    NVIC_PRI7_R = (NVIC_PRI7_R & 0xFF00FFFF) | (0x02 <<21);         //Set priority 2
    NVIC_EN0_R = 1 << (30 - 0);             //Interrupt PortF enable
}
