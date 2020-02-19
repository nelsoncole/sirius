/*
 * File Name: process.c
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


FOCUS 	*focus		= NULL;
THREAD	*thread_focus 	= NULL;
THREAD	*zzzz_		= NULL;
UINTN	key_msg_focos   = 0; 
UINTN	key_msg_exec_console   = 0;


UINTN initialize_focus()
{

	thread_focus = thread_ready_queue;

	focus = (FOCUS*)malloc(sizeof(FOCUS));

	focus->pid = 0;
	focus->pd = (UINT32)kernel_page_directory;

	return 0;

}


UINT32 switch_cr3(UINT32 pdbase)
{
	UINT32 old = 0;

	__asm__ __volatile__("\
	movl %%cr3,%%edx;\
	movl %%eax,%%cr3;":"=d"(old):"a"(pdbase));

	// deixa a MMU terminar
	wait_ns(120);

	return old;

}

UINTN set_focus(UINTN pid)
{

	THREAD	*p = thread_ready_queue;

	while(p->pid != pid)p = p->next;

	focus->pd = p->cr3;
	focus->pid = p->pid;

	zzzz_ = p;


	return 0;
}

UINTN msg_set_focus()
{

	UINTN x = 0;

	do {
		if((thread_focus->prv == 1) && (thread_focus->pid != focus->pid )) 
		{

			zzzz_ = thread_focus;
			break;

		}

		thread_focus = thread_focus->next;

		if(!(thread_focus) && (!(x))) {
			thread_focus = thread_ready_queue;
			x++;
		} else { zzzz_ = NULL; return -1;}
		
	}while(TRUE);

	focus->pd = thread_focus->cr3;
	focus->pid = thread_focus->pid;

	key_msg_focos  = 0;


	return 0;
}
