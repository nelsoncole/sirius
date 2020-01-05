;==========================================================================================
;
; File Name: flush.asm
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
BITS 32
section .text
extern bootdevice_num
GLOBAL gdt_flush

	gdt_flush:
    	push ebp
	mov ebp,esp
    	mov eax, [esp+8]
    	lgdt [eax]
    	jmp 0x08:.flush
.flush:
    	mov ax, 0x10
    	mov ds, ax
    	mov es, ax
    	mov fs, ax
    	mov gs, ax
    	mov esp,ebp
	pop ebp
    	ret

GLOBAL lidt_install
EXTERN idt
lidt_install:

    	push ebp
	mov ebp,esp

    	mov eax,idt_ptr
    	lidt [eax]
    	mov esp,ebp
	pop ebp
    	ret



IDT_LIMIT equ $ - 48*8
idt_ptr:
	dw IDT_LIMIT - 1
	dd idt

global execkernelx86
execkernelx86:
    	mov eax,[esp+4]
	mov ebx,[esp+8]

	cli
	push ebx
	push ss
	push esp
	pushf
	push cs
    	push eax
	iretd

section .data 
