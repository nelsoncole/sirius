/*
 * File Name: apic.c
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


UINTN lapicbase = 0;


static UINT32 getapicbase()
{
	UINT32 a,d;
	
	getmsr(IA32_APIC_BASE_MSR,&a,&d);
	
	return a;
}

UINTN enableapic(UINT32 apicbase)
{
	UINT32 a = (apicbase &0xfffff000) | IA32_APIC_BASE_MSR_ENABLE;
	UINT32 d = 0;
	
	setmsr(IA32_APIC_BASE_MSR,a,d);
	
	return 0;
	
	
}

VOID 
apic_eoi_register()
{
	*(UINT32*)(lapicbase + APIC_EOI) = 0;

}


UINTN apic_initialize(UINTN _lapicbase) {


	lapicbase = _lapicbase;

	//Legacy PIC mask all off
	// OCW1
	outportb(0x21,0xFF);
	outportb(0xA1,0xFF);

	// Local APIC ID Register
	*(volatile UINT32*)(lapicbase + APIC_ID) = APIC_ID_0 << 24;
	
	//Task Priority Register (TPR), to inhibit softint delivery
	*(volatile UINT32*)(lapicbase + APIC_TPR) = 0; //0x20;
	
	// Timer interrupt vector, to disable timer interrupts
	*(volatile UINT32*)(lapicbase + APIC_LVT_TIMER) = APIC_CONFIG_DATA_LVT(0,1/*Masked*/,0,0,0,NULL,0x20/*Vetor*/);
	
	// Performance counter interrupt, to disable performance counter interrupts
	*(volatile UINT32*)(lapicbase + APIC_LVT_PERFORMANCE) = APIC_CONFIG_DATA_LVT(NULL,0/*No Masked*/,0,0,0,0,0x21/*Vetor*/);
	
	// Local interrupt 0, to enable normal external interrupts, Trigger Mode = Level
	*(volatile UINT32*)(lapicbase + APIC_LVT_LINT0) = APIC_CONFIG_DATA_LVT(NULL,1/*Masked*/,NULL,NULL,1,7,0x22/*Vetor*/);
	
	// Local interrupt 1, to enable normal NMI processing
	*(volatile UINT32*)(lapicbase + APIC_LVT_LINT1) = APIC_CONFIG_DATA_LVT(NULL,1/*Masked*/,NULL,NULL,0,4,0x23/*Vetor*/);
	
	// Error interrupt, to disable error interrupts
	*(volatile UINT32*)(lapicbase + APIC_LVT_ERROR) = APIC_CONFIG_DATA_LVT(NULL,1/* Masked*/,NULL,NULL,NULL,0,0x24/*Vetor*/);
	
	// Spurious interrupt Vector Register, to enable the APIC and set
	// spurious vector to 15
	*(volatile UINT32*)(lapicbase + APIC_S_INT_VECTOR) = 0x10f;
	
	
	// Global enable
	enableapic(getapicbase());
	print("APIC Global enable %x\n",getapicbase());
	
	return 0;
	
}

VOID apic_initial_count_timer(UINTN value)
{
	*(volatile UINT32*)(lapicbase + APIC_INITIAL_COUNT_TIMER ) = value;

}
UINTN apic_timer()
{
	// Timer interrupt vector, to enable timer interrupts
	*(volatile UINT32*)(lapicbase + APIC_LVT_TIMER) = APIC_CONFIG_DATA_LVT\
	(0/*One-shot*/,1/*Masked*/,NULL,NULL,0,NULL,0x20/*Vetor*/);
	
	//Divide Configuration Register, to divide by 128
	*(volatile UINT32*)(lapicbase + APIC_DIVIDE_TIMER) = 0xA;

	// Initial Count Register
	*(volatile UINT32*)(lapicbase + APIC_INITIAL_COUNT_TIMER) = 123456;
	
	return 0;
	
}

VOID apic_timer_umasked(){

	UINT32 data = *(volatile UINT32*)(lapicbase + APIC_LVT_TIMER);

	*(volatile UINT32*)(lapicbase + APIC_LVT_TIMER) = data &0xFFFEFFFF;

	//Divide Configuration Register, to divide by 128 0xA divide by 32 0x8
	*(volatile UINT32*)(lapicbase + APIC_DIVIDE_TIMER) = 0x8;

	// Initial Count Register
	*(volatile UINT32*)(lapicbase + APIC_INITIAL_COUNT_TIMER) = 123456;
	

}

VOID apic_timer_masked(){

	UINT32 data = *(volatile UINT32*)(lapicbase + APIC_LVT_TIMER);
	*(volatile UINT32*)(lapicbase + APIC_LVT_TIMER) = (data | 1 << 16);

}



// I/O APIC


IOAPIC_REDIR_TBL ioapic_redir_table[24];

VIRTUAL_ADDRESS ioapic_base;

UINT32 read_ioapic_register(VIRTUAL_ADDRESS _ioapic_base, UINTN reg) {

	*(volatile UINT8*)(_ioapic_base + IO_APIC_IND) = reg;
	
	return ( *(volatile UINT32*)(_ioapic_base + IO_APIC_DAT));
}

VOID write_ioapic_register(VIRTUAL_ADDRESS _ioapic_base, UINTN reg, UINT32 val) {

	*(volatile UINT8*)(_ioapic_base + IO_APIC_IND) = reg;

	*(volatile UINT32*)(_ioapic_base + IO_APIC_DAT) = val;
}

UINTN ioapic_masked(UINTN n)
{

	ioapic_redir_table[n].mask = 1;
	UINT32 *data = (UINT32*)&ioapic_redir_table;
	write_ioapic_register(ioapic_base,IO_APIC_REDIR_TBL(n),*(UINT32*)(data + (n *2)));


	return 0;
}

UINTN ioapic_umasked(UINTN n)
{

	ioapic_redir_table[n].mask = 0;
	UINT32 *data = (UINT32*)&ioapic_redir_table;
	write_ioapic_register(ioapic_base,IO_APIC_REDIR_TBL(n),*(UINT32*)(data + (n *2)));


	return 0;
}


UINTN set_ioapic_redir_table(	VIRTUAL_ADDRESS _ioapic_base,
				UINTN n,UINT8 vector,UINT8 delv_mode,
				UINT8 dest_mode,UINT8 delv_status,
				UINT8 pin_polarity,UINT8 remote_IRR,
				UINT8 trigger_mode,UINT8 mask,
				UINT8 destination)
{

	UINT32 *data = (UINT32*)&ioapic_redir_table;

	ioapic_redir_table[n].vector 		= vector;
	ioapic_redir_table[n].delv_mode 	= delv_mode;
	ioapic_redir_table[n].dest_mode 	= dest_mode;
	ioapic_redir_table[n].delv_status	= delv_status;
	ioapic_redir_table[n].pin_polarity	= pin_polarity;
	ioapic_redir_table[n].remote_IRR	= remote_IRR;
	ioapic_redir_table[n].trigger_mode	= trigger_mode;
	ioapic_redir_table[n].mask		= mask;
	ioapic_redir_table[n].destination	= destination;

	write_ioapic_register(_ioapic_base,IO_APIC_REDIR_TBL(n),*(UINT32*)(data + (n *2)));
	write_ioapic_register(_ioapic_base,IO_APIC_REDIR_TBL(n) + 1,*(UINT32*)(data + (n *2) + 1));


	return 0;
}


UINTN ioapic_cofiguration(VIRTUAL_ADDRESS _ioapic_base)
{

	UINTN n;

	for(n = 0; n < 24; n++) {
	
	set_ioapic_redir_table(_ioapic_base,n/*IRQn*/,0x40 + n/*vector*/,\
	0/*Delivery Mode*/,0/*Destination Mode*/,0/*RO*/,0/*Interrupt Input Pin Polarity*/,\
	0/*RO*/,0,1/* masked*/,APIC_ID_0);

	}




	return 0;

}

UINTN ioapic_initialize()
{
	//UINTN io_apic_base_addr;

	// Mapear o IO APIC BASE
	// mem_map((PHYSICAL_ADDRESS)IO_APIC_IND,(VIRTUAL_ADDRESS *)&io_apic_base_addr,0x1000/*4KiB*/,0x13);

	ioapic_base = IO_APIC_BASE;

	write_ioapic_register(ioapic_base,IO_APIC_ID,APIC_ID_0);

	ioapic_cofiguration(ioapic_base);


	print("0x%x\n",ioapic_base);
	


	return 0;
}















