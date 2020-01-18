/*
 * File Name: keyboard.c
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

// Algumas macros define para o keyboard
#define ESC 0x1B
#define	TAB '\t'
#define ENTER '\n'
#define	SPACE ' '
#define BACKSPACE '\b'

#define	KEY_F1 		(0x3B)
#define KEY_F2 		(KEY_F1 +1)
#define KEY_F3 		(KEY_F2 +1)
#define KEY_F4 		(KEY_F3 +1)
#define KEY_F5 		(KEY_F4 +1)
#define KEY_F6 		(KEY_F5 +1)
#define KEY_F7 		(KEY_F6 +1)
#define KEY_F8 		(KEY_F7 +1)
#define KEY_F9 		(KEY_F8 +1)
#define KEY_F10		(KEY_F9 +1)
#define KEY_F11		(0x57)
#define KEY_F12 	(KEY_F11 +1)

#define NUMELOCK 0
#define SCROLLOCK 0
/*
#define DEL 0x91
#define HOME 0x92
#define END 0x93
#define PGUP 0x94
#define PGDN 0x95
#define UP 0x96
#define LEFT 0x97
#define DOWN 0x98
#define RIGHT 0x99
*/

#define ALT	0
#define CTRL	0
#define LSHIFT	0
#define RSHIFT	0
#define PRTSCR	0

#define CAPSLOCK 0


UINT8 keyboard_charset = 0;

// Algumas variáveis estática, para o controle 
static UINTN shift = 0;
static UINTN caps_lock = 0;

UINT8 key_buffer[2];
UINTN count = 0;



// Este é o nosso mapa de caracters minúsculas
UINT8 ascii_minusculas[256] = {
	0/*NULL*/,ESC,'1','2','3','4','5','6','7','8','9','0','-','=',BACKSPACE,
	TAB,'q','w','e','r','t','y','u','i','o','p','[',']',ENTER,
	0,'a','s','d','f','g','h','j','k','l',';','\'','`',
	0,'\\','z','x','c','v','b','n','m',',','.','/',0,0,0,
	SPACE,
};


// Este é o nosso mapa de caracters maiúsculas
UINT8 ascii_maiusculas[256] = {
	0/*NULL*/,ESC,'!','@','#','$','%','^','&','*','(',')','_','+',BACKSPACE,
	TAB,'Q','W','E','R','T','Y','U','I','O','P','{','}',ENTER,
	0,'A','S','D','F','G','H','J','K','L',':','"','~',
	0,'|','Z','X','C','V','B','N','M','<','>','?',0,0,0,
	SPACE,
};




// Esta função será usada para ler dados do teclado na porta 0x60, fora do IRQ1
UINT8 keyboard_read()
{
	kbdc_wait(0);
	
	UINT8 rc = inportb(0x60);
    	wait_ns(200);

    	return (rc);

}


// Esta função será usada para escrever dados do teclado na porta 0x60, fora do IRQ1
VOID keyboard_write(UINT8 write){

	kbdc_wait(1);

	outportb(0x60,write);
    	wait_ns(200);

}


// Esta rotina faz o Auto-teste 0xaa êxito, 0xfc erro
static UINTN KEYBOARD_BAT_TEST(){
    
    	UINT8 val;

    	while(TRUE) {

        val = keyboard_read();

        if(val == KBDC_OK)return 0;
        else if(val == KBDC_ERROR) {
        
            return -1; 
        }
    
        // Reenviar o comando. 
        // OBS: este comando não é colocado em buffer
        keyboard_write(KBDC_RESEND);       
        

        }

}


UINTN keyboard_install(){

    	//Reseta o teclado
	kbdc_wait(1);
	outportb(0x60,KBDC_RESET);
	wait_ns(200);
	
    	// Espera os dados descer, ACK
    	while(keyboard_read() != KBDC_ACK);
    
    	// Basic Assurance Test (BAT)
    	if(KEYBOARD_BAT_TEST() != 0) {

    	// Nelson aqui precisaremos de criar uma rotina de tratamento de erro do teclado

        print("\nkeyboard error!");

    	}  

    	// espera nossa controladora termina
	kbdc_wait(1);


    	// Habilita IRQ1
	//irq_enable(1);

	
	return 0;
        

}


// Este é o nosso manipulador do mouse no vetor IDT
// TODO: 
// O Bit 7 (0x80) na scancode lido, nos informa se a tecla foi precionada ou solta  		
// Exemplo: O shift left quando precionado gera o scancode 0x2A quando solto 0xAA (0x2A + 0x80 = 0xAA)
VOID keyboard_handler(VOID){

	static UINT32 oldcr3 = 0;
	
	if(count >= 2)count = 0;

    	UINT8 scancode = inportb(0x60);

	key_buffer[count++] = scancode;
	

	// Control kernel
	if(scancode == KEY_F1) {

		key_msg_focos = 1;

		return;
	}

	// Control kernel
	if(scancode == KEY_F2 && key_msg_exec_console == 0) {

		key_msg_exec_console = 1;

		return;
	}

	
    	if(scancode & 0x80) {

        	if((scancode == 0xAA) || (scancode == 0xB6))
        	shift = 0;

    	} else {
            	if((scancode == 0x2A) || (scancode == 0x36)) {
            		shift = 1;
            		return;
            	}



    	}


	
    	if(scancode &0x80 || (scancode == 0x45)) {
    
        	

    	} else {

		
		
         	if( (shift == 1 ) || (caps_lock == 1)) {
                	keyboard_charset = ascii_maiusculas[scancode];


        	} else {

                	keyboard_charset = ascii_minusculas[scancode];


			
            	}

			

			if(!(focus->pid))return;
			if(focus->pid != current_thread->pid) {
		
			oldcr3 = switch_cr3(focus->pd);

			key[0] = keyboard_charset &0xff;

			key[1] = key_buffer[0] &0xff;
			key[1] |= (key_buffer[1] << 8) &0xff00;

			switch_cr3(oldcr3);

			} else { key[0] = keyboard_charset &0xff; }
			



        }	

 
}


