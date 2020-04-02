/*
 * File Name: exceptions.c
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


extern VOID trap(INTN n,UINT32 offset,UINT16 sel,UINT8 dpl );
extern VOID interrupter(INTN n,UINT32 offset,UINT16 sel,UINT8 dpl );


extern void isr00();	extern void isr10();	extern void isr20();	extern void isr30();		
extern void isr01();	extern void isr11();	extern void isr21();	extern void isr31();
extern void isr02();	extern void isr12();	extern void isr22();
extern void isr03();	extern void isr13();	extern void isr23();
extern void isr04();	extern void isr14();	extern void isr24();
extern void isr05();	extern void isr15();	extern void isr25();
extern void isr06();	extern void isr16();	extern void isr26();
extern void isr07();	extern void isr17();	extern void isr27();
extern void isr08();	extern void isr18();	extern void isr28();
extern void isr09();	extern void isr19();	extern void isr29();


CHAR8 *exception_mensagem[32]={
	"Divide error\n",		// 0 - fault  -- no error 
	"Debug exception\n", 	// 1 - fault/trap -- no error 
	"NMI Interrupter\n",	//  2 - interrupter -- no error
	"Breakpoint\n",		// 3 - trap -- no error
	"Overflow\n",		// 4 - trap -- no error
	"BOUND Ranger exception\n", // 5 - trap -- no error
	"Invalide opcode (Undefined opcode)\n", // 6 - fault -- no error
	"Device not avaliable (Not Math coprocessor\n)", // 7 - fault -- no error
	"Double Fault (Erro de codigo)\n",		// 8 - about -- error code
	"Coprocessor segment overrun (reservado)\n",	// 9 - fault -- no error
	"Invalide TSS (Erro de codigo)\n", 	// 10 - fault -- error code
	"Segment not present (Erro de codigo)\n",		// 11 - fault -- error code
	"Stack segment fault (Erro de codigo)\n",	// 12 - fault -- error code
	"General protetion (Erro de codigo)\n",	// 13 - fault -- error code
	"Page fault (Erro de codigo)\n",		// 14 - fault -- error code
	"Intel reserved do not use 15\n",		// 15
	"x87 FPU Floating-Point error (Math fault)\n",	// 16 - fault -- no error
	"Alignment check (Erro de codigo)\n",		// 17 - fault -- error code
	"Machine check\n",	// 18 - about -- no error 
	"SIND Floating-Point exception\n", // 19 - fault -- no error
	"Virtualization exception\n",	// 20 - fault -- no error
	"Intel reserved do not use 21\n",	// 21
	"Intel reserved do not use 22\n",	// 22
	"Intel reserved do not use 23\n",	// 23
	"Intel reserved do not use 24\n",	// 24
	"Intel reserved do not use 25\n",	// 25
	"Intel reserved do not use 26\n",	// 26
	"Intel reserved do not use 27\n",	// 27
	"Intel reserved do not use 28\n",	// 28
	"Intel reserved do not use 29\n",    // 29
	"Intel reserved do not use 30\n",	// 30
	"Intel reserved do not use 31\n",	// 31

};



VOID fault_exception(INTN  n)
{
	UINTN  	cr2,cr3,cr4;


	if(n == 8 || n == 18)SetAttribute(GW_RED,GW_WHITE);
	else SetAttribute(GW_YELLOW,GW_WHITE);
		OutputString(exception_mensagem[n]);
	if(n == 14){

		__asm__ __volatile__("movl %%cr2,%k0":"=a"(cr2):);
		__asm__ __volatile__("movl %%cr3,%k0":"=a"(cr3):);
		__asm__ __volatile__("movl %%cr4,%k0":"=a"(cr4):);
		print("CR2 = 0x%x\nCR3 = 0x%x\nCR4 = 0x%x\n",cr2,cr3,cr4);

		}

	print("PID = %d\nEBP = 0x%x\nESP = 0x%x\n",getpid(), current_thread->ebp, current_thread->esp);
	print("CS = 0x%x\nDS = 0x%x\nES = 0x%x\n",current_thread->cs, current_thread->ds, current_thread->es);
	print("FS = 0x%x\nGS = 0x%x\nSS = 0x%x\n",current_thread->fs, current_thread->gs, current_thread->ss);

	for(;;);

	if(!exit());
	else for (;;);	

}



VOID exceptions_install()
{
	REG reg;
	reg.cs = 8;
	
    	trap(0x00,(UINTN)isr00,reg.cs,0);
	trap(0x01,(UINTN)isr01,reg.cs,0);
    	interrupter(0x02,(UINTN)isr02,reg.cs,0);
	trap(0x03,(UINTN)isr03,reg.cs,0);
	trap(0x04,(UINTN)isr04,reg.cs,0);
	trap(0x05,(UINTN)isr05,reg.cs,0);
	trap(0x06,(UINTN)isr06,reg.cs,0);
	trap(0x07,(UINTN)isr07,reg.cs,0);
	trap(0x08,(UINTN)isr08,reg.cs,0);
	trap(0x09,(UINTN)isr09,reg.cs,0);
	trap(0x0A,(UINTN)isr10,reg.cs,0);
	trap(0x0B,(UINTN)isr11,reg.cs,0);
	trap(0x0C,(UINTN)isr12,reg.cs,0);
	trap(0x0D,(UINTN)isr13,reg.cs,0);
	trap(0x0E,(UINTN)isr14,reg.cs,0);
	trap(0x0F,(UINTN)isr15,reg.cs,0);
	trap(0x10,(UINTN)isr16,reg.cs,0);
	trap(0x11,(UINTN)isr17,reg.cs,0);
	trap(0x12,(UINTN)isr18,reg.cs,0);
	trap(0x13,(UINTN)isr19,reg.cs,0);
	trap(0x24,(UINTN)isr20,reg.cs,0);
	trap(0x15,(UINTN)isr21,reg.cs,0);
	trap(0x16,(UINTN)isr22,reg.cs,0);
	trap(0x17,(UINTN)isr23,reg.cs,0);
	trap(0x18,(UINTN)isr24,reg.cs,0);
	trap(0x19,(UINTN)isr25,reg.cs,0);
	trap(0x1A,(UINTN)isr26,reg.cs,0);
	trap(0x1B,(UINTN)isr27,reg.cs,0);
	trap(0x1C,(UINTN)isr28,reg.cs,0);
	trap(0x1D,(UINTN)isr29,reg.cs,0);
	trap(0x1E,(UINTN)isr30,reg.cs,0);
	trap(0x1F,(UINTN)isr31,reg.cs,0);
	
	
	
	
}
