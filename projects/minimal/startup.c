#include <stdint.h>

/* defined in linker script */
extern uint32_t _estack;

extern uint32_t _etext;

extern uint32_t _sdata;
extern uint32_t _edata;

extern uint32_t _sbss;
extern uint32_t _ebss;

/* in main.c */
extern void main(void);

/* in libc */
extern void __libc_init_array(void);
void _init() {};

void Reset_Handler(void) 
{	
    uint32_t *init_values_ptr = &_etext;
    uint32_t *data_ptr = &_sdata;

    /* Copy init values from text (=flash) to data (=ram) */
    if (init_values_ptr != data_ptr) {
        for (; data_ptr < &_edata;) {
            *data_ptr++ = *init_values_ptr++;
        }
    }

    /* fill the the bss segment with all zero's */
    for (uint32_t *bss_ptr = &_sbss; bss_ptr < &_ebss;) {
        *bss_ptr++ = 0;
    }

    // initialize c++ stuff
    __libc_init_array();

    main();

    // main should never return!
    while (1);
}

void Default_Handler(void)
{
	while(1);
}

void NMI_Handler 					(void) __attribute__ ((weak, alias("Default_Handler")));
void HardFault_Handler 				(void) __attribute__ ((weak, alias("Default_Handler")));
void MemManage_Handler 				(void) __attribute__ ((weak, alias("Default_Handler")));
void BusFault_Handler 				(void) __attribute__ ((weak, alias("Default_Handler")));
void UsageFault_Handler 			(void) __attribute__ ((weak, alias("Default_Handler")));
void SVC_Handler 					(void) __attribute__ ((weak, alias("Default_Handler")));
void DebugMon_Handler 				(void) __attribute__ ((weak, alias("Default_Handler")));
void PendSV_Handler   				(void) __attribute__ ((weak, alias("Default_Handler")));
void SysTick_Handler  				(void) __attribute__ ((weak, alias("Default_Handler")));

const void *vectors[] __attribute__ ((section (".vectors"))) =
{
    &_estack,
    Reset_Handler,
    NMI_Handler,
    HardFault_Handler,
	MemManage_Handler,
	BusFault_Handler,
	UsageFault_Handler,
	0,
	0,
	0,
	0,
	SVC_Handler,
	DebugMon_Handler,
	0,
	PendSV_Handler,
	SysTick_Handler
};
