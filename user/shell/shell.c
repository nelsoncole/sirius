/*
 * File Name: shell.c
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
#include <string.h>
#include <stdlib.h>
#include <sys/sys.h>

#define MSG_CLEARSCREEN 1
#define MSG_SETCURSOR 2
#define MSG_EXIT 0x80

#define SHELL_CMD_NUM 17 + 3

void (*call_loader)  (int argc,char **argv) = 0;// NULL;

typedef struct _COMMAND {
  char *name;
  void *fun;
  char *help;
}__attribute__((packed)) COMMAND;

int cmd_version(int argc,char **argv);
int cmd_time(int argc,char **argv);
int cmd_shutdown(int argc,char **argv);
int cmd_run(int argc,char **argv);
int cmd_rename(int argc,char **argv);
int cmd_reboot(int argc,char **argv);
int cmd_new(int argc,char **argv);
int cmd_mov(int argc,char **argv);
int cmd_info(int argc,char **argv);
int cmd_help(int argc,char **argv);
int cmd_exit(int argc,char **argv);
int cmd_echo(int argc,char **argv);
int cmd_dir(int argc,char **argv);
int cmd_del(int argc,char **argv);
int cmd_date(int argc,char **argv);
int cmd_copy(int argc,char **argv);
int cmd_cls(int argc,char **argv);
int cmd_cd(int argc,char **argv);



int cmd_salve(int argc,char **argv) {

	char *dp_str = (char*)stdout->header.buffer;

	if(argc != 1) { 

		printf("salve: error, argc %d\n",argc);
		return -1;
	}

	FILE *fp = fopen("stdout.txt","w+");

	if(fp == NULL) {
 
		printf("Errro ao abrir %s\n",argv[1]);

		return 0;
	}

	//fputs (dp_str,fp);

	for(int i=0;i < (65536*2);i++){

		if(i >= stdout->header.size)break;
		if(!*dp_str)break;
		if(fputc(*dp_str++,fp)  == EOF )break;

		//if(fputc('a',fp) == EOF )break;
	}

	//rewind (fp);

	if(fclose(fp)) {

		printf("%s: successfull\nFile name: \"%s\"\n",argv[0],"stdout.txt");

	}else printf("salve: error\n");


	return 0;


}


COMMAND cmd_table[] = {
    	{"?",           cmd_help,           "This help"                                     	},
    	{"cd",          cmd_cd,             "Change current directory"                      	},
    	{"cls",         cmd_cls,            "Clear screen"                                  	},
    	{"copy",        cmd_copy,           "Copy file or directory"                        	},
    	{"date",        cmd_date,           "Date"                                          	},
    	{"del",         cmd_del,            "Delete file or directory"                      	},
    	{"dir",         cmd_dir,            "List directory"                                	},
    	{"echo",        cmd_echo,           "This ---"                                      	},
    	{"exit",        cmd_exit,           "Exit shell"                                    	},
    	{"help",        cmd_help,           "This help"                             		},
	{"info",        cmd_info,           "This System information"   		   	},
    	{"mov",         cmd_mov,            "Move file or directory"                        	},
    	{"new",         cmd_new,            "New file or directory"                         	},
    	{"reboot",      cmd_reboot,         "Reboot system"                                 	},
    	{"rename",      cmd_rename,         "Rename file or directory"                      	},
	{"run",		cmd_run,	    "Execute or run application"			},
    	{"shutdown",    cmd_shutdown,       "Shutdown your computer locally or remotely"    	},
   	{"time",        cmd_time,           "Time"                                          	},
    	{"version",     cmd_version,        "Shell version"                                 	},
	{"salve",	cmd_salve,	    "Salve string no arquivo de nome[x]"		},
};


char **argv_malloc(size_t size) {

	char **argv = (char**) malloc(0x2000);

	int i;
	for(i= 0 ;i < 32;i++) argv[i] =(char*) (((size_t)argv) + (128* i) + 128 );
	
	return (char**) argv;

}

int argc_num(char **argv,const char *src) {

	int i, c = 0;
	const char *p_src = src;
	char *p_dest = (char *) argv[0];
	int fl = 0;
	
	do{

		char *p_dest = (char *) argv[c];

		while(*p_src) { 
			if(*p_src == ' ')p_src++;
			else break;
		}

		if(*p_src) {
			fl = 0;
			for(i=0;i < 128;i++) { 

				if(*p_src == '"' && fl == 0){
					p_src++; //FIXME nao e necessario

					fl = 1;	

				}else if (*p_src == '"' && fl == 1) {
					
					p_src++; //FIXME nao e necessario
					fl = 0;	

				}

				if(*p_src == '\n')p_src++;
				else *p_dest++ = *p_src++;

				if(*p_src == ' ' && fl == 0 ){ 
					*p_dest = '\0';
					break;
				}

			} 
			c++;
		}

	}while(*p_src);

	p_dest = p_dest;

	return c;
}

int main()
{

	int i;
	char *cmd_name = (char*) malloc(0x1000);

	char **argv = argv_malloc(32);
	int argc;
	
	for(;;) {
		memset(cmd_name,0,0x1000);
		//fputs( "__________________\n",stdout);
		printf("[ sirius@nelson ]# ");

		fgets (cmd_name,0x1000,stdin);

		for(i=0;i<32;i++)memset(argv[i],0,128);
		argc = argc_num(argv,cmd_name);
	
		for(i=0;i < SHELL_CMD_NUM;i++) {

            		if((strcmp(argv[0]/*cmd_name*/,cmd_table[i].name)) == 0) {

				call_loader = (void(*)(int,char **))(cmd_table[i].fun);

				call_loader(argc,argv);
                		break;
            		}
            		else if(i == (SHELL_CMD_NUM - 1)) {
            			printf("%s: command not found\n",argv[0]);

			}
			
        	}

	}


	free(cmd_name);
	//exit();
	return 0;
}


int cmd_cd(int argc,char **argv)
{
    	return 0;
}

int cmd_cls(int argc,char **argv)
{

	write_stx(MSG_CLEARSCREEN,0,0,stdx);

    	return 0;
}

int cmd_copy(int argc,char **argv)
{

	if(argc < 2)return 0;

	char *str = (char*)malloc(0x1000);

	FILE *fd = fopen(argv[1],"r");

	if(fd != NULL) {
		fgets(str,0x1000,fd);
		
		puts(str);
		putchar('\n');

		close(fd);

	}

    	return 0;
}

int cmd_date(int argc,char **argv)
{
    	return 0;

}


int cmd_del(int argc,char **argv)
{
	if(argc != 2){ printf("delete file: error\n"); return 1;}

	remove (argv[1]);

    	return 0;
}


int cmd_dir(int argc,char **argv)
{
	FILE *fd = open(".",ATTR_DIRECTORY,"r");

	if(fd != NULL) {
		for(int i=0;i< fd->header.blocks;i++) printf("%d - %s\n",i,(char*)(fd->header.buffer + (64*i)));
		close(fd);

	}

	

    	return  0;
}

int cmd_echo(int argc,char **argv)
{

	if(argc != 3) { 

		printf("echo: error, argc %d\n",argc);
		return -1;
	}

	FILE *fp = fopen(argv[1],"w+");

	if(fp == NULL) {
 
		printf("Errro ao abrir %s\n",argv[1]);

		return 0;
	}

	char *str = argv[2];

	fputs (str,fp);

	//rewind (fp);

	if(fclose(fp)) {

		printf("%s: successfull\nFile name: \"%s\"\n",argv[0],argv[1]);

	}else printf("%s: error\n",argv[0]);


	return 0;
}	

int cmd_exit(int argc,char **argv)
{
	write_stx(MSG_EXIT,0,0,stdx);

    	return 0;
}
int cmd_help(int argc,char **argv)
{
        int i;
       	puts("Commands:\n");
        for(i=0;i< SHELL_CMD_NUM;i++){
        	puts(cmd_table[i].name);
        	//set_cursor
		write_stx(MSG_SETCURSOR,15,0,stdx);

        	puts(cmd_table[i].help);
		putchar('\n');

        }

        return 0;

}
int cmd_info(int argc,char **argv)
{
	puts("System information:\
	\nKERNEL_OF_NUM_PAGE_TABLE          12          // size 48 MiB\
	\nSTART_ALLOC_PAGES_VIRTUAL_ADDRESS 0x01000000  // mark 16 MiB\
	\nEND_ALLOC_PAGES_VIRTUAL_ADDRESS   0x03000000  // mark 48 MiB\
	\nAPP_VIRTUAL_ADDRESS               0x10000000  // mark 256 MiB\
	\nAPP HEADER:\
	\nHEADER_MAGIC      0x00  // size 4 Bytes\
    	\nHEADER_FLAGS      0x04  // size 4 Bytes\
	\nheader            0x08  // size 4 Bytes\
	\n_start            0x0C  // size 4 Bytes\
    	\n_end              0x10  // size 4 Bytes\
	\nstack             0x14  // size 4 Bytes\
	\nAPP INFO:\
	\n0x10000000 - 0x1000EFFF   ELF Header      // size 4 KiB\
	\n0x10001020 - 0x100010FF   Struct GUI      // size 224 Bytes\
	\n0x10001100 - 0x10001103   Key             // size 4 bytes\
	\n0x10001104 - 0x10001107   X               // size 4 bytes\
	\n0x10001108 - 0x1000110B   Y               // size 4 bytes\
	\n0x1000110C - 0x1000110F  *Detail Hardware // size 4 bytesn\
	\n0x10001110 - 0x10001113  *Device          // size 4 bytes\
	\n0x10001114 - 0x10001117   PID             // size 4 bytes\
	\n0x10001118 - 0x1000111B  *FOCUS           // size 4 bytes\
	\n0x1000111C - 0x1000111F  *GwFOCUS         // size 4 bytes\
	\n0x10001120 - 0x10001123  *MOUSE           // size 4 bytes\
	\n0x10001124 - 0x10001127  *RCT             // size 4 bytes\
	\n0x10001128 - 0x100011F3   reserved        // size 208 bytes\
	\n0x100011F4 - 0x100011F7   pwd             // size 4 bytes\
	\n0x100011F8 - 0x100011FB   argc            // size 4 bytes\
	\n0x100011FC - 0x100011FF   **argv          // size 4 bytes\
	\n0x10001200 - 0x100012FF   **argv data     // size 256 bytes\n");

        return 0;

}

int cmd_mov(int argc,char **argv)
{

    	return 0;
}

int cmd_new(int argc,char **argv)
{

	if(argc != 3){ printf("new file: error\n"); return 1;}

	char *str = (char *) argv[1];

	if((strcmp(str,"-a")) == 0) {

		if(creat(argv[2],ATTR_ARCHIVE)) printf("Create new file: \"%s\" error\n",argv[2]); 
	
	} else if((strcmp(str,"-d")) == 0) {

		if(creat(argv[2],ATTR_DIRECTORY)) printf("Create new directory: \"%s\" error\n",argv[2]); 
	
	} else {

		printf("Create new file/directory: error\n");


	}
    	return 0;
}


int cmd_reboot(int argc,char **argv)
{
    	// This syscall reboot system.
    	puts("System rebooting ...\n");

	int i = 100000;
	while(i--);

	__asm__ __volatile__("int $0x72"::"a"(4));
    	return 0;
}

int cmd_rename(int argc,char **argv)
{
    	puts("Function not implemented\n");
    	return 0;
}

int cmd_run(int argc,char **argv)
{		

	unsigned int pid_ret = exectve(argc,argv);

	if(!pid_ret) {

		printf("Run: \"%s\" error\n",argv[1]);
		return -1;
	}


	// wait pid
	write_stx(X_MSG_WAIT_PID,pid_ret,0,stdxserver);

	fputc('\n',stdout);

	return 0;


}



int cmd_shutdown(int argc,char **argv)
{
    	puts("Too much claw, the road is long.\n");
    	return 0;

}
int cmd_time(int argc,char **argv)
{
	unsigned i = 0;
	while(1) {

		printf("%d ",i++);	

	}


    	return 0;

}

int cmd_version(int argc,char **argv)
{
    	puts("Sirius Operating System v2.00\n");
    	return 0;
}
