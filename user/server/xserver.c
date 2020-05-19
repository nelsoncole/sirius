/*
 * File Name: xserver.c
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

#include <sys/sys.h>
#include <stdlib.h>
#include <string.h>


#define true 1

STDX *pipe = NULL;
unsigned int *idt = NULL;

unsigned int write_idt(unsigned int pid_ret,unsigned int pid_cur)
{

	unsigned int *p_idt = idt;
	int i;


	if(!cheksum_pid(pid_ret)) return -1;


	while(true) {

		for(i=0;i<512;i++)
		{
			if(*p_idt == 0)
			{ 
				*p_idt++ = pid_ret;
				*p_idt++ = pid_cur;

				return 0;

			}

			p_idt++;
			p_idt++; 

		}


		p_idt = idt;

	}


	return -1;

}


unsigned int read_idt(unsigned int pid)
{

	unsigned int *p_idt = idt;
	int i;

	unsigned int rc = 0;

	for(i=0;i<512;i++)
	{
		if(pid == *p_idt)
		{ 

			*p_idt++ = 0;
			rc = *p_idt;
			*p_idt = 0;

			return rc;

		}

		p_idt++;
		p_idt++; 

	}


	return 0;

}


int main()
{
	int flg = 0;
	int r;

	idt = (unsigned int*) malloc(0x1000);
	memset(idt,0,0x1000);

	unsigned int pid = 0;
	int i;

	// 1 - All mount devices disks // PATA/ SATA e USB2.0
	int rc = 0;

	__asm__ __volatile__("int $0x72":"=a"(rc):"a"(5));

	for(i=0;i < rc;i++)
	{
		mount_sd(i);
	}




	//inicializa tarefas

	/*unsigned int phys = 0;
	__asm__ __volatile__("int $0x72":"=a"(phys):"a"(0x18));
	unsigned int *system_lock = (unsigned int *) phys;

	char _argv[256];
	char **argv = (char**) _argv;

	argv[0] = (char*)malloc(0x1000);*/


	/*strcpy(argv[0],"login.bin");
	r = exectve2(1,argv);

	while(*system_lock == 1234);*/

	/*strcpy(argv[0],"desktop.bin");

	r = exectve2(1,argv);
	strcpy(argv[0],"taskbar.bin");
	r = exectve2(1,argv);*/


	while(true) 
	{
		pipe = read_stx(stdxserver);

		if(pipe != NULL)
		{
			switch(pipe->id) 
			{
				case X_MSG_WAIT_PID:

				if(!write_idt(pipe->data1,pipe->pid))
				{
					flg = 0; // sem retorno
					pipe->pid = 0xFFFFFFFF;

				}else {

					flg = 1;
					pipe->id  = -1; // return

				}
					break;

				case X_MSG_INT:

				pid = read_idt(pipe->pid);

				if(pid) {

					unlockthread(pid);
				}

				flg = 1;
				pipe->id  = 0; // return

					break;

				default:

					flg = 0;

					break;

			}
		
			if(flg) 
			{
				pid = pipe->pid;
				pipe->pid = 0xFFFFFFFF; // lock
				flg = 0;
				// desbloquear processo
				// task switch
				unlockthread(pid);
				taskswitch_pid(pid);
			}
		}


	}

	return 0;
}
