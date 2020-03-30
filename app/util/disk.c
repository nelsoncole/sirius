/*
 * File Name: disk.c
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
 

/* 	Esta ferramenta tem o foco de listar na tela, discos e partições 
	montadas no Sirius, apagar, criar e formatar partições de disco */


#include <sys/sys.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define STR_MENU_SIZE 4
#define STR_TYPE_SYZE 4

#define SPACE 12

const char *menu[STR_MENU_SIZE]={
    	"Name",
    	"Size",
    	"Type",
	"MountPoint",
};

const char *type[STR_TYPE_SYZE]={
    	"disk",
    	"part",
    	"rom",
	"null",
};

SD *sdx = NULL;


static int read_list(SD *src);

int main(int argc, char **argv)
{
	int i;
	SD *root = (SD*) malloc(0x10000);
	SD *p_src = root;

	unsigned size = 0;
	int _type = 3;

	unsigned int str[2];
	unsigned int str2[2];

	memset(root,0,0x10000);
	memset(str,0,8);
	memset(str2,0,8);

	fputs("DISK (Sirius) version 0.0.1\n",stdout);


	read_list(root);


	// list disk
	for(i=0;i<STR_MENU_SIZE;i++)
	{
		set_cursor_x(SPACE*i);
		fputs(menu[i],stdout);

	}

	for(i=0; i < 2048;i++) {

		if(!p_src->id[0]) break;

		memcpy(str,p_src->id,4);

		if((p_src->id[2] != '\0') && (p_src->id[3] == '\0'))
		{
			_type = 0;

			fputc('\n',stdout);
			printf("%s",str);

			memcpy(str2,str,4);

		} else {

			_type = 1;

			fputc('\n',stdout);
			printf("|--%s",str);

		}



		size = (p_src->num_sectors*p_src->byte_of_sector/1024/1024);
		set_cursor_x(SPACE);
		printf("%d MiB",size);

		set_cursor_x(SPACE*2);
		printf("%s",type[_type]);

		set_cursor_x(SPACE*3);
		if((p_src->id[2] != '\0') && (p_src->id[3] == '\0')) putchar('/');
		else printf("/%s/",str2);

		p_src++;

	}


	fputs("\n____________________________________________",stdout);


	free(root);	

	return 0;
}


static int read_list(SD *src)
{
	SD *p_src = src;

	SD *bf = (SD*) (stdsd->header.buffer);
	SD *bf2 = NULL;

	int i,t;
	for(i=0;i<64;i++)
	{
		bf2 = bf = (SD*) (stdsd->header.buffer + (i*1024));

		if(bf2->partnum == 12345)
		{
			
			memcpy(p_src++,bf2++,sizeof(SD));
			
			for(t=0;t<32 -1;t++)
			{

				if((bf2->num_sectors != 0) ) 
				{
					memcpy(p_src++,bf2,sizeof(SD));
				}

				bf2++;
			}



		}


	}

	return 0;

}
