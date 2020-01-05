/*
 * File Name: dscptor.h
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
#ifndef __DSCPTOR_H__
#define __DSCPTOR_H__
 
 typedef struct {
	UINT64 limit_15_0 :16; 
	UINT64 base_15_0 : 16;
	UINT64 base_23_16 : 8;
	UINT64 type : 4;
	UINT64 s:1;
	UINT64 dpl: 2;
	UINT64 p:1;
	UINT64 limit_19_16: 4;
	UINT64 avl :1;
	UINT64 l:1;
	UINT64 db:1;
	UINT64 g:1; 
	UINT64 base_31_24 : 8;
	 
 }__attribute__((packed)) GDT;
 
typedef struct {
    	UINT16 limit;
	UINT64 base;

}__attribute__((packed)) GDTR;


typedef struct {

    	UINT64 offset_15_0 :16; 
	UINT64 sel : 16;
	UINT64 unused :8;
	UINT64 flags : 5;
	UINT64 dpl : 2;
    	UINT64 p :1;
	UINT64 offset_31_16 : 16;


}__attribute__ ((packed)) IDT;

typedef struct {
    	UINT16 	limit;
	UINT64 	base;

}__attribute__((packed)) IDTR;


typedef struct _TSS {

	UINT16	priv;
	UINT16	reserved0;
	UINT32	esp0;
	UINT16	ss0;
	UINT16	reserved1;
	UINT32	esp1;
	UINT16	ss1;
	UINT16	reserved2;
	UINT32	esp2;
	UINT16	ss2;
	UINT16	reserved3;
	UINT32	cr3;
	UINT32	eip;
	UINT32 	eflags;
	UINT32	eax, ecx, edx, ebx, esp, ebp, esi, edi;
	UINT16	es;
	UINT16	reserved4;
	UINT16	cs;
	UINT16	reserved5;
	UINT16	ss;
	UINT16	reserved6;
	UINT16	ds;
	UINT16	reserved7;
	UINT16	fs;
	UINT16	reserved8;
	UINT16	gs;
	UINT16	reserved9;
	UINT16	ldt_sel;
	UINT16	reserved10;
	UINT16	trap;
	UINT16	io_map;

}__attribute__((packed)) TSS;

UINTN tss_install();
VOID tss_esp0(UINT32 esp);


#endif


