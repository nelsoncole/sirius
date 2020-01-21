/*
 * File Name: exec.c
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


UINTN do_exec(CONST CHAR8 *name,UINT8 prv)
{
	VIRTUAL_ADDRESS addr = APP_VIRTUAL_ADDRESS;
	UINTN pid 	= 0;
	UINTN esp0 	= 0;
	UINT32 *p	= NULL;
	THREAD *thread 	= thread_ready_queue;

	INTN i;
	UINT32 table_addr = 0;

	PAGE_DIRECTORY *pd,*oldpd;
	PAGE_TABLE *pt, *_pt;

	// allocar memoria
	alloc_pages(0,1,(VIRTUAL_ADDRESS *)&pd);
	alloc_pages(0,1,(VIRTUAL_ADDRESS *)&pt);

	// copiar o directory do kernel
	copymem(pd,kernel_page_directory,0x1000);

	// limpar nova pt
	setmem((UINT8*)pt,0x1000,0);



	UINTN d = (UINTN) (addr >> 22 &0x3FF);
    	//UINTN t = (UINTN) (addr >> 12 &0x3FF);
    	//UINTN o = (UINTN) (addr  &0xFFF); // unused


	// Page Table

	_pt = pt;

	

	for(i = 0; i < 1024;i++){

		_pt->p = 1;
		_pt->rw= 1;

		if((prv&1) == 1)_pt->us= 1;

		_pt++;
	}


	FRAME *frame = NULL;

	alloc_frame(pt,4096/64/*4 MiB*/,(VIRTUAL_ADDRESS *)&frame);

	

	// Page Directory
	pd[d].p = 1;
	pd[d].rw= 1;

	if((prv&1) == 1)pd[d].us= 1;

	table_addr =(UINT32)(pt);
	pd[d].addrpt = (table_addr >>12) &0xFFFFF;

	__asm__ __volatile__("movl %%cr3,%%eax":"=a"(oldpd));

	load_page_diretory(pd);
	//flush_tlb();
	// Espera o MMU, completar a primeira traducao 
	wait_ns(1000000);

		//FIXME

	if(FatOpenFile(__bpb__,__data__,__root__,name,0,__vfs__)) {
		print("OpenFile %s Error\n",name);

		free_pages(pt);
		free_pages(pd);
		free_frame(frame);

	}else {

		if(Read(__vfs__,(VOID*)addr) ) {
		print("FileRead Error %s\n",name);
		free_pages(pt);
		free_pages(pd);
		free_frame(frame);
	} else {

 

		// Copy struct GUI
		copymem((UINT8*)addr + 0x1020,(UINT8*)0x101038,sizeof(GUI));

		// envia o PID
		p  	= (UINT32*)(addr + 0x1114);
		*p++ 	= next_pid;
		*p	= (UINT32)focus;


		if((prv&1) == 1)esp0 =(UINT32)(&stack_esp_0);/*(UINTN)malloc(0x2000);*/

		pid = create_thread((VOID*)(addr + 0x1000),pd,0,(addr + 0x1020)/*Boot info*/,0,0,0x101FFFF0/*ESP*/,\
			esp0/*ESP0*/,prv);


		// salve frame

		while(thread->pid != pid)
			thread = thread->next;

		thread->frame = frame;

		}
				
	}


	

	load_page_diretory(oldpd);
	flush_tlb();


    	return (pid);

}


