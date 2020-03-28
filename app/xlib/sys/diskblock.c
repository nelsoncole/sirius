/*
 * File Name: diskblock.c
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
 
#include <sys/diskblock.h>


int block_read(int media_id,int count,unsigned int offset /*LBA START*/,void *buffer)
{
	int rc;
	unsigned int _offset = offset;

	int i = count;
	int tmp_count = 0;

	do {

		if(i < 4) tmp_count = i;
		else tmp_count = 4;

		__asm__ __volatile__("int $0x72":"=a"(rc):"a"(0x10),"d"(media_id),
		"c"(tmp_count),"b"(0/*LBA32-47*/),"D"(buffer),"S"(_offset/*LBA0-31*/));

		i -= tmp_count;

	}while(i > 4);
	
	return rc; 

}

int block_write(int media_id,int count,unsigned int offset /*LBA START*/,void *buffer)
{
	int rc;
	unsigned int _offset = offset;

	int i = count;
	int tmp_count = 0;

	do {

		if(i < 4) tmp_count = i;
		else tmp_count = 4;

		__asm__ __volatile__("int $0x72":"=a"(rc):"a"(0x11),"d"(media_id),
		"c"(count),"b"(0/*LBA32-47*/),"D"(buffer),"S"(_offset/*LBA0-31*/));
	
		i -= tmp_count;

	}while(i > 4);
	
	return rc; 

}

unsigned int block_sectors(int media_id)
{
	unsigned int rc = 0;
	
	__asm__ __volatile__("int $0x72":"=a"(rc):"a"(0xE),"d"(media_id));

	return rc;
}

unsigned int block_bps(int media_id)
{
	unsigned int rc = 0;
	
	__asm__ __volatile__("int $0x72":"=a"(rc):"a"(0xF),"d"(media_id));

	return rc;
}
