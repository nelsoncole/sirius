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

#include <io.h>




GUI *G = NULL;

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
VOID refreshrate() 
{
	/*__copymem(G->FrameBuffer,G->BankBuffer,\
	G->HorizontalResolution * (G->VerticalResolution) *(G->Bpp/8));*/

	 __copymem__(G->FrameBuffer,G->BankBuffer,\
	G->HorizontalResolution * (G->VerticalResolution) *(G->Bpp/8));
	
}


VOID clearscreen() 
{
	//__setmem(G->BankBuffer,0,G->HorizontalResolution * (G->VerticalResolution) * (G->Bpp/8));
	setmem(G->BankBuffer,G->HorizontalResolution * (G->VerticalResolution) * (G->Bpp/8),0);
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


GW_HAND *CreateBox(GW_HAND *GwHand,		UINTN X,
						UINTN Y,
						UINTN Width,
						UINTN Height, 
						UINTN Style,
						UINTN Flag)
{
	
	GW_HAND *Hand  = (GW_HAND*) alloc(1/*sizeof(GW_HAND)*/); // 4KiB
	
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



// FOR CONSOLE INITIAL

UINTN 
BoxSetAttribute(IN UINTN Foreground,IN UINTN Background)
{
	Box->Font.FgColor 		= ColorTable[Foreground &0xf];
	Box->Font.BgColor 		= ColorTable[Background &0xf];

	return 0;
}


UINTN 
OutputString(IN CHAR8 *string)
{
	CHAR8 *Tmp = (CHAR8*) string;
	
	UpdateBoxDraw(Box);
	
	while(*Tmp)
	BoxDraw(Box,*Tmp++,Box->Font.FgColor,Box->Font.BgColor,Box->Buffer);


	//salve cursor
	Box->Font.SetPositionCursorX = Box->Font.CursorX;
	Box->Font.SetPositionCursorY = Box->Font.CursorY;

	refreshrate();
	
	return (0);
	
}

UINTN 
BoxClearScreen()
{

	//__setmem(Box->Buffer,Box->Font.BgColor,Box->Width * Box->Height * (G->Bpp/8));

	setmem(Box->Buffer,Box->Width * Box->Height * (G->Bpp/8),Box->Font.BgColor);

	Box->Font.SetPositionCursorX = Box->Font.SetPositionCursorY = 0;

	refreshrate();

	return (0);
	
}







