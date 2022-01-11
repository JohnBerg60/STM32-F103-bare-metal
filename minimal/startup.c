/* defined in linker script */
extern unsigned int _estack;
extern void main(void);

void Reset_Handler(void) 
{
    main();
}

const void *vectors[] __attribute__ ((section (".vectors"))) =
{
    &_estack,
    Reset_Handler
};