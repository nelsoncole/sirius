/*
 * File Name: lvt.c
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
extern VOID trap(INTN n,UINT32 offset,UINT16 sel,UINT8 dpl );

extern VOID lvt0();
extern VOID lvt1();
extern VOID lvt2();
extern VOID lvt3();
extern VOID lvt4();

VOID lvt_function(INTN  n){
	
	switch(n - 0x20){
		
		case 0: // APIC Timer
			apic_initial_count_timer(100000);
		break;
		case 1: // APIC_LVT_PERFORMANCE
		
			OutputString("APIC_LVT_PERFORMANCE\n");
		break;
		case 2: // APIC_LVT_LINT0
			OutputString("APIC_LVT_LINT0\n");
		break;
		case 3: // APIC_LVT_LINT1
			OutputString("APIC_LVT_LINT1\n");
		break;
		case 4: // APIC_LVT_ERROR

			OutputString("APIC_LVT_ERROR\n");
		break;
		default:
			print("Default LVT n: %d\n",n - 0x20);
		break;
	}
	
	apic_eoi_register();
	
}


VOID lvt_install()
{
	REG reg;
	reg.cs = 8;
	
    	interrupter(0x20,(UINTN)lvt0,reg.cs,0);
	interrupter(0x21,(UINTN)lvt1,reg.cs,0);
	interrupter(0x22,(UINTN)lvt2,reg.cs,0);
	interrupter(0x23,(UINTN)lvt3,reg.cs,0);
	interrupter(0x24,(UINTN)lvt4,reg.cs,0);
}
