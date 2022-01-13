// idea: https://www.youtube.com/watch?v=4vgnM9-EEeA&list=LL&index=1
#include <stdint.h>

// The current clock frequency
uint32_t SystemCoreClock = 8000000;

extern volatile uint32_t systick_count;
extern void SysTick_Config(uint32_t ticks);

// Delay some milliseconds.
// Note that effective time may be up to 1ms shorter than requested.
void delay(uint32_t ms)
{
    uint32_t ends = systick_count + ms;
    while (systick_count < ends);
}

typedef volatile struct 
{
    uint32_t APB2ENR;
} RCC_TypeDef;
    
#define RCC_APB2ENR (uint32_t)(0x40021000 + 0x18)
#define RCC ((RCC_TypeDef *)RCC_APB2ENR)

// entry after startup
int main(void)
{
    volatile uint32_t *gpioc_crh = (uint32_t *)(0x40011000 + 0x04);
    volatile uint32_t *gpioc_odr = (uint32_t *)(0x40011000 + 0x0c);

    // Initialize systick timer for 1 ms intervals
    SysTick_Config(SystemCoreClock / 1000);

    // enable clock to GPIO port c
    RCC->APB2ENR |= 1 << 4;

    // configure PC13 pin for open drain output, 2 MHz max
    *gpioc_crh = (*gpioc_crh & ~(0xf << ((13 - 8) * 4))) | (6 << ((13 - 8) * 4));

    while (1)
    {
        // simple wait
        delay(1350);

        // toggle output data register pin
        *gpioc_odr ^= 1 << 13;
    }
}
