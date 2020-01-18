/*
 * File Name: exit.c
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

/*
 * Removemos o item da lista e desalocamos as memorias
 *
 *
 */

UINTN exit_thread = 0;

UINTN exit()
{
	UINTN rc = 0;

	// retorna error se nao tiver nenhuma thread na lista
	if (!current_thread) rc = -1;
	else if(!current_thread->pid) rc = -1;
	else {

	// Pega o proximo item e aponta para o inicio
	THREAD	*next = current_thread->next;
	THREAD	*p = thread_ready_queue;
	// salve ponteiro
	THREAD	*current = current_thread;

	// Percorre a lista ate achar o p->next igual ao current_thread
	do{

		if(p->next == current)break;
		p = p->next;

	}while(TRUE);

	// aponta o p->next para o next
	p->next = next;
	

	if(!current->_static) {
		current->flag = -1;
		free_pages(current->pd);
		free_pages(current->pt);
		free_frame(current->frame);
	}


	// Exit focos
	if(focus->pid == current->pid ){

		focus->pd = thread_ready_queue->cr3;
		focus->pid = thread_ready_queue->pid;
	}

	free(current);

	exit_thread = 1;


	// Tast switch
	__asm__ __volatile__("int $0x71"::);



	rc = 0;

	}


	return rc;
}
