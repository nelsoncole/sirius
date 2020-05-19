/*
 * File Name: trm.c
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gx.h>
#include <sys/sys.h>

#define MSG_CLEARSCREEN 0x1
#define MSG_SETCURSOR 0x2
#define MSG_EXIT 0x80

FILE *f = 0;

void local_message(gx_hand_t *w, gx_message_t *m) 
{
	switch (m->type) {
		case GX_EXIT:
			exit_window(w);
			exit(0);
		break;

	}


}
void message(gx_hand_t *w,gx_hand_t *box,char *buf, unsigned int *ln, int *scroll, int *cursor) {
	int flg = 0;
	unsigned int pid = 0;
	STDX *pipe = read_stx(stdx);

	if(pipe != NULL) {
		switch(pipe->id) {
		 	case MSG_CLEARSCREEN:
				// clear screen
				*cursor = 0;
				*scroll = 0;
				memset(buf,0,0x10000);
				clearscreen_box(box);

				pipe->id  = 0; // return
				flg = 1;

			break;
			case MSG_SETCURSOR:
				/*length = box->Font.SizeX;

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

				}*/

				pipe->id  = 0; // return
				flg = 1;

			break;
			case MSG_EXIT:
				exit_window(w);
				exit(0);
				flg = 1;
			break;

			default:
				flg = 0;

			break;

		}
		
		if(flg) {
			pid = pipe->pid;
			pipe->pid = 0xFFFFFFFF; // lock
			flg = 0;
			// desbloquear processo
			// task switch
			unlockthread(pid);
			taskswitch_pid(pid);
		}
	}
}

void mmv(char *s1, char *s2) 
{
	unsigned char *us1 = (unsigned char *) s1;
	unsigned char *us2 = (unsigned char *) s2;

	while (*us2) *us1++ = *us2++;

}

void disciplina(char *buf, int c, int *cursor) {

	unsigned char *ubuf = (unsigned char *)buf + *cursor;
	int i;
	if(c == '\b') {
		if(*cursor > 0) {
			 *--ubuf = '\0';
			 *cursor -= 1;
		}
	} else if (c == '\t'){

		for(i=0;i<8;i++) {
			*ubuf++ = ' ';
			*cursor +=1;
		}
			
	} else { 
		*ubuf++ = c;
		*cursor += 1;
	}

}

int cpy(char **box, char *buf, unsigned int *ln, int w, int h) {

	int j,i,c;
	unsigned char *line;
	unsigned char *ubuf = (unsigned char *)buf; 

	for(i=0;i < h; i++) {
		line = (unsigned char *) box[i]; 
		for(j=0;j < w; j++) {
			
			c = *ubuf++;
			
			if(c == '\n') { 
				j++;
				break;
			}
			*line++ = c;

			if(!c) { 
				ln[i] = j;
				return (0);	
			}
		}

		ln[i] = j;
		*line = '\0';
	}

	if(i >=h) return (ln[0]);

	return (0);

}

int main(int argc, char **argv) {

	gx_message_t m;

	int x = getmouse_x();
	int y = getmouse_y();

	gx_hand_t *w = create_window (x/*100*/,y/* 100*/, 560, 400, 0);
	create_title(w, "Terminal Virtual", 1, 0);
	gx_hand_t *b = create_box(w,2, 32, w->w - 4, w->h - 34, 0);

	gx_mouse_init();


	char *buf = (char *) malloc(0x10000); //64 KiB
	unsigned int *ln = (unsigned int *) calloc(sizeof(unsigned int),1024);
	memset(buf,0,0x10000);
	memset(ln,0,sizeof(unsigned int)*1024);

	char **txt = (char **) get_buffer_box(b); // buffer


	// execute shell, criando um processo filho
	int rc = 0;
	char filename[] = "shell.bin";

	f = fopen(filename,"r");
	if (f != NULL) { 
		__asm__ __volatile__("int $0x72":"=a"(rc):"a"(6),"d"(0),"c"(0),"b"(0),"D"(f)); 
		fclose(f);
	}

	if(!rc) {
		printf("error: shell.sys not found\n");
	}

	int c, cursor = 0, scroll = 0;
	for(;;) {	

		m = gx_mouse (w);
		local_message(w,&m);
		

		c = fgetc(stdout);
		if( c == EOF || c == 0){
			
			// leitura de mensagem
			message(w,b,buf,ln, &scroll, &cursor);
				 continue;
		}

		if(cursor >= 65536) {
			// clear screen
			cursor = 0;
			scroll = 0;
			memset(buf,0,0x10000);
			clearscreen_box(b);
		}

		disciplina(buf , c, &cursor);
		scroll += cpy(txt, buf + scroll, ln, b->w/b->font.cols, b->h/b->font.rows);
	}
	return 0;

}
