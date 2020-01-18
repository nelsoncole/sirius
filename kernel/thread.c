/*
 * File Name: thread.c
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

THREAD	*current_thread; // A tarefa actualmente em execunção
THREAD	*thread_ready_queue; // O início da lista.

// O próximo ID do processo disponível.
UINTN 	next_pid = 0; 


// registers
UINT32	context_eip;
UINT32	context_eflag;
UINT32	context_cr3;

//registers 
UINT32	context_eax;
UINT32	context_ebx;
UINT32	context_ecx;
UINT32	context_edx;

// registers 
UINT32	context_edi;
UINT32	context_esi;

// registers
UINT32	context_ebp;
UINT32	context_esp;

// segments
UINT32	context_cs;
UINT32	context_es;
UINT32	context_ds;
UINT32	context_fs;
UINT32	context_gs;
UINT32	context_ss;

// outros
UINT32	context_esp0;
UINT32	context_pid;

  

UINTN initialize_thread()
{

	next_pid = 0; 

   
    	current_thread 		= thread_ready_queue = (THREAD*)malloc(sizeof(THREAD));
    	current_thread->pid 	= next_pid++;
    	current_thread->eax 	= 0;
    	current_thread->ebx 	= 0;
    	current_thread->ecx 	= 0;
    	current_thread->edx 	= 0;
    	current_thread->edi 	= 0;
    	current_thread->esi 	= 0;
    	current_thread->ebp 	= 0;
    	current_thread->esp 	= 0;
    	current_thread->eip 	= 0;
    	
    	current_thread->cs 	= 0x8;
    	current_thread->ds 	= 0x10;
    	current_thread->es 	= 0x10;
    	current_thread->fs 	= 0x10;
    	current_thread->gs 	= 0x10;
    	current_thread->ss 	= 0x10;

	current_thread->eflag 	=0x3202;
    	current_thread->cr3 	= (UINT32)kernel_page_directory;

    	current_thread->esp0 	= 0; //4KiB

	tss_esp0(current_thread->esp0);

	current_thread->_static = 1;

	current_thread->prv	= 0;

   
    	current_thread->next 	= NULL;

	current_thread->flag	= 0;




	return 0;

}

// Todas os processos serão criados a partir desta função
UINTN create_thread(	VOID (*main)(),
			PAGE_DIRECTORY *page_directory,
			UINT32 eax,
			UINT32 ebx,
			UINT32 ecx,
			UINT32 edx,
			UINT32 esp,
			UINT32 esp0,
			UINT8 privileg)
{

    	THREAD	*new_thread	=(THREAD*)malloc(sizeof(THREAD)); 
    	new_thread->pid 	= next_pid++;

    	new_thread->eax 	= eax;
    	new_thread->ebx 	= ebx;
    	new_thread->ecx 	= ecx;
    	new_thread->edx 	= edx;
    	new_thread->edi 	= 0;
    	new_thread->esi 	= new_thread->pid;
    	new_thread->ebp 	= 0;
    	new_thread->esp 	= esp;
	new_thread->eip  	= (UINT32)main;

    	if((privileg&1) == 1) { // processo do userspace
    	new_thread->cs	= 0x1B;
    	new_thread->ds 	= 0x23;
    	new_thread->es 	= 0x23;
    	new_thread->fs 	= 0x23;
    	new_thread->gs 	= 0x23;
    	new_thread->ss 	= 0x23;


	new_thread->prv = 1;

    	}

	if(!(privileg&1)){ // processo do kernelspace
    	new_thread->cs 	= 0x8;
    	new_thread->ds 	= 0x10;
    	new_thread->es 	= 0x10;
    	new_thread->fs 	= 0x10;
    	new_thread->gs 	= 0x10;
    	new_thread->ss 	= 0x10;
	
	new_thread->prv = 0;

    	}

	if(privileg&2) new_thread->_static = 1;
	else new_thread->_static = 0;
	
   
	new_thread->eflag 	= 0x3202;
    	new_thread->cr3 	= (UINT32)page_directory;

	new_thread->esp0   	= esp0;


	new_thread->pd   	= (PAGE_DIRECTORY*)page_directory;

	UINTN d = (UINTN) (APP_VIRTUAL_ADDRESS >> 22 &0x3FF);
	new_thread->pt   	= (PAGE_TABLE*)((UINTN)(page_directory[d].addrpt << 12));

    	new_thread->next 	= NULL;

	new_thread->flag	= 0;

    
     
    	// Adicionar novo elemento, no final da lista
    	// tmp aponta para inicio da lista
    	THREAD *tmp = thread_ready_queue;
    	while (tmp->next)
    	tmp = tmp->next;
    
    	tmp->next = new_thread;


   	return (new_thread->pid);
}


VOID task_switch(VOID){

	// verifica exit
	if(!(exit_thread)) {

    	// Salve o contexto da actual tarefa
    	// em execução   
	current_thread->eax 	= context_eax;
    	current_thread->ebx 	= context_ebx;
    	current_thread->ecx 	= context_ecx;
    	current_thread->edx 	= context_edx;
    	current_thread->edi 	= context_edi;
    	current_thread->esi 	= context_esi;
    	current_thread->ebp 	= context_ebp;
    	current_thread->esp 	= context_esp;
    	current_thread->eip 	= context_eip;

	current_thread->cs 	= context_cs;
    	current_thread->ds 	= context_ds;
    	current_thread->es 	= context_es;
    	current_thread->fs 	= context_fs;
    	current_thread->gs 	= context_gs;
    	current_thread->ss 	= context_ss;

	current_thread->eflag 	= context_eflag;
    	current_thread->cr3 	= context_cr3;

	}else exit_thread = 0;

    	// Pula, se ainda não tiver 
    	// a tarefa inicializada...
    	if (!current_thread) goto end;
    	else{

    	// Obter a próxima tarefa a ser executada.
    	current_thread 	= current_thread->next;

    	// Se caímos no final da lista vinculada, 
    	// comece novamente do início.
    	if (!current_thread)
    	current_thread = thread_ready_queue->next;

    	}

    	// Restaura o contexto da próxima
    	// tarefa a ser executada...
	context_eax 	= current_thread->eax;
    	context_ebx 	= current_thread->ebx;
    	context_ecx 	= current_thread->ecx;
    	context_edx 	= current_thread->edx;
    	context_edi 	= current_thread->edi;
    	context_esi 	= current_thread->esi;
    	context_ebp 	= current_thread->ebp;
    	context_esp 	= current_thread->esp;
    	context_eip 	= current_thread->eip;

	context_cs 	= current_thread->cs;
    	context_ds	= current_thread->ds;
    	context_es	= current_thread->es;
    	context_fs	= current_thread->fs;
    	context_gs	= current_thread->gs;
    	context_ss	= current_thread->ss;

	context_eflag	= current_thread->eflag;
    	context_cr3	= current_thread->cr3;

    	context_esp0	= current_thread->esp0;

	
	// O ID
	context_pid	= current_thread->pid;


    	// stack esp0
	tss_esp0(context_esp0);

	load_page_diretory((PAGE_DIRECTORY *)(context_cr3));
	flush_tlb();



	// Aqui enviamos a mensagem para a thread

	



end:
	return;
    
}

// Retorna o pid do processo atual.
UINTN getpid()
{
    return current_thread->pid;
}
UINTN getcr3()
{
    return current_thread->cr3;
}
