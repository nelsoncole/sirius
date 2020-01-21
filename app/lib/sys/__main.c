/*
 * File Name: __main.c
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

UINTN spin_lock = 0;

extern INTN main();

FOCUS 	*__focus	= NULL;
UINT32	__pid = -1;

CHAT *ready_queue_host_chat;
CHAT *host_chat;

typedef struct _BOOT_INFO{
	GUI Graphic;
	
}__attribute__((packed)) BOOT_INFO;

UINTN __main(BOOT_INFO *boot_info)
{	
	UINT32 *p = NULL;

	// GUI	
	G = (GUI *)&boot_info->Graphic;

	// pid e focus

	p = (UINT32*)0x10001114;
	__pid = *p;
	__focus = (FOCUS*)(*++p);


	// inicializa chat

	ready_queue_host_chat = host_chat = (CHAT*) MSG_VIRTUAL_ADDR;
	host_chat->type = 0;
	host_chat->next = NULL;
 
	spin_lock = 0;

	exit(main());

	for(;;);
}

VOID send_msg(UINT32 type,UINT32 p1,UINT32 p2)
{

	while(spin_lock);
	spin_lock++;
	__asm__ __volatile__("int $0x72"::"a"(6),"d"(type),"c"(p1),"b"(p2));

	spin_lock--;

}
UINTN read_msg(UINT32 *p1,UINT32 *p2) 
{
	UINT16 *input 	= (UINT16*) 0x10001100; // extended key

	while(spin_lock);
	spin_lock++;


	CHAT *chat;
	UINT32 type = 0;


	// Teclado

	if(input[1]) {
		*p1 = input[1] &0xffff;
		*p2 = 0;
		type = MSG_READ_KEY;


		input[0] = 0;
		input[1] = 0;

		goto done;

	}


	host_chat = host_chat->next;


	if(!(host_chat)) { // final da lista

		host_chat = ready_queue_host_chat;

		goto done;
		
	}

	if(host_chat/*verifica se ha msg na fila*/) {

		if(!host_chat->type)goto done;

		*p1 = host_chat->p1;
		*p2 = host_chat->p2;
		type = host_chat->type;



		//remover da lista de mensagens
		// Percorre a lista ate achar o p->next igual ao current
		chat = ready_queue_host_chat;

		while(chat->next != host_chat) chat = chat->next;

		// aponta o chat->next para o current->next
		chat->next = host_chat->next;


		__free(host_chat);


	}

done:

	spin_lock--;

	return type;
}


VOID exit(INTN rc)
{
	__asm__ __volatile__("int $0x72"::"a"(1));

}


VOID *__malloc(UINTN size){

	UINTN rc;

	__asm__ __volatile__("int $0x72":"=a"(rc):"a"(2),"d"(size));

	return (VOID*)(rc);

};


VOID __free(VOID *buffer){

	__asm__ __volatile__("int $0x72"::"a"(3),"d"(buffer));
}
