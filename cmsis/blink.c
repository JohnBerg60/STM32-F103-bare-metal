#include <stdint.h>
#include "stm32f1xx.h"

// delay loop for 8 MHz CPU clock with optimizer enabled
void delay(uint32_t msec)
{
    for (uint32_t j=0; j < 2000UL * msec; j++)
    {
        __NOP();
    }
}

void SystemInit (void) { }

int main()
{
    // Enable Port C clock
    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPCEN);

    // PA5 and PC13 = Output for LEDs
    MODIFY_REG(GPIOC->CRH, GPIO_CRH_CNF13 + GPIO_CRH_MODE13, GPIO_CRH_MODE13_0);

    while(1)
    {
        // LED Pin -> High
        WRITE_REG(GPIOC->BSRR, GPIO_BSRR_BS13);        
        delay(100);

        // LED Pin -> Low
        WRITE_REG(GPIOC->BSRR, GPIO_BSRR_BR13);        
        delay(100);
    }
}