#include <stdint.h>
#include <Servo.h>

    /*  Nested Vectored Interrupt Controller Registers (NVIC) */
#define NVIC_ST_CTRL_R          (*((volatile uint32_t *)0xE000E010))
#define NVIC_ST_RELOAD_R        (*((volatile uint32_t *)0xE000E014))
#define NVIC_ST_CURRENT_R       (*((volatile uint32_t *)0xE000E018))
#define NVIC_EN1_R              (*((volatile uint32_t *)0xE000E104))

    /*  System Control Registers (SYSCTL) */
#define SYSCTL_RCGCTIMER_R      (*((volatile uint32_t *)0x400FE604))
#define SYSCTL_RCGCGPIO_R       (*((volatile uint32_t *)0x400FE608))
#define SYSCTL_PRTIMER_R        (*((volatile uint32_t *)0x400FEA04))
#define SYSCTL_PRGPIO_R         (*((volatile uint32_t *)0x400FEA08))

    /*  Timer registers (TIMER2) */
#define TIMER2_CFG_R            (*((volatile uint32_t *)0x40032000))
#define TIMER2_TAMR_R           (*((volatile uint32_t *)0x40032004))
#define TIMER2_CTL_R            (*((volatile uint32_t *)0x4003200C))
#define TIMER2_TAILR_R          (*((volatile uint32_t *)0x40032028))
#define TIMER2_TAMATCHR_R       (*((volatile uint32_t *)0x40032030))
#define TIMER2_TAPR_R           (*((volatile uint32_t *)0x40032038))


    /*  GPIO Registers (PortM)*/
#define GPIO_PORTM_DIR_R        (*((volatile uint32_t *)0x40063400))
#define GPIO_PORTM_AFSEL_R      (*((volatile uint32_t *)0x40063420))
#define GPIO_PORTM_DEN_R        (*((volatile uint32_t *)0x4006351C))
#define GPIO_PORTM_PCTL_R       (*((volatile uint32_t *)0x4006352C))


void GPTM2A_Init(void){
    SYSCTL_RCGCTIMER_R |= 0x04;                                                     // GPTM_2 clock enable
    while ((SYSCTL_PRTIMER_R & 0x04) == 0) {};                                      //  Wait for GPTM_2 clock to stabilize

    SYSCTL_RCGCGPIO_R |= 0x0800;                                                    // Clock PortM enable
    while ((SYSCTL_PRGPIO_R & 0x0800) == 0) {};                                     // Wait for PortM clock to stabilize

    GPIO_PORTM_DIR_R |= 0x01;                                                       // PortM[0] => Output
    GPIO_PORTM_AFSEL_R |= 0x01;                                                     //  PortM[0] => Alternate hardware function -> Enable
    GPIO_PORTM_PCTL_R |= 0x00000003;                                               //  PortM[0] => GPIO controlled by the function T2CCP0
    GPIO_PORTM_DEN_R |= 0x01;                                                       //  PortM[0] => Digital functions -> Enabled

    TIMER2_CTL_R &= ~0x01;                                                          //  TIMER_2 => Timer A -> TAEN -> Timer disabled

    TIMER2_CFG_R = 0x4;                                                             //  TIMER_2 => GPTMCFG -> 16-bit timer configuration

    TIMER2_TAMR_R = (TIMER2_TAMR_R & ~0x08) | (0x1 << 3);                           //  TIMER_2 => Timer A -> TAAMS -> PWM mode enabled
    TIMER2_TAMR_R = (TIMER2_TAMR_R & ~0x04) | (0x0 << 2);                           //  TIMER_2 => Timer A -> TACMR -> Edge-Count mode
    TIMER2_TAMR_R = (TIMER2_TAMR_R & ~0x03) | (0x2 << 0);                           //  TIMER_2 => Timer A -> TAMR -> Periodic timer mode

    TIMER2_CTL_R &= ~0x0040;                                                        //  TIMER_2 => Timer A -> TAPWML -> PWM Output Level -> Unaffected

    TIMER2_TAPR_R = ((320000 & 0x00FF0000) >>16);                                                           // Pre scale value from 4 E200 (50Hz)
    TIMER2_TAILR_R = (320000 & 0x0000FFFF);                                                       // Non scalable value from 4 E2000  (50Hz)
    TIMER2_TAMATCHR_R = (160000);                                                     // Duty cycle 50%

    TIMER2_CTL_R |= 0x01;                                                           //  TIMER_2 => Timer A -> TAEN -> Timer enabled
}

void Servo_write(float DutyC)
{
    uint32_t val;
    val = (int)((DutyC/100.0)*320000);
    TIMER2_TAMATCHR_R = val;
}
