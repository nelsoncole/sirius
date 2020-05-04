/*
 * File Name: mouse.c
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

#define SINAL_N -
#define SINAL_P +



CONST CHAR8 *id_mouse_strings[]={
    "Default",
    "Unknown",
    "Unknown",    
    "IntelliMouse"
};



unsigned char 	buttons;
int  		mouse_x;
int  		mouse_y;
int 		count_mouse;
int 		first_time;


// Algumas variáveis de controle

static UINTN largura_da_tela = 1024;
static UINTN altura_da_tela  = 768;
UINTN speed = 1;
UINTN mouse_position = 0;

static VOID mouse_refresh();


// Esta função será usada para escrever dados do mouse na porta 0x60, fora do IRQ12
VOID mouse_write(UINT8 write){

	kbdc_wait(1);
	outportb(0x64,0xD4);
	kbdc_wait(1);
	outportb(0x60,write);

}


// Esta função será usada para ler dados do mouse na porta 0x60, fora do IRQ12
UINT8 mouse_read(){

    
	kbdc_wait(0);
	return inportb(0x60);
}


// Esta rotina faz o Auto-teste 0xaa êxito, 0xfc erro
UINTN MOUSE_BAT_TEST(){
    
    	UINT8 val;

    	while(TRUE){

        	val = mouse_read();

        	if(val == KBDC_OK)return 0;
        	else if(val == KBDC_ERROR) {
       			return -1; 
       		}
    
        	// Reenviar o comando. 
        	// OBS: este comando não é colocado em buffer
        	mouse_write(KBDC_RESEND);       
        

        }

}



// Instalação do mouse
void mouse_install(){
	unsigned char tmp;

	largura_da_tela = G->HorizontalResolution;
	altura_da_tela	= G->VerticalResolution;

	mouse_x = 0;
	mouse_y = 0;
	buttons = 0;
	count_mouse= 0;
	first_time = 1;

	
	//Defina a leitura do byte actual de configuração do controlador PS/2
	kbdc_wait(1);
  	outportb(0x64, 0x20);

	// Enable the interrupts
	// Second PS/2
	kbdc_wait(0);
	tmp = inportb(0x60);

	tmp |= 2;

	// defina, a escrita  de byte de configuração do controlador PS/2
	kbdc_wait(1);
	outportb(0x64, 0x60);

	kbdc_wait(1);
  	outportb(0x60, tmp);

	// activar a segunda porta PS/2
	kbdc_wait(1);
	outportb(0x64,0xA8);

	kbdc_wait(1); //espera


	// mouse_write(KBDC_RESET);
	// mouse_read();  // ACK

	// Tell the mouse to use default settings
	mouse_write(MOUSE_DEFAULT);
	mouse_read();  // ACK

	// Enable the mouse
	mouse_write(ENABLE_MOUSE);
	mouse_read();  // ACK

	// IRQ set handler
	// Enable IRQ Interrupt



	//mouse = (MOUSE *) malloc(sizeof(MOUSE));

}


// Este é o manipulador do IRQ12
void mouse_handler(void) {
    	// DICA DO TIO FRED
    	// Ao que parce ignorar a leitura do primeiro byte
    	// coloca nossos dados na ordem status, x, y

	if (first_time != 0) {
        	first_time = 0;
        	inportb(0x60);
		return;
    	}

	static unsigned char status;
	status = inportb(0x64);
    	if ((status & 0x01) && (status & 0x20)) {

		switch(count_mouse++) {
			case 0:
				buttons = inportb(0x60);
			break;
			case 1:
				if(buttons & 0x10) // negative
					mouse_x = (int) inportb(0x60) | 0xFFFFFF00;
				else
					mouse_x = (int) inportb(0x60);
			break;
			case 2:
				if(buttons & 0x20) // negative
					mouse_y = (int) inportb(0x60) | 0xFFFFFF00;
				else
					mouse_y = (int) inportb(0x60);
				
				if( (buttons & 0x80) || (buttons & 0x40) == 0 ) { 
					// x/y overflow ?

					mouse_refresh();	
         
        			}


				count_mouse = 0;

			break;
			default:
				count_mouse = 0;
			break;
		}

    	}		

}

// Actualizador do ponto de acomulação do mouse
static void mouse_refresh(){

/*                                      Mouse default (0xF6)
 *
 *   _____________________________________________________________________________________________
 *  { overflow y | overflow x | signal y | signal x| always 1 | middle btn | right btn | left btn }
 *  {____________|____________|__________|_________|__________|____________|___________|__________}
 *  {                                       Delta X                                               }
 *  {_____________________________________________________________________________________________}
 *  {                                       Delta Y                                               }
 *  {_____________________________________________________________________________________________}
 *
 *
 */    

	mouse->x += mouse_x;// coordenada X
	mouse->y -= mouse_y; // Coordenada Y
	mouse->z += 0;
	mouse->b = buttons;

	
	if(mouse->x < 0) mouse->x = 0;
     	else if(mouse->x > largura_da_tela) mouse->x = largura_da_tela;
     	if(mouse->y < 0) mouse->y = 0;
     	else if(mouse->y >altura_da_tela ) mouse->y = altura_da_tela;

	/*
    	ClearScreen();	
     	print("(%d,%x,%d) ",mouse_x, mouse->b ,mouse_y ); */
    

}


