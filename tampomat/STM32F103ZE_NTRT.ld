/*
 *   Filippo Rusco
 *
 *   Linker file for
 *   cpu:       STM32F103ZE
 *   platform:  NTRT
 *   toorchain: CodeSourcery arm-2009q3-68-arm-none-eabi
 */


ENTRY(Reset_Handler)

STACK_SIZE = 0x4000; 

MEMORY
{
    FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 512K
    SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 64K
    SRAM_EXT (rwx) : ORIGIN = 0x60000000, LENGTH = 4M
}

SECTIONS
{
	.text :
	{
	_vectors = .;
	PROVIDE(_vectors = _vectors);
	KEEP(*(.isr_vector))  /* Cortex-M3 and STM32 vector table  (stm32f10x_vectors.c) */
	_evectors = .;
	PROVIDE(_evectors = _evectors);
	_text = .;
	PROVIDE(_text = _text);
	*(.text*)
	*(.rodata*)
	. = ALIGN(4); /* needed by the startup.c to copy the data section aligned */
	_etext = .;
	PROVIDE(_etext = _etext);
	} > FLASH
	
	.data : AT (ADDR(.text) + SIZEOF(.text))
	{	
	_data = .;
	PROVIDE(_data = _data);
	*(vtable)
	*(.data*)
	_edata = .;
	PROVIDE(_edata = _edata);
	} > SRAM

	/* BUG 40521 GCC */
    /DISCARD/ :
    {
    	*(.eh_*)
    }
    
    

	.bss :
	{
	. = ALIGN(4);
	_bss = .;
	PROVIDE(_bss = _bss);
	*(.bss*)
	*(COMMON)
	_ebss = .;
	PROVIDE(_ebss = _ebss);
	. = ALIGN(4);
	_end = .;
	PROVIDE(_end = _end);
	_heap = .;
	PROVIDE(_heap = _heap);
	. += STACK_SIZE ;
	_main_stack = .;
	PROVIDE(_main_stack = _main_stack);
	} > SRAM

	_esram = ORIGIN(SRAM)+LENGTH(SRAM)-4;
	PROVIDE(_esram = _esram);

/*
	.main_stack : AT (ADDR(.data) + SIZEOF(.data) + SIZEOF(.bss) + 16)
	{
	. = ALIGN(4);
	_main_stack = .;
	PROVIDE(_main_stack = _main_stack);
	} > SRAM

	.___PSP :
	{

	_ePSP = .;	
	} > SRAM

	.___heap :
	{
	
	} > SRAM
*/
}
