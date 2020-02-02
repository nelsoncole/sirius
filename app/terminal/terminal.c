/*
 * File Name: terminal.c
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
 
#include <io.h>
#include <stdio.h>


INTN main()
{


	int offset = 0;
	int sizeout = 0;


	GW_HAND *window = CreateWindow("# Terminal",0,100,100,700,500,
	GW_STYLE(FORE_GROUND(GW_WHITE) | BACK_GROUND(GW_BLACK) | BACK_GROUND_STYLE(GW_GRAY)),GW_FLAG_VISIBLE);

	GW_HAND *box = CreateObject(window,TEXT("GW_HANDLE_BOX"),GW_HANDLE_BOX,2,2,window->Area.Width -2,
	window->Area.Height - 2,GW_STYLE(FORE_GROUND(GW_WHITE) | BACK_GROUND(GW_BLACK)),GW_FLAG_INVISIBLE);


	// execute shell, criando um processo filho

	int rc = 0;
	char filename[] = "shell.sys";
	__asm__ __volatile__("int $0x72":"=a"(rc):"a"(7),"d"(filename),"c"(0x1));

	if(!rc) {

		//exit();

		for(;;);
	}
	

	// enviar sms para box
	Send(box,GW_FLAG_VISIBLE,GW_SMG_FLAG_BIT); 

	// loop
	while(TRUE) {


		// calculando o tamanho da tela em caracteres
		sizeout = (box->Font.SizeX * box->Font.SizeY);

		offset = ( (stdout->header.offset/sizeout) * sizeout );	

		Send(box,(unsigned int)(stdout->header.buffer + offset),GW_SMG_NORMAL_BIT);

		WindowFocus(window);

		
	}

	//exit();
	return 0;
}
