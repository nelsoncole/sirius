/*
 * File Name: paging.c
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


PAGE_DIRECTORY *kernel_page_directory = (PAGE_DIRECTORY*) OS_PAGE_ADDR; 
PAGE_TABLE *kernel_page_table = (PAGE_TABLE*) (OS_PAGE_ADDR + 0x1000); 


VOID load_page_diretory(PAGE_DIRECTORY  *phy_addr)
{

	__asm__ __volatile__("movl %k0,%%cr3"::"r"(phy_addr));
	wait_ns(120); // Wait Translation Lookaside Buffer (TLB)
}

VOID page_enable()
{	__asm__ __volatile__("movl %%cr0,%%eax;"
			     "orl $0x80000000,%%eax;"
			     "movl %%eax,%%cr0"::);
}
VOID __native_flush_tlb_single(UINT32 addr)
{
   	__asm__ __volatile__("invlpg (%0)" ::"r" (addr) : "memory");
	wait_ns(120);
}

VOID flush_tlb()
{
  	UINT32 addr;
   	__asm__ __volatile__("movl %%cr3,%%eax" :"=a" (addr):);
   	__asm__ __volatile__("movl %%eax,%%cr3" ::"a" (addr));
	wait_ns(120);

	
}

VOID page_install(VOID)
{	
	INTN i;
	UINT32 table_addr = 0;
	UINT32 physical_addr = 0;
	
	PAGE_TABLE *pt = kernel_page_table;
	PAGE_DIRECTORY *pd =kernel_page_directory;

	setmem((PAGE_DIRECTORY*)kernel_page_directory,sizeof(PAGE_DIRECTORY)*1024,0);
	setmem((PAGE_TABLE*) kernel_page_table,sizeof(PAGE_TABLE)*KERNEL_OF_NUM_PAGE_TABLE*1024,0);
	
	__asm__ __volatile__("cli");

	
	// PTE de identidade 48 MiB
	for(i =0; i < (1024*KERNEL_OF_NUM_PAGE_TABLE); i++) {
		
		pt->p = 1;
		pt->rw= 1;
		pt->us= 1;
		pt->frames = ((physical_addr+(i*0x1000)) >>12) &0xFFFFF;

		pt++;
	
	}

	// PDE de identidade 48 MiB
	for(i =0; i < KERNEL_OF_NUM_PAGE_TABLE; i++){
		pd->p = 1;
		pd->rw= 1;
		pd->us= 1;
		table_addr =(UINT32)(kernel_page_table + (1024 *i));
		pd->addrpt = (table_addr >>12) &0xFFFFF;
		pd++;
	}


	load_page_diretory(kernel_page_directory);
	
	page_enable();

	// Espera o MMU, completar a primeira traducao 
	wait_ns(90000000);
}
