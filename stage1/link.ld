/**
 *	Copyright (c) 2019, nelsoncole
 *	Nelson Sapalo da Siva Cole
 *	Lubango, 30 de Dezembro de 2019 - Hora: 12h59
 *	File Name: link.ld
 **/
 
OUTPUT_FORMAT("binary")
OUTPUT_ARCH(i386)
ENTRY(_start)
phys = 0x8000;
SECTIONS
{
  .text phys  : {
    *(.text)
  }
  .data  : {
    *(.data)
  }
  .bss  :  { 					
    *(.bss)
  }
}
