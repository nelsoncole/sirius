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

#include <gx.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
 
FILE *f;


static int cmd_salve(const char *name,gx_hand_t *edit) 
{
	if(!*name) return -1;
	int rc = 0;
	
	char *buf = (char*) get_buffer_editbox(edit);
	int len = strlen(buf);

	if( len > get_len_editbox(edit) ) return -1;

	f = fopen(name,"w");

	if( f == NULL) return -1;
	
	if( fwrite(buf,1,len,f) != len ) rc = -1;

	fclose(f);

	return rc;
}

static int open(const char *name,gx_hand_t *edit,gx_hand_t *fname) 
{

	if(!*name) return -1;
	
	char *buf = (char*) malloc(get_len_editbox(edit));
	memset(buf,0,get_len_editbox(edit));
	
	f = fopen(name,"r");
	if( f == NULL) {

		free(buf);
		return -1;
	}


	fseek(f,0,SEEK_END);
	int len = ftell(f);
	fseek(f,0,SEEK_SET);

	if( len > get_len_editbox(edit) ) {

		fclose(f);
		free(buf);
		return -1;
	}


	
	
	if( fread(buf,1,len,f) != len ) {

		fclose(f);
		free(buf);
		return -1;
	}


	
	while(*buf && len--)r_motor_editbox(edit, *buf++);
	while(*name)r_motor_editbox(fname, *name++);

	fclose(f);
	free(buf);
	return 0;


}

int main(int argc, char **argv)
{

	gx_message_t m;
	gx_mouse_init();

	gx_hand_t *window = create_window (0,0, 560, 500, 0);
	create_title(window, "Simples Texto Editor", 1, 0);

	/*gx_hand_t *l1 = */create_label(window,"Nome do arquivo",8,32,window->font.cols*16,16,0);

	gx_hand_t *fname = create_editbox(window,8,50,window->font.cols*32,window->font.rows+4,256, 0/*normal*/);
	gx_hand_t *salve = create_button(window, "Salvar",fname->w+16,50, window->font.cols*8, window->font.rows+4, 0);

	gx_hand_t *edit = create_editbox(window,8,74,window->w-16,window->h - 80, 65536, 0);



	if(argc > 1) open(argv[1],edit,fname);

	for(;;) {	

		m = gx_mouse (window);

		switch(m.type) {
			case GX_OK:
				if( cmd_salve(get_buffer_editbox(fname), edit) );

			break;


		}


	}
	return 0;
}
