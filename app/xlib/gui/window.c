/*
 * File Name: window.c
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


#include <stdlib.h>
#include <io.h>

extern char *font;

GW_HAND *create_window_mouse(long X,long Y, long Width, long Height,long Style,long Flag)
{

	GW_HAND *GwHand  = (GW_HAND*) malloc(sizeof(GW_HAND));

	// font buffer
	font = (CHAR8*) malloc(0x1000);
	copymem(font,font8x16,256*16);
	
	GwHand->X 		= X; // coordenada X
	GwHand->Y 		= Y; // Coordenada Y
	GwHand->Width	= Width; // largura
	GwHand->Height	= Height; // altura
	GwHand->Style	= Style;
	GwHand->Flag	= Flag;
	GwHand->Buffer	= (VOID*) G->BankBuffer; // 

	// area
	GwHand->Area.X 		= X; // area coordenada X
	GwHand->Area.Y 		= Y; // area Coordenada Y
	GwHand->Area.Width	= Width; // area largura
	GwHand->Area.Height	= Height; // area altura



	GwHand->box = CreateBox(GwHand,8,8,GwHand->Width,GwHand->Height, 
	GW_STYLE(FORE_GROUND(GwHand->Style &0xff) | BACK_GROUND(GwHand->Style >> 8 &0xff)),GW_FLAG_VISIBLE);
	

	unsigned int *p = (unsigned int*)0x10001120;
	MOUSE *mouse = (MOUSE*)(*p);

	mouse->handle = (unsigned int) GwHand;	


	return (GW_HAND*) GwHand;	

}

int update_window_mouse(GW_HAND *_GwHand)
{
	GW_HAND *GwHand = _GwHand;

	GwHand->Area.X 		= GwHand->X; // area coordenada X
	GwHand->Area.Y 		= GwHand->Y; // area Coordenada Y
	GwHand->Area.Width	= GwHand->Width; // area largura
	GwHand->Area.Height	= GwHand->Height; // area altura
	

	if(!(GwHand->Flag &GW_FLAG_VISIBLE)) return 1;


	// area do cliente
	DrawArea (GwHand->Area.X,GwHand->Area.Y,GwHand->Area.Width,GwHand->Area.Height, \
				ColorTable[GwHand->Style >> 8 &0xff],GwHand->Buffer);

	// Area da borda
	DrawRect (GwHand->X,GwHand->Y,GwHand->Width,GwHand->Height,ColorTable[GW_GRAY],GwHand->Buffer);



	// Data
	if((GwHand->box->Flag &GW_FLAG_READY)) {

		____UpdateBoxDrawString(GwHand,(UINT8*)(GwHand->Msg1));

			
	}
	
	
	
	
	return 0;
}























