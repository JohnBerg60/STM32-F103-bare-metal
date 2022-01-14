#include <stdint.h>
#include "stm32f1xx.h"

// initalizes USART1 
void init_usart()
{
    // Enable clock for Port A, alternate functions and USART1
    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPAEN + RCC_APB2ENR_AFIOEN);
    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_USART1EN);
    

    // PA9 (TxD) shall use the alternate function with push-pull
    MODIFY_REG(GPIOA->CRH, GPIO_CRH_CNF9 + GPIO_CRH_MODE9, GPIO_CRH_CNF9_1 + GPIO_CRH_MODE9_1);

    // Enable transmitter, receiver and receive-interrupt of USART2
    USART1->CR1 = USART_CR1_UE + USART_CR1_TE + USART_CR1_RE + USART_CR1_RXNEIE;

    // Set baudrate, assuming that USART2 is clocked with 
    // the same frequency as the CPU core (no prescalers).
    USART1->BRR = (SystemCoreClock / 9600);
    
    // With > 36 MHz system clock, the USART1 receives usually half of it:
    // USART1->BRR = (SystemCoreClock / 2 / 9600);

    // Enable interrupt in NVIC
    NVIC_EnableIRQ(USART1_IRQn);
}

// called after each received character
void USART1_IRQHandler()
{
    char received = USART1->DR;

    // send echo back
    while(!(USART1->SR & USART_SR_TXE));
    USART1->DR = received;
}

// Redirect standard output to the serial port
int _write(int file, char *ptr, int len)
{
    for (int i=0; i<len; i++)
    {
        while(!(USART1->SR & USART_SR_TXE));
        USART1->DR = *ptr++;
    }
    return len;
}

// stubs, to satisfy stdio requierements
int _read(int file, char *ptr, int len) { return 0; }
int _close(int file) { return -1; }
int _lseek(int file, int ptr, int dir) { return 0; }
int _open(const char *name, int flags, int mode) { return -1; }
int _fstat(int file, struct stat *st) { return 0; }
int _isatty(int file) { return 1; }
int _sbrk(int incr) { return 0; }