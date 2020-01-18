/*
 * File Name: console.c
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
#include <stdarg.h>


#if defined(__x86_64__) || defined(__aarch64__)
	#define	INT_MAX 0x7fffffffffffffff // -------	// max value of an "int"
	#define HEX_LEN 16
	
#elif defined(__i386__) || defined(__arm__)
	#define	INT_MAX	0x7fffffff // 2147483647	// max value of an "int"
	#define HEX_LEN 8

#else
#error YOUR ARCH HERE
#endif


UINTN X = 0,Y = 0;


UINT16 *input 	= (UINT16*) 0x10001100;
CHAR8 *vram 	= NULL;
GW_HAND *whd	= NULL;
GW_HAND *boxhd 	= NULL;
UINTN cursor_x 	=0;
UINTN cursor_y 	=0;
UINTN cursor_x_size,cursor_y_size;
UINTN scroll	= 0;
UINTN end_char 	= 0;

UINTN cls();
UINTN set_cursor(UINTN x, UINTN y);
UINTN set_cursor_x(UINTN x);
UINTN set_cursor_y(UINTN y);
UINTN putchar(UINTN ch);
VOID  puts(CONST CHAR8 *string);
VOID  printf(CONST CHAR8 *format,...);

UINTN getchar();
VOID gets(CHAR8 *string);

extern UINTN shell();




INTN main()
{


	vram = (CHAR8*)__malloc(0x3000); // 12 KiB

	whd = CreateWindow("Console",0,100,100,600,450,
	GW_STYLE(FORE_GROUND(GW_WHITE) | BACK_GROUND(GW_BLACK) | BACK_GROUND_STYLE(GW_BLUE)),GW_FLAG_VISIBLE);

	
	boxhd = CreateObject(whd,TEXT("GW_HANDLE_BOX"),GW_HANDLE_BOX,2,2,whd->Area.Width -2,whd->Area.Height - 2, 
	GW_STYLE(FORE_GROUND(GW_WHITE) | BACK_GROUND(GW_BLACK)),GW_FLAG_INVISIBLE);


	// salve tamanho do cusor
	cursor_x_size = boxhd->Font.SizeX;
	cursor_y_size = boxhd->Font.SizeY;

	cursor_x 	= 0;
	cursor_y 	= 0;
	scroll		= 0;
	end_char 	= 0;
	
	// limpar vram
	// FIXME 4 KiB e para garantir a seguraca do processo
	setmem(vram,0x3000,' ');

	// size char
	end_char = (UINTN)(vram + 0x2000);


	// enviar sms para box
	Send(boxhd,GW_FLAG_VISIBLE,GW_SMG_FLAG_BIT);

	
	while(TRUE) {
		
		printf(":>");

		WindowFocus(whd);

		Send(boxhd,(UINTN)(vram + scroll*cursor_x_size),GW_SMG_NORMAL_BIT);

		shell();
	}


	return 0;
}

UINTN cls()
{
	setmem(vram,0x2000,' ');
	cursor_x =0;
	cursor_y =0;

	scroll = 0;
	return 0;
}

UINTN set_cursor(UINTN x, UINTN y)
{	
	cursor_x = x;
	cursor_y = y;
   
	return 0;
	
}

UINTN set_cursor_x(UINTN x)
{
	cursor_x = x;
    	return 0;
	
}

UINTN set_cursor_y(UINTN y)
{
	
	cursor_y = y;
    	return 0;
}

UINTN putchar(UINTN ch)
{

	if(!ch)return -1;
	
	CHAR8 *p_vram = (vram + ((cursor_y * cursor_x_size) + cursor_x));

	if(((UINTN)p_vram) >= end_char){

		cls();

	}


	
	if(ch == '\b' && cursor_x){
		cursor_x--;
		*(--p_vram) = ' ';
		//cursor_x--;
	 }
	 
    	else if(ch == '\t'){
		(cursor_x += 8);//&~(8-1);
		}
		
    	else if(ch == '\n'){
		cursor_x = 0;
		cursor_y++;
		//update_cursor();
	
		}
	else if(ch >= ' '){
	
		//*posicao = ch | atributo;
		*p_vram = ch;
		cursor_x++;
	}

	// fom da linha
	if(cursor_x >= cursor_x_size ){
		cursor_x =0;
		cursor_y++;

		}
	
	// scroll
	if(cursor_y >= (cursor_y_size + scroll)){

		scroll++;
		
	}

	
	//update_cursor();

	return ch;

	
 }


VOID puts(CONST CHAR8 *string) 
{

	UINTN i;
	if(!string)return;
	 
	for(i=0;i <strlen(string);i++) {
		putchar(string[i]);

	}

}

VOID atoi(INTN value, CHAR8* valuestring) {
  	
	INTN min_flag;
  	CHAR8 swap, *p;
 	min_flag = 0;

  	if (0 > value)
  	{
    		*valuestring++ = '-';
    		value = -INT_MAX> value ? min_flag = INT_MAX : -value;
  	}

  	p = valuestring;

  	do
  	{
    		*p++ = (CHAR8)(value % 10) + '0';
    		value /= 10;
  	} while (value);

  	if (min_flag != 0)
  	{
    		++*valuestring;
  	}
  	*p-- = '\0';

  	while (p > valuestring)
  	{
    		swap = *valuestring;
    		*valuestring++ = *p;
    		*p-- = swap;
  	}
}

VOID i2hex(UINTN val, CHAR8* dest, INTN len)
{
	CHAR8* cp;
	char x;
	UINTN n;
	n = val;
	cp = &dest[len];
	while (cp > dest)
	{
		x = n & 0xF;
		n >>= 4;
		*--cp = x + ((x > (HEX_LEN+1)) ? 'A' - 10 : '0');
	}
    dest[len]='\0';
}


VOID printf(CONST CHAR8 *format,...)
{
	CHAR8* ap;
	va_start (ap,format);
	UINTN index = 0;	
	INTN  d;
	UINTN c;
	CHAR8 *s;
	CHAR8 buffer[256];

	while (format[index])
	{
		switch (format[index])
		{
		case '%':
			++index;
			switch (format[index])
			{
			
    			case 'C':
			case 'c':
				c = (CHAR8) va_arg (ap, INTN);
				putchar(c);
				break;
     
    			case 'S':
			case 's':
				s = va_arg (ap, CHAR8*);
				puts(s);
				break;

			case 'd':
			case 'i':
				d = va_arg (ap, INTN);
				atoi(d, buffer);
				puts(buffer);
				break;

			case 'X':
			case 'x':
				d = va_arg (ap, UINTN);
				i2hex(d, buffer,8);
				puts(buffer);
				break;
			
			default:
				putchar('%');
				putchar('%');
				break;
				
				
			}
			break;

		default:
			putchar(format[index]); //
			break;
		}
		++index;
	}
}

UINTN __putchar(UINTN ch) {


	putchar(ch);
	Send(boxhd,(UINTN)(vram + scroll*cursor_x_size),GW_SMG_NORMAL_BIT);

	return 0;
}

// input
UINTN getchar()
{
	UINTN rc;

	while(!(*input&0xffff)) WindowFocus(whd); //FIXME

	rc = *input;
	*input = 0;

	if(rc == '\n')return rc;

	__putchar(rc);

	return rc;
	

}

VOID gets(CHAR8 *string)
{
	
	CHAR8 *p = string;
	UINTN rc,i;
	

	while(TRUE) {

		rc = getchar();
		

		if(rc == '\t')for(i = 0; i < 8;i++)*p++ = ' ';
		else if(rc == '\b')*--p = '\0';
		else if(rc == '\n'){*p = '\0';break;}
		else *p++ = rc;

	
	}
	




}



















