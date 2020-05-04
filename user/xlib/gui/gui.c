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
#include <string.h>
#include <io.h>

GW_HAND	*GlobalGwFocus = NULL;
GW_HAND	*GlobalObjectFocus = NULL;
GW_HAND	*Box = NULL;

CHAR8 *font = NULL;


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
	memcpy(G->FrameBuffer,G->BankBuffer,\
	G->HorizontalResolution * (G->VerticalResolution - W_TASK_BARRA_SIZE) * (G->Bpp/8));
	
}


VOID app_clearscreen() 
{
	__setmem(G->BankBuffer,0x00000000,(G->HorizontalResolution * G->VerticalResolution\
	* (G->Bpp/8)));
}

void app_color_clearscreen(long rgb) 
{
	__setmem(G->BankBuffer,rgb,(G->HorizontalResolution * G->VerticalResolution\
	* (G->Bpp/8)));
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

GW_HAND *CreateMouse(UINTN X,UINTN Y,UINTN Style,UINTN Flag)
{

	GW_HAND *GwHand  = (GW_HAND*) malloc(sizeof(GW_HAND));

	GwHand->X 		= X; // coordenada X
	GwHand->Y 		= Y; // Coordenada Y

	GwHand->Buffer	= (VOID*) G->BankBuffer; //

 
	return (GW_HAND*) GwHand;
}
