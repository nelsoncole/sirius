/*
 * File Name: irqs.c
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

extern void irq00();	extern void irq08();
extern void irq01();	extern void irq09();
extern void irq02();	extern void irq10();
extern void irq03();	extern void irq11();
extern void irq04();	extern void irq12();
extern void irq05();	extern void irq13();
extern void irq06();	extern void irq14();
extern void irq07();	extern void irq15();


void irq_install()
{

	pic_install();
	interrupter(32,(uint32_t)irq00,0x8,0);
	interrupter(33,(uint32_t)irq01,0x8,0);
	interrupter(34,(uint32_t)irq02,0x8,0);
	interrupter(35,(uint32_t)irq03,0x8,0);
	interrupter(36,(uint32_t)irq04,0x8,0);
	interrupter(37,(uint32_t)irq05,0x8,0);
	interrupter(38,(uint32_t)irq06,0x8,0);
	interrupter(39,(uint32_t)irq07,0x8,0);
	interrupter(40,(uint32_t)irq08,0x8,0);
	interrupter(41,(uint32_t)irq09,0x8,0);
	interrupter(42,(uint32_t)irq10,0x8,0);
	interrupter(43,(uint32_t)irq11,0x8,0);
	interrupter(44,(uint32_t)irq12,0x8,0);
	interrupter(45,(uint32_t)irq13,0x8,0);
	interrupter(46,(uint32_t)irq14,0x8,0);
	interrupter(47,(uint32_t)irq15,0x8,0);

}



// Lista de funções c de entrada ao manipulador de IRQs.
static void *fnvetors_handler[16] = {
    0,//&timer_handler, // 0
    0,//&keyboard_handler, // 1
    0, // 2
    0, // 3
    0, // 4
    0, // 5
    0, // 6
    0, // 7
    0, // 8
    0, // 9
    0, // 10
    0, // 11
    0, // 12
    0, // 13
    0, //&ata_irq_handler1, // 14
    0, //&ata_irq_handler2 // 15
   
};

// Função para desabilitar IRQ n
void irq_interrupter(uint32_t irqv)
{
	
	irqv -= 32;

    	if(irqv == 0 || irqv < 16){
    	void *addr = fnvetors_handler[irqv];
    	// Chamda de função correspondente
    	__asm__ __volatile__ ("call *%0"::"r"(addr));

    	}else {

        //printf("Erro irqv %d",irqv);

    	}

    	if(irqv >=8 )outportb(0xA0,0x20);
	outportb(0x20,0x20);
	
}



void irq_enable(int irq)
{
	
	uint16_t OCW1 = 0xFFFB;
	
	OCW1 &= ~(1 << irq);
	
	if(irq < 8 ) outportb(0x21, inportb(0x21) & OCW1);
	else {
	irq -= 8;
	
	outportb(0xA1,inportb(0xA1) & (OCW1 >> 8));}

}

void irq_disable(int irq)
{

	uint16_t OCW1 = 0x0000;
	
	OCW1  |= (1 << irq);
	
	if(irq < 8 )outportb(0x21, inportb(0x21) | OCW1);
	else outportb(0xA1,inportb(0xA1) | ( OCW1 >> 8));

}
