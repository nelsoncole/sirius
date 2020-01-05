/*
 * File Name: paging.h
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
 
#ifndef __PAGING_H__
#define __PAGING_H__

#define KERNEL_OF_NUM_PAGE_TABLE 		12 		// For 48 MiB
#define START_ALLOC_PAGES_VIRTUAL_ADDRESS 	0x01000000  	// mark 16 MiB
#define END_ALLOC_PAGES_VIRTUAL_ADDRESS 	0x03000000  	// mark 48 MiB
#define APP_VIRTUAL_ADDRESS 			0x10000000  	// mark 256 MiB

typedef struct _PAE_PAGE_TABLE
	{
	UINT64 p :1;
	UINT64 rw :1;
	UINT64 us :1;
	UINT64 pwt :1;
	UINT64 pcd :1;
	UINT64 a :1;
	UINT64 d :1;
	UINT64 pat :1;
	UINT64 g :1;
	UINT64 ign2 :3;
	UINT64 frames :40;
	UINT64 rs2 :12;

	}__attribute__((packed)) PAE_PAGE_TABLE;

typedef struct _PAE_PAGE_DIRECTORY
	{
	UINT64 p :1;
	UINT64 rw :1;
	UINT64 us :1;
	UINT64 pwt :1;
	UINT64 pcd :1;
	UINT64 a :1;
	UINT64 ign1 :1;
	UINT64 ps :1;
	UINT64 ign2 :4;
	UINT64 phy_addr_pt :40;
	UINT64 rs2 :12;
	}__attribute__((packed)) PAE_PAGE_DIRECTORY;
	
	
//Page_directory_pointer_table
typedef struct _PAE_PAGE_DIRECTORY_POINTER_TABLE
	{
	UINT64 p :1;
	UINT64 rs0 :2;
	UINT64 pwt :1;
	UINT64 pcd :1;
	UINT64 rs1 :4;
	UINT64 ign :3;
	UINT64 phy_addr_pd :40;
	UINT64 rs2 :12;

	}__attribute__((packed)) PAE_PAGE_DIRECTORY_POINTER_TABLE;
	
typedef struct _PAGE_TABLE
	{
	UINT32 p :1;
	UINT32 rw :1;
	UINT32 us :1;
	UINT32 pwt :1;
	UINT32 pcd :1;
	UINT32 a :1;
	UINT32 d :1;
	UINT32 pat :1;
	UINT32 g :1;
	UINT32 ign :3;
	UINT32 frames :20;

	}__attribute__((packed)) PAGE_TABLE;

typedef struct _PAGE_DIRECTORY
	{
	UINT32 p :1;
	UINT32 rw :1;
	UINT32 us :1;
	UINT32 pwt :1;
	UINT32 pcd :1;
	UINT32 a :1;
	UINT32 ign :1;
	UINT32 ps :1;
	UINT32 ignored :4;
	UINT32 addrpt :20;

	}__attribute__((packed)) PAGE_DIRECTORY;
	
VOID load_page_diretory(PAGE_DIRECTORY  *phy_addr);
VOID __native_flush_tlb_single(UINT32 addr);
VOID flush_tlb();
VOID page_install(VOID);
	




#endif
