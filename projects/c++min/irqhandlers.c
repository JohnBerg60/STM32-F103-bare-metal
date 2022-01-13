volatile unsigned int tickCount = 0;

void SysTick_Handler(void)
{
    tickCount++;
}
