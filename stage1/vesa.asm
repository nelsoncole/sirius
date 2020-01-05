;==========================================================================================
;
; File Name: vesa.asm
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



LDSBASE equ 0x600
LDSVESABASE equ 0x4020

extern puts16


bits 16
section .data
msg0: 	db "VESA VBE ERROR",10,13,0
var0:   dw 0
var1:   db 32   ; BPB de pelomenos 32-bit
var2:   dw 1024 ; X Resolution de Pelomenos  => 1024 
var3:   dw 0x118  ; Especil mode  
global vbe_mode_info_block 
vbe_mode_info_block: times 512 db 0
section .text

	global init_vbe16
init_vbe16:
; Info da chip VBE


    	call .vbe_infos
    	cmp ax,0x004F
    	jne error

	push ds
	xor si,si
	mov ds,si
	mov si,LDSBASE
	mov di,LDSVESABASE
	mov es,di
	xor di,di
	mov cx,0x200
	cld
	rep movsb
	pop ds

;   Info do modo de video

	; verifica se especial mode bpp = 32 bits
	
	MOV 	CX, WORD [var3]
	CALL 	vbe_info_mode
    	CMP 	AX, 0x004F
    	JNE 	error
	MOV 	AL, BYTE[var1]
 	CMP 	BYTE [vbe_mode_info_block + 25], AL
	JZ 	.SUCCESSFULL
	JMP	.NO_SUCCESSFULL
.SUCCESSFULL:
	MOV	AX, WORD [var3]
	MOV 	WORD [var0], AX
	PUSH	ES
	JMP	.end2
	
.NO_SUCCESSFULL:
	; Procurar outro modo.


	push es
	mov ax,word[LDSBASE+16]
	mov es,ax
	mov di,word[LDSBASE+14]
	xor bx,bx
.next:
	mov ax,word[di+bx]
	mov word[var0],ax

	mov cx,word[var0]
	cmp cx,0xffff
	jz .end

    	call vbe_info_mode
    	cmp ax,0x004F
    	jne error

	mov al,byte[var1]
 	cmp byte[vbe_mode_info_block+25],al
	jz .ok
	jmp .continue
.ok:
	mov ax,word[var2]
	cmp word[vbe_mode_info_block+18],ax
	jge .end2
.continue:

	add bx,2
	jmp .next
.end:
	cmp byte[var1],0
	jne .continue2
	cmp word[var2],800
	jz .error	
	mov word[var2],800
	mov byte[var1],32	
	xor bx,bx
	jmp .next

.continue2:
	sub byte[var1],8 ; TODO SUB 8-bytes or 16-bytes
	xor bx,bx
	jmp .next
.error:
	
	pop es
	jmp error

.end2:
	pop es

	push ds
	xor si,si
	mov ds,si
	mov si,vbe_mode_info_block
	mov di,LDSVESABASE
	mov es,di
	mov di,0x200
	mov cx,0x200
	cld
	rep movsb
	pop ds

; Nelson Aqui, e por onde controlamos, o hablitar, desabiliatr do VBE, comentando descomentando o RET	
	 ;ret	
;   Define modo de video 
    
    	xor ebx,ebx
	mov bx,word[var0]
    	or bx, 0x4000
    	call vbe_define_mode
    	cmp ax,0x004F
    	jne error


    	ret




; Obtém infos, sobre o cartão VBE
.vbe_infos:
    	xor eax,eax    
    	push di
    	push es
    	mov ax,0x4F00
    	mov di,LDSBASE
    	mov dword[di],"VBE2"
    	int 0x10
    	pop es
    	pop di
    	ret




; Obtém infos, do modo VBE
; cx, numero de modo
vbe_info_mode:
    	xor eax,eax
	mov ax,0
	mov es,ax    
    	push di
    	push es
    	mov ax,0x4F01
    	mov di,vbe_mode_info_block
    	int 0x10
    	pop es
    	pop di
    	ret

; Define modo de vídeo
; bx numero de modo
vbe_define_mode:
    	xor eax,eax  
    	push di
    	push es
    	mov ax,0x4F02
    	int 0x10
    	pop es
    	pop di
    	ret


error:
	mov si,msg0  
    	call puts16 
	xor ax,ax
	int 0x16
    	ret
