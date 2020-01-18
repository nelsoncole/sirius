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
	uint16_t val;__asm__ __volatile__("inw %w1,%w0":"=a"(val):"d"(p));val; })

#define outportw(p,val) __asm__ __volatile__ ("outw %w0,%w1"::"a"(val),"d"(p))

#define inpw(p)({\
	uint16_t val;__asm__ __volatile__("inw %w1,%w0":"=a"(val):"d"(p));val; })

#define outpw(p,val) __asm__ __volatile__ ("outw %w0,%w1"::"a"(val),"d"(p))


//IO R/W DWORD
#define inportl(p)({\
	uint32_t val;\
	__asm__ __volatile__("inl %w1,%k0":"=a"(val):"d"(p));val; })

#define outportl(p,val) __asm__ __volatile__ ("outl %k0,%w1"::"a"(val),"d"(p))

//IO R/W DWORD
#define inportd(p)({\
	uint32_t val;\
	__asm__ __volatile__("inl %w1,%k0":"=a"(val):"d"(p));val; })

#define outportd(p,val) __asm__ __volatile__ ("outl %k0,%w1"::"a"(val),"d"(p))


#define inpl(p)({\
	uint32_t val;\
	__asm__ __volatile__("inl %w1,%k0":"=a"(val):"d"(p));val; })

#define outpl(p,val) __asm__ __volatile__ ("outl %k0,%w1"::"a"(val),"d"(p))

#define cli() __asm__ __volatile__ ("cli")
#define sti() __asm__ __volatile__ ("sti;out %b0,$0x80"::"a"(0))
#define hlt() __asm__ __volatile__ ("hlt")
#define nop() __asm__ __volatile__ ("nop")


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
    #define NULL    ((VOID *) 0)
#endif

#include "typ.h"
#include "gui.h"
#include "bmp.h"


#define DEBUG_MSG
#ifdef DEBUG_MSG
#define debug(format, ...){printf(format, ##__VA_ARGS__);}
#else
#define debug(format, ...) (void)0
#endif


// CHAT
#define MSG_READ_KEY	1
#define MSG_READ_FILE	2
#define MSG_READ_DIR	3
#define MSG_WRITE_FILE	4
#define MSG_WRITE_DIR	5
#define MSG_VIRTUAL_ADDR	0x10001200;

#define	KEY_UP 		(0x48E0)
#define	KEY_LEFT 	(0x4BE0)
#define	KEY_RIGHT 	(0x4DE0)
#define	KEY_DOWN 	(0x50E0)

#define	KEY_ENTER 	(0x1C1C)

typedef struct _CHAT {
	UINT32	id;
	UINT32	p1;
	UINT32	p2;
	UINT32	type;
	UINT32 	process;
	
	struct _CHAT *next;

}__attribute__((packed)) CHAT;


typedef struct _VFS_FILE_HEADER 
{
	// File Header 4 KiB
	CHAR8	filename[256];
	UINT8 	attr;
	UINT32	size;
	UINT8	dev;
	UINT8	p_entry;
	UINT16	bps;
	UINT8	count;	// numeros de sector por bloco
	UINT32	blocks;
	UINT8	rsv[256 - 14];

}__attribute__ ((packed)) VFS_FILE_HEADER;

typedef struct _VFS 
{
	// File Header 4 KiB
	VFS_FILE_HEADER header;

	// LBA block start
	UINT32	block[1];

}__attribute__ ((packed)) VFS;


typedef struct _MOUSE {

	UINT32	x;
	UINT32	y;
	UINT32	z;
	UINT32	b;

}__attribute__((packed)) MOUSE;


// header
extern FOCUS	       	*__focus;
extern UINT32		__pid;

extern GUI *G;
extern UINT32 ColorTable[16];
extern UINT16 font8x16[256*16];
//extern UINT16 cursor18x18[18];


// CHAT
VOID send_msg(UINT32 type,UINT32 p1,UINT32 p2);
UINTN read_msg(UINT32 *p1,UINT32 *p2);


// String
// ASM
extern VOID __copymem(VOID *dest, VOID *src, UINTN size);
extern VOID __setmem(VOID *dest, UINT32 val, UINTN size);

UINTN strlen(CONST CHAR8 *s);
VOID *copymem(IN VOID *Destination,IN VOID *Source,IN UINTN Length);
VOID *setmem(IN VOID *Buffer,IN UINTN Size,IN UINT8 Value);
CHAR8 *strcpy(CHAR8 *Destination, CONST CHAR8 *Source);
UINTN strncpy(CHAR8 *Destination,CONST CHAR8 *Source,UINTN count);
UINTN strcmp (CHAR8* Destination, CONST CHAR8* Source);
UINTN strncmp (CHAR8* Destination,CONST CHAR8* Source,UINTN count);


// 
VOID exit(INTN rc);
VOID *__malloc(UINTN size);
VOID __free(VOID *buffer);


UINTN BitMAP(	VOID *Data,	
		UINTN X,
		UINTN Y,
		VOID *BankBuffer);



#endif
