#include <stdint.h>
#include "stm32f1xx.h"

// The current clock frequency
uint32_t SystemCoreClock=8000000;

// Counts milliseconds
volatile uint32_t systick_count=0;

// Interrupt handler
void SysTick_Handler()
{
    systick_count++;
}

// Delay some milliseconds.
// Note that effective time may be up to 1ms shorter than requested.
void delay_ms(int ms)
{
    uint32_t start=systick_count;
    while (systick_count-start<ms);
}

// called in assembler startup file
// in cmsis defined in source/templates/system_stm32f1xx.cB
void SystemInit (void) { }
void _init(void) { }

int main()
{    
    // Initialize systick timer for 1 ms intervals
    SysTick_Config(SystemCoreClock/1000);

    // Enable Port C clock
    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPCEN);

    // PA5 and PC13 = Output for LEDs
    MODIFY_REG(GPIOC->CRH, GPIO_CRH_CNF13 + GPIO_CRH_MODE13, GPIO_CRH_MODE13_0);

    while(1)
    {
        // LED Pin -> High
        WRITE_REG(GPIOC->BSRR, GPIO_BSRR_BS13);        
        delay_ms(250);

        // LED Pin -> Low
        WRITE_REG(GPIOC->BSRR, GPIO_BSRR_BR13);        
        delay_ms(250);
    }
}