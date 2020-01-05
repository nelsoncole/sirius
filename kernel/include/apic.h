/*
 * File Name: apic.h
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
 
 #ifndef __APIC_H__
 #define __APIC_H__
 
 
#define IA32_APIC_BASE_MSR 0x1B
#define IA32_APIC_BASE_MSR_BSP 0x100
#define IA32_APIC_BASE_MSR_ENABLE 0x800
 
 // Local APIC
 
#define IA32_LOCAL_APIC_BASE_ADDR 0xFEE00000

#define APIC_ID 			0x20
#define APIC_TPR 			0x80 
#define APIC_EOI			0xb0
#define APIC_S_INT_VECTOR 		0xf0
#define APIC_LVT_TIMER 			0x320
#define APIC_LVT_PERFORMANCE		0x340
#define APIC_LVT_LINT0			0x350
#define APIC_LVT_LINT1			0x360
#define APIC_LVT_ERROR			0x370
#define APIC_INITIAL_COUNT_TIMER 	0x380
#define APIC_CURRENT_COUNT_TIMER	0x390
#define APIC_DIVIDE_TIMER		0x3e0

#define APIC_CONFIG_DATA_LVT(TimerMode,Mask,TriggerMode,Remote,InterruptInput,DeliveryMode,Vector)(\
(((UINT32)(TimerMode) &0x3 ) << 17) |\
(((UINT32)(Mask) &0x1 ) << 16) |\
(((UINT32)(TriggerMode) &0x1 ) << 15) |\
(((UINT32)(Remote) &0x1 ) << 14) |\
(((UINT32)(InterruptInput) &0x1 ) << 13) |\
(((UINT32)(DeliveryMode) &0x7 ) << 8) |\
((UINT32)(Vector) &0xff )\
)

#define APIC_ID_0 0x0

VOID apic_eoi_register();
VOID apic_initial_count_timer(UINTN value);
UINTN apic_timer();
UINTN apic_initialize(UINTN _lapicbase);

VOID apic_timer_umasked();
VOID apic_timer_masked();

/*  I/O APIC
 *
 *
 *
 */


#define IO_APIC_BASE 	0xFEC00000
#define IO_APIC_IND 	0
#define IO_APIC_DAT 	(IO_APIC_IND +0x10)
#define IO_APIC_EOIR	(IO_APIC_IND +0x40)

#define IO_APIC_ID 		0x0
#define IO_APIC_VER 		0x1
#define IO_APIC_ARB		0x2
#define IO_APIC_REDIR_TBL(n) 	(0x10 + 2 * n) //REDIR_TBL[0:23] 64 bits 


typedef volatile struct _IOAPIC_REDIR_TBL {
	UINT64 	vector		: 8;
	UINT64	delv_mode 	: 3;
	UINT64	dest_mode	: 1;
	UINT64	delv_status	: 1;
	UINT64	pin_polarity	: 1;
	UINT64	remote_IRR	: 1;
	UINT64	trigger_mode	: 1;
	UINT64	mask		: 1;
	UINT64	rsv		: 39;
	UINT64	destination	: 8;
	
}__attribute__((packed)) IOAPIC_REDIR_TBL;

UINTN ioapic_initialize();
UINTN ioapic_masked(UINTN n);
UINTN ioapic_umasked(UINTN n);

 
 
#endif






























