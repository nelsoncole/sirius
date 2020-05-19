/*
 * File Name: string.c
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
#include <os.h>

UINTN strlen(CONST CHAR8 *s)
{
	char *tmp = (char*)s;
	
	while(*tmp != '\0')tmp++;

	return (size_t)(tmp - s);
}

VOID  *copymem(IN VOID *Destination,IN VOID *Source,IN UINTN Length)
{	

	UINTN i;
	UINT8 *p_dest = (UINT8*)Destination;
	UINT8 *p_src  = (UINT8*)Source;
	

	for(i =0; i<Length;i++)*p_dest++ = *p_src++;
		

	return Destination;
}


VOID *setmem(IN VOID *Buffer,IN UINTN Size,IN UINT8 Value)
{
	UINT8 *tmp = (UINT8 *)Buffer;
	UINTN count = Size;
    	for(; count != 0; count--) *tmp++ = Value;
	return Buffer;
}


CHAR8 *strcpy(CHAR8 *Destination, CONST CHAR8 *Source)
{
    CHAR8 *p = (CHAR8*)Destination;
    INTN i =0;
    while (Source[i] != '\0'){
    Destination[i] = Source[i]; i++;
    }
    return p;
}

UINTN strncpy(CHAR8 *Destination,CONST CHAR8 *Source,UINTN count)
{
    	UINTN i;
	for(i =0;i <count;i++)
	Destination[i] = Source[i];	
	Destination[i] ='\0';
    	return i;
}

UINTN strcmp (CHAR8* Destination, CONST CHAR8* Source)
{

    while((*Destination++ == *Source++))
    if((*Destination == *Source) && (*Destination + *Source == 0))
    return 0;


    return -1;
}
UINTN strncmp (CHAR8* Destination,CONST CHAR8* Source,UINTN count)
{
	
	if(!count)
	return -1;

	while(--count)
	if(*Destination++ != *Source++)
	break;

	return (count);
}


void  *memcpy(void *dest,const void *src,unsigned len)
{	

	int i;
	unsigned char *p_dest = (unsigned char *)dest;
	unsigned char *p_src  = (unsigned char *)src;
	

	for(i =0; i<len;i++)*p_dest++ = *p_src++;
		

	return dest;
}


void *memset(void *buffer,int val,unsigned count)
{
	unsigned char *tmp = (unsigned char *) buffer;

    	for(; count != 0; count--) *tmp++ = val;
	return buffer;
}

