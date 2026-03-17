#ifndef REALTIME_H_
#define REALTIME_H_

#include <stdint.h>                                                                 /*  Definición de los tamaños de tipo entero */

    /*  Nested Vectored Interrupt Controller Registers (NVIC) */                    /*  pp146   REGISTER MAP */
#define NVIC_EN0_R              (*((volatile uint32_t *)0xE000E100))                /*  pp154   Interrupt 0-31 Set Enable */
#define NVIC_PRI5_R             (*((volatile uint32_t *)0xE000E414))                /*  pp159   Interrupt 20-23 Priority */

    /*  System Control Registers (SYSCTL) */                                        /*  pp247   REGISTER MAP */
#define SYSCTL_RCGCTIMER_R      (*((volatile uint32_t *)0x400FE604))                /*  pp380   16/32-Bit GPTM Run Mode Clock Gating Control */
#define SYSCTL_PRTIMER_R        (*((volatile uint32_t *)0x400FEA04))                /*  pp497   16/32-Bit GPTM Peripheral Ready */


    /*  Timer registers (TIMER0) */                                                 /*  pp974   REGISTER MAP */
#define TIMER1_CFG_R            (*((volatile uint32_t *)0x40031000))                /*  pp976   GPTM Configuration */
#define TIMER1_TAMR_R           (*((volatile uint32_t *)0x40031004))                /*  pp977   GPTM Timer A Mode */
#define TIMER1_CTL_R            (*((volatile uint32_t *)0x4003100C))                /*  pp986   GPTM Control */
#define TIMER1_RIS_R            (*((volatile uint32_t *)0x4003101C))                /*  pp996   GPTM Raw Interrupt Status */
#define TIMER1_ICR_R            (*((volatile uint32_t *)0x40031024))                /*  pp1002  GPTM Interrupt Clear */
#define TIMER1_TAILR_R          (*((volatile uint32_t *)0x40031028))                /*  pp1004  GPTM Timer A Interval Load */
#define TIMER1_TAPR_R           (*((volatile uint32_t *)0x40031038))                /*  Pre scale A*/
#define TIMER1_IMR_R            (*((volatile uint32_t *)0x40031018))                /* timer1 interrupt*/

void GPTM1_Init(void);
#endif /* REALTIME_H_ */
