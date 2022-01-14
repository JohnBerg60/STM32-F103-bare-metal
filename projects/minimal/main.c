#include <stdint.h>
#include <stdio.h>
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

void setup(void) 
{ 
    // Initialize systick timer for 1 ms intervals
    SysTick_Config(SystemCoreClock / 1000);
    init_gpio();
    init_usart();
}

// entry after startup
int main(void)
{
    char buff[32]; 
    int cnt = 0;

    while (1)
    {
        // reset bit 13 (=led on) from gpio C output data register
        GPIOC->ODR &= ~(1 << 13);
        delay(50);

        // set bit 13 (=led off) from gpio C output data register
        GPIOC->ODR |= 1 << 13;
        delay(250);

        snprintf(buff, sizeof(buff), "loop: %d \r\n", cnt++);
        print(buff);
    }
}