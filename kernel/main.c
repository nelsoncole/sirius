/*
 * File Name: main.c
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
 
#include <os.h> 

typedef struct _BOOT_INFO {
	UINT32 	UID;
	UINT32	DEV;
	UINT8   r0[0x28];
	GUI Graphic;
	
}__attribute__((packed)) BOOT_INFO;


UINTN 
GraphicInitialize(GUI *Graphic)
{
	
	// GUI
	G = (GUI *) Graphic;
	
	return 0;
}

extern void e1000_irq_wait();
extern int e1000_send_arp ( unsigned char source_ip[4], unsigned char target_ip[4], unsigned char target_mac[6] );
extern int e1000();
int terminal = 0;
VOID thread_main()
{
	UINTN pid;
	terminal = 0;


	debug("Initialize thread main\n");

	/*e1000_irq_wait();

	unsigned char source_ip[4] = {127,0,0,1}; // 192.168.33.1
	unsigned char target_ip[4] = {127,0,0,1}; // UNITEL
	unsigned char target_mac[6] = {0xff,0xff,0xff,0xff,0xff,0xff};


	cli();
	e1000_send_arp (source_ip, target_ip, target_mac);
	sti();*/


	while(TRUE) {

		// foco F1
		if(key_msg_focos) {
		 	cli();
		 	msg_set_focus();
			sti();	
		}

		// execute console F2
		if(key_msg_exec_console || terminal != 0) {
			cli();

			pid = do_exec("trm.bin",1);
			set_focus(pid);
			key_msg_exec_console = 0;

			terminal = 0;

			sti();	

		
		}

	}

}

extern unsigned int g_mm_mp;
int main(BOOT_INFO *boot_info)
{	
	//UINTN pid;
	unsigned int local_apic_virtual_addr;

	UID = boot_info->UID;
	DEV = boot_info->DEV;

	// GUI
	GraphicInitialize(&boot_info->Graphic);	

/*

 *	A paginação e outras estruturas de memória, deve ser as primeiras coisas
 *	a se inicializar, para evitar possíveis erros com o Display Console 

 */

	
	page_install();
	ram_initialize();
	alloc_pages_initialize();

	// intialize global
	g_mm_mp = 0;
	
	// Mapear o  Linear Frame Buffer e alocar memoria virtal para o Bank Buffer
	//mem_map((PHYSICAL_ADDRESS)G->FrameBuffer,
	//(VIRTUAL_ADDRESS *)&G->FrameBuffer,1024/*4MiB*/,0x13+0x4);
	
	mm_mp( (unsigned int) G->FrameBuffer, (unsigned int *) &G->FrameBuffer,1024/*4MiB*/,0x13 | 0x4);

	alloc_pages(0,1024/*4 MiB*/,(VIRTUAL_ADDRESS *)&G->BankBuffer);

	alloc_pages(0,1024/*4 MiB*/,(VIRTUAL_ADDRESS *)&G->WindowScreen);

	unsigned int phys = 0;
	alloc_pages(0,1,(VIRTUAL_ADDRESS *)&phys);
	memset((void*)phys,0,0x1000);
	G->l.l = (unsigned int ) phys;

	alloc_pages(0,1,(VIRTUAL_ADDRESS *)&phys);
	memset((void*)phys,0,0x1000);
	G->l.l2 = (unsigned int ) phys;	

	alloc_pages(0,1,(VIRTUAL_ADDRESS *)&phys);
	memset((void*)phys,0,0x1000);

	G->l.pid = (unsigned int *) phys;
	G->l.spin_lock = (unsigned int *) phys + 0x20;
	G->l.spin_lock[0] = 1;
	G->l.spin_lock[1] = 0;
	G->l.spin_lock[2] = 0;
	G->l.spin_lock[3] = 0;

	font = (CHAR8 *)(font8x16);

	ZERO = (unsigned char *)malloc(0x10000); // 64 KiB

	__vfs__ = (VFS*)malloc(0x8000); // 32 KiB
	__vfsbuf__ = (unsigned char *)malloc(0x8000); // 32 KiB

	

	// clear screen
	// Limpar o BankBuffer
	clearscreen();


	GW_HAND *Window = (GW_HAND*) malloc(sizeof(GW_HAND));

	Window->X 	= 0; // coordenada X
	Window->Y 	= 0; // Coordenada Y
	Window->Width	= G->HorizontalResolution; // largura
	Window->Height	= G->VerticalResolution; // altura
	Window->Buffer	= (VOID*) G->BankBuffer; // 

	

	Window->box = CreateBox(Window,0,0,Window->Width,Window->Height, 
	GW_STYLE(FORE_GROUND(GW_WHITE) | BACK_GROUND(GW_BLACK)),0);
	Box =	Window->box;

	// console
	__buffer__ = (CHAR8*)malloc(0x2000);
	__string__ = __buffer__ + 512;


	

	// clear screen
	ClearScreen();


	//FIXME
	BitMAP(	(UINTN*)0xA00000,300,100,G->BankBuffer); 
	refreshrate();
	
	print("Initialize Kernel: Sirius v2.0...\n");

	print("Install GDTR\n");	gdt_install();
	print("Install TSS\n");		tss_install();
	print("Install IDTR\n");	idt_install();




	//for(;;);

	// Mapear o Local APIC BASE
	//mem_map((PHYSICAL_ADDRESS)0xFEC00000/*IA32_LOCAL_APIC_BASE_ADDR*/,
	//(VIRTUAL_ADDRESS *)&local_apic_virtual_addr,1024/*4MiB*/,0x13);

	mm_mp( (unsigned int) IA32_LOCAL_APIC_BASE_ADDR, (unsigned int *) &local_apic_virtual_addr,1024/*4 MiB*/,0x13);
	
	apic_initialize(local_apic_virtual_addr);
	print("Install APIC Timer\n");	apic_timer();

	print("I/O APIC initialize ");
	ioapic_initialize();

	// Keyboard
	ioapic_umasked(1);
	// Mouse
	//ioapic_umasked(12);


	//ioapic_umasked(8);

	for(int i=3;i<24;i++)ioapic_umasked(i);


	// data initialize
	alloc_pages(0,1,(VIRTUAL_ADDRESS *)&phys);
	GwFocus = (UINT32*) phys;
	alloc_pages(0,1,(VIRTUAL_ADDRESS *)&phys);
	mouse	= (MOUSE*) phys;
	alloc_pages(0,1,(VIRTUAL_ADDRESS *)&phys);
	rtc	= (UINT8*) phys;

	setmem(GwFocus,0x1000,0);
	setmem(mouse,0x1000,0);
	setmem(rtc,0x1000,0);

	print("Instal Keyboard\n");	keyboard_install();
	print("Install Mouse\n");	mouse_install();


	print("Install RTC\n");		rtc_install();
	// RTC 


	print("Initialize Massa Storage Controller\n");
	ata_initialize();


	//e1000(); // internet


	
	// Multitasking
	print("Initialize Multitasking\n",initialize_thread());

	print("Initialize focus\n",initialize_focus());
	
	// testing thread
	create_thread(&thread_main,kernel_page_directory,0,0,0,0,(UINT32)malloc(0x2000),0,2);

	print("testing application on system\n");
	


	// TODO inicializacao do device

	device =(unsigned int*) malloc(0x1000);
	setmem(device,0x1000,0);

	sd = open(0,"std");
	xserver = open(0,"std");
	gserver = open(0,"std");

	device[0] = (unsigned int) sd;
	device[1] = (unsigned int) xserver;
	device[2] = (unsigned int) gserver;


	// Montar partiao do sistema
	if(conect_sd(DEV)) {
		print("PANIC: System partition not fould\n");
		for(;;);

	}
	
	SD *sdax = read_sdx("sda");
	SD *sda0 = read_sdn("sda1",sdax);
	print("%s/",sdax);
	print("%s:\nUID %x\nDevice Number %d\n",sda0,sda0->UID,sda0->devnum);
	print("LBA Start %d\nLBA End %d\n",sda0->lba_start,sda0->lba_end);
	print("Byte Of Sector %d\nPartition Number %d\n",sda0->byte_of_sector,sda0->partnum);
	print("Number Of Sector %d\nPartition Size %d\n",sda0->num_sectors,sda0->num_sectors*sda0->byte_of_sector/1024/1024);


	// LOADER MBR
	MBR *mbr = (MBR*)malloc(0x1000);

	if(read_sector(DEV,1,0,mbr) )
		print("Read MBR Error\n");
	else {


	// OPEN ROOT
	FAT = (VOID*)malloc(0x1000);
	__data__ = (FAT_DATA*)malloc(0x1000);

	
	VOLUME v;
	v.lba_start = mbr->part[0].lba_start;
	v.dev_num = DEV;

	__bpb__ = FatReadBPB(&v);
	if(!__bpb__) print("BPB Error");
	else __root__ =FatOpenRoot(__bpb__,__data__);


	}


	// FIXME debug
	//clearscreen();
	//ClearScreen(); 
	//for(;;);	

	// USER
	do_exec("xserver.bin",1);
	do_exec("gserver.bin",0x81);
	do_exec("desktop.bin",1);

	do_exec("taskbar.bin",1);
	//do_exec("trm.bin",1);
	//do_exec("test.bin",1);

	apic_timer_umasked();


	FILE *f = open ("logo.bmp","rx");
	if(f == NULL) { 
		print("open \"logo.bmp\" error\n");
	} else {

		if(read(G->WindowScreen,1,f->header.size,f) != f->header.size);
		close(f);
	}
	

	/*clearscreen();
	BitMAP(	(UINTN*)G->WindowScreen,220,100,G->BankBuffer);
	refreshrate(); */ //refresh_screen();



	
	sti(); //Enable eflag interrupt


	global_controll_task_switch = 0;
	key_msg_exec_console = 0;

	//debug("Done!\n");

	while(TRUE)/* Thread 0*/ {

		__asm__ __volatile__("hlt");
	
	}


	return (0);
}
