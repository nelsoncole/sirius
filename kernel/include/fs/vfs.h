/*
 * File Name: vfs.h
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


#ifndef __VFS_H__
#define __VFS_H__

#define ATTR_ARCHIVE 	0
#define ATTR_DIRECTORY 	1


typedef struct _VFS_FILE_HEADER 
{
	// File Header 4 KiB
	CHAR8	filename[256];
	UINT8 	attr;
	UINT32	size;
	UINT8	dev;
	UINT8	p_entry;
	UINT16	bps;
	// números de sector por bloco
	UINT8	count;
	// número total de blocos
	UINT32	blocks;	
	UINT32	offset_lo;
	UINT32	buffer;
	// definido em libc padrão
	UINT8	mode[4]; 
	UINT32	size_hi;
	UINT32	offset_hi;
	struct _VFS_FILE_HEADER *current;
	struct _VFS_FILE_HEADER *next;
	UINT8	rsv[256 - 42];

}__attribute__ ((packed)) VFS_FILE_HEADER;

typedef struct _VFS 
{
	// File Header 4 KiB
	VFS_FILE_HEADER header;

	// LBA block start
	UINT32	block[1];

}__attribute__ ((packed)) VFS;



#endif
