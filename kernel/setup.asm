;==========================================================================================
;
; File Name: setup.asm
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
extern main  ;same defined reference in PE+ object
global _start
_start:
	jmp start
	times 0x1000 -($-$$)   	db 0
start:
	; Nelson, este é um bom momento 
	; para alterar a pilha do kernel.
	mov esp, stack

	; i386
	push ebx
	call main
	add esp,4
	
L1:
	hlt
	jmp L1;
	
global gdt_flush
global idt_flush

gdt_flush:
    	push ebp
	mov ebp, esp
    	mov eax, [esp+8]
    	lgdt [eax]
    	jmp 0x08:.flush
.flush:
    	mov ax, 0x10
    	mov ds, ax
    	mov es, ax
    	mov fs, ax
    	mov gs, ax
	mov ss, ax

    	mov esp, ebp
	pop ebp
    	ret

idt_flush:
    	push ebp
	mov ebp,esp
    	mov eax, [esp+8]
    	lidt [eax]
    	mov esp,ebp
	pop ebp
    	ret

; Esta função copiar 4 bytes no
; *Source para o *Destination
global __copymem
__copymem:
	push ebp
	mov ebp,esp

    	mov edi, [esp+  8]
	mov esi, [esp+ 12]
	mov ecx, [esp+ 16]

	xor edx, edx
	mov eax, ecx
	mov ebx, 4
	div ebx
	mov ecx,eax
.loop:
	mov eax, dword[esi]
	mov dword[edi], eax

	add edi,4
	add esi,4
	
	loop .loop

	mov esp,ebp
	pop ebp
    	ret

; Esta função escreve valor de 
; 4 bytes no *Destination
global __setmem
__setmem:
	push ebp
	mov ebp,esp

    	mov edi, [esp+  8]
	mov esi, [esp+ 12]
	mov ecx, [esp+ 16]

	xor edx, edx
	mov eax, ecx
	mov ebx, 4
	div ebx
	mov ecx,eax
.loop:
	mov dword[edi], esi

	add edi,4
	
	loop .loop

	mov esp,ebp
	pop ebp
    	ret


global stack_esp,stack_esp_0
section .bss
	resb 1024*128 ;128 KB
stack:
	resb 1024*32 ;32 KB
stack_esp:
	resb 1024*32 ;32 KB
stack_esp_0:
