
#ifndef GPIO_H_
#define GPIO_H_

#define SYSCTL_RCGCGPIO_R       (*((volatile uint32_t *)0x400FE608))                /*  pp382   GPIO Run Mode Clock Gating Control */
#define SYSCTL_PRGPIO_R         (*((volatile uint32_t *)0x400FEA08))                /*  pp499   GPIO Peripheral Ready */
/*  Nested Vectored Interrupt Controller Registers (NVIC) */
#define NVIC_ST_CTRL_R          (*((volatile uint32_t *)0xE000E010))                /*  pp150   SysTick Control and Status Register */
#define NVIC_ST_RELOAD_R        (*((volatile uint32_t *)0xE000E014))                /*  pp152   SysTick Reload Value Register */
#define NVIC_ST_CURRENT_R       (*((volatile uint32_t *)0xE000E018))                /*  pp153   SysTick Current Value Register */
#define NVIC_EN0_R              (*((volatile uint32_t *)0xE000E100))                /*  pp154   Interrupt 0-31 Set Enable */

#define NVIC_PRI7_R            (*((volatile uint32_t *)0xE000E41C))                 /*  pp159   Interrupt 28-31 Priority */

//*************************PORT F*************************
#define GPIO_PORTF_DIR_R (*((volatile uint32_t *) 0X4005D400))
#define GPIO_PORTF_DEN_R (*((volatile uint32_t *) 0X4005D51C))
#define GPIO_PORTF_PUR_R (*((volatile uint32_t *) 0X4005D510))
#define GPIO_PORTF_DATA_R (*((volatile uint32_t *) (0x4005D000 | 0x0C << 2)))       //Port F2 & F3
#define GPIO_PORTF_IS_R (*((volatile uint32_t *) 0x4005D404))
#define GPIO_PORTF_IBE_R (*((volatile uint32_t *) 0x4005D408))
#define GPIO_PORTF_IEV_R (*((volatile uint32_t *) 0x4005D40C))
#define GPIO_PORTF_IM_R (*((volatile uint32_t *) 0x4005D410))
#define GPIO_PORTF_ICR_R (*((volatile uint32_t *) 0x4005D41C))
#define GPIO_PORTF_MIS_R (*((volatile uint32_t *) 0x4005D418))


void PortF_Init(void);

#endif /* GPIO_H_ */
