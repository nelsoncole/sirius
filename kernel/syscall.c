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

#define SYSCALL_NUM 25

extern VOID interrupter(INTN n,UINT32 offset,UINT16 sel,UINT8 dpl );
extern void int114(); // system call
extern void int113(); // exit
extern void int112(); // taskswitch


static VOID *sys_malloc(UINTN size)
{
	void *addr = malloc(size);

    	return addr;
}

static VOID sys_free(VOID *buf)
{
    	free(buf);
}


extern void CPU_reset(void);
static VOID sys_reboot()
{
    	CPU_reset();
}


UINTN sys_do_exec_child(CONST CHAR8 *name,UINT8 prv) {

	return do_exec_child(current_thread,name,prv);

}

int syscall_exectve(int argc,char **argv,char *pwd,FILE *fp) {

	int pid = exectve(argc,argv,pwd,fp);

	if(pid) set_focus(pid);

	return pid;

}
int syscall_exectve_child(int argc,char **argv,char *pwd,FILE *fp)
{

	THREAD *father_thread = current_thread;

	return exectve_child(argc,argv,pwd,fp,father_thread);

}

int syscall_getpid() {

	return getpid();

}

int syscall_cheksum_pid(unsigned int pid)
{

	return cheksum_pid(pid);
}

int syscall_lockthread()
{
	return lockthread();
}

int syscall_unlockthread(unsigned int pid)
{

	return unlockthread(pid);

}

void syscall_taskswitch_pid(unsigned int pid) 
{
	taskswitch_pid(pid);
}

int syscall_read_sector(int p,int count,unsigned int lba32_47 ,void *buffer,unsigned int lba0_31) {

	UINT64 addr = ( (lba0_31 &0xFFFFFFFF) /*| (lba32_47 << 32) */);

	return read_sector(p,count,addr,buffer);

}

int syscall_write_sector(int p,int count,unsigned int lba32_47 ,void *buffer,unsigned int lba0_31) {

	UINT64 addr = ( (lba0_31 &0xFFFFFFFF) /*| (lba32_47 << 32) */);

	return write_sector(p,count,addr,buffer);

}

unsigned int syscall_sectors(int devnum)
{
	return ata_sectors(devnum);
}

unsigned int syscall_bps(int devnum)
{
	return ata_bps(devnum);

}

int syscall_ata_ptotal()
{
	return ata_ptotal();
}

unsigned int syscall_inpb(int p)
{
	return inpb(p);

}

unsigned int syscall_inpw(int p)
{
	return inpw(p);

}

unsigned int syscall_inpl(int p)
{
	return inpl(p);

}

void syscall_outpb(int p,unsigned int val)
{
	outpb(p,val);
}

void syscall_outpw(int p,unsigned int val)
{
	outpw(p,val);
}

void syscall_outpl(int p,unsigned int val)
{
	outpl(p,val);
}


void syscall_set_focus_kbdc(unsigned int pid)
{
	set_focus(pid);
}


void *syscall_lock() 
{	return (unsigned int*)system_lock;

}

extern int terminal;
void syscall_unknown(void){

	terminal = 1; // FIXME improvisorio, faz com que o kernel execute o Terminal

}


// eax, edx, ecx, ebx, edi, esi

VOID *syscall_table[SYSCALL_NUM]={
    	&syscall_unknown,	// eax, 0	null
    	&syscall_set_focus_kbdc,// eax, 1    	syscall_set_focus_kbdc, edx = pid
	&sys_malloc,       	// eax, 2    	sys_malloc, edx = size
	&sys_free,       	// eax, 3    	sys_free, edx = buf
	&sys_reboot,       	// eax, 4    	sys_reboot
	// ata
	&syscall_ata_ptotal,	// eax, 5	syscall_ata_ptotal
	// thread
	&syscall_exectve_child, // eax, 6	syscall_exectve_child, edx = argc, ecx = argv, ebx = pwd, edi = FILE
	&sys_do_exec_child,	// eax, 7	sys_do_exec_child, edx = filename, ecx = prv
	&syscall_exectve,	// eax, 8	syscall_exectve, edx = argc, ecx = argv, ebx = pwd, edi = FILE
	&syscall_getpid,	// eax, 9	syscall_getpid
	&syscall_lockthread,	// eax, 10	syscall_lockthread
	&syscall_unlockthread,	// eax, 11	syscall_unlockthread edx = pid
	&syscall_taskswitch_pid,// eax, 12	syscall_taskswitch_pid, edx = pid
	&syscall_cheksum_pid,	// eax, 13	syscall_cheksum_pid, edx = pid
	// ata
	&syscall_sectors,	// eax, 14	syscall_sectors, edx = devnum 
	&syscall_bps,		// eax, 15	syscall_bps, edx = devnum
	&syscall_read_sector,	// eax, 0x10	read_sector, edx = MediaID, ecx = count, ebx = LBA32-47, edi = buffer, esi = LBA0-31
	&syscall_write_sector,	// eax, 0x11	write_sector, edx = MediaID, ecx = count, ebx = LBA32-47, edi = buffer, esi = LBA0-31
	// io
	&syscall_inpb,		// eax, 0x12	syscall_inpb, edx = port
	&syscall_inpw,		// eax, 0x13	syscall_inpw, edx = port
	&syscall_inpl,		// eax, 0x14	syscall_inpl, edx = port
	&syscall_outpb,		// eax, 0x15	syscall_outpb, edx = port, ecx = val
	&syscall_outpw,		// eax, 0x16	syscall_outpw, edx = port, ecx = val
	&syscall_outpl,		// eax, 0x17	syscall_outpl, edx = port, ecx = val
	&syscall_lock,		// eax, 0x18

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

	// ring 3
	interrupter(0x70,(UINTN)int112,reg.cs,3);
	interrupter(0x71,(UINTN)int113,reg.cs,3);
    	interrupter(0x72,(UINTN)int114,reg.cs,3);
	
}
