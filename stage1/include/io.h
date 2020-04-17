/*
 * File Name: io.h
 *
 *
 * BSD 3-Clause License
 * 
 * Copyright (c) 2019, nelsoncole
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#ifndef __IO_H__
#define __IO_H__
#include <typedef.h>
#include <typ.h>

#define false 	0
#define true 	1
#define null 	0

#ifndef IN 	// Prototype argument decoration for EFI parameters to indicate their direction
    #define IN 	// IN - argument is passed into the function
    #define OUT	// OUT - argument (pointer) is returned from the function
    #define OPTIONAL	// OPTIONAL - argument is optional
#endif


#ifndef CONST
   #define CONST const
#endif
#ifndef TRUE
    #define TRUE    ((BOOLEAN) 1)
    #define FALSE   ((BOOLEAN) 0)
#endif

#ifndef NULL
    #define NULL 0  //   ((VOID *) 0)
#endif

//IO Delay
#define io_delay() __asm__ __volatile__("out %%al,$0x80"::)

//IO R/W BYTE
#define inportb(p)({\
	uint8_t val;\
	__asm__ __volatile__("in %w1,%b0":"=a"(val):"d"(p));val; })

#define outportb(p,val)__asm__ __volatile__("out %b0,%w1"::"a"(val),"d"(p))

#define inpb(p)({\
	uint8_t val;\
	__asm__ __volatile__("in %w1,%b0":"=a"(val):"d"(p));val; })

#define outpb(p,val)__asm__ __volatile__("out %b0,%w1"::"a"(val),"d"(p))


//IO R/W WORD
#define inportw(p)({\
	uint32_t val;__asm__ __volatile__("inw %w1,%w0":"=a"(val):"d"(p));val; })

#define outportw(p,val) __asm__ __volatile__ ("outw %w0,%w1"::"a"(val),"d"(p))

#define inpw(p)({\
	uint32_t val;__asm__ __volatile__("inw %w1,%w0":"=a"(val):"d"(p));val; })

#define outpw(p,val) __asm__ __volatile__ ("outw %w0,%w1"::"a"(val),"d"(p))


//IO R/W DWORD
#define inportl(p)({\
	uint32_t val;\
	__asm__ __volatile__("inl %w1,%k0":"=a"(val):"d"(p));val; })

#define outportl(p,val) __asm__ __volatile__ ("outl %k0,%w1"::"a"(val),"d"(p))

#define inpl(p)({\
	uint32_t val;\
	__asm__ __volatile__("inl %w1,%k0":"=a"(val):"d"(p));val; })

#define outpl(p,val) __asm__ __volatile__ ("outl %k0,%w1"::"a"(val),"d"(p))

#define cli() __asm__ __volatile__ ("cli")
#define sti() __asm__ __volatile__ ("sti")
#define hlt() __asm__ __volatile__ ("hlt")
#define nop() __asm__ __volatile__ ("nop")


#include "string.h"
#include "idt.h"
#include "exceptions.h"
#include "pic.h"
#include "irqs.h"
#include "gui/gui.h"
#include "pci.h"
#include "disk/ata.h"
#include "disk/ahci.h"
#include "mbr.h"
#include "fs/vfs.h"
#include "fs/fat.h"

extern	UINT32 UID;
extern 	UINT32 HDN;
extern	GUI g[1];	
extern	GUI *G;
extern	GW_HAND	*Box;
extern	UINT32 ColorTable[16];
extern 	UINT16 font8x16[128*16];
extern	CHAR8 *font;

extern 	CHAR8 *__buffer__;
extern 	CHAR8 *__string__;


// ASM
extern VOID __copymem(VOID *dest, VOID *src, UINTN size);
extern VOID __setmem(VOID *dest, UINT32 val, UINTN size);

// MAIN
VOID *alloc(UINTN size);


// CONSOLE
UINTN 
ClearScreen();
UINTN 
SetAttribute(IN UINTN Foreground,IN UINTN Background);
UINTN  
EnableCursor(IN BOOLEAN Visible);
UINTN  
SetCursorPosition(IN UINTN Column,IN UINTN Row);
UINTN  
print(CONST CHAR8 *format,...);


//DISK
VOID initialize_gui();
int read_sector(int dev,unsigned count,unsigned long long addr,void *buffer);

//Media
UINTN Read(IN VFS *vfs,OUT VOID *buffer);
UINTN Write(IN VFS *vfs,IN VOID *buffer);

//MBOOT
VOID mboot(UINT32 arg);
extern VOID execkernelx86(UINT32,VOID *);






#endif















