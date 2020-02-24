;==========================================================================================
;
; File Name: vector.asm
;
;
; BSD 3-Clause License
; 
; Copyright (c) 2019, nelsoncole
; All rights reserved.
; 
; Redistribution and use in source and binary forms, with or without
; modification, are permitted provided that the following conditions are met:
; 
; 1. Redistributions of source code must retain the above copyright notice, this
;    list of conditions and the following disclaimer.
; 
; 2. Redistributions in binary form must reproduce the above copyright notice,
;    this list of conditions and the following disclaimer in the documentation
;    and/or other materials provided with the distribution.
; 
; 3. Neither the name of the copyright holder nor the names of its
;    contributors may be used to endorse or promote products derived from
;   this software without specific prior written permission.
;
; THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
; AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
; IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
; DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
; FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
; DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
; SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
; CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
; OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
; OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
;
;
;========================================================================================

bits 32
section .text
global isr00,isr01,isr02,isr03,isr04,isr05,isr06,isr07,isr08,isr09
global isr10,isr11,isr12,isr13,isr14,isr15,isr16,isr17,isr18,isr19
global isr20,isr21,isr22,isr23,isr24,isr25,isr26,isr27,isr28,isr29
global isr30,isr31


extern fault_exception
isr_jmp:
	cli
	pushad
	push ds
	push es
	push fs
	push gs
    	push ss
	mov eax,[esp+52]	; pega vector number
	push eax
	mov eax,0x10
	mov ds,ax
	mov es,ax
	mov fs,ax
	mov gs,ax

	call fault_exception
	pop eax
	
    	pop ss
	pop gs
	pop fs
	pop es
	pop ds
	popad	
	add esp,8
	sti
	iretd
isr00:
	push dword 0
	push dword 0
	jmp isr_jmp
isr01:
	push dword 0
	push dword 1
	jmp isr_jmp
isr02:
	push dword 0
	push dword 2
	jmp isr_jmp
isr03:
	push dword 0
	push dword 3
	jmp isr_jmp
isr04:
	push dword 0
	push dword 4
	jmp isr_jmp
isr05:
	push dword 0
	push dword 5
	jmp isr_jmp
isr06:
	push dword 0
	push dword 6
	jmp isr_jmp
isr07:
	push dword 0
	push dword 7
	jmp isr_jmp
isr08:
	;error code
	push dword 8
	jmp isr_jmp
isr09:
	push dword 0
	push dword 9
	jmp isr_jmp
isr10:
	;error code
	push dword 10
	jmp isr_jmp
isr11:
	;error code
	push dword 11
	jmp isr_jmp
isr12:
	;error code
	push dword 12
	jmp isr_jmp
isr13:
	;error code
	push dword 13
	jmp isr_jmp
isr14:
	;error code
	push dword 14
	jmp isr_jmp
isr15:
	push dword 0
	push dword 15
	jmp isr_jmp
isr16:
	push dword 0
	push dword 16
	jmp isr_jmp
isr17:
	;error code
	push dword 17
	jmp isr_jmp
isr18:
	push dword 0
	push dword 18
	jmp isr_jmp
isr19:
	push dword 0
	push dword 19
	jmp isr_jmp
isr20:
	push dword 0
	push dword 20
	jmp isr_jmp
isr21:
	push dword 0
	push dword 21
	jmp isr_jmp
isr22:
	push dword 0
	push dword 22
	jmp isr_jmp
isr23:
	push dword 0
	push dword 23
	jmp isr_jmp
isr24:
	push dword 0
	push dword 24
	jmp isr_jmp
isr25:
	push dword 0
	push dword 25
	jmp isr_jmp
isr26:
	push dword 0
	push dword 26
	jmp isr_jmp
isr27:
	push dword 0
	push dword 27
	jmp isr_jmp
isr28:
	push dword 0
	push dword 28
	jmp isr_jmp
isr29:
	push dword 0
	push dword 29
	jmp isr_jmp
isr30:
	push dword 0
	push dword 30
	jmp isr_jmp
isr31:
	push dword 0
	push dword 31
	jmp isr_jmp
	




global lvt0,lvt1,lvt2,lvt3,lvt4

extern lvt_function
lvt_jmp:
	cli
	pushad
	push ds
	push es
	push fs
	push gs
    	push ss
	mov eax,[esp+52]	; pega vector number
	push eax
	mov eax,0x10
	mov ds,ax
	mov es,ax
	mov fs,ax
	mov gs,ax

	call lvt_function
	
	pop eax
	
    	pop ss
	pop gs
	pop fs
	pop es
	pop ds
	popad	
	add esp,8
	sti
	iretd



; registers
extern	context_eip;
extern	context_eflag;
extern	context_cr3;

; registers 
extern	context_eax;
extern	context_ebx;
extern	context_ecx;
extern	context_edx;

; registers 
extern	context_edi;
extern	context_esi;

; registers
extern	context_ebp;
extern	context_esp;

; segments
extern	context_cs;
extern	context_es;
extern	context_ds;
extern	context_fs;
extern	context_gs;
extern	context_ss;




extern task_switch;
extern global_controll_task_switch;
lvt0:
	

	cmp dword [global_controll_task_switch],0
	jnz _no

	pop dword [context_eip	]
    	pop dword [context_cs	]
    	pop dword [context_eflag]
    	pop dword [context_esp	]
    	pop dword [context_ss	]

	;registers 
    	mov dword [context_eax], eax 
    	mov dword [context_ebx], ebx 
    	mov dword [context_ecx], ecx
    	mov dword [context_edx], edx

	;registers  
    	mov dword [context_edi], edi
    	mov dword [context_esi], esi
	
	;registers
	mov dword [context_ebp], ebp

	;segments
    	xor eax, eax
    	mov ax, ds
    	mov word [context_ds], ax
    	mov ax, es
    	mov word [context_es], ax
    	mov ax, fs
    	mov word [context_fs], ax
    	mov ax, gs
    	mov word [context_gs], ax


	mov eax, cr3
    	mov dword [context_cr3], eax
    	

	call task_switch


	;segments
    	xor eax, eax
    	mov ax,	word [context_ds]
    	mov ds,	ax
	mov ax,	word [context_es]
    	mov es,	ax
	mov ax,	word [context_fs]
    	mov fs,	ax
	mov ax,	word [context_gs]
    	mov gs,	ax
    	

	;registers
	mov ebp, dword [context_ebp]

	;registers  
    	mov edi, dword [context_edi]
    	mov esi, dword [context_esi]

	;registers 
    	mov eax, dword [context_eax]
    	mov ebx, dword [context_ebx]
    	mov ecx, dword [context_ecx]
    	mov edx, dword [context_edx]


	push dword [context_ss	 ]
	push dword [context_esp	 ]
	push dword [context_eflag]
	push dword [context_cs	 ]
	push dword [context_eip	 ]


; LVT0
_no:
	push dword 0
	push dword 32
	jmp lvt_jmp



lvt1:
	push dword 0
	push dword 33
	jmp lvt_jmp
lvt2:
	push dword 0
	push dword 34
	jmp lvt_jmp
lvt3:
	push dword 0
	push dword 35
	jmp lvt_jmp
lvt4:
	push dword 0
	push dword 36
	jmp lvt_jmp




global irq0,irq1,irq2,irq3,irq4,irq5,irq6,irq7,irq8,irq9
global irq10,irq11,irq12,irq13,irq14,irq15,irq16,irq17,irq18,irq19
global irq20,irq21,irq22,irq23

extern irq_function
irq_jmp:
	cli
	pushad
	push ds
	push es
	push fs
	push gs
    	push ss
	mov eax,[esp+52]	; pega vector number
	push eax
	mov eax,0x10
	mov ds,ax
	mov es,ax
	mov fs,ax
	mov gs,ax

	call irq_function
	
	pop eax
	
    	pop ss
	pop gs
	pop fs
	pop es
	pop ds
	popad	
	add esp,8

	sti
	iretd


irq0:
	push dword 0
	push dword 64
	jmp irq_jmp
irq1:
	push dword 0
	push dword 65
	jmp irq_jmp
irq2:
	push dword 0
	push dword 66
	jmp irq_jmp
irq3:
	push dword 0
	push dword 67
	jmp irq_jmp
irq4:
	push dword 0
	push dword 68
	jmp irq_jmp
irq5:
	push dword 0
	push dword 69
	jmp irq_jmp
irq6:
	push dword 0
	push dword 70
	jmp irq_jmp
irq7:
	push dword 0
	push dword 71
	jmp irq_jmp
irq8:
	push dword 0
	push dword 72
	jmp irq_jmp
irq9:
	push dword 0
	push dword 73
	jmp irq_jmp

irq10:
	push dword 0
	push dword 74
	jmp irq_jmp
irq11:
	push dword 0
	push dword 75
	jmp irq_jmp
irq12:
	push dword 0
	push dword 76
	jmp irq_jmp
irq13:
	push dword 0
	push dword 77
	jmp irq_jmp
irq14:
	push dword 0
	push dword 78
	jmp irq_jmp
irq15:
	push dword 0
	push dword 79
	jmp irq_jmp
irq16:
	push dword 0
	push dword 80
	jmp irq_jmp
irq17:
	push dword 0
	push dword 81
	jmp irq_jmp
irq18:
	push dword 0
	push dword 82
	jmp irq_jmp
irq19:
	push dword 0
	push dword 83
	jmp irq_jmp
irq20:
	push dword 0
	push dword 84
	jmp irq_jmp
irq21:
	push dword 0
	push dword 85
	jmp irq_jmp
irq22:
	push dword 0
	push dword 86
	jmp irq_jmp
irq23:
	push dword 0
	push dword 87
	jmp irq_jmp



global int113,int114,
extern syscall_handler

int113:
	cli

	pop dword [context_eip	]
    	pop dword [context_cs	]
    	pop dword [context_eflag]
    	pop dword [context_esp	]
    	pop dword [context_ss	]


	;registers 
    	mov dword [context_eax], eax 
    	mov dword [context_ebx], ebx 
    	mov dword [context_ecx], ecx
    	mov dword [context_edx], edx

	;registers  
    	mov dword [context_edi], edi
    	mov dword [context_esi], esi
	
	;registers
	mov dword [context_ebp], ebp

	;segments
    	xor eax, eax
    	mov ax, ds
    	mov word [context_ds], ax
    	mov ax, es
    	mov word [context_es], ax
    	mov ax, fs
    	mov word [context_fs], ax
    	mov ax, gs
    	mov word [context_gs], ax


	mov eax, cr3
    	mov dword [context_cr3], eax
    	

	call task_switch


	;segments
    	xor eax, eax
    	mov ax,	word [context_ds]
    	mov ds,	ax
	mov ax,	word [context_es]
    	mov es,	ax
	mov ax,	word [context_fs]
    	mov fs,	ax
	mov ax,	word [context_gs]
    	mov gs,	ax
    	

	;registers
	mov ebp, dword [context_ebp]

	;registers  
    	mov edi, dword [context_edi]
    	mov esi, dword [context_esi]

	;registers 
    	mov eax, dword [context_eax]
    	mov ebx, dword [context_ebx]
    	mov ecx, dword [context_ecx]
    	mov edx, dword [context_edx]


	push dword [context_ss	 ]
	push dword [context_esp	 ]
	push dword [context_eflag]
	push dword [context_cs	 ]
	push dword [context_eip	 ]
	sti
    	iretd


sytem_num dd 0;
int114:   
	cli
	push ds
	push es
    	push ss
	push fs
	push gs

    	push ebx
    	push ecx
    	push edx
    	push edi
    	push esi
    	push ebp
    	push esp

    	push eax
	mov eax,0x10
	mov ds,ax
	mov es,ax
	mov fs,ax
	mov gs,ax
	pop eax


	mov dword[sytem_num], eax
	push dword[sytem_num]
	call syscall_handler
	add esp, 4

    	pop esp
    	pop ebp
    	pop esi
    	pop edi
    	pop edx
    	pop ecx
    	pop ebx

	pop gs
	pop fs
    	pop ss
	pop es
	pop ds
    
	sti
	iret



	
