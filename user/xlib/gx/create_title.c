#include <gx.h>
#include <string.h>
#include <stdlib.h>

#define null 0

gx_hand_t *create_title(gx_hand_t *window, const char *title, int x, unsigned int style)
{
	gx_hand_t *t = (gx_hand_t *)malloc(sizeof(gx_hand_t));
	
	memset(t, 0, sizeof(gx_hand_t));

	t->x0 = 1;
	t->y0 = 1;
	t->x = window->x + 1;
	t->y = window->y + 1;
	t->w = window->w - 1;
	t->h = 24;

	t->flag = 0;
	t->style = style;

	t->type = GX_TYPE_TITLE;

	t->vmm = (void *) window->vmm;

	memcpy(&t->font,&window->font,sizeof(struct font));
	
	t->next = null;
	t->tail = null;

	//color
	t->c[0] = 0x80808000;
	t->c[1] = 0xc0c0c000;
	t->c[2] = 0xc0c0c000;

	// title

	int len = strlen(title);
	int pos = (t->w/t->font.cols) - len;
	pos = pos/2;
	t->m[1] = pos;
	
	t->m[0] = (unsigned int) malloc(len+1);
	memset((void*)t->m[0],0,len+1);


	memcpy((void*)t->m[0],title,len);

	// ps
	/*window->ps[0].id = (unsigned int) t;
	window->ps[0].type = GX_TYPE_TITLE;*/
	ps_add_list(window, t);

	t->spin_lock = 0;
	t->pid = 0;

	// add list
	obj_add_list( window, t);

	
	return (gx_hand_t *) t;
}

void exit_title(gx_hand_t *t)
{
	if(!t) return;

	while(t->spin_lock);
	t->flag |= 1;

	free((void*) t->m[0]);
	free(t);

}

int update_title(gx_hand_t *window, gx_hand_t *t)
{
	if(t->flag&1) return (-1);

	t->x = t->x0 + window->x;
	t->y = t->y0 + window->y;

	gx_rect( t->x, t->y, t->w, t->h, t->c[1] /*bg*/, t->vmm);
	gx_putchar(&t->font,'X',t->x + 8, t->y + 3, t->c[0], t->c[2]/*bg*/, t->vmm);
	//gx_line_horizontal (t->x, t->y + 24, t->w, -1, t->vmm);	



	char *lstr = (char *) t->m[0]; // buffer
	if(!lstr) return (-1);

	int unicode;
	int i;
	
	for(i= 0;i < t->w/t->font.cols;i++) {
		if(!*lstr) break;

		lstr = utf8_convert(lstr,&unicode);

		gx_putchar(&t->font,unicode, t->x + t->font.cols * (i+ t->m[1]), t->y+3, 0,t->c[1], t->vmm);
			
	}

	return (0);

}
