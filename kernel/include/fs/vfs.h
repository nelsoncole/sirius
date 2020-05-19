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

struct _FAT_BPB;

typedef struct _VFS 
{
	short level;		// Nível do buffer cheio/vazio
	unsigned flags;		// Sinalizadores de status
	char fd;		// Descritor de arquivo
	unsigned char hold;	// Caractere ungetc se não existir um buffer
	unsigned bsize;		// Tamanho do buffer
	unsigned char *buffer;	// Buffer de transferência
	unsigned char *curp;	// Ponteiro activo atual
	unsigned istemp;	// Indicador de arquivo temporário
	short token;		// Usado para verificação de validade
	//
	unsigned char mode[4];
	//
	char fname[256];	// Nome do arquivo aberto
	unsigned size;		// Tamanho do arquivo
	unsigned int off;	// Deslocamento
	unsigned int off2;
	//
	unsigned bps;		// Bytes por sector
	unsigned tnb;		// Total numero de blocos 
	unsigned count;		// Numero de sectores por bloco
	unsigned devn;		// Numero do dispositivo
	unsigned char *superblock;
	unsigned char *root;	//
	unsigned d_entry;	// Entrada no directorio  
	unsigned fs_type;	// Tipo de sistema de arquivo  

	unsigned int *table;	// 1024x1024

}__attribute__ ((packed)) VFS;



#endif
