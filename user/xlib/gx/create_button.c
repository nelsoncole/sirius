#include <gx.h>
#include <string.h>
#include <stdlib.h>

#define null 0

gx_hand_t *create_button(gx_hand_t *window, const char *str,int x, int y, int w, int h, unsigned int style)
{
	gx_hand_t *b = (gx_hand_t *)malloc(sizeof(gx_hand_t));
	
	memset(b, 0, sizeof(gx_hand_t));

	b->x0 = x;
	b->y0 = y;
	b->x = x + window->x;
	b->y = y + window->y;
	b->w = w;
	b->h = h;

	b->flag = 0;
	b->style = style;

	b->type = GX_TYPE_BUTTON;
	b->subtype = style &0xff;

	b->vmm = (void *) window->vmm;

	memcpy(&b->font,&window->font,sizeof(struct font));
	
	b->next = null;
	b->tail = null;

	// label
	b->m[0] = (unsigned int) malloc(0x1000);
	memset((void*)b->m[0],0,0x1000);

	int len = strlen(str);
	int pos = (b->w/b->font.cols) - len;
	pos = pos/2;
	b->m[1] = pos;

	memcpy((void*)b->m[0],str,len);

	// color
	b->c[0] =  0; //fg
	b->c[1] = 0xc0c0c000; //bg

	// lista de event
	ps_add_list(window, b);

	b->spin_lock = 0;
	b->pid = 0;

	// add list
	obj_add_list( window, b);
	
	return (gx_hand_t *) b;
}

void exit_button(gx_hand_t *b)
{
	if(!b) return;

	while(b->spin_lock);
	b->flag |= 1;

	free((void*) b->m[0]);
	free(b);

}

int update_button(gx_hand_t *window, gx_hand_t *b)
{
	if(b->flag&1) return (-1);

	b->x = b->x0 + window->x;
	b->y = b->y0 + window->y;

	b->spin_lock = 1;

	gx_rect( b->x, b->y, b->w, b->h, b->c[1] /*bg*/, b->vmm);

	if(b->f)gx_border (b->x, b->y, b->w, b->h, 1, 0xff000000, b->vmm);
	else gx_border (b->x, b->y, b->w, b->h, 1, 0xE0FFFF00, b->vmm);

	char *lstr = (char *) (b->m[0]); // buffer
	if(!lstr) return (-1);

	int unicode;
	int i;
	
	for(i= 0;i < b->w/b->font.cols;i++) {
		if(!*lstr) break;

		lstr = utf8_convert(lstr,&unicode);
		gx_putchar(&b->font,unicode, b->x + b->font.cols * (i+ b->m[1]), b->y+3, b->c[0],b->c[1], b->vmm);
			
	}

	b->spin_lock = 0;
	return (0);
}
