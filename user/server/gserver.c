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
#include <gx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern GUI *G;

void update_obj (gx_hand_t *w) { 

	gx_hand_t *t = w->tail;
	while(t) {
		switch (t->type) {

			case GX_TYPE_TITLE:
				update_title(w,t);
			break;

			case GX_TYPE_BOX:
				update_box(w,t);
			break;
			case GX_TYPE_LABEL:
				update_label(w,t);
			break;
			case GX_TYPE_EDITBOX:
				update_editbox(w,t);
			break;
			case GX_TYPE_BUTTON:
				update_button(w,t);
			break;
		}
			
		t = t->tail;
	}

}


void update () 
{
	int r;
	gx_hand_t *l = (gx_hand_t *) G->l.l;
	gx_hand_t *l2 = (gx_hand_t *) G->l.l2;
	gx_hand_t *w = l->next;
	gx_hand_t *w2 = l2->next;

	if(G->l.spin_lock[1]) return;

	G->l.spin_lock[1] = 1;

	while (w) {

		r = update_window(w);
		if(r) {
			w = w->next;
			continue;
		}

		update_obj (w);

		w = w->next;
	}

	// l2
	while (w2) {

		r = update_window(w2);
		if(r) {
			w2 = w2->next;
			continue;
		}
		update_obj (w2);

		w2 = w2->next;
	}

	G->l.spin_lock[1] = 0;
}

int main(int argc, char **argv)
{
	MOUSE *mouse = (MOUSE*)(*(unsigned int*)(0x10001120));

	gx_hand_t *l = (gx_hand_t *) G->l.l;
	memset(l, 0, sizeof(gx_hand_t));
	gx_hand_t *l2 = (gx_hand_t *) G->l.l2;
	memset(l2, 0, sizeof(gx_hand_t));
	
	// Global init 
	// pronto
	G->l.spin_lock[0] = 0;

	int x = 0,y =0;

	while(TRUE) {

		gx_clearscreen(G->BankBuffer);
		//clear cursor
		DrawMouse(x,y,0, G->BankBuffer,cursor18x18);
		// FIXME a memoria deve ser de ring3
		BitMAP(	G->WindowScreen,220,100,G->BankBuffer);

		while(G->l.spin_lock[1]);

		update ();
	

		if(mouse->handle)update_window_mouse((GW_HAND *)mouse->handle);
		x = mouse->x;
		y = mouse->y;
		DrawMouse(x,y,0xDCDCDC/*ColorTable[GW_GREEN]*/, G->BankBuffer,gx_cursor18x18);

		//app_refreshrate();

		gx_refreshscreen( G->FrameBuffer, G->BankBuffer );

	}

	return 0;
}
