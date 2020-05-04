#include <gx.h>

gx_hand_t *mx = 0;
int magic = 0;
MOUSE *_mouse_r = 0;

void gx_mouse_init()
{
	_mouse_r = (MOUSE*)(*(unsigned int*)(0x10001120));

	magic = 1234;
	mx = 0;
}

void restore () {
	if(!mx) return;
	mx->m[3] = 0xc0c0c000;
	mx = 0;
}

gx_message_t gx_mouse (gx_hand_t *w) {

	gx_message_t r;

	int x = _mouse_r->x;
	int y = _mouse_r->y;
	
	r.id = 0;
	r.org = 1;
	r.type = 0;

	if(magic != 1234) return (r);
	if(G->l.pid[0] != w->pid) return (r);

	int i;
	gx_hand_t *h;
	if(!w->ps) return (r);

	for (i = 0;i < 512;i++) 
	{
		if (!w->ps[i].id) break;
		
		h = (gx_hand_t *) w->ps[i].id;
		if ((h->x + h->w) >= x && h->x <= x && (h->y + h->h) >= y && h->y <= y) 
		{
			switch (w->ps[i].type) {

				case GX_TYPE_TITLE:
					if((h->x + 8+8) >= x && (h->x + 8) <= x) {
						mx = (gx_hand_t *) w->ps[i].id;
						h->m[3] = 0xff000000;
						
						if(_mouse_r->b&0x1) {
							while(_mouse_r->b&0x1); // espera soltar
							r.type = GX_EXIT;
						}
						
					}else {

						// movimentar janela
						int x0 = x - w->x;
						int y0 = y - w->y;

						while(_mouse_r->b&0x1) 	{

							w->x = _mouse_r->x - x0;
							w->y = _mouse_r->y - y0;
						}					
						restore ();
						return (r);

					}
				break;
				default:
					// restore
					h->m[1] = 0xc0c0c000;
				break;
			}



			switch (r.type) {
				case GX_EXIT:
					exit_window(w);
					exit(0);
				break;

			}
		
			r.id = w->ps[i].id;
			return (r);
		}

		// default
		restore ();
		
		
	}

	return (r);
} 
