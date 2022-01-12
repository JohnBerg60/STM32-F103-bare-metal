// idea: https://www.youtube.com/watch?v=4vgnM9-EEeA&list=LL&index=1
#include <stdint.h>

// The current clock frequency
uint32_t SystemCoreClock = 8000000;

// Counts milliseconds
volatile uint32_t systick_count;

// Delay some milliseconds.
// Note that effective time may be up to 1ms shorter than requested.
void delay(uint32_t ms)
{
    uint32_t ends = systick_count + ms;
    while (systick_count < ends);
}

// Interrupt handler
void SysTick_Handler()
{
    systick_count++;
}

void SysTick_Config(uint32_t ticks)
{
    volatile uint32_t *systick_load = (uint32_t *)(0xE000E010 + 0x04);
    volatile uint32_t *systick_val = (uint32_t *)(0xE000E010 + 0x08);
    volatile uint32_t *systick_ctrl = (uint32_t *)(0xE000E010 + 0x00);

    // Set reload register
    *systick_load = ticks - 1;

    // Set current value register
    *systick_val = 0;

    // Set the clock source of the system timer as AHBCLK=72M
    // Enable system timer interrupt
    *systick_ctrl  |= (1 << 1) | (1 << 2);

    // Enable timer
    *systick_ctrl  |= 1 << 0;
}

int main(void)
{
    volatile uint32_t *rcc_apb2enr = (uint32_t *)(0x40021000 + 0x18);
    volatile uint32_t *gpioc_crh = (uint32_t *)(0x40011000 + 0x04);
    volatile uint32_t *gpioc_odr = (uint32_t *)(0x40011000 + 0x0c);

    // Initialize systick timer for 1 ms intervals
    SysTick_Config(SystemCoreClock / 1000);

    // enable clock to GPIO port c
    *rcc_apb2enr |= 1 << 4;

    // configure PC13 pin for open drain output, 2 MHz max
    *gpioc_crh = (*gpioc_crh & ~(0xf << ((13 - 8) * 4))) | (6 << ((13 - 8) * 4));

    while (1)
    {
        // simple wait
        delay(150);

        // toggle output data register pin
        *gpioc_odr ^= 1 << 13;
    }
}
