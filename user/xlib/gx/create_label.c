#include <gx.h>
#include <string.h>
#include <stdlib.h>

#define null 0

gx_hand_t *create_label(gx_hand_t *window, const char *str,int x, int y, int w, int h, unsigned int style)
{
	gx_hand_t *l = (gx_hand_t *)malloc(sizeof(gx_hand_t));
	
	memset(l, 0, sizeof(gx_hand_t));

	l->x0 = x;
	l->y0 = y;
	l->x = x + window->x;
	l->y = y + window->y;
	l->w = w;
	l->h = h;

	l->flag = 0;
	l->style = style;

	l->type = GX_TYPE_LABEL;

	l->vmm = (void *) window->vmm;

	memcpy(&l->font,&window->font,sizeof(struct font));
	
	l->next = null;
	l->tail = null;

	// label
	l->m[0] = (unsigned int) malloc(0x1000);
	memset((void*)l->m[0],0,0x1000);

	l->m[1] = (unsigned int) malloc(0x1000);
	memset((void*)l->m[1],0,0x1000);

	l->m[2] = 0;
	
	l->c[0] = -1; // color fg 
	l->c[1] = 0x80808000; // color bg

	
	int len = strlen(str);
	memcpy((void*)l->m[0],str,len);

	l->spin_lock = 0;
	l->pid = 0;

	// add list
	obj_add_list( window, l);

	
	return (gx_hand_t *) l;
}

void exit_label(gx_hand_t *l)
{
	if(!l) return;

	while(l->spin_lock);
	l->flag |= 1;

	free((void*) l->m[0]);
	free((void*) l->m[1]);
	free(l);

}

int update_str_label(gx_hand_t *l, const char *str)
{
	char *lstr;

	while(G->l.spin_lock[0]);
	
	if(l->m[2] == 0) lstr = (char *) l->m[1];
	else lstr = (char *) l->m[0];



	if(!lstr) return (-1);

	memset(lstr,0,0x1000);

	int len = strlen(str);
	memcpy(lstr,str,len);
	
	while(G->l.spin_lock[2]);
	G->l.spin_lock[2] = 1;

	while(G->l.spin_lock[1]);
	//swap
	if(l->m[2] == 0) l->m[2] = 1;
	else l->m[2] = 0;

	G->l.spin_lock[2] = 0;
	return 0;	
}

int update_label(gx_hand_t *window, gx_hand_t *l)
{
	if(l->flag&1) return (-1);

	l->x = l->x0 + window->x;
	l->y = l->y0 + window->y;	


	char *lstr = (char *) ((l->m[2]&1)? l->m[1] : l->m[0]); // buffer
	if(!lstr) return (-1);

	int unicode;
	int i;
	
	for(i= 0;i < l->w/l->font.cols;i++) {
		if(!*lstr) break;

		lstr = utf8_convert(lstr,&unicode);

		gx_putchar(&l->font,unicode, l->x + l->font.cols * i, l->y+3, l->c[0],l->c[1], l->vmm);
			
	}

	return (0);
}
