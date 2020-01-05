/*
 * File Name: irq.c
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
extern VOID interrupter(INTN n,UINT32 offset,UINT16 sel,UINT8 dpl );

extern VOID irq0();	extern VOID irq10();	extern VOID irq20();
extern VOID irq1();	extern VOID irq11();	extern VOID irq21();
extern VOID irq2();	extern VOID irq12();	extern VOID irq22();
extern VOID irq3();	extern VOID irq13();	extern VOID irq23();
extern VOID irq4();	extern VOID irq14();	
extern VOID irq5();	extern VOID irq15();
extern VOID irq6();	extern VOID irq16();
extern VOID irq7();	extern VOID irq17();
extern VOID irq8();	extern VOID irq18();
extern VOID irq9();	extern VOID irq19();

UINTN scancode;

VOID irq_function(INTN  n){

	switch(n - 0x40){
		
		case 0: 
			
			break;
		case 1:
 			keyboard_handler();
			break;
		case 2:
			
			break;
		case 3:

			break;
		case 4:

			break;
		case 5:
 
			break;
		case 6:

			break;
		case 7:

			break;
		case 8:
			rtc_handler();
			break;
		case 9:

			break;
		case 10: 
			
			break;
		case 11:
 
			break;
		case 12:
 			// MOUSE
			mouse_handler();
			break;
		case 13:

			break;
		case 14:

			break;
		case 15:
 
			break;
		case 16:

			break;
		case 17:

			break;
		case 18:
 
			break;
		case 19:

			break;
		case 20: 
			
			break;
		case 21:
 
			break;
		case 22:
 
			break;
		case 23:

			break;
		
		default:
			print("Default IRQ n: %d\n",n - 0x40);
		break;
	}
	


	apic_eoi_register();
	
}


VOID irq_install()
{
	REG reg;
	reg.cs = 8;
	
    	interrupter(0x40,(UINTN)irq0,reg.cs,0);
	interrupter(0x41,(UINTN)irq1,reg.cs,0);
	interrupter(0x42,(UINTN)irq2,reg.cs,0);
	interrupter(0x43,(UINTN)irq3,reg.cs,0);
	interrupter(0x44,(UINTN)irq4,reg.cs,0);
	interrupter(0x45,(UINTN)irq5,reg.cs,0);
	interrupter(0x46,(UINTN)irq6,reg.cs,0);
	interrupter(0x47,(UINTN)irq7,reg.cs,0);
	interrupter(0x48,(UINTN)irq8,reg.cs,0);
	interrupter(0x49,(UINTN)irq9,reg.cs,0);
	interrupter(0x4A,(UINTN)irq10,reg.cs,0);
	interrupter(0x4B,(UINTN)irq11,reg.cs,0);
	interrupter(0x4C,(UINTN)irq12,reg.cs,0);
	interrupter(0x4D,(UINTN)irq13,reg.cs,0);
	interrupter(0x4E,(UINTN)irq14,reg.cs,0);
	interrupter(0x4F,(UINTN)irq15,reg.cs,0);
	interrupter(0x50,(UINTN)irq16,reg.cs,0);
	interrupter(0x51,(UINTN)irq17,reg.cs,0);
	interrupter(0x52,(UINTN)irq18,reg.cs,0);
	interrupter(0x53,(UINTN)irq19,reg.cs,0);
	interrupter(0x54,(UINTN)irq20,reg.cs,0);
	interrupter(0x55,(UINTN)irq21,reg.cs,0);
	interrupter(0x56,(UINTN)irq22,reg.cs,0);
	interrupter(0x57,(UINTN)irq23,reg.cs,0);
}





























