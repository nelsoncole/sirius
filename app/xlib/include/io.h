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


#ifndef IN 	// Prototype argument decoration for EFI parameters to indicate their direction
    #define IN 	// IN - argument is passed into the function
    #define OUT	// OUT - argument (pointer) is returned from the function
    #define OPTIONAL	// OPTIONAL - argument is optional
#endif


#ifndef CONST
   #define CONST const
#endif

#include "typ.h"
#include "gui/gui.h"
#include "gui/bmp.h"


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


// header
extern FOCUS	       	*__focus;
extern UINT32		__pid;
extern UINTN 		spin_lock;

extern GUI *G;
extern UINT32 ColorTable[16];
extern UINT16 font8x16[256*16];
extern UINT16 cursor18x18[18];


// String
// ASM
extern VOID __copymem(VOID *dest, VOID *src, UINTN size);
extern VOID __setmem(VOID *dest, UINT32 val, UINTN size);

VOID *copymem(IN VOID *Destination,IN VOID *Source,IN UINTN Length);
VOID *setmem(IN VOID *Buffer,IN UINTN Size,IN UINT8 Value);


// 
VOID exit(INTN rc);


UINTN BitMAP(	VOID *Data,	
		UINTN X,
		UINTN Y,
		VOID *BankBuffer);



#endif
