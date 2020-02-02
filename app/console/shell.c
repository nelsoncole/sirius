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
 
#include <io.h>
#include <string.h>

#define SHELL_CMD_NUM 17 + 2

extern UINTN cls();
extern UINTN set_cursor_x(UINTN x);
extern UINTN set_cursor_y(UINTN y);
extern UINTN set_cursor(UINTN x, UINTN y);
extern UINTN putchar(UINTN ch);
extern VOID puts(CONST CHAR8 *string);
extern VOID printf(CONST CHAR8 *format,...);

extern UINTN getchar();
extern VOID gets(CHAR8 *string);





VOID (*call_loader)  (UINTN argc,UINT32 *argv[]) = NULL;

typedef struct _COMMAND {
  char *name;
  void *fun;
  char *help;
}__attribute__((packed)) COMMAND;

UINTN cmd_version(INTN argc,CHAR8 *argv[]);
UINTN cmd_time(INTN argc,CHAR8 *argv[]);
UINTN cmd_shutdown(INTN argc,CHAR8 *argv[]);
UINTN cmd_rename(INTN argc,CHAR8 *argv[]);
UINTN cmd_reboot(INTN argc,CHAR8 *argv[]);
UINTN cmd_new(INTN argc,CHAR8 *argv[]);
UINTN cmd_mov(INTN argc,CHAR8 *argv[]);
UINTN cmd_info(INTN argc,CHAR8 *argv[]);
UINTN cmd_help(INTN argc,CHAR8 *argv[]);
UINTN cmd_exit(INTN argc,CHAR8 *argv[]);
UINTN cmd_echo(INTN argc,CHAR8 *argv[]);
UINTN cmd_dir(INTN argc,CHAR8 *argv[]);
UINTN cmd_del(INTN argc,CHAR8 *argv[]);
UINTN cmd_date(INTN argc,CHAR8 *argv[]);
UINTN cmd_copy(INTN argc,CHAR8 *argv[]);
UINTN cmd_cls(INTN argc,CHAR8 *argv[]);
UINTN cmd_cd(INTN argc,CHAR8 *argv[]);

UINTN cmd_pci(INTN argc,CHAR8 *argv[])
{

	__asm__ __volatile__("int $0x72"::"a"(5),"d"(2));
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
    	{"shutdown",    cmd_shutdown,       "Shutdown your computer locally or remotely"    	},
   	{"time",        cmd_time,           "Time"                                          	},
    	{"version",     cmd_version,        "Shell version"                                 	},
	{"pci",     	cmd_pci,      	    "Info PCI Device"                                 	},
};

UINTN shell()
{
	UINTN i;
	CHAR8 cmd_buffer[256];
	CHAR8 *cmd_name = (char*) __malloc(0x1000);

	setmem(cmd_buffer,256,0);
	setmem(cmd_name,0x1000,0);

	gets(cmd_name);

	putchar('\n');
	
	for(i=0;i < SHELL_CMD_NUM;i++)
        {
            if((strcmp(cmd_name,cmd_table[i].name)) == 0)
            {

		call_loader = (VOID(*)(UINTN,UINT32 **))(cmd_table[i].fun);

		call_loader(0,0);
                break;
            }
            else if(i == (SHELL_CMD_NUM - 1))
            printf("%s: command not found\n",cmd_buffer);
        }


	return 0;
}


UINTN cmd_cd(INTN argc,CHAR8 *argv[])
{

    	return 0;
}

UINTN cmd_cls(INTN argc,CHAR8 *argv[])
{
	cls();
    	return 0;
}

UINTN cmd_copy(INTN argc,CHAR8 *argv[])
{

    	return 0;
}

UINTN cmd_date(INTN argc,CHAR8 *argv[])
{
    	return 0;

}


UINTN cmd_del(INTN argc,CHAR8 *argv[])
{
    	return 0;
}


UINTN cmd_dir(INTN argc,CHAR8 *argv[])
{
    	return  0;
}

UINTN cmd_echo(INTN argc,CHAR8 *argv[])
{
    	return 0;
}	

extern GW_HAND *whd;
extern CHAR8 *vram;
UINTN cmd_exit(INTN argc,CHAR8 *argv[])
{
	__free(vram);
	gui_exit(whd);
	exit(0);
    	return 0;
}
UINTN cmd_help(INTN argc,CHAR8 *argv[])
{
        UINTN i;
        printf("Commands:\n");
        for(i=0;i< SHELL_CMD_NUM;i++){
        printf("%s",cmd_table[i].name);
        set_cursor_x(15);
        printf("%s\n",cmd_table[i].help);

        }

        return 0;

}
UINTN cmd_info(INTN argc,CHAR8 *argv[])
{
	printf("System information:\
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
	\n0x10001110 - 0x10001113  *RCT             // size 4 bytes\
	\n0x10001114 - 0x10001117   PID             // size 4 bytes\
	\n0x10001118 - 0x1000111B  *FOCUS           // size 4 bytes\
	\n0x1000111C - 0x1000111F  *MOUSE           // size 4 bytes\
	\n0x10001120 - 0x100011FF   Reserved        // size 224 bytes\
	\n0x10001200 - 0x100012FF   Channel         // size 256 bytes\n");

        return 0;

}

UINTN cmd_mov(INTN argc,CHAR8 *argv[])
{

	whd->X = whd->X - 300;
    	return 0;
}

UINTN cmd_new(INTN argc,CHAR8 *argv[])
{
	whd->X = whd->X + 300;

    	return 0;
}


UINTN cmd_reboot(INTN argc,CHAR8 *argv[])
{
    	// This syscall reboot system.
    	printf("System rebooting ...\n");

	UINTN i = 100000000;
	while(i--);

	__asm__ __volatile__("int $0x72"::"a"(4));
    	return 0;
}

UINTN cmd_rename(INTN argc,CHAR8 *argv[])
{
    	printf("Function not implemented\n");
    	return 0;
}



UINTN cmd_shutdown(INTN argc,CHAR8 *argv[])
{
    	printf("Too much claw, the road is long.\n");
    	return 0;

}
UINTN cmd_time(INTN argc,CHAR8 *argv[])
{
    	return 0;

}

UINTN cmd_version(INTN argc,CHAR8 *argv[])
{
    	printf("Sirius Operating System v2.00\n");
    	return 0;
}


