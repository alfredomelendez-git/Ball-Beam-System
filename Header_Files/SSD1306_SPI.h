#include <stdint.h>

#ifndef SSD1306_SPI_H_
#define SSD1306_SPI_H_

    /*  Nested Vectored Interrupt Controller Registers (NVIC) */                    /*  pp146   Register map */
#define NVIC_ST_CTRL_R          (*((volatile uint32_t *)0xE000E010))                /*  pp150   SysTick Control and Status Register */
#define NVIC_ST_RELOAD_R        (*((volatile uint32_t *)0xE000E014))                /*  pp152   SysTick Reload Value Register */
#define NVIC_ST_CURRENT_R       (*((volatile uint32_t *)0xE000E018))                /*  pp153   SysTick Current Value Register */

    /*  System Control Registers (SYSCTL) */                                        /*  pp247   REGISTER MAP */
#define SYSCTL_RCGCGPIO_R       (*((volatile uint32_t *)0x400FE608))                /*  pp382   GPIO Run Mode Clock Gating Control */
#define SYSCTL_RCGCSSI_R        (*((volatile uint32_t *)0x400FE61C))                /*  pp390   SSI Run Mode Clock Gating Control */
#define SYSCTL_PRGPIO_R         (*((volatile uint32_t *)0x400FEA08))                /*  pp499   GPIO Peripheral Ready */
#define SYSCTL_PRSSI_R          (*((volatile uint32_t *)0x400FEA1C))                /*  pp507   SSI Peripheral Ready */

    /*  SSI Registers (SSI_0) */                                                    /*  pp1243  REGISTER MAP */
#define SSI0_CR0_R              (*((volatile uint32_t *)0x40008000))                /*  pp1245  QSSI Control 0 */
#define SSI0_CR1_R              (*((volatile uint32_t *)0x40008004))                /*  pp1247  QSSI Control 1 */
#define SSI0_DR_R               (*((volatile uint32_t *)0x40008008))                /*  pp1249  QSSI Data */
#define SSI0_SR_R               (*((volatile uint32_t *)0x4000800C))                /*  pp1250  QSSI Status */
#define SSI0_CPSR_R             (*((volatile uint32_t *)0x40008010))                /*  pp1252  QSSI Clock Prescale */
#define SSI0_IM_R               (*((volatile uint32_t *)0x40008014))                /*  pp1253  QSSI Interrupt Mask */
#define SSI0_RIS_R              (*((volatile uint32_t *)0x40008018))                /*  pp1255  QSSI Raw Interrupt Status */
#define SSI0_MIS_R              (*((volatile uint32_t *)0x4000801C))                /*  pp1257  QSSI Masked Interrupt Status */
#define SSI0_ICR_R              (*((volatile uint32_t *)0x40008020))                /*  pp1259  QSSI Interrupt Clear */
#define SSI0_DMACTL_R           (*((volatile uint32_t *)0x40008024))                /*  pp1260  QSSI DMA Control */
#define SSI0_PP_R               (*((volatile uint32_t *)0x40008FC0))                /*  pp1261  QSSI Peripheral Properties */
#define SSI0_CC_R               (*((volatile uint32_t *)0x40008FC8))                /*  pp1262  QSSI Clock Configuration */

    /*  GPIO Registers (PortA) */                                                   /*  pp757   REGISTER MAP */
#define GPIO_PORTA_AHB_AFSEL_R  (*((volatile uint32_t *)0x40058420))                /*  pp770   GPIO Alternate Function Select */
#define GPIO_PORTA_AHB_DEN_R    (*((volatile uint32_t *)0x4005851C))                /*  pp781   GPIO Digital Enable */
#define GPIO_PORTA_AHB_PCTL_R   (*((volatile uint32_t *)0x4005852C))                /*  pp787   GPIO Port Control */
#define GPIO_PORTA_AHB_PUR_R    (*((volatile uint32_t *)0x40058510))

    /*  GPIO Registers (PortH) */                                                   /*  pp757   Register map */
#define GPIO_PORTH_AHB_DATA_R   (*((volatile uint32_t *)(0x4005F000 | 0x07 << 2)))  /*  pp759   GPIO Data */
#define GPIO_PORTH_AHB_DIR_R    (*((volatile uint32_t *)0x4005F400))                /*  pp760   GPIO Direction */
#define GPIO_PORTH_AHB_DEN_R    (*((volatile uint32_t *)0x4005F51C))                /*  pp781   GPIO Digital Enable */


void PortH_Init(void);
void SSI0_Init(void);
void SSI0_sendCommand (uint8_t command);
void SSI0_sendData (uint8_t data);
void OLED_CursorSet(uint8_t x,uint8_t y);
void OLED_Clear(void);
void OLED_8x16_P(uint8_t x, uint8_t page, const uint8_t *charMap);
void OLED_reset(void);
void OLED_init(void);




#endif /* SSD1306_SPI_H_ */
