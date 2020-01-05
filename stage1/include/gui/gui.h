/**
 *	Copyright (c) 2013 Sirius Corporation
 *	Nelson Sapalo da Siva Cole
 *	Lubango, 13 de Setembro de 2019 - Hora: 18h033
 *	File Name: gui.h
 **/
 

#ifndef __GUI_H__
#define __GUI_H__
 
#define W_TASK_BARRA_SIZE 32

// Type
#define W_BARRA_TITUL_SIZE 24
#define GW_HANDLE_EDIT 		1
#define GW_HANDLE_BUTTON	2
#define GW_HANDLE_LABEL 	3
#define GW_HANDLE_LIST 		4
#define GW_HANDLE_BOX 		5

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
#define BACK_GROUND_TITLE(x)( x << 16 &0xff0000)

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


#include "bmp.h"


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

	VOID	*List;
	
	
	

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
VOID refreshrate();
VOID clearscreen(); //Tela


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
OutputString(IN CHAR8 *string);
UINTN 
BoxClearScreen();




#endif


















