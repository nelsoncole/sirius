/*
 * File Name: bmp.c
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

UINTN BitMAP(	VOID *Data,	
		UINTN X,
		UINTN Y,
		VOID *BankBuffer)
{
	BMP *bmp = (BMP*) Data;
	UINT32 *paleta = (UINT32*)(((UINTN)bmp) + 40 + 14);
	UINT8 *data_area = (UINT8*) (((UINTN)bmp) + bmp->bformat.offset_bits);
	UINTN color;
	UINTN x,y,i = 0;



	if(bmp->bformat.type != 0x4D42){
		print("BitMAP error\n");
		return -1;

	}


	

	for(y = bmp->binfo.height; y > 0; y--) {
	for(x = 0; x < bmp->binfo.width;  x++) {
	
		// <= 8-Bit
		if(bmp->binfo.count <= 8) {
		if(bmp->binfo.count == 4) {
		
		print("Not suport BitMAP 4-bit");
		return 2;
		
		}else
		color = *(UINT32*)((paleta) + *(UINT8*/*count*/)(((UINTN)data_area) + i++));
		
		// > 8-Bit
		}else if(bmp->binfo.count > 8){



		if(bmp->binfo.count == 24){
	
			color =  (*(UINT32*)((data_area) + (3*i++))) &0xffffff;

		}else if(bmp->binfo.count == 32){

			color =  (*(UINT32*)((data_area) + (4*i++))) &0xffffffff;

		}else {
			print("Not suport BitMAP > 8-bit");
			return 2;
		}
		
	
		}

		
		PutPixelBuff(X + x,Y + y,color,BankBuffer);

	}
	}



	return 0;

}



















