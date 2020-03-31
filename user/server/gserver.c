/*
 * File Name: gserver.c
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
#include <ctype.h>

extern GUI *G;

INTN main(INTN argc,CHAR8 *argv[])
{
	UINT32 *p = (UINT32*)0x1000111C;
	UINT32 *GwFocus = (UINT32 *)(*p++);
	MOUSE *mouse = (MOUSE*)(*p++);

	UINTN __flag__ = 0;

	GW_HAND *obj = NULL;
	GW_HAND *window	= NULL;
	GW_HAND *list = (GW_HAND*)(G->List);

	window	= list->next;
	if(window) obj = window->tail;
	else obj = NULL;


	while(TRUE) {

		app_clearscreen();
		
		//BitMAP(	(UINTN*)0xA00000,250,100,G->BankBuffer);
	
		// UPDATE
		while(window) {

			if(*GwFocus == (UINT32)window) {
				
				window = window->next;
				if(window) obj = window->tail;
				__flag__ = 1;
				continue;
			}

			UpdateWindow(window);
			while(obj) {
				UpdateObject(window,obj);
				obj = obj->tail;
			}


			
			window = window->next;
			if(window) obj = window->tail;
			else obj = NULL;	

		}

		// Focus
		if(__flag__ == 1) {
			__flag__ = 0;
			window	= (GW_HAND *)(*GwFocus);
			if(window) obj 	= window->tail;

			UpdateWindow(window);

			while(obj) 
			{
				UpdateObject(window,obj);
				obj = obj->tail;

			}

		}
	

		if(mouse->handle)update_window_mouse((GW_HAND *)mouse->handle);

		DrawMouse(mouse->x,mouse->y,ColorTable[GW_GREEN], G->BankBuffer,cursor18x18);

		app_refreshrate();


		//rewind
		//window = list;
		window	= list->next;
		if(window) obj = window->tail;
		else obj = NULL;
		

	}


	return 0;
}
