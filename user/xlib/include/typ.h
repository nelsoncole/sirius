/*
 * File Name: typ.h
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

// Common UEFI data types
typedef char BOOLEAN;
typedef int INTN;
typedef unsigned UINTN;

typedef char INT8;
typedef unsigned char UINT8;

typedef short INT16;
typedef unsigned short UINT16; 
 
typedef long INT32;
typedef unsigned int UINT32;

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

typedef struct _FOCUS 
	{
	UINTN 			pid;
	PHYSICAL_ADDRESS 	pd;
	

	}__attribute__((packed)) FOCUS;

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
	UINT32	size2;
	UINT8	dev;
	UINT8	p_entry;
	UINT32	bps;
	// números de sector por bloco
	UINT32	count;
	// número total de blocos
	UINT32	blocks;	
	UINT32	offset;
	UINT32	offset2;
	UINT32	buffer;
	// definido em libc padrão
	UINT8	mode[4];
	UINT8	flag;
	UINT32  bpb; 
	struct _VFS_FILE_HEADER *current;
	struct _VFS_FILE_HEADER *next;
	UINT8	rsvx[256 - 43];

}__attribute__ ((packed)) VFS_FILE_HEADER;

typedef struct _VFS 
{
	// File Header 512 Bytes
	VFS_FILE_HEADER header;

	// LBA block start
	UINT32	block[1024-128];

}__attribute__ ((packed)) VFS;


typedef struct _MOUSE {

	UINT32	x;
	UINT32	y;
	UINT32	z;
	UINT32	b;
	UINT32	handle;

}__attribute__((packed)) MOUSE;

typedef struct _SD {
	char id[4];
	unsigned int devnum;
	unsigned int UID;
	unsigned int partnum;
	unsigned int lba_start;
	unsigned int lba_end;
	unsigned int num_sectors;
	unsigned short byte_of_sector;
	unsigned short pid;

}__attribute__((packed)) SD;



#endif
