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


THREAD *exit_child(THREAD *thread)
{

	
	if(!thread) return 0;
	else if(!thread->pid) return 0;


	THREAD *tail = thread->tail;

	if(!tail) return 0;

	if(tail) thread->tail  = tail->tail;


	// Pega o proximo item e aponta para o inicio
	THREAD	*next = tail->next;
	THREAD	*p = thread_ready_queue;
	// salve ponteiro
	THREAD	*current = tail;


	// Percorre a lista ate achar o p->next igual ao current_thread
	do{

		if(p->next == current)break;
		p = p->next;

	}while(p);

	if(!p) return 0;

	// aponta o p->next para o next
	p->next = next;

	current->status = -1; // Processo morto
	free_pages(current->pd);
	free_pages(current->pt);
	free_frame(current->frame);
	free(current);

	return thread;

}




int exit() 
{
	// Pega o proximo item e aponta para o inicio
	THREAD	*next = current_thread->next;
	THREAD	*p = thread_ready_queue;
	// salve ponteiro
	THREAD	*current = current_thread;
	
	THREAD *thread_child = current_thread;

	// Percorre a lista ate achar o p->next igual ao current_thread
	do{

		if(p->next == current)break;
		p = p->next;

	}while(p);

	if(!p) return -1;

	// aponta o p->next para o next
	p->next = next;


	if(!(current->status&1)) // processo pai
	{
		// exit processo filho
		while(thread_child)
		{	
			thread_child = exit_child(thread_child);
		}

		if(!current->_static) 
		{
			current->status = -1; // Processo morto
			free_pages(current->pd);
			free_pages(current->pt);
			free_frame(current->frame);
		}


		// Exit focos
		if(focus->pid == current->pid ) 
		{
			focus->pd = thread_ready_queue->cr3;
			focus->pid = thread_ready_queue->pid;
		}


		close(current->stdin);
		close(current->stdout);
		close(current->stderr);
		close(current->stdx);
		



	} else if(current->status&1) { // processo filho

		// exit processo filho
		while(thread_child)
		{	
			thread_child = exit_child(thread_child);
		}

		if(!current->_static) 
		{
			current->status = -1; // Processo morto
			free_pages(current->pd);
			free_pages(current->pt);
			free_frame(current->frame);
		}


		// Exit focos
		if(focus->pid == current->pid ) 
		{
			focus->pd = thread_ready_queue->cr3;
			focus->pid = thread_ready_queue->pid;
		}


		current->alpha->tail = NULL;

	}

	if(current->prv&1)free((void*)current->esp0);
	free(current);
	return 0;

}



