/*
 * File Name: dscptor.c
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

extern VOID gdt_flush(UINT32*);
extern VOID idt_flush(UINT32*);

extern VOID exceptions_install();
extern VOID lvt_install();
extern VOID irq_install();
extern VOID syscall_install();

GDT gdt[256];
GDTR gdtr[1];

IDT idt[256];
IDTR idtr[1];

TSS tss[1];

// GDT
static
VOID set_gdt(INTN n,UINT32 limit,UINT32 base,UINT8 type,
UINT8 s,UINT8 dpl,UINT8 p,UINT8 avl,UINT8 l,UINT8 db,UINT8 g)
{


	gdt[n].limit_15_0 = limit &0xFFFF;
	gdt[n].base_15_0 = base &0xFFFF;
	gdt[n].base_23_16 = base >> 16 &0xFF;
	gdt[n].type=type &0xF;
	gdt[n].s=s &0x1;
	gdt[n].dpl =dpl &0x3;
	gdt[n].p=p &0x1;
	gdt[n].limit_19_16= limit >>16 &0xF;
	gdt[n].avl=avl &1;
	gdt[n].l=l &1;
	gdt[n].db=db &1;
	gdt[n].g=g &1;
	gdt[n].base_31_24 = base  >> 24 &0xFF;

}


UINTN gdt_install(VOID)
{

	setmem(gdt,(sizeof(GDT)*256)-1,0);
	//     (n,limit ,base ,type,s,dpl,p,avl,l,db,g)
	set_gdt(0,0,0,0,0,0,0,0,0,0,0);
	set_gdt(1,0xFFFFF,0x00000000,0xA,1,0,1,0,0,1,1); //dpl 0
	set_gdt(2,0xFFFFF,0x00000000,0x2,1,0,1,0,0,1,1); //dpl 0
	set_gdt(3,0xFFFFF,0x00000000,0xA,1,3,1,0,0,1,1); //dpl 3
	set_gdt(4,0xFFFFF,0x00000000,0x2,1,3,1,0,0,1,1); //dpl 3
	set_gdt(5,sizeof(TSS)-1,(UINT32)tss,0x9,0,3/*dpl*/,1,0,0,1,1); //tss dpl 3
	//set_gdt(6,sizeof(ldt_t)-1,(uint32_t)&ldt,0x2,0,3,1,0,0,1,1); //ldt dpl 3



    	gdtr->limit = (sizeof(GDT)*256)-1;
    	gdtr->base = (UINT32)gdt;
        gdt_flush((UINT32*)gdtr);

	return 0;
  
}


// TSS
static 
VOID load_ltr(UINT16 tr){
	__asm__ __volatile__ (\
	"movw %w0,%%ax;\
	ltrw %%ax;"\
	::"a"(tr)\
	);

}

// Instaloador da TSS
// TODO: O TSS em nosso caso somente seria necessário 
// para perservar e tratar as pilhas dos diferentes níveis de privilégio
// Todo OS que usa mecanismo de proteção de níveis de privilégio 0,1,2 e 3
// mesmo quando o mesmo usa Multitarefa de software, este deve por pelomenos configurar um TSS
UINTN tss_install()
{

	setmem(&tss,sizeof(TSS),0);

	tss->ss0 	= 0x10; //RPL 0
	tss->esp0	= (UINT32)(&stack_esp_0);  // TODO Default TSS ESP0 
	tss->cs		= 0xb; // CPL e RPL 3 user
	tss->ss=tss->ds=tss->es=tss->fs=tss->gs = 0x13; // CPL e RPL 3 user

    	tss->esp =  (UINT32)(&stack_esp); // TODO

	load_ltr(0x2b);   // RPL 3 user


    

	return 0;
}

VOID tss_esp0(UINT32 esp)
{
	tss->esp0 = esp;
}


// IDT

static
VOID set_gate_idt(INTN n,UINT32 offset,UINT16 sel,UINT8 flags,UINT8 dpl,UINT8 p)
{

            idt[n].offset_15_0 = offset &0xFFFF;
            idt[n].sel = sel;
            idt[n].unused = 0;
            idt[n].flags = flags;
            idt[n].dpl = dpl;
            idt[n].p = p;
            idt[n].offset_31_16 = offset >> 16 &0xFFFF;



}


UINTN idt_install(VOID)
{

    	setmem(idt,(sizeof(IDT)*256)-1,0);
    
	// 0 - 0x1F
    	exceptions_install();
	// 0x20 - 0x24 
	lvt_install();
	// 0x40 -
    	irq_install();
	// 0x72
	syscall_install();

	idtr->limit = (sizeof(IDT)*256)-1;
    	idtr->base = (UINTN)idt;
    
    	idt_flush((UINT32*)idtr);

	return 0;
	

}

VOID trap(INTN n,UINT32 offset,UINT16 sel,UINT8 dpl )
{
    	set_gate_idt(n,offset,sel,0x8F,dpl,1);

}

VOID interrupter(INTN n,UINT32 offset,UINT16 sel,UINT8 dpl )
{
	set_gate_idt(n,offset,sel,0x8E,dpl,1);

}


