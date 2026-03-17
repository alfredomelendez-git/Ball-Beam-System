
#ifndef UART_H_
#define UART_H_

    /*  Nested Vectored Interrupt Controller Registers (NVIC) */                    /*  pp146   REGISTER MAP */
#define NVIC_ST_CTRL_R          (*((volatile uint32_t *)0xE000E010))                /*  pp150   SysTick Control and Status Register */
#define NVIC_ST_RELOAD_R        (*((volatile uint32_t *)0xE000E014))                /*  pp152   SysTick Reload Value Register */
#define NVIC_ST_CURRENT_R       (*((volatile uint32_t *)0xE000E018))                /*  pp153   SysTick Current Value Register */

    /*  System Control Registers (SYSCTL) */                                        /*  pp247   REGISTER MAP */
#define SYSCTL_RCGCGPIO_R       (*((volatile uint32_t *)0x400FE608))                /*  pp382   GPIO Run Mode Clock Gating Control */
#define SYSCTL_RCGCUART_R       (*((volatile uint32_t *)0x400FE618))                /*  pp388   UART Run Mode Clock Gating Control */
#define SYSCTL_PRGPIO_R         (*((volatile uint32_t *)0x400FEA08))                /*  pp499   GPIO Peripheral Ready */
#define SYSCTL_PRUART_R         (*((volatile uint32_t *)0x400FEA18))                /*  pp505   UART Peripheral Ready */

    /*  UART Registers (UART_0) */                                                  /*  pp1173  REGISTER MAP */
#define UART0_DR_R              (*((volatile uint32_t *)0x4000C000))                /*  pp1175  UART Data */
#define UART0_FR_R              (*((volatile uint32_t *)0x4000C018))                /*  pp1180  UART Flag */
#define UART0_IBRD_R            (*((volatile uint32_t *)0x4000C024))                /*  pp1184  UART Integer Baud-Rate Divisor */
#define UART0_FBRD_R            (*((volatile uint32_t *)0x4000C028))                /*  pp1185  UART Fractional Baud-Rate Divisor */
#define UART0_LCRH_R            (*((volatile uint32_t *)0x4000C02C))                /*  pp1186  UART Line Control */
#define UART0_CTL_R             (*((volatile uint32_t *)0x4000C030))                /*  pp1188  UART Control */
#define UART0_CC_R              (*((volatile uint32_t *)0x4000CFC8))                /*  pp1213  UART Clock Configuration */

    /*  GPIO Registers (PortA) */                                                   /*  pp757   REGISTER MAP */
#define GPIO_PORTA_AHB_AFSEL_R  (*((volatile uint32_t *)0x40058420))                /*  pp770   GPIO Alternate Function Select */
#define GPIO_PORTA_AHB_DEN_R    (*((volatile uint32_t *)0x4005851C))                /*  pp781   GPIO Digital Enable */
#define GPIO_PORTA_AHB_PCTL_R   (*((volatile uint32_t *)0x4005852C))                /*  pp787   GPIO Port Control */


void UART0_Init(void);
void SysTick_OneShot(uint32_t delay);
void UART0_DataTransmit(char *data);

#endif /* UART_H_ */
