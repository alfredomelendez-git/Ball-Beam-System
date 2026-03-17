#include <stdint.h>
#include <ADC.h>

/**
 * Definición de los apuntadores para acceso a memoria
 */
    // Definir los apuntadores necesarios para el código
    /*  System Control Registers (SYSCTL) */
#define SYSCTL_PLLFREQ0_R       (*((volatile uint32_t *)0x400FE160))                /*  PLL Frequency 0 */
#define SYSCTL_PLLSTAT_R        (*((volatile uint32_t *)0x400FE168))                /*  PLL Status */
#define SYSCTL_RCGCGPIO_R       (*((volatile uint32_t *)0x400FE608))                /*  GPIO Run Mode Clock Gating Control */
#define SYSCTL_RCGCADC_R        (*((volatile uint32_t *)0x400FE638))                //   ADC Run Mode Clock Gating Control */
#define SYSCTL_PRGPIO_R         (*((volatile uint32_t *)0x400FEA08))                //   GPIO Peripheral Ready */
#define SYSCTL_PRADC_R          (*((volatile uint32_t *)0x400FEA38))                //   ADC Peripheral Ready */

/************************ADC REGISTERS********************/
#define ADC1_ACTSS_R    (*((volatile uint32_t *)0x40039000))
#define ADC1_RIS_R      (*((volatile uint32_t *)0x40039004))                //  ADC Raw Interrupt Status
#define ADC1_ISC_R      (*((volatile uint32_t *)0x4003900C))                //  ADC Interrupt Status and Clear
#define ADC1_EMUX_R     (*((volatile uint32_t *)0x40039014))                //  ADC Event Multiplexer Select
#define ADC1_PSSI_R     (*((volatile uint32_t *)0x40039028))                //  ADC Processor Sample Sequence Initiate
#define ADC1_SSMUX1_R   (*((volatile uint32_t *)0x40039060))                //  ADC Sample Sequence Input Multiplexer Select 1
#define ADC1_SSCTL1_R   (*((volatile uint32_t *)0x40039064))                //  ADC Sample Sequence Control 1
#define ADC1_SSFIFO1_R  (*((volatile uint32_t *)0x40039068))                //  ADC Sample Sequence Result FIFO 1
#define ADC1_SSEMUX1_R  (*((volatile uint32_t *)0x40039078))                //  ADC Sample Sequence Extended Input Multiplexer Select 1

/*----------------------GPIO REGISTERS----------------------*/
//PortE
#define GPIO_PORTE_DIR_R (*((volatile uint32_t *) 0X4005C400))
#define GPIO_PORTE_DEN_R (*((volatile uint32_t *) 0X4005C51C))
#define GPIO_PORTE_AFSEL_R (*((volatile uint32_t *) 0X4005C420))
#define GPIO_PORTE_AMSEL_R (*((volatile uint32_t *) 0X4005C528))


#define SYSCTL_PLLFREQ0_PLLPWR  0x00800000                                          /*  PLL Power */

void ADC1_Init(void){
        SYSCTL_RCGCADC_R |= 02;
    while((SYSCTL_PRADC_R & 0x02 ) == 0);

    SYSCTL_RCGCGPIO_R |= 0x10;
    while((SYSCTL_PRGPIO_R & 0x10) == 0);
    GPIO_PORTE_AFSEL_R |= 0x07;                 //alternate function Port E[0:2]
    GPIO_PORTE_DEN_R &= ~0x07;                  // Port E[0:2] digital functions disable
    GPIO_PORTE_AMSEL_R |= 0x07;                 // Analog/Digital isolation Port E[0:2] disabled

        ADC1_ACTSS_R &= ~0x02;                  // Sample sequencer disabled
        ADC1_EMUX_R = 0x00;                     // Trigger event by software

        ADC1_SSMUX1_R |= 0x0132;                // Channel 1,2,3 -> 4 control bits {0000 0011 0010 0001}
       ADC1_SSEMUX1_R |= 0x0000;                //0000 channels 1-15 enable

       ADC1_SSCTL1_R |= 0x0644;                 // Sequencer configuration
       ADC1_ACTSS_R |= 0X02;                    // Sample sequencer enable

       SYSCTL_PLLFREQ0_R |= (0x1 <<23) ;        // PLL enable (clock sync)
       while((SYSCTL_PLLSTAT_R & 0x01) == 0);   // Wait until Peripherical sync
       SYSCTL_PLLFREQ0_R &= ~ (0x01 <<23);      // PLL disable
}

void ADC_read(uint32_t* X)
{
    ADC1_PSSI_R = 0x02;                 // SS1 initialize
    while((ADC1_RIS_R & 0x02) == 0);    //wait until sequence process ends
    // Read all values
    X[0] = ADC1_SSFIFO1_R;
    X[1] = ADC1_SSFIFO1_R;
    X[2] = ADC1_SSFIFO1_R;
    ADC1_ISC_R |= 0x0002;               // Clean IN flag & INR flag
}
