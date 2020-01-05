/*
 * File Name: mboot.c
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


// esta macro conver seg:offset para flat
#define __ptr_cov32(p)\
( (16 * (p >>16 &0xFFFF)) + (p &0xFFFF) )


extern UINT8 bootdevice_num;
GUI g[1];

VOID initialize_gui()
{

		// BIOS VESA Video and Audio
	UINT8 *vbe = (UINT8*)0x40200;
	UINT8 *vbe_info_mode = (UINT8*)0x40200 + 0x200;



	// GUI
	g->Pitch 		= (*(UINT16*)(vbe_info_mode + 16));
	g->Bpp 			= (*(UINT8*)(vbe_info_mode  + 25));
	g->Planes		= (*(UINT8*)(vbe_info_mode  + 24));
	g->HorizontalResolution	= (*(UINT16*)(vbe_info_mode + 18));  	//	width;
	g->VerticalResolution	= (*(UINT16*)(vbe_info_mode + 20)); 	//	hight;
	g->PixelsPerScanLine	= g->HorizontalResolution;//GraphicsOutputProtocol->Mode->Info->PixelsPerScanLine;	
	g->MaxPixelClock	= 0;
	g->FrameBuffer		= (VOID*) (*(UINT32*)(vbe_info_mode + 40));
	g->FrameBufferSize	= (*(UINT16*)(vbe + 18));
	g->BankBuffer		= (VOID*) alloc(1024*2); // 8MB
	g->TaskBuffer		= NULL;
	g->WindowScreen		= NULL;
	g->WindowBackground	= NULL;
			
	g->Area.HorizontalStart = 0;
	g->Area.VerticalStart 	= 0;
	g->Area.HorizontalEnd 	= g->HorizontalResolution;
	g->Area.VerticalEnd 	= g->VerticalResolution;



}

VOID mboot(UINT32 arg)
{

	CHAR8 *mboot = (CHAR8*)arg;
	CHAR8 *tmp = (CHAR8*)0;
	//uint32_t data = 0;
	//int i;



	// Boot Device Info
	// BIOSes Get Device Parameters (INT 13h FN 48h)
	CHAR8 *bootDevice = (CHAR8*)0x40000;
	(*(UINT32*)(mboot + 0x0)) = UID;	// UID
	(*(UINT32*)(mboot + 0x4)) = HDN;	// Boot Device Number
	(*(UINT32*)(mboot + 0x8)) = 0;
	(*(UINT32*)(mboot + 0xC)) = 0;
	// Device Parameter Table Extension (DPTE)
	// Pointer to the Device Parameter Table Extension (DPTE), flat 32-bits.
	tmp =(CHAR8*) (16 * ((*(UINT32*)(bootDevice+26)) >>16 &0xFFFF) + ((*(UINT32*)(bootDevice+26)) &0xFFFF));
	(*(UINT32*)(mboot + 0x10)) = (UINT32)tmp;

	// Copia parametro GUI
	copymem(mboot + 0x30,g,sizeof(GUI));


	// Physical Memoey Map (INT 0x15 AX,E820h)
	// 8 bytes RAM SIZE 




}
