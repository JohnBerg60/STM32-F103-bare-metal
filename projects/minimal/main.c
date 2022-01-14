#include <stdint.h>
#include "stm32f1xx.h"

// The current clock frequency
uint32_t SystemCoreClock = 8000000;

extern volatile uint32_t systick_count;
extern void SysTick_Config(uint32_t ticks);
extern void init_gpio();
extern void init_usart();
extern uint32_t print(char *string);

// Delay some milliseconds.
// Note that effective time may be up to 1ms shorter than requested.
void delay(uint32_t ms)
{
    uint32_t ends = systick_count + ms;
    while (systick_count < ends);
}

// entry after startup
int main(void)
{
    //char str[12];

    // Initialize systick timer for 1 ms intervals
    SysTick_Config(SystemCoreClock / 1000);
    init_gpio();
    init_usart();

    while (1)
    {
        // reset bit 13 (=led on) from gpio C output data register
        GPIOC->ODR &= ~(1 << 13);
        delay(100);

        // set bit 13 (=led off) from gpio C output data register
        GPIOC->ODR |= 1 << 13;
        delay(900);

        //sprintf(str, "%d", 42);
        print("test - \r\n");
    }
}
