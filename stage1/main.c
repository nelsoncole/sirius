/*
 * File Name: main.c
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

#include <io.h>

extern int gpu();

extern UINT8 bootdevice_num;
extern VOID *FAT;

UINT32 UID = 0; //Unique Disk ID
UINT32 HDN = 0; //Boot Device

UINT32 HEAP_BASE;
VOID *alloc(UINTN size)
{		

	UINT32 addr = HEAP_BASE;
	HEAP_BASE += (0x1000 * size);
	return ((VOID*)addr);
}

VOID main()
{

	// Initialize HEAP
	HEAP_BASE = 0x01000000; // 16 MiB

	initialize_gui();
	G = g;
	font = (CHAR8 *)(font8x16);

	// clear screen
	// Limpar o BankBuffer
	clearscreen();


	GW_HAND *Window = (GW_HAND*) alloc(1/*sizeof(GW_HAND)*/); // 4KiB 

	Window->X 	= 0; // coordenada X
	Window->Y 	= 0; // Coordenada Y
	Window->Width	= G->HorizontalResolution; // largura
	Window->Height	= G->VerticalResolution; // altura
	Window->Buffer	= (VOID*) G->BankBuffer; // 

	

	Window->box = CreateBox(Window,0,0,Window->Width,Window->Height, 
	GW_STYLE(FORE_GROUND(GW_WHITE) | BACK_GROUND(GW_BLACK)),0);
	Box =	Window->box;

	// console
	__buffer__ = (CHAR8*)alloc(1);
	__string__ = __buffer__ + 512;

	// clear screen
	ClearScreen();

	
	print("Sirius Operating System (SLoader v2.0)\
	\nGUI Info:\
	\nPitch               %d\
	\nBpp                 %d\
	\nPlanes              %d\
	\nWidth               %d\
	\nHeight              %d\
	\nPixelsPerScanLine   %d\
	\nMaxPixelClock       %d\
	\nFrameBuffer         0x%x\
	\nFrameBufferSize     %d MiB\
	\nBankBuffer          0x%x\
	\nGW_HAND             0x%x\n",
	g->Pitch,
	g->Bpp,
	g->Planes,
	g->HorizontalResolution,
	g->VerticalResolution,
	g->PixelsPerScanLine,	
	g->MaxPixelClock,
	g->FrameBuffer,
	((g->FrameBufferSize)*64)/1024,
	g->BankBuffer,
	Window);

	/*gpu();
	for(;;);*/


    	idt_install();
	ata_initialize();
    	//timer_install(1);
    	//keyboard_install();


	// Configure HDN
	UINT32 *tmp;
	MBR *mbr = (MBR*)alloc(1);
	for(HDN = 0;HDN < 4;HDN++) {
		if(read_sector(HDN,1,0,mbr) ) print("Read MBR error Dev%\n",HDN);
		else {
			tmp = (UINT32*)mbr;
	
			if(tmp[0x1B8/4] == UID)break;

		}
	}

	// Verifica a existencia de erro
	if(HDN >= 4) {

		print("Boot Device Error\n");
		for(;;);
	}

	print("Unique Disk ID 0x%x\nBoot Device Number %d\n",UID,HDN);

	UINT32 addr;
   	__asm__ __volatile__("movl %%cr4,%%eax" :"=a" (addr):);


	if(read_sector(HDN,1,0,mbr) )
		print("Read MBR error\n");
	else
		print("Unique Disk ID 0x%x\nDrive attributes 0x%x\nPartition type 0x%x\
		\nLBA of partition start %d\nPartition size = %d MiB\n",
		mbr->unique_disk_id,mbr->part[0].dev_attr,mbr->part[0].part_type,
		mbr->part[0].lba_start,(mbr->part[0].num_sectors*512)/1024/1024);

	FAT = (VOID*)alloc(1);
	VOLUME v;
	FAT_DATA data;
	FAT_DIRECTORY *root = NULL;
	VFS *vfs = (VFS*)alloc(1);

	v.lba_start = mbr->part[0].lba_start;
	v.dev_num = HDN;

	FAT_BPB *bpb = FatReadBPB(&v);
	if(!bpb) print("Erro bpb");
	else root =FatOpenRoot(bpb,&data);

	if(root){
		if(FatOpenFile(bpb,&data,root,"LOGO.BMP",0,vfs))
			print("OpenFile LOGO.BMP Error\n");

		else {
			CHAR8 *buffer = (CHAR8*)0xA00000;//LOGO.BMP
			if(Read(vfs,buffer) ) print("Read LOGO.BMP Error\n");
			else {

				if(BitMAP((UINT32*)buffer,260,130,G->BankBuffer));
				else refreshrate();
			}

		}


		if(FatOpenFile(bpb,&data,root,"KERNEL.BIN",0,vfs))
			print("OpenFile KERNEL.BIN Error\n");

		else {
			if(Read(vfs,(CHAR8*)0x100000) ) print("Read KERNEL.BIN Error\n");
			else {

				mboot((UINT32)0x101008);
				execkernelx86((UINT32)0x101000/*startkernel*/,(UINT32*)0x101008/* ebx*/);

			}
		}



	}else print("Error root directory");
	print("SLoader Panic\n");
	for(;;);//Panic

	
}
