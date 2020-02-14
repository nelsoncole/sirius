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

#define SYSCALL_NUM 18

extern VOID interrupter(INTN n,UINT32 offset,UINT16 sel,UINT8 dpl );
extern void int114(); // system call
extern void int113(); // exit

static VOID sys_exit()
{
    	exit();
}


static VOID *sys_malloc(UINTN size)
{
	void *addr = malloc(size);

    	return addr;
}

static VOID sys_free(VOID *buf)
{
    	free(buf);
}

static VOID sys_reboot()
{

	cli();
    	kbdc_wait(0);
	outportb(0x64,0xFE);
	kbdc_wait(0);
}


UINTN sys_do_exec_child(CONST CHAR8 *name,UINT8 prv) {

	return do_exec_child(current_thread,name,prv);

}


int syscall_read_sector(int p,int count,unsigned int lba32_47 ,void *buffer,unsigned int lba0_31) {

	UINT64 addr = ( (lba0_31 &0xFFFFFFFF) /*| (lba32_47 << 32) */);

	return read_sector(p,count,addr,buffer);

}

int syscall_write_sector(int p,int count,unsigned int lba32_47 ,void *buffer,unsigned int lba0_31) {

	UINT64 addr = ( (lba0_31 &0xFFFFFFFF) /*| (lba32_47 << 32) */);

	return write_sector(p,count,addr,buffer);

}

void syscall_unknown(void){

}

VOID *syscall_table[SYSCALL_NUM]={
    	syscall_unknown,	// eax, 0	null
    	&sys_exit,       	// eax, 1    	sys_exit
	&sys_malloc,       	// eax, 2    	sys_malloc, edx = size
	&sys_free,       	// eax, 3    	sys_free, edx = buf
	&sys_reboot,       	// eax, 4    	sys_reboot
	&syscall_unknown,	// eax, 5	reserved
	&syscall_unknown,	// eax, 9	reserved
	&sys_do_exec_child,	// eax, 7	sys_do_exec_child, edx = filename, ecx = prv
	&syscall_unknown,	// eax, 8	reserved
	&syscall_unknown,	// eax, 9	reserved
	&syscall_unknown,	// eax, 10	reserved
	&syscall_unknown,	// eax, 11	reserved
	&syscall_unknown,	// eax, 12	reserved
	&syscall_unknown,	// eax, 13	reserved
	&syscall_unknown,	// eax, 14	reserved
	&syscall_unknown,	// eax, 15	reserved
	&syscall_read_sector,	// eax, 0x10	read_sector, edx = MediaID, ecx = count, cbx = LBA32-47, edi = buffer, esi = LBA0-31
	&syscall_write_sector,	// eax, 0x11	write_sector, edx = MediaID, ecx = count, cbx = LBA32-47, edi = buffer, esi = LBA0-31
};

static VOID invalidsyscall(UINT32 num)
{
	print("Invalid syscall: EAX,0x%x INT 0x72\n",num);

}

UINTN syscall_handler(UINTN num){

    	UINT32 eax =0;

    	if(num < SYSCALL_NUM) {

		VOID *addr =syscall_table[num];

		__asm__ __volatile__ ("\
		pushl %%esi;\
		pushl %%edi;\
		pushl %%ebx;\
		pushl %%ecx;\
		pushl %%edx;\
		call *%k1;\
    		addl $20,%%esp;\
		":"=a"(eax):"r"(addr));

	}else {

    		__asm__ __volatile__ ("\
		call *%k1;\
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
