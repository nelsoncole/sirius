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
#include <sys/sys.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

FILE *fp = NULL;

char *mem = NULL;
char *p_mem;
char *line;
long filesize = 0;

int main(int argc, char **argv)
{

	GW_HAND *window = CreateWindow(TEXT("# Edit"),0,0,0,800,600,
	GW_STYLE(FORE_GROUND(GW_BLACK) | BACK_GROUND(GW_DARKGRAY) | BACK_GROUND_STYLE(GW_WHITE)),GW_FLAG_VISIBLE);

	

	/*GW_HAND *area = CreateObject(window,TEXT(argv[1]),GW_HANDLE_BOX,24,0,window->Area.Width - 24,
	window->Area.Height,GW_STYLE(FORE_GROUND(GW_WHITE) | BACK_GROUND(GW_DARKGRAY)),GW_FLAG_VISIBLE);*/


	/*GW_HAND *box = CreateObject(window,TEXT("GW_HANDLE_BOX"),GW_HANDLE_BOX,24,24,window->Area.Width - 24,
	window->Area.Height - 24,GW_STYLE(FORE_GROUND(GW_WHITE) | BACK_GROUND(GW_DARKGRAY)),GW_FLAG_INVISIBLE);*/


	mem = (char*) malloc (0x10000);
	long pool = (long) malloc (0x10000);
	char **lines = (char **) malloc (0x1000);
	int i,x;

	memset((char*)pool,0,0x10000);
	memset(lines,0,0x1000);
	memset(mem,0,0x10000);


	for ( i=0 ; i < 47; i++ ) {

		lines[i] = (char*) pool + (128*i);
	}

	if (argc > 1) { fp = fopen(argv[1],"r+");
	
	if( fp != NULL) 
	{

		fseek(fp,0,SEEK_END);
		filesize = ftell(fp);
		rewind(fp);

		if(fread (mem,sizeof(char),filesize,fp)) 
		{
			p_mem = mem;

			for(i=0 ; i < 256 ; i++) 
			{
				line = lines[i];

				if(!*p_mem)break;
				for( x=0; x < (64) ; x++ ) { 
					
					if(!*p_mem)break;
					if( *p_mem == '\n' ) 
					{ 
						*line = '\0';
						p_mem++;
						break;
					} else if (*p_mem == '\t') {
						p_mem++;
						*line++ = ' '; x++;
						*line++ = ' '; x++;
						*line++ = ' '; x++;
						*line++ = ' '; x++;
						*line++ = ' '; x++;
						*line++ = ' '; x++;
						*line++ = ' '; x++;							
						*line++ = ' '; x++;

					} else *line++ = *p_mem++;
				}

				

			}

		}


		fclose(fp);

		// enviar sms para box
		//Send(box,GW_FLAG_VISIBLE,GW_SMG_FLAG_BIT);
		//Send(box,(unsigned int)(lines),GW_SMG_NORMAL_BIT);
		

	} }


	
	// loop
	while(TRUE) 
	{

		WindowFocus(window);

		
	}

	//exit();
	return 0;
}
