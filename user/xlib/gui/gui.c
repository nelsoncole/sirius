/*
 * File Name: gui.c
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


#include <ctype.h>
#include <stdlib.h>
#include <io.h>

GUI *G = NULL;

GW_HAND	*GlobalGwFocus = NULL;
GW_HAND	*GlobalObjectFocus = NULL;
GW_HAND	*Box = NULL;

CHAR8 *font = NULL;

UINT32 ColorTable[16] = {0x000000,0x0000FF,0x008000,0x00ffff,0xFF0000,0xAA00AA,\
0xAA5500,0x808080,0x555555,0x5555FF,0x55FF55,0x55FFFF,0xFF5555,0xFF55FF,0xFFFF00,0xFFFFFF};



VOID PutPixel(UINTN X, UINTN Y, UINTN Color)
{
	
	*(UINT32*)((UINT32*)G->FrameBuffer + (G->PixelsPerScanLine * Y) + X) = Color;
	
}

VOID PutPixelBuff(UINTN X, UINTN Y, UINTN Color,VOID *Buffer)
{
	
	UINT32 *buf = (UINT32*)Buffer;

	if((X >= G->HorizontalResolution) || (Y >= G->VerticalResolution) ) return;	

	*(UINT32*)((UINT32*)buf + (G->PixelsPerScanLine * Y) + X) = Color;
	
}

// PixelClock default
VOID RefreshRate() 
{
	copymem(G->FrameBuffer,G->BankBuffer,G->HorizontalResolution * G->VerticalResolution * (G->Bpp/8));
	
}


VOID APP_RefreshRate() 
{
	__copymem(G->FrameBuffer,G->BankBuffer,\
	G->HorizontalResolution * (G->VerticalResolution - W_TASK_BARRA_SIZE) * (G->Bpp/8));
	
}

VOID app_refreshrate() 
{
	__copymem(G->FrameBuffer,G->BankBuffer,\
	G->HorizontalResolution * (G->VerticalResolution - W_TASK_BARRA_SIZE) * (G->Bpp/8));
	
}


VOID app_clearscreen() 
{
	__setmem(G->BankBuffer,0,(G->HorizontalResolution * G->VerticalResolution\
	* (G->Bpp/8)));
}


VOID TASK_RefreshRate() 
{
	__copymem(G->FrameBuffer + (G->HorizontalResolution * \
	(G->VerticalResolution - W_TASK_BARRA_SIZE) * (G->Bpp/8)),G->TaskBuffer,\
	G->HorizontalResolution * W_TASK_BARRA_SIZE * (G->Bpp/8));
	
}

VOID TASK_Clearscreen() 
{
	__setmem(G->TaskBuffer,0,G->HorizontalResolution * W_TASK_BARRA_SIZE * (G->Bpp/8));
}



UINTN LineDrawHorizontal ( 	UINTN X1,
							UINTN X2,
                       		UINTN Y, 
							UINTN Color,
							VOID *Buffer) 
{

	INTN i;
	for(i = 0; i < X2; i++) {

		PutPixelBuff(X1 + i,Y,Color,Buffer);

		

	}
	return (X2 - X1);
}


UINTN LineDrawVertical ( 	UINTN Y1,
							UINTN Y2,
                       		UINTN X, 
							UINTN Color,
							VOID *Buffer)
{

	INTN i;
	for(i = 0; i < Y2; i++) {

		PutPixelBuff(X,Y1 + i,Color,Buffer);
	
	}

	return (Y2 - Y1);

}


UINTN DrawRect (UINTN X1,
				UINTN Y1,
				UINTN X2,
                UINTN Y2, 
				UINTN Color,
				VOID *Buffer)
{
	// Top
	LineDrawHorizontal ( X1, X2, Y1,Color,Buffer);
	
	// Left
	LineDrawVertical (Y1,Y2,X1,Color,Buffer);
	
	// bottom
	LineDrawHorizontal ( X1, X2, Y1 + Y2,Color,Buffer);
	
	// right
	LineDrawVertical (Y1,Y2 + 1,X1 + X2,Color,Buffer);

	return ((X2 - X1) * (Y2 - Y1));

}

UINTN DrawRectC (UINTN X1,
				UINTN Y1,
				UINTN X2,
                UINTN Y2, 
				UINTN Color,
				VOID *Buffer)
{
	// Top
	LineDrawHorizontal ( X1+1, X2-1, Y1,Color,Buffer);
	
	// Left
	LineDrawVertical (Y1+1,Y2-1,X1,Color,Buffer);
	
	// bottom
	LineDrawHorizontal ( X1+1, X2-1, Y1 + Y2,Color,Buffer);
	
	// right
	LineDrawVertical (Y1+1,Y2-1,X1 + X2,Color,Buffer);

	return ((X2 - X1) * (Y2 - Y1));

}

UINTN DrawArea (UINTN X1,
				UINTN Y1,
				UINTN X2,
                UINTN Y2, 
				UINTN Color,
				VOID *Buffer)
{

	UINTN i;
	
	for(i = 0; i < Y2; i++) {
		
		LineDrawHorizontal ( X1, X2, Y1 + i,Color,Buffer);
	}
	return ((X2 - X1) * (Y2 - Y1));

}


VOID DrawChar( 	UINTN X,
				UINTN Y,	
				UINTN C,
				UINTN FgColor,
				UINTN BgColor, 
				VOID *Buffer,
				VOID *FontBuffer )
{
   
	UINT16 font = 0;
	CONST UINT16 *font_data = (CONST UINT16*) FontBuffer;
   	INTN cx, cy;
	UINT16 mask;
    
   for(cy=0;cy < 16;cy++){
		mask = 1;
        font = font_data[(C *16) + cy];
		
                for(cx = 8 -1 ;cx >= 0;cx--){
 
                		if(font &mask)
				PutPixelBuff(X + cx,Y + cy,FgColor,Buffer);
						
                        else 
				PutPixelBuff(X + cx,Y + cy,BgColor,Buffer);
						
				mask += mask;
          }
        
    }
        
    

}

VOID DrawCharTransparent(	UINTN X,
							UINTN Y,	
							UINTN C,
							UINTN FgColor, 
							VOID *Buffer,
							VOID *FontBuffer)
{
	UINT16 font = 0;
	CONST UINT16 *font_data = (CONST UINT16*) FontBuffer;
   	INTN cx, cy;
	UINT16 mask;
    
   for(cy=0;cy < 16;cy++){
		mask = 1;
        font = font_data[(C *16) + cy];
		
                for(cx = 8 -1 ;cx >= 0;cx--){ 
                       if(font &mask)
			PutPixelBuff(X + cx,Y + cy,FgColor,Buffer);
			mask += mask;
                  
          }
        
    }

}				
				

UINTN InitializeBoxDraw(GW_HAND *_Hand,
						UINTN X,	
						UINTN Y,
						UINTN FgColor,
						UINTN BgColor,
						VOID *Buffer,
						VOID *FontBuffer)
{
	

	GW_HAND *Hand = _Hand;
	
	Hand->Font.FontSizeX 	= X;
	Hand->Font.FontSizeY 	= Y;
	Hand->Font.FgColor 		= FgColor;
	Hand->Font.BgColor 		= BgColor;
	Hand->Buffer		= (VOID*)Buffer; 
	Hand->Font.FontBuffer 	= (VOID*)FontBuffer;

	Hand->Font.Area.Width 	= Hand->Width;
	Hand->Font.Area.Height 	= Hand->Height;
	
	Hand->Font.SizeX		= Hand->Font.Area.Width/Hand->Font.FontSizeX;
	Hand->Font.SizeY		= Hand->Font.Area.Height/Hand->Font.FontSizeY;
	
	Hand->Font.SetPositionCursorX 		= Hand->Font.SetPositionCursorY = Hand->Font.RSizeX = 0;
	
	return (0);
}

UINTN UpdateBoxDraw(GW_HAND *_Hand)
{
	GW_HAND *Hand = _Hand;
	Hand->Font.Area.X 		= Hand->X;
	Hand->Font.Area.Y 		= Hand->Y;
	Hand->Font.Area.Width 	= Hand->Width;
	Hand->Font.Area.Height 	= Hand->Height;
	
	Hand->Font.SizeX		= Hand->Font.Area.Width/Hand->Font.FontSizeX;
	Hand->Font.SizeY		= Hand->Font.Area.Height/Hand->Font.FontSizeY;
	
	Hand->Font.CursorX = Hand->Font.SetPositionCursorX;
	Hand->Font.CursorY = Hand->Font.SetPositionCursorY;
	
	Hand->Font.RSizeX = 0;
	
	return (0);
}


UINTN BoxDraw(	GW_HAND *_Hand,
				UINTN C,
				UINTN FgColor,
				UINTN BgColor, 
				VOID *Buffer)
{
	
	GW_HAND *Hand = _Hand;
	if(Hand->Font.CursorY >= Hand->Font.SizeY){
		//scroll
		Hand->Font.CursorX = Hand->Font.CursorY = 0;
		Hand->Font.RSizeX = 0;
	}
	if( (Hand->Font.CursorX >= Hand->Font.SizeX) || (C == '\n')){
		Hand->Font.CursorY++;
		Hand->Font.CursorX = 0;
		Hand->Font.RSizeX = 0;
	}
	if(C == '\t'){
			Hand->Font.CursorX += 8;
	}
	else if(C >=' '){				
		
		DrawCharTransparent(Hand->Font.CursorX * (Hand->Font.FontSizeX) + Hand->Font.Area.X - Hand->Font.RSizeX,
						Hand->Font.CursorY * Hand->Font.FontSizeY + Hand->Font.Area.Y,
						C,
						FgColor,Buffer,Hand->Font.FontBuffer);/*

		DrawChar(Hand->Font.CursorX * (Hand->Font.FontSizeX) + Hand->Font.Area.X - Hand->Font.RSizeX,
						Hand->Font.CursorY * Hand->Font.FontSizeY + Hand->Font.Area.Y,
						C,
						FgColor,BgColor,Buffer,Hand->Font.FontBuffer);*/
							
		Hand->Font.CursorX++;
		
							
	}
	
	
	return (C);
}


UINTN BoxDrawSectCursor(GW_HAND *_Hand,UINTN X,UINTN Y)
{
	GW_HAND *Hand = _Hand;

	Hand->Font.SetPositionCursorX = X; 
	Hand->Font.SetPositionCursorY = Y;

	return 0;
}


UINTN 
BoxDrawString(GW_HAND *_Hand,CHAR8 *string)
{
	CHAR8 *Tmp = (CHAR8*) string;
	GW_HAND *Hand = _Hand;
	
	UpdateBoxDraw(Hand);
	
	while(*Tmp)
	BoxDraw(Hand,*Tmp++,Hand->Font.FgColor,Hand->Font.BgColor,Hand->Buffer);


	//salve cursor
	Hand->Font.SetPositionCursorX = Hand->Font.CursorX;
	Hand->Font.SetPositionCursorY = Hand->Font.CursorY;

	
	return (0);
	
}

UINTN 
UpdateBoxDrawString(GW_HAND *_Hand,VOID *Buffer)
{

	UINTN i;
	CHAR8 *Tmp = (CHAR8*) Buffer;
	GW_HAND *Hand = _Hand;
	

	BoxDrawSectCursor(Hand,0,0);
	UpdateBoxDraw(Hand);
	
	
	for(i = 0; i < (Hand->Font.SizeX*Hand->Font.SizeY); i++)
	BoxDraw(Hand,*Tmp++,Hand->Font.FgColor,Hand->Font.BgColor,Hand->Buffer);


	//salve cursor
	Hand->Font.SetPositionCursorX = Hand->Font.CursorX;
	Hand->Font.SetPositionCursorY = Hand->Font.CursorY;

	
	return (0);
	
}

UINTN 
__UpdateBoxDrawString(GW_HAND *_Hand,VOID *Buffer)
{

	UINTN i,c;
	GW_HAND *Hand = _Hand;

	char **buf = (char**) Buffer;
	char *line;	


	for(i = 0; i < Hand->Font.SizeY;i++ ) {
		BoxDrawSectCursor(Hand,0,i);
		UpdateBoxDraw(Hand);
	
		line = buf[i];

		if(line == NULL)continue;

		c = 0;

		while(line[c]) {
			
			if(line[c] == '\n') break;
			BoxDraw(Hand,line[c++],Hand->Font.FgColor,Hand->Font.BgColor,Hand->Buffer);
			if(c >= Hand->Font.SizeX)break;

		}

	}


	//salve cursor
	Hand->Font.SetPositionCursorX = Hand->Font.CursorX;
	Hand->Font.SetPositionCursorY = Hand->Font.CursorY;

	
	return (0);
	
}


UINTN 
____UpdateBoxDrawString(GW_HAND *_Hand,VOID *Buffer)
{

	UINTN i,c;
	GW_HAND *Hand = _Hand->box;

	char **buf = (char**) Buffer;
	char *line;
	


	for(i = 0; i < Hand->Font.SizeY;i++ ) {
		BoxDrawSectCursor(Hand,0,i);
		__UpdateBoxDraw(_Hand,Hand);
	
		line = buf[i];

		if(line == NULL)continue;

		c = 0;

		while(line[c]) {
			BoxDraw(Hand,line[c++],Hand->Font.FgColor,Hand->Font.BgColor,Hand->Buffer);

			if(c >= Hand->Font.SizeX)break;

		}

	}


	//salve cursor
	Hand->Font.SetPositionCursorX = Hand->Font.CursorX;
	Hand->Font.SetPositionCursorY = Hand->Font.CursorY;

	
	return (0);
	
}


// usar no console
GW_HAND *__CreateBox(GW_HAND *GwHand,		GW_HAND *_Hand,
						UINTN X,
						UINTN Y,
						UINTN Width,
						UINTN Height, 
						UINTN Style,
						UINTN Flag)
{
	
	GW_HAND *Hand  = _Hand; //(GW_HAND*) malloc(sizeof(GW_HAND));
	
	Hand->X 	= GwHand->Area.X + X; // coordenada X
	Hand->Y 	= GwHand->Area.Y + Y; // Coordenada Y
	Hand->Width	= Width; // largura
	Hand->Height	= Height; // altura
	Hand->Style	= Style;
	Hand->Flag	= Flag;
	Hand->Buffer	= (VOID*) GwHand->Buffer; // 

	// area
	Hand->Area.X 		= X; // area coordenada X
	Hand->Area.Y 		= Y;// area Coordenada Y
	Hand->Area.Width	= Width; // area largura
	Hand->Area.Height	= Height; // area altura

	
	
	//Font
	InitializeBoxDraw(Hand,8,16,ColorTable[Style &0xff],ColorTable[Style >> 8 &0xff],
	GwHand->Buffer,font/*font8x16*/);

	return (GW_HAND*) Hand;	
	

}


GW_HAND *CreateBox(GW_HAND *GwHand,		UINTN X,
						UINTN Y,
						UINTN Width,
						UINTN Height, 
						UINTN Style,
						UINTN Flag)
{
	
	GW_HAND *Hand  = (GW_HAND*) malloc(sizeof(GW_HAND));
	
	Hand->X 	= GwHand->X + X; // coordenada X
	Hand->Y 	= GwHand->Y + Y; // Coordenada Y
	Hand->Width	= Width; // largura
	Hand->Height	= Height; // altura
	Hand->Style	= Style;
	Hand->Flag	= Flag;
	Hand->Buffer	= (VOID*) GwHand->Buffer; // 

	// area
	Hand->Area.X 		= X; // area coordenada X
	Hand->Area.Y 		= Y;// area Coordenada Y
	Hand->Area.Width	= Width; // area largura
	Hand->Area.Height	= Height; // area altura

	
	
	//Font
	InitializeBoxDraw(Hand,8,16,ColorTable[Style &0xff],ColorTable[Style >> 8 &0xff]/*bg color*/,
	GwHand->Buffer,/*font8x16*/font);

	return (GW_HAND*) Hand;	

}

UINTN __UpdateBoxDraw(GW_HAND *GwHand,GW_HAND *_Hand)
{
	GW_HAND *Hand 			= _Hand;

	Hand->X 	= GwHand->X + Hand->Area.X; // coordenada X
	Hand->Y 	= GwHand->Y + Hand->Area.Y; // Coordenada Y

	Hand->Font.Area.X 		= Hand->X;
	Hand->Font.Area.Y 		= Hand->Y;
	Hand->Font.Area.Width 		= Hand->Width;
	Hand->Font.Area.Height 		= Hand->Height;
	
	Hand->Font.SizeX		= Hand->Font.Area.Width/Hand->Font.FontSizeX;
	Hand->Font.SizeY		= Hand->Font.Area.Height/Hand->Font.FontSizeY;
	
	Hand->Font.CursorX = Hand->Font.SetPositionCursorX;
	Hand->Font.CursorY = Hand->Font.SetPositionCursorY;
	
	Hand->Font.RSizeX = 0;
	
	return (0);
}

/*


	Task

*/


GW_HAND *CreateTask(	UINTN X,
			UINTN Y,
			UINTN Width,
			UINTN Height, 
			UINTN Style,
			UINTN Flag)
{

	// font buffer
	font = (CHAR8*) malloc(0x1000);
	copymem(font,font8x16,256*16);
	
	GW_HAND *GwHand  = (GW_HAND*) malloc(sizeof(GW_HAND));
			
	GwHand->X 		= X; // coordenada X
	GwHand->Y 		= Y; // Coordenada Y
	GwHand->Width	= Width; // largura
	GwHand->Height	= Height; // altura
	GwHand->Style	= Style;
	GwHand->Flag	= Flag;
	GwHand->Buffer	= (VOID*) G->TaskBuffer; // 

	// area
	GwHand->Area.X 		= X; // area coordenada X
	GwHand->Area.Y 		= Y; // area Coordenada Y
	GwHand->Area.Width	= Width; // area largura
	GwHand->Area.Height	= Height; // area altura

	// Registrar janela
	//
	//
	//
	

	// area
	DrawArea (GwHand->Area.X + 1,GwHand->Area.Y + 1,GwHand->Area.Width -1,GwHand->Area.Height -1, \
				ColorTable[GwHand->Style >> 8 &0xff],GwHand->Buffer);


	// Barra de titule
	LineDrawHorizontal (GwHand->X,GwHand->Width,GwHand->Y,ColorTable[GwHand->Style &0xff],GwHand->Buffer);


	return (GW_HAND*) GwHand;	

}

UINTN UpdateTask(GW_HAND *_GwHand)
{
	
	GW_HAND *GwHand  = _GwHand;
			

	// area
	GwHand->Area.X 		= GwHand->X; // area coordenada X
	GwHand->Area.Y 		= GwHand->Y; // area Coordenada Y
	GwHand->Area.Width	= GwHand->Width; // area largura
	GwHand->Area.Height	= GwHand->Height; // area altura
	// Registrar janela
	//
	//
	//
	

	// area
	DrawArea (GwHand->Area.X + 1,GwHand->Area.Y + 1,GwHand->Area.Width -1,GwHand->Area.Height -1, \
				ColorTable[GwHand->Style >> 8 &0xf],GwHand->Buffer);


	// Barra de titule
	LineDrawHorizontal (GwHand->X,GwHand->Width,GwHand->Y,ColorTable[GwHand->Style &0xff],GwHand->Buffer);




	return (0);	

}



/*

	Windows

*/

//#define W_BARRA_TITUL_SIZE 24 // vertical pixel


/*        	Estrutura de nossa janela
 *
 *   1 - Barra de tÃ­tulo (tÃ­tulo, status, icone)
 *   2 - Borda
 *   3 - Coordenada X
 *   4 - Coordenada Y
 *   5 - Largura
 *   6 - Altura
 *   7 - Area
 *   8 - 0
 *   9 - Menu
 *  10 - Estilo (Scroll (Horizontal e Vertical))
 *  11 - Flag
 *
 *
 */
 
GW_HAND *CreateWindow(	CONST CHAR8 *Title,
						VOID *Menu,
						UINTN X,
						UINTN Y,
						UINTN Width,
						UINTN Height, 
						UINTN Style,
						UINTN Flag)
{

	GW_HAND *GwHand  = (GW_HAND*) malloc(sizeof(GW_HAND));

	// font buffer
	font = (CHAR8*) malloc(0x1000);
	copymem(font,font8x16,256*16);

	// title
	CHAR8 *_title = (CHAR8*) malloc(256);
	setmem(_title,256,0);
	//strcpy(_title,Title);
	char *p_dest = (char*)_title;
	char *p_src  = (char*)Title;
    	while (*p_src != '\0') *p_dest++ = *p_src++;
	
	GwHand->Title 	= (CHAR8*) _title;		
	GwHand->Menu 	= (VOID*) Menu;
	GwHand->X 		= X; // coordenada X
	GwHand->Y 		= Y; // Coordenada Y
	GwHand->Width	= Width; // largura
	GwHand->Height	= Height; // altura
	GwHand->Style	= Style;
	GwHand->Flag	= Flag;
	GwHand->Buffer	= (VOID*) G->BankBuffer; // 

	// area
	GwHand->Area.X 		= X + W_BORDER_SIZE; // area coordenada X
	GwHand->Area.Y 		= Y + W_BARRA_TITUL_SIZE; // area Coordenada Y
	GwHand->Area.Width	= Width - (W_BORDER_SIZE*2); // area largura
	GwHand->Area.Height	= Height - W_BARRA_TITUL_SIZE - W_BORDER_SIZE; // area altura



	GwHand->box = CreateBox(GwHand,8,8,GwHand->Width,GwHand->Height, 
	GW_STYLE(FORE_GROUND(GwHand->Style &0xff) | BACK_GROUND(GwHand->Style >> 8 &0xff)),GW_FLAG_VISIBLE);
	


	// Registrar janela
	//
	GwHand->Flag |= 1;

	//
	//


	// barra de title
	DrawArea (GwHand->X,GwHand->Y,GwHand->Width,W_BARRA_TITUL_SIZE, \
				ColorTable[GwHand->Style >> 16 &0xff],GwHand->Buffer);


	// Roda-pe
	DrawArea (GwHand->X,GwHand->Y + GwHand->Height - W_BORDER_SIZE,GwHand->Width,W_BORDER_SIZE, \
				ColorTable[GwHand->Style >> 16 &0xff],GwHand->Buffer);

	// esquerda
	DrawArea (GwHand->X,GwHand->Y + W_BARRA_TITUL_SIZE,W_BORDER_SIZE,GwHand->Height - W_BARRA_TITUL_SIZE, \
				ColorTable[GwHand->Style >> 16 &0xff],GwHand->Buffer);


	// direita
	DrawArea (GwHand->X + GwHand->Width - W_BORDER_SIZE ,\
	GwHand->Y + W_BARRA_TITUL_SIZE,W_BORDER_SIZE,GwHand->Height - W_BARRA_TITUL_SIZE,\
	ColorTable[GwHand->Style >> 16 &0xff],GwHand->Buffer);


	// Area da borda
	DrawRect (GwHand->X,GwHand->Y,GwHand->Width,GwHand->Height,ColorTable[GW_GRAY],GwHand->Buffer);


	// area do cliente
	DrawArea (GwHand->Area.X,GwHand->Area.Y,GwHand->Area.Width,GwHand->Area.Height, \
				ColorTable[GwHand->Style >> 8 &0xff],GwHand->Buffer);

	
	// Title
	CHAR8 *Tmp = (CHAR8*) Title;
	BoxDrawSectCursor(GwHand->box,0,0);

	__UpdateBoxDraw(GwHand,GwHand->box);

	BoxDrawString(GwHand->box,Tmp);
	
	
	
	// Status

	BoxDrawSectCursor(GwHand->box,GwHand->box->Font.SizeX - 3,0);
	UpdateBoxDraw(GwHand->box);
	BoxDraw(GwHand->box,'x',ColorTable[GwHand->Style &0xff],ColorTable[GwHand->Style >> 8 &0xff],GwHand->box->Buffer);
	




	// Lista

	GwHand->next	= NULL;
	GwHand->tail	= NULL;
	GW_HAND *p 	= (GW_HAND *)(G->List);

	

	// adicionar no final da list
	while(p->next)
		p = p->next;

	p->next = (GW_HAND*) GwHand;	



	


	return (GW_HAND*) GwHand;	

}

UINTN UpdateWindow(GW_HAND *_GwHand)
{
	GW_HAND *GwHand = _GwHand;

	GwHand->Area.X 		= GwHand->X + W_BORDER_SIZE; // area coordenada X
	GwHand->Area.Y 		= GwHand->Y + W_BARRA_TITUL_SIZE; // area Coordenada Y
	GwHand->Area.Width	= GwHand->Width - (W_BORDER_SIZE*2); // area largura
	GwHand->Area.Height	= GwHand->Height - W_BARRA_TITUL_SIZE - W_BORDER_SIZE; // area altura
	

	if(!(GwHand->Flag &GW_FLAG_VISIBLE)) return 1;
	// Area de janela
	//DrawRect (GwHand->X,GwHand->Y,GwHand->Width,GwHand->Height,ColorTable[GW_WHITE],GwHand->Buffer);


	// barra de title
	DrawArea (GwHand->X,GwHand->Y,GwHand->Width,W_BARRA_TITUL_SIZE, \
				ColorTable[GwHand->Style >> 16 &0xff],GwHand->Buffer);


	// Roda-pe
	DrawArea (GwHand->X,GwHand->Y + GwHand->Height - W_BORDER_SIZE,GwHand->Width,W_BORDER_SIZE, \
				ColorTable[GwHand->Style >> 16 &0xff],GwHand->Buffer);

	// esquerda
	DrawArea (GwHand->X,GwHand->Y + W_BARRA_TITUL_SIZE,W_BORDER_SIZE,GwHand->Height - W_BARRA_TITUL_SIZE, \
				ColorTable[GwHand->Style >> 16 &0xff],GwHand->Buffer);


	// direita
	DrawArea (GwHand->X + GwHand->Width - W_BORDER_SIZE ,\
	GwHand->Y + W_BARRA_TITUL_SIZE,W_BORDER_SIZE,GwHand->Height - W_BARRA_TITUL_SIZE,\
	ColorTable[GwHand->Style >> 16 &0xff],GwHand->Buffer);


	// Area da borda
	DrawRect (GwHand->X,GwHand->Y,GwHand->Width,GwHand->Height,ColorTable[GW_GRAY],GwHand->Buffer);


	// area do cliente
	DrawArea (GwHand->Area.X,GwHand->Area.Y,GwHand->Area.Width,GwHand->Area.Height, \
				ColorTable[GwHand->Style >> 8 &0xff],GwHand->Buffer);



	
	// Title
	CHAR8 *Tmp = (CHAR8*) GwHand->Title;
	BoxDrawSectCursor(GwHand->box,0,0);

	__UpdateBoxDraw(GwHand,GwHand->box);
	
	BoxDrawString(GwHand->box,Tmp);
	
	
	// Status
	BoxDrawSectCursor(GwHand->box,GwHand->box->Font.SizeX - 3,0);
	UpdateBoxDraw(GwHand->box);
	BoxDraw(GwHand->box,'x',ColorTable[GwHand->Style &0xff],\
	ColorTable[GwHand->Style >> 8 &0xff],GwHand->box->Buffer);
	
	
	
	
	return 0;
}



GW_HAND *CreateObject(	GW_HAND *GwHand, CONST CHAR8 *Title,
						UINTN Type,
						UINTN X,
						UINTN Y,
						UINTN Width,
						UINTN Height, 
						UINTN Style,
						UINTN Flag)
{
	GW_HAND *Hand = (GW_HAND*) malloc(sizeof(GW_HAND));



	CHAR8 *_title = (CHAR8*) malloc(256);
	setmem(_title,256,0);
	//strcpy(_title,Title);
	char *p_dest = (char*)_title;
	char *p_src  = (char*)Title;
    	while (*p_src != '\0') *p_dest++ = *p_src++;

	Hand->Title 		= (CHAR8*) _title;
	Hand->Area.X 		= X;
	Hand->Area.Y		= Y;
	Hand->Area.Width	= Width;
	Hand->Area.Height	= Height;
	
	Hand->X 		= GwHand->Area.X + X; // coordenada X
	Hand->Y 		= GwHand->Area.Y + Y; // Coordenada Y
	Hand->Width		= Width; // largura
	Hand->Height		= Height; // altura
	Hand->Style		= Style;
	Hand->Flag		= Flag &0x7fffffff;// &GW_FLANG_READY;//Flag;

	Hand->Buffer		= (VOID*) GwHand->Buffer;
	
	Hand->Type 		= Type;
	Hand->Msg1		= 0;
	Hand->Msg2		= 0;
	
	
	switch(Hand->Type){

		case GW_HANDLE_BUTTON:
		Hand->box = CreateBox(Hand,4,4,Hand->Width - 2,Hand->Height -2, 
		GW_STYLE(FORE_GROUND(Hand->Style &0xff) | BACK_GROUND(Hand->Style >> 8 &0xff)),GW_FLAG_VISIBLE);

		// Area
		DrawRectC (Hand->X,Hand->Y,Hand->Width,Hand->Height,ColorTable[Hand->Style &0xff],Hand->Buffer);
		DrawArea (Hand->X + 1,Hand->Y + 1,Hand->Width -1,Hand->Height -1, \
		ColorTable[Hand->Style >> 8 &0xff],Hand->Buffer);
		
		//FIXME tem que calcular o centro
		Hand->box->X += 16;
		Hand->box->Y += 4;

			break;
		
		case GW_HANDLE_LABEL:
		Hand->box = CreateBox(Hand,4,4,Hand->Width - 2,Hand->Height -2, 
		GW_STYLE(FORE_GROUND(Hand->Style &0xff) | BACK_GROUND(Hand->Style >> 8 &0xff)),GW_FLAG_VISIBLE);
			
		// Area
		DrawRect (Hand->X,Hand->Y,Hand->Width,Hand->Height,ColorTable[Hand->Style >> 8 &0xff],Hand->Buffer);
		DrawArea (Hand->X + 1,Hand->Y + 1,Hand->Width -1,Hand->Height -1,\
		ColorTable[Hand->Style >> 8 &0xff],Hand->Buffer);


			break;
		
		case GW_HANDLE_LIST:
		Hand->box = CreateBox(Hand,4,4,Hand->Width - 2,Hand->Height -2, 
		GW_STYLE(FORE_GROUND(Hand->Style &0xff) | BACK_GROUND(Hand->Style >> 8 &0xff)),GW_FLAG_VISIBLE);
	
		// Area
		DrawRect (Hand->X,Hand->Y,Hand->Width,Hand->Height,ColorTable[Hand->Style >> 8 &0xff],Hand->Buffer);
		DrawArea (Hand->X + 1,Hand->Y + 1,Hand->Width -1,Hand->Height -1, \
		ColorTable[Hand->Style >> 8 &0xff],Hand->Buffer);
				
		//FIXME Nelson aqui devemos implementar outra estrategia
		/*Hand->Buffer = (VOID*) malloc(0x1000); 
		setmem(Hand->Buffer,0x1000,0);*/

			break;

		case GW_HANDLE_BOX:
		Hand->box = CreateBox(Hand,4,4,Hand->Width - 4,Hand->Height - 4, 
		GW_STYLE(FORE_GROUND(Hand->Style &0xff) | BACK_GROUND(Hand->Style >> 8 &0xff)),GW_FLAG_VISIBLE);


		__CreateBox(GwHand,Hand,X,Y,Width,Height,
		GW_STYLE(FORE_GROUND(Hand->Style &0xff) | BACK_GROUND(Hand->Style >> 8 &0xff)),GW_FLAG_VISIBLE);
		

			break;

		case GW_HANDLE_FILE:

		Hand->box = CreateBox(Hand,4 + 8,8 + 18,Hand->Width - (4 + 8),Hand->Height - (8 + 18), 
		GW_STYLE(FORE_GROUND(Hand->Style &0xff) | BACK_GROUND(Hand->Style >> 8 &0xff)),GW_FLAG_VISIBLE);


				break;
			
		default:
			
			break;
		
	}
	


	// arvore

	Hand->next	= NULL;
	Hand->tail	= NULL;

	GW_HAND *p 	= GwHand;



	// adicionar no final da list
	while(p->tail)
		p = p->tail;

	p->tail = Hand;	

	
	return (GW_HAND*) Hand;
	
}


UINTN UpdateObject(	GW_HAND *GwHand,GW_HAND *_Hand)
{
	

	UINTN i,y,x;
	
	GW_HAND *Hand = _Hand;
	CHAR8 *text;

	Hand->X 		= GwHand->Area.X + Hand->Area.X; // coordenada X
	Hand->Y 		= GwHand->Area.Y + Hand->Area.Y; // Coordenada Y

	
	if(!(Hand->Flag&GW_FLAG_VISIBLE)) return 1;
	
	switch(Hand->Type){
		
		case GW_HANDLE_LABEL:			
		// Area
		DrawRect (Hand->X,Hand->Y,Hand->Width,Hand->Height,ColorTable[Hand->Style >> 8 &0xff],Hand->Buffer);
		DrawArea (Hand->X + 1,Hand->Y + 1,Hand->Width -1,Hand->Height -1,\
		ColorTable[Hand->Style >> 8 &0xff],Hand->Buffer);
				
		// Data
		text = (CHAR8*) Hand->Title;

		BoxDrawSectCursor(Hand->box,0,0);
		__UpdateBoxDraw(Hand,Hand->box);
	
		while(*text) {
			BoxDraw(Hand->box,*text++,ColorTable[Hand->Style &0xff],\
			ColorTable[Hand->Style >> 8 &0xff],Hand->box->Buffer);
		}	
			break;

		case GW_HANDLE_BUTTON:
		// Area
		DrawRectC (Hand->X,Hand->Y,Hand->Width,Hand->Height,ColorTable[Hand->Style &0xff],Hand->Buffer);
		DrawArea (Hand->X + 1,Hand->Y + 1,Hand->Width -1,Hand->Height -1, \
		ColorTable[Hand->Style >> 8 &0xff],Hand->Buffer);
		
		// Data
		text = (CHAR8*) Hand->Title;

		

		BoxDrawSectCursor(Hand->box,2,0);
		__UpdateBoxDraw(Hand,Hand->box);
	
		while(*text) {
			BoxDraw(Hand->box,*text++,ColorTable[Hand->Style &0xff],\
			ColorTable[Hand->Style >> 8 &0xff],Hand->box->Buffer);
		}
			break;
			
		case GW_HANDLE_LIST:
		// Area
		DrawRect (Hand->X,Hand->Y,Hand->Width,Hand->Height,ColorTable[Hand->Style >> 8 &0xff],Hand->Buffer);
		DrawArea (Hand->X + 1,Hand->Y + 1,Hand->Width -1,Hand->Height -1, \
		ColorTable[Hand->Style >> 8 &0xff],Hand->Buffer);
				
		// Data
		if((Hand->Flag &GW_FLAG_READY)) {
			// New item
				
			// FIXME
			CHAR8 *Tmp = 0;// (CHAR8*) Hand->Buffer;
			BoxDrawSectCursor(Hand->box,0,0);
			__UpdateBoxDraw(Hand,Hand->box);
	
			while(*Tmp)
				BoxDraw(Hand->box,*Tmp++,ColorTable[GwHand->Style &0xff],\
				ColorTable[Hand->Style >> 8 &0xff],Hand->box->Buffer);
					
	
			// Select item
			if(GlobalObjectFocus == Hand) {
				if((Hand->Msg2 * 16) < (Hand->Area.Height - 16) )
					DrawRect (Hand->X + 8,Hand->Y + 8 + (Hand->Msg2 *16),\
					Hand->Width - 20 ,16,0x808080,Hand->Buffer);
			}
				
		}
			break;

		case GW_HANDLE_BOX:
		
		// Data
		if((Hand->Flag &GW_FLAG_READY)) {

			__UpdateBoxDrawString(Hand,(UINT8*)(Hand->Msg1));

			
		}

			break;

		case GW_HANDLE_FILE:

		LineDrawVertical (Hand->Y,Hand->Height,Hand->X,ColorTable[Hand->Style >> 8 &0xff],Hand->Buffer);

		LineDrawHorizontal (Hand->X,Hand->Width,Hand->Y + 18,ColorTable[Hand->Style >> 8 &0xff],Hand->Buffer);
		
		VFS *vfs = (VFS*) Hand->Msg1;

		y = 0;
		x = 0;
		for(i=0;i < vfs->header.blocks;i++) {
	
			text = (CHAR8*) (vfs->header.buffer + (i*64));

			BoxDrawSectCursor(Hand->box,x,y);
			__UpdateBoxDraw(Hand,Hand->box);
	
			while(*text) {
				BoxDraw(Hand->box,*text++,ColorTable[Hand->Style &0xff],\
				ColorTable[Hand->Style >> 8 &0xff],Hand->box->Buffer);
			}

			y++;


		}


		DrawRectC (Hand->X + 8,Hand->Y + (18 + 8) + (Hand->Msg2*16),
		Hand->Width - (16 + 8),16,ColorTable[1],Hand->Buffer);

				break;
			
		default:
			
			break;
		
	}
	return 0;
}



UINTN GwStrLength(CHAR8 *Source)
{
	CHAR8 *src = (CHAR8*)Source;
	UINTN i = 0;
	
	while(src[i] != 0)++i;
	
	return (UINTN)i;
}

UINTN Send(GW_HAND *_Hand,UINTN Msg1, UINTN Msg2)
{
	/*CHAR8 *src;
	UINTN data1;
	UINTN data2;*/
	
	GW_HAND *Hand = _Hand;

	if((Msg2&GW_SMG_FLAG_BIT)) {
		switch(Msg1){
					
			case GW_FLAG_INVISIBLE:
				Hand->Flag &= ~GW_FLAG_VISIBLE;
				break;
			case GW_FLAG_VISIBLE:
				Hand->Flag |= GW_FLAG_VISIBLE;
				break;
		}
		goto done;
	}
	
	switch(Hand->Type){
		
		case GW_HANDLE_LABEL:
		
				Hand->Msg1 = Msg1;
				Hand->Msg2 = Msg2 &GW_SMG_NORMAL_BIT;
		
				Hand->Flag |= GW_FLAG_READY;
			
			break;
		case GW_HANDLE_LIST:
		
			if(!Msg1) {
				Hand->Msg2 = Msg2 &GW_SMG_NORMAL_BIT;
				goto done;
			}
			
			/*Hand->Msg1 = Msg1;
			
			src = (CHAR8*) Msg1;
			
			data1 = GwStrLength(Hand->Buffer);
			
			data2 = GwStrLength(src);
		
			// FIXME
			copymem(Hand->Buffer + data1,src,data2);
			
			*(CHAR8*)(Hand->Buffer + data1 + data2) = '\n';
			
			Hand->Flag  |= GW_FLAG_READY;*/
			
			break;

		case GW_HANDLE_BOX:
		
				Hand->Msg1 = Msg1;
				Hand->Msg2 = Msg2 &GW_SMG_NORMAL_BIT;

				Hand->Flag |= GW_FLAG_READY;
			
			break;

		case GW_HANDLE_FILE:

		if(!Msg1) {
			Hand->Msg2 = Msg2 &GW_SMG_NORMAL_BIT;
			goto done;
		}

		
		Hand->Msg1 = Msg1;
		Hand->Msg2 = Msg2 &GW_SMG_NORMAL_BIT;

		Hand->Flag |= GW_FLAG_READY;
			
			break;

		default:
		
			break;
	}
done:
	
	return 0;
}

GW_HAND *Receive()
{
		//if(!(GlobalObjectFocus->Flag)) return 0;
		
		switch(GlobalObjectFocus->Type){
		
			case GW_HANDLE_LABEL:
				break;
				
			case GW_HANDLE_LIST:
					GlobalObjectFocus->RMsg1 = 1;
					GlobalObjectFocus->RMsg2 = GlobalObjectFocus->Msg2;
				break;
		default:
		
			break;
	}
			
		
		return (GW_HAND *)GlobalObjectFocus;

}


UINTN GwSetFocus(GW_HAND *Hand)
{
	
	GlobalGwFocus = Hand;
	
	return 0;
	
}

UINTN ObjectSetFocus(GW_HAND *Hand)
{
	
	GlobalObjectFocus = Hand;
	
	return 0;
	
}

// FOR CONSOLE INITIAL

UINTN 
BoxSetAttribute(IN UINTN Foreground,IN UINTN Background)
{
	Box->Font.FgColor 		= ColorTable[Foreground &0xf];
	Box->Font.BgColor 		= ColorTable[Background &0xf];

	return 0;
}


UINTN 
outputstring(IN CHAR8 *string)
{
	CHAR8 *Tmp = (CHAR8*) string;
	
	UpdateBoxDraw(Box);
	
	while(*Tmp)
	BoxDraw(Box,*Tmp++,Box->Font.FgColor,Box->Font.BgColor,Box->Buffer);


	//salve cursor
	Box->Font.SetPositionCursorX = Box->Font.CursorX;
	Box->Font.SetPositionCursorY = Box->Font.CursorY;

	RefreshRate();
	
	return (0);
	
}


// Window Focus
UINTN WindowFocus(GW_HAND *window)
{

	UINT32 *p = (UINT32*)0x1000111C;
	UINT32 *GwFocus = (UINT32 *)(*p);

	if(__focus->pid == __pid) {

		*GwFocus = (UINT32)window;
	}

	return 0;

}

VOID DrawMouse(	UINTN X,
		UINTN Y,	
		UINTN FgColor, 
		VOID *Buffer,
		VOID *MouseBuffer)
{
	UINT16 font = 0;
	CONST UINT16 *font_data = (CONST UINT16*) MouseBuffer;
   	INTN cx, cy;
	UINT16 mask;
    
   for(cy=0;cy < 18;cy++){
		mask = 1;
        font = font_data[cy];
		
                for(cx = 18 -1 ;cx >= 0;cx--){ 
                       if(font &mask)
			PutPixelBuff(X + cx,Y + cy,FgColor,Buffer);
			mask += mask;
                  
          }
        
    }

}

GW_HAND *CreateMouse(UINTN X,UINTN Y,UINTN Style,UINTN Flag)
{

	GW_HAND *GwHand  = (GW_HAND*) malloc(sizeof(GW_HAND));

	GwHand->X 		= X; // coordenada X
	GwHand->Y 		= Y; // Coordenada Y

	GwHand->Buffer	= (VOID*) G->BankBuffer; //

 
	return (GW_HAND*) GwHand;
}


UINTN UpdateMouse(GW_HAND *Hand)
{

	DrawMouse(Hand->X,Hand->Y/* Y*/,ColorTable[GW_LIGHTGREEN],Hand->Buffer,cursor18x18);

	return 0;

}

// Window Focus
int ExitWindowFocus()
{
	UINT32 *p = (UINT32*)0x1000111C;
	UINT32 *GwFocus = (UINT32 *)(*p);

	*GwFocus = (UINT32) 0;

	return 0;

}

UINTN gui_exit(GW_HAND *Hand)
{

	GW_HAND *next 		= Hand->next;
	GW_HAND *obj = NULL;

	if(Hand) obj 	= Hand->tail;

	GW_HAND *list = (GW_HAND*)(G->List);
	GW_HAND *current 	= Hand;


	// desabilitar janela
	// exit foco
	ExitWindowFocus();

	while(obj) {

		free(obj->Title);
		free(obj->box);
		free(obj);

	 	obj = obj->tail;
		
	}

	// Percorre a lista ate achar o p->next igual ao current
	do{

		if(list->next == current) break;

		list = list->next;

	}while(list);

	if(!list) return -1;

	// aponta o p->next para o next
	list->next = next;

	// remove a janela da lista
	free(current->Title);
	free(current->box);
	free(current);

	return 0;
}
