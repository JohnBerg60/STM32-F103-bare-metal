// idea: https://www.youtube.com/watch?v=4vgnM9-EEeA&list=LL&index=1

void reset(void)
{
    volatile unsigned int *rcc_apb2enr = (unsigned int *)(0x40021000 + 0x18);
    volatile unsigned int *gpioc_crh   = (unsigned int *)(0x40011000 + 0x04);
    volatile unsigned int *gpioc_odr   = (unsigned int *)(0x40011000 + 0x0c);
    volatile unsigned i;

    // enable clock to GPIO port c
    *rcc_apb2enr |= 1 << 4;

    // configure PC13 pin for open drain output, 2 MHz max
    *gpioc_crh = (*gpioc_crh & ~ (0xf << ((13 - 8) * 4 ))) | (6 << ((13 - 8) * 4));

    while(1)
    {
        // simple wait
        for(i = 0; i < 500000; i++);

        // toggle output data register pin
        *gpioc_odr ^= 1 << 13;
    }
}

int STACK[256];

const void *vectors[] __attribute__ ((section (".vectors"))) =
{
    STACK + sizeof(STACK) / sizeof(*STACK),
    reset
};