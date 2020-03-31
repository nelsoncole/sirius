/*
 * File Name: gui.h
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
 

#ifndef __GUI_H__
#define __GUI_H__
 
#define W_TASK_BARRA_SIZE 32

// Type
#define W_BARRA_TITUL_SIZE 	32
#define W_BORDER_SIZE 		6
#define GW_HANDLE_EDIT 		1
#define GW_HANDLE_BUTTON	2
#define GW_HANDLE_LABEL 	3
#define GW_HANDLE_LIST 		4
#define GW_HANDLE_BOX 		5
#define GW_HANDLE_FILE 		6

// Flags
#define GW_FLAG_INVISIBLE 	0
#define GW_FLAG_VISIBLE 	1
#define GW_FLAG_READY 		2
#define GW_FLAG_FOCO 		0x80000000

#define GW_SMG_FLAG_BIT		0x80000000
#define GW_SMG_NORMAL_BIT	0x7FFFFFFF

#define GW_SMG_BUTTON		0
#define GW_SMG_EDIT		1
#define GW_SMG_LIST		2


// Text string
#define TEXT(x)(x)

// Style bit 0-8 color 
#define GW_STYLE(x) x
#define FORE_GROUND(x)(x &0xff)
#define BACK_GROUND(x)( x << 8 &0xff00)
#define BACK_GROUND_STYLE(x)( x << 16 &0xff0000)

#define FONT_SIZE_X 8
#define FONT_SIZE_Y 16

// Color attributes
#define GW_BLACK 0x00
#define GW_BLUE 0x01
#define GW_GREEN 0x02
#define GW_CYAN 0x03
#define GW_RED 0x04
#define GW_MAGENTA 0x05
#define GW_BROWN 0x06
#define GW_GRAY 0x07
#define GW_DARKGRAY  0x08
#define GW_LIGHTBLUE 0x09
#define GW_LIGHTGREEN 0x0A
#define GW_LIGHTCYAN 0x0B
#define GW_LIGHTRED 0x0C
#define GW_LIGHTMAGENTA 0x0D
#define GW_YELLOW 0x0E
#define GW_WHITE 0x0F





typedef struct _GUI_AREA {
	
	UINTN HorizontalStart;
	UINTN VerticalStart;
	UINTN HorizontalEnd;
	UINTN VerticalEnd;

}__attribute__((packed)) GUI_AREA;

typedef struct _GUI
{
	// Default
	UINTN	Pitch;
	UINTN  	Bpp;
	UINTN  	Planes;
    	UINTN	HorizontalResolution;  		//	width;
	UINTN	VerticalResolution;  		//	hight;
 	UINTN	PixelsPerScanLine;	
    	UINTN	MaxPixelClock;
	UINTN 	FrameBufferSize;
	VOID	*FrameBuffer;
	VOID	*BankBuffer;
	VOID	*TaskBuffer;
	VOID	*WindowScreen;
	VOID	*WindowBackground; 
	
	// Extra
	GUI_AREA Area;

	UINT32	List;
	
	
	

}__attribute__ ((packed)) GUI;

///
typedef struct _GW_AREA {
	UINTN X;
	UINTN Y;
	UINTN Width;
	UINTN Height; 
}GW_AREA;

typedef struct _GW_FONT {

	GW_AREA Area;
	UINTN SizeX;
	UINTN SizeY;
	UINTN FontSizeX;
	UINTN FontSizeY;
	UINTN FgColor;
	UINTN BgColor;
	UINTN CursorX;
	UINTN CursorY;
	
	UINTN SetPositionCursorX;
	UINTN SetPositionCursorY;
	
	UINTN RSizeX;
	
	VOID *FontBuffer;
	

}__attribute__((packed)) GW_FONT;  

typedef struct _GW_HAND {

	CHAR8 *Title;
	VOID *Menu;
	UINTN X;
	UINTN Y;
	UINTN Width;
	UINTN Height; 
	UINTN Style;
	UINTN Flag; // Nelson o bit 1 Ã© para informar, se a janela jÃ¡ foi registrada
	VOID *Buffer;
	
	GW_AREA Area;
	GW_FONT Font;
	
	UINTN Type;
	UINTN Msg1;
	UINTN Msg2;
	UINTN RMsg1;
	UINTN RMsg2;

	struct _GW_HAND *box;

	struct _GW_HAND *next;

	struct _GW_HAND *tail;

	
}__attribute__ ((packed)) GW_HAND;




VOID PutPixel(UINTN X, UINTN Y, UINTN Color);
VOID PutPixelBuff(UINTN X, UINTN Y, UINTN Color,VOID *Buffer);
VOID RefreshRate();
VOID APP_RefreshRate();
VOID TASK_RefreshRate();
VOID TASK_Clearscreen();


VOID app_clearscreen();
VOID app_refreshrate();

UINTN LineDrawHorizontal ( 	UINTN X1,
							UINTN X2,
                       		UINTN Y, 
							UINTN Color,
							VOID *Buffer);


UINTN LineDrawVertical ( 	UINTN Y1,
							UINTN Y2,
                       		UINTN X, 
							UINTN Color,
							VOID *Buffer);


UINTN DrawRectC (UINTN X1,
				UINTN Y1,
				UINTN X2,
                UINTN Y2, 
				UINTN Color,
				VOID *Buffer);

UINTN DrawRect (UINTN X1,
				UINTN Y1,
				UINTN X2,
                UINTN Y2, 
				UINTN Color,
				VOID *Buffer);
				
VOID DrawChar( 	UINTN X,
				UINTN Y,	
				UINTN C,
				UINTN FgColor,
				UINTN BgColor, 
				VOID *Buffer,
				VOID *FontBuffer );
VOID DrawCharTransparent(	UINTN X,
							UINTN Y,	
							UINTN C,
							UINTN FgColor, 
							VOID *Buffer,
							VOID *FontBuffer);
				
UINTN DrawArea (UINTN X1,
				UINTN Y1,
				UINTN X2,
                UINTN Y2, 
				UINTN Color,
				VOID *Buffer);
				

/*

	Box

*/

UINTN UpdateBoxDraw(GW_HAND *_Hand);
UINTN InitializeBoxDraw(GW_HAND *_Hand,
						UINTN X,	
						UINTN Y,
						UINTN FgColor,
						UINTN BgColor,
						VOID *Buffer,
						VOID *FontBuffer);
UINTN BoxDraw(	GW_HAND *_Hand,
				UINTN C,
				UINTN FgColor,
				UINTN BgColor, 
				VOID *Buffer);
UINTN 
BoxDrawString(GW_HAND *_Hand,CHAR8 *string);

UINTN 
UpdateBoxDrawString(GW_HAND *_Hand,VOID *Buffer);

UINTN 
__UpdateBoxDrawString(GW_HAND *_Hand,VOID *Buffer);

UINTN 
____UpdateBoxDrawString(GW_HAND *_Hand,VOID *Buffer);

// Usar no console
GW_HAND *__CreateBox(GW_HAND *GwHand,		GW_HAND *_Hand,
						UINTN X,
						UINTN Y,
						UINTN Width,
						UINTN Height, 
						UINTN Style,
						UINTN Flag);

GW_HAND *CreateBox(GW_HAND *GwHand,		UINTN X,
						UINTN Y,
						UINTN Width,
						UINTN Height, 
						UINTN Style,
						UINTN Flag);
UINTN __UpdateBoxDraw(GW_HAND *GwHand,GW_HAND *_Hand);

UINTN 
BoxSetAttribute(IN UINTN Foreground,IN UINTN Background);
UINTN 
outputstring(IN CHAR8 *string);

UINTN 
cls();
/*

	Task
*/				

GW_HAND *CreateTask(	UINTN X,
			UINTN Y,
			UINTN Width,
			UINTN Height, 
			UINTN Style,
			UINTN Flag);
UINTN UpdateTask(GW_HAND *_GwHand);

/*
	Window

	
*/
GW_HAND *CreateWindow(	CONST CHAR8 *Title,
						VOID *Menu,
						UINTN X,
						UINTN Y,
						UINTN Width,
						UINTN Height, 
						UINTN Style,
						UINTN Flag);

GW_HAND *CreateObject(	GW_HAND *_GwHand, CONST CHAR8 *Title,
						UINTN Type,
						UINTN X,
						UINTN Y,
						UINTN Width,
						UINTN Height, 
						UINTN Style,
						UINTN Flag);

UINTN UpdateWindow(GW_HAND *_GwHand);
UINTN UpdateObject(	GW_HAND *GwHand,GW_HAND *_Hand);
						
UINTN Send(GW_HAND *_Hand,UINTN Msg1, UINTN Msg2);
GW_HAND *Receive();
UINTN GwSetFocus(GW_HAND *Hand);
UINTN ObjectSetFocus(GW_HAND *Hand);



VOID DrawMouse(	UINTN X,
		UINTN Y,	
		UINTN FgColor, 
		VOID *Buffer,
		VOID *MouseBuffer);

UINTN WindowFocus(GW_HAND *window);
UINTN gui_exit(GW_HAND *Hand);





GW_HAND *create_window_mouse(long X,long Y, long Width, long Height,long Style,long Flag);
int update_window_mouse(GW_HAND *_GwHand);


#endif





















