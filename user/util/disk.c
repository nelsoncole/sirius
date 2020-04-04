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


#define STR_MENU_SIZE 6
#define STR_TYPE_SYZE 4
#define STR_DK_MENU_SIZE 10

#define SPACE 12

const char *menu[STR_MENU_SIZE]={
    	"Name",
	"Flag",
    	"Size",
    	"Type",
	"FileSystem",
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

enum DISK_MENU { 
DK_CLEAN 	= 'c',
DK_DELETE	= 'd', 
DK_FORMAT 	= 'f',
DK_LIST 	= 'l',
DK_MENU		= 'm',
DK_NEW 		= 'n',
DK_CREATE	= 'o',
DK_QUIT		= 'q',
DK_SELECT	= 's',
DK_WRITE	= 'w'};

const char *cmd_menu[STR_DK_MENU_SIZE]={
    	"c	clean disk",
    	"d	delete a partition",
    	"f	format a partition",
	"l	list known disk and partition types",
	"m	print this menu",
	"n	add new partition",
	"o	create a new empty MBR partition table",
	"q	quit or exit programe",
	"s	select device",
	"w	write table to disk",
};

FILE *dev = NULL;
unsigned char *data;

static int cmd_l (SD *root);
static void cmd_m();
static int cmd_s();
static int cmd_n(int flag);
static int cmd_w(int flag);

int main(int argc, char **argv)
{
	SD *root = (SD*) malloc(0x10000);
	char *buf = (char *) malloc(0x1000);
	data = (unsigned char *) malloc(0x1000);
	int cmd;
	int quit = 1;
	int flag = 0;

	memset(buf,0,0x1000);

	dev = NULL;

	fputs("MBR disk (Sirius) version 0.0.1\n",stdout);

	//read_list(root);

	for ( ; ; ) {


		fputs("Command (m for help): ",stdout);

		//*cmd = fgetc(stdin);
		fgets (buf,0x1000,stdin);
	
		cmd = buf[0];


		switch(cmd) {
			case DK_MENU: 	cmd_m();
				break;
			
			case DK_QUIT: 	quit = 0;
					fputs("MBR disk exiting ...",stdout);
				break;
		
			case DK_LIST: 
					memset(root,0,0x10000);
					read_list(root);
					cmd_l (root);
				break;
			case DK_SELECT: fputs("Select device (MountPoint + Name): ",stdout);
					flag = cmd_s();
					
				break;
			case DK_NEW: 	flag = cmd_n(flag);
				break;
			case DK_WRITE:	flag = cmd_w(flag);
				break;

			default:
				break;
		}


		if(!quit) break;
		fputc('\n',stdout);

		memset(buf,0,0x1000);


	}


	free(data);
	free(root);

	if(dev != NULL) fclose(dev);	

	return 0;
}


static int cmd_l (SD *root)  {

	int i;
	SD *p_src = root;

	unsigned size = 0;
	int _type = 3;

	unsigned int str[2];
	unsigned int str2[2];
	memset(str,0,8);
	memset(str2,0,8);


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
		set_cursor_x(SPACE*2);
		printf("%d MiB",size);

		set_cursor_x(SPACE*3);
		printf("%s",type[_type]);

		set_cursor_x(SPACE*5);
		if((p_src->id[2] != '\0') && (p_src->id[3] == '\0')) putchar('/');
		else printf("/%s/",str2);

		p_src++;

	}


	//fputs("\n____________________________________________",stdout);
	fputc('\n',stdout);

	return (0);

}

static void cmd_m()
{
	int i;

	fputs("Command action\n",stdout);

	for(i=0; i < STR_DK_MENU_SIZE; i++) {
		set_cursor_x(4); 
		fputs(cmd_menu[i],stdout);
		fputc('\n',stdout);
	}

}


static int cmd_n(int flag)
{

	flag &=~0x80;

	MBR *tble = (MBR *) data;
	SD *info = (SD*) (stdsd->header.buffer);
	int n;


	if(flag != 1 || dev == NULL) { 
		fputs("\nAdd new partition error",stdout);
		return (flag);
	}


	memset(tble,0,0x1000);
	rewind(dev);
	if (fread(tble,1,512,dev) != 512) {
		fputs("\nAdd new partition error, read sector",stdout);
		return (flag);
		
	}


	for(n=0; n < 2048; n++) {
		if(dev->header.dev == info->devnum) break;
		info++;
	}

	if(n >= 2048) {

		fputs("\nAdd new partition error, unidade nao conectada",stdout);
		return (flag);
	}


	printf("\nUnique Disk ID 0x%x	Disk size ( %d MiB )\n",
	tble->unique_disk_id,(info->num_sectors*info->byte_of_sector)/1024/1024);

	for(n =0; n < 4; n++) {
		set_cursor_x(4);
		printf("Part[%d] size (%d MiB)\n",n+1,
		(tble->part[n].num_sectors*info->byte_of_sector)/1024/1024);
	}

	fputs("Add new partition number (1-4): ",stdout);

	char s[256];
	memset(s,0,256);
	fgets (s,256,stdin);

	n = *s - 0x30 - 1;

	if(tble->part[n].num_sectors != 0) {

		fputs("\nBusy partition",stdout);
		return (flag);
	}

	size_t start = 0;

	if(n == 0) {

		if( (tble->part[n].lba_start <= (tble->part[n+1].lba_start +\
		tble->part[n+2].lba_start +tble->part[n+3].lba_start)) ) {

			start = 8;

		}else { printf("\nerror 1"); return (flag); }

	}else if( (tble->part[0].lba_start >= tble->part[1].lba_start) &&\
	(tble->part[0].lba_start >= tble->part[2].lba_start) &&\
	(tble->part[0].lba_start >= tble->part[3].lba_start)) {

		start = (tble->part[0].lba_start + tble->part[0].num_sectors);

	}else if( (tble->part[1].lba_start >= tble->part[2].lba_start) &&\
	(tble->part[1].lba_start >= tble->part[3].lba_start)) {

		start = (tble->part[1].lba_start + tble->part[1].num_sectors);

	}else if( (tble->part[2].lba_start >= tble->part[3].lba_start) ){

		start = (tble->part[2].lba_start + tble->part[2].num_sectors);

	}else if(n < 4 && n != 0) {

		if( (tble->part[n].lba_start != tble->part[n+1].lba_start) ) {

			start = (tble->part[n-1].lba_start + tble->part[n-1].num_sectors);

		}else { printf("\nerror 2"); return (flag); }

	}else { printf("\nerror 3"); return (flag); }


	tble->part[n].dev_attr = 0x80;
	tble->part[n].lba_start = start;
	tble->part[n].num_sectors = 65536;

	fputs("Size (number in MiB): ",stdout);
	fgets (s,256,stdin);

	printf("New Part[%d] LBA Start %d, Sectors %d",n+1,
	tble->part[n].lba_start,tble->part[n].num_sectors);


	flag |= 0x80;

	return (flag);

}

static int cmd_w(int flag)
{

	MBR *tble = (MBR *) data;

	if((!(flag&0x80)) || (dev == NULL)) {

		printf("write error flag 0x%x",flag);
		return (flag);
	}


	rewind(dev);
	if (fwrite(tble,1,512,dev) != 512) {
		fputs("Error write sector",stdout);
		return (flag);
		
	}

	int devnum = dev->header.dev;

	fclose(dev);
	dev = NULL;

	// enviar mensagem no xserver, para que reconecta a particao

	update_mount_sd(devnum);


	return (0);


}

// return 1, um disco foi seleccionado
// return 2, uma particao foi seleccionada
// return 0, nenhum 
static int cmd_s()
{

	if(dev) {

		printf("Busy\n");
		return (0);

	}

	int rc = 0;
	char id[256] = "/dev\0";
	char s[256];
	memset(s,0,256);

	fgets (s,256,stdin);

	int len = strlen(s) - 1;
	s[len] = '\0'; // clean caracter '\n'

	if(len == 4) rc = 1;
	else if(len == 9) rc = 2;
	else {
		printf("Select error %s\n",s);
		return (0);

	}

	strcat(id,s);

	dev = fopen(id,"r+");
	if(dev != NULL) {

		printf("Device \"%s\" active\n",id);
		return (rc);

	}

	printf("Error: \"%s\" device not found\n",s);
	return (0);
	
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
