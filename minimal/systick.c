#include <stdint.h>

// Counts milliseconds
volatile uint32_t systick_count;

typedef volatile struct 
{
    uint32_t CTRL;
    uint32_t LOAD;
    uint32_t VAL;
} SYSTICK_TypeDef;

#define SYSTICK_BASE (uint32_t)(0xE000E010)
#define SYSTICK ((SYSTICK_TypeDef *)SYSTICK_BASE)

void SysTick_Config(uint32_t ticks)
{
    // Set reload register
    SYSTICK->LOAD = ticks - 1;

    // Set current value register
    SYSTICK->VAL = 0;

    // Set the clock source of the system timer as AHBCLK=72M
    // Enable system timer interrupt
    SYSTICK->CTRL  |= (1 << 1) | (1 << 2);

    // Enable timer
    SYSTICK->CTRL  |= 1 << 0;
}

// Interrupt handler
void SysTick_Handler()
{
    systick_count++;
}