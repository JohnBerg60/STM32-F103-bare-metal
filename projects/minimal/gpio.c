#include <stdint.h>
#include "stm32f1xx.h"

void init_gpio()
{
    // Alternate Function I/O clock enable 
    RCC->APB2ENR |= 1 << 0;

    // enable clock to GPIO port A
    RCC->APB2ENR |= 1 << 2;

    // enable clock to GPIO port C
    RCC->APB2ENR |= 1 << 4;

    // enable clock USART1
    RCC->APB2ENR |= 1 << 14;

    // set PA to <unknown> mode
    GPIOA->CRH |= (0x3U << 4)  | (0x2U << 6);
    GPIOA->CRH &= ~(0x1U << 6);

    // configure PC13 pin for open drain output, 2 MHz max
    GPIOC->CRH = (GPIOC->CRH & ~(0xf << ((13 - 8) * 4))) | (6 << ((13 - 8) * 4));
}