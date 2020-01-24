/**
 *
 *	File Name: link.ld
 **/
 

OUTPUT_FORMAT("elf32-i386")
ENTRY(_start)
SECTIONS
{
	. = 0x101000;
	
	_start = .;
	
	.text : AT(ADDR(.text) - 0x100000)
	{
		 code = .;
    		*(.text)
    		*(.rodata)
    		. = ALIGN(4096);
	}
	
	.data : AT(ADDR(.data) - 0x100000)
	{
		data = .;
    		*(.data)
    		. = ALIGN(4096);
	}
	
	.bss : AT(ADDR(.bss) - 0x100000)
	{
		bss = .;
    		*(.bss)
		*(COMMON)
		. = ALIGN(4096);
	}
	
	_end = .;
}
