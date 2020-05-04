#include <gx.h>
#include <string.h>
#include <stdlib.h>

#define null 0

gx_hand_t *create_box(gx_hand_t *window,int x, int y, int w, int h,unsigned int style)
{
	gx_hand_t *b = (gx_hand_t *)malloc(sizeof(gx_hand_t));
	
	memset(b, 0, sizeof(gx_hand_t));
	b->spin_lock = 0;

	b->x0 = x;
	b->y0 = y;
	b->x = x + window->x;
	b->y = y + window->y;
	b->w = w;
	b->h = h;

	b->flag = 0;
	b->style = style;

	b->type = GX_TYPE_BOX;

	b->vmm = (void *) window->vmm;

	memcpy(&b->font,&window->font,sizeof(struct font));
	
	b->next = null;
	b->tail = null;


	// initialize buffer (64 KiB) 
	// 1024*60 = char

	int pool = (int) malloc(0x10000);
	char **txt = (char **) pool;
	memset(txt,0,0x10000);

	for(int i=0;i< 60;i++) {
		txt[i] = (char *) pool + 0x1000 + (i*1024);
	}

	b->m[0] = (unsigned int)txt;

	b->spin_lock = 0;
	b->pid = 0;
	
	// add list
	obj_add_list( window, b);

	
	return (gx_hand_t *) b;
}

void *get_buffer_box(gx_hand_t *box)
{
	return (void*) box->m[0];
}

void clearscreen_box(gx_hand_t *box)
{
	while(box->spin_lock);

	box->flag |= 1;

	memset((void*) box->m[0] + 0x1000,0,0x10000 - 0x1000);

	box->flag &=~1;

}

void exit_box(gx_hand_t *box) {
	
	if(!box) return;

	while(box->spin_lock);
	box->flag |= 1;

	free((void*) box->m[0]);
	free(box);
}

int update_box(gx_hand_t *window, gx_hand_t *box)
{
	if(box->flag&1) return (-1);

	box->x = box->x0 + window->x;
	box->y = box->y0 + window->y;


	unsigned char **lines = (unsigned char **) box->m[0]; // buffer
	if(!lines) return (-1);

	box->spin_lock = 1;

	int unicode;
	char *lstr;
	int i,j;
	for(i=0;i < box->h/box->font.rows;i++) {

		lstr = (char *)lines[i];
		if(!lstr) {
			box->spin_lock = 0;
		 	return (-1);
		}
		for(j=0;j < box->w/box->font.cols;j++) {
			if(!*lstr) break;
			lstr = utf8_convert(lstr,&unicode);
			gx_putchar(&box->font,unicode, box->x + box->font.cols*j, 
			box->y + box->font.rows*i, -1 ,0x80808000, box->vmm);
			
		}
	}

	box->spin_lock = 0;

	return (0);
}
