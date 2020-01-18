/*
 * File Name: type.h
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
 
#ifndef __TYP_H__
#define __TYP_H__

typedef unsigned char uint8_t;
typedef	unsigned short uint16_t;
typedef	unsigned int uint32_t;
typedef	unsigned long long uint64_t;
typedef unsigned size_t;

// Common UEFI data types
typedef char BOOLEAN;
typedef int INTN;
typedef unsigned UINTN;

typedef char INT8;
typedef unsigned char UINT8;

typedef short INT16;
typedef unsigned short UINT16; 
 
typedef long INT32;
typedef unsigned long UINT32;

typedef long long INT64;
typedef unsigned long long UINT64;

//typedef signed INT128;
//typedef unsigned UNT128;

typedef char  CHAR8;
typedef short unsigned int CHAR16;

typedef void VOID;

#if defined(__x86_64__) || defined(__aarch64__)
	typedef UINT64 VIRTUAL_ADDRESS;
	typedef UINT64 PHYSICAL_ADDRESS;
	
#elif defined(__i386__) || defined(__arm__)
	typedef UINT32 PHYSICAL_ADDRESS;
	typedef UINT32 VIRTUAL_ADDRESS;
#else
#error YOUR ARCH HERE
#endif


typedef struct {
	UINT16 	cs;
	UINT16 	ds;
	UINT16	es;
	UINT16 	ss;
	UINT16	fs;
	UINT16	gs;
	UINTN 	a;
	UINTN 	b;
	UINTN 	c;
	UINTN 	d;
	
}REG;


typedef struct _MOUSE {

	UINT32	x;
	UINT32	y;
	UINT32	z;
	UINT32	b;

}__attribute__((packed)) MOUSE;



#endif
