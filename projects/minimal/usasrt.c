#include <stdint.h>
#include "stm32f1xx.h"

extern uint32_t SystemCoreClock;

void init_usart()
{
    // Set baudrate, assuming that USART2 is clocked with 
    // the same frequency as the CPU core (no prescalers).
    USART1->BRR = (SystemCoreClock / 9600);

    // only enable transmitter, receiver USART1, no interrupt!
    USART1->CR1 |= (1 << 3); /*!< Transmitter Enable */
    USART1->CR1 |= (1 << 2); /*!< Receiver Enable */
    USART1->CR1 |= (1 << 13); /*!< USART Enable */
}

void put_char(uint8_t byte)
{
    USART1->DR = (int)(byte);

    while (!(USART1->SR & (1 << 7)));  /*!< Transmit Data Register Empty */
}

uint32_t print(char *string)
{
    uint32_t count = 0;
    
    while(*string)
    {
        put_char(*string);
        string++;
        count++;
    }

    return(count);
}
