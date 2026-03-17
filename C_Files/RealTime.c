#include <RealTime.h>

void GPTM1_Init(void){

    SYSCTL_RCGCTIMER_R |= 0x02;                         //GPTM1 clock enable
    while ((SYSCTL_PRTIMER_R & 0x02) == 0);
    TIMER1_CTL_R &= ~0x01;                              //GPTM1 disable

    TIMER1_CFG_R = 0x04;                                //Timer1A 16-bit timer configuration
    TIMER1_TAMR_R = (TIMER1_TAMR_R & ~0x03) | 0x02;     //TIMER_1Timer A Continuous mode timer mode
        // 6. Cargar el valor inicial del temporizador en el registro GPTMTnILR.
    TIMER1_TAILR_R = 0x3880;                            //Temporizer initial value  (5ms)
    TIMER1_TAPR_R = 0x01;                               //Preescaler value
    NVIC_PRI5_R = (NVIC_PRI5_R & 0xFFFF00FF) | (0x01  <<13);      //Set priority to 1
    NVIC_EN0_R |= (1 << 21);                            //Enable interruption
    TIMER1_IMR_R |= 0x01;                               //TimeOut Interrupt Mask
    TIMER1_ICR_R = 0x01;                                //Interruption flag cleared

    TIMER1_CTL_R |= 0x01;                               //GPTM1A enable

}
