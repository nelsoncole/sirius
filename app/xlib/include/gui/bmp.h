/*
 * File Name: bmp.h
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
#ifndef __BMP_H__
#define __BMP_H__


typedef struct _BMP_FILE {
	UINT16 	type;
	UINT32 	size;
	UINT16	rsv1;
	UINT16	rsv2;
	UINT32	offset_bits;
}__attribute__((packed)) BMP_FILE;

typedef struct _BMP_INFO {
	UINT32 	size;
	UINT32 	width;
	UINT32	height;
	UINT16	planes;
	UINT16	count;
	UINT32	compress;
	UINT32	size_imag;
	UINT32	hres;
	UINT32	vres;
	UINT32	clr_used;
	UINT32	clr_impor;
	
	
}__attribute__((packed)) BMP_INFO;

typedef struct _BMP_TABLE_COLOR {
	UINT8 	blue;
	UINT8 	green;
	UINT8	red;
	UINT8	rsv;
	
}__attribute__((packed)) BMP_TABLE_COLOR;

typedef struct _BMP {
	BMP_FILE bformat;
	BMP_INFO binfo;
}__attribute__((packed)) BMP;


UINTN BitMAP(	VOID *Data,	
		UINTN X,
		UINTN Y,
		VOID *BankBuffer);



#endif
