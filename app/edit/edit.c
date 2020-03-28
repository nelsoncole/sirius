/*
 * File Name: edit.c
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
#include <string.h>
#include <stdlib.h>

FILE *fd;

int main(int argc, char **argv)
{

	GW_HAND *window = CreateWindow(TEXT("# Edit"),0,200,200,400,200,
	GW_STYLE(FORE_GROUND(GW_WHITE) | BACK_GROUND(GW_WHITE) | BACK_GROUND_STYLE(GW_DARKGRAY)),GW_FLAG_VISIBLE);


	GW_HAND *box = CreateObject(window,TEXT("GW_HANDLE_BOX"),GW_HANDLE_BOX,0,0,window->Area.Width,
	window->Area.Height,GW_STYLE(FORE_GROUND(GW_BLACK) | BACK_GROUND(GW_WHITE)),GW_FLAG_INVISIBLE);


	if ( argc != 3) {

		//msgbox(error)
		//exit(1);
		for(;;)WindowFocus(window);

	}


	fd = fopen(argv[2],"r+");

	if ( fd == NULL) {

		//msgbox(error)
		//exit(1);
		for(;;)WindowFocus(window);


	}



	char **buf = (char**) malloc (0x1000); //4 KiB
	long pool  = (long) malloc (0x10000); //64 KiB

	memset((char*)pool,0,0x10000);
	memset(buf,0,0x1000);

	buf[0] = (char*) pool;


	char *line = buf[0];
	fgets(line,0x1000,fd);


	// enviar sms para box
	Send(box,GW_FLAG_VISIBLE,GW_SMG_FLAG_BIT); 

	// loop
	while(TRUE) 
	{

		Send(box,(unsigned int)(buf),GW_SMG_NORMAL_BIT);
		WindowFocus(window);

		
	}

	//exit();
	return 0;
}
