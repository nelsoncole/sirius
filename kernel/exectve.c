/*
 * File Name: exectve.c
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




UINTN __do_exec_child(THREAD *father_thread,CONST CHAR8 *name,UINT8 prv)
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


	UINTN d = (UINTN) (addr >> 22 &0x3FF);
    	//UINTN t = (UINTN) (addr >> 12 &0x3FF);
    	//UINTN o = (UINTN) (addr  &0xFFF); // unused


	HEADER *header = (HEADER*)(ZERO);

	FILE *fpx = open (name,"rx");

	if(fpx == NULL) { 
		print("open \"%s\" error\n",name);
		return 0;
	}

	
	rewind(fpx);
	if(read(header,1,0x2000,fpx) != 0x2000) { 
		print("read header \"%s\"error\n",name);
		close(fpx);
		return 0;
	} else{
		for(int i = 0;i < (2048 - (sizeof(HEADER)));i++) {
			header = (HEADER*) ( (unsigned int) header + 4);
			if(header->magic == HEADER_MAGIC)break;				
				
		}

		// cheksum
		if(header->magic != HEADER_MAGIC) {
			return 0;
		}else {

			rewind(fpx);
		}
			
	}


	int filesize = fpx->header.size;
	int physize = (header->end -  (header->start - 0x1000));	



	// allocar memoria
	alloc_pages(0,1,(VIRTUAL_ADDRESS *)&pd);
	alloc_pages(0,1,(VIRTUAL_ADDRESS *)&pt);

	// copiar o directory do kernel
	copymem(pd,kernel_page_directory,0x1000);

	// limpar nova pt
	setmem((UINT8*)pt,0x1000,0);




	// Page Table

	_pt = pt;

	int pagesize = (physize/4096);
	if((physize%4096)) pagesize++;

	for(i = 0; i < pagesize +1;i++){

		_pt->p = 1;
		_pt->rw= 1;

		if((prv&1) == 1)_pt->us= 1;

		_pt++;
	}


	FRAME *frame = NULL;


	// Um bloco e de 64 KiB
	int physblocks = ((pagesize*4)/64);
	if(((pagesize*4)%64)) physblocks++;

	alloc_frame(pt,physblocks +1,(VIRTUAL_ADDRESS *)&frame);

	

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
	wait_ns(10000);


	if(/*Read(fpx,(void*)addr)*/read((void*)addr,1,filesize,fpx) != filesize) {
		print("read \"%s\"error\n",name);
		free_pages(pt);
		free_pages(pd);
		free_frame(frame);
		close(fpx);
		return 0;

	} else {

 

		// Copy struct GUI
		copymem((UINT8*)header->start + 0x20,(UINT8*)0x101038,sizeof(GUI));

		// envia o PID
		p  	= (UINT32*)(header->start + 0x114);
		*p++ 	= next_pid;
		*p	= (UINT32)focus;


		if((prv&1) == 1)esp0 =(UINT32)(&stack_esp_0);/*(UINTN)malloc(0x2000);*/

		pid = create_thread_child(father_thread,(void*)(header->start),pd,0,(header->start + 0x20)/*Boot info*/,0,0,header->stack,\
			esp0/*ESP0*/,prv);



		// salve frame

		while(thread->pid != pid)
			thread = thread->next;

		thread->frame = frame;

				
	}

	close(fpx);

	load_page_diretory(oldpd);
	flush_tlb();


    	return (pid);



}










int exectve(int argc,char **argv,FILE *fp)
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


	UINTN d = (UINTN) (addr >> 22 &0x3FF);
    	//UINTN t = (UINTN) (addr >> 12 &0x3FF);
    	//UINTN o = (UINTN) (addr  &0xFFF); // unused


	HEADER *header = (HEADER*)(ZERO);
	
	rewind(fp);
	if(read(header,1,0x2000,fp) != 0x2000) { 
		//print("read header \"%s\"error\n",fp->header.filename);
		return 0;
	} else{
		for(int i = 0;i < (2048 - (sizeof(HEADER)));i++) {
			header = (HEADER*) ( (unsigned int) header + 4);
			if(header->magic == HEADER_MAGIC)break;				
				
		}

		// cheksum
		if(header->magic != HEADER_MAGIC) {
			return 0;
		}else {

			rewind(fp);
		}
			
	}


	int filesize = fp->header.size;
	int physize = (header->end -  (header->start - 0x1000));	



	// allocar memoria
	alloc_pages(0,1,(VIRTUAL_ADDRESS *)&pd);
	alloc_pages(0,1,(VIRTUAL_ADDRESS *)&pt);

	// copiar o directory do kernel
	copymem(pd,kernel_page_directory,0x1000);

	// limpar nova pt
	setmem((UINT8*)pt,0x1000,0);




	// Page Table

	_pt = pt;

	int pagesize = (physize/4096);
	if((physize%4096)) pagesize++;

	for(i = 0; i < pagesize +1;i++){

		_pt->p = 1;
		_pt->rw= 1;
		_pt->us= 1;
		_pt++;
	}


	FRAME *frame = NULL;


	// Um bloco e de 64 KiB
	int physblocks = ((pagesize*4)/64);
	if(((pagesize*4)%64)) physblocks++;

	alloc_frame(pt,physblocks +1,(VIRTUAL_ADDRESS *)&frame);

	

	// Page Directory
	pd[d].p = 1;
	pd[d].rw= 1;
	pd[d].us= 1;

	table_addr =(UINT32)(pt);
	pd[d].addrpt = (table_addr >>12) &0xFFFFF;

	__asm__ __volatile__("movl %%cr3,%%eax":"=a"(oldpd));

	load_page_diretory(pd);
	//flush_tlb();
	// Espera o MMU, completar a primeira traducao 
	wait_ns(10000);


	if(read((void*)addr,1,filesize,fp) != filesize) {
		//print("read \"%s\"error\n",fp->header.filename);
		free_pages(pt);
		free_pages(pd);
		free_frame(frame);
		return 0;

	} else {

 

		// Copy struct GUI
		copymem((UINT8*)header->start + 0x20,(UINT8*)0x101038,sizeof(GUI));

		// envia o PID
		p  	= (UINT32*)(header->start + 0x114);
		*p++ 	= next_pid;
		*p++	= (UINT32) focus;
		*p++	= (UINT32) GwFocus;
		*p++	= (UINT32) mouse;
		*p++	= (UINT32) rtc;
		


		esp0 =(UINT32)(&stack_esp_0);/*(UINTN)malloc(0x2000);*/
		pid = create_thread((void*)(header->start),pd,(unsigned int)argv,(header->start + 0x20)/*Boot info*/,argc,0,header->stack,\
			esp0/*ESP0*/,1 /*prv*/);


		// salve frame
		while(thread->pid != pid)
			thread = thread->next;

		thread->frame = frame;

				
	}

	load_page_diretory(oldpd);
	flush_tlb();


    	return (pid);

}
