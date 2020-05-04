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
	unsigned short 	type;
	unsigned int 	size;
	unsigned short	rsv1;
	unsigned short	rsv2;
	unsigned int	offset_bits;
}__attribute__((packed)) BMP_FILE;

typedef struct _BMP_INFO {
	unsigned int 	size;
	unsigned int 	width;
	unsigned int	height;
	unsigned short	planes;
	unsigned short	count;
	unsigned int	compress;
	unsigned int	size_imag;
	unsigned int	hres;
	unsigned int	vres;
	unsigned int	clr_used;
	unsigned int	clr_impor;
	
	
}__attribute__((packed)) BMP_INFO;

typedef struct _BMP_TABLE_COLOR {
	unsigned char 	blue;
	unsigned char 	green;
	unsigned char	red;
	unsigned char	rsv;
	
}__attribute__((packed)) BMP_TABLE_COLOR;

typedef struct _BMP {
	BMP_FILE bformat;
	BMP_INFO binfo;
}__attribute__((packed)) BMP;



#endif
