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
#include <string.h>
#include <stdlib.h>


int main()
{
	
	char **buf = (char**) malloc (0x1000); //4 KiB
	long pool  = (long) malloc (0x10000); //64 KiB
	char *line;
	long scrolldown = 0;
	int count = 0;
	long length = 0;
	int i = 0;
	int write = 0;
	int ch;
	int num = 0;
	long lines = 0;

	memset((char*)pool,' ',0x10000);
	memset(buf,0,0x1000);



	GW_HAND *window = CreateWindow("# Terminal",0,400,200,600,500,/*0,100,20,900,650,*/
	GW_STYLE(FORE_GROUND(GW_WHITE) | BACK_GROUND(GW_BLACK) | BACK_GROUND_STYLE(GW_DARKGRAY)),GW_FLAG_VISIBLE);

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
	line = buf[num++] = (char*) pool;

	// loop
	while(TRUE) {


		// calculando o tamanho da lina de tela em caracteres
		length = box->Font.SizeX;

		ch = fgetc(stdout);

		if(write >= 0x10000 || num >= 1024)for(;;) { 
		// Panic FIXME Nelson quando atingir o limite do buffer 64 KiB ou 1024 Linhas, chamar clear e reiniciar o stdout

		}

		if(ch != EOF) {

			
			if(ch == '\b') { 
				*--line = '\0';
				count--;
				write--;

			}else if ( (ch == '\n') || (count == length) ) {
				*line = '\0';
				count = 0;
				write++;
				line = buf[num++] = (char*) pool + write;
				if(++lines >= box->Font.SizeY ) scrolldown++;
				
			
			}else if(ch == '\t') {
				for(i =0;i < 8;i++) {
					if(count == length) {
						*line++ = '\0';
						count = 0;
						if(write >= 0x10000 || num >= 1024)for(;;) { 
						/* Panic FIXME Nelson quando atingir o limite 
						do buffer 64 KiB ou 1024 Linhas, chamar clear e reiniciar o stdout */

						}
						line = buf[num++] = (char*) pool + write;
						if(++lines >= box->Font.SizeY ) scrolldown++;
						*line++ = ' ';
						count++;
						write++;

					}else {
						*line++ = ' ';
						count++;
						write++;
					}

				}

			}else {
				if(count == length) {
					*line++ = '\0';
					count = 0;
					line = buf[num++] = (char*) pool + write;
					if(++lines >= box->Font.SizeY ) scrolldown++;
					*line++ = ch;
					count++;
					write++;

				}else {
					*line++ = ch;
					count++;
					write++;
				}

			}

		}


		Send(box,(unsigned int)(buf + scrolldown),GW_SMG_NORMAL_BIT);
		WindowFocus(window);

		
	}

	//exit();
	return 0;
}
