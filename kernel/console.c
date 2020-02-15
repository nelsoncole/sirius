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


#include <os.h>
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
 


CHAR8 *__buffer__ = NULL;
CHAR8 *__string__ = NULL;

UINTN 
ClearScreen() {

	BoxClearScreen();
	 return  0;
 }
 

UINTN 
SetAttribute(IN UINTN Foreground,IN UINTN Background) {
	return BoxSetAttribute(Foreground,Background);

}

UINTN  
EnableCursor(IN BOOLEAN Visible) {
	return 0;
}

UINTN  
SetCursorPosition(IN UINTN Column,IN UINTN Row) {
	return 0;
}


static VOID atoi(INTN value, CHAR8* valuestring) {
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

static VOID i2hex(UINTN val, CHAR8* dest, INTN len)
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


UINTN  
print(CONST CHAR8 *format,...) 
{

	UINTN rc = 0;
	

	VOID* ap;
	va_start (ap,format);
	INTN index = 0;	
	INTN d;
	INTN c;
	CHAR8 *s;
	CHAR8 *p_strig  =(CHAR8*) __string__;

	setmem(p_strig,4096-512,0);

	while ((*(CHAR8*)(format + index))/*format[index]*/)
	{
		switch ((*(CHAR8*)(format + index))/*format[index]*/)
		{
		case '%':
			++index;
			switch ((*(CHAR8*)(format + index))/*format[index]*/)
			{
			
				case 'C':
				case 'c':
				c = (CHAR8) va_arg (ap, INTN);
				*p_strig++ = c;
				break;
     
				case 'S':
				case 's':
				s = va_arg (ap, CHAR8*);
				while(*s)*p_strig++ = *s++;
				break;

				case 'd':
				case 'i':
				d = va_arg (ap, UINTN);
				atoi(d, __buffer__);
				s = (CHAR8*)__buffer__;
				while(*s)*p_strig++ = *s++;
				break;

				case 'X':
				case 'x':
				d = va_arg (ap, INTN);
				i2hex(d, __buffer__,HEX_LEN);
				s = (CHAR8*)__buffer__;
				while(*s)*p_strig++ = *s++;
				break;
			
				default:
				*p_strig++ = '%';
				*p_strig++ = '%';
				break;
				
			}
			break;

		default:
		c = (*(CHAR8*)(format + index));/*format[index]*/
		
		*p_strig++ = c;
		
		break;
		}
		++index;
    }
	
	
	OutputString((CHAR8*)__string__);
	return rc;
}






// FIXME 
char *dp_str = (char*)0x800000;

void dp_init(){

	setmem(dp_str,0x10000,0);

}

void dputs(const char *str)
{
	const char *p_str = str;


	while(*p_str) *dp_str++ = *p_str++;
	
	
}

UINTN  
dprintf(CONST CHAR8 *format,...) 
{

	UINTN rc = 0;
	

	VOID* ap;
	va_start (ap,format);
	INTN index = 0;	
	INTN d;
	INTN c;
	CHAR8 *s;
	char str[256];
	CHAR8 *p_strig  =(CHAR8*) str;

	setmem(p_strig,256,0);

	while ((*(CHAR8*)(format + index))/*format[index]*/)
	{
		switch ((*(CHAR8*)(format + index))/*format[index]*/)
		{
		case '%':
			++index;
			switch ((*(CHAR8*)(format + index))/*format[index]*/)
			{
			
				case 'C':
				case 'c':
				c = (CHAR8) va_arg (ap, INTN);
				*p_strig++ = c;
				break;
     
				case 'S':
				case 's':
				s = va_arg (ap, CHAR8*);
				while(*s)*p_strig++ = *s++;
				break;

				case 'd':
				case 'i':
				d = va_arg (ap, UINTN);
				atoi(d, __buffer__);
				s = (CHAR8*)__buffer__;
				while(*s)*p_strig++ = *s++;
				break;

				case 'X':
				case 'x':
				d = va_arg (ap, INTN);
				i2hex(d, __buffer__,HEX_LEN);
				s = (CHAR8*)__buffer__;
				while(*s)*p_strig++ = *s++;
				break;
			
				default:
				*p_strig++ = '%';
				*p_strig++ = '%';
				break;
				
			}
			break;

		default:
		c = (*(CHAR8*)(format + index));/*format[index]*/
		
		*p_strig++ = c;
		
		break;
		}
		++index;
    }
	
	
	dputs(str);
	return rc;
}














