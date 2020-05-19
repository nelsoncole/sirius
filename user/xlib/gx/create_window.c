#include <gx.h>
#include <string.h>
#include <stdlib.h>
#include <sys/sys.h>

#define null 0

gx_hand_t *create_window (int x, int y, int w, int h, unsigned int style) 
{
	gx_hand_t *g = (gx_hand_t *)malloc(sizeof(gx_hand_t));
	
	memset(g, 0, sizeof(gx_hand_t));

	g->x = x;
	g->y = y;
	g->w = w;
	g->h = h;

	g->flag = 0;
	g->style = style;

	g->type = GX_TYPE_WINDOW;

	g->vmm = (void *) G->BankBuffer;
	
	g->next = null;
	g->tail = null;

	// font
	gx_font_init (&g->font, "font.tf", 1);

	// ps
	g->ps = (struct ps *) calloc (sizeof(struct ps),512);
	memset(g->ps, 0, sizeof(struct ps)*512);

	g->spin_lock = 0;
	g->pid = getpid();
	
	// add list
	window_add_list(g);

	return (gx_hand_t *) g;
}

void exit_window(gx_hand_t *w) 
{	
	if(!w) return;

	gx_hand_t *tail = w->tail;
	while(w->spin_lock);
	w->flag |= 1;
	
	window_remove_list(w);
	// rmover todos os objectos

	while(tail) {
		switch (tail->type) {
			case GX_TYPE_TITLE:
				exit_title(tail);
			break;
			case GX_TYPE_BOX:
				exit_box(tail);
			break;
			case GX_TYPE_LABEL:
				exit_label(tail);
			break;
			case GX_TYPE_EDITBOX:
				exit_editbox(tail);
			break;
			case GX_TYPE_BUTTON:
				exit_button(tail);
			break;
		}
		tail = tail->tail;
	}

	gx_font_end(&w->font);
	free(w);
}

int update_window(gx_hand_t *w)
{	if(w->flag&1) return (-1);
	
	int x = w->x;
	int y = w->y;

	gx_rect( x, y, w->w, w->h, 0x80808000 /*bg*/, w->vmm);
	gx_border (x, y, w->w, w->h, 1, 0xE0FFFF00, w->vmm);

	return (0);
}
