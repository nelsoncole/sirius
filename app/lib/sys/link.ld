/**
 *	
 *	File Name: link.ld
 **/
 

OUTPUT_FORMAT("elf32-i386")
ENTRY(_start)
SECTIONS
{
	. = 0x10001000;
	
	_start = .;
	
	.text ALIGN(0x1000) : AT(ADDR(.text) - 0x10000000)
	{
		*(.text .text.*)
	}
	
	.rodata ALIGN(0x1000) : AT(ADDR(.rodata) - 0x10000000)
	{
		*(.rodata .rodata.*)
	}
	
	.data ALIGN(0x1000) : AT(ADDR(.data) - 0x10000000)
	{
		*(.data .data.*)
	}
	
	.bss ALIGN(0x1000) : AT(ADDR(.bss) - 0x10000000)
	{
		*(.bss .bss.*)
		*(COMMON)
	}
	
	_end = .;
}
