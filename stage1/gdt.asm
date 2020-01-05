;==========================================================================================
;
; File Name: gdt.asm
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
section .data
global gdtr

gdt:

;0x0 Entrada Nula, em um GDT a primeira entrada é sempre nula

	dw 0		; Limit
	dw 0		; Base 15:00
	db 0		; Base 23:16
	db 0		; Flags
	db 0		; Flags e Limit 19:16
	db 0		; Base 31:24

; 0x8 CS LIMIT 0xFFFFF, BASE 0x00000000, DPL=0, G=1, S=1 code ou data

	dw 0xFFFF   	; Limit
	dw 0        	; Base 15:00
	db 0        	; Base 23:16
	db 0x9A     	; Flags
	db 0xCF     	; Flags e Limit 19:16
	db 0	    	; Base 31:24
	

; 0x10 DS LIMIT 0xFFFFF, BASE 0x00000000, DPL=0, G=1, S=1 code ou data

	dw 0xFFFF	; Limit
	dw 0		; Base 15:00
	db 0		; Base 23:16
	db 0x92	  	; Flags
	db 0xCF        	; Flags e Limit 19:16
	db 0           	; Base 31:24

;0x18 ;SEGMENTO DE CODIGO (CS 16-BITS) DPL 0, LIMIT 0XFFFF BASE 0X00000000 G 0 1B

	dw 0xFFFF
	dw 0x0
	db 0x0
	db 0x9E
	db 0x0
	db 0x0

;0x20 SEGMENTO DE DADOS (CD 16-BITS)  LIMIT 0XFFFF BASE 0X00000000 G 0 1B

	dw 0xFFFF
	dw 0x0
	db 0x0
	db 0x92
	db 0x0
	db 0x0


gdt_end:



; Crindo nosso ponteiro gdtr

gdtr:
	dw gdt_end - gdt -1	; LIMIT
	dd gdt			; BASE

