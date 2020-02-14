/*
 * File Name: ps2.c
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


// Esta função é usada para a calibragem do IBF e OBF
// Se a entra é 0 = IBF, se entrada é 1 = OBF
VOID kbdc_wait(UINT8 type)
{
	INTN spin = 10000000; 

	if(type == 0) {
		while(!(inportb(0x64)&1)) { if(!spin)break; spin--; }
       	}

	else if(type == 1) {

                 while((inportb(0x64)&2)) { if(!spin)break; spin--; }

      	}else wait_ns(400);

}


// Aqui! Faremos a configuração do PS/2 habilitando o auxiliary device (mouse)
UINTN ps2_install()
{
	
	UINT8 tmp;


    	//Desativar os dispositivos PS/2, 
    	//isto envita que os dispositivos PS/2 
    	//envie dados no momento da configuração.

    	//Desativar a primeira porta PS/2
  	kbdc_wait(1);
	outportb(0x64,0xAD);
    
    	//Desativar a segunda porta PS/2  
	kbdc_wait(1);
	outportb(0x64,0xA7);

  
    	//Defina a leitura do byte actual de configuração do controlador PS/2
	kbdc_wait(1);    
	outportb(0x64,0x20);

	/*
    	* 0 	First PS/2 port interrupt (1 = enabled, 0 = disabled)
	* 1 	Second PS/2 port interrupt (1 = enabled, 0 = disabled, only if 2 PS/2 ports supported)
	* 2 	System Flag (1 = system passed POST, 0 = your OS shouldn't be running)
	* 3 	Should be zero
	* 4 	First PS/2 port clock (1 = disabled, 0 = enabled)
	* 5 	Second PS/2 port clock (1 = disabled, 0 = enabled, only if 2 PS/2 ports supported)
	* 6 	First PS/2 port translation (1 = enabled, 0 = disabled)
	* 7 	Must be zero 
	*/
	kbdc_wait(0);
	tmp=inportb(0x60)|0x3; 

	// defina, a escrita  de byte de configuração do controlador PS/2
	kbdc_wait(1);
	outportb(0x64,0x60);

    	// devolvemos o byte de configuração modificado
	kbdc_wait(1);
	outportb(0x60,tmp);  
	


    	//Agora temos dois dispositivos sereais teclado e mouse (PS/2).
	

    	//Activar a primeira porta PS/2
	kbdc_wait(1);
	outportb(0x64,0xAE);  

    	// activar a segunda porta PS/2
	kbdc_wait(1);
	outportb(0x64,0xA8);

     	// espera   
	kbdc_wait(1);
	
	return 0;

}
