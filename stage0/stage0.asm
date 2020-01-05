;==========================================================================================
;
; File Name: stage0.asm
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

BPB_BASE_ADDR	EQU 0x600
ROOT_BASE_ADDR	EQU 0x800
DAP		EQU	0x7E00
[BITS 16]
[ORG 0x7C00]

	CLI
        MOV     AX, CS
        MOV     DS, AX
	MOV	AX, 0x9000
	MOV 	SS, AX
	XOR	AX, AX
	MOV	SP, AX ; SP = 0x90000
	JMP 	0:START

	FileName	DB "STAGE1  BIN"


START:
	STI

	MOV 	BYTE [BS_DrvNum], DL

    	MOV 	AX, 3
    	INT 	0x10

; Verifica presença de extensão BIOS EDDs    
    	MOV 	BX, 0x55AA
	MOV 	DL, BYTE [BS_DrvNum]
	MOV 	AH, 0x41
	INT 	0x13
	JC 	ERROR1
	CMP 	BX, 0xAA55
	JNE 	ERROR1


; 1º Carregando o BPB na memorio
	MOV	CX, 1
	XOR	DI, DI
	MOV	BX, BPB_BASE_ADDR
	MOV	EAX,DWORD[PT1 + 0x8]
	CALL	ReadSectors
	

; 2º Calculando o RootDirSectors FAT12/16
	MOV 	SI, BPB_BASE_ADDR
	MOV	EAX, 32
    	MUL 	WORD [SI + 17]			; BPB_RootEntCnt
    	MOVZX 	ECX, WORD [SI + 11]		; BPB_BytsPerSec
    	SUB 	ECX, 1
    	ADD 	EAX, ECX
    	XOR 	DX, DX
    	DIV 	WORD [SI + 11]			; BPB_BytsPerSec
    	MOV 	DWORD [RootDirSectors], EAX

; 3º Calculando o 1º sector de dados, FirstDataSector FAT12/16
	MOV 	SI, BPB_BASE_ADDR
	XOR	EAX, EAX
	MOV 	AX, WORD [SI + 22]		; BPB_FATSz16
	MUL 	BYTE [SI + 16]			; BPB_NumFATs
	ADD 	AX, WORD [SI + 14]		; BPB_RsvdSecCnt
    	ADD 	AX, WORD [RootDirSectors]
	MOV 	DWORD [FirstDataSector], EAX



; 4º Calculando o 1º sector do Root Directory, FirstRootDirSector FAT12/16
	MOV 	SI, BPB_BASE_ADDR
	MOV 	EAX, DWORD [FirstDataSector]
    	SUB 	EAX, DWORD [RootDirSectors]
	ADD 	EAX, DWORD [SI + 28]		; BPB_HiddSec
    	MOV 	DWORD [FirstRootDirSector], EAX
	

; 5º Carregando o Root Directory na memorio FAT12/16
	MOV 	CX, WORD [RootDirSectors]
    	MOVZX 	EAX, WORD [FirstRootDirSector]
	XOR 	DI, DI
	MOV 	BX, ROOT_BASE_ADDR
	CALL 	ReadSectors
	JMP 	STAGE1

; 6º Aqui vamos analizar todas as entradas no Root Dir 
; ate achar o arquivo de nome 8.3 "STAGE1  BIN" (stage1.bin)
STAGE1:
	MOV 	BX, ROOT_BASE_ADDR
	MOV 	CX, 128
._1:
	PUSH	CX

	CMP 	BYTE [BX], 0		;Se for 0 entrada nao usada
	JE 	._2
	CMP 	BYTE [BX +11], 0x20	; Se for 0x20 arquivo de nome 8.3
	JNE 	._2

	
	MOV 	CX, 11
	MOV 	DI, BX
	MOV 	SI, FileName
._CMPSTR:
	MOV	AL, [SI]
	CMP	BYTE [DI], AL
	JNZ	._2
	INC	DI
	INC	SI
	LOOP 	._CMPSTR
	JMP 	.SUCCESSFULL
	


._2:
	ADD 	BX, 32
    	POP	CX
	LOOP	._1
	SUB	BX, 32
	JMP	.NO_SUCCESSFULL

.NO_SUCCESSFULL:
    	POP	CX
	JMP	ERROR1	


.SUCCESSFULL:
    	POP	CX


; 7º Lendo o stage1.bin
    	XOR	ESI, ESI
	MOV 	DI, BPB_BASE_ADDR

	MOV 	AX, WORD [BX + 26]
	SUB 	AX, 2

	MOVZX 	CX, BYTE[DI + 13]	; BPB_SecPerClus
	MUL 	CX

    	ADD 	AX, WORD [FirstDataSector] 
	ADD 	AX, WORD [PT1 + 0x8] 
	XCHG 	SI, AX  ; pega o First_Sector_Of_Cluster e põe em si

	XOR 	EDX, EDX
    	MOVZX 	ECX, WORD [DI + 11] 	; BPB_BytsPerSec
	MOV 	EAX, DWORD[BX + 28]
	DIV 	ECX
    	CMP 	EDX, 0
    	JE 	.pula
    	ADD 	EAX, 1 ; Isto para evitar não ler todo arquivo

.pula:
	
	XCHG 	CX, AX
	MOV 	EAX, ESI
	MOV 	BX, 0x8000
	XOR 	DI, DI
	CALL ReadSectors


; 8º Executa o stage2.bin
	MOV	EBX,[UID]
	XOR	DX, DX
	MOV 	DL, BYTE[BS_DrvNum]

	PUSH	EAX
	PUSH 	DX
	JMP 	0x0000:0x8000

	

; Erros
ERROR:
	POPA		; desempilha todos registradores de uso geral
    	MOV 	SI, String
    	JMP 	ERROR_START
ERROR1:	
	MOV	SI, String
ERROR_START:
    	CALL	Puts
	XOR	AX, AX
	INT 	0x16
	INT 	0x19	; reboot


Puts:
	PUSHA
.next:
	CLD 			; flag de direcção
	LODSB			; a cada loop carrega si p --> al, actualizando si
	CMP 	AL, 0		; compara al com o 0
	JE  	.end		; se al for 0 pula para o final do programa	
	MOV 	AH, 0x0E	; função TTY da BIOS imprime caracter na tela
	INT 	0x10		; interrupção de vídeo
	jmp 	.next
.end:
	POPA
	RET


ReadSectors:
	; CX --> count
	; DI --> Segmento
	; BX --> Offset
	; EAX --> Starting sector LBA47
	
	MOV	SI, DAP
.loop:
	PUSHA
	MOV 	[SI + 0], BYTE 0x10	; Tamanho da DAP
	MOV 	[SI + 1], BYTE 0	; reservado
	MOV 	[SI + 2], WORD 1	; Sector count
	MOV 	[SI + 4], BX		; Offset
	MOV 	[SI + 6], DI		; Segment
	MOV 	[SI + 8], EAX		; Start sector LBA47
	MOV 	[SI +12], DWORD 0	; Starting sector LBA47
	MOV 	DL, BYTE [BS_DrvNum]
	MOV 	AH, 0x42
	INT 	0x13
	JC 	ERROR	
	POPA
	
	DEC 	CX
	JCXZ 	.end	
	ADD 	BX, 0x200
	CMP 	BX, 0x0000	; 64KB
	JNE 	._1
	ADD 	DI, 0x1000	; 4 KB
._1:
	INC 	EAX
	JMP 	.loop
.end:
	
	RET

; Dados	
BS_DrvNum 		DB 0
RootDirSectors 		DD 0
FirstDataSector 	DD 0
FirstRootDirSector 	DD 0

String 	DB "BOOT ERROR",0


 
; OBS: Vamos considerar em apenas a primeira partição
; Esta deve ser FAT12 ou 16

TIMES (0x1B8 - ($-$$)) NOP

UID TIMES 0x04 	DB 0 		; Unique Disk ID
RSV TIMES 0x02	DB 0		; Optional, reserved 0x0000
PT1 TIMES 0x10 	DB 0    	; First Partition Entry
PT2 TIMES 0x10 	DB 0            ; Second Partition Entry
PT3 TIMES 0x10 	DB 0            ; Third Partition Entry
PT4 TIMES 0x10 	DB 0            ; Fourth Partition Entry

TIMES (510 - ($-$$)) NOP
DW 0xAA55
