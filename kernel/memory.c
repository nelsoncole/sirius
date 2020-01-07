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


extern UINTN _end();

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
UINTN ram_initialize() 
{

	RamBMP = (UINT8*) _end;


	// For 4 GiB
	setmem((UINT8*)RamBMP,(RAM_BLOCK_SIZE*1024)/8,0);

	// Mask first 48 MiB, reserved KERNEL_OF_NUM_PAGE_TABLE
	setmem((UINT8*)RamBMP,(KERNEL_OF_NUM_PAGE_TABLE*RAM_BLOCK_SIZE)/8,-1);

	index_mem_map_pt = 0;


	return ((UINTN)RamBMP);

}


static UINTN ram_free()
{

	UINT8 *p = (UINT8*)(RamBMP);
	
	UINTN offset = (KERNEL_OF_NUM_PAGE_TABLE*RAM_BLOCK_SIZE);

	// determinar offset byte e bit
	UINTN byte = (offset - 1) /8;
	UINTN bit  = ((offset -1) %8) &0xf;


	while(TRUE) {

	
		if( !(*(UINT8*)(p + byte)&RamBitValue[bit]) )break;
		offset++;
		byte = (offset - 1) /8;
		bit  = ((offset -1) %8) &0xf;


	}


	return offset;

}

UINTN alloc_frame(IN OUT PAGE_TABLE *pt,IN UINTN size,OUT VIRTUAL_ADDRESS *frame)
{

	// First block
	FRAME *__frame = (FRAME*)malloc(sizeof(FRAME));
	FRAME *__new_frame = NULL;


	PAGE_TABLE *_pt = pt;

	UINT8 *p = (UINT8*)(RamBMP);
	UINTN offset;
	UINTN byte;
	UINTN bit;
	UINTN phy_addr;

	UINTN i,i2,i3 = size;


	// OUT frist frame
	*(VIRTUAL_ADDRESS*)(frame) = (UINTN)__frame;


	for(i = 0; i < size; i++) {

		i3--;

		offset = ram_free();

		// determinar offset byte e bit
		byte = (offset - 1) /8;
		bit  = ((offset -1) %8) &0xf;

	
		// busy
		*(UINT8*)(p + byte) |=  1 << bit;


		// determine o endereço físico
		phy_addr = ( (offset*RAM_BLOCK_SIZE)*1024 );

		// alocar memória fisica
		for(i2 = 0; i2 < (RAM_BLOCK_SIZE/4);i2++) {

			_pt->frames = (phy_addr >>12) &0xFFFFF;
			_pt++;
			phy_addr += 0x1000;
		}

		__frame->offset = offset;

		if(i3) {

			__new_frame = (FRAME*)malloc(sizeof(FRAME));

			__frame->next = __new_frame;

			__frame	      = __new_frame;

		} else __frame->next	= NULL;


	}

	

	return 0;

}

UINTN free_frame(IN VOID *frame)
{

	UINT8 *p = (UINT8*)(RamBMP);

	FRAME *__frame = (FRAME *) frame;
	FRAME *__current_frame = NULL;

	UINTN offset;
	UINTN byte;
	UINTN bit;

	while(__frame) {


		offset = __frame->offset;

		// determinar offset byte e bit
		byte = (offset - 1) /8;
		bit  = ((offset -1) %8) &0xf;

	
		// busy
		*(UINT8*)(p + byte) &=  ~1 << bit;

		__current_frame = __frame;

		__frame = __frame->next;

		free(__current_frame);
	}

	
	



	return 0;

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
			IN UINTN size,
			OUT VIRTUAL_ADDRESS *addr)
{


	UINTN _i=0,i=0,_ok = 0;
	UINT8 *p = (UINT8*) AllocTablePages;
	UINT8 *_p;


	// check free pool and define index
	while(TRUE){
	
		if(*p == 0) _ok++;
		else if(_ok)_ok = 0; //reiniciar

		_i++;
		p++;
	
		if(_ok == size)break;
	

	}
	

	// check space
	if(_i > ((END_ALLOC_PAGES_VIRTUAL_ADDRESS - START_ALLOC_PAGES_VIRTUAL_ADDRESS)/0x1000) ){
		cli();
		print("Kernel panic: alloc pages error\n");
		for(;;);
	}

	// assert _p
	for(i=0;i < _ok ; i++){
		p--;
		_i--;
	}

	
	_p = p;

	// Mask
	for(i =0;i < size;i++){
		*_p++ = 1;
	}

	// eof
	*--_p = 0xff;

	*(VIRTUAL_ADDRESS*)(addr) = (START_ALLOC_PAGES_VIRTUAL_ADDRESS + (0x1000*_i));
	

	return (0);
}

VOID free_pages(IN VOID *addr)
{
	UINTN _i,i = (((UINTN)addr) - START_ALLOC_PAGES_VIRTUAL_ADDRESS)/0x1000;

	

	UINT8 *p = (UINT8*) AllocTablePages;
	UINT8 eof = 0;

	for(_i = 0;eof != 0xff;_i++){

		eof = *(UINT8*)(p + i + _i);

		*(UINT8*)(p + i + _i) = 0 &0xff;
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
	free_pages(buffer);
} 
