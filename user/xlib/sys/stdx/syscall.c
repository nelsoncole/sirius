/*
 * File Name: syscall.c
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

void set_focus_kbdc(unsigned int pid)
{
	__asm__ __volatile__("int $0x72"::"a"(1),"d"(pid));

}

int getpid() 
{
	
	int rc = 0;
	__asm__ __volatile__("int $0x72":"=a"(rc):"a"(9));
	
	return rc;
}

void taskswitch() 
{

	__asm__ __volatile__("int $0x70"::);
	
}

void taskswitch_pid(unsigned int pid) 
{
	//__asm__ __volatile__("int $0x72":"=a"(rc):"a"(12),"d"(pid));
	__asm__ __volatile__("int $0x70"::);
	
}

int lockthread() 
{
	
	int rc = 0;
	__asm__ __volatile__("int $0x72":"=a"(rc):"a"(10));
	
	return rc;
}

int unlockthread(unsigned int pid) 
{
	
	int rc = 0;
	__asm__ __volatile__("int $0x72":"=a"(rc):"a"(11),"d"(pid));
	
	return rc;
}

int cheksum_pid(unsigned int pid) 
{

	int rc = 0;

	__asm__ __volatile__("int $0x72":"=a"(rc):"a"(13),"d"(pid));

	return rc;

}

