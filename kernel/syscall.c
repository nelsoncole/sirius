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
#include <os.h>

#define SYSCALL_NUM 6

extern VOID interrupter(INTN n,UINT32 offset,UINT16 sel,UINT8 dpl );
extern void int114(); // system call
extern void int113(); // exit

static VOID sys_exit()
{
    	exit();
}


static VOID *sys_malloc(UINTN size)
{
    	return malloc(size);
}

static VOID sys_free(VOID *buf)
{
    	free(buf);
}

static VOID sys_reboot()
{
    	kbdc_wait(1);
	outportb(0x64,0xFE);
}

static VOID sys_pci(UINTN max_bus) {
	pci_get_info(0,max_bus);

}


VOID *syscall_table[SYSCALL_NUM]={
    	0,			// eax, 0	null
    	&sys_exit,       	// eax, 1    	sys_exit
	&sys_malloc,       	// eax, 2    	sys_malloc
	&sys_free,       	// eax, 3    	sys_free
	&sys_reboot,       	// eax, 4    	sys_reboot
	&sys_pci		// eax, 5	sys_pci, edx = max_bus
};

static VOID invalidsyscall(UINT32 num)
{
	print("Invalid syscall: EAX,%d INT 0x72\n",num);

}

UINTN syscall_handler(UINTN num){

    	UINT32 eax =0;

    	if(num < SYSCALL_NUM) {

		VOID *addr =syscall_table[num];

		__asm__ __volatile__ ("\
		pushl %%ebx;\
		pushl %%ecx;\
		pushl %%edx;\
		call *%k1;\
    		addl $12,%%esp;\
		":"=a"(eax):"r"(addr));

	}else {

    		__asm__ __volatile__ ("\
		pushl %%ebx;\
		pushl %%ecx;\
		pushl %%edx;\
		call *%k1;\
    		addl $12,%%esp;\
		":"=a"(eax):"r"(invalidsyscall),"d"(num));
    	}

    	return (eax);
}

VOID syscall_install()
{
	REG reg;
	reg.cs = 0x8; 


	interrupter(0x71,(UINTN)int113,reg.cs,0);
	// ring 3
    	interrupter(0x72,(UINTN)int114,reg.cs,3);
	
}
