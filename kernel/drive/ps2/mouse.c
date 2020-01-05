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



// TODO: Em minha configuração do mouse, levo apeito o sinal das variáveis...
signed char buffer_mouse[3];
UINT8     buttons;
signed int  mouse_x;
signed int  mouse_y;

GW_HAND *MouseHand;

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
    	wait_ns(200);

}


// Esta função será usada para ler dados do mouse na porta 0x60, fora do IRQ12
UINT8 mouse_read(){

    
	kbdc_wait(0);
	UINT8 val = inportb(0x60);
    	wait_ns(200);
    	return val;

}


// Esta rotina faz o Auto-teste 0xaa êxito, 0xfc erro
static UINTN MOUSE_BAT_TEST(){
    
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
UINTN mouse_install(){

    	// reseta o mouse
	mouse_write(KBDC_RESET);
    	//Espera o dados descer (ACK)
    	while( mouse_read() != KBDC_ACK);


    	// Basic Assurance Test (BAT)
    	if(MOUSE_BAT_TEST() != 0) {

    		// Aqui! Precisaremos de fazer alguma coisa, em casos de erro
    		print("\n Mouse error!");

    	}

    	// Use mouse default
	mouse_write(MOUSE_DEFAULT);
   	//Espera o dados descer (ACK)
    	while( mouse_read() != KBDC_ACK);


    	// habilita o mouse.
	mouse_write(ENABLE_MOUSE);
    	//Espera o dados descer (ACK)
	while( mouse_read() != KBDC_ACK);


    	// espera nossa controladora terminar
    	kbdc_wait(1);

    	// habilita o IRQ12
	//irq_enable(12);


	largura_da_tela = G->HorizontalResolution;
	altura_da_tela	= G->VerticalResolution;

	return 0;

}


// Este é o manipulador do IRQ12
VOID mouse_handler(VOID) {

static int first_time = 1;
static int count_mouse=0;


    	// DICA DO TIO FRED
    	// Ao que parce ignorar a leitura do primeiro byte
    	// coloca nossos dados na ordem status, x, y
    	if (first_time != 0) {
        	first_time = 0;
        	//inportb(0x60);
    	}else {

		buffer_mouse[count_mouse++]=inportb(0x60);
	
    		if(count_mouse==3) {
    			buttons = buffer_mouse[0];	
			mouse_refresh();
			count_mouse=0;
	    	}

    	}		

}

// Actualizador do ponto de acomulação do mouse
static VOID mouse_refresh(){

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

     	// Em nossa configuração, não precisamos nos preocupar com o SIGNAL, 
     	// ele é tratado nos cálculos, tendo enconta o sinal da variável (signed)
     	// O nosso mouse_y crescerá para baixo...
     	mouse_x =(buttons &0x40)? mouse_y  : mouse_x + buffer_mouse[1];
     	mouse_y =(buttons &0x80)? mouse_y  : mouse_y - buffer_mouse[2];

     	if(mouse_x < 0) mouse_x =0;
     	else if(mouse_x >largura_da_tela) mouse_x = largura_da_tela;
     	if(mouse_y < 0) mouse_y =0;
     	else if(mouse_y >altura_da_tela ) mouse_y = altura_da_tela;

     	mouse_position = mouse_x + (largura_da_tela  * mouse_y);



	MouseHand->X 		= mouse_x;// coordenada X
	MouseHand->Y 		= mouse_y; // Coordenada Y

    	/*ClearScreen();	
     	print("((%d)   (%d,%d,%d)) ",mouse_position, mouse_x, buttons ,mouse_y );*/
    

}
