/* Board Support Package (BSP) for the EK-TM4C123GXL board */
#include <stdint.h>  /* Standard integers. WG14/N843 C99 Standard */
#include <stdbool.h>

#include "bsp.h"
#include "system_TM4C123.h"  
#include "TM4C123GH6PM.h" /* the TM4C MCU Peripheral Access Layer (TI) */

/* on-board LEDs */
#define LED_RED   (1U << 1)
#define LED_BLUE  (1U << 2)
#define LED_GREEN (1U << 3)
#define SW1				(1U << 4)


static uint32_t volatile l_tickCtr;

bool sw1_pushed;

void SysTick_Handler(void) {
   ++l_tickCtr;
}

void GPIOF_Handler(void) {
	GPIOF_AHB->ICR |= SW1;
	sw1_pushed = true;
	
}
void BSP_init(void) {
	SYSCTL->RCGCGPIO  |= (1U << 5); /* enable Run mode for GPIOF */
	SYSCTL->GPIOHBCTL |= (1U << 5); /* enable AHB for GPIOF */
	GPIOF_AHB->DIR |= (LED_RED | LED_BLUE | LED_GREEN);
	GPIOF_AHB->DIR &= ~SW1;
	GPIOF_AHB->DEN |= (LED_RED | LED_BLUE | LED_GREEN | SW1);

		// Device header
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / BSP_TICKS_PER_SEC);

	
	//enable interrupt on PORTF
	GPIOF_AHB->AFSEL &= ~SW1;
	GPIOF_AHB->IS &= ~SW1;
	GPIOF_AHB->IBE &= ~SW1;
	GPIOF_AHB->IEV &= ~SW1;
	GPIOF_AHB->ICR |= SW1;
	GPIOF_AHB->IM |= SW1;
	//NVIC->IP
	GPIOF_AHB->PUR |= SW1;
	NVIC_SetPriority(30U,5U);
	NVIC_EnableIRQ(30U);	


		
	__enable_irq();
}

uint32_t BSP_tickCtr(void) {
	uint32_t tickCtr;

	__disable_irq();
	tickCtr = l_tickCtr;
	__enable_irq();

	return tickCtr;
}

void BSP_delay(uint32_t ticks) {
	uint32_t start = BSP_tickCtr();
	while ((BSP_tickCtr() - start) < ticks) {
	}
}

void BSP_ledRedOn(void) {
	GPIOF_AHB->DATA |= LED_RED;
}

void BSP_ledRedOff(void) {
	GPIOF_AHB->DATA &= ~LED_RED;
}

void BSP_ledBlueOn(void) {
	GPIOF_AHB->DATA |= LED_BLUE;;
}

void BSP_ledBlueOff(void) {
	GPIOF_AHB->DATA &= ~LED_BLUE;
}

void BSP_ledGreenOn(void) {
	GPIOF_AHB->DATA |= LED_GREEN;
}

void BSP_ledGreenOff(void) {
	GPIOF_AHB->DATA &= ~LED_GREEN;
}


void Q_onAssert(char const *module, int loc) {
	/* TBD: damage control */
	(void)module; /* avoid the "unused parameter" compiler warning */
	(void)loc;	/* avoid the "unused parameter" compiler warning */
	NVIC_SystemReset();
}
