#include <gx.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define null 0

gx_hand_t *create_editbox(gx_hand_t *window,int x, int y, int w, int h,unsigned int len, unsigned int style)
{
	if(len > 65536) return 0;

	gx_hand_t *e = (gx_hand_t *)malloc(sizeof(gx_hand_t));
	
	memset(e, 0, sizeof(gx_hand_t));

	e->x0 = x;
	e->y0 = y;
	e->x = x + window->x;
	e->y = y + window->y;
	e->w = w;
	e->h = h;

	e->flag = 0;
	e->style = style;

	e->type = GX_TYPE_EDITBOX;
	e->subtype = style &0xff;

	e->vmm = (void *) window->vmm;

	memcpy(&e->font,&window->font,sizeof(struct font));
	
	e->next = null;
	e->tail = null;

	// color
	e->c[0] =  0; //fg
	e->c[1] = -1; //bg



	e->m[0] = (unsigned int) malloc(len+8); //65536
	memset((void*)e->m[0],0,len+8);
	e->m[1] = len;
	e->m[2] = 0; // cursor

	// lista de event
	ps_add_list(window, e);

	e->spin_lock = 0;
	e->pid = 0;

	// add list
	obj_add_list( window, e);

	
	return (gx_hand_t *) e;
}


void *get_buffer_editbox(gx_hand_t *e)
{
	return (unsigned int*) e->m[0];
}

unsigned int get_len_editbox(gx_hand_t *e) {

	return e->m[1];
}

int get_cursor_editbox(gx_hand_t *e) {

	return e->m[2];
}

void update_cursor_editbox(gx_hand_t *e, int x) {
	switch(x) {
		case 0: if(e->m[2] < get_len_editbox(e))e->m[2] +=1;
		break;
		case 1: if(e->m[2] > 0)e->m[2] -=1;
		break;		
	}
}

void clrs_editbox(gx_hand_t *e) {

	unsigned char *b = (unsigned char *) get_buffer_editbox(e);
	int len = get_len_editbox(e);

	e->flag |= 1;

	memset(b,0,len+8);
	e->m[2] = 0;

	e->flag &=~1;

	
}


void motor_editbox(gx_hand_t *e) {

	if(!e->f) return;

	unsigned char *b = (unsigned char *) get_buffer_editbox(e);
	int cur = get_cursor_editbox(e);
	int len = get_len_editbox(e);

	int c = getckey();

	if(c == (-1) || c == 0) return;

	if(c == '\b') {
		update_cursor_editbox(e, 1);
		cur = get_cursor_editbox(e);
		*(char*)(b + cur) = '\0';

	} else {

		if(cur == len) return;
		*(char*)(b + cur) = c;
	
		update_cursor_editbox(e, 0);
	}
}

void r_motor_editbox(gx_hand_t *e, int c) { 

	unsigned char *b = (unsigned char *) get_buffer_editbox(e);
	int cur = get_cursor_editbox(e);
	int len = get_len_editbox(e);

	
	if(c == (-1) || c == 0) return;

	if(c == '\b') {
		update_cursor_editbox(e, 1);
		cur = get_cursor_editbox(e);
		*(char*)(b + cur) = '\0';

	} else {

		if(cur == len) return;
		*(char*)(b + cur) = c;
	
		update_cursor_editbox(e, 0);
	}
}

void exit_editbox(gx_hand_t *e)
{
	if(!e) return;

	while(e->spin_lock);
	e->flag |= 1;

	free((void*) e->m[0]);
	free(e);

}

int update_editbox(gx_hand_t *window, gx_hand_t *e)
{
	if(e->flag&1) return (-1);

	e->x = e->x0 + window->x;
	e->y = e->y0 + window->y;

	e->spin_lock = 1;

	gx_rect( e->x, e->y, e->w, e->h, e->c[1] /*bg*/, e->vmm);

	if(e->f)gx_border (e->x, e->y, e->w, e->h, 1, 0xff0000, e->vmm);

	char *b = (char *) get_buffer_editbox(e);
	int unicode = 0,i = 0,j = 0;

	unsigned int c = 0, len = get_len_editbox(e);

	
	for(i=0;i < (e->h-2)/e->font.rows;i++) {
_p1:
	for(j=0;j < (e->w-2)/e->font.cols;j++) {
		if(!*b) break;

		if( c++ >= len) { 
			e->spin_lock = 0;
			return (0);
		}

		if(*b == '\n' || *b == '\t' || *b == '\r') { 
	
			if(*b == '\n') { 

				b++;
				i++;
				goto _p1;
			}

			if(*b == '\t') j += 7;

			b++;
			continue;
		}

		b = utf8_convert(b,&unicode);
		if(e->subtype == 1) unicode = '*'; // senha

		gx_putchar(&e->font,unicode, 1+ e->x + e->font.cols*j, 
		1 +e->y + e->font.rows*i, e->c[0] ,e->c[1], e->vmm);

	} if(!*b) {  break; } }


	if(e->m[3] ) {
		if(e->f) {
			gx_putchar(&e->font,'|', 1+ e->x + e->font.cols * j, 
			1 +e->y + e->font.rows * i, e->c[0] ,e->c[1], e->vmm);
		}
		e->m[3] = 0;
	}else e->m[3] += 1;
	

	e->spin_lock = 0;
	return (0);
}
