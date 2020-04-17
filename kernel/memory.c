/*
 * File Name: memory.c
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

UINTN index_mem_map_pt = 0;
UINT8 *RamBMP = NULL;
UINT8 *AllocTablePages = NULL;



#define UMaskBit(addr)addr


#define RAM_BLOCK_SIZE 64


UINTN RamBitValue[8]={1,2,4,8,16,32,64,128};




/*
 * Repartiremos a memória fisica em blocos de 64 KiB
 *
 * 64 Blocks = 4 MiB
 * 65536 Blocks = 4 GiB
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */

unsigned int ram_initialize() 
{

	RamBMP = (unsigned char*) (_end);

	// For 4 GiB
	setmem((unsigned char*)RamBMP,(RAM_BLOCK_SIZE*1024)/8,0);

	// Mask first 48 MiB, reserved KERNEL_OF_NUM_PAGE_TABLE
	setmem((unsigned char*)RamBMP,(KERNEL_OF_NUM_PAGE_TABLE*RAM_BLOCK_SIZE)/8,-1);

	index_mem_map_pt = 0;


	return ((unsigned int)RamBMP);

}

static unsigned int ram_free()
{

	unsigned char *bmp = (unsigned char*)(RamBMP);
	
	unsigned int offset = (KERNEL_OF_NUM_PAGE_TABLE*RAM_BLOCK_SIZE);


	// determinar offset byte e bit
	int byte = offset /8;
	int bit  = (offset %8);


	while(TRUE/*deve depender da memoria maxima*/) {

	
		if( !(*(unsigned char*)(bmp + byte)&RamBitValue[bit]) ) break;

		offset++;
		byte = offset /8;
		bit  = (offset %8) &0xf;


	}


	return offset;

}


int alloc_frame(PAGE_TABLE *pt,int count,VIRTUAL_ADDRESS *frame)
{

	if(!count) 
	{
		*(VIRTUAL_ADDRESS*)(frame) = (unsigned int)0;
		return -1;
	}




	// First block
	unsigned int *__frame;

	alloc_pages(0,1,(VIRTUAL_ADDRESS *)&__frame);
	setmem((unsigned char*)__frame,0x1000,0);
	
	// OUT frist frame
	*(VIRTUAL_ADDRESS*)(frame) = (unsigned int)__frame;


	PAGE_TABLE *p_pt = pt;

	unsigned char *bmp = (unsigned char*)(RamBMP);
	unsigned int offset;
	unsigned int phy_addr;
	int byte;
	int bit;
	int i,t;


	for(i=0;i < count;i++)
	{

		// localizar area livre
		offset = ram_free();

		// salve offset
		*__frame++ = offset;


		// determinar offset byte e bit
		byte = offset /8;
		bit  = offset %8;

	
		// busy
		*(unsigned char*)(bmp + byte) |= RamBitValue[bit];


		// determine o endereço físico
		phy_addr = ( (offset*RAM_BLOCK_SIZE)*1024 );

		// alocar memória fisica
		for(t = 0; t < (RAM_BLOCK_SIZE/4);t++) {

			p_pt->frames = (phy_addr >>12) &0xFFFFF;
			p_pt++;
			phy_addr += 0x1000;
		}

		

	}

	return 0;

}


void free_frame(void *frame)
{


	if(!frame) return;

	unsigned char *bmp = (unsigned char*)(RamBMP);

	unsigned int *__frame = (unsigned int *) frame;
	unsigned int offset;
	int byte;
	int bit;
	int i;


	for(i=0;i<1024;i++)
	{
		offset = *__frame++;

		if(!offset) break;

		// determinar offset byte e bit
		byte = offset /8;
		bit  = offset %8;

		// busy
		*(unsigned char*)(bmp + byte) &=~RamBitValue[bit];;

	}


	

	free_pages(frame);

}



UINTN alloc_pages_initialize()
{

	AllocTablePages = (UINT8*) ((_end) + 0x1000*256);
	
	// For 32 MiB = (8192*4KiB)
	setmem((UINT8*)AllocTablePages,8192*sizeof(UINT8),0);

	return ((UINTN)AllocTablePages);
}

/**
 * Aloca bloco de 4KiB
 *
 **/
UINTN alloc_pages(	IN UINTN type,
			IN UINTN _size,
			OUT VIRTUAL_ADDRESS *addr)
{

	int i;
	char *tmp = (char*)AllocTablePages;
	size_t count;
	unsigned char flag;
	char *start_buf = NULL;
	int start;

	//chek space
	count = 0;
	flag = 1;
	start = (-1);
	for(i=0;i<8192;i++) {

		
		if(start == (-1)) { 
		
			start = i;
			start_buf = tmp; 

		}

		if(!(*tmp)) {

			count++;

			if(count == _size) {
				 
				flag = 0; break;
				 
			}
 

		}else {
		
			count = 0;
			start = (-1);

		}

		tmp++;
	}


	if(flag || i > 8192) {

		print("KERNEL PANIC: AllocTablePages()\n"); 
		for(;;);
	}

	// Marcar ocupado
	for(i=0;i < _size;i++){
 
		*start_buf = 0x11;
		start_buf++;

	}

	start_buf--;
	*start_buf = 0xFF;

	
	*(VIRTUAL_ADDRESS*)(addr) = (START_ALLOC_PAGES_VIRTUAL_ADDRESS + (0x1000*start));


	return (0);

}

VOID free_pages(IN VOID *addr)
{
	unsigned int start =( (END_ALLOC_PAGES_VIRTUAL_ADDRESS - START_ALLOC_PAGES_VIRTUAL_ADDRESS)/0x1000 ) -\
	( (END_ALLOC_PAGES_VIRTUAL_ADDRESS - ((unsigned)addr)) /0x1000);


	unsigned char *tmp1 = (unsigned char*) AllocTablePages;
	unsigned char *tmp  = (unsigned char*) (tmp1 + start);

	int i;


	for(i=0;i<8192;i++) {
	
		if(*tmp == 0xff) {
			*tmp = 0;
			return;

		}else *tmp++ = 0;


	}

}




UINTN mem_map(	IN PHYSICAL_ADDRESS phy_addr,
		OUT VIRTUAL_ADDRESS *virt_addr,
		IN UINTN size,
		IN UINTN flag) 
{

	UINTN d = (UINTN) (phy_addr >> 22 &0x3FF);
    	UINTN t = (UINTN) (phy_addr >> 12 &0x3FF);
    	//UINTN o = (UINTN) (phy_addr  &0xFFF); // unused



	PAGE_TABLE *pt 		= (PAGE_TABLE*)(((UINTN)kernel_page_table) +\
	(KERNEL_OF_NUM_PAGE_TABLE * 0x1000) + (t*4) + (index_mem_map_pt*0x1000));

	PAGE_DIRECTORY *pd 	= (PAGE_DIRECTORY*)(((UINTN)kernel_page_directory) + d*4);
	
	PAGE_TABLE *_pt 	= pt; 
	
	INTN i;
	UINT32 table_addr = 0;



	setmem(pt,sizeof(PAGE_TABLE)*size,0);

	// PTE de identidade 
	for(i =0; i < size; i++){
		
		pt->p = flag &1;
		pt->rw= (flag >>1)&1;
		pt->us= (flag >>2)&1;
		pt->pcd= (flag >>4)&1;
		pt->frames = ((phy_addr+(i*0x1000)) >>12) &0xFFFFF;

		pt++;
	
	}



	UINTN __s = size/1024;
	if(size%1024)__s += 1;

	// PDE de identidade
	for(i =0; i < __s; i++){
		pd->p = flag &1;
		pd->rw= (flag >>1)&1;
		pd->us= (flag >>2)&1;
		pd->pcd= (flag >>4)&1;
		table_addr =(UINT32)(_pt + (1024 *i));
		pd->addrpt = (table_addr >>12) &0xFFFFF;
		pd++;

		index_mem_map_pt++;
	}


	*(VIRTUAL_ADDRESS*)(virt_addr) = phy_addr;

	flush_tlb(); // Actualiza TLB



	return 0;


}


VOID *malloc(UINTN size) 
{
	VIRTUAL_ADDRESS *addr = NULL;

	UINTN _s = size/0x1000;
	if(size%0x1000)_s +=1;

	alloc_pages(0,_s,(VIRTUAL_ADDRESS *)&addr);
	
	return (VOID*) addr;
	
}


VOID free(VOID *buffer)
{
	if(!buffer) return;
	free_pages(buffer);

	//*(unsigned int*)(buffer) = 0;
} 
