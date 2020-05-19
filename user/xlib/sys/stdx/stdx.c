/*
 * File Name: stdx.c
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

#include <sys/sys.h>


// 32 KiB ----> keyboard mensagem 
// 28 KiB ----> normal mensagem 
// 4 KiB ----> mouse mensagem 

// fd->header.offset for keyboard
// fd->header.offset2 for normal
// fd->header.block for mouse


#define SIZE_NORMAL_MENSAGEM 1792

int write_stx(unsigned int id, unsigned int data1,unsigned int data2,FILE *fd)
{

	STDX *pipe = (STDX*) (fd->buffer + (32*1024));
	int i;

	for(i=0;i<SIZE_NORMAL_MENSAGEM;i++)
	{
		if(pipe->pid == 0) break;
		else pipe++;

	}

	// test
	if(i >= SIZE_NORMAL_MENSAGEM) return 0;

	pipe->id = id;
	pipe->pid = getpid();
	pipe->data1 = data1;
	pipe->data2 = data2;
	

	// bloquear processo
	// task switch
	lockthread();
	taskswitch();

	i = pipe->id;

	pipe->pid = 0;

	return i;
}


STDX *read_stx(FILE *fd)
{
	STDX *pipe = (STDX*) (fd->buffer + (32*1024));
	int i;

	for(i=0;i<SIZE_NORMAL_MENSAGEM;i++)
	{
		if((pipe->pid != 0) && (pipe->pid != 0xFFFFFFFF) ) break;
		else pipe++;

	}


	// test
	if(i >= SIZE_NORMAL_MENSAGEM) return 0;

	return pipe;
}
