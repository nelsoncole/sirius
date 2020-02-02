/*
 * File Name: thread.h
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
#ifndef __THREAD_H__
#define __THREAD_H__

#define MSG_READ_KEY	1
#define MSG_READ_FILE	2
#define MSG_READ_DIR	3
#define MSG_WRITE_FILE	4
#define MSG_WRITE_DIR	5
#define MSG_VIRTUAL_ADDR	0x10001200;


typedef struct _THREAD {
	// Registradores geral
	UINT32 	eax;
	UINT32	ebx;
	UINT32	ecx;
	UINT32	edx;
	UINT32	edi;
	UINT32	esi;
	UINT32	ebp;
	UINT32	esp;
	UINT32	eip;

	// Registradores  segmento
    	UINT32	cs;
	UINT32	ds;
	UINT32	es;
	UINT32	fs;
	UINT32	gs;
    	UINT32	ss;
	
	// Outros registradores 
	UINT32	eflag;
    	UINT32	cr3;

	// Registradores null
	UINT32	esp0;
	
	// ID de tarefa  (conhecido como PID)
    	UINT32	pid;
	UINT32	prv;

	// alloc page
	PAGE_DIRECTORY *pd;
	PAGE_TABLE *pt;
	FRAME *frame;

	UINT32 _static;
	UINT32 flag;

	// Stream default
	FILE 	*stdin;
	FILE 	*stdout;
	FILE	*stderr;

	// linker do próximo     
    	struct _THREAD *next;
	struct _THREAD *tail;


}__attribute__((packed)) THREAD;

typedef struct _CHAT {
	UINT32	id;
	UINT32	p1;
	UINT32	p2;
	UINT32	type;
	UINT32 	process;
	
	struct _CHAT *next;

}__attribute__((packed)) CHAT;


UINTN initialize_thread();
UINTN create_thread(	VOID (*main)(),
			PAGE_DIRECTORY *page_directory,
			UINT32 eax,
			UINT32 ebx,
			UINT32 ecx,
			UINT32 edx,
			UINT32 esp,
			UINT32 esp0,
			UINT8 privileg);

UINTN create_thread_child(THREAD	*thread,	
			VOID (*main)(),
			PAGE_DIRECTORY *page_directory,
			UINT32 eax,
			UINT32 ebx,
			UINT32 ecx,
			UINT32 edx,
			UINT32 esp,
			UINT32 esp0,
			UINT8 privileg);
UINTN getpid();
UINTN getcr3();







#endif
