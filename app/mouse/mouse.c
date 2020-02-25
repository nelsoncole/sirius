/*
 * File Name: mouse.c
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
#include <stdlib.h>
#include <string.h>

#define TRUE 1


const char *str_mouse_menu[10] = {
	"Open",
	"Open with",
	"Run Terminal",
	"Cut",
	"Copy",
	"Paste",
	"Delete",
	"New folder",
	"New document",
	"Properties",
};

int main()
{

	unsigned int *p = (unsigned int*)0x10001120;
	MOUSE *mouse = (MOUSE*)(*p);

	long pool  = (long) malloc (0x10000); //64 KiB
	memset((char*)pool,0,0x10000);

	char **buf = (char**) malloc(0x1000); // 4KiB
	memset(buf,0,0x1000);
	char *line;
	int i;


	GW_HAND *window = create_window_mouse(mouse->x,mouse->y,160,180,
	GW_STYLE(FORE_GROUND(GW_BLACK) | BACK_GROUND(GW_WHITE) | BACK_GROUND_STYLE(GW_BLUE)),GW_FLAG_INVISIBLE);


	
	for(i=0;i<65536/256;i++)
	{

		buf[i] = (char*) pool + (i*256);

	}

	for(i=0;i < 10;i++)
	{

		line = buf[i];
		strcpy(line,str_mouse_menu[i]);

	}


	window->Msg1 = (unsigned int )buf;
	window->box->Flag = GW_FLAG_READY;


	while (TRUE) {


		


		if(mouse->b&0x2) 
		{

			while(mouse->b&0x2);

			if(window->Flag == GW_FLAG_VISIBLE) 
			{
				window->Flag = GW_FLAG_INVISIBLE;

			} else { 

				window->Flag = GW_FLAG_VISIBLE;

				window->X = mouse->x;
				window->Y = mouse->y;
			}
		}







	}


	return 0;
}
