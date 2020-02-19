/*
 * File Name: crt0.c
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
 
#include <io.h>
#include <ctype.h>

extern int main();
void exit(int rc);

FOCUS 	*__focus	= NULL;
UINT32	__pid = -1;

typedef struct _BOOT_INFO{
	void *Graphic;
	
}__attribute__((packed)) BOOT_INFO;

int crt0(BOOT_INFO *boot_info)
{

	

	UINT32 *p = NULL;

	// GUI	
	G = (GUI *)&boot_info->Graphic;

	// pid e focus

	p = (UINT32*)0x10001114;
	__pid = *p;
	__focus = (FOCUS*)(*++p);


	exit(main());

	for(;;);
}


void exit(int rc)
{
	__asm__ __volatile__("int $0x72"::"a"(1));

}


VOID  *copymem(IN VOID *Destination,IN VOID *Source,IN UINTN Length)
{	

	UINTN i;
	UINTN *p_dest = (UINTN*)Destination;
	UINTN *p_src  = (UINTN*)Source;
	
	for(i = 0;i < Length;i++)
		*p_dest++ = *p_src++;

	return p_dest;
}


VOID *setmem(IN VOID *Buffer,IN UINTN Size,IN UINT8 Value)
{
	UINT8 *tmp = (UINT8 *)Buffer;
	UINTN count = Size;
    	for(; count != 0; count--) *tmp++ = Value;
	return Buffer;
}
