/*
 * File Name: os.h
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
#ifndef __OS_H__
#define __OS_H__


#ifndef IN 	// Prototype argument decoration for EFI parameters to indicate their direction
    #define IN 	// IN - argument is passed into the function
    #define OUT	// OUT - argument (pointer) is returned from the function
    #define OPTIONAL	// OPTIONAL - argument is optional
#endif


#ifndef CONST
   #define CONST const
#endif
#ifndef TRUE
    #define TRUE    ((BOOLEAN) 1)
    #define FALSE   ((BOOLEAN) 0)
#endif

#ifndef NULL
    #define NULL    ((VOID *) 0)
#endif


#define A32_EFER_MSR 0xC0000080
#define OS_PAGE_ADDR 0x10000




#include "typ.h"
#include "io.h"
#include "cpuid.h"
#include "paging.h"
#include "gui/gui.h"
#include "dscptor.h"
#include "apic.h"
#include "thread.h"
#include "drive/pci.h"
#include "drive/ps2.h"
#include "drive/rtc.h"
#include "drive/disk/ata.h"
#include "drive/nic.h"
#include "mbr.h"
#include "fs/vfs.h"
#include "fs/fat.h"
// LIB
#include "lib/lib.h"




#define debug(format, ...){\
	cli();\
	print(format, ##__VA_ARGS__);\
	sti();} 

#define HEADER_MAGIC 0x454C4F43



// Externs		
extern 	UINTN		stack_esp;
extern 	UINTN 		stack_esp_0;
extern 	PAGE_DIRECTORY 	*kernel_page_directory; 
extern 	PAGE_TABLE 	*kernel_page_table;
extern 	UINT16 		buffer_keyboard[KEYBOARD_BUFFER_SIZE/2];
extern 	UINTN 		buffer_keyboard_msg;
extern 	THREAD		*current_thread;
extern 	THREAD		*thread_ready_queue;
extern 	UINTN		next_pid;
extern 	GUI 		*G;
extern 	GW_HAND	       	*Box;
extern 	CHAR8 		*font;
extern 	UINT16 		font8x16[128*16];
extern 	UINT32 		ColorTable[16];
extern 	UINT32 		global_controll_task_switch;
extern 	UINTN 		exit_thread;
extern 	FOCUS	       	*focus;
extern 	THREAD		*thread_focus;
extern 	UINTN		key_msg_focos;
extern 	UINTN		key_msg_exec_console;

extern UINTN	_end();
extern unsigned char *ZERO;
extern unsigned char *__vfsbuf__;

// CHAT
extern CHAT *ready_queue_host_chat, *host_chat;

//Boot Info
extern 	UINT32 		UID;
extern 	UINT32 		DEV;

//APP USE
// Ponteiros para o USER mode 
extern UINT32 		*GwFocus;
extern MOUSE 		*mouse;
extern UINT32 		*rtc;

// console
extern CHAR8 *__buffer__;
extern CHAR8 *__string__;


// FAT
extern VOID 		*FAT;
extern FAT_BPB 		*__bpb__;
extern FAT_DIRECTORY 	*__root__;
extern FAT_DATA 	*__data__;
extern VFS 		*__vfs__;



// FILE
extern unsigned int *device;
extern FILE *sd;
extern FILE *xserver;
extern FILE *gserver;


// OS
VOID wait_ns(UINTN count);

// Memory
unsigned int ram_initialize(); 
int alloc_frame(PAGE_TABLE *pt,int count,VIRTUAL_ADDRESS *frame);
void free_frame(void *frame);

UINTN alloc_pages_initialize();

UINTN alloc_pages(	IN UINTN type,
		 	IN UINTN size,
			OUT VIRTUAL_ADDRESS *addr);

VOID free_pages(IN VOID *addr);

UINTN mem_map(	IN PHYSICAL_ADDRESS phy_addr,
		OUT VIRTUAL_ADDRESS *virt_addr,
		IN UINTN size,
		IN UINTN flag);

VOID *malloc(UINTN size);
VOID free(VOID *buffer);

// System
VOID initialize_gui();

UINTN gdt_install(VOID);
UINTN idt_install(VOID);

UINTN do_exec(CONST CHAR8 *name,UINT8 prv);
UINTN do_exec_child(THREAD *father_thread,CONST CHAR8 *name,UINT8 prv);
int exit();

//  MSR
UINTN 
getmsr(UINT32 msr, UINT32 *low, UINT32 *hight);
UINTN 
setmsr(UINT32 msr, UINT32 low, UINT32 hight);


// Process
UINTN initialize_focus();
UINT32 switch_cr3(UINT32 pdbase);
UINTN set_focus(UINTN pid);
UINTN msg_set_focus();




// String
UINTN strlen(CONST CHAR8 *s);
VOID *copymem(IN VOID *Destination,IN VOID *Source,IN UINTN Length);
VOID *setmem(IN VOID *Buffer,IN UINTN Size,IN UINT8 Value);
CHAR8 *strcpy(CHAR8 *Destination, CONST CHAR8 *Source);
UINTN strncpy(CHAR8 *Destination,CONST CHAR8 *Source,UINTN count);
UINTN strcmp (CHAR8* Destination, CONST CHAR8* Source);
UINTN strncmp (CHAR8* Destination,CONST CHAR8* Source,UINTN count);

// ASM
extern VOID __copymem(VOID *dest, VOID *src, UINTN Length);
extern VOID __setmem(VOID *dest, UINT32 val, UINTN Length);

void *memset(void *buffer,int val,unsigned count);



// console
UINTN 
ClearScreen();
UINTN 
SetAttribute(IN UINTN Foreground,IN UINTN Background);
UINTN  
EnableCursor(IN BOOLEAN Visible);
UINTN  
SetCursorPosition(IN UINTN Column,IN UINTN Row);
UINTN  
print(CONST CHAR8 *format,...);


//disk
int read_sector(int dev,unsigned count,unsigned long long addr,void *buffer);
int write_sector(int dev,unsigned count,unsigned long long addr,void *buffer);

//Media
UINTN Read(IN VFS *vfs,OUT VOID *buffer);
UINTN Write(IN VFS *vfs,IN VOID *buffer);

//Net
UINTN nic_install();

// FS
UINTN fat_initialize();
UINTN fat_filesystem(IN CHAR8 *path,IN UINTN dev,OUT VOID *out,IN UINT8 flg);
UINTN media_initialize();


// 
UINTN file_read(OUT VOID *buf,IN CHAR8 *name);
UINTN file_write(IN VOID *buf,CHAR8 *name);



//FIXME
UINTN  
dprintf(CONST CHAR8 *format,...);


// Exectve

int exectve(int argc,char **argv,char *pwd,FILE *fp);
int exectve_child(int argc,char **argv,char *pwd,FILE *fp,THREAD *father_thread);


// Devices
int conect_sd(int dev);
SD *read_sdx(const char *s);
SD *read_sdn(const char *s,SD *sdx);


#endif
