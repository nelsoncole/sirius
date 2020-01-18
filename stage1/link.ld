/**
 *	Copyright (c) 2019, nelsoncole
 *	Nelson Sapalo da Siva Cole
 *	Lubango, 30 de Dezembro de 2019 - Hora: 12h59
 *	File Name: link.ld
 **/
 
OUTPUT_FORMAT("binary")
OUTPUT_ARCH(i386)
ENTRY(_start)
SECTIONS
{
	. = 0x8000;
	
	_start = .;
	
	.text : AT(ADDR(.text) - 0x8000)
	{
		*(.text .text.*)
	}
	
	.rodata : AT(ADDR(.rodata) - 0x8000)
	{
		*(.rodata .rodata.*)
	}
	
	.data : AT(ADDR(.data) - 0x8000)
	{
		*(.data .data.*)
	}
	
	.bss : AT(ADDR(.bss) - 0x8000)
	{
		*(.bss .bss.*)
		*(COMMON)
	}
	
	_end = .;
}
