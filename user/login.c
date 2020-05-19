/*
 * File Name: login.c
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

#include <gx.h> 
#include <string.h>

int main(int argc, char **argv)
{

	char s1[] = "Usuário";
	char s2[] = "Senha";
	char s3[] = "Login";

	int x1 = 300;
	int x0 = (G->HorizontalResolution/2) - (x1/2);
	int y1 = 200;
	int y0 = (G->VerticalResolution/2) - (y1/2);

	unsigned int phys = 0;
	__asm__ __volatile__("int $0x72":"=a"(phys):"a"(0x18));
	unsigned int *system_lock = (unsigned int *) phys;


	gx_message_t m;
	gx_mouse_init();

	gx_hand_t *w = create_window ( x0,y0, x1, y1, 0);
	create_title(w, "Login", 1, 0);

	create_label(w,s1,8,32,(strlen(s1)*w->font.cols),w->font.rows,0);
	gx_hand_t *user = create_editbox(w,8,32+24,w->w-16,w->font.rows+8,256, 0/*normal*/);

	create_label(w,s2,8,32+60,(strlen(s2)*w->font.cols),w->font.rows,0);
	gx_hand_t *password = create_editbox(w,8,32+24+60,w->w-16,w->font.rows+8,256, 1/*senha*/);

	gx_hand_t *ok = create_button(w, s3,110,32+120, 80, w->font.rows+8, 0);

	for(;;) {	

		m = gx_mouse (w);

		switch(m.type) {

			case GX_OK:

				exit_window(w);
				*system_lock = 0;
				exit(0);
				
			break;


		}


	}
	return 0;
}
