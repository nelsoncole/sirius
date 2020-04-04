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

#include <sys/sys.h>


#define MSG_CLEARSCREEN 0x1
#define MSG_SETCURSOR 0x2
#define MSG_EXIT 0x80


void clearscreen(char **buf,char *line,long pool,int *write,
int *count,int *num,long *scrolldown,long *lines) 
{

	memset((char*)pool,0,0x10000);
	memset(buf,0,0x1000);
	*scrolldown = 0;
	*count = 0;
	*write = 0;
	*num = 0;
	*lines = 0;

	rewind (stdout);

}


int main()
{


	unsigned int *p = (unsigned int*)0x10001120;
	MOUSE *mouse = (MOUSE*)(*p);
	
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

	memset((char*)pool,0,0x10000);
	memset(buf,0,0x1000);

	int flg = 0;
	unsigned int pid = 0;



	GW_HAND *window = CreateWindow("# Terminal",0,mouse->x,mouse->y,800,600,/*0,100,20,900,650,*/
	GW_STYLE(FORE_GROUND(GW_DARKGRAY) | BACK_GROUND(GW_DARKGRAY) | BACK_GROUND_STYLE(GW_WHITE)),GW_FLAG_VISIBLE);

	GW_HAND *box = CreateObject(window,TEXT("GW_HANDLE_BOX"),GW_HANDLE_BOX,4,2,window->Area.Width -4,
	window->Area.Height - 2,GW_STYLE(FORE_GROUND(GW_LIGHTGREEN) | BACK_GROUND(GW_DARKGRAY)),GW_FLAG_INVISIBLE);


	

	// execute shell, criando um processo filho
	int rc = 0;
	char filename[] = "shell.sys";
	//__asm__ __volatile__("int $0x72":"=a"(rc):"a"(7),"d"(filename),"c"(0x1));

	FILE *fp = fopen(filename,"r");
	if (fp != NULL) { __asm__ __volatile__("int $0x72":"=a"(rc):"a"(6),"d"(0),"c"(0),"b"(0),"D"(fp)); fclose(fp);}

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

		if(write >= 0x10000 || num >= 1024)
		{ 
			//FIXME Limite do buffer 64 KiB ou 1024 Linhas, 
			// chamar clear e reiniciar o stdout

			clearscreen(buf,line,pool,&write,&count,&num,&scrolldown,&lines);
			line = buf[num++] = (char*) pool;

		}
		if((ch != EOF) && (ch != 0)) {

			
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
						if(write >= 0x10000 || num >= 1024) 
						{ 
							//FIXME Limite do buffer 64 KiB ou 1024 Linhas, 
							// chamar clear e reiniciar o stdout

							clearscreen(buf,line,pool,&write,&count,&num,&scrolldown,&lines);
							line = buf[num++] = (char*) pool;

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

		} else {


			


			// FIXME tratamento da comunicacao entre processos
			STDX *pipe = read_stx(stdx);

			if(pipe != NULL)
			{
				switch(pipe->id) 
				{
					case MSG_CLEARSCREEN:

					clearscreen(buf,line,pool,&write,&count,&num,&scrolldown,&lines);
					line = buf[num++] = (char*) pool;

					pipe->id  = 0; // return
					flg = 1;

						break;

					case MSG_SETCURSOR:
					length = box->Font.SizeX;

					if(pipe->data1 >= length || pipe->data1 == 0) 
					{ 
						pipe->id  = -1; // return
						flg = 1;
						break;
					}


					line = buf[num - 1]; //inicio na linha
					for(i = 0;i < pipe->data1;i++) 
					{
						if(!(*line))
						{
							*line++ = ' ';
							count++;
							write++;
						} else line++;	

					}

					pipe->id  = 0; // return
					flg = 1;

						break;
					case MSG_EXIT:
					gui_exit(window);
					exit(0);
					flg = 1;
						break;

					default:
						flg = 0;

						break;

					}
		
					if(flg) 
					{
						pid = pipe->pid;
						pipe->pid = 0xFFFFFFFF; // lock
						flg = 0;
						// desbloquear processo
						// task switch
						unlockthread(pid);
						taskswitch_pid(pid);
					}
			

			}


			//////////////////////////////////////////////////
		


		}


		Send(box,(unsigned int)(buf + scrolldown),GW_SMG_NORMAL_BIT);
		WindowFocus(window);

		
	}

	//exit(0);
	return 0;
}
