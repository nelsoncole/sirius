;==========================================================================================
;
; File Name: stage1.asm
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


BITS 16
section .text
GLOBAL _start
_start:
	JMP 0:START

	EXTERN	puts16
	EXTERN	gatea20
	EXTERN	gdtr
	EXTERN	bootdevice_num
	EXTERN	GetDeviceParameters
	EXTERN	init_vbe16
	EXTERN 	UID


START:
	CLI	
    	MOV AX, CS
	MOV DS, AX
	MOV ES, AX
    	MOV AX, 0x9000
	MOV SS, AX
	XOR AX, AX
	mov SP, AX  ; SP = 0x90000
	STI


; Salvar o valor de disco 
	MOV	BYTE [bootdevice_num],DL
	MOV	DWORD [UID], EBX

	MOV 	SI, String
	CALL 	puts16
	MOV 	SI, String0
	CALL 	puts16


; BIOSes Get Device Parameters

	MOV 	SI, String1
	CALL 	puts16
	CALL	GetDeviceParameters

; VESA VBE
	MOV 	SI, String2
	CALL 	puts16
	CALL	init_vbe16

; Habilita Gate A20
	MOV 	SI, String3
	CALL 	puts16
	CALL 	gatea20

; Testa a Gate A20 

	


; Mascara interrupções e desabilita NMI
	
	cli 	    ;

	in al,0x70
	or al,0x80
	out 0x70,al

; Instala a GDT
    	mov eax,gdtr
	db 0x66		; Execute GDT 32-Bits
	lgdt [eax]

; Habilita o PE 
	mov eax,cr0
	or eax,1
	mov cr0,eax

; Pula para modo protegido
	jmp dword 0x8:ModoProtegido



; Retornos de Erro
booterro:
	call puts16
	pop si

; Espera uma interrupção do teclado
	xor ax,ax
	int 0x16
	int 0x18



bits 32
extern main
ModoProtegido:
    	mov eax,0x10
	mov ds,ax
	mov es,ax
	mov fs,ax
	mov gs,ax
	mov ss,ax
	mov esp,0x90000	; Nossa pilha

	
	call main
	
	

; Põe o processador para dormir
L1:	cli
	hlt
	jmp L1

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


	String		DB "Sirius Operating System v2.0",10,13,0
	String0		DB "BootLoader initialize...",10,13,0
	String1		DB "Initialize BIOSes Get Device Parameters",10,13,0
	String2		DB "Initialize VESA VBE",10,13,0
	String3		DB "Habilita Gate A20",10,13,0
